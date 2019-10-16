
#include "model.h"

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
//   randomOrder   Order of combinations to be generated with this parameter, default:false
//                      false:    PICT_PAIRWISE_GENERATION  
//                      true:     PICT_DEFAULT_RANDOM_SEED  
//   valueWeights   Array of weights, one per value, must be of ValueCount size
// 
// Returns:
//   true           Allocation succeeded
//   false          Allocation failed
//
// ////////////////////////////////////////////////////////////////////////////
Napi::Value Model::AddParameter(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if(info.Length()<1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    } else if (!info[0].IsNumber()) {
            Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
            return env.Null();
    }

    size_t valueCount = info[0].As<Napi::Number>().Int64Value();
    bool randomOrder = info.Length()>1 && info[1].IsBoolean() && info[1].As<Napi::Boolean>().Value();
    
    PICT_HANDLE res = nullptr;

    if(info.Length()>2 && info[2].IsArray()) {
        auto valueWeights = new unsigned int[valueCount];
        auto vws = info[2].As<Napi::Array>();
        for(unsigned int idx=0; idx<valueCount; idx++) {
            valueWeights[idx] = static_cast<Napi::Value>(vws[idx]).As<Napi::Number>().Int32Value();
        }
        res = PictAddParameter(this->_model, valueCount, randomOrder ? PICT_DEFAULT_RANDOM_SEED : PICT_PAIRWISE_GENERATION, valueWeights);
        delete [] valueWeights;
    } else {
        res = PictAddParameter(this->_model, valueCount, randomOrder ? PICT_DEFAULT_RANDOM_SEED : PICT_PAIRWISE_GENERATION);
    }

    return Napi::Boolean::New(env, res!=nullptr);
}

PICT_HANDLE Model::Get() {
    return this->_model;
}


Napi::Object Model::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "PictModel", {
        InstanceMethod("AddParameter", &Model::AddParameter)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PictModel", func);
    return exports;
}



