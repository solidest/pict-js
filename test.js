
const {PictTask, PictModel} = require("./lib/binding.js");
const assert = require("assert");
const Pict= require('./src/pict');

assert(PictTask, "The expected class is undefined");
assert(PictModel, "The expected class is undefined");

function testBasic()
{
    const task =  new PictTask();
    assert.ok(task, "constructor error");
    const model =  new PictModel();
    assert.ok(model, "constructor error");

    let p1 = model.AddParameter(5, 2);
    assert.ok(p1 && p1.GetCount()===5);
    let p2 = model.AddParameter(4, 3);
    assert.ok(p2 && p2.GetCount()===4);
    let p3 = model.AddParameter(3, 2, [1, 2, 1]);
    assert.ok(p3 && p3.GetCount()===3);
    assert(task.SetRootModel(model));
    assert.ok(task.GetTotalParameterCount()===3, "set root model error");

    let sub1 = new PictModel();
    let sub2 = new PictModel();
    assert.ok(model.AttachChildModel(sub1));
    assert.ok(model.AttachChildModel(sub2));

    sub1.AddParameter(3);
    sub1.AddParameter(3);
    sub2.AddParameter(2);
    sub2.AddParameter(2);

    assert.ok(task.AddExclusion([p1, 0, p2, 3, p3, 0]));
    assert.ok(task.AddExclusion([p1, 2]));
    assert.ok(task.AddSeed([p1, 1, p2, 1, p3, 1]));

    let ress = task.Generate();
    assert.ok(ress);
    console.log(`Results:(${ress.length})`);
    for(let res of ress) {
        console.log(res);
    }

    console.log(task, model, p3);

    let paras = {};
    paras["工作模式"] = {values:['制冷', '制热', '除湿'], weights:[1,1,1]};
    paras["传感器正常"] = {values:[true, false], weights:[1,2]};
    paras["设置温度"] = {values:[15, 16, 17, 31, 32, 33], weights:[1,1,1,1,1,1]};
    paras["室内温度"] = {values:[13, 17, 15, 19, 31], weights:[1,1,1,1,1]};

    const pict = new Pict(paras, 2);
    let result1 = pict.Generate("传感器正常 and 设置温度<=室内温度+2");
    console.log(result1.length, "===================")
    for (let row of result1) {
        console.log(row);
    }
    let result2 = pict.Generate("设置温度==室内温度+2 and ~传感器正常");
    //...
    console.log(result2.length, "===================")
    for(let row of result2) {
        console.log(row);
    }
    let result3 = pict.Generate();
    console.log("all results:", result3.length);
}

testBasic();
//assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");
