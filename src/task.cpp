
#include "task.h"
#include "model.h"

using namespace Napi;


Napi::FunctionReference Task::constructor;

Task::Task(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Task>(info) {
    this->_task = PictCreateTask();
}

void Task::Finalize(Napi::Env env){
    PictDeleteTask(this->_task);
    this->_task = NULL;
}

// ////////////////////////////////////////////////////////////////////////////
//
// Associates a model tree with a task.
//
// Parameters:
//   model     Valid handle of the root of the model hierarchy
// 
// Returns:
//   true/null
//
// ////////////////////////////////////////////////////////////////////////////
Napi::Value Task::SetRootModel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if(info.Length()<1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    } else if(!info[0].IsObject()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    auto o = info[0].As<Napi::Object>();
    auto p = Napi::ObjectWrap<Model>::Unwrap(o);
    if(nullptr!=p) {
        PictSetRootModel(this->_task,  p->Get());
        return Napi::Boolean::New(env, true);
    }

    return env.Null();
}


Napi::Value Task::GetTotalParameterCount(const Napi::CallbackInfo &info){
    size_t totalCount = PictGetTotalParameterCount(this->_task);
    
    return Napi::Number::New(info.Env(), totalCount);
}



Napi::Object Task::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "PictTask", {
        InstanceMethod("SetRootModel", &Task::SetRootModel),
        InstanceMethod("GetTotalParameterCount", &Task::GetTotalParameterCount)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PictTask", func);
    return exports;
}



