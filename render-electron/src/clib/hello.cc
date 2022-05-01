#include <napi.h>

#include "render.h"
#include "scene.h"
using namespace minirender;

static std::shared_ptr<Renderer> r;
static std::shared_ptr<Scene> s;

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::String::New(env, "world");
}

Napi::Boolean LoadFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    // return Napi::Number::New(env, reinterpret_cast<int64_t>(&*r));
    return Napi::Boolean::New(env, false);
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    // return Napi::Number::New(env, reinterpret_cast<int64_t>(&*r));
    return Napi::Boolean::New(env, false);
  }
  return Napi::Boolean::New(env, s->AddModel(info[0].As<Napi::String>()));
  // return Napi::Number::New(env, reinterpret_cast<int64_t>(&*r));
}

Napi::Value Render(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Napi::Uint8Array buffer = Napi::Uint8Array::New(env, Renderer::width * Renderer::height * 4);
  r->RenderToBuffer(buffer.Data());
  // r->RenderToFile("111.tga");

  return buffer;
}

Napi::Number vert(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::Number::New(env, s->vert());
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  r = std::make_shared<Renderer>();
  s = std::make_shared<Scene>();

  r->SetScene(s);

  exports.Set(Napi::String::New(env, "hello"), Napi::Function::New(env, Method));
  exports.Set(Napi::String::New(env, "loadFile"), Napi::Function::New(env, LoadFile));
  exports.Set(Napi::String::New(env, "render"), Napi::Function::New(env, Render));
  exports.Set(Napi::String::New(env, "vert"), Napi::Function::New(env, vert));
  return exports;
}

NODE_API_MODULE(hello, Init)
