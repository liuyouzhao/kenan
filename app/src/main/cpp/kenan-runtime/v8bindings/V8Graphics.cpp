//
// Created by hujia on 7/14/20.
//
#include "ObjectWrap.h"
#include "V8Graphics.h"
#include "Image.h"
#include "EntityManager.h"

using namespace v8;
using namespace kenan_graphics;
using namespace kenan_system;

#ifdef MODULE_POOL_NAME
#undef MODULE_POOL_NAME
#endif
#define MODULE_POOL_NAME "graphics-image"

namespace kenan_v8bindings
{

void V8Graphics::Graphics_Image_Create(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_Create : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = new Image();
    EntityManager::instance()->store<Image>(name, MODULE_POOL_NAME, image);
    args.GetReturnValue().Set(args[0]->ToString());
}

void V8Graphics::Graphics_Image_Destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_Destroy : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);
    EntityManager::instance()->eliminate(name, MODULE_POOL_NAME);
}

void V8Graphics::Graphics_Image_SetSrc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : number of args exception! argn must:2");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    char src[PATH_MAX_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    Local<String> path = args[1]->ToString();
    mark->WriteUtf8(name);
    path->WriteUtf8(src);

    Image *image = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    /// TODO: load image in another thread
}

void V8Graphics::Graphics_Image_GetSrc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    std::string path = image->getSrc();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), path.c_str()));
}


void V8Graphics::Graphics_Image_GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getWidth()));
}

void V8Graphics::Graphics_Image_GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getHeight()));
}

void V8Graphics::Graphics_Image_SetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 2)
    CHECK_V8_ARGS2(args, String, Array)

    HandleScope scope(args.GetIsolate());
    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *img = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);

    int w = img->getWidth();
    int h = img->getHeight();


    int *pixels = new int[w * h];
    v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(args[1]);
    for (unsigned int i = 0; i < w * h; i++) {
        Local<Value> pix = array->Get(v8::Int32::New(args.GetIsolate(), i));
        double dp = pix->NumberValue();
        pixels[i] = (int)dp;
    }
    img->setPixels(pixels, w, h);
    delete[] pixels;
}

void V8Graphics::Graphics_Image_GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)
    HandleScope scope(args.GetIsolate());

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *img = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);

	unsigned int* intArr = img->getImageData();

	int w = img->getWidth();
	int h = img->getHeight();

	v8::Local<v8::Array> pixels = v8::Array::New(args.GetIsolate(), w * h);
	for (unsigned int i = 0; i < w * h; i++) {
	    pixels->Set(v8::Int32::New(args.GetIsolate(), i), v8::Uint32::New(args.GetIsolate(), intArr[i]));
	}
	args.GetReturnValue().Set(pixels);
}

}