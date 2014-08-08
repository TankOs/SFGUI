#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <SFGUI/GLLoader.hpp>

#if defined(__APPLE__)
#include <dlfcn.h>

static void* AppleGLGetProcAddress (const char *name)
{
	static void* image = NULL;

	if (NULL == image)
		image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);

	return (image ? dlsym(image, name) : NULL);
}
#define IntGetProcAddress(name) AppleGLGetProcAddress(name)
#endif /* __APPLE__ */

#if defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#pragma warning(disable: 4996)
#endif

static PROC WinGetProcAddress(const char *name)
{
	static HMODULE glMod = NULL;
	PROC pFunc = wglGetProcAddress((LPCSTR)name);

	if (pFunc) return pFunc;

	if (NULL == glMod)
		glMod = GetModuleHandleA("OpenGL32.dll");

	return (PROC)GetProcAddress(glMod, (LPCSTR)name);
}

#define IntGetProcAddress(name) WinGetProcAddress(name)
#endif

/* Linux, FreeBSD, other */
#ifndef IntGetProcAddress
	#include <GL/glx.h>

	#define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
#endif

int sfgogl_ext_SGIS_texture_edge_clamp = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_multitexture = sfgogl_LOAD_FAILED;
int sfgogl_ext_EXT_blend_minmax = sfgogl_LOAD_FAILED;
int sfgogl_ext_EXT_blend_subtract = sfgogl_LOAD_FAILED;
int sfgogl_ext_EXT_blend_func_separate = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_vertex_buffer_object = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_vertex_program = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_fragment_program = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_shading_language_100 = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_shader_objects = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_vertex_shader = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_fragment_shader = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_texture_non_power_of_two = sfgogl_LOAD_FAILED;
int sfgogl_ext_EXT_blend_equation_separate = sfgogl_LOAD_FAILED;
int sfgogl_ext_EXT_framebuffer_object = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_vertex_array_object = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_geometry_shader4 = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_explicit_attrib_location = sfgogl_LOAD_FAILED;
int sfgogl_ext_ARB_explicit_uniform_location = sfgogl_LOAD_FAILED;

void (CODEGEN_FUNCPTR *sfg_ptrc_glActiveTextureARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClientActiveTextureARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1dARB)(GLenum, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1fARB)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1iARB)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1sARB)(GLenum, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord1svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2dARB)(GLenum, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2fARB)(GLenum, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2iARB)(GLenum, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2sARB)(GLenum, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord2svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3dARB)(GLenum, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3fARB)(GLenum, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3iARB)(GLenum, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3sARB)(GLenum, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord3svARB)(GLenum, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4dARB)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4dvARB)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4fARB)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4fvARB)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4iARB)(GLenum, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4ivARB)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4sARB)(GLenum, GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultiTexCoord4svARB)(GLenum, const GLshort *) = NULL;

static int Load_ARB_multitexture(void)
{
	int numFailed = 0;
	sfg_ptrc_glActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glActiveTextureARB");
	if(!sfg_ptrc_glActiveTextureARB) numFailed++;
	sfg_ptrc_glClientActiveTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glClientActiveTextureARB");
	if(!sfg_ptrc_glClientActiveTextureARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble))IntGetProcAddress("glMultiTexCoord1dARB");
	if(!sfg_ptrc_glMultiTexCoord1dARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord1dvARB");
	if(!sfg_ptrc_glMultiTexCoord1dvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glMultiTexCoord1fARB");
	if(!sfg_ptrc_glMultiTexCoord1fARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord1fvARB");
	if(!sfg_ptrc_glMultiTexCoord1fvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glMultiTexCoord1iARB");
	if(!sfg_ptrc_glMultiTexCoord1iARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord1ivARB");
	if(!sfg_ptrc_glMultiTexCoord1ivARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort))IntGetProcAddress("glMultiTexCoord1sARB");
	if(!sfg_ptrc_glMultiTexCoord1sARB) numFailed++;
	sfg_ptrc_glMultiTexCoord1svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord1svARB");
	if(!sfg_ptrc_glMultiTexCoord1svARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord2dARB");
	if(!sfg_ptrc_glMultiTexCoord2dARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord2dvARB");
	if(!sfg_ptrc_glMultiTexCoord2dvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord2fARB");
	if(!sfg_ptrc_glMultiTexCoord2fARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord2fvARB");
	if(!sfg_ptrc_glMultiTexCoord2fvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint))IntGetProcAddress("glMultiTexCoord2iARB");
	if(!sfg_ptrc_glMultiTexCoord2iARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord2ivARB");
	if(!sfg_ptrc_glMultiTexCoord2ivARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord2sARB");
	if(!sfg_ptrc_glMultiTexCoord2sARB) numFailed++;
	sfg_ptrc_glMultiTexCoord2svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord2svARB");
	if(!sfg_ptrc_glMultiTexCoord2svARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord3dARB");
	if(!sfg_ptrc_glMultiTexCoord3dARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord3dvARB");
	if(!sfg_ptrc_glMultiTexCoord3dvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord3fARB");
	if(!sfg_ptrc_glMultiTexCoord3fARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord3fvARB");
	if(!sfg_ptrc_glMultiTexCoord3fvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint))IntGetProcAddress("glMultiTexCoord3iARB");
	if(!sfg_ptrc_glMultiTexCoord3iARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord3ivARB");
	if(!sfg_ptrc_glMultiTexCoord3ivARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord3sARB");
	if(!sfg_ptrc_glMultiTexCoord3sARB) numFailed++;
	sfg_ptrc_glMultiTexCoord3svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord3svARB");
	if(!sfg_ptrc_glMultiTexCoord3svARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glMultiTexCoord4dARB");
	if(!sfg_ptrc_glMultiTexCoord4dARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glMultiTexCoord4dvARB");
	if(!sfg_ptrc_glMultiTexCoord4dvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glMultiTexCoord4fARB");
	if(!sfg_ptrc_glMultiTexCoord4fARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glMultiTexCoord4fvARB");
	if(!sfg_ptrc_glMultiTexCoord4fvARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4iARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint))IntGetProcAddress("glMultiTexCoord4iARB");
	if(!sfg_ptrc_glMultiTexCoord4iARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4ivARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glMultiTexCoord4ivARB");
	if(!sfg_ptrc_glMultiTexCoord4ivARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4sARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glMultiTexCoord4sARB");
	if(!sfg_ptrc_glMultiTexCoord4sARB) numFailed++;
	sfg_ptrc_glMultiTexCoord4svARB = (void (CODEGEN_FUNCPTR *)(GLenum, const GLshort *))IntGetProcAddress("glMultiTexCoord4svARB");
	if(!sfg_ptrc_glMultiTexCoord4svARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendEquationEXT)(GLenum) = NULL;

static int Load_EXT_blend_minmax(void)
{
	int numFailed = 0;
	sfg_ptrc_glBlendEquationEXT = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBlendEquationEXT");
	if(!sfg_ptrc_glBlendEquationEXT) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendFuncSeparateEXT)(GLenum, GLenum, GLenum, GLenum) = NULL;

static int Load_EXT_blend_func_separate(void)
{
	int numFailed = 0;
	sfg_ptrc_glBlendFuncSeparateEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLenum))IntGetProcAddress("glBlendFuncSeparateEXT");
	if(!sfg_ptrc_glBlendFuncSeparateEXT) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBindBufferARB)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBufferDataARB)(GLenum, GLsizeiptrARB, const GLvoid *, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBufferSubDataARB)(GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteBuffersARB)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenBuffersARB)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetBufferParameterivARB)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetBufferPointervARB)(GLenum, GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetBufferSubDataARB)(GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsBufferARB)(GLuint) = NULL;
void * (CODEGEN_FUNCPTR *sfg_ptrc_glMapBufferARB)(GLenum, GLenum) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glUnmapBufferARB)(GLenum) = NULL;

static int Load_ARB_vertex_buffer_object(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindBufferARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindBufferARB");
	if(!sfg_ptrc_glBindBufferARB) numFailed++;
	sfg_ptrc_glBufferDataARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizeiptrARB, const GLvoid *, GLenum))IntGetProcAddress("glBufferDataARB");
	if(!sfg_ptrc_glBufferDataARB) numFailed++;
	sfg_ptrc_glBufferSubDataARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLintptrARB, GLsizeiptrARB, const GLvoid *))IntGetProcAddress("glBufferSubDataARB");
	if(!sfg_ptrc_glBufferSubDataARB) numFailed++;
	sfg_ptrc_glDeleteBuffersARB = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteBuffersARB");
	if(!sfg_ptrc_glDeleteBuffersARB) numFailed++;
	sfg_ptrc_glGenBuffersARB = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenBuffersARB");
	if(!sfg_ptrc_glGenBuffersARB) numFailed++;
	sfg_ptrc_glGetBufferParameterivARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetBufferParameterivARB");
	if(!sfg_ptrc_glGetBufferParameterivARB) numFailed++;
	sfg_ptrc_glGetBufferPointervARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLvoid **))IntGetProcAddress("glGetBufferPointervARB");
	if(!sfg_ptrc_glGetBufferPointervARB) numFailed++;
	sfg_ptrc_glGetBufferSubDataARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLintptrARB, GLsizeiptrARB, GLvoid *))IntGetProcAddress("glGetBufferSubDataARB");
	if(!sfg_ptrc_glGetBufferSubDataARB) numFailed++;
	sfg_ptrc_glIsBufferARB = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsBufferARB");
	if(!sfg_ptrc_glIsBufferARB) numFailed++;
	sfg_ptrc_glMapBufferARB = (void * (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glMapBufferARB");
	if(!sfg_ptrc_glMapBufferARB) numFailed++;
	sfg_ptrc_glUnmapBufferARB = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glUnmapBufferARB");
	if(!sfg_ptrc_glUnmapBufferARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBindProgramARB)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteProgramsARB)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDisableVertexAttribArrayARB)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEnableVertexAttribArrayARB)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenProgramsARB)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramEnvParameterdvARB)(GLenum, GLuint, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramEnvParameterfvARB)(GLenum, GLuint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramLocalParameterdvARB)(GLenum, GLuint, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramLocalParameterfvARB)(GLenum, GLuint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramStringARB)(GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetProgramivARB)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetVertexAttribPointervARB)(GLuint, GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetVertexAttribdvARB)(GLuint, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetVertexAttribfvARB)(GLuint, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetVertexAttribivARB)(GLuint, GLenum, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsProgramARB)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramEnvParameter4dARB)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramEnvParameter4dvARB)(GLenum, GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramEnvParameter4fARB)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramEnvParameter4fvARB)(GLenum, GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramLocalParameter4dARB)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramLocalParameter4dvARB)(GLenum, GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramLocalParameter4fARB)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramLocalParameter4fvARB)(GLenum, GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramStringARB)(GLenum, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1dARB)(GLuint, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1dvARB)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1fARB)(GLuint, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1fvARB)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1sARB)(GLuint, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib1svARB)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2dARB)(GLuint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2dvARB)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2fARB)(GLuint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2fvARB)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2sARB)(GLuint, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib2svARB)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3dARB)(GLuint, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3dvARB)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3fARB)(GLuint, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3fvARB)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3sARB)(GLuint, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib3svARB)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NbvARB)(GLuint, const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NivARB)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NsvARB)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NubARB)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NubvARB)(GLuint, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NuivARB)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4NusvARB)(GLuint, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4bvARB)(GLuint, const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4dARB)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4dvARB)(GLuint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4fARB)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4fvARB)(GLuint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4ivARB)(GLuint, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4sARB)(GLuint, GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4svARB)(GLuint, const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4ubvARB)(GLuint, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4uivARB)(GLuint, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttrib4usvARB)(GLuint, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexAttribPointerARB)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *) = NULL;

