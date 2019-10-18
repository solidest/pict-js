const NP = require('number-precision');
NP.enableBoundaryChecking(false);

//表达式抽象类
class Exp {
    value_str() {
        throw ('error value_str');
    }

    value_bool() {
        throw ('error value_bool');
    }

    can_calc() {
        throw ('error is_calc');
    }

    get_para() {
        return null;
    }

    *get_para_list() {
        let p = this.get_para();
        if(p) {
            yield p;
        }
        if(this._leftExp) {
            let pl = this._leftExp.get_para_list();
            p = pl.next();
            while(p.value) {
                yield p.value;
                p = pl.next();
            }
        }
        if(this._rightExp) {
            let pl = this._rightExp.get_para_list();
            p = pl.next();
            while(p.value) {
                yield p.value;
                p = pl.next();
            }
        }
        return null;
    }
}


//判断式基础类
class BinExp extends Exp {
    can_calc() {
        return false;
    }
}

//not
class ExpNot extends BinExp {
    constructor(rightExp) {
        if (!rightExp) {
            throw ('error ExpNot');
        }
        super();
        this._rightExp = rightExp;
    }

    value_str() {
        return '~' + this._rightExp.value_str();
    }

    value_bool(ctx) {
        let right = this._rightExp.value_bool(ctx);
        return !right;
    }
}

//and
class ExpAnd extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpAnd');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + ' and ' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_bool(ctx);
        let right = this._rightExp.value_bool(ctx);
        return (left && right);
    }
}

//or
class ExpOr extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpAnd');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + ' or ' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_bool(ctx);
        let right = this._rightExp.value_bool(ctx);
        return (left || right);
    }
}

//相等
class ExpBeEqual extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error BeequalExp');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '==' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = null;
        if (this._leftExp.can_calc()) {
            left = this._leftExp.value_num(ctx);
        } else {
            left = this._leftExp.value_bool(ctx);
        }
        let right = null;
        if (this._rightExp.can_calc()) {
            right = this._rightExp.value_num(ctx);
        } else {
            right = this._rightExp.value_bool(ctx);
        }
        return (left === right);
    }
}

//不相等
class ExpNotEqual extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpNotequal');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '~=' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = null;
        if (this._leftExp.can_calc()) {
            left = this._leftExp.value_num(ctx);
        } else {
            left = this._leftExp.value_bool(ctx);
        }
        let right = null;
        if (this._rightExp.can_calc()) {
            right = this._rightExp.value_num(ctx);
        } else {
            right = this._rightExp.value_bool(ctx);
        }
        return (left !== right);
    }
}

//大于
class ExpGreater extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpGreater');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '>' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_num(ctx);
        let right = this._rightExp.value_num(ctx);
        return (left > right);
    }
}

//大于等于
class ExpGreaterEqual extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpGreaterEqual');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '>=' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_num(ctx);
        let right = this._rightExp.value_num(ctx);
        return (left >= right);
    }
}

//小于
class ExpLess extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpLess');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '<' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_num(ctx);
        let right = this._rightExp.value_num(ctx);
        return (left < right);
    }
}

//小于等于
class ExpLessEqual extends BinExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpLessEqual');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '<=' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let left = this._leftExp.value_num(ctx);
        let right = this._rightExp.value_num(ctx);
        return (left <= right);
    }
}

//计算式基础类
class CalcExp extends Exp {
    value_num() {
        throw ('error value_num');
    }
}

//变量
class ExpVar extends CalcExp {
    constructor(var_id) {
        super();
        if (!var_id) {
            throw ("error ExpNumber")
        }
        this._var = var_id;
    }
    value_num(ctx) {
        return ctx[this._var];
    }

    value_str() {
        return this._var;
    }

    value_bool(ctx) {
        if (!ctx[this._var]) {
            return false;
        } else {
            return true;
        }
    }

    can_calc() {
        return true;
    }

    get_para() {
        return this._var;
    }
}

//数字
class ExpNumber extends CalcExp {
    constructor(num) {
        super();
        if (isNaN(num)) {
            throw ("error ExpNumber")
        }
        this._v = Number(num);
    }

    value_num() {
        return this._v;
    }

    value_str() {
        return (NP.strip(this._v)).toString();
    }

    value_bool() {
        return NP.strip(this._v) !== 0;
    }

    can_calc() {
        return true;
    }
}

//加法
class ExpPlus extends CalcExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpPlus');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_num(ctx) {
        return NP.plus(this._leftExp.value_num(ctx), this._rightExp.value_num(ctx));
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '+' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let res = this.value_num(ctx);
        return (res != 0);
    }

    can_calc() {
        return this._leftExp.can_calc() && this._rightExp.can_calc();
    }
}

//减法
class ExpMinus extends CalcExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpMinus');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_num(ctx) {
        return NP.minus(this._leftExp.value_num(ctx), this._rightExp.value_num(ctx));
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '-' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let res = this.value_num(ctx);
        return (res != 0);
    }

    can_calc() {
        return this._leftExp.can_calc() && this._rightExp.can_calc();
    }
}

//乘法
class ExpTimes extends CalcExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpTimes');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_num(ctx) {
        return NP.times(this._leftExp.value_num(ctx), this._rightExp.value_num(ctx));
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '*' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let res = this.value_num(ctx);
        return (res != 0);
    }

    can_calc() {
        return this._leftExp.can_calc() && this._rightExp.can_calc();
    }
}

//除法
class ExpDivide extends CalcExp {
    constructor(leftExp, rightExp) {
        if (!leftExp || !rightExp) {
            throw ('error ExpDivide');
        }
        super();
        this._leftExp = leftExp;
        this._rightExp = rightExp;
    }

    value_num(ctx) {
        return NP.divide(this._leftExp.value_num(ctx), this._rightExp.value_num(ctx));
    }

    value_str() {
        return '(' + this._leftExp.value_str() + '/' + this._rightExp.value_str() + ')';
    }

    value_bool(ctx) {
        let res = this.value_num(ctx);
        return (res != 0);
    }

    can_calc() {
        return this._leftExp.can_calc() && this._rightExp.can_calc();
    }
}


module.exports = {
    ExpVar,
    ExpNumber,
    ExpPlus,
    ExpMinus,
    ExpTimes,
    ExpDivide,
    ExpBeEqual,
    ExpNotEqual,
    ExpGreater,
    ExpGreaterEqual,
    ExpLess,
    ExpLessEqual,
    ExpNot,
    ExpAnd,
    ExpOr
}