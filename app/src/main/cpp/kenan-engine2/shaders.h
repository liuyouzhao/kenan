#ifndef SHADER_H
#define SHADER_H

namespace shaders
{
#define BLUR_SIZE "u_radio"
#define BLUE_TEX_SIZE "u_tex_size"

    extern const char* NORMAL_FLAG_SHADER;
    extern const char* NORMAL_VERTEX_SHADER;

    extern const char* NORMAL_NOTEXTURE_FLAG_SHADER;
    extern const char* NORMAL_NOTEXTURE_VERTEX_SHADER;

    extern const char* SHADOW_FLAG_SHADER;
    extern const char* SHADOW_VERTEX_SHADER;
    
    extern const char* BLUR_FLAG_SHADER;
    extern const char* BLUR_VERTEX_SHADER;

} // namespace shaders

#endif // SHADER_H