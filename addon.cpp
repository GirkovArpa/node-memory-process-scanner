#include <napi.h>
#include <windows.h>
#include <chrono>
#include <iostream>
#include <thread>

using namespace Napi;

Value scan(const CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::string str = info[0].ToString().Utf8Value();
    return String::New(env, "foo bar baz");
}

Napi::Object Init(Napi::Env env, Object exports) {
    exports.Set("scan", Function::New(env, scan));
    return exports;
}

NODE_API_MODULE(foo, Init)