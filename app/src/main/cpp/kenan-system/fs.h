#ifndef FS_H
#define FS_H

namespace DCanvas
{

unsigned int* dataFromFile(const char* filename, int& width, int& height);
unsigned int* dataFromstream(const char* data, int &width, int& height, int len);
unsigned int* convertFromYUV(unsigned char* stream, int w, int h, int len);
unsigned int* convertFromRGB565(char* stream, int w, int h, int len);

} // namespace DCanvas

#endif // FS_H
