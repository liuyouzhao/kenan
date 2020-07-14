//
// Created by hujia on 7/13/20.
//

#include "EntityPool.h"


namespace kenan_system {

EntityPool::~EntityPool() {
    clean();
}

EntityPool *EntityPool::create(const char *name) {
    EntityPool *pool = new EntityPool();
    pool->name = std::string(name);
    return pool;
}

EntityPool *EntityPool::create(std::string name) {
    EntityPool *pool = new EntityPool();
    pool->name = name;
    return pool;
}

void EntityPool::destroy(EntityPool **pool) {
    delete *pool;
    *pool = NULL;
}

void EntityPool::set(const char *key, unsigned int ptr) {
    set(std::string(key), ptr);
}

void EntityPool::set(std::string key, unsigned int ptr) {
    if(entityMap.find(key) != entityMap.end()) {
        del(key);
    }
    keySet.push_back(key);
    keyIndex[key] = keySet.size() - 1;
    entityMap[key] = ptr;
}

unsigned int EntityPool::get(const char *key) {
    std::string k = std::string(key);
    return get(k);
}

unsigned int EntityPool::get(std::string key) {
    if(entityMap.find(key) != entityMap.end()) {
        return entityMap[key];
    }
    return 0;
}

unsigned int EntityPool::del(const char *key, bool soft) {
    return del(std::string(key), soft);
}

unsigned int EntityPool::del(std::string key, bool soft) {
    if(entityMap.find(key) != entityMap.end()) {
        unsigned int ptr = entityMap[key];
        int ki = keyIndex[key];
        keySet.erase(keySet.begin() + ki);
        keyIndex.erase(key);
        entityMap.erase(key);

        if(!soft) {
            delete (int*)ptr;
            return 0;
        }

        return ptr;
    }
    return 0;
}

template<class T>
T* EntityPool::delT(const char *ckey, bool soft) {
    std::string key = std::string(ckey);
    if(entityMap.find(key) != entityMap.end()) {
        T *ptr = static_cast<T*>(entityMap[key]);
        int ki = keyIndex[key];
        keySet.erase(keySet.begin() + ki);
        keyIndex.erase(key);
        entityMap.erase(key);

        if(!soft) {
            delete ptr;
            return NULL;
        }

        return ptr;
    }
    return NULL;
}

template<class T>
T* EntityPool::delT(std::string key, bool soft) {
    if(entityMap.find(key) != entityMap.end()) {
        T *ptr = static_cast<T*>(entityMap[key]);
        int ki = keyIndex[key];
        keySet.erase(keySet.begin() + ki);
        keyIndex.erase(key);
        entityMap.erase(key);

        if(!soft) {
            delete ptr;
            return NULL;
        }

        return ptr;
    }
    return NULL;
}

void EntityPool::clean() {
    for(std::vector<std::string>::iterator it = keySet.begin(); it != keySet.end(); ++it) {
        unsigned int ptr = entityMap[*it];
        delete (int*)ptr;
    }
    keySet.clear();
    keyIndex.clear();
}

}