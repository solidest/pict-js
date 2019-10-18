/*
** task.h for pict-js in /home/pi/pict-js/src
**
** Made by solidest
** Login   <>
**
** Started on  Wed Oct 16 2:53:32 PM 2019 solidest
** Last update Thu Oct 16 3:32:51 PM 2019 solidest
*/
#ifndef _TASK_H_
#define _TASK_H_

#include <napi.h>
#include "../ms_pict/api/pictapi.h"

#include <iostream>


class Task : public Napi::ObjectWrap<Task> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Task(const Napi::CallbackInfo &info);
    virtual void Finalize(Napi::Env env);
    Napi::Value SetRootModel(const Napi::CallbackInfo &info);
    Napi::Value GetTotalParameterCount(const Napi::CallbackInfo &info);
    Napi::Value AddExclusion(const Napi::CallbackInfo &info);
    Napi::Value AddSeed(const Napi::CallbackInfo &info);
    Napi::Value Generate(const Napi::CallbackInfo &info);

  private:
    static Napi::FunctionReference constructor;
    PICT_HANDLE _task;
};

#endif 
