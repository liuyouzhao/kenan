//
// Created by hujia on 08/07/20.
//

#include "SAL.h"

static unsigned char *loadBase64ImageRGBA(const char* base64, int *outWidth, int *outHeight) {

}

SI_ImageOp si_imageOp = {
    .SI_loadBase64ImageRGBA = loadBase64ImageRGBA
};