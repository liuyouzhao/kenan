//
// Created by hujia on 7/13/20.
//

#ifndef KENAN_ENTITYMANAGER_H
#define KENAN_ENTITYMANAGER_H

#include <map>
#include <string>
#include <pthread.h>
#include "defines.h"
#include "EntityPool.h"

#undef LOG_TAG
#define  LOG_TAG    "EntityManager"

namespace kenan_system {

class EntityManager {
public:
    EntityManager();

    static EntityManager *instance() {
        if(sInstance == NULL) {
            sInstance = new EntityManager();
        }
        return sInstance;
    }

    template<class T>
    void store(const char *name, const char *pool, T *ptr) {
        std::string stringPool = std::string(pool);
        pthread_mutex_lock(&mutex);
        if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
            pools[stringPool]->set<T>(name, ptr);
        }
        else {
            pools[stringPool] = EntityPool::create(stringPool);
            pools[stringPool]->set<T>(name, ptr);
        }
        pthread_mutex_unlock(&mutex);
    }

    template<class T>
    T* retrieve(const char *name, const char *pool) {
        pthread_mutex_lock(&mutex);
        std::string stringPool = std::string(pool);
        if(pools.find(stringPool) != pools.end() && pools[stringPool] != NULL) {
            EntityPool *entityPool =  pools[stringPool];
            T *ptr = entityPool->get<T>(name);
            if(ptr == NULL) {
                LOGE("Resource MARK: %s NOT exist.", name);
            }
            pthread_mutex_unlock(&mutex);
            return entityPool->get<T>(name);
        }
        LOGE("Resource POOL: %s NOT exist.", pool);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    void eliminate(const char *pool);
    void eliminate(const char *name, const char *pool);

private:
    std::map<std::string, EntityPool*> pools;
    static EntityManager *sInstance;
    pthread_mutex_t mutex;
};

}

#endif //KENAN_ENTITYMANAGER_H
