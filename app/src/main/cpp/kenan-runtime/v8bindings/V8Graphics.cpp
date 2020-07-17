//
// Created by hujia on 7/14/20.
//
#include "defines.h"
#include "ObjectWrap.h"
#include "V8Graphics.h"
#include "V8EventService.h"
#include "Image.h"
#include "EntityManager.h"
#include "ReturnStatus.h"
#include "RuntimeLoop.h"

using namespace v8;
using namespace kenan_graphics;
using namespace kenan_system;
using namespace kenan_runtime;

#ifdef MODULE_POOL_NAME
#undef MODULE_POOL_NAME
#endif
#define MODULE_POOL_NAME "graphics-image"

#undef LOG_TAG
#define  LOG_TAG    "V8Image"

namespace kenan_v8bindings
{

v8::Handle<v8::Value> V8Graphics::genSingleton(v8::Isolate *isolate)
{
    v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(isolate);
    v8::Local<v8::Template> proto_t = t->PrototypeTemplate();
    proto_t->Set(isolate, "image_create", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_Create));
    proto_t->Set(isolate, "image_destroy", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_Destroy));
    proto_t->Set(isolate, "image_set_src", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_SetSrc));
    proto_t->Set(isolate, "image_get_src", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_GetSrc));
    proto_t->Set(isolate, "image_get_width", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_GetWidth));
    proto_t->Set(isolate, "image_get_height", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_GetHeight));
    proto_t->Set(isolate, "image_set_image_data", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_SetImageData));
    proto_t->Set(isolate, "image_get_image_data", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_GetImageData));
    proto_t->Set(isolate, "image_get_texture", v8::FunctionTemplate::New(isolate, V8Graphics::Graphics_Image_GetTexture));

    v8::Local<v8::ObjectTemplate> instance_t = t->InstanceTemplate();
    v8::Local<v8::Object> instance = instance_t->NewInstance();

    return instance;
}

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


class AsyncImageLoadDTO {
public:
    AsyncImageLoadDTO(std::string s, std::string i, Image *p):
        source(s),
        id(i),
        ptr(p) {}

    std::string source;
    std::string id;
    Image *ptr;
};

static void *__asyncImageLoadFunc(void *data) {
    AsyncImageLoadDTO *dto = (AsyncImageLoadDTO*) data;
    Image *image = dto->ptr;
    ReturnStatus rs;
    rs = image->load(dto->source.c_str());
    if(!rs.status) {
        V8EventService::instance()->postEvent(V8Event(
                                              std::string(V8E_GRAPHICS_IMAGE_ONLOAD),
                                              std::string(V8E_SUCCESS),
                                              std::string(dto->id.c_str())));
    }
    else {
        V8EventService::instance()->postEvent(V8Event(
                                              std::string(V8E_GRAPHICS_IMAGE_ONLOAD),
                                              std::string(V8E_FAILED),
                                              std::string(dto->id.c_str()),
                                              rs.error));
    }
    LOGD("%s %d", __FUNCTION__, __LINE__);
    return NULL;
}

/**
* @params:
    Compulsory: image_id(string)
    Compulsory: file_path/base64(string)
    Optional: synchronized(boolean) default async
*/
void V8Graphics::Graphics_Image_SetSrc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 2)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_SetSrc : number of args exception! argn must:2 or 3");
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

    /// load in this thread
    if(args.Length() >= 3 && args[2]->ToBoolean()->Value()) {
        ReturnStatus rs = image->load(src);
        if(!rs.status) {
            V8EventService::instance()->postEvent(V8Event(
                                                  std::string(V8E_GRAPHICS_IMAGE_ONLOAD),
                                                  std::string(V8E_SUCCESS),
                                                  std::string(name)));
        }
        else {
            V8EventService::instance()->postEvent(V8Event(
                                                  std::string(V8E_GRAPHICS_IMAGE_ONLOAD),
                                                  std::string(V8E_FAILED),
                                                  std::string(name),
                                                  rs.error));
        }
    }
    else { /// Do load in thread
    /// TODO: finish this feature of async loading
#if 0
        AsyncImageLoadDTO  *ayncDto = new AsyncImageLoadDTO(std::string(src), std::string(name), image);
        RuntimeMessage message(std::string(""), ayncDto, __asyncImageLoadFunc, DataDestructType::DELETE);
        RuntimeLoop::instance()->sendMessage(message);
#endif
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

void V8Graphics::Graphics_Image_GetTexture(const v8::FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handleScope(args.GetIsolate());
    if(args.Length() < 1)
    {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_GetTexture : number of args exception! argn must:1");
        return;
    }

    char name[TEXT_BUFFER_LENGTH] = {0};
    Local<String> mark = args[0]->ToString();
    mark->WriteUtf8(name);

    Image *image = EntityManager::instance()->retrieve<Image>(name, MODULE_POOL_NAME);
    if(image == NULL) {
        THROW_EXCEPTION(args.GetIsolate(), TError, "Graphics_Image_GetTexture : No such image ID");
        return;
    }
    args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), image->getTexture()));
}


void Hook_Image_OnLoad(void *image) {
    V8EventService::instance()->hookOnFinishResourceLoading(image);
}

}