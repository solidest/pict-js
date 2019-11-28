const {
    PictTask,
    PictModel
} = require("../build/Release/pict_js-native");
const BinParser = require('./parser/BinParser');


//生成笛卡尔集合
function product(elements) {
    if (!Array.isArray(elements)) {
        throw new TypeError();
    }
    var end = elements.length - 1,
        result = [];

    function addTo(curr, start) {
        var first = elements[start],
            last = (start === end);

        for (var i = 0; i < first.length; ++i) {
            var copy = curr.slice();
            copy.push(first[i]);

            if (last) {
                result.push(copy);
            } else {
                addTo(copy, start + 1);
            }
        }
    }

    if (elements.length) {
        addTo([], 0);
    } else {
        result.push([]);
    }
    return result;
}


//解析参数列表
function parseParaList(ast) {
    let ps = ast.get_para_list();
    let p = ps.next();
    let res = [];
    while (p.value) {
        if (!res.includes(p)) {
            res.push(p.value);
        }
        p = ps.next();
    }
    return res;
}


class Pict {

    constructor(paras, wise = 2) {

        this.wise = wise;        
        let lparas = [];
        let nparas = {};
        let pcount = 0;
        
        //整理一份参数引用数组
        for (let p in paras) {
            let len = paras[p].values.length;
            let para = {
                name: p,
                len: len,
                values: paras[p].values,
                weights: paras[p].weights,
            };
            lparas.push(para);
            nparas[p] = para;
            pcount++;
        }

        this.pcount = pcount;
        this.lparas = lparas;
        this.nparas = nparas;
    }

    _Generate(task, model, condition) {

        for(let p of this.lparas) {
            p.ref = model.AddParameter(p.len, this.wise, p.weights);
        }

        //约束求解
        if (condition) {
            let ast = BinParser.parse(condition);
            let uparas = parseParaList(ast);
            let len = uparas.length;
            let pvs = [];
            for(let pname of uparas) {
                pvs.push(this.nparas[pname].values);
            }
            if(len>0) {
                let ctxs = product(pvs);
                for(let ctxv of ctxs) {
                    let ctx = {};
                    for(let i=0; i<len; i++) {
                        ctx[uparas[i]] = ctxv[i];
                    }
                    
                    if(!ast.value_bool(ctx)) {
                        let excs = [];
                        for(let vn in ctx) {
                            let rp = this.nparas[vn];
                            excs.push(rp.ref);
                            excs.push(rp.values.indexOf(ctx[vn]));
                        }
                        task.AddExclusion(excs);
                    }
                }
            }
        }

        //生成结果
        let rows = task.Generate();
        let res = [];
        for (let r of rows) {
            let pw = {};
            for (let i = 0; i < this.pcount; i++) {
                let para = this.lparas[i];
                let value = para.values[r[i]];
                pw[para.name] = value;
            }
            res.push(pw);
        }
        return res;

    }

    Generate(condition = null) {

        let task = new PictTask();
        let model = new PictModel();
        task.SetRootModel(model);

        let res = null;
        try {
            res = this._Generate(task, model, condition);
        } catch (error) {
            console.log(error)
        }
        task.Close();
        model.Close();
        return res;
    }
}

module.exports = Pict;