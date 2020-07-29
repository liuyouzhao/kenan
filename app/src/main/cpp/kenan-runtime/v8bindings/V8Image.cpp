//
// Created by hujia on 7/29/20.
//
#include "defines.h"
#include "ObjectWrap.h"
#include "V8EventService.h"
#include "Image.h"
#include "EntityManager.h"
#include "ReturnStatus.h"
#include "V8Image.h"

using namespace kenan_graphics;
using namespace kenan_system;

#ifdef MODULE_POOL_NAME
#undef MODULE_POOL_NAME
#endif
#define MODULE_POOL_NAME "graphics-image"

#undef LOG_TAG
#define  LOG_TAG    "V8Image"

namespace kenan_v8bindings {

void V8Image::Create(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)

    HandleScope handleScope(args.GetIsolate());

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = new Image();
    EntityManager::instance()->store<Image>(name, MODULE_POOL_NAME, image);
    args.GetReturnValue().Set(args[0]->ToString());
}

void V8Image::Destroy(const v8::FunctionCallbackInfo<v8::Value>& args) {
    V8Base::Destroy(args, MODULE_POOL_NAME);
}


/**
* @params:
    Compulsory: image_id(string)
    Compulsory: file_path/base64(string)
    Optional: synchronized(boolean) default async
*/
void V8Image::SetSrc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 2)
    CHECK_V8_ARGS2(args, String, String)

    HandleScope handleScope(args.GetIsolate());

    std::string name = getMarkFromArgument(args);
    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    char src[PATH_MAX_LENGTH] = {0};
    Local<String> path = args[1]->ToString();
    path->WriteUtf8(src);

    ReturnStatus rs = image->load(src);
    if(!rs.status) {
        args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), true));
    }
    else {
        args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));
    }
}

void V8Image::GetSrc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)

    HandleScope handleScope(args.GetIsolate());

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    std::string path = image->getSrc();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), path.c_str()));
}


void V8Image::GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)

    HandleScope handleScope(args.GetIsolate());

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getWidth()));
}

void V8Image::GetHeight(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)

    HandleScope handleScope(args.GetIsolate());

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getHeight()));
}

void V8Image::SetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 2)
    CHECK_V8_ARGS2(args, String, Array)

    HandleScope scope(args.GetIsolate());

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

    int w = image->getWidth();
    int h = image->getHeight();

    int *pixels = new int[w * h];
    v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(args[1]);
    for (unsigned int i = 0; i < w * h; i++) {
        Local<Value> pix = array->Get(v8::Int32::New(args.GetIsolate(), i));
        double dp = pix->NumberValue();
        pixels[i] = (int)dp;
    }
    image->setPixels(pixels, w, h);
    delete[] pixels;
}

void V8Image::GetImageData(const v8::FunctionCallbackInfo<v8::Value>& args) {
    CHECK_V8_ARGNUM(args, 1)
    CHECK_V8_ARGS1(args, String)
    HandleScope scope(args.GetIsolate());

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }

	unsigned int* intArr = image->getImageData();

	int w = image->getWidth();
	int h = image->getHeight();

	v8::Local<v8::Array> pixels = v8::Array::New(args.GetIsolate(), w * h);
	for (unsigned int i = 0; i < w * h; i++) {
	    pixels->Set(v8::Int32::New(args.GetIsolate(), i), v8::Uint32::New(args.GetIsolate(), intArr[i]));
	}
	args.GetReturnValue().Set(pixels);
}

void V8Image::GetTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(!checkArgumentLength(args, 1))
        return;

    Image *image = getPointerByArgs<Image>(args, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : No such image ID");
        return;
    }
    LOGD("image->getTexture() %d", image->getTexture());
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getTexture()));
}


void Hook_Image_OnLoad(void *image) {
    V8EventService::instance()->hookOnFinishResourceLoading(image);
}

}