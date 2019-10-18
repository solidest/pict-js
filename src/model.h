/*
** model.h for pict-js in /home/pi/pict-js/src
**
** Made by solidest
** Login   <>
**
** Started on  Wed Oct 16 3:56:16 PM 2019 solidest
** Last update Sat Oct 18 9:26:58 AM 2019 solidest
*/

#ifndef _MODEL_H_
# define _MODEL_H_

#include <napi.h>
#include "../ms_pict/api/pictapi.h"

class Model : public Napi::ObjectWrap<Model> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Model(const Napi::CallbackInfo &info);
    virtual void Finalize(Napi::Env env);
    PICT_HANDLE Get();
    Napi::Value AddParameter(const Napi::CallbackInfo &info);
    Napi::Value AttachChildModel(const Napi::CallbackInfo &info);

  private:
    static Napi::FunctionReference constructor;
    PICT_HANDLE _model;
};


#endif /* !_MODEL_H_ */
