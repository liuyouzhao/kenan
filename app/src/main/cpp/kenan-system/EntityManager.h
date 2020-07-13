//
// Created by hujia on 7/13/20.
//

#ifndef KENAN_ENTITYMANAGER_H
#define KENAN_ENTITYMANAGER_H

#include <map>

namespace kenan_system {

class EntityPool;

class EntityManager {
public:
    template<class T>
    void store(const char *name, const char *pool, T *ptr);

    template<class T>
    T* retrieve(const char *name, const char *pool);

    void eliminate(const char *pool);
    void eliminate(const char *name, const char *pool);

private:
    std::map<std::string, EntityPool*> pools;
};

}

#endif //KENAN_ENTITYMANAGER_H
