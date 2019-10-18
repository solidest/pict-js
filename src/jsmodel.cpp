
#include "jsmodel.h"
#include "jsparameter.h"

using namespace Napi;


Napi::FunctionReference Model::constructor;


Model::Model(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Model>(info) {
    this->_model = PictCreateModel();
}

void Model::Finalize(Napi::Env env){
    PictDeleteModel(this->_model);
    this->_model = NULL;
}

// ////////////////////////////////////////////////////////////////////////////
//
// Adds a parameter to a model
//
// Parameters:
//   valueCount     Numer of values this parameter has
//   order          Order of combinations to be generated with this parameter
//   valueWeights   Array of weights, one per value, must be of ValueCount size
// 
// Returns:
//   Parameter     Allocation succeeded
//   null          Allocation failed
//
// ////////////////////////////////////////////////////////////////////////////
Napi::Value Model::AddParameter(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if(info.Length()<1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    } else if (!info[0].IsNumber()) {
            Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Null();
    }

    size_t valueCount = info[0].As<Napi::Number>().Int64Value();
    auto order = info.Length()>1 && info[1].IsNumber() ? info[1].As<Napi::Number>().Int32Value() : PICT_PAIRWISE_GENERATION;
    
    PICT_HANDLE res = nullptr;

    if(info.Length()>2 && info[2].IsArray()) {
        auto valueWeights = new unsigned int[valueCount];
        auto vws = info[2].As<Napi::Array>();
        for(unsigned int idx=0; idx<valueCount; idx++) {
            valueWeights[idx] = static_cast<Napi::Value>(vws[idx]).As<Napi::Number>().Int32Value();
        }
        res = PictAddParameter(this->_model, valueCount, order, valueWeights);
        delete [] valueWeights;
    } else {
        res = PictAddParameter(this->_model, valueCount, order);
    }

    if(res==nullptr) {
        return env.Null();
    }
    return Parameter::NewInstance(env, res, valueCount);
}

PICT_HANDLE Model::Get() {
    return this->_model;
}

// ////////////////////////////////////////////////////////////////////////////
//
// Wires a model to another model to create a model-submodel hierarchy
//
// Parameters:
//   modelChild    
//   order       Order of combinations to be generated with this submodel
//  
// Returns:
//   true
//   null
//
// ////////////////////////////////////////////////////////////////////////////
Napi::Value Model::AttachChildModel(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if(info.Length()<1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }
    auto order = info.Length()>1 && info[1].IsNumber() ? info[1].As<Napi::Number>().Int32Value() : PICT_PAIRWISE_GENERATION;

    auto o = info[0].As<Napi::Object>();
    auto p = Napi::ObjectWrap<Model>::Unwrap(o);
    PictAttachChildModel(this->_model, p->_model, order);
    return Napi::Boolean::New(env, true);
}



Napi::Object Model::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "PictModel", {
        InstanceMethod("AddParameter", &Model::AddParameter),
        InstanceMethod("AttachChildModel", &Model::AttachChildModel),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PictModel", func);
    return exports;
}



