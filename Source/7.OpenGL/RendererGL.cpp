#include "RendererGL.hpp"
#include "GL/glew.h"

RendererGL::RendererGL(SDL_Window *window)
    : window_(window)
{
}

bool RendererGL::Initialize()
{
    // core OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // version 4.2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    // RGBA
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // double buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    context_ = SDL_GL_CreateContext(window_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("failed to initialize GLEW");
        return false;
    }

    return true;
}

RendererGL::~RendererGL()
{
#ifdef _DEBUG
    SDL_Log("~RendererGL()");
#endif // _DEBUG
    if (context_)
    {
        DestoryRenderer();
    }
}

void RendererGL::DestoryRenderer()
{
    if (context_)
    {
        SDL_GL_DeleteContext(context_);
    }
}

int RendererGL::DrawTexture(ITextureAsset &tex, const RendererRect &rect, double angle)
{
    return -1;
}

int RendererGL::DrawTextureSimple(ITextureAsset &tex, const std::optional<RendererRect> &src, std::optional<RendererRect> dest)
{
    return -1;
}

void RendererGL::Present()
{
    SDL_GL_SwapWindow(window_);
}

void RendererGL::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void RendererGL::RenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    constexpr float kColor = 255.0;
    RenderColorFloat(r / kColor, g / kColor, b / kColor, a / kColor);
}

void RendererGL::RenderColorFloat(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
