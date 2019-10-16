#include <napi.h>
#include "pict_test.h"

using namespace Napi;


Napi::String Method(const Napi::CallbackInfo& info) {
  test_pict();
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "PictJs"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(addon, Init)
