#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include "Image.h"
#include "ImageData.h"
#include "CanvasPattern.h"
#include "CanvasGradient.h"
#include <gl2d_impl.h>

#include <set>
#include <string>
#define MAXIMAGEDATA  62914560 //60 *1024 * 1024

namespace DCanvas
{
class Image;
class Canvas;
class ImageData;
class CanvasPattern;
class CanvasGradient;

class MemoryManager
{
public:
    static MemoryManager* getManager();

    ~MemoryManager();

    void Destroy();

    void addFrameGarbagePoint(CanvasGradient *);
    void clearFrameGarbagePoint();

    /*here has 3 types we can use as one type ObjectWrap maybe can be done*/
    void AddImage(Image* );
    void RemoveImage(Image* );
    void DeleteImage(Image*);
    
    void AddImageData(ImageData *);
    void RemoveImageData(ImageData*);
    void DeleteImageData(ImageData*);
    
    void AddCanvasPattern(CanvasPattern*);
    void RemoveCanvasPattern(CanvasPattern*);
    CanvasPattern* FindCanvasPattern(Image* img, std::string type, int cavnasId);
    void AddCanvasGradient(CanvasGradient*);
    void RemoveCanvasGradient(CanvasGradient*);
    HTEXTURE FindCanvasGradient(CanvasGradient* cg);
#ifdef __DEBUGMEMORY__
    static unsigned int			s_memory_count;
#endif

	void clearResourceWithCanvasId(int canvasId);
private:
    MemoryManager() { s_imagedata_size = 0; }
    
    static MemoryManager*       s_self;
    static unsigned int         s_imagedata_size;


    std::set<Image*>            m_imageObjs;
    std::set<ImageData*>        m_imageDataObjs;
    std::set<CanvasPattern*>    m_canvasPatternObjs;
    std::set<CanvasGradient*>   m_canvasGradientObjs;

    std::set<CanvasGradient*>   m_garbageArray;
};

} // namespace DCanvas

#ifndef MEMORYNEW_H
#define MEMORYNEW_H
#ifdef __DEBUGMEMORY__
/*
void *operator new(unsigned int size, const char *file, int line);
void *operator new[](size_t c);
void operator delete(void * p);
void operator delete [] (void * p);
void operator delete(void * p, const char *file, int line);
void operator delete [] (void * p, const char *file, int line);
void* __malloc(int);
void __free(void*);
*/
#endif
#endif


#endif // MEMORYMANAGER_H
