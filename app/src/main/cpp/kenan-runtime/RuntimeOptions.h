//
// Created by hujia on 7/10/20.
//

#ifndef KENAN_RUNTIMEOPTIONS_H
#define KENAN_RUNTIMEOPTIONS_H

#include <string>

namespace kenan_runtime {

class RuntimeOptions {
friend class RuntimeApi;
public:
    void setWorkingDirectory(const char *dir);
    void setDefaultScriptPath(const char *path);
    void setScreenSize(int screenWidth, int screenHeight);
    void setEntryScriptCode(std::string code);
    void setMainThreadsNumber(const int num) { mainThreadsNumber = num; }

    const char* getWorkingDirectory() { return workingDirectory.c_str();    }
    const char* getDefaultScriptPath() {    return defaultScriptPath.c_str();   }

private:
    std::string workingDirectory;
    std::string defaultScriptPath;
    std::string entryScriptCode;

    int screenWidth;
    int screenHeight;
    int mainThreadsNumber;
};

}


#endif //KENAN_RUNTIMEOPTIONS_H
