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
    RuntimeOptions();
    void setWorkingDirectory(const char *dir);
    void setDefaultScriptPath(const char *path);
    void setScreenSize(int screenWidth, int screenHeight);
    void setEntryScriptCode(std::string code);
    void setCustomerThreadsNumber(const int num) { customerThreadsNumber = num; }

    const char* getWorkingDirectory() { return workingDirectory.c_str();    }
    const char* getDefaultScriptPath() {    return defaultScriptPath.c_str();   }

    int getScreenWidth() {  return screenWidth; }
    int getScreenHeight() {  return screenHeight; }

private:
    std::string workingDirectory;
    std::string defaultScriptPath;
    std::string entryScriptCode;
    std::string mainTaskId;

    int screenWidth;
    int screenHeight;
    int customerThreadsNumber;
    int resourceThreadsNumber;
};

}


#endif //KENAN_RUNTIMEOPTIONS_H
