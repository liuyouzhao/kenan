//
// Created by hujia on 08/07/20.
//
#include "defines.h"
#include "SAL.h"
#include "SALFileOperation.h"

#define LOG_TAG "SALFileOperation"

namespace kenan_sal {

int SAL_systemReadFile(const char *filename, char *buf, int &len, int maxBufferSize, SAL_FOLDER_TYPE type) {
    switch(type) {
    case SAL_FOLDER_TYPE::RW_LOCATION:
        if(!si_fileOp.SI_readFileFromRWLocation) {
            LOGE("%s SI_readFileFromRWLocation NOT hooked", __FUNCTION__);
            return -1;
        }
        return si_fileOp.SI_readFileFromRWLocation(filename, buf, len, maxBufferSize);
    break;
    case SAL_FOLDER_TYPE::RO_LOCATION:
        if(!si_fileOp.SI_readFileFromROLocation) {
            LOGE("%s SI_readFileFromROLocation NOT hooked", __FUNCTION__);
            return -1;
        }
        return si_fileOp.SI_readFileFromROLocation(filename, buf, len, maxBufferSize);
    break;
    }
    LOGE("%s SI_FOLDER_TYPE is not valid type=%d", __FUNCTION__, type);
    return -1;
}

int SAL_systemWriteFile(const char *filename, char *buf, int len, bool append) {
    if(append) {
        if(!si_fileOp.SI_writeFileToRWLocation_AppendNew) {
            LOGE("%s SI_writeFileToRWLocation_AppendNew NOT hooked", __FUNCTION__);
            return -1;
        }
        return si_fileOp.SI_writeFileToRWLocation_AppendNew(filename, buf, len);
    }
    else {
        if(!si_fileOp.SI_writeFileToRWLocation_UpdateNew) {
            LOGE("%s SI_writeFileToRWLocation_UpdateNew NOT hooked", __FUNCTION__);
            return -1;
        }
        return si_fileOp.SI_writeFileToRWLocation_UpdateNew(filename, buf, len);
    }
}

}