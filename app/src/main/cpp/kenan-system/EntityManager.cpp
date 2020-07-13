//
// Created by hujia on 7/13/20.
//

#include "EntityManager.h"
#include "EntityPool.h"
#include <string>

namespace kenan_system {

template<class T>
void EntityManager::store(const char *name, const char *pool, T *ptr) {
    std::string stringPool = std::string(pool);
    if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
        pools[stringPool]->set(name, ptr);
    }
    else {
        pools[stringPool] = EntityPool::create(stringPool);
        pools[stringPool]->set(name, ptr);
    }
}

template<class T>
T* EntityManager::retrieve(const char *name, const char *pool) {
    std::string stringPool = std::string(pool);
    if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
       EntityPool *entityPool =  pools[stringPool];
       return entityPool->get(name);
    }
    return NULL;
}

void EntityManager::eliminate(const char *pool) {
    std::string stringPool = std::string(pool);
    if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
       EntityPool::destroy(&(pools[stringPool]));
    }
}

void EntityManager::eliminate(const char *name, const char *pool) {
    std::string stringPool = std::string(pool);
    if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
       EntityPool *entityPool =  pools[stringPool];
       entityPool->del(name);
    }
}

}