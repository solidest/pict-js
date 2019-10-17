#include <napi.h>

#include "task.h"
#include "model.h"
#include "parameter.h"

using namespace Napi;

    // _PictCreateTask
    // PictAddExclusion
    // PictAddSeed
    // PictGenerate
    // PictAttachChildModel
    // PictResetResultFetching
    // PictGetNextResultRow
    // _PictSetRootModel
    // _PictGetTotalParameterCount
    // _PictDeleteTask
    // _PictCreateModel
    // _PictAddParameter
    // _PictDeleteModel

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Task::Init(env, exports);
    Model::Init(env, exports);
    Parameter::Init(env, exports);
    return exports;
}

NODE_API_MODULE(addon, Init)