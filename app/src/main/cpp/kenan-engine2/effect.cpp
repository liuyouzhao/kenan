#include "defines.h"
#include "gl2d_impl.h"
#include "effect.h"

#define MAX_EFFECTS_NUMBER 100


Gl2DEffect* GEFFECTS[MAX_EFFECTS_NUMBER] = {0};

int current_length = 0;

HEFFECT CALL Gl2d_Impl::Effect_Load(gl2EffectState shader)
{
    if(current_length > MAX_EFFECTS_NUMBER)
        return -1;

    HEFFECT hEffect = -1;
    const char* _vertex_shader = NULL;
    const char* _flag_shader = NULL;

    switch (shader)
    {
        case GL2D_NORMAL_NOTEXTURE:
            _vertex_shader = shaders::NORMAL_NOTEXTURE_VERTEX_SHADER;
            _flag_shader = shaders::NORMAL_NOTEXTURE_FLAG_SHADER;
            break;
        case GL2D_NORMAL:
            _vertex_shader = shaders::NORMAL_VERTEX_SHADER;
            _flag_shader = shaders::NORMAL_FLAG_SHADER;
            break;
        case GL2D_BI:
            break;
        case GL2D_DARK:
            break;
        case GL2D_BLUR:
            _vertex_shader = shaders::BLUR_VERTEX_SHADER;
            _flag_shader = shaders::BLUR_FLAG_SHADER;
            break;
    }

    GEFFECTS[current_length] = new Gl2DEffect();
    GEFFECTS[current_length]->CreateEffect(_vertex_shader, _flag_shader);

    current_length ++;
    hEffect = current_length;
    return hEffect;
}

void CALL Gl2d_Impl::Effect_Free(HEFFECT eff)
{
    HEFFECT hEffect = eff - 1;
    GEFFECTS[hEffect]->FreeThisEffect();
}

void CALL Gl2d_Impl::Effect_Free_All()
{
    LOOP(i, MAX_EFFECTS_NUMBER)
    {
        if(GEFFECTS[i])
        {
            SAFE_DELETE(GEFFECTS[i]);
        }
    }
    current_length = 0;
}

void CALL Gl2d_Impl::Effect_Active(HEFFECT eff, bool hasTex)
{
    HEFFECT hEffect = eff - 1;
    //GEFFECTS[hEffect]->UseThisEffect(hasTex);
    CurEffect = hEffect;
    return;
}

void CALL Gl2d_Impl::Effect_SetFloat(HEFFECT effect, float value, const char* name)
{
    HEFFECT hEffect = effect - 1;
    GEFFECTS[hEffect]->UseThisEffect();
    GEFFECTS[hEffect]->SetFloat(value, name);
}

void CALL Gl2d_Impl::Effect_SetFloatv2(HEFFECT effect, float x, float y, const char* name)
{
    HEFFECT hEffect = effect - 1;
    GEFFECTS[hEffect]->UseThisEffect();
    GEFFECTS[hEffect]->SetFloatv2(x, y, name);
}

void CALL Gl2d_Impl::Effect_SetInt(HEFFECT effect, int value, const char* name)
{
    HEFFECT hEffect = effect - 1;
    GEFFECTS[hEffect]->UseThisEffect();
    GEFFECTS[hEffect]->SetInt(value, name);
}

void Gl2DEffect::SetFloat(float value, const char* name)
{
    // First gets the location of that variable in the shader using its name
    int i32Location = glGetUniformLocation(uiProgramObject, name);
    // Then passes the matrix to that variable
    glUniform1f( i32Location, value);
}

void Gl2DEffect::SetInt(int value, const char* name)
{
    // First gets the location of that variable in the shader using its name
    int i32Location = glGetUniformLocation(uiProgramObject, name);
    // Then passes the matrix to that variable
    glUniform1i( i32Location, value);
}

void Gl2DEffect::SetFloatv2(float x, float y, const char* name)
{
    // First gets the location of that variable in the shader using its name
    int i32Location = glGetUniformLocation(uiProgramObject, name);
    // Then passes the matrix to that variable
    glUniform2f( i32Location, x, y);
}

void Gl2DEffect::FreeThisEffect()
{
    // Frees the OpenGL handles for the program and the 2 shaders
    glDeleteProgram(uiProgramObject);
    glDeleteShader(uiVertShader);
    glDeleteShader(uiFragShader);
}

bool Gl2DEffect::CreateEffect(const char* _vertex_shader, const char* _flag_shader)
{
    /////// Frag Shader
    // Create the fragment shader object
    uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the source code into it
    glShaderSource(uiFragShader, 1, (const char**)&_flag_shader, NULL);

    // Compile the source code
    glCompileShader(uiFragShader);

    // Check if compilation succeeded
    GLint bShaderCompiled;
    glGetShaderiv(uiFragShader, GL_COMPILE_STATUS, &bShaderCompiled);
    if (!bShaderCompiled)
    {
        return false;
    }

    /////// Vert Shader
    // Loads the vertex shader in the same way
    uiVertShader = glCreateShader(GL_VERTEX_SHADER);

    // Load the source code into it
    glShaderSource(uiVertShader, 1, (const char**)&_vertex_shader, NULL);

    // Compile the source code
    glCompileShader(uiVertShader);

    // Check if compilation succeeded
    glGetShaderiv(uiVertShader, GL_COMPILE_STATUS, &bShaderCompiled);
    if (!bShaderCompiled)
    {
        return false;
    }

    /////// Create shader program
    // Create the shader program
    uiProgramObject = glCreateProgram();

    // Attach the fragment and vertex shaders to it
    glAttachShader(uiProgramObject, uiFragShader);
    glAttachShader(uiProgramObject, uiVertShader);

    // Bind the custom vertex attribute "myVertex" to location VERTEX_ARRAY
    glBindAttribLocation(uiProgramObject, VERTEX_ARRAY, "myVertex");
    glBindAttribLocation(uiProgramObject, TEXCOORD_ARRAY, "myUV");
    glBindAttribLocation(uiProgramObject, COLOR_ARRAY, "myColor");

    // Link the program
    glLinkProgram(uiProgramObject);

    // Check if linking succeeded in the same way we checked for compilation success
    GLint bLinked;
    glGetProgramiv(uiProgramObject, GL_LINK_STATUS, &bLinked);
    if (!bLinked)
    {
        return false;
    }

    return true;
}

void Gl2DEffect::UseThisEffect()
{
    // Actually use the created program
    glUseProgram(uiProgramObject);

    // Sets the sampler2D variable to the first texture unit
    glUniform1i(glGetUniformLocation(uiProgramObject, "sampler2d"), 0);

    //if(hasTex)
    //{
        // Sets the sampler2D variable to the first texture unit
    //    glUniform1i(glGetUniformLocation(uiProgramObject, "sampler2d"), 0);
    //}
}