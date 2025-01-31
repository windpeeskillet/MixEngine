﻿#include "../../MixEngine.h"
#include "MxWindow.h"
#include "../Exceptions/MxExceptions.hpp"
#include <stb_image/stb_image.h>
#include <map>
#include <iostream>

namespace Mix {
    Window* Window::Get() {
        return MixEngine::Instance().getModule<Window>();
    }

    Window::Window(const std::string& _title, const Vector2i& _size, Flags<WindowFlag> _windowFlag) {
        if (SDL_Init(SDL_INIT_VIDEO))
            MX_EXCEPT("Failed to initialize SDL2");

        mWindow = SDL_CreateWindow(_title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   _size.x,
                                   _size.y,
                                   ToSDLWindowFlags(_windowFlag));
        if (!mWindow)
            MX_EXCEPT("Failed to create window");
    }

    Window::~Window() {
        if (mWindow)
            SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    void Window::setIcon(const std::filesystem::path& _path) {
        const int format = STBI_rgb_alpha;
        int channels;
        glm::ivec2 size;
        auto data = static_cast<unsigned char*>(stbi_load(_path.string().c_str(),
                                                &size.x,
                                                &size.y,
                                                &channels,
                                                format));
        if (!data)
            return;

        SDL_Surface* icon = SDL_CreateRGBSurfaceWithFormatFrom(data,
                                                               size.x,
                                                               size.y,
                                                               32,
                                                               4 * size.x,
                                                               SDL_PIXELFORMAT_RGBA32);
        if (!icon) {
            stbi_image_free(data);
            return;
        }

        SDL_SetWindowIcon(mWindow, icon);
        SDL_FreeSurface(icon);
        stbi_image_free(data);
    }

    Vector2i Window::getDrawableSize() const {
        Vector2i size;
        if (mWindow)
            SDL_Vulkan_GetDrawableSize(mWindow, &size.x, &size.y);
        return size;
    }

    Vector2i Window::getExtent() const {
        Vector2i size;
        if (mWindow)
            SDL_GetWindowSize(mWindow, &size.x, &size.y);
        return size;
    }

    std::string Window::getTitle() const {
        if (mWindow)
            return SDL_GetWindowTitle(mWindow);
        return "";
    }

    void Window::setTitle(const std::string& _title) const {
        if (mWindow)
            SDL_SetWindowTitle(mWindow, _title.c_str());
    }

    std::vector<const char*> Window::getRequiredInstanceExts() const {
        unsigned int count;
        SDL_Vulkan_GetInstanceExtensions(mWindow, &count, nullptr);
        std::vector<const char*> result(count);
        SDL_Vulkan_GetInstanceExtensions(mWindow, &count, result.data());
        return result;
    }

    Uint32 Window::ToSDLWindowFlags(Flags<WindowFlag> _flags) {
        // Check invalid combination
        bool ok = true;
        if (_flags.isSet(WindowFlag::Hidden) && _flags.isSet(WindowFlag::Shown)) {
            std::cout << "WindowFlag: Combination of HIDDEN and SHOWN is invalid. HIDDEN will be unset." << std::endl;
            _flags.unset(WindowFlag::Hidden);
        }

        if (_flags.isSet(WindowFlag::Minimized) && _flags.isSet(WindowFlag::Maximized)) {
            std::cout << "WindowFlag: Combination of MINIMIZED and MAXIMIZED is invalid. MINIMIZED will be unset." << std::endl;
            _flags.unset(WindowFlag::Hidden);
        }

        static std::map<WindowFlag, Uint32> mapToSDlWindowFlag = {
            {WindowFlag::Fullscreen,	SDL_WINDOW_FULLSCREEN},
            {WindowFlag::Shown,			SDL_WINDOW_SHOWN},
            {WindowFlag::Hidden,		SDL_WINDOW_HIDDEN},
            {WindowFlag::Borderless,	SDL_WINDOW_BORDERLESS},
            {WindowFlag::Resizable,		SDL_WINDOW_RESIZABLE},
            {WindowFlag::Minimized,		SDL_WINDOW_MINIMIZED},
            {WindowFlag::Maximized,		SDL_WINDOW_MAXIMIZED},
            {WindowFlag::Vulkan,		SDL_WINDOW_VULKAN},
            {WindowFlag::FullScreen_Desktop, SDL_WINDOW_FULLSCREEN_DESKTOP}
        };

        Uint32 result = 0;
        for (auto pair : mapToSDlWindowFlag) {
            if (_flags.isSet(pair.first))
                result |= pair.second;
        }

        return result;
    }
}