static int Load_ARB_vertex_program(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindProgramARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindProgramARB");
	if(!sfg_ptrc_glBindProgramARB) numFailed++;
	sfg_ptrc_glDeleteProgramsARB = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteProgramsARB");
	if(!sfg_ptrc_glDeleteProgramsARB) numFailed++;
	sfg_ptrc_glDisableVertexAttribArrayARB = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glDisableVertexAttribArrayARB");
	if(!sfg_ptrc_glDisableVertexAttribArrayARB) numFailed++;
	sfg_ptrc_glEnableVertexAttribArrayARB = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glEnableVertexAttribArrayARB");
	if(!sfg_ptrc_glEnableVertexAttribArrayARB) numFailed++;
	sfg_ptrc_glGenProgramsARB = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenProgramsARB");
	if(!sfg_ptrc_glGenProgramsARB) numFailed++;
	sfg_ptrc_glGetProgramEnvParameterdvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble *))IntGetProcAddress("glGetProgramEnvParameterdvARB");
	if(!sfg_ptrc_glGetProgramEnvParameterdvARB) numFailed++;
	sfg_ptrc_glGetProgramEnvParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat *))IntGetProcAddress("glGetProgramEnvParameterfvARB");
	if(!sfg_ptrc_glGetProgramEnvParameterfvARB) numFailed++;
	sfg_ptrc_glGetProgramLocalParameterdvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble *))IntGetProcAddress("glGetProgramLocalParameterdvARB");
	if(!sfg_ptrc_glGetProgramLocalParameterdvARB) numFailed++;
	sfg_ptrc_glGetProgramLocalParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat *))IntGetProcAddress("glGetProgramLocalParameterfvARB");
	if(!sfg_ptrc_glGetProgramLocalParameterfvARB) numFailed++;
	sfg_ptrc_glGetProgramStringARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLvoid *))IntGetProcAddress("glGetProgramStringARB");
	if(!sfg_ptrc_glGetProgramStringARB) numFailed++;
	sfg_ptrc_glGetProgramivARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetProgramivARB");
	if(!sfg_ptrc_glGetProgramivARB) numFailed++;
	sfg_ptrc_glGetVertexAttribPointervARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLvoid **))IntGetProcAddress("glGetVertexAttribPointervARB");
	if(!sfg_ptrc_glGetVertexAttribPointervARB) numFailed++;
	sfg_ptrc_glGetVertexAttribdvARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLdouble *))IntGetProcAddress("glGetVertexAttribdvARB");
	if(!sfg_ptrc_glGetVertexAttribdvARB) numFailed++;
	sfg_ptrc_glGetVertexAttribfvARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLfloat *))IntGetProcAddress("glGetVertexAttribfvARB");
	if(!sfg_ptrc_glGetVertexAttribfvARB) numFailed++;
	sfg_ptrc_glGetVertexAttribivARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint *))IntGetProcAddress("glGetVertexAttribivARB");
	if(!sfg_ptrc_glGetVertexAttribivARB) numFailed++;
	sfg_ptrc_glIsProgramARB = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsProgramARB");
	if(!sfg_ptrc_glIsProgramARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glProgramEnvParameter4dARB");
	if(!sfg_ptrc_glProgramEnvParameter4dARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLdouble *))IntGetProcAddress("glProgramEnvParameter4dvARB");
	if(!sfg_ptrc_glProgramEnvParameter4dvARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glProgramEnvParameter4fARB");
	if(!sfg_ptrc_glProgramEnvParameter4fARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLfloat *))IntGetProcAddress("glProgramEnvParameter4fvARB");
	if(!sfg_ptrc_glProgramEnvParameter4fvARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glProgramLocalParameter4dARB");
	if(!sfg_ptrc_glProgramLocalParameter4dARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLdouble *))IntGetProcAddress("glProgramLocalParameter4dvARB");
	if(!sfg_ptrc_glProgramLocalParameter4dvARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glProgramLocalParameter4fARB");
	if(!sfg_ptrc_glProgramLocalParameter4fARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLfloat *))IntGetProcAddress("glProgramLocalParameter4fvARB");
	if(!sfg_ptrc_glProgramLocalParameter4fvARB) numFailed++;
	sfg_ptrc_glProgramStringARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glProgramStringARB");
	if(!sfg_ptrc_glProgramStringARB) numFailed++;
	sfg_ptrc_glVertexAttrib1dARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble))IntGetProcAddress("glVertexAttrib1dARB");
	if(!sfg_ptrc_glVertexAttrib1dARB) numFailed++;
	sfg_ptrc_glVertexAttrib1dvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib1dvARB");
	if(!sfg_ptrc_glVertexAttrib1dvARB) numFailed++;
	sfg_ptrc_glVertexAttrib1fARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat))IntGetProcAddress("glVertexAttrib1fARB");
	if(!sfg_ptrc_glVertexAttrib1fARB) numFailed++;
	sfg_ptrc_glVertexAttrib1fvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib1fvARB");
	if(!sfg_ptrc_glVertexAttrib1fvARB) numFailed++;
	sfg_ptrc_glVertexAttrib1sARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort))IntGetProcAddress("glVertexAttrib1sARB");
	if(!sfg_ptrc_glVertexAttrib1sARB) numFailed++;
	sfg_ptrc_glVertexAttrib1svARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib1svARB");
	if(!sfg_ptrc_glVertexAttrib1svARB) numFailed++;
	sfg_ptrc_glVertexAttrib2dARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib2dARB");
	if(!sfg_ptrc_glVertexAttrib2dARB) numFailed++;
	sfg_ptrc_glVertexAttrib2dvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib2dvARB");
	if(!sfg_ptrc_glVertexAttrib2dvARB) numFailed++;
	sfg_ptrc_glVertexAttrib2fARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib2fARB");
	if(!sfg_ptrc_glVertexAttrib2fARB) numFailed++;
	sfg_ptrc_glVertexAttrib2fvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib2fvARB");
	if(!sfg_ptrc_glVertexAttrib2fvARB) numFailed++;
	sfg_ptrc_glVertexAttrib2sARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort))IntGetProcAddress("glVertexAttrib2sARB");
	if(!sfg_ptrc_glVertexAttrib2sARB) numFailed++;
	sfg_ptrc_glVertexAttrib2svARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib2svARB");
	if(!sfg_ptrc_glVertexAttrib2svARB) numFailed++;
	sfg_ptrc_glVertexAttrib3dARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib3dARB");
	if(!sfg_ptrc_glVertexAttrib3dARB) numFailed++;
	sfg_ptrc_glVertexAttrib3dvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib3dvARB");
	if(!sfg_ptrc_glVertexAttrib3dvARB) numFailed++;
	sfg_ptrc_glVertexAttrib3fARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib3fARB");
	if(!sfg_ptrc_glVertexAttrib3fARB) numFailed++;
	sfg_ptrc_glVertexAttrib3fvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib3fvARB");
	if(!sfg_ptrc_glVertexAttrib3fvARB) numFailed++;
	sfg_ptrc_glVertexAttrib3sARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort, GLshort))IntGetProcAddress("glVertexAttrib3sARB");
	if(!sfg_ptrc_glVertexAttrib3sARB) numFailed++;
	sfg_ptrc_glVertexAttrib3svARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib3svARB");
	if(!sfg_ptrc_glVertexAttrib3svARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NbvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLbyte *))IntGetProcAddress("glVertexAttrib4NbvARB");
	if(!sfg_ptrc_glVertexAttrib4NbvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NivARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttrib4NivARB");
	if(!sfg_ptrc_glVertexAttrib4NivARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NsvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib4NsvARB");
	if(!sfg_ptrc_glVertexAttrib4NsvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NubARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte))IntGetProcAddress("glVertexAttrib4NubARB");
	if(!sfg_ptrc_glVertexAttrib4NubARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NubvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLubyte *))IntGetProcAddress("glVertexAttrib4NubvARB");
	if(!sfg_ptrc_glVertexAttrib4NubvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NuivARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttrib4NuivARB");
	if(!sfg_ptrc_glVertexAttrib4NuivARB) numFailed++;
	sfg_ptrc_glVertexAttrib4NusvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLushort *))IntGetProcAddress("glVertexAttrib4NusvARB");
	if(!sfg_ptrc_glVertexAttrib4NusvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4bvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLbyte *))IntGetProcAddress("glVertexAttrib4bvARB");
	if(!sfg_ptrc_glVertexAttrib4bvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4dARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertexAttrib4dARB");
	if(!sfg_ptrc_glVertexAttrib4dARB) numFailed++;
	sfg_ptrc_glVertexAttrib4dvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLdouble *))IntGetProcAddress("glVertexAttrib4dvARB");
	if(!sfg_ptrc_glVertexAttrib4dvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4fARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertexAttrib4fARB");
	if(!sfg_ptrc_glVertexAttrib4fARB) numFailed++;
	sfg_ptrc_glVertexAttrib4fvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLfloat *))IntGetProcAddress("glVertexAttrib4fvARB");
	if(!sfg_ptrc_glVertexAttrib4fvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4ivARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLint *))IntGetProcAddress("glVertexAttrib4ivARB");
	if(!sfg_ptrc_glVertexAttrib4ivARB) numFailed++;
	sfg_ptrc_glVertexAttrib4sARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glVertexAttrib4sARB");
	if(!sfg_ptrc_glVertexAttrib4sARB) numFailed++;
	sfg_ptrc_glVertexAttrib4svARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLshort *))IntGetProcAddress("glVertexAttrib4svARB");
	if(!sfg_ptrc_glVertexAttrib4svARB) numFailed++;
	sfg_ptrc_glVertexAttrib4ubvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLubyte *))IntGetProcAddress("glVertexAttrib4ubvARB");
	if(!sfg_ptrc_glVertexAttrib4ubvARB) numFailed++;
	sfg_ptrc_glVertexAttrib4uivARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLuint *))IntGetProcAddress("glVertexAttrib4uivARB");
	if(!sfg_ptrc_glVertexAttrib4uivARB) numFailed++;
	sfg_ptrc_glVertexAttrib4usvARB = (void (CODEGEN_FUNCPTR *)(GLuint, const GLushort *))IntGetProcAddress("glVertexAttrib4usvARB");
	if(!sfg_ptrc_glVertexAttrib4usvARB) numFailed++;
	sfg_ptrc_glVertexAttribPointerARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *))IntGetProcAddress("glVertexAttribPointerARB");
	if(!sfg_ptrc_glVertexAttribPointerARB) numFailed++;
	return numFailed;
}


static int Load_ARB_fragment_program(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindProgramARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindProgramARB");
	if(!sfg_ptrc_glBindProgramARB) numFailed++;
	sfg_ptrc_glDeleteProgramsARB = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteProgramsARB");
	if(!sfg_ptrc_glDeleteProgramsARB) numFailed++;
	sfg_ptrc_glGenProgramsARB = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenProgramsARB");
	if(!sfg_ptrc_glGenProgramsARB) numFailed++;
	sfg_ptrc_glGetProgramEnvParameterdvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble *))IntGetProcAddress("glGetProgramEnvParameterdvARB");
	if(!sfg_ptrc_glGetProgramEnvParameterdvARB) numFailed++;
	sfg_ptrc_glGetProgramEnvParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat *))IntGetProcAddress("glGetProgramEnvParameterfvARB");
	if(!sfg_ptrc_glGetProgramEnvParameterfvARB) numFailed++;
	sfg_ptrc_glGetProgramLocalParameterdvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble *))IntGetProcAddress("glGetProgramLocalParameterdvARB");
	if(!sfg_ptrc_glGetProgramLocalParameterdvARB) numFailed++;
	sfg_ptrc_glGetProgramLocalParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat *))IntGetProcAddress("glGetProgramLocalParameterfvARB");
	if(!sfg_ptrc_glGetProgramLocalParameterfvARB) numFailed++;
	sfg_ptrc_glGetProgramStringARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLvoid *))IntGetProcAddress("glGetProgramStringARB");
	if(!sfg_ptrc_glGetProgramStringARB) numFailed++;
	sfg_ptrc_glGetProgramivARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetProgramivARB");
	if(!sfg_ptrc_glGetProgramivARB) numFailed++;
	sfg_ptrc_glIsProgramARB = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsProgramARB");
	if(!sfg_ptrc_glIsProgramARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glProgramEnvParameter4dARB");
	if(!sfg_ptrc_glProgramEnvParameter4dARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLdouble *))IntGetProcAddress("glProgramEnvParameter4dvARB");
	if(!sfg_ptrc_glProgramEnvParameter4dvARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glProgramEnvParameter4fARB");
	if(!sfg_ptrc_glProgramEnvParameter4fARB) numFailed++;
	sfg_ptrc_glProgramEnvParameter4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLfloat *))IntGetProcAddress("glProgramEnvParameter4fvARB");
	if(!sfg_ptrc_glProgramEnvParameter4fvARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4dARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glProgramLocalParameter4dARB");
	if(!sfg_ptrc_glProgramLocalParameter4dARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4dvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLdouble *))IntGetProcAddress("glProgramLocalParameter4dvARB");
	if(!sfg_ptrc_glProgramLocalParameter4dvARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4fARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glProgramLocalParameter4fARB");
	if(!sfg_ptrc_glProgramLocalParameter4fARB) numFailed++;
	sfg_ptrc_glProgramLocalParameter4fvARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, const GLfloat *))IntGetProcAddress("glProgramLocalParameter4fvARB");
	if(!sfg_ptrc_glProgramLocalParameter4fvARB) numFailed++;
	sfg_ptrc_glProgramStringARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glProgramStringARB");
	if(!sfg_ptrc_glProgramStringARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glAttachObjectARB)(GLhandleARB, GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCompileShaderARB)(GLhandleARB) = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sfg_ptrc_glCreateProgramObjectARB)(void) = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sfg_ptrc_glCreateShaderObjectARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteObjectARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDetachObjectARB)(GLhandleARB, GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetActiveUniformARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetAttachedObjectsARB)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *) = NULL;
GLhandleARB (CODEGEN_FUNCPTR *sfg_ptrc_glGetHandleARB)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetInfoLogARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetObjectParameterfvARB)(GLhandleARB, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetObjectParameterivARB)(GLhandleARB, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetShaderSourceARB)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *) = NULL;
GLint (CODEGEN_FUNCPTR *sfg_ptrc_glGetUniformLocationARB)(GLhandleARB, const GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetUniformfvARB)(GLhandleARB, GLint, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetUniformivARB)(GLhandleARB, GLint, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLinkProgramARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glShaderSourceARB)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform1fARB)(GLint, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform1fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform1iARB)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform1ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform2fARB)(GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform2fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform2iARB)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform2ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform3fARB)(GLint, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform3fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform3iARB)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform3ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform4fARB)(GLint, GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform4fvARB)(GLint, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform4iARB)(GLint, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniform4ivARB)(GLint, GLsizei, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniformMatrix2fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniformMatrix3fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUniformMatrix4fvARB)(GLint, GLsizei, GLboolean, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glUseProgramObjectARB)(GLhandleARB) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glValidateProgramARB)(GLhandleARB) = NULL;

static int Load_ARB_shader_objects(void)
{
	int numFailed = 0;
	sfg_ptrc_glAttachObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLhandleARB))IntGetProcAddress("glAttachObjectARB");
	if(!sfg_ptrc_glAttachObjectARB) numFailed++;
	sfg_ptrc_glCompileShaderARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glCompileShaderARB");
	if(!sfg_ptrc_glCompileShaderARB) numFailed++;
	sfg_ptrc_glCreateProgramObjectARB = (GLhandleARB (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glCreateProgramObjectARB");
	if(!sfg_ptrc_glCreateProgramObjectARB) numFailed++;
	sfg_ptrc_glCreateShaderObjectARB = (GLhandleARB (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCreateShaderObjectARB");
	if(!sfg_ptrc_glCreateShaderObjectARB) numFailed++;
	sfg_ptrc_glDeleteObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glDeleteObjectARB");
	if(!sfg_ptrc_glDeleteObjectARB) numFailed++;
	sfg_ptrc_glDetachObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLhandleARB))IntGetProcAddress("glDetachObjectARB");
	if(!sfg_ptrc_glDetachObjectARB) numFailed++;
	sfg_ptrc_glGetActiveUniformARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *))IntGetProcAddress("glGetActiveUniformARB");
	if(!sfg_ptrc_glGetActiveUniformARB) numFailed++;
	sfg_ptrc_glGetAttachedObjectsARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLhandleARB *))IntGetProcAddress("glGetAttachedObjectsARB");
	if(!sfg_ptrc_glGetAttachedObjectsARB) numFailed++;
	sfg_ptrc_glGetHandleARB = (GLhandleARB (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetHandleARB");
	if(!sfg_ptrc_glGetHandleARB) numFailed++;
	sfg_ptrc_glGetInfoLogARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *))IntGetProcAddress("glGetInfoLogARB");
	if(!sfg_ptrc_glGetInfoLogARB) numFailed++;
	sfg_ptrc_glGetObjectParameterfvARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLenum, GLfloat *))IntGetProcAddress("glGetObjectParameterfvARB");
	if(!sfg_ptrc_glGetObjectParameterfvARB) numFailed++;
	sfg_ptrc_glGetObjectParameterivARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLenum, GLint *))IntGetProcAddress("glGetObjectParameterivARB");
	if(!sfg_ptrc_glGetObjectParameterivARB) numFailed++;
	sfg_ptrc_glGetShaderSourceARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, GLsizei *, GLcharARB *))IntGetProcAddress("glGetShaderSourceARB");
	if(!sfg_ptrc_glGetShaderSourceARB) numFailed++;
	sfg_ptrc_glGetUniformLocationARB = (GLint (CODEGEN_FUNCPTR *)(GLhandleARB, const GLcharARB *))IntGetProcAddress("glGetUniformLocationARB");
	if(!sfg_ptrc_glGetUniformLocationARB) numFailed++;
	sfg_ptrc_glGetUniformfvARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLint, GLfloat *))IntGetProcAddress("glGetUniformfvARB");
	if(!sfg_ptrc_glGetUniformfvARB) numFailed++;
	sfg_ptrc_glGetUniformivARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLint, GLint *))IntGetProcAddress("glGetUniformivARB");
	if(!sfg_ptrc_glGetUniformivARB) numFailed++;
	sfg_ptrc_glLinkProgramARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glLinkProgramARB");
	if(!sfg_ptrc_glLinkProgramARB) numFailed++;
	sfg_ptrc_glShaderSourceARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLsizei, const GLcharARB **, const GLint *))IntGetProcAddress("glShaderSourceARB");
	if(!sfg_ptrc_glShaderSourceARB) numFailed++;
	sfg_ptrc_glUniform1fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat))IntGetProcAddress("glUniform1fARB");
	if(!sfg_ptrc_glUniform1fARB) numFailed++;
	sfg_ptrc_glUniform1fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform1fvARB");
	if(!sfg_ptrc_glUniform1fvARB) numFailed++;
	sfg_ptrc_glUniform1iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glUniform1iARB");
	if(!sfg_ptrc_glUniform1iARB) numFailed++;
	sfg_ptrc_glUniform1ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform1ivARB");
	if(!sfg_ptrc_glUniform1ivARB) numFailed++;
	sfg_ptrc_glUniform2fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat))IntGetProcAddress("glUniform2fARB");
	if(!sfg_ptrc_glUniform2fARB) numFailed++;
	sfg_ptrc_glUniform2fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform2fvARB");
	if(!sfg_ptrc_glUniform2fvARB) numFailed++;
	sfg_ptrc_glUniform2iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glUniform2iARB");
	if(!sfg_ptrc_glUniform2iARB) numFailed++;
	sfg_ptrc_glUniform2ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform2ivARB");
	if(!sfg_ptrc_glUniform2ivARB) numFailed++;
	sfg_ptrc_glUniform3fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform3fARB");
	if(!sfg_ptrc_glUniform3fARB) numFailed++;
	sfg_ptrc_glUniform3fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform3fvARB");
	if(!sfg_ptrc_glUniform3fvARB) numFailed++;
	sfg_ptrc_glUniform3iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform3iARB");
	if(!sfg_ptrc_glUniform3iARB) numFailed++;
	sfg_ptrc_glUniform3ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform3ivARB");
	if(!sfg_ptrc_glUniform3ivARB) numFailed++;
	sfg_ptrc_glUniform4fARB = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glUniform4fARB");
	if(!sfg_ptrc_glUniform4fARB) numFailed++;
	sfg_ptrc_glUniform4fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLfloat *))IntGetProcAddress("glUniform4fvARB");
	if(!sfg_ptrc_glUniform4fvARB) numFailed++;
	sfg_ptrc_glUniform4iARB = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint, GLint))IntGetProcAddress("glUniform4iARB");
	if(!sfg_ptrc_glUniform4iARB) numFailed++;
	sfg_ptrc_glUniform4ivARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, const GLint *))IntGetProcAddress("glUniform4ivARB");
	if(!sfg_ptrc_glUniform4ivARB) numFailed++;
	sfg_ptrc_glUniformMatrix2fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix2fvARB");
	if(!sfg_ptrc_glUniformMatrix2fvARB) numFailed++;
	sfg_ptrc_glUniformMatrix3fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix3fvARB");
	if(!sfg_ptrc_glUniformMatrix3fvARB) numFailed++;
	sfg_ptrc_glUniformMatrix4fvARB = (void (CODEGEN_FUNCPTR *)(GLint, GLsizei, GLboolean, const GLfloat *))IntGetProcAddress("glUniformMatrix4fvARB");
	if(!sfg_ptrc_glUniformMatrix4fvARB) numFailed++;
	sfg_ptrc_glUseProgramObjectARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glUseProgramObjectARB");
	if(!sfg_ptrc_glUseProgramObjectARB) numFailed++;
	sfg_ptrc_glValidateProgramARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB))IntGetProcAddress("glValidateProgramARB");
	if(!sfg_ptrc_glValidateProgramARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBindAttribLocationARB)(GLhandleARB, GLuint, const GLcharARB *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetActiveAttribARB)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *) = NULL;
GLint (CODEGEN_FUNCPTR *sfg_ptrc_glGetAttribLocationARB)(GLhandleARB, const GLcharARB *) = NULL;

static int Load_ARB_vertex_shader(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindAttribLocationARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, const GLcharARB *))IntGetProcAddress("glBindAttribLocationARB");
	if(!sfg_ptrc_glBindAttribLocationARB) numFailed++;
	sfg_ptrc_glGetActiveAttribARB = (void (CODEGEN_FUNCPTR *)(GLhandleARB, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLcharARB *))IntGetProcAddress("glGetActiveAttribARB");
	if(!sfg_ptrc_glGetActiveAttribARB) numFailed++;
	sfg_ptrc_glGetAttribLocationARB = (GLint (CODEGEN_FUNCPTR *)(GLhandleARB, const GLcharARB *))IntGetProcAddress("glGetAttribLocationARB");
	if(!sfg_ptrc_glGetAttribLocationARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendEquationSeparateEXT)(GLenum, GLenum) = NULL;

static int Load_EXT_blend_equation_separate(void)
{
	int numFailed = 0;
	sfg_ptrc_glBlendEquationSeparateEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendEquationSeparateEXT");
	if(!sfg_ptrc_glBlendEquationSeparateEXT) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBindFramebufferEXT)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBindRenderbufferEXT)(GLenum, GLuint) = NULL;
GLenum (CODEGEN_FUNCPTR *sfg_ptrc_glCheckFramebufferStatusEXT)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteFramebuffersEXT)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteRenderbuffersEXT)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferRenderbufferEXT)(GLenum, GLenum, GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTexture1DEXT)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTexture2DEXT)(GLenum, GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTexture3DEXT)(GLenum, GLenum, GLenum, GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenFramebuffersEXT)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenRenderbuffersEXT)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenerateMipmapEXT)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetFramebufferAttachmentParameterivEXT)(GLenum, GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetRenderbufferParameterivEXT)(GLenum, GLenum, GLint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsFramebufferEXT)(GLuint) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsRenderbufferEXT)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRenderbufferStorageEXT)(GLenum, GLenum, GLsizei, GLsizei) = NULL;

static int Load_EXT_framebuffer_object(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindFramebufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindFramebufferEXT");
	if(!sfg_ptrc_glBindFramebufferEXT) numFailed++;
	sfg_ptrc_glBindRenderbufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindRenderbufferEXT");
	if(!sfg_ptrc_glBindRenderbufferEXT) numFailed++;
	sfg_ptrc_glCheckFramebufferStatusEXT = (GLenum (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCheckFramebufferStatusEXT");
	if(!sfg_ptrc_glCheckFramebufferStatusEXT) numFailed++;
	sfg_ptrc_glDeleteFramebuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteFramebuffersEXT");
	if(!sfg_ptrc_glDeleteFramebuffersEXT) numFailed++;
	sfg_ptrc_glDeleteRenderbuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteRenderbuffersEXT");
	if(!sfg_ptrc_glDeleteRenderbuffersEXT) numFailed++;
	sfg_ptrc_glFramebufferRenderbufferEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint))IntGetProcAddress("glFramebufferRenderbufferEXT");
	if(!sfg_ptrc_glFramebufferRenderbufferEXT) numFailed++;
	sfg_ptrc_glFramebufferTexture1DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture1DEXT");
	if(!sfg_ptrc_glFramebufferTexture1DEXT) numFailed++;
	sfg_ptrc_glFramebufferTexture2DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTexture2DEXT");
	if(!sfg_ptrc_glFramebufferTexture2DEXT) numFailed++;
	sfg_ptrc_glFramebufferTexture3DEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLuint, GLint, GLint))IntGetProcAddress("glFramebufferTexture3DEXT");
	if(!sfg_ptrc_glFramebufferTexture3DEXT) numFailed++;
	sfg_ptrc_glGenFramebuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenFramebuffersEXT");
	if(!sfg_ptrc_glGenFramebuffersEXT) numFailed++;
	sfg_ptrc_glGenRenderbuffersEXT = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenRenderbuffersEXT");
	if(!sfg_ptrc_glGenRenderbuffersEXT) numFailed++;
	sfg_ptrc_glGenerateMipmapEXT = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGenerateMipmapEXT");
	if(!sfg_ptrc_glGenerateMipmapEXT) numFailed++;
	sfg_ptrc_glGetFramebufferAttachmentParameterivEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum, GLint *))IntGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
	if(!sfg_ptrc_glGetFramebufferAttachmentParameterivEXT) numFailed++;
	sfg_ptrc_glGetRenderbufferParameterivEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetRenderbufferParameterivEXT");
	if(!sfg_ptrc_glGetRenderbufferParameterivEXT) numFailed++;
	sfg_ptrc_glIsFramebufferEXT = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsFramebufferEXT");
	if(!sfg_ptrc_glIsFramebufferEXT) numFailed++;
	sfg_ptrc_glIsRenderbufferEXT = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsRenderbufferEXT");
	if(!sfg_ptrc_glIsRenderbufferEXT) numFailed++;
	sfg_ptrc_glRenderbufferStorageEXT = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLsizei, GLsizei))IntGetProcAddress("glRenderbufferStorageEXT");
	if(!sfg_ptrc_glRenderbufferStorageEXT) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glBindVertexArray)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteVertexArrays)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenVertexArrays)(GLsizei, GLuint *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsVertexArray)(GLuint) = NULL;

static int Load_ARB_vertex_array_object(void)
{
	int numFailed = 0;
	sfg_ptrc_glBindVertexArray = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glBindVertexArray");
	if(!sfg_ptrc_glBindVertexArray) numFailed++;
	sfg_ptrc_glDeleteVertexArrays = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteVertexArrays");
	if(!sfg_ptrc_glDeleteVertexArrays) numFailed++;
	sfg_ptrc_glGenVertexArrays = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenVertexArrays");
	if(!sfg_ptrc_glGenVertexArrays) numFailed++;
	sfg_ptrc_glIsVertexArray = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsVertexArray");
	if(!sfg_ptrc_glIsVertexArray) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTextureARB)(GLenum, GLenum, GLuint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTextureFaceARB)(GLenum, GLenum, GLuint, GLint, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFramebufferTextureLayerARB)(GLenum, GLenum, GLuint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glProgramParameteriARB)(GLuint, GLenum, GLint) = NULL;

static int Load_ARB_geometry_shader4(void)
{
	int numFailed = 0;
	sfg_ptrc_glFramebufferTextureARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint, GLint))IntGetProcAddress("glFramebufferTextureARB");
	if(!sfg_ptrc_glFramebufferTextureARB) numFailed++;
	sfg_ptrc_glFramebufferTextureFaceARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint, GLint, GLenum))IntGetProcAddress("glFramebufferTextureFaceARB");
	if(!sfg_ptrc_glFramebufferTextureFaceARB) numFailed++;
	sfg_ptrc_glFramebufferTextureLayerARB = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLuint, GLint, GLint))IntGetProcAddress("glFramebufferTextureLayerARB");
	if(!sfg_ptrc_glFramebufferTextureLayerARB) numFailed++;
	sfg_ptrc_glProgramParameteriARB = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum, GLint))IntGetProcAddress("glProgramParameteriARB");
	if(!sfg_ptrc_glProgramParameteriARB) numFailed++;
	return numFailed;
}

void (CODEGEN_FUNCPTR *sfg_ptrc_glAccum)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glAlphaFunc)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBegin)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBitmap)(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendFunc)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCallList)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCallLists)(GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClear)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClearAccum)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClearDepth)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClearIndex)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClearStencil)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glClipPlane)(GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3b)(GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3ub)(GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3ubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3ui)(GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3uiv)(const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3us)(GLushort, GLushort, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor3usv)(const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4b)(GLbyte, GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4ub)(GLubyte, GLubyte, GLubyte, GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4ubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4ui)(GLuint, GLuint, GLuint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4uiv)(const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4us)(GLushort, GLushort, GLushort, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColor4usv)(const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColorMask)(GLboolean, GLboolean, GLboolean, GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColorMaterial)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyPixels)(GLint, GLint, GLsizei, GLsizei, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCullFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteLists)(GLuint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDepthFunc)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDepthMask)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDepthRange)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDisable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDrawBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDrawPixels)(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEdgeFlag)(GLboolean) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEdgeFlagv)(const GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEnable)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEnd)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEndList)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord1d)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord1dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord1f)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord1fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalCoord2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalMesh1)(GLenum, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalMesh2)(GLenum, GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalPoint1)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEvalPoint2)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFeedbackBuffer)(GLsizei, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFinish)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFlush)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFogf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFogfv)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFogi)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFogiv)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFrontFace)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glFrustum)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
GLuint (CODEGEN_FUNCPTR *sfg_ptrc_glGenLists)(GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetBooleanv)(GLenum, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetClipPlane)(GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetDoublev)(GLenum, GLdouble *) = NULL;
GLenum (CODEGEN_FUNCPTR *sfg_ptrc_glGetError)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetFloatv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetIntegerv)(GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetLightfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetLightiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetMapdv)(GLenum, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetMapfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetMapiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetMaterialfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetMaterialiv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetPixelMapfv)(GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetPixelMapuiv)(GLenum, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetPixelMapusv)(GLenum, GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetPolygonStipple)(GLubyte *) = NULL;
const GLubyte * (CODEGEN_FUNCPTR *sfg_ptrc_glGetString)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexEnvfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexEnviv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexGendv)(GLenum, GLenum, GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexGenfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexGeniv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexImage)(GLenum, GLint, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexLevelParameterfv)(GLenum, GLint, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexLevelParameteriv)(GLenum, GLint, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexParameterfv)(GLenum, GLenum, GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetTexParameteriv)(GLenum, GLenum, GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glHint)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexd)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexdv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexf)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexfv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexi)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexiv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexs)(GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexsv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glInitNames)(void) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsEnabled)(GLenum) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsList)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightModelf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightModelfv)(GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightModeli)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightModeliv)(GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLighti)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLightiv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLineStipple)(GLint, GLushort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLineWidth)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glListBase)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLoadIdentity)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLoadMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLoadMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLoadName)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glLogicOp)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMap1d)(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMap1f)(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMap2d)(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMap2f)(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMapGrid1d)(GLint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMapGrid1f)(GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMapGrid2d)(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMapGrid2f)(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMaterialf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMaterialfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMateriali)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMaterialiv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMatrixMode)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultMatrixd)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glMultMatrixf)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNewList)(GLuint, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3b)(GLbyte, GLbyte, GLbyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3bv)(const GLbyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormal3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPassThrough)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelMapfv)(GLenum, GLsizei, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelMapuiv)(GLenum, GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelMapusv)(GLenum, GLsizei, const GLushort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelStoref)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelStorei)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelTransferf)(GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelTransferi)(GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPixelZoom)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPointSize)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPolygonMode)(GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPolygonStipple)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPopAttrib)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPopMatrix)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPopName)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPushAttrib)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPushMatrix)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPushName)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRasterPos4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glReadBuffer)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectd)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectdv)(const GLdouble *, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectf)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectfv)(const GLfloat *, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRecti)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectiv)(const GLint *, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRects)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRectsv)(const GLshort *, const GLshort *) = NULL;
GLint (CODEGEN_FUNCPTR *sfg_ptrc_glRenderMode)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRotated)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glRotatef)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glScaled)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glScalef)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glScissor)(GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glSelectBuffer)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glShadeModel)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glStencilFunc)(GLenum, GLint, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glStencilMask)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glStencilOp)(GLenum, GLenum, GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1d)(GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1f)(GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1i)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1s)(GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord1sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoord4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexEnvf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexEnvfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexEnvi)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexEnviv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGend)(GLenum, GLenum, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGendv)(GLenum, GLenum, const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGenf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGenfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGeni)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexGeniv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexImage1D)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexParameterf)(GLenum, GLenum, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexParameterfv)(GLenum, GLenum, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexParameteri)(GLenum, GLenum, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexParameteriv)(GLenum, GLenum, const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTranslated)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTranslatef)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2d)(GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2f)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2i)(GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2s)(GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex2sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3d)(GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3f)(GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3i)(GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3s)(GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex3sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4d)(GLdouble, GLdouble, GLdouble, GLdouble) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4dv)(const GLdouble *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4f)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4fv)(const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4i)(GLint, GLint, GLint, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4iv)(const GLint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4s)(GLshort, GLshort, GLshort, GLshort) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertex4sv)(const GLshort *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glViewport)(GLint, GLint, GLsizei, GLsizei) = NULL;

GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glAreTexturesResident)(GLsizei, const GLuint *, GLboolean *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glArrayElement)(GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBindTexture)(GLenum, GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glColorPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyTexImage1D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyTexImage2D)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyTexSubImage1D)(GLenum, GLint, GLint, GLint, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyTexSubImage2D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDeleteTextures)(GLsizei, const GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDisableClientState)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDrawArrays)(GLenum, GLint, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDrawElements)(GLenum, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEdgeFlagPointer)(GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glEnableClientState)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGenTextures)(GLsizei, GLuint *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glGetPointerv)(GLenum, GLvoid **) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexPointer)(GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexub)(GLubyte) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glIndexubv)(const GLubyte *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glInterleavedArrays)(GLenum, GLsizei, const GLvoid *) = NULL;
GLboolean (CODEGEN_FUNCPTR *sfg_ptrc_glIsTexture)(GLuint) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glNormalPointer)(GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPolygonOffset)(GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPopClientAttrib)(void) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPrioritizeTextures)(GLsizei, const GLuint *, const GLfloat *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glPushClientAttrib)(GLbitfield) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexCoordPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glVertexPointer)(GLint, GLenum, GLsizei, const GLvoid *) = NULL;

void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendColor)(GLfloat, GLfloat, GLfloat, GLfloat) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glBlendEquation)(GLenum) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glCopyTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glDrawRangeElements)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *) = NULL;
void (CODEGEN_FUNCPTR *sfg_ptrc_glTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *) = NULL;

static int Load_Version_1_2(void)
{
	int numFailed = 0;
	sfg_ptrc_glAccum = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glAccum");
	if(!sfg_ptrc_glAccum) numFailed++;
	sfg_ptrc_glAlphaFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glAlphaFunc");
	if(!sfg_ptrc_glAlphaFunc) numFailed++;
	sfg_ptrc_glBegin = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBegin");
	if(!sfg_ptrc_glBegin) numFailed++;
	sfg_ptrc_glBitmap = (void (CODEGEN_FUNCPTR *)(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *))IntGetProcAddress("glBitmap");
	if(!sfg_ptrc_glBitmap) numFailed++;
	sfg_ptrc_glBlendFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glBlendFunc");
	if(!sfg_ptrc_glBlendFunc) numFailed++;
	sfg_ptrc_glCallList = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glCallList");
	if(!sfg_ptrc_glCallList) numFailed++;
	sfg_ptrc_glCallLists = (void (CODEGEN_FUNCPTR *)(GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glCallLists");
	if(!sfg_ptrc_glCallLists) numFailed++;
	sfg_ptrc_glClear = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glClear");
	if(!sfg_ptrc_glClear) numFailed++;
	sfg_ptrc_glClearAccum = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearAccum");
	if(!sfg_ptrc_glClearAccum) numFailed++;
	sfg_ptrc_glClearColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glClearColor");
	if(!sfg_ptrc_glClearColor) numFailed++;
	sfg_ptrc_glClearDepth = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glClearDepth");
	if(!sfg_ptrc_glClearDepth) numFailed++;
	sfg_ptrc_glClearIndex = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glClearIndex");
	if(!sfg_ptrc_glClearIndex) numFailed++;
	sfg_ptrc_glClearStencil = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glClearStencil");
	if(!sfg_ptrc_glClearStencil) numFailed++;
	sfg_ptrc_glClipPlane = (void (CODEGEN_FUNCPTR *)(GLenum, const GLdouble *))IntGetProcAddress("glClipPlane");
	if(!sfg_ptrc_glClipPlane) numFailed++;
	sfg_ptrc_glColor3b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte))IntGetProcAddress("glColor3b");
	if(!sfg_ptrc_glColor3b) numFailed++;
	sfg_ptrc_glColor3bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glColor3bv");
	if(!sfg_ptrc_glColor3bv) numFailed++;
	sfg_ptrc_glColor3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glColor3d");
	if(!sfg_ptrc_glColor3d) numFailed++;
	sfg_ptrc_glColor3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glColor3dv");
	if(!sfg_ptrc_glColor3dv) numFailed++;
	sfg_ptrc_glColor3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glColor3f");
	if(!sfg_ptrc_glColor3f) numFailed++;
	sfg_ptrc_glColor3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glColor3fv");
	if(!sfg_ptrc_glColor3fv) numFailed++;
	sfg_ptrc_glColor3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glColor3i");
	if(!sfg_ptrc_glColor3i) numFailed++;
	sfg_ptrc_glColor3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glColor3iv");
	if(!sfg_ptrc_glColor3iv) numFailed++;
	sfg_ptrc_glColor3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glColor3s");
	if(!sfg_ptrc_glColor3s) numFailed++;
	sfg_ptrc_glColor3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glColor3sv");
	if(!sfg_ptrc_glColor3sv) numFailed++;
	sfg_ptrc_glColor3ub = (void (CODEGEN_FUNCPTR *)(GLubyte, GLubyte, GLubyte))IntGetProcAddress("glColor3ub");
	if(!sfg_ptrc_glColor3ub) numFailed++;
	sfg_ptrc_glColor3ubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glColor3ubv");
	if(!sfg_ptrc_glColor3ubv) numFailed++;
	sfg_ptrc_glColor3ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint))IntGetProcAddress("glColor3ui");
	if(!sfg_ptrc_glColor3ui) numFailed++;
	sfg_ptrc_glColor3uiv = (void (CODEGEN_FUNCPTR *)(const GLuint *))IntGetProcAddress("glColor3uiv");
	if(!sfg_ptrc_glColor3uiv) numFailed++;
	sfg_ptrc_glColor3us = (void (CODEGEN_FUNCPTR *)(GLushort, GLushort, GLushort))IntGetProcAddress("glColor3us");
	if(!sfg_ptrc_glColor3us) numFailed++;
	sfg_ptrc_glColor3usv = (void (CODEGEN_FUNCPTR *)(const GLushort *))IntGetProcAddress("glColor3usv");
	if(!sfg_ptrc_glColor3usv) numFailed++;
	sfg_ptrc_glColor4b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte, GLbyte))IntGetProcAddress("glColor4b");
	if(!sfg_ptrc_glColor4b) numFailed++;
	sfg_ptrc_glColor4bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glColor4bv");
	if(!sfg_ptrc_glColor4bv) numFailed++;
	sfg_ptrc_glColor4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glColor4d");
	if(!sfg_ptrc_glColor4d) numFailed++;
	sfg_ptrc_glColor4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glColor4dv");
	if(!sfg_ptrc_glColor4dv) numFailed++;
	sfg_ptrc_glColor4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glColor4f");
	if(!sfg_ptrc_glColor4f) numFailed++;
	sfg_ptrc_glColor4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glColor4fv");
	if(!sfg_ptrc_glColor4fv) numFailed++;
	sfg_ptrc_glColor4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glColor4i");
	if(!sfg_ptrc_glColor4i) numFailed++;
	sfg_ptrc_glColor4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glColor4iv");
	if(!sfg_ptrc_glColor4iv) numFailed++;
	sfg_ptrc_glColor4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glColor4s");
	if(!sfg_ptrc_glColor4s) numFailed++;
	sfg_ptrc_glColor4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glColor4sv");
	if(!sfg_ptrc_glColor4sv) numFailed++;
	sfg_ptrc_glColor4ub = (void (CODEGEN_FUNCPTR *)(GLubyte, GLubyte, GLubyte, GLubyte))IntGetProcAddress("glColor4ub");
	if(!sfg_ptrc_glColor4ub) numFailed++;
	sfg_ptrc_glColor4ubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glColor4ubv");
	if(!sfg_ptrc_glColor4ubv) numFailed++;
	sfg_ptrc_glColor4ui = (void (CODEGEN_FUNCPTR *)(GLuint, GLuint, GLuint, GLuint))IntGetProcAddress("glColor4ui");
	if(!sfg_ptrc_glColor4ui) numFailed++;
	sfg_ptrc_glColor4uiv = (void (CODEGEN_FUNCPTR *)(const GLuint *))IntGetProcAddress("glColor4uiv");
	if(!sfg_ptrc_glColor4uiv) numFailed++;
	sfg_ptrc_glColor4us = (void (CODEGEN_FUNCPTR *)(GLushort, GLushort, GLushort, GLushort))IntGetProcAddress("glColor4us");
	if(!sfg_ptrc_glColor4us) numFailed++;
	sfg_ptrc_glColor4usv = (void (CODEGEN_FUNCPTR *)(const GLushort *))IntGetProcAddress("glColor4usv");
	if(!sfg_ptrc_glColor4usv) numFailed++;
	sfg_ptrc_glColorMask = (void (CODEGEN_FUNCPTR *)(GLboolean, GLboolean, GLboolean, GLboolean))IntGetProcAddress("glColorMask");
	if(!sfg_ptrc_glColorMask) numFailed++;
	sfg_ptrc_glColorMaterial = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glColorMaterial");
	if(!sfg_ptrc_glColorMaterial) numFailed++;
	sfg_ptrc_glCopyPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum))IntGetProcAddress("glCopyPixels");
	if(!sfg_ptrc_glCopyPixels) numFailed++;
	sfg_ptrc_glCullFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glCullFace");
	if(!sfg_ptrc_glCullFace) numFailed++;
	sfg_ptrc_glDeleteLists = (void (CODEGEN_FUNCPTR *)(GLuint, GLsizei))IntGetProcAddress("glDeleteLists");
	if(!sfg_ptrc_glDeleteLists) numFailed++;
	sfg_ptrc_glDepthFunc = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDepthFunc");
	if(!sfg_ptrc_glDepthFunc) numFailed++;
	sfg_ptrc_glDepthMask = (void (CODEGEN_FUNCPTR *)(GLboolean))IntGetProcAddress("glDepthMask");
	if(!sfg_ptrc_glDepthMask) numFailed++;
	sfg_ptrc_glDepthRange = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glDepthRange");
	if(!sfg_ptrc_glDepthRange) numFailed++;
	sfg_ptrc_glDisable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDisable");
	if(!sfg_ptrc_glDisable) numFailed++;
	sfg_ptrc_glDrawBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDrawBuffer");
	if(!sfg_ptrc_glDrawBuffer) numFailed++;
	sfg_ptrc_glDrawPixels = (void (CODEGEN_FUNCPTR *)(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glDrawPixels");
	if(!sfg_ptrc_glDrawPixels) numFailed++;
	sfg_ptrc_glEdgeFlag = (void (CODEGEN_FUNCPTR *)(GLboolean))IntGetProcAddress("glEdgeFlag");
	if(!sfg_ptrc_glEdgeFlag) numFailed++;
	sfg_ptrc_glEdgeFlagv = (void (CODEGEN_FUNCPTR *)(const GLboolean *))IntGetProcAddress("glEdgeFlagv");
	if(!sfg_ptrc_glEdgeFlagv) numFailed++;
	sfg_ptrc_glEnable = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnable");
	if(!sfg_ptrc_glEnable) numFailed++;
	sfg_ptrc_glEnd = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glEnd");
	if(!sfg_ptrc_glEnd) numFailed++;
	sfg_ptrc_glEndList = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glEndList");
	if(!sfg_ptrc_glEndList) numFailed++;
	sfg_ptrc_glEvalCoord1d = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glEvalCoord1d");
	if(!sfg_ptrc_glEvalCoord1d) numFailed++;
	sfg_ptrc_glEvalCoord1dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glEvalCoord1dv");
	if(!sfg_ptrc_glEvalCoord1dv) numFailed++;
	sfg_ptrc_glEvalCoord1f = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glEvalCoord1f");
	if(!sfg_ptrc_glEvalCoord1f) numFailed++;
	sfg_ptrc_glEvalCoord1fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glEvalCoord1fv");
	if(!sfg_ptrc_glEvalCoord1fv) numFailed++;
	sfg_ptrc_glEvalCoord2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glEvalCoord2d");
	if(!sfg_ptrc_glEvalCoord2d) numFailed++;
	sfg_ptrc_glEvalCoord2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glEvalCoord2dv");
	if(!sfg_ptrc_glEvalCoord2dv) numFailed++;
	sfg_ptrc_glEvalCoord2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glEvalCoord2f");
	if(!sfg_ptrc_glEvalCoord2f) numFailed++;
	sfg_ptrc_glEvalCoord2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glEvalCoord2fv");
	if(!sfg_ptrc_glEvalCoord2fv) numFailed++;
	sfg_ptrc_glEvalMesh1 = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint))IntGetProcAddress("glEvalMesh1");
	if(!sfg_ptrc_glEvalMesh1) numFailed++;
	sfg_ptrc_glEvalMesh2 = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint))IntGetProcAddress("glEvalMesh2");
	if(!sfg_ptrc_glEvalMesh2) numFailed++;
	sfg_ptrc_glEvalPoint1 = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glEvalPoint1");
	if(!sfg_ptrc_glEvalPoint1) numFailed++;
	sfg_ptrc_glEvalPoint2 = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glEvalPoint2");
	if(!sfg_ptrc_glEvalPoint2) numFailed++;
	sfg_ptrc_glFeedbackBuffer = (void (CODEGEN_FUNCPTR *)(GLsizei, GLenum, GLfloat *))IntGetProcAddress("glFeedbackBuffer");
	if(!sfg_ptrc_glFeedbackBuffer) numFailed++;
	sfg_ptrc_glFinish = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glFinish");
	if(!sfg_ptrc_glFinish) numFailed++;
	sfg_ptrc_glFlush = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glFlush");
	if(!sfg_ptrc_glFlush) numFailed++;
	sfg_ptrc_glFogf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glFogf");
	if(!sfg_ptrc_glFogf) numFailed++;
	sfg_ptrc_glFogfv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glFogfv");
	if(!sfg_ptrc_glFogfv) numFailed++;
	sfg_ptrc_glFogi = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glFogi");
	if(!sfg_ptrc_glFogi) numFailed++;
	sfg_ptrc_glFogiv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glFogiv");
	if(!sfg_ptrc_glFogiv) numFailed++;
	sfg_ptrc_glFrontFace = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glFrontFace");
	if(!sfg_ptrc_glFrontFace) numFailed++;
	sfg_ptrc_glFrustum = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glFrustum");
	if(!sfg_ptrc_glFrustum) numFailed++;
	sfg_ptrc_glGenLists = (GLuint (CODEGEN_FUNCPTR *)(GLsizei))IntGetProcAddress("glGenLists");
	if(!sfg_ptrc_glGenLists) numFailed++;
	sfg_ptrc_glGetBooleanv = (void (CODEGEN_FUNCPTR *)(GLenum, GLboolean *))IntGetProcAddress("glGetBooleanv");
	if(!sfg_ptrc_glGetBooleanv) numFailed++;
	sfg_ptrc_glGetClipPlane = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble *))IntGetProcAddress("glGetClipPlane");
	if(!sfg_ptrc_glGetClipPlane) numFailed++;
	sfg_ptrc_glGetDoublev = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble *))IntGetProcAddress("glGetDoublev");
	if(!sfg_ptrc_glGetDoublev) numFailed++;
	sfg_ptrc_glGetError = (GLenum (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glGetError");
	if(!sfg_ptrc_glGetError) numFailed++;
	sfg_ptrc_glGetFloatv = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat *))IntGetProcAddress("glGetFloatv");
	if(!sfg_ptrc_glGetFloatv) numFailed++;
	sfg_ptrc_glGetIntegerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint *))IntGetProcAddress("glGetIntegerv");
	if(!sfg_ptrc_glGetIntegerv) numFailed++;
	sfg_ptrc_glGetLightfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetLightfv");
	if(!sfg_ptrc_glGetLightfv) numFailed++;
	sfg_ptrc_glGetLightiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetLightiv");
	if(!sfg_ptrc_glGetLightiv) numFailed++;
	sfg_ptrc_glGetMapdv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble *))IntGetProcAddress("glGetMapdv");
	if(!sfg_ptrc_glGetMapdv) numFailed++;
	sfg_ptrc_glGetMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetMapfv");
	if(!sfg_ptrc_glGetMapfv) numFailed++;
	sfg_ptrc_glGetMapiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetMapiv");
	if(!sfg_ptrc_glGetMapiv) numFailed++;
	sfg_ptrc_glGetMaterialfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetMaterialfv");
	if(!sfg_ptrc_glGetMaterialfv) numFailed++;
	sfg_ptrc_glGetMaterialiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetMaterialiv");
	if(!sfg_ptrc_glGetMaterialiv) numFailed++;
	sfg_ptrc_glGetPixelMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat *))IntGetProcAddress("glGetPixelMapfv");
	if(!sfg_ptrc_glGetPixelMapfv) numFailed++;
	sfg_ptrc_glGetPixelMapuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint *))IntGetProcAddress("glGetPixelMapuiv");
	if(!sfg_ptrc_glGetPixelMapuiv) numFailed++;
	sfg_ptrc_glGetPixelMapusv = (void (CODEGEN_FUNCPTR *)(GLenum, GLushort *))IntGetProcAddress("glGetPixelMapusv");
	if(!sfg_ptrc_glGetPixelMapusv) numFailed++;
	sfg_ptrc_glGetPolygonStipple = (void (CODEGEN_FUNCPTR *)(GLubyte *))IntGetProcAddress("glGetPolygonStipple");
	if(!sfg_ptrc_glGetPolygonStipple) numFailed++;
	sfg_ptrc_glGetString = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetString");
	if(!sfg_ptrc_glGetString) numFailed++;
	sfg_ptrc_glGetTexEnvfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexEnvfv");
	if(!sfg_ptrc_glGetTexEnvfv) numFailed++;
	sfg_ptrc_glGetTexEnviv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexEnviv");
	if(!sfg_ptrc_glGetTexEnviv) numFailed++;
	sfg_ptrc_glGetTexGendv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble *))IntGetProcAddress("glGetTexGendv");
	if(!sfg_ptrc_glGetTexGendv) numFailed++;
	sfg_ptrc_glGetTexGenfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexGenfv");
	if(!sfg_ptrc_glGetTexGenfv) numFailed++;
	sfg_ptrc_glGetTexGeniv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexGeniv");
	if(!sfg_ptrc_glGetTexGeniv) numFailed++;
	sfg_ptrc_glGetTexImage = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLenum, GLvoid *))IntGetProcAddress("glGetTexImage");
	if(!sfg_ptrc_glGetTexImage) numFailed++;
	sfg_ptrc_glGetTexLevelParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLfloat *))IntGetProcAddress("glGetTexLevelParameterfv");
	if(!sfg_ptrc_glGetTexLevelParameterfv) numFailed++;
	sfg_ptrc_glGetTexLevelParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint *))IntGetProcAddress("glGetTexLevelParameteriv");
	if(!sfg_ptrc_glGetTexLevelParameteriv) numFailed++;
	sfg_ptrc_glGetTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat *))IntGetProcAddress("glGetTexParameterfv");
	if(!sfg_ptrc_glGetTexParameterfv) numFailed++;
	sfg_ptrc_glGetTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint *))IntGetProcAddress("glGetTexParameteriv");
	if(!sfg_ptrc_glGetTexParameteriv) numFailed++;
	sfg_ptrc_glHint = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glHint");
	if(!sfg_ptrc_glHint) numFailed++;
	sfg_ptrc_glIndexMask = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIndexMask");
	if(!sfg_ptrc_glIndexMask) numFailed++;
	sfg_ptrc_glIndexd = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glIndexd");
	if(!sfg_ptrc_glIndexd) numFailed++;
	sfg_ptrc_glIndexdv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glIndexdv");
	if(!sfg_ptrc_glIndexdv) numFailed++;
	sfg_ptrc_glIndexf = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glIndexf");
	if(!sfg_ptrc_glIndexf) numFailed++;
	sfg_ptrc_glIndexfv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glIndexfv");
	if(!sfg_ptrc_glIndexfv) numFailed++;
	sfg_ptrc_glIndexi = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glIndexi");
	if(!sfg_ptrc_glIndexi) numFailed++;
	sfg_ptrc_glIndexiv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glIndexiv");
	if(!sfg_ptrc_glIndexiv) numFailed++;
	sfg_ptrc_glIndexs = (void (CODEGEN_FUNCPTR *)(GLshort))IntGetProcAddress("glIndexs");
	if(!sfg_ptrc_glIndexs) numFailed++;
	sfg_ptrc_glIndexsv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glIndexsv");
	if(!sfg_ptrc_glIndexsv) numFailed++;
	sfg_ptrc_glInitNames = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glInitNames");
	if(!sfg_ptrc_glInitNames) numFailed++;
	sfg_ptrc_glIsEnabled = (GLboolean (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glIsEnabled");
	if(!sfg_ptrc_glIsEnabled) numFailed++;
	sfg_ptrc_glIsList = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsList");
	if(!sfg_ptrc_glIsList) numFailed++;
	sfg_ptrc_glLightModelf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glLightModelf");
	if(!sfg_ptrc_glLightModelf) numFailed++;
	sfg_ptrc_glLightModelfv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLfloat *))IntGetProcAddress("glLightModelfv");
	if(!sfg_ptrc_glLightModelfv) numFailed++;
	sfg_ptrc_glLightModeli = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glLightModeli");
	if(!sfg_ptrc_glLightModeli) numFailed++;
	sfg_ptrc_glLightModeliv = (void (CODEGEN_FUNCPTR *)(GLenum, const GLint *))IntGetProcAddress("glLightModeliv");
	if(!sfg_ptrc_glLightModeliv) numFailed++;
	sfg_ptrc_glLightf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glLightf");
	if(!sfg_ptrc_glLightf) numFailed++;
	sfg_ptrc_glLightfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glLightfv");
	if(!sfg_ptrc_glLightfv) numFailed++;
	sfg_ptrc_glLighti = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glLighti");
	if(!sfg_ptrc_glLighti) numFailed++;
	sfg_ptrc_glLightiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glLightiv");
	if(!sfg_ptrc_glLightiv) numFailed++;
	sfg_ptrc_glLineStipple = (void (CODEGEN_FUNCPTR *)(GLint, GLushort))IntGetProcAddress("glLineStipple");
	if(!sfg_ptrc_glLineStipple) numFailed++;
	sfg_ptrc_glLineWidth = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glLineWidth");
	if(!sfg_ptrc_glLineWidth) numFailed++;
	sfg_ptrc_glListBase = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glListBase");
	if(!sfg_ptrc_glListBase) numFailed++;
	sfg_ptrc_glLoadIdentity = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glLoadIdentity");
	if(!sfg_ptrc_glLoadIdentity) numFailed++;
	sfg_ptrc_glLoadMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glLoadMatrixd");
	if(!sfg_ptrc_glLoadMatrixd) numFailed++;
	sfg_ptrc_glLoadMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glLoadMatrixf");
	if(!sfg_ptrc_glLoadMatrixf) numFailed++;
	sfg_ptrc_glLoadName = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glLoadName");
	if(!sfg_ptrc_glLoadName) numFailed++;
	sfg_ptrc_glLogicOp = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glLogicOp");
	if(!sfg_ptrc_glLogicOp) numFailed++;
	sfg_ptrc_glMap1d = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *))IntGetProcAddress("glMap1d");
	if(!sfg_ptrc_glMap1d) numFailed++;
	sfg_ptrc_glMap1f = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *))IntGetProcAddress("glMap1f");
	if(!sfg_ptrc_glMap1f) numFailed++;
	sfg_ptrc_glMap2d = (void (CODEGEN_FUNCPTR *)(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *))IntGetProcAddress("glMap2d");
	if(!sfg_ptrc_glMap2d) numFailed++;
	sfg_ptrc_glMap2f = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *))IntGetProcAddress("glMap2f");
	if(!sfg_ptrc_glMap2f) numFailed++;
	sfg_ptrc_glMapGrid1d = (void (CODEGEN_FUNCPTR *)(GLint, GLdouble, GLdouble))IntGetProcAddress("glMapGrid1d");
	if(!sfg_ptrc_glMapGrid1d) numFailed++;
	sfg_ptrc_glMapGrid1f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat))IntGetProcAddress("glMapGrid1f");
	if(!sfg_ptrc_glMapGrid1f) numFailed++;
	sfg_ptrc_glMapGrid2d = (void (CODEGEN_FUNCPTR *)(GLint, GLdouble, GLdouble, GLint, GLdouble, GLdouble))IntGetProcAddress("glMapGrid2d");
	if(!sfg_ptrc_glMapGrid2d) numFailed++;
	sfg_ptrc_glMapGrid2f = (void (CODEGEN_FUNCPTR *)(GLint, GLfloat, GLfloat, GLint, GLfloat, GLfloat))IntGetProcAddress("glMapGrid2f");
	if(!sfg_ptrc_glMapGrid2f) numFailed++;
	sfg_ptrc_glMaterialf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glMaterialf");
	if(!sfg_ptrc_glMaterialf) numFailed++;
	sfg_ptrc_glMaterialfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glMaterialfv");
	if(!sfg_ptrc_glMaterialfv) numFailed++;
	sfg_ptrc_glMateriali = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glMateriali");
	if(!sfg_ptrc_glMateriali) numFailed++;
	sfg_ptrc_glMaterialiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glMaterialiv");
	if(!sfg_ptrc_glMaterialiv) numFailed++;
	sfg_ptrc_glMatrixMode = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glMatrixMode");
	if(!sfg_ptrc_glMatrixMode) numFailed++;
	sfg_ptrc_glMultMatrixd = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glMultMatrixd");
	if(!sfg_ptrc_glMultMatrixd) numFailed++;
	sfg_ptrc_glMultMatrixf = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glMultMatrixf");
	if(!sfg_ptrc_glMultMatrixf) numFailed++;
	sfg_ptrc_glNewList = (void (CODEGEN_FUNCPTR *)(GLuint, GLenum))IntGetProcAddress("glNewList");
	if(!sfg_ptrc_glNewList) numFailed++;
	sfg_ptrc_glNormal3b = (void (CODEGEN_FUNCPTR *)(GLbyte, GLbyte, GLbyte))IntGetProcAddress("glNormal3b");
	if(!sfg_ptrc_glNormal3b) numFailed++;
	sfg_ptrc_glNormal3bv = (void (CODEGEN_FUNCPTR *)(const GLbyte *))IntGetProcAddress("glNormal3bv");
	if(!sfg_ptrc_glNormal3bv) numFailed++;
	sfg_ptrc_glNormal3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glNormal3d");
	if(!sfg_ptrc_glNormal3d) numFailed++;
	sfg_ptrc_glNormal3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glNormal3dv");
	if(!sfg_ptrc_glNormal3dv) numFailed++;
	sfg_ptrc_glNormal3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glNormal3f");
	if(!sfg_ptrc_glNormal3f) numFailed++;
	sfg_ptrc_glNormal3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glNormal3fv");
	if(!sfg_ptrc_glNormal3fv) numFailed++;
	sfg_ptrc_glNormal3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glNormal3i");
	if(!sfg_ptrc_glNormal3i) numFailed++;
	sfg_ptrc_glNormal3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glNormal3iv");
	if(!sfg_ptrc_glNormal3iv) numFailed++;
	sfg_ptrc_glNormal3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glNormal3s");
	if(!sfg_ptrc_glNormal3s) numFailed++;
	sfg_ptrc_glNormal3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glNormal3sv");
	if(!sfg_ptrc_glNormal3sv) numFailed++;
	sfg_ptrc_glOrtho = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glOrtho");
	if(!sfg_ptrc_glOrtho) numFailed++;
	sfg_ptrc_glPassThrough = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPassThrough");
	if(!sfg_ptrc_glPassThrough) numFailed++;
	sfg_ptrc_glPixelMapfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLfloat *))IntGetProcAddress("glPixelMapfv");
	if(!sfg_ptrc_glPixelMapfv) numFailed++;
	sfg_ptrc_glPixelMapuiv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLuint *))IntGetProcAddress("glPixelMapuiv");
	if(!sfg_ptrc_glPixelMapuiv) numFailed++;
	sfg_ptrc_glPixelMapusv = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLushort *))IntGetProcAddress("glPixelMapusv");
	if(!sfg_ptrc_glPixelMapusv) numFailed++;
	sfg_ptrc_glPixelStoref = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPixelStoref");
	if(!sfg_ptrc_glPixelStoref) numFailed++;
	sfg_ptrc_glPixelStorei = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPixelStorei");
	if(!sfg_ptrc_glPixelStorei) numFailed++;
	sfg_ptrc_glPixelTransferf = (void (CODEGEN_FUNCPTR *)(GLenum, GLfloat))IntGetProcAddress("glPixelTransferf");
	if(!sfg_ptrc_glPixelTransferf) numFailed++;
	sfg_ptrc_glPixelTransferi = (void (CODEGEN_FUNCPTR *)(GLenum, GLint))IntGetProcAddress("glPixelTransferi");
	if(!sfg_ptrc_glPixelTransferi) numFailed++;
	sfg_ptrc_glPixelZoom = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glPixelZoom");
	if(!sfg_ptrc_glPixelZoom) numFailed++;
	sfg_ptrc_glPointSize = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glPointSize");
	if(!sfg_ptrc_glPointSize) numFailed++;
	sfg_ptrc_glPolygonMode = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum))IntGetProcAddress("glPolygonMode");
	if(!sfg_ptrc_glPolygonMode) numFailed++;
	sfg_ptrc_glPolygonStipple = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glPolygonStipple");
	if(!sfg_ptrc_glPolygonStipple) numFailed++;
	sfg_ptrc_glPopAttrib = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glPopAttrib");
	if(!sfg_ptrc_glPopAttrib) numFailed++;
	sfg_ptrc_glPopMatrix = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glPopMatrix");
	if(!sfg_ptrc_glPopMatrix) numFailed++;
	sfg_ptrc_glPopName = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glPopName");
	if(!sfg_ptrc_glPopName) numFailed++;
	sfg_ptrc_glPushAttrib = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glPushAttrib");
	if(!sfg_ptrc_glPushAttrib) numFailed++;
	sfg_ptrc_glPushMatrix = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glPushMatrix");
	if(!sfg_ptrc_glPushMatrix) numFailed++;
	sfg_ptrc_glPushName = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glPushName");
	if(!sfg_ptrc_glPushName) numFailed++;
	sfg_ptrc_glRasterPos2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glRasterPos2d");
	if(!sfg_ptrc_glRasterPos2d) numFailed++;
	sfg_ptrc_glRasterPos2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos2dv");
	if(!sfg_ptrc_glRasterPos2dv) numFailed++;
	sfg_ptrc_glRasterPos2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glRasterPos2f");
	if(!sfg_ptrc_glRasterPos2f) numFailed++;
	sfg_ptrc_glRasterPos2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos2fv");
	if(!sfg_ptrc_glRasterPos2fv) numFailed++;
	sfg_ptrc_glRasterPos2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glRasterPos2i");
	if(!sfg_ptrc_glRasterPos2i) numFailed++;
	sfg_ptrc_glRasterPos2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos2iv");
	if(!sfg_ptrc_glRasterPos2iv) numFailed++;
	sfg_ptrc_glRasterPos2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glRasterPos2s");
	if(!sfg_ptrc_glRasterPos2s) numFailed++;
	sfg_ptrc_glRasterPos2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos2sv");
	if(!sfg_ptrc_glRasterPos2sv) numFailed++;
	sfg_ptrc_glRasterPos3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRasterPos3d");
	if(!sfg_ptrc_glRasterPos3d) numFailed++;
	sfg_ptrc_glRasterPos3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos3dv");
	if(!sfg_ptrc_glRasterPos3dv) numFailed++;
	sfg_ptrc_glRasterPos3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRasterPos3f");
	if(!sfg_ptrc_glRasterPos3f) numFailed++;
	sfg_ptrc_glRasterPos3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos3fv");
	if(!sfg_ptrc_glRasterPos3fv) numFailed++;
	sfg_ptrc_glRasterPos3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glRasterPos3i");
	if(!sfg_ptrc_glRasterPos3i) numFailed++;
	sfg_ptrc_glRasterPos3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos3iv");
	if(!sfg_ptrc_glRasterPos3iv) numFailed++;
	sfg_ptrc_glRasterPos3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glRasterPos3s");
	if(!sfg_ptrc_glRasterPos3s) numFailed++;
	sfg_ptrc_glRasterPos3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos3sv");
	if(!sfg_ptrc_glRasterPos3sv) numFailed++;
	sfg_ptrc_glRasterPos4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRasterPos4d");
	if(!sfg_ptrc_glRasterPos4d) numFailed++;
	sfg_ptrc_glRasterPos4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glRasterPos4dv");
	if(!sfg_ptrc_glRasterPos4dv) numFailed++;
	sfg_ptrc_glRasterPos4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRasterPos4f");
	if(!sfg_ptrc_glRasterPos4f) numFailed++;
	sfg_ptrc_glRasterPos4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glRasterPos4fv");
	if(!sfg_ptrc_glRasterPos4fv) numFailed++;
	sfg_ptrc_glRasterPos4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glRasterPos4i");
	if(!sfg_ptrc_glRasterPos4i) numFailed++;
	sfg_ptrc_glRasterPos4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glRasterPos4iv");
	if(!sfg_ptrc_glRasterPos4iv) numFailed++;
	sfg_ptrc_glRasterPos4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glRasterPos4s");
	if(!sfg_ptrc_glRasterPos4s) numFailed++;
	sfg_ptrc_glRasterPos4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glRasterPos4sv");
	if(!sfg_ptrc_glRasterPos4sv) numFailed++;
	sfg_ptrc_glReadBuffer = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glReadBuffer");
	if(!sfg_ptrc_glReadBuffer) numFailed++;
	sfg_ptrc_glReadPixels = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *))IntGetProcAddress("glReadPixels");
	if(!sfg_ptrc_glReadPixels) numFailed++;
	sfg_ptrc_glRectd = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRectd");
	if(!sfg_ptrc_glRectd) numFailed++;
	sfg_ptrc_glRectdv = (void (CODEGEN_FUNCPTR *)(const GLdouble *, const GLdouble *))IntGetProcAddress("glRectdv");
	if(!sfg_ptrc_glRectdv) numFailed++;
	sfg_ptrc_glRectf = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRectf");
	if(!sfg_ptrc_glRectf) numFailed++;
	sfg_ptrc_glRectfv = (void (CODEGEN_FUNCPTR *)(const GLfloat *, const GLfloat *))IntGetProcAddress("glRectfv");
	if(!sfg_ptrc_glRectfv) numFailed++;
	sfg_ptrc_glRecti = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glRecti");
	if(!sfg_ptrc_glRecti) numFailed++;
	sfg_ptrc_glRectiv = (void (CODEGEN_FUNCPTR *)(const GLint *, const GLint *))IntGetProcAddress("glRectiv");
	if(!sfg_ptrc_glRectiv) numFailed++;
	sfg_ptrc_glRects = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glRects");
	if(!sfg_ptrc_glRects) numFailed++;
	sfg_ptrc_glRectsv = (void (CODEGEN_FUNCPTR *)(const GLshort *, const GLshort *))IntGetProcAddress("glRectsv");
	if(!sfg_ptrc_glRectsv) numFailed++;
	sfg_ptrc_glRenderMode = (GLint (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glRenderMode");
	if(!sfg_ptrc_glRenderMode) numFailed++;
	sfg_ptrc_glRotated = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glRotated");
	if(!sfg_ptrc_glRotated) numFailed++;
	sfg_ptrc_glRotatef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glRotatef");
	if(!sfg_ptrc_glRotatef) numFailed++;
	sfg_ptrc_glScaled = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glScaled");
	if(!sfg_ptrc_glScaled) numFailed++;
	sfg_ptrc_glScalef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glScalef");
	if(!sfg_ptrc_glScalef) numFailed++;
	sfg_ptrc_glScissor = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glScissor");
	if(!sfg_ptrc_glScissor) numFailed++;
	sfg_ptrc_glSelectBuffer = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glSelectBuffer");
	if(!sfg_ptrc_glSelectBuffer) numFailed++;
	sfg_ptrc_glShadeModel = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glShadeModel");
	if(!sfg_ptrc_glShadeModel) numFailed++;
	sfg_ptrc_glStencilFunc = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLuint))IntGetProcAddress("glStencilFunc");
	if(!sfg_ptrc_glStencilFunc) numFailed++;
	sfg_ptrc_glStencilMask = (void (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glStencilMask");
	if(!sfg_ptrc_glStencilMask) numFailed++;
	sfg_ptrc_glStencilOp = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLenum))IntGetProcAddress("glStencilOp");
	if(!sfg_ptrc_glStencilOp) numFailed++;
	sfg_ptrc_glTexCoord1d = (void (CODEGEN_FUNCPTR *)(GLdouble))IntGetProcAddress("glTexCoord1d");
	if(!sfg_ptrc_glTexCoord1d) numFailed++;
	sfg_ptrc_glTexCoord1dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord1dv");
	if(!sfg_ptrc_glTexCoord1dv) numFailed++;
	sfg_ptrc_glTexCoord1f = (void (CODEGEN_FUNCPTR *)(GLfloat))IntGetProcAddress("glTexCoord1f");
	if(!sfg_ptrc_glTexCoord1f) numFailed++;
	sfg_ptrc_glTexCoord1fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord1fv");
	if(!sfg_ptrc_glTexCoord1fv) numFailed++;
	sfg_ptrc_glTexCoord1i = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glTexCoord1i");
	if(!sfg_ptrc_glTexCoord1i) numFailed++;
	sfg_ptrc_glTexCoord1iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord1iv");
	if(!sfg_ptrc_glTexCoord1iv) numFailed++;
	sfg_ptrc_glTexCoord1s = (void (CODEGEN_FUNCPTR *)(GLshort))IntGetProcAddress("glTexCoord1s");
	if(!sfg_ptrc_glTexCoord1s) numFailed++;
	sfg_ptrc_glTexCoord1sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord1sv");
	if(!sfg_ptrc_glTexCoord1sv) numFailed++;
	sfg_ptrc_glTexCoord2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glTexCoord2d");
	if(!sfg_ptrc_glTexCoord2d) numFailed++;
	sfg_ptrc_glTexCoord2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord2dv");
	if(!sfg_ptrc_glTexCoord2dv) numFailed++;
	sfg_ptrc_glTexCoord2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glTexCoord2f");
	if(!sfg_ptrc_glTexCoord2f) numFailed++;
	sfg_ptrc_glTexCoord2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord2fv");
	if(!sfg_ptrc_glTexCoord2fv) numFailed++;
	sfg_ptrc_glTexCoord2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glTexCoord2i");
	if(!sfg_ptrc_glTexCoord2i) numFailed++;
	sfg_ptrc_glTexCoord2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord2iv");
	if(!sfg_ptrc_glTexCoord2iv) numFailed++;
	sfg_ptrc_glTexCoord2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glTexCoord2s");
	if(!sfg_ptrc_glTexCoord2s) numFailed++;
	sfg_ptrc_glTexCoord2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord2sv");
	if(!sfg_ptrc_glTexCoord2sv) numFailed++;
	sfg_ptrc_glTexCoord3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTexCoord3d");
	if(!sfg_ptrc_glTexCoord3d) numFailed++;
	sfg_ptrc_glTexCoord3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord3dv");
	if(!sfg_ptrc_glTexCoord3dv) numFailed++;
	sfg_ptrc_glTexCoord3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTexCoord3f");
	if(!sfg_ptrc_glTexCoord3f) numFailed++;
	sfg_ptrc_glTexCoord3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord3fv");
	if(!sfg_ptrc_glTexCoord3fv) numFailed++;
	sfg_ptrc_glTexCoord3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glTexCoord3i");
	if(!sfg_ptrc_glTexCoord3i) numFailed++;
	sfg_ptrc_glTexCoord3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord3iv");
	if(!sfg_ptrc_glTexCoord3iv) numFailed++;
	sfg_ptrc_glTexCoord3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glTexCoord3s");
	if(!sfg_ptrc_glTexCoord3s) numFailed++;
	sfg_ptrc_glTexCoord3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord3sv");
	if(!sfg_ptrc_glTexCoord3sv) numFailed++;
	sfg_ptrc_glTexCoord4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTexCoord4d");
	if(!sfg_ptrc_glTexCoord4d) numFailed++;
	sfg_ptrc_glTexCoord4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glTexCoord4dv");
	if(!sfg_ptrc_glTexCoord4dv) numFailed++;
	sfg_ptrc_glTexCoord4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTexCoord4f");
	if(!sfg_ptrc_glTexCoord4f) numFailed++;
	sfg_ptrc_glTexCoord4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glTexCoord4fv");
	if(!sfg_ptrc_glTexCoord4fv) numFailed++;
	sfg_ptrc_glTexCoord4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glTexCoord4i");
	if(!sfg_ptrc_glTexCoord4i) numFailed++;
	sfg_ptrc_glTexCoord4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glTexCoord4iv");
	if(!sfg_ptrc_glTexCoord4iv) numFailed++;
	sfg_ptrc_glTexCoord4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glTexCoord4s");
	if(!sfg_ptrc_glTexCoord4s) numFailed++;
	sfg_ptrc_glTexCoord4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glTexCoord4sv");
	if(!sfg_ptrc_glTexCoord4sv) numFailed++;
	sfg_ptrc_glTexEnvf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexEnvf");
	if(!sfg_ptrc_glTexEnvf) numFailed++;
	sfg_ptrc_glTexEnvfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexEnvfv");
	if(!sfg_ptrc_glTexEnvfv) numFailed++;
	sfg_ptrc_glTexEnvi = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexEnvi");
	if(!sfg_ptrc_glTexEnvi) numFailed++;
	sfg_ptrc_glTexEnviv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexEnviv");
	if(!sfg_ptrc_glTexEnviv) numFailed++;
	sfg_ptrc_glTexGend = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLdouble))IntGetProcAddress("glTexGend");
	if(!sfg_ptrc_glTexGend) numFailed++;
	sfg_ptrc_glTexGendv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLdouble *))IntGetProcAddress("glTexGendv");
	if(!sfg_ptrc_glTexGendv) numFailed++;
	sfg_ptrc_glTexGenf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexGenf");
	if(!sfg_ptrc_glTexGenf) numFailed++;
	sfg_ptrc_glTexGenfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexGenfv");
	if(!sfg_ptrc_glTexGenfv) numFailed++;
	sfg_ptrc_glTexGeni = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexGeni");
	if(!sfg_ptrc_glTexGeni) numFailed++;
	sfg_ptrc_glTexGeniv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexGeniv");
	if(!sfg_ptrc_glTexGeniv) numFailed++;
	sfg_ptrc_glTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage1D");
	if(!sfg_ptrc_glTexImage1D) numFailed++;
	sfg_ptrc_glTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage2D");
	if(!sfg_ptrc_glTexImage2D) numFailed++;
	sfg_ptrc_glTexParameterf = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLfloat))IntGetProcAddress("glTexParameterf");
	if(!sfg_ptrc_glTexParameterf) numFailed++;
	sfg_ptrc_glTexParameterfv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLfloat *))IntGetProcAddress("glTexParameterfv");
	if(!sfg_ptrc_glTexParameterfv) numFailed++;
	sfg_ptrc_glTexParameteri = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, GLint))IntGetProcAddress("glTexParameteri");
	if(!sfg_ptrc_glTexParameteri) numFailed++;
	sfg_ptrc_glTexParameteriv = (void (CODEGEN_FUNCPTR *)(GLenum, GLenum, const GLint *))IntGetProcAddress("glTexParameteriv");
	if(!sfg_ptrc_glTexParameteriv) numFailed++;
	sfg_ptrc_glTranslated = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glTranslated");
	if(!sfg_ptrc_glTranslated) numFailed++;
	sfg_ptrc_glTranslatef = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glTranslatef");
	if(!sfg_ptrc_glTranslatef) numFailed++;
	sfg_ptrc_glVertex2d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble))IntGetProcAddress("glVertex2d");
	if(!sfg_ptrc_glVertex2d) numFailed++;
	sfg_ptrc_glVertex2dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex2dv");
	if(!sfg_ptrc_glVertex2dv) numFailed++;
	sfg_ptrc_glVertex2f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glVertex2f");
	if(!sfg_ptrc_glVertex2f) numFailed++;
	sfg_ptrc_glVertex2fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex2fv");
	if(!sfg_ptrc_glVertex2fv) numFailed++;
	sfg_ptrc_glVertex2i = (void (CODEGEN_FUNCPTR *)(GLint, GLint))IntGetProcAddress("glVertex2i");
	if(!sfg_ptrc_glVertex2i) numFailed++;
	sfg_ptrc_glVertex2iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex2iv");
	if(!sfg_ptrc_glVertex2iv) numFailed++;
	sfg_ptrc_glVertex2s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort))IntGetProcAddress("glVertex2s");
	if(!sfg_ptrc_glVertex2s) numFailed++;
	sfg_ptrc_glVertex2sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex2sv");
	if(!sfg_ptrc_glVertex2sv) numFailed++;
	sfg_ptrc_glVertex3d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertex3d");
	if(!sfg_ptrc_glVertex3d) numFailed++;
	sfg_ptrc_glVertex3dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex3dv");
	if(!sfg_ptrc_glVertex3dv) numFailed++;
	sfg_ptrc_glVertex3f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertex3f");
	if(!sfg_ptrc_glVertex3f) numFailed++;
	sfg_ptrc_glVertex3fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex3fv");
	if(!sfg_ptrc_glVertex3fv) numFailed++;
	sfg_ptrc_glVertex3i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint))IntGetProcAddress("glVertex3i");
	if(!sfg_ptrc_glVertex3i) numFailed++;
	sfg_ptrc_glVertex3iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex3iv");
	if(!sfg_ptrc_glVertex3iv) numFailed++;
	sfg_ptrc_glVertex3s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort))IntGetProcAddress("glVertex3s");
	if(!sfg_ptrc_glVertex3s) numFailed++;
	sfg_ptrc_glVertex3sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex3sv");
	if(!sfg_ptrc_glVertex3sv) numFailed++;
	sfg_ptrc_glVertex4d = (void (CODEGEN_FUNCPTR *)(GLdouble, GLdouble, GLdouble, GLdouble))IntGetProcAddress("glVertex4d");
	if(!sfg_ptrc_glVertex4d) numFailed++;
	sfg_ptrc_glVertex4dv = (void (CODEGEN_FUNCPTR *)(const GLdouble *))IntGetProcAddress("glVertex4dv");
	if(!sfg_ptrc_glVertex4dv) numFailed++;
	sfg_ptrc_glVertex4f = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glVertex4f");
	if(!sfg_ptrc_glVertex4f) numFailed++;
	sfg_ptrc_glVertex4fv = (void (CODEGEN_FUNCPTR *)(const GLfloat *))IntGetProcAddress("glVertex4fv");
	if(!sfg_ptrc_glVertex4fv) numFailed++;
	sfg_ptrc_glVertex4i = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLint, GLint))IntGetProcAddress("glVertex4i");
	if(!sfg_ptrc_glVertex4i) numFailed++;
	sfg_ptrc_glVertex4iv = (void (CODEGEN_FUNCPTR *)(const GLint *))IntGetProcAddress("glVertex4iv");
	if(!sfg_ptrc_glVertex4iv) numFailed++;
	sfg_ptrc_glVertex4s = (void (CODEGEN_FUNCPTR *)(GLshort, GLshort, GLshort, GLshort))IntGetProcAddress("glVertex4s");
	if(!sfg_ptrc_glVertex4s) numFailed++;
	sfg_ptrc_glVertex4sv = (void (CODEGEN_FUNCPTR *)(const GLshort *))IntGetProcAddress("glVertex4sv");
	if(!sfg_ptrc_glVertex4sv) numFailed++;
	sfg_ptrc_glViewport = (void (CODEGEN_FUNCPTR *)(GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glViewport");
	if(!sfg_ptrc_glViewport) numFailed++;
	sfg_ptrc_glAreTexturesResident = (GLboolean (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *, GLboolean *))IntGetProcAddress("glAreTexturesResident");
	if(!sfg_ptrc_glAreTexturesResident) numFailed++;
	sfg_ptrc_glArrayElement = (void (CODEGEN_FUNCPTR *)(GLint))IntGetProcAddress("glArrayElement");
	if(!sfg_ptrc_glArrayElement) numFailed++;
	sfg_ptrc_glBindTexture = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint))IntGetProcAddress("glBindTexture");
	if(!sfg_ptrc_glBindTexture) numFailed++;
	sfg_ptrc_glColorPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glColorPointer");
	if(!sfg_ptrc_glColorPointer) numFailed++;
	sfg_ptrc_glCopyTexImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint))IntGetProcAddress("glCopyTexImage1D");
	if(!sfg_ptrc_glCopyTexImage1D) numFailed++;
	sfg_ptrc_glCopyTexImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint))IntGetProcAddress("glCopyTexImage2D");
	if(!sfg_ptrc_glCopyTexImage2D) numFailed++;
	sfg_ptrc_glCopyTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei))IntGetProcAddress("glCopyTexSubImage1D");
	if(!sfg_ptrc_glCopyTexSubImage1D) numFailed++;
	sfg_ptrc_glCopyTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage2D");
	if(!sfg_ptrc_glCopyTexSubImage2D) numFailed++;
	sfg_ptrc_glDeleteTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *))IntGetProcAddress("glDeleteTextures");
	if(!sfg_ptrc_glDeleteTextures) numFailed++;
	sfg_ptrc_glDisableClientState = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glDisableClientState");
	if(!sfg_ptrc_glDisableClientState) numFailed++;
	sfg_ptrc_glDrawArrays = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLsizei))IntGetProcAddress("glDrawArrays");
	if(!sfg_ptrc_glDrawArrays) numFailed++;
	sfg_ptrc_glDrawElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawElements");
	if(!sfg_ptrc_glDrawElements) numFailed++;
	sfg_ptrc_glEdgeFlagPointer = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLvoid *))IntGetProcAddress("glEdgeFlagPointer");
	if(!sfg_ptrc_glEdgeFlagPointer) numFailed++;
	sfg_ptrc_glEnableClientState = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glEnableClientState");
	if(!sfg_ptrc_glEnableClientState) numFailed++;
	sfg_ptrc_glGenTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, GLuint *))IntGetProcAddress("glGenTextures");
	if(!sfg_ptrc_glGenTextures) numFailed++;
	sfg_ptrc_glGetPointerv = (void (CODEGEN_FUNCPTR *)(GLenum, GLvoid **))IntGetProcAddress("glGetPointerv");
	if(!sfg_ptrc_glGetPointerv) numFailed++;
	sfg_ptrc_glIndexPointer = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glIndexPointer");
	if(!sfg_ptrc_glIndexPointer) numFailed++;
	sfg_ptrc_glIndexub = (void (CODEGEN_FUNCPTR *)(GLubyte))IntGetProcAddress("glIndexub");
	if(!sfg_ptrc_glIndexub) numFailed++;
	sfg_ptrc_glIndexubv = (void (CODEGEN_FUNCPTR *)(const GLubyte *))IntGetProcAddress("glIndexubv");
	if(!sfg_ptrc_glIndexubv) numFailed++;
	sfg_ptrc_glInterleavedArrays = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glInterleavedArrays");
	if(!sfg_ptrc_glInterleavedArrays) numFailed++;
	sfg_ptrc_glIsTexture = (GLboolean (CODEGEN_FUNCPTR *)(GLuint))IntGetProcAddress("glIsTexture");
	if(!sfg_ptrc_glIsTexture) numFailed++;
	sfg_ptrc_glNormalPointer = (void (CODEGEN_FUNCPTR *)(GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glNormalPointer");
	if(!sfg_ptrc_glNormalPointer) numFailed++;
	sfg_ptrc_glPolygonOffset = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat))IntGetProcAddress("glPolygonOffset");
	if(!sfg_ptrc_glPolygonOffset) numFailed++;
	sfg_ptrc_glPopClientAttrib = (void (CODEGEN_FUNCPTR *)(void))IntGetProcAddress("glPopClientAttrib");
	if(!sfg_ptrc_glPopClientAttrib) numFailed++;
	sfg_ptrc_glPrioritizeTextures = (void (CODEGEN_FUNCPTR *)(GLsizei, const GLuint *, const GLfloat *))IntGetProcAddress("glPrioritizeTextures");
	if(!sfg_ptrc_glPrioritizeTextures) numFailed++;
	sfg_ptrc_glPushClientAttrib = (void (CODEGEN_FUNCPTR *)(GLbitfield))IntGetProcAddress("glPushClientAttrib");
	if(!sfg_ptrc_glPushClientAttrib) numFailed++;
	sfg_ptrc_glTexCoordPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glTexCoordPointer");
	if(!sfg_ptrc_glTexCoordPointer) numFailed++;
	sfg_ptrc_glTexSubImage1D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage1D");
	if(!sfg_ptrc_glTexSubImage1D) numFailed++;
	sfg_ptrc_glTexSubImage2D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage2D");
	if(!sfg_ptrc_glTexSubImage2D) numFailed++;
	sfg_ptrc_glVertexPointer = (void (CODEGEN_FUNCPTR *)(GLint, GLenum, GLsizei, const GLvoid *))IntGetProcAddress("glVertexPointer");
	if(!sfg_ptrc_glVertexPointer) numFailed++;
	sfg_ptrc_glBlendColor = (void (CODEGEN_FUNCPTR *)(GLfloat, GLfloat, GLfloat, GLfloat))IntGetProcAddress("glBlendColor");
	if(!sfg_ptrc_glBlendColor) numFailed++;
	sfg_ptrc_glBlendEquation = (void (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glBlendEquation");
	if(!sfg_ptrc_glBlendEquation) numFailed++;
	sfg_ptrc_glCopyTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))IntGetProcAddress("glCopyTexSubImage3D");
	if(!sfg_ptrc_glCopyTexSubImage3D) numFailed++;
	sfg_ptrc_glDrawRangeElements = (void (CODEGEN_FUNCPTR *)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *))IntGetProcAddress("glDrawRangeElements");
	if(!sfg_ptrc_glDrawRangeElements) numFailed++;
	sfg_ptrc_glTexImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexImage3D");
	if(!sfg_ptrc_glTexImage3D) numFailed++;
	sfg_ptrc_glTexSubImage3D = (void (CODEGEN_FUNCPTR *)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))IntGetProcAddress("glTexSubImage3D");
	if(!sfg_ptrc_glTexSubImage3D) numFailed++;
	return numFailed;
}

typedef int (*PFN_LOADFUNCPOINTERS)(void);
typedef struct sfgogl_StrToExtMap_s
{
	const char *extensionName;
	int *extensionVariable;
	PFN_LOADFUNCPOINTERS LoadExtension;
} sfgogl_StrToExtMap;

static sfgogl_StrToExtMap ExtensionMap[19] = {
	{"GL_SGIS_texture_edge_clamp", &sfgogl_ext_SGIS_texture_edge_clamp, NULL},
	{"GL_ARB_multitexture", &sfgogl_ext_ARB_multitexture, Load_ARB_multitexture},
	{"GL_EXT_blend_minmax", &sfgogl_ext_EXT_blend_minmax, Load_EXT_blend_minmax},
	{"GL_EXT_blend_subtract", &sfgogl_ext_EXT_blend_subtract, NULL},
	{"GL_EXT_blend_func_separate", &sfgogl_ext_EXT_blend_func_separate, Load_EXT_blend_func_separate},
	{"GL_ARB_vertex_buffer_object", &sfgogl_ext_ARB_vertex_buffer_object, Load_ARB_vertex_buffer_object},
	{"GL_ARB_vertex_program", &sfgogl_ext_ARB_vertex_program, Load_ARB_vertex_program},
	{"GL_ARB_fragment_program", &sfgogl_ext_ARB_fragment_program, Load_ARB_fragment_program},
	{"GL_ARB_shading_language_100", &sfgogl_ext_ARB_shading_language_100, NULL},
	{"GL_ARB_shader_objects", &sfgogl_ext_ARB_shader_objects, Load_ARB_shader_objects},
	{"GL_ARB_vertex_shader", &sfgogl_ext_ARB_vertex_shader, Load_ARB_vertex_shader},
	{"GL_ARB_fragment_shader", &sfgogl_ext_ARB_fragment_shader, NULL},
	{"GL_ARB_texture_non_power_of_two", &sfgogl_ext_ARB_texture_non_power_of_two, NULL},
	{"GL_EXT_blend_equation_separate", &sfgogl_ext_EXT_blend_equation_separate, Load_EXT_blend_equation_separate},
	{"GL_EXT_framebuffer_object", &sfgogl_ext_EXT_framebuffer_object, Load_EXT_framebuffer_object},
	{"GL_ARB_vertex_array_object", &sfgogl_ext_ARB_vertex_array_object, Load_ARB_vertex_array_object},
	{"GL_ARB_geometry_shader4", &sfgogl_ext_ARB_geometry_shader4, Load_ARB_geometry_shader4},
	{"GL_ARB_explicit_attrib_location", &sfgogl_ext_ARB_explicit_attrib_location, NULL},
	{"GL_ARB_explicit_uniform_location", &sfgogl_ext_ARB_explicit_uniform_location, NULL}
};

static int g_extensionMapSize = 19;

static sfgogl_StrToExtMap *FindExtEntry(const char *extensionName)
{
	int loop;
	sfgogl_StrToExtMap *currLoc = ExtensionMap;
	for(loop = 0; loop < g_extensionMapSize; ++loop, ++currLoc)
	{
		if(strcmp(extensionName, currLoc->extensionName) == 0)
			return currLoc;
	}

	return NULL;
}

static void ClearExtensionVars(void)
{
	sfgogl_ext_SGIS_texture_edge_clamp = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_multitexture = sfgogl_LOAD_FAILED;
	sfgogl_ext_EXT_blend_minmax = sfgogl_LOAD_FAILED;
	sfgogl_ext_EXT_blend_subtract = sfgogl_LOAD_FAILED;
	sfgogl_ext_EXT_blend_func_separate = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_vertex_buffer_object = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_vertex_program = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_fragment_program = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_shading_language_100 = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_shader_objects = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_vertex_shader = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_fragment_shader = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_texture_non_power_of_two = sfgogl_LOAD_FAILED;
	sfgogl_ext_EXT_blend_equation_separate = sfgogl_LOAD_FAILED;
	sfgogl_ext_EXT_framebuffer_object = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_vertex_array_object = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_geometry_shader4 = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_explicit_attrib_location = sfgogl_LOAD_FAILED;
	sfgogl_ext_ARB_explicit_uniform_location = sfgogl_LOAD_FAILED;
}


