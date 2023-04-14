#include "Game.hpp"

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include <glm/glm.hpp>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <optional>

using SDL_WinPtr = SDL_Window *;

class Game::Impl
{
public:
    std::optional<SDL_WinPtr> optWindow;
    bool mIsRunning = true;
    // Use validation layers if this is a debug build
    std::vector<const char *> layers;
    // Create the Vulkan instance.
    vk::Instance instance;
    // Create a Vulkan surface for rendering
    VkSurfaceKHR c_surface;
};

Game::Game()
    : impl(std::make_unique<Game::Impl>())
{
#if defined(_DEBUG)
    impl->layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
}

Game::~Game() {}

bool Game::Initialize(const std::string &windowsName, int x, int y, int w, int h)
{
    // Create an SDL window that supports Vulkan rendering.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Could not initialize SDL.");
        return false;
    }

    impl->optWindow = SDL_CreateWindow(windowsName.c_str(), x,
                                       y, w, h, SDL_WINDOW_VULKAN);
    if (impl->optWindow == nullptr)
    {
        SDL_Log("Could not create SDL window.");
        return false;
    }

    // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
    unsigned extension_count;
    if (!SDL_Vulkan_GetInstanceExtensions(impl->optWindow.value(), &extension_count, nullptr))
    {
        SDL_Log("Could not get the number of required instance extensions from SDL.");
        return false;
    }

    std::vector<const char *> extensions(extension_count);
    if (!SDL_Vulkan_GetInstanceExtensions(impl->optWindow.value(), &extension_count, extensions.data()))
    {
        SDL_Log("Could not get the names of required instance extensions from SDL.");
        return false;
    }

    // vk::ApplicationInfo allows the programmer to specifiy some basic information about the
    // program, which can be useful for layers and tools to provide more debug information.
    vk::ApplicationInfo appInfo = vk::ApplicationInfo()
                                      .setPApplicationName("Vulkan C++ Windowed Program Template")
                                      .setApplicationVersion(1)
                                      .setPEngineName("LunarG SDK")
                                      .setEngineVersion(1)
                                      .setApiVersion(VK_API_VERSION_1_0);

    // vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
    // are needed.
    vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
                                          .setFlags(vk::InstanceCreateFlags())
                                          .setPApplicationInfo(&appInfo)
                                          .setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
                                          .setPpEnabledExtensionNames(extensions.data())
                                          .setEnabledLayerCount(static_cast<uint32_t>(impl->layers.size()))
                                          .setPpEnabledLayerNames(impl->layers.data());

    try
    {
        impl->instance = vk::createInstance(instInfo);
    }
    catch (const std::exception &e)
    {
        SDL_Log("Could not create a Vulkan instance: %s", e.what());
        return false;
    }

    if (!SDL_Vulkan_CreateSurface(impl->optWindow.value(), static_cast<VkInstance>(impl->instance), &impl->c_surface))
    {
        SDL_Log("Could not create a Vulkan surface.");
        return false;
    }

    return true;
}

bool Game::Initialize()
{
    return Initialize("Vulkan Environment", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768);
}

void Game::RunLoop()
{
    while (impl->mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    vkDestroySurfaceKHR(impl->instance, impl->c_surface, nullptr);
    // or impl->instance.destroy(); ?
    vkDestroyInstance(impl->instance, nullptr);
    SDL_DestroyWindow(impl->optWindow.value());
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            impl->mIsRunning = false;
            break;
        default:
            break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE])
    {
        impl->mIsRunning = false;
    }
}

void Game::UpdateGame()
{
}

void Game::GenerateOutput()
{
}
