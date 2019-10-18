
#include "jsparameter.h"
#include "jsmodel.h"

using namespace Napi;


Napi::FunctionReference Parameter::constructor;

Parameter::Parameter(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Parameter>(info) {
    this->_parameter = nullptr;
    this->_valuesCount = -1;
}

void Parameter::Finalize(Napi::Env env){
    this->_parameter = nullptr;
    this->_valuesCount = -1;
}

Napi::Value Parameter::GetCount(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->_valuesCount);
}


Napi::Object Parameter::NewInstance(Napi::Env env, PICT_HANDLE parameter, size_t valuesCount) {
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = constructor.New({});
    auto p = Napi::ObjectWrap<Parameter>::Unwrap(obj);
    p->_parameter = parameter;
    p->_valuesCount = valuesCount;
    return scope.Escape(napi_value(obj)).ToObject();
}


Napi::Object Parameter::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "PictParameter", {
        InstanceMethod("GetCount", &Parameter::GetCount)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PictParameter", func);
    return exports;
}



