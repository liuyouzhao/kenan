//
// Created by hujia on 7/13/20.
//

#include "EntityManager.h"

namespace kenan_system {

EntityManager* EntityManager::sInstance = NULL;

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