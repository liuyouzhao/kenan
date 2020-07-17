//
// Created by hujia on 7/13/20.
//

#ifndef KENAN_ENTITYMANAGER_H
#define KENAN_ENTITYMANAGER_H

#include <map>
#include <string>
#include "EntityPool.h"

namespace kenan_system {

class EntityManager {
public:

    static EntityManager *instance() {
        if(sInstance == NULL) {
            sInstance = new EntityManager();
        }
        return sInstance;
    }

    template<class T>
    void store(const char *name, const char *pool, T *ptr) {
        std::string stringPool = std::string(pool);
        if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
            pools[stringPool]->set<T>(name, ptr);
        }
        else {
            pools[stringPool] = EntityPool::create(stringPool);
            pools[stringPool]->set<T>(name, ptr);
        }
    }

    template<class T>
    T* retrieve(const char *name, const char *pool) {
        std::string stringPool = std::string(pool);
        if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
           EntityPool *entityPool =  pools[stringPool];
           return entityPool->get<T>(name);
        }
        return NULL;
    }

    void eliminate(const char *pool);
    void eliminate(const char *name, const char *pool);

private:
    std::map<std::string, EntityPool*> pools;
    static EntityManager *sInstance;
};

}

#endif //KENAN_ENTITYMANAGER_H