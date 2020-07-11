//
// Created by hujia on 08/07/20.
//

#ifndef KENAN_SALFILEOPERATION_H
#define KENAN_SALFILEOPERATION_H

namespace kenan_sal {

enum SAL_FOLDER_TYPE {
    RW_LOCATION,
    RO_LOCATION
};

int SAL_systemReadFile(const char *filename, char *buf, int &len, int maxBufferSize, SAL_FOLDER_TYPE type);
int SAL_systemWriteFile(const char *filename, char *buf, int len, bool append);
}

#endif //KENAN_SALFILEOPERATION_H
