//
// Created by hujia on 7/13/20.
//

#ifndef KENAN_ENTITYPOOL_H
#define KENAN_ENTITYPOOL_H

#include <map>
#include <vector>
#include <string>

namespace kenan_system {

class EntityPool {

public:
    static EntityPool *create(const char *name);
    static EntityPool *create(std::string name);
    static void destroy(EntityPool **pool);

    ~EntityPool();

    void set(const char *key, unsigned int ptr);
    void set(std::string key, unsigned int ptr);
    unsigned int get(const char *key);
    unsigned int get(std::string key);

    template<class T>
    void set(const char *key, T *ptr);
    template<class T>
    void set(std::string key, T *ptr);

    template<class T>
    T* get(const char *key);
    template<class T>
    T* get(std::string key);

    unsigned int del(const char *key, bool soft = false);
    unsigned int del(std::string key, bool soft = false);

    template<class T>
    T* delT(const char *key, bool soft);
    template<class T>
    T* delT(std::string key, bool soft);

    void clean();
private:
    std::string name;
    std::vector<std::string> keySet;
    std::map<std::string, int> keyIndex;
    std::map<std::string, unsigned int> entityMap;
};

}

#endif //KENAN_ENTITYPOOL_H
