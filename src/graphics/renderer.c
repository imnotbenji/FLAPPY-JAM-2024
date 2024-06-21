// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______   ______   __   __   _____    ______   ______   ______   ______     //
//  /\  == \ /\  ___\ /\ "-.\ \ /\  __-. /\  ___\ /\  == \ /\  ___\ /\  == \    //
//  \ \  __< \ \  __\ \ \ \-.  \\ \ \/\ \\ \  __\ \ \  __< \ \  __\ \ \  __<    //
//   \ \_\ \_\\ \_____\\ \_\\"\_\\ \____- \ \_____\\ \_\ \_\\ \_____\\ \_\ \_\  //
//    \/_/ /_/ \/_____/ \/_/ \/_/ \/____/  \/_____/ \/_/ /_/ \/_____/ \/_/ /_/  //
//                                                                              //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// graphics/renderer.c

#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphics/renderer.h"

typedef void          GLvoid;
typedef ptrdiff_t     GLintptr;
typedef ptrdiff_t     GLsizeiptr;
typedef unsigned int  GLenum;
typedef unsigned int  GLbitfield;
typedef unsigned char GLboolean;
typedef char          GLchar;
typedef int           GLint;
typedef unsigned int  GLuint;
typedef int           GLsizei;
typedef float         GLfloat;
typedef double        GLdouble;

#define GL_TRUE                          1
#define GL_FALSE                         0
#define GL_NONE                          0
#define GL_ZERO                          0x0000
#define GL_ONE                           0x0001
#define GL_UNSIGNED_BYTE                 0x1401
#define GL_INT                           0x1404
#define GL_UNSIGNED_INT                  0x1405
#define GL_FLOAT                         0x1406
#define GL_BOOL                          0x8B56
#define GL_COMPILE_STATUS                0x8B81
#define GL_LINK_STATUS                   0x8B82
#define GL_VALIDATE_STATUS               0x8B83
#define GL_INFO_LOG_LENGTH               0x8B84
#define GL_COLOR_BUFFER_BIT              0x4000
#define GL_TEXTURE0                      0x84C0
#define GL_TEXTURE_2D                    0x0DE1
#define GL_TEXTURE_WRAP_S                0x2802
#define GL_TEXTURE_WRAP_T                0x2803
#define GL_TEXTURE_MAG_FILTER            0x2800
#define GL_TEXTURE_MIN_FILTER            0x2801
#define GL_NEAREST                       0x2600
#define GL_LINEAR                        0x2601
#define GL_RGBA                          0x1908
#define GL_RGBA16F                       0x881A
#define GL_REPEAT                        0x2901
#define GL_CLAMP_TO_EDGE                 0x812F
#define GL_CLAMP_TO_BORDER               0x812D
#define GL_MIRRORED_REPEAT               0x8370
#define GL_FRONT_AND_BACK                0x0408
#define GL_LINES                         0x0001
#define GL_TRIANGLES                     0x0004
#define GL_TRIANGLE_STRIP                0x0005
#define GL_QUADS                         0x0007
#define GL_FRAGMENT_SHADER               0x8B30
#define GL_VERTEX_SHADER                 0x8B31
#define GL_ARRAY_BUFFER                  0x8892
#define GL_ELEMENT_ARRAY_BUFFER          0x8893
#define GL_TEXTURE_BUFFER                0x8C2A
#define GL_DYNAMIC_DRAW                  0x88E8
#define GL_STATIC_DRAW                   0x88E4
#define GL_FRAMEBUFFER                   0x8D40
#define GL_FRAMEBUFFER_BINDING           0x8CA6
#define GL_FRAMEBUFFER_COMPLETE          0x8CD5
#define GL_READ_FRAMEBUFFER              0x8CA8
#define GL_DRAW_FRAMEBUFFER              0x8CA9
#define GL_COLOR_ATTACHMENT0             0x8CE0
#define GL_UNSIGNED_INT_24_8             0x84FA
#define GL_TEXTURE_COMPARE_MODE          0x884C
#define GL_COMPARE_REF_TO_TEXTURE        0x884E
#define GL_CULL_FACE                     0x0B44
#define GL_FRONT                         0x0404
#define GL_BACK                          0x0405
#define GL_BLEND                         0x0BE2
#define GL_FUNC_ADD                      0x8006
#define GL_FUNC_REVERSE_SUBTRACT         0x800B
#define GL_SRC_ALPHA                     0x0302
#define GL_ONE_MINUS_SRC_ALPHA           0x0303
#define GL_NO_ERROR                      0x0000
#define GL_INVALID_ENUM                  0x0500
#define GL_INVALID_VALUE                 0x0501
#define GL_INVALID_OPERATION             0x0502
#define GL_STACK_OVERFLOW                0x0503
#define GL_STACK_UNDERFLOW               0x0504
#define GL_OUT_OF_MEMORY                 0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

typedef void* (*GLLoadFunc)(const char* name);
typedef const GLenum (*GLGETERRORPROC)(void);
typedef void (*GLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (*GLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void (*GLENABLEPROC)(GLenum mode);
typedef void (*GLDISABLEPROC)(GLenum mode);
typedef void (*GLCLEARPROC)(GLenum mask);
typedef void (*GLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*GLVIEWPORTPROC)(GLint x, GLint y, GLint width, GLint height);
typedef void (*GLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (*GLDRAWELEMENTSPROC)(GLenum mode, GLint count, GLenum type, void* indices);
typedef void (*GLGENVERTEXARRAYSPROC)(GLint n, GLuint* arrays);
typedef void (*GLBINDVERTEXARRAYPROC)(GLuint id);
typedef void (*GLGENBUFFERSPROC)(GLint n, GLuint* arrays);
typedef void (*GLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (*GLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (*GLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (*GLDELETEBUFFERSPROC)(GLint n, GLuint* buffers);
typedef void (*GLDELETEVERTEXARRAYSPROC)(GLint n, GLuint* arrays);
typedef void (*GLENABLEVERTEXATTRIBARRAYPROC)(GLuint location);
typedef void (*GLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLint stride, const void* pointer);
typedef void (*GLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (*GLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
typedef void (*GLGENTEXTURESPROC)(GLint n, void* textures);
typedef void (*GLTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalFormat, GLint width, GLint height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (*GLTEXPARAMETERIPROC)(GLenum target, GLenum name, GLint param);
typedef void (*GLTEXPARAMETERFVPROC)(GLenum target, GLenum name, GLfloat* param);
typedef void (*GLACTIVETEXTUREPROC)(GLuint id);
typedef void (*GLBINDTEXTUREPROC)(GLenum target, GLuint id);
typedef void (*GLDELETETEXTURESPROC)(GLint n, GLuint* textures);
typedef GLuint (*GLCREATESHADERPROC)(GLenum type);
typedef void (*GLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (*GLDELETESHADERPROC)(GLuint shader);
typedef void (*GLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
typedef void (*GLCOMPILESHADERPROC)(GLuint shader);
typedef void (*GLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* result);
typedef GLuint (*GLCREATEPROGRAMPROC)(void);
typedef void (*GLDELETEPROGRAMPROC)(GLuint program);
typedef void (*GLLINKPROGRAMPROC)(GLuint program);
typedef void (*GLVALIDATEPROGRAMPROC)(GLuint pipeline);
typedef void (*GLUSEPROGRAMPROC)(GLuint program);
typedef void (*GLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef GLint (*GLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void (*GLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (*GLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (*GLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (*GLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*GLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (*GLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void (*GLUNIFORMMATRIX4FVPROC)(GLint location, GLint count, GLboolean transpose, const GLfloat* value);
typedef void (*GLCULLFACEPROC)(GLenum mode);
typedef void (*GLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (*GLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
typedef void (*GLGENFRAMEBUFFERSPROC)(GLint n, void* textures);
typedef void (*GLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef const GLenum (*GLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
typedef void (*GLBINDFRAMEBUFFERPROC)(GLenum target, GLuint id);
typedef void (*GLDELETEFRAMEBUFFERSPROC)(GLint n, GLuint* textures);
typedef void (*GLDRAWBUFFERPROC)(GLenum buf);
typedef void (*GLREADBUFFERPROC)(GLenum mode);
typedef void (*GLDRAWBUFFERSPROC)(GLsizei n, const GLenum* bufs);
typedef void (*GLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

GLGETERRORPROC                gl_glGetError;
GLGETSHADERINFOLOGPROC        gl_glGetShaderInfoLog;
GLGETPROGRAMINFOLOGPROC       gl_glGetProgramInfoLog;
GLENABLEPROC                  gl_glEnable;
GLDISABLEPROC                 gl_glDisable;
GLCLEARPROC                   gl_glClear;
GLCLEARCOLORPROC              gl_glClearColor;
GLVIEWPORTPROC                gl_glViewport;
GLDRAWARRAYSPROC              gl_glDrawArrays;
GLDRAWELEMENTSPROC            gl_glDrawElements;
GLGENVERTEXARRAYSPROC         gl_glGenVertexArrays;
GLBINDVERTEXARRAYPROC         gl_glBindVertexArray;
GLGENBUFFERSPROC              gl_glGenBuffers;
GLBINDBUFFERPROC              gl_glBindBuffer;
GLBUFFERDATAPROC              gl_glBufferData;
GLBUFFERSUBDATAPROC           gl_glBufferSubData;
GLDELETEBUFFERSPROC           gl_glDeleteBuffers;
GLDELETEVERTEXARRAYSPROC      gl_glDeleteVertexArrays;
GLENABLEVERTEXATTRIBARRAYPROC gl_glEnableVertexAttribArray;
GLVERTEXATTRIBPOINTERPROC     gl_glVertexAttribPointer;
GLVERTEXATTRIBIPOINTERPROC    gl_glVertexAttribIPointer;
GLVERTEXATTRIBDIVISORPROC     gl_glVertexAttribDivisor;
GLGENTEXTURESPROC             gl_glGenTextures;
GLTEXIMAGE2DPROC              gl_glTexImage2D;
GLTEXPARAMETERIPROC           gl_glTexParameteri;
GLTEXPARAMETERFVPROC          gl_glTexParameterfv;
GLACTIVETEXTUREPROC           gl_glActiveTexture;
GLBINDTEXTUREPROC             gl_glBindTexture;
GLDELETETEXTURESPROC          gl_glDeleteTextures;
GLCREATESHADERPROC            gl_glCreateShader;
GLATTACHSHADERPROC            gl_glAttachShader;
GLDELETESHADERPROC            gl_glDeleteShader;
GLSHADERSOURCEPROC            gl_glShaderSource;
GLCOMPILESHADERPROC           gl_glCompileShader;
GLGETSHADERIVPROC             gl_glGetShaderiv;
GLCREATEPROGRAMPROC           gl_glCreateProgram;
GLDELETEPROGRAMPROC           gl_glDeleteProgram;
GLLINKPROGRAMPROC             gl_glLinkProgram;
GLVALIDATEPROGRAMPROC         gl_glValidateProgram;
GLUSEPROGRAMPROC              gl_glUseProgram;
GLGETPROGRAMIVPROC            gl_glGetProgramiv;
GLGETUNIFORMLOCATIONPROC      gl_glGetUniformLocation;
GLUNIFORM1IPROC               gl_glUniform1i;
GLUNIFORM1FPROC               gl_glUniform1f;
GLUNIFORM2FPROC               gl_glUniform2f;
GLUNIFORM3FPROC               gl_glUniform3f;
GLUNIFORM4FPROC               gl_glUniform4f;
GLUNIFORM1FVPROC              gl_glUniform1fv;
GLUNIFORMMATRIX4FVPROC        gl_glUniformMatrix4fv;
GLCULLFACEPROC                gl_glCullFace;
GLBLENDFUNCSEPARATEPROC       gl_glBlendFuncSeparate;
GLBLENDEQUATIONSEPARATEPROC   gl_glBlendEquationSeparate;
GLGENFRAMEBUFFERSPROC         gl_glGenFramebuffers;
GLFRAMEBUFFERTEXTURE2DPROC    gl_glFramebufferTexture2D;
GLCHECKFRAMEBUFFERSTATUSPROC  gl_glCheckFramebufferStatus;
GLBINDFRAMEBUFFERPROC         gl_glBindFramebuffer;
GLDELETEFRAMEBUFFERSPROC      gl_glDeleteFramebuffers;
GLDRAWBUFFERPROC              gl_glDrawBuffer;
GLREADBUFFERPROC              gl_glReadBuffer;
GLDRAWBUFFERSPROC             gl_glDrawBuffers;
GLBLITFRAMEBUFFERPROC         gl_glBlitFramebuffer;

#ifdef DEBUG
#define GL_CALL(fn)                  \
    do {                             \
        errors_clear();              \
        fn;                          \
        errors_check(#fn, __LINE__); \
    } while (0)

#define GL_CALL_RETURN(fn) (errors_clear(), errors_check(#fn, __LINE__), fn)
#else
#define GL_CALL(x)        x;
#define GL_CALL_RETURN(x) (x);
#endif

#define glGetError                     gl_glGetError
#define glGetShaderInfoLog(...)        GL_CALL(gl_glGetShaderInfoLog(__VA_ARGS__))
#define glGetProgramInfoLog(...)       GL_CALL(gl_glGetProgramInfoLog(__VA_ARGS__))
#define glEnable(...)                  GL_CALL(gl_glEnable(__VA_ARGS__))
#define glDisable(...)                 GL_CALL(gl_glDisable(__VA_ARGS__))
#define glClear(...)                   GL_CALL(gl_glClear(__VA_ARGS__))
#define glClearColor(...)              GL_CALL(gl_glClearColor(__VA_ARGS__))
#define glViewport(...)                GL_CALL(gl_glViewport(__VA_ARGS__))
#define glDrawArrays(...)              GL_CALL(gl_glDrawArrays(__VA_ARGS__))
#define glDrawElements(...)            GL_CALL(gl_glDrawElements(__VA_ARGS__))
#define glGenVertexArrays(...)         GL_CALL(gl_glGenVertexArrays(__VA_ARGS__))
#define glBindVertexArray(...)         GL_CALL(gl_glBindVertexArray(__VA_ARGS__))
#define glGenBuffers(...)              GL_CALL(gl_glGenBuffers(__VA_ARGS__))
#define glBindBuffer(...)              GL_CALL(gl_glBindBuffer(__VA_ARGS__))
#define glBufferData(...)              GL_CALL(gl_glBufferData(__VA_ARGS__))
#define glBufferSubData(...)           GL_CALL(gl_glBufferSubData(__VA_ARGS__))
#define glDeleteBuffers(...)           GL_CALL(gl_glDeleteBuffers(__VA_ARGS__))
#define glDeleteVertexArrays(...)      GL_CALL(gl_glDeleteVertexArrays(__VA_ARGS__))
#define glEnableVertexAttribArray(...) GL_CALL(gl_glEnableVertexAttribArray(__VA_ARGS__))
#define glVertexAttribPointer(...)     GL_CALL(gl_glVertexAttribPointer(__VA_ARGS__))
#define glVertexAttribIPointer(...)    GL_CALL(gl_glVertexAttribIPointer(__VA_ARGS__))
#define glVertexAttribDivisor(...)     GL_CALL(gl_glVertexAttribDivisor(__VA_ARGS__))
#define glCreateShader(...)            GL_CALL_RETURN(gl_glCreateShader(__VA_ARGS__))
#define glGenTextures(...)             GL_CALL(gl_glGenTextures(__VA_ARGS__))
#define glTexImage2D(...)              GL_CALL(gl_glTexImage2D(__VA_ARGS__))
#define glTexParameteri(...)           GL_CALL(gl_glTexParameteri(__VA_ARGS__))
#define glTexParameterfv(...)          GL_CALL(gl_glTexParameterfv(__VA_ARGS__))
#define glActiveTexture(...)           GL_CALL(gl_glActiveTexture(__VA_ARGS__))
#define glBindTexture(...)             GL_CALL(gl_glBindTexture(__VA_ARGS__))
#define glDeleteTextures(...)          GL_CALL(gl_glDeleteTextures(__VA_ARGS__))
#define glAttachShader(...)            GL_CALL(gl_glAttachShader(__VA_ARGS__))
#define glDeleteShader(...)            GL_CALL(gl_glDeleteShader(__VA_ARGS__))
#define glShaderSource(...)            GL_CALL(gl_glShaderSource(__VA_ARGS__))
#define glCompileShader(...)           GL_CALL(gl_glCompileShader(__VA_ARGS__))
#define glGetShaderiv(...)             GL_CALL(gl_glGetShaderiv(__VA_ARGS__))
#define glCreateProgram(...)           GL_CALL_RETURN(gl_glCreateProgram(__VA_ARGS__))
#define glDeleteProgram(...)           GL_CALL(gl_glDeleteProgram(__VA_ARGS__))
#define glLinkProgram(...)             GL_CALL(gl_glLinkProgram(__VA_ARGS__))
#define glValidateProgram(...)         GL_CALL(gl_glValidateProgram(__VA_ARGS__))
#define glUseProgram(...)              GL_CALL(gl_glUseProgram(__VA_ARGS__))
#define glGetProgramiv(...)            GL_CALL(gl_glGetProgramiv(__VA_ARGS__))
#define glGetUniformLocation(...)      GL_CALL_RETURN(gl_glGetUniformLocation(__VA_ARGS__))
#define glUniform1i(...)               GL_CALL(gl_glUniform1i(__VA_ARGS__))
#define glUniform1f(...)               GL_CALL(gl_glUniform1f(__VA_ARGS__))
#define glUniform2f(...)               GL_CALL(gl_glUniform2f(__VA_ARGS__))
#define glUniform3f(...)               GL_CALL(gl_glUniform3f(__VA_ARGS__))
#define glUniform4f(...)               GL_CALL(gl_glUniform4f(__VA_ARGS__))
#define glUniform1fv(...)              GL_CALL(gl_glUniform1fv(__VA_ARGS__))
#define glUniformMatrix4fv(...)        GL_CALL(gl_glUniformMatrix4fv(__VA_ARGS__))
#define glCullFace(...)                GL_CALL(gl_glCullFace(__VA_ARGS__))
#define glBlendFuncSeparate(...)       GL_CALL(gl_glBlendFuncSeparate(__VA_ARGS__))
#define glBlendEquationSeparate(...)   GL_CALL(gl_glBlendEquationSeparate(__VA_ARGS__))
#define glGenFramebuffers(...)         GL_CALL(gl_glGenFramebuffers(__VA_ARGS__))
#define glFramebufferTexture2D(...)    GL_CALL(gl_glFramebufferTexture2D(__VA_ARGS__))
#define glCheckFramebufferStatus(...)  GL_CALL(gl_glCheckFramebufferStatus(__VA_ARGS__))
#define glBindFramebuffer(...)         GL_CALL(gl_glBindFramebuffer(__VA_ARGS__))
#define glDeleteFramebuffers(...)      GL_CALL(gl_glDeleteFramebuffers(__VA_ARGS__))
#define glDrawBuffer(...)              GL_CALL(gl_glDrawBuffer(__VA_ARGS__))
#define glReadBuffer(...)              GL_CALL(gl_glReadBuffer(__VA_ARGS__))
#define glDrawBuffers(...)             GL_CALL(gl_glDrawBuffers(__VA_ARGS__))
#define glBlitFramebuffer(...)         GL_CALL(gl_glBlitFramebuffer(__VA_ARGS__))

static void errors_clear(void) { while (glGetError() != GL_NO_ERROR); }

static const char* errors_get_string(GLenum error)
{
    switch (error)
    {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default: return "INVALID ERROR CODE";
    }
}

static bool errors_check(const char* fn, uint32_t line)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s, %s, %d\n", errors_get_string(error), fn, (int)line);
        return false;
    }
    return true;
}

void renderer_bind(void* (*fn)(const char*))
{
    gl_glGetError                = (GLGETERRORPROC)fn("glGetError");
    gl_glGetShaderInfoLog        = (GLGETSHADERINFOLOGPROC)fn("glGetShaderInfoLog");
    gl_glGetProgramInfoLog       = (GLGETPROGRAMINFOLOGPROC)fn("glGetProgramInfoLog");
    gl_glEnable                  = (GLENABLEPROC)fn("glEnable");
    gl_glDisable                 = (GLDISABLEPROC)fn("glDisable");
    gl_glClear                   = (GLCLEARPROC)fn("glClear");
    gl_glClearColor              = (GLCLEARCOLORPROC)fn("glClearColor");
    gl_glViewport                = (GLVIEWPORTPROC)fn("glViewport");
    gl_glDrawArrays              = (GLDRAWARRAYSPROC)fn("glDrawArrays");
    gl_glDrawElements            = (GLDRAWELEMENTSPROC)fn("glDrawElements");
    gl_glGenVertexArrays         = (GLGENVERTEXARRAYSPROC)fn("glGenVertexArrays");
    gl_glBindVertexArray         = (GLBINDVERTEXARRAYPROC)fn("glBindVertexArray");
    gl_glGenBuffers              = (GLGENBUFFERSPROC)fn("glGenBuffers");
    gl_glBindBuffer              = (GLBINDBUFFERPROC)fn("glBindBuffer");
    gl_glBufferData              = (GLBUFFERDATAPROC)fn("glBufferData");
    gl_glBufferSubData           = (GLBUFFERSUBDATAPROC)fn("glBufferSubData");
    gl_glDeleteBuffers           = (GLDELETEBUFFERSPROC)fn("glDeleteBuffers");
    gl_glDeleteVertexArrays      = (GLDELETEVERTEXARRAYSPROC)fn("glDeleteVertexArrays");
    gl_glEnableVertexAttribArray = (GLENABLEVERTEXATTRIBARRAYPROC)fn("glEnableVertexAttribArray");
    gl_glVertexAttribPointer     = (GLVERTEXATTRIBPOINTERPROC)fn("glVertexAttribPointer");
    gl_glVertexAttribIPointer    = (GLVERTEXATTRIBIPOINTERPROC)fn("glVertexAttribIPointer");
    gl_glVertexAttribDivisor     = (GLVERTEXATTRIBDIVISORPROC)fn("glVertexAttribDivisor");
    gl_glGenTextures             = (GLGENTEXTURESPROC)fn("glGenTextures");
    gl_glTexImage2D              = (GLTEXIMAGE2DPROC)fn("glTexImage2D");
    gl_glTexParameteri           = (GLTEXPARAMETERIPROC)fn("glTexParameteri");
    gl_glTexParameterfv          = (GLTEXPARAMETERFVPROC)fn("glTexParameterfv");
    gl_glActiveTexture           = (GLACTIVETEXTUREPROC)fn("glActiveTexture");
    gl_glBindTexture             = (GLBINDTEXTUREPROC)fn("glBindTexture");
    gl_glDeleteTextures          = (GLDELETETEXTURESPROC)fn("glDeleteTextures");
    gl_glCreateShader            = (GLCREATESHADERPROC)fn("glCreateShader");
    gl_glAttachShader            = (GLATTACHSHADERPROC)fn("glAttachShader");
    gl_glDeleteShader            = (GLDELETESHADERPROC)fn("glDeleteShader");
    gl_glShaderSource            = (GLSHADERSOURCEPROC)fn("glShaderSource");
    gl_glCompileShader           = (GLCOMPILESHADERPROC)fn("glCompileShader");
    gl_glGetShaderiv             = (GLGETSHADERIVPROC)fn("glGetShaderiv");
    gl_glCreateProgram           = (GLCREATEPROGRAMPROC)fn("glCreateProgram");
    gl_glDeleteProgram           = (GLDELETEPROGRAMPROC)fn("glDeleteProgram");
    gl_glLinkProgram             = (GLLINKPROGRAMPROC)fn("glLinkProgram");
    gl_glValidateProgram         = (GLVALIDATEPROGRAMPROC)fn("glValidateProgram");
    gl_glUseProgram              = (GLUSEPROGRAMPROC)fn("glUseProgram");
    gl_glGetProgramiv            = (GLGETPROGRAMIVPROC)fn("glGetProgramiv");
    gl_glGetUniformLocation      = (GLGETUNIFORMLOCATIONPROC)fn("glGetUniformLocation");
    gl_glUniform1i               = (GLUNIFORM1IPROC)fn("glUniform1i");
    gl_glUniform1f               = (GLUNIFORM1FPROC)fn("glUniform1f");
    gl_glUniform2f               = (GLUNIFORM2FPROC)fn("glUniform2f");
    gl_glUniform3f               = (GLUNIFORM3FPROC)fn("glUniform3f");
    gl_glUniform4f               = (GLUNIFORM4FPROC)fn("glUniform4f");
    gl_glUniform1fv              = (GLUNIFORM1FVPROC)fn("glUniform1fv");
    gl_glUniformMatrix4fv        = (GLUNIFORMMATRIX4FVPROC)fn("glUniformMatrix4fv");
    gl_glCullFace                = (GLCULLFACEPROC)fn("glCullFace");
    gl_glBlendFuncSeparate       = (GLBLENDFUNCSEPARATEPROC)fn("glBlendFuncSeparate");
    gl_glBlendEquationSeparate   = (GLBLENDEQUATIONSEPARATEPROC)fn("glBlendEquationSeparate");
    gl_glGenFramebuffers         = (GLGENFRAMEBUFFERSPROC)fn("glGenFramebuffers");
    gl_glFramebufferTexture2D    = (GLFRAMEBUFFERTEXTURE2DPROC)fn("glFramebufferTexture2D");
    gl_glCheckFramebufferStatus  = (GLCHECKFRAMEBUFFERSTATUSPROC)fn("glCheckFramebufferStatus");
    gl_glBindFramebuffer         = (GLBINDFRAMEBUFFERPROC)fn("glBindFramebuffer");
    gl_glDeleteFramebuffers      = (GLDELETEFRAMEBUFFERSPROC)fn("glDeleteFramebuffers");
    gl_glDrawBuffer              = (GLDRAWBUFFERPROC)fn("glDrawBuffer");
    gl_glReadBuffer              = (GLREADBUFFERPROC)fn("glReadBuffer");
    gl_glDrawBuffers             = (GLDRAWBUFFERSPROC)fn("glDrawBuffers");
    gl_glBlitFramebuffer         = (GLBLITFRAMEBUFFERPROC)fn("glBlitFramebuffer");
}

void renderer_viewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

void renderer_clear_color(void) { glClear(GL_COLOR_BUFFER_BIT); }

void renderer_clear_color_set(float r, float g, float b) { glClearColor(r, g, b, 1); }

void renderer_enable_cull(CULL_MODE cull)
{
    if (cull == CULL_NONE)
    {
        glDisable(GL_CULL_FACE);
        return;
    }

    glEnable(GL_CULL_FACE);
    switch (cull)
    {
        default:
        case CULL_FRONT: glCullFace(GL_FRONT); break;
        case CULL_BACK: glCullFace(GL_BACK); break;
    }
}

void renderer_enable_blend(BLEND_MODE blend)
{
    GLenum color_equation;
    GLenum color_src;
    GLenum color_dst;
    GLenum alpha_equation;
    GLenum alpha_src;
    GLenum alpha_dst;

    switch (blend)
    {
        default:
        case BLEND_NORMAL:
            color_equation = GL_FUNC_ADD;
            color_src      = GL_ONE;
            color_dst      = GL_ONE_MINUS_SRC_ALPHA;
            alpha_equation = GL_FUNC_ADD;
            alpha_src      = GL_ONE;
            alpha_dst      = GL_ONE_MINUS_SRC_ALPHA;
            break;

        case BLEND_NON_PREMULTIPLIED:
            color_equation = GL_FUNC_ADD;
            color_src      = GL_SRC_ALPHA;
            color_dst      = GL_ONE_MINUS_SRC_ALPHA;
            alpha_equation = GL_FUNC_ADD;
            alpha_src      = GL_SRC_ALPHA;
            alpha_dst      = GL_ONE_MINUS_SRC_ALPHA;
            break;

        case BLEND_SUBTRACT:
            color_equation = GL_FUNC_REVERSE_SUBTRACT;
            color_src      = GL_ONE;
            color_dst      = GL_ONE;
            alpha_equation = GL_FUNC_ADD;
            alpha_src      = GL_ONE;
            alpha_dst      = GL_ONE;
            break;

        case BLEND_ADDITIVE:
            color_equation = GL_FUNC_ADD;
            color_src      = GL_SRC_ALPHA;
            color_dst      = GL_ONE;
            alpha_equation = GL_FUNC_ADD;
            alpha_src      = GL_SRC_ALPHA;
            alpha_dst      = GL_ONE;
            break;
    }

    glEnable(GL_BLEND);
    glBlendEquationSeparate(color_equation, alpha_equation);
    glBlendFuncSeparate(color_src, color_dst, alpha_src, alpha_dst);
}

void renderer_disable_cull(void) { glDisable(GL_CULL_FACE); }

void renderer_disable_blend(void) { glDisable(GL_BLEND); }

static GLenum draw_mode_get_gl_enum(DRAW_MODE mode)
{
    switch (mode)
    {
        case DRAW_LINES: return GL_LINES;
        case DRAW_TRIANGLES: return GL_TRIANGLES;
        case DRAW_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
        case DRAW_QUADS: return GL_QUADS;
    }
}

void renderer_draw_arrays(DRAW_MODE mode, size_t vertices_len)
{
    GLenum gl_type = draw_mode_get_gl_enum(mode);

    glDrawArrays(gl_type, 0, (GLsizei)vertices_len);
}

void renderer_draw_elements(DRAW_MODE mode, size_t indices_len)
{
    GLenum gl_type = draw_mode_get_gl_enum(mode);

    glDrawElements(gl_type, (GLsizei)indices_len, GL_UNSIGNED_INT, NULL);
}

uint32_t renderer_index_buffer_generate_static(void* data, size_t size)
{
    uint32_t id;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return id;
}

uint32_t renderer_index_buffer_generate_dynamic(size_t size)
{
    uint32_t id;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

    return id;
}

uint32_t renderer_vertex_buffer_generate_static(void* data, size_t size)
{
    uint32_t id;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return id;
}

uint32_t renderer_vertex_buffer_generate_dynamic(size_t size)
{
    uint32_t id;

    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

    return id;
}

uint32_t renderer_vertex_array_generate(void)
{
    uint32_t id;

    glGenVertexArrays(1, &id);

    return id;
}

uint32_t renderer_texture_generate(const void* pixels, int width, int height, TEXTURE_FORMAT format)
{
    uint32_t id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    switch (format)
    {
        case TEXTURE_FORMAT_FLOAT: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, pixels); break;
        case TEXTURE_FORMAT_UBYTE: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels); break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return id;
}

static uint32_t shader_compile_source(uint32_t type, const char* source)
{
    uint32_t shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint compiled;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

#ifdef DEBUG
    if (compiled == GL_FALSE)
    {
        GLint len = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

        char* error = (char*)malloc(len);

        glGetShaderInfoLog(shader, len, &len, error);

        printf("[%s] %s", type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER", error);
        free(error);

        glDeleteShader(shader);
    }
#endif

    assert(compiled == GL_TRUE);

    return shader;
}

uint32_t renderer_shader_generate(const char* vertex_shader_source, const char* fragment_shader_source)
{
    uint32_t vertex_shader   = shader_compile_source(GL_VERTEX_SHADER, vertex_shader_source);
    uint32_t fragment_shader = shader_compile_source(GL_FRAGMENT_SHADER, fragment_shader_source);

    GLint vertex_compiled;
    GLint fragment_compiled;

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);

    assert(vertex_compiled == GL_TRUE);
    assert(fragment_compiled == GL_TRUE);

    uint32_t program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    GLint linked;
    GLint validated;

    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validated);

#ifdef DEBUG
    if (linked == GL_FALSE)
    {
        GLint len = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        char* error = (char*)malloc(len);

        glGetProgramInfoLog(program, len, &len, error);

        printf("[SHADER PROGRAM] %s", error);
        free(error);

        glDeleteProgram(program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
#endif

    assert(linked == GL_TRUE);
    // assert(validated == GL_TRUE);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

uint32_t renderer_frame_buffer_generate(void)
{
    uint32_t id;
    glGenFramebuffers(1, &id);
    return id;
}

uint32_t renderer_frame_buffer_attachment_generate(int width, int height, ATTACHMENT_TYPE attachment)
{
    uint32_t id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    assert(attachment != ATTACHMENT_NONE);

    switch (attachment)
    {
        default: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); break;
        case ATTACHMENT_FLOAT: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL); break;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return id;
}

void renderer_index_buffer_bind(uint32_t id) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }

void renderer_index_buffer_unbind(void) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void renderer_index_buffer_delete(uint32_t id) { glDeleteBuffers(1, &id); }

void renderer_vertex_buffer_bind(uint32_t id) { glBindBuffer(GL_ARRAY_BUFFER, id); }

void renderer_vertex_buffer_unbind(void) { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void renderer_vertex_buffer_delete(uint32_t id) { glDeleteBuffers(1, &id); }

void renderer_vertex_array_bind(uint32_t id) { glBindVertexArray(id); }

void renderer_vertex_array_unbind(void) { glBindVertexArray(0); }

void renderer_vertex_array_delete(uint32_t id) { glDeleteVertexArrays(1, &id); }

void renderer_texture_bind(uint32_t id, uint32_t slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void renderer_texture_unbind(void) { glBindTexture(GL_TEXTURE_2D, 0); }

void renderer_texture_delete(uint32_t id) { glDeleteTextures(1, &id); }

void renderer_shader_bind(uint32_t id) { glUseProgram(id); }

void renderer_shader_unbind(void) { glUseProgram(0); }

void renderer_shader_delete(uint32_t id) { glDeleteProgram(id); }

void renderer_frame_buffer_bind(uint32_t id) { glBindFramebuffer(GL_FRAMEBUFFER, id); }

void renderer_frame_buffer_unbind(void) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void renderer_frame_buffer_delete(uint32_t id) { glDeleteFramebuffers(1, &id); }

void renderer_frame_buffer_attachment_bind(uint32_t id, uint32_t texture, ATTACHMENT_TYPE attachment, int slot)
{
    GLenum type;

    if (attachment == ATTACHMENT_NONE)
    {
        return;
    }

    switch (attachment)
    {
        default: type = GL_COLOR_ATTACHMENT0 + slot; break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, texture, 0);
}

void renderer_frame_buffer_attachment_unbind(uint32_t id, ATTACHMENT_TYPE attachment, int slot)
{
    GLenum type;

    if (attachment == ATTACHMENT_NONE)
    {
        return;
    }

    switch (attachment)
    {
        default: type = GL_COLOR_ATTACHMENT0 + slot; break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, 0, 0);
}

static uint32_t attribute_type_get_size(ATTRIBUTE_TYPE type)
{
    switch (type)
    {
        case ATTRIBUTE_BYTE: return 1;
        case ATTRIBUTE_BOOL:
        case ATTRIBUTE_INT:
        case ATTRIBUTE_FLOAT: return 4;
        case ATTRIBUTE_UVEC2:
        case ATTRIBUTE_IVEC2:
        case ATTRIBUTE_VEC2: return 4 * 2;
        case ATTRIBUTE_UVEC3:
        case ATTRIBUTE_IVEC3:
        case ATTRIBUTE_VEC3: return 4 * 3;
        case ATTRIBUTE_UVEC4:
        case ATTRIBUTE_IVEC4:
        case ATTRIBUTE_VEC4: return 4 * 4;
        case ATTRIBUTE_MAT4: return 4 * 4 * 4;
    }

    return 0;
}

static uint32_t attribute_type_get_count(ATTRIBUTE_TYPE type)
{
    switch (type)
    {
        case ATTRIBUTE_BYTE:
        case ATTRIBUTE_BOOL:
        case ATTRIBUTE_INT:
        case ATTRIBUTE_FLOAT: return 1;
        case ATTRIBUTE_UVEC2:
        case ATTRIBUTE_IVEC2:
        case ATTRIBUTE_VEC2: return 2;
        case ATTRIBUTE_UVEC3:
        case ATTRIBUTE_IVEC3:
        case ATTRIBUTE_VEC3: return 3;
        case ATTRIBUTE_UVEC4:
        case ATTRIBUTE_IVEC4:
        case ATTRIBUTE_VEC4: return 4;
        case ATTRIBUTE_MAT4: return 4 * 4;
    }

    return 0;
}

static GLenum attribute_type_get_gl_enum(ATTRIBUTE_TYPE type)
{
    switch (type)
    {
        case ATTRIBUTE_BYTE: return GL_UNSIGNED_BYTE;
        case ATTRIBUTE_BOOL: return GL_BOOL;
        case ATTRIBUTE_FLOAT:
        case ATTRIBUTE_VEC2:
        case ATTRIBUTE_VEC3:
        case ATTRIBUTE_VEC4:
        case ATTRIBUTE_MAT4: return GL_FLOAT;
        case ATTRIBUTE_INT:
        case ATTRIBUTE_IVEC2:
        case ATTRIBUTE_IVEC3:
        case ATTRIBUTE_IVEC4: return GL_INT;
        case ATTRIBUTE_UVEC2:
        case ATTRIBUTE_UVEC3:
        case ATTRIBUTE_UVEC4: return GL_UNSIGNED_INT;
    }

    return 0;
}

static uint32_t buffer_layout_get_stride(ATTRIBUTE_TYPE* layout, size_t layout_len)
{
    uint32_t stride = 0U;

    for (int i = 0; i < layout_len; ++i)
    {
        ATTRIBUTE_TYPE attribute = layout[i];
        stride += attribute_type_get_size(attribute);
    }

    return stride;
}

void renderer_vertex_array_add_buffer(uint32_t id, uint32_t vertex_buffer_id, size_t layout_len, ATTRIBUTE_TYPE* layout)
{
    renderer_vertex_array_bind(id);
    renderer_vertex_buffer_bind(vertex_buffer_id);

    uint32_t  stride = buffer_layout_get_stride(layout, layout_len);
    uintptr_t offset = 0x0;

    size_t index = 0u;

    for (int i = 0; i < layout_len; ++i)
    {
        ATTRIBUTE_TYPE type = layout[i];

        bool is_integer = attribute_type_get_gl_enum(type) != GL_FLOAT;

        if (type == ATTRIBUTE_MAT4)
        {
            for (int j = 0; j < 4; ++j)
            {
                glEnableVertexAttribArray(index);

                if (is_integer)
                {
                    glVertexAttribIPointer(index, 4, GL_FLOAT, stride, (const void*)offset);
                }
                else
                {
                    glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
                }

                glVertexAttribDivisor(index, 0);

                offset += sizeof(float) * 4;
                ++index;
            }
        }
        else
        {
            glEnableVertexAttribArray(index);

            if (is_integer)
            {
                glVertexAttribIPointer(index, attribute_type_get_count(type), attribute_type_get_gl_enum(type), stride, (const void*)offset);
            }
            else
            {
                glVertexAttribPointer(index, attribute_type_get_count(type), attribute_type_get_gl_enum(type), GL_FALSE, stride, (const void*)offset);
            }

            glVertexAttribDivisor(index, 0);

            offset += attribute_type_get_size(type);
            ++index;
        }
    }
}

void renderer_vertex_buffer_subdata(void* data, size_t size) { glBufferSubData(GL_ARRAY_BUFFER, 0, size, data); }

void renderer_index_buffer_subdata(void* data, size_t size) { glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data); }

void renderer_texture_set_wrap(TEXTURE_WRAP wrap)
{
    switch (wrap)
    {
        default:
        case TEXTURE_WRAP_CLAMP:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;

        case TEXTURE_WRAP_REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;

        case TEXTURE_WRAP_MIRROR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;

        case TEXTURE_WRAP_BORDER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            break;
    }
}

void renderer_texture_set_filter(TEXTURE_FILTER filter)
{
    switch (filter)
    {
        default:
        case TEXTURE_FILTER_NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case TEXTURE_FILTER_LINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
    }
}

int renderer_shader_get_uniform_location(uint32_t id, const char* name) { return glGetUniformLocation(id, name); }

void renderer_shader_set_uniformi(int location, int value) { glUniform1i(location, value); }

void renderer_shader_set_uniformf(int location, float value) { glUniform1f(location, value); }

void renderer_shader_set_uniform2f(int location, float* value) { glUniform2f(location, value[0], value[1]); }

void renderer_shader_set_uniform3f(int location, float* value) { glUniform3f(location, value[0], value[1], value[2]); }

void renderer_shader_set_uniform4f(int location, float* value) { glUniform4f(location, value[0], value[1], value[2], value[3]); }

void renderer_shader_set_uniformfv(int location, size_t len, float* value) { glUniform1fv(location, len, value); }

void renderer_shader_set_uniform4x4f(int location, float* value) { glUniformMatrix4fv(location, 1, GL_FALSE, value); }

void renderer_frame_buffer_copy(uint32_t from, uint32_t to, int src[4], int dst[4], ATTACHMENT_TYPE attachments)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, from);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to);

    GLenum type = 0u;

    bool has_color = attachments & (ATTACHMENT_COLOR);

    if (has_color) type |= GL_COLOR_BUFFER_BIT;

    glBlitFramebuffer(src[0], src[1], src[2], src[3], dst[0], dst[1], dst[2], dst[3], type, GL_NEAREST);
}

void renderer_frame_buffer_draw(size_t buffers_len)
{
    GLenum buffers[16];

    for (int i = 0; i < buffers_len; ++i)
    {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(buffers_len, &buffers[0]);
}

void renderer_frame_buffer_no_draw(void)
{
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void renderer_frame_buffer_attachment_depth_compare(void) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE); }
