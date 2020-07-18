//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_V8MAINENGINE_H
#define KENAN_V8MAINENGINE_H

#include "V8Main.h"

using namespace v8;

namespace kenan_v8bindings {

class V8MainEngine: public V8Main {
public:
    static V8MainEngine *instance() {
        if(!sInstance) sInstance = new V8MainEngine();
        return sInstance;
    }

    void onFrameUpdateCallback();

private:
    static V8MainEngine *sInstance;
};

}

#endif //KENAN_V8MAINENGINE_H
