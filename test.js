
const {PictTask, PictModel} = require("./lib/binding.js");
const assert = require("assert");

assert(PictTask, "The expected class is undefined");
assert(PictModel, "The expected class is undefined");

function testBasic()
{
    const task =  new PictTask();
    assert.ok(task, "constructor error");
    const model =  new PictModel();
    assert.ok(model, "constructor error");
    let p1 = model.AddParameter(30);
    assert.ok(p1 && p1.GetCount()===30);
    let p2 = model.AddParameter(30, false);
    assert.ok(p2 && p2.GetCount()===30);
    let p3 = model.AddParameter(3, false, [10, 10, 8]);
    assert.ok(p3 && p3.GetCount()===3);
    let t = task.SetRootModel(model);
    console.log(t);
    assert.ok(task.GetTotalParameterCount()===3, "set root model error");

    console.log(task, model);
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");