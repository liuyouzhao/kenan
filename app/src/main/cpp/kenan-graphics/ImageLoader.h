#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "Image.h"
#include <string>
#include "CloudxResourceLoader.h"

namespace DCanvas
{

class resultData
{
public:
    void setData(void* data) { this->m_data = data; }
    void* getData() { return m_data; }
    void setImage(Image* img) { this->m_img = img; }
    Image* getImage() { return m_img; }
    void setUrl(std::string url) { m_url = url; }
    std::string getUrl() { return m_url; }
    void setPath(std::string path) { m_path = path; }
    std::string getPath() { return m_path; }
    void setPort(int port) { m_port = port; }
    int getPort() { return m_port; }
    resultData() { m_port = 80; }

private:
    std::string m_url;
    std::string m_path;
    int         m_port;

    Image* m_img;

    void*  m_data;
};

class ImageLoader : public ResourceLoader
{
public:
    virtual ~ImageLoader();
    static   ImageLoader* create();
    void     load(char* url, int length, Image* img);
    bool       startThread();
protected:
    static void* notifyFinished(void*, int, void* img);
    static void* thread_func(void*);

private:
    ImageLoader();
    Image* m_img;
};

} // namespace DCanvas

#endif // IMAGELOADER_H