static void LoadExtByName(const char *extensionName)
{
	sfgogl_StrToExtMap *entry = NULL;
	entry = FindExtEntry(extensionName);
	if(entry)
	{
		if(entry->LoadExtension)
		{
			int numFailed = entry->LoadExtension();
			if(numFailed == 0)
			{
				*(entry->extensionVariable) = sfgogl_LOAD_SUCCEEDED;
			}
			else
			{
				*(entry->extensionVariable) = sfgogl_LOAD_SUCCEEDED + numFailed;
			}
		}
		else
		{
			*(entry->extensionVariable) = sfgogl_LOAD_SUCCEEDED;
		}
	}
}


static void ProcExtsFromExtString(const char *strExtList)
{
	size_t iExtListLen = strlen(strExtList);
	const char *strExtListEnd = strExtList + iExtListLen;
	const char *strCurrPos = strExtList;
	char strWorkBuff[256];

	while(*strCurrPos)
	{
		/*Get the extension at our position.*/
		int iStrLen = 0;
		const char *strEndStr = strchr(strCurrPos, ' ');
		int iStop = 0;
		if(strEndStr == NULL)
		{
			strEndStr = strExtListEnd;
			iStop = 1;
		}

		iStrLen = (int)((ptrdiff_t)strEndStr - (ptrdiff_t)strCurrPos);

		if(iStrLen > 255)
			return;

		strncpy(strWorkBuff, strCurrPos, static_cast<size_t>(iStrLen));
		strWorkBuff[iStrLen] = '\0';

		LoadExtByName(strWorkBuff);

		strCurrPos = strEndStr + 1;
		if(iStop) break;
	}
}

int sfgogl_LoadFunctions(void)
{
	int numFailed = 0;
	ClearExtensionVars();

	sfg_ptrc_glGetString = (const GLubyte * (CODEGEN_FUNCPTR *)(GLenum))IntGetProcAddress("glGetString");
	if(!sfg_ptrc_glGetString) return sfgogl_LOAD_FAILED;

	ProcExtsFromExtString((const char *)sfg_ptrc_glGetString(GL_EXTENSIONS));
	numFailed = Load_Version_1_2();

	if(numFailed == 0)
		return sfgogl_LOAD_SUCCEEDED;
	else
		return sfgogl_LOAD_SUCCEEDED + numFailed;
}

static int g_major_version = 0;
static int g_minor_version = 0;

static void ParseVersionFromString(int *pOutMajor, int *pOutMinor, const char *strVersion)
{
	const char *strDotPos = NULL;
	int iLength = 0;
	char strWorkBuff[10];
	*pOutMinor = 0;
	*pOutMajor = 0;

	strDotPos = strchr(strVersion, '.');
	if(!strDotPos)
		return;

	iLength = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
	strncpy(strWorkBuff, strVersion, static_cast<size_t>(iLength));
	strWorkBuff[iLength] = '\0';

	*pOutMajor = atoi(strWorkBuff);
	strDotPos = strchr(strVersion + iLength + 1, ' ');
	if(!strDotPos)
	{
		/*No extra data. Take the whole rest of the string.*/
		strcpy(strWorkBuff, strVersion + iLength + 1);
	}
	else
	{
		/*Copy only up until the space.*/
		int iLengthMinor = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
		iLengthMinor = iLengthMinor - (iLength + 1);
		strncpy(strWorkBuff, strVersion + iLength + 1, static_cast<size_t>(iLengthMinor));
		strWorkBuff[iLengthMinor] = '\0';
	}

	*pOutMinor = atoi(strWorkBuff);
}

static void GetGLVersion(void)
{
	ParseVersionFromString(&g_major_version, &g_minor_version, (const char*)glGetString(GL_VERSION));
}

int sfgogl_GetMajorVersion(void)
{
	if(g_major_version == 0)
		GetGLVersion();
	return g_major_version;
}

int sfgogl_GetMinorVersion(void)
{
	if(g_major_version == 0) /* Yes, check the major version to get the minor one. */
		GetGLVersion();
	return g_minor_version;
}

int sfgogl_IsVersionGEQ(int majorVersion, int minorVersion)
{
	if(g_major_version == 0)
		GetGLVersion();

	if(majorVersion > g_major_version) return 0;
	if(majorVersion < g_major_version) return 1;
	if(g_minor_version >= minorVersion) return 1;
	return 0;
}

