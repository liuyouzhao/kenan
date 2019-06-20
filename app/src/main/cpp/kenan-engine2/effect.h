/*
** gl's 2d Engine 1.00
** Copyright (C) 2011-2014, aliyun cloud apps
**
** Common core implementation header
*/
#ifndef EFFECT_H
#define EFFECT_H

#include "shaders.h"

// Index to bind the attributes to vertex shaders
#define GL2D_VERTEX_ARRAY    0
#define GL2D_TEXCOORD_ARRAY    1
#define GL2D_COLOR_ARRAY    2

class Gl2DEffect
{
public:
    bool CreateEffect(const char* _vertex_shader, const char* _flag_shader);

    void UseThisEffect();

    void FreeThisEffect();

    void SetFloat(float value, const char* name);
    void SetFloatv2(float x, float y, const char* name);
    void SetInt(int value, const char* name);

protected:
private:
    bool hasFreed;

    GLuint uiFragShader;
    GLuint uiVertShader;
    /* Used to hold the program handle (made out of the two previous shaders */
    GLuint uiProgramObject;
};

#endif // EFFECT_H
