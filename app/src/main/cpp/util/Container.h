#ifndef CONTAINER_H
#define CONTAINER_H

#include <android/log.h>
#define  LOG_TAG    "Container"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#include <stdlib.h>

namespace DCanvas
{

template <typename T>
inline void TSwap(T& a, T& b)
{
    T c(a);
    a = b;
    b = c;
}

template <typename T>
class TDArray
{
public:
    TDArray()
    {
        LOGE("TDArray");
        fReserve = fCount = 0;
        fArray = NULL;
    }

    TDArray(const T src[], size_t count)
    {
        fReserve = fCount = 0;
        fArray = NULL;
        if (count)
        {
            fArray = new T[count];
            memcpy(fArray, src, sizeof(T) * count);
            fReserve = fCount = count;
        }
    }

    TDArray(const TDArray<T>& src)
    {
        fReserve = fCount = 0;
        fArray = NULL;
        TDArray<T> tmp(src.fArray, src.fCount);
        this->swap(tmp);
    }

    ~TDArray()
    {
        if(fArray)
            delete[] fArray;
    }

    TDArray<T>& operator=(const TDArray<T>& src)
    {
        if (this != &src)
        {
            if (src.fCount > fReserve)
            {
                TDArray<T> tmp(src.fArray, src.fCount);
                this->swap(tmp);
            }
            else
            {
                memcpy(fArray, src.fArray, sizeof(T) * src.fCount);
                fCount = src.fCount;
            }
        }
        return *this;
    }

    friend int operator==(const TDArray<T>& a, const TDArray<T>& b)
    {
        return  a.fCount == b.fCount &&
                (a.fCount == 0
                || !memcmp(a.fArray, b.fArray, a.fCount * sizeof(T)));
    }

    void swap(TDArray<T>& other)
    {
        TSwap(fArray, other.fArray);
        TSwap(fReserve, other.fReserve);
        TSwap(fCount, other.fCount);
    }

    bool isEmpty() const { return fCount == 0; }
    int count() const { return fCount; }
    T*  begin() const { return fArray; }
    T*  end() const { return fArray ? fArray + fCount : NULL; }

    T&  operator[](int index) const
    {
        //assert(index > fCount);
        return fArray[index];
    }

    void reset()
    {
        if (fArray)
        {
            delete[] fArray;
            fArray = NULL;
            fReserve = fCount = 0;
        }
        else
        {
        }
    }

    void rewind()
    {
        // same as setCount(0)
        fCount = 0;
    }

    void setCount(size_t count)
    {
        if (count > fReserve)
        {
            this->growBy(count - fCount);
        }
        else
        {
            fCount = count;
        }
    }

    void setReserve(size_t reserve)
    {
        if (reserve > fReserve)
        {
            size_t count = fCount;
            this->growBy(reserve - fCount);
            fCount = count;
        }
    }

    T* append()
    {
        return this->append(1, NULL);
    }

    T* append(size_t count, const T* src = NULL)
    {
        unsigned oldCount = fCount;
        if (count)
        {
            //ASSERT(src == NULL || fArray == NULL ||
            //src + count <= fArray || fArray + oldCount <= src);

            this->growBy(count);
            if (src)
            {
                memcpy(fArray + oldCount, src, sizeof(T) * count);
            }
        }
        return fArray + oldCount;
    }

    T* insert(size_t index)
    {
        return this->insert(index, 1, NULL);
    }

    T* insert(size_t index, size_t count, const T* src = NULL) {
        int oldCount = fCount;
        this->growBy(count);
        T* dst = fArray + index;
        memmove(dst + count, dst, sizeof(T) * (oldCount - index));
        if (src) {
            memcpy(dst, src, sizeof(T) * count);
        }
        return dst;
    }

    void remove(size_t index, size_t count = 1)
    {
        fCount = fCount - count;
        memmove(fArray + index, fArray + index + count, sizeof(T) * (fCount - index));
    }

    int find(const T& elem) const
    {
        const T* iter = fArray;
        const T* stop = fArray + fCount;

        for (; iter < stop; iter++)
        {
            if (*iter == elem)
            {
                return (int) (iter - fArray);
            }
        }
        return -1;
    }

    int rfind(const T& elem) const
    {
        const T* iter = fArray + fCount;
        const T* stop = fArray;

        while (iter > stop)
        {
            if (*--iter == elem)
            {
                return iter - stop;
            }
        }
        return -1;
    }

    T*          push() { return this->append(); }
    void        push(const T& elem) { *this->append() = elem; }
    const T&    top() const { return (*this)[fCount - 1]; }
    T&          top() { return (*this)[fCount - 1]; }
    void        pop(T* elem) { if (elem) *elem = (*this)[fCount - 1]; --fCount; }
    void        pop() { --fCount; }

    void deleteAll()
    {
        T*  iter = fArray;
        T*  stop = fArray + fCount;
        while (iter < stop)
        {
            delete (*iter);
            iter += 1;
        }
        this->reset();
    }

private:
    T*      fArray;
    size_t  fReserve, fCount;

    void growBy(size_t extra)
    {
        if (fCount + extra > fReserve)
        {
            size_t size = fCount + extra + 4;
            size += size >> 2;
            T* fold = fArray;
            fArray = new T[size];

            if (fold)
            {
                memcpy(fArray, fold, fCount * sizeof(T));
                delete[] fold;
            }
            fReserve = size;
        }
        fCount += extra;
    }
};

} // namespace

#endif // CONTAINER_H
