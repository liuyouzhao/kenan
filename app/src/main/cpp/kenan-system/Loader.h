#ifndef LOADER_H
#define LOADER_H

namespace DCanvas
{

class Loader
{

public:
    Loader(){}
    virtual ~Loader(){}

protected:
    virtual void* notifyFinished(void*) = 0;

private:

};

} // namespace DCanvas

#endif // LOADER_H
