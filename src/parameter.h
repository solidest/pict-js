/*
** parameter.h for pict-js in /home/pi/pict-js/src
**
** Made by solidest
** Login   <>
**
** Started on  Thu Oct 17 5:49:48 PM 2019 solidest
** Last update Fri Oct 17 6:08:44 PM 2019 solidest
*/

#ifndef _PARAMETER_H_
# define _PARAMETER_H_

#include <napi.h>
#include "../ms_pict/api/pictapi.h"

class Parameter : public Napi::ObjectWrap<Parameter> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, PICT_HANDLE parameter, size_t valuesCount);
    Parameter(const Napi::CallbackInfo &info);
    virtual void Finalize(Napi::Env env);
    Napi::Value GetCount(const Napi::CallbackInfo &info);
    PICT_H Get() {return this->_parameter;}

  private:
    static Napi::FunctionReference constructor;
    PICT_HANDLE _parameter;
    size_t _valuesCount;
};



#endif /* !_PARAMETER_H_ */
