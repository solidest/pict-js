
#include "jstask.h"
#include "jsmodel.h"
#include "jsparameter.h"

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
        return env.Null();
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

// ////////////////////////////////////////////////////////////////////////////
//
// Adds an exclusion to the task.  Exclusions define combinations  which should
// not appear in the output. Such a combination has a variable number of items.
// Each item is a pair consisting of a pointer to the involved parameter and a
// zero-based index to its value. See the definition of PICT_EXCLUSION_ITEM.
//
// Parameters:
//   task                Valid handle of a task
//   exclusionItems      Array of PICT_EXCLUSION_ITEM's
//   exclusionItemCount  How many elements are passed in the array
// 
// Returns:
//   PICT_SUCCESS
//   PICT_OUT_OF_MEMORY
//
// ////////////////////////////////////////////////////////////////////////////

Napi::Value Task::AddExclusion(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    if(info.Length()!=1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    auto arrs = info[0].As<Napi::Array>();
    int len = arrs.Length()/2;
    auto excs = new PICT_EXCLUSION_ITEM[len];
    for(int i=0; i<len; i++) {
        auto p = Napi::ObjectWrap<Parameter>::Unwrap(static_cast<Napi::Value>(arrs[(uint32_t)i*2]).As<Napi::Object>())->Get();
        if(!p) {
            delete [] excs;
            Napi::TypeError::New(env, "Wrong argument1").ThrowAsJavaScriptException();
            return env.Null();
        }
        excs[i].Parameter = p;
        excs[i].ValueIndex =  static_cast<Napi::Value>(arrs[(uint32_t)(i*2+1)]).As<Napi::Number>().Int32Value();
    }
    // Napi::TypeError::New(env, "Wrong argument2").ThrowAsJavaScriptException();
    PictAddExclusion(this->_task, excs, len);
    delete [] excs;
    return Napi::Boolean::New(env, true);
}

// ////////////////////////////////////////////////////////////////////////////
//
// Adds a seed to the task.Seeds define combinations that have to appear in the
// output (if they do not violate any exclusions).  A seed is a variable-length 
// list of parameter-value pairs. Each pair consists of a pointer  to the param
// and a zero-based index to its value. See the definition of PICT_SEED_ITEM.
//
// Parameters:
//   task           Valid handle of a task
//   seedItems      Array of PICT_SEED_ITEM's
//   seedItemCount  How many elements are passed in the array
// 
// Returns:
//   PICT_SUCCESS
//   PICT_OUT_OF_MEMORY
//
// ////////////////////////////////////////////////////////////////////////////

Napi::Value Task::AddSeed(const Napi::CallbackInfo &info)
{
    auto env = info.Env();
    if(info.Length()!=1 || !info[0].IsArray()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    auto arrs = info[0].As<Napi::Array>();
    int len = arrs.Length()/2;
    auto seeds = new PICT_SEED_ITEM[len];
    for(int i=0; i<len; i++) {
        auto p = Napi::ObjectWrap<Parameter>::Unwrap(static_cast<Napi::Value>(arrs[(uint32_t)i*2]).As<Napi::Object>())->Get();
        if(!p) {
            delete [] seeds;
            Napi::TypeError::New(env, "Wrong1").ThrowAsJavaScriptException();
            return env.Null();
        }
        seeds[i].Parameter = p;
        seeds[i].ValueIndex =  static_cast<Napi::Value>(arrs[(uint32_t)(i*2+1)]).As<Napi::Number>().Int32Value();
    }
    PictAddSeed(this->_task, seeds, len);
    delete [] seeds;
    return Napi::Boolean::New(env, true);
}

// ////////////////////////////////////////////////////////////////////////////
//
// Generates results.  Should only be called after all structures are fully set 
// up and models wired to the task. Otherwise expect access violations.
//
// Parameters:
//   task       Valid handle to a task
// 
// Returns:
//   PICT_SUCCESS           Success
//   PICT_OUT_OF_MEMORY     Not enough memory to complete generation
//   PICT_GENERATION_ERROR  An internal problem with the engine
//
// ////////////////////////////////////////////////////////////////////////////

Napi::Value Task::Generate(const Napi::CallbackInfo &info) {

    auto env = info.Env();
    PictGenerate(this->_task);
    // wcout << "1" << endl;
    PICT_RESULT_ROW row = PictAllocateResultBuffer(this->_task);
    size_t paramCount = PictGetTotalParameterCount(this->_task);
    // wcout << "2" << endl;
    PictResetResultFetching(this->_task);
    auto res = Napi::Array::New(env);
    size_t ridx = 0;
    while(PictGetNextResultRow(this->_task, row))
    {
        auto ress = Napi::Array::New(env);
        for(size_t index = 0; index < paramCount; ++index)
        {
            auto pidx = static_cast<unsigned int>(row[index]);
            ress.Set(index, Napi::Number::New(env, static_cast<unsigned int>(row[index])));
            // wcout << static_cast<unsigned int>( row[ index ] )<< L" ";
        }
        res.Set(ridx, ress);
        ++ridx;
    }
    PictFreeResultBuffer(row) ;
    return res;
}



Napi::Value Task::GetTotalParameterCount(const Napi::CallbackInfo &info){
    size_t totalCount = PictGetTotalParameterCount(this->_task);
    
    return Napi::Number::New(info.Env(), totalCount);
}



Napi::Object Task::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "PictTask", {
        InstanceMethod("SetRootModel", &Task::SetRootModel),
        InstanceMethod("GetTotalParameterCount", &Task::GetTotalParameterCount),
        InstanceMethod("AddExclusion", &Task::AddExclusion),
        InstanceMethod("AddSeed", &Task::AddSeed),
        InstanceMethod("Generate", &Task::Generate),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PictTask", func);
    return exports;
}



