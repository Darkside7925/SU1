#pragma once

#include <cstdint>

// Minimal EGL types and constants for compilation
typedef void* EGLDisplay;
typedef void* EGLContext;
typedef void* EGLSurface;
typedef void* EGLConfig;
typedef intptr_t EGLNativeDisplayType;
typedef intptr_t EGLNativePixmapType;
typedef intptr_t EGLNativeWindowType;
typedef int32_t EGLint;
typedef unsigned int EGLBoolean;
typedef unsigned int EGLenum;

// EGL constants
#define EGL_FALSE 0
#define EGL_TRUE 1
#define EGL_NONE 0x3038
#define EGL_NO_DISPLAY ((EGLDisplay)0)
#define EGL_NO_CONTEXT ((EGLContext)0)
#define EGL_NO_SURFACE ((EGLSurface)0)
#define EGL_DEFAULT_DISPLAY ((EGLNativeDisplayType)0)

// Display and surface attributes
#define EGL_SURFACE_TYPE 0x3033
#define EGL_WINDOW_BIT 0x0004
#define EGL_RED_SIZE 0x3024
#define EGL_GREEN_SIZE 0x3023
#define EGL_BLUE_SIZE 0x3022
#define EGL_ALPHA_SIZE 0x3021
#define EGL_DEPTH_SIZE 0x3025
#define EGL_STENCIL_SIZE 0x3026
#define EGL_SAMPLES 0x3031
#define EGL_RENDERABLE_TYPE 0x3040
#define EGL_OPENGL_ES2_BIT 0x0004

// Context attributes
#define EGL_CONTEXT_CLIENT_VERSION 0x3098

// Platform extensions
#define EGL_PLATFORM_GBM_MESA 0x31D7

// Function declarations (minimal stubs)
extern "C" {
EGLDisplay eglGetPlatformDisplay(EGLenum platform, void* native_display, const EGLint* attrib_list);
EGLBoolean eglInitialize(EGLDisplay dpy, EGLint* major, EGLint* minor);
EGLBoolean eglChooseConfig(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint* attrib_list);
EGLSurface eglCreatePlatformWindowSurface(EGLDisplay dpy, EGLConfig config, void* native_window, const EGLint* attrib_list);
EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
EGLBoolean eglSwapBuffers(EGLDisplay dpy, EGLSurface surface);
EGLBoolean eglDestroySurface(EGLDisplay dpy, EGLSurface surface);
EGLBoolean eglDestroyContext(EGLDisplay dpy, EGLContext ctx);
EGLBoolean eglTerminate(EGLDisplay dpy);
}
