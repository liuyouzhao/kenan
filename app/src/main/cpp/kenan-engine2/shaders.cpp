#include "shaders.h"

namespace shaders
{
    const char* NORMAL_FLAG_SHADER = "\
                                    varying mediump vec4 _color;\
                                    varying mediump vec2 _uv;\
                                    uniform sampler2D sampler2d;\
                                    void main (void)\
                                    {\
                                    gl_FragColor = texture2D(sampler2d,_uv) * _color;\
                                    }";

    const char* NORMAL_VERTEX_SHADER = "\
                                    attribute highp vec4 myVertex;\
                                    attribute highp vec4 myColor;\
                                    attribute highp vec2 myUV;\
                                    varying mediump vec4 _color;\
                                    varying mediump vec2 _uv;\
                                    uniform mediump vec2 u_tex_size;\
                                    void main()\
                                    {\
                                    _color = myColor;\
                                    _uv = myUV;\
                                    gl_Position = myVertex;\
                                    }";

    const char* NORMAL_NOTEXTURE_FLAG_SHADER = "\
                                    varying mediump vec4 _color;\
                                    void main (void)\
                                    {\
                                    gl_FragColor = _color;\
                                    }";

    const char* NORMAL_NOTEXTURE_VERTEX_SHADER =  "\
                                    attribute highp vec4 myVertex;\
                                    attribute highp vec4 myColor;\
                                    attribute highp vec4 myUV;\
                                    varying mediump vec4 _color;\
                                    void main()\
                                    {\
                                    _color = myColor;\
                                    gl_Position = myVertex;\
                                    }";

    const char* SHADOW_FLAG_SHADER = "\
                                    varying mediump vec4 _color;\
                                    varying mediump vec4 _uv;\
                                    void main (void)\
                                    {\
                                    gl_FragColor = _color;\
                                    }";

    const char* SHADOW_VERTEX_SHADER = "\
                                    attribute highp vec4 myVertex;\
                                    attribute highp vec4 myColor;\
                                    attribute highp vec4 myUV;\
                                    varying mediump vec4 _color;\
                                    varying mediump vec4 _uv;\
                                    void main()\
                                    {\
                                    _color = myColor;\
                                    _uv = myUV;\
                                    gl_Position = myVertex;\
                                    }";

    const char* BLUR_FLAG_SHADER = "\
                                    varying mediump vec4 _color;\
                                    varying mediump vec2 _uv;\
                                    \
                                    mediump vec4 _colorTmp = vec4(0.0, 0.0, 0.0, 0.0);\
                                    mediump vec4 _colorFinal = vec4(1.0, 1.0, 1.0, 1.0);\
                                    \
                                    varying mediump vec2 _tex_size;\
                                    uniform sampler2D sampler2d;\
                                    \
                                    mediump vec2 _uvg = vec2(0.0, 0.0);   \
                                    mediump float alpha = 0.0;\
                                    mediump float j = 0.0; \
                                    mediump float i = 0.0; \
                                    void main (void)\
                                    {\
                                         for (i = -2.0; i <= 2.0; i+=1.0) { \
                                            for(j = -2.0; j <= 2.0; j+=1.0) { \
                                                _uvg = vec2(_uv.x+(j/128.0), _uv.y+(i/128.0));\
                                                _colorTmp = texture2D(sampler2d, _uvg);\
                                                alpha += _colorTmp.a * 0.04;\
                                            } \
                                         } \
                                        \
                                        gl_FragColor = _color;\
                                        gl_FragColor.a = alpha * _color.a;\
                                    }";

    const char* BLUR_VERTEX_SHADER = "\
                                    attribute highp vec4 myVertex;\
                                    attribute highp vec4 myColor;\
                                    attribute highp vec2 myUV;\
                                    varying mediump vec4 _color;\
                                    varying mediump vec2 _uv;\
                                    varying mediump vec2 _tex_size;\
                                    \
                                    uniform mediump vec2 u_tex_size;\
                                    void main()\
                                    {\
                                    _color = myColor;\
                                    _uv = myUV;\
                                    _tex_size = u_tex_size;\
                                    gl_Position = myVertex;\
                                    }";

} // namespace shaders
