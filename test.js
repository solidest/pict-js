
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
    console.log(ress.length);
    for(let res of ress) {
        console.log(res);
    }

    console.log(task, model);
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");