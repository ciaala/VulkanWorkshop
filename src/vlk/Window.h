//
// Created by crypt on 11/13/2016.
//


#include "vlk.h"

#include "ApplicationContext.h"
#include <string>

#ifndef VULKANWORKSHOP_WINDOW_H
#define VULKANWORKSHOP_WINDOW_H

namespace vlk {

    class Window {
    public:
        struct HWND__ *window;
        struct HINSTANCE__ *connection;
        /** window / surface / */
        VkSurfaceKHR surface;

        Window(int width, int height, const char *name);

        void init(ApplicationContext *applicationContext);

    private:
        std::string name;
        int height;
        int width;

        VkResult init_surface_extension(ApplicationContext *applicationContext);

        void checkQueueSupportPresenting(ApplicationContext *applicationContext);
    };
}
#endif //VULKANWORKSHOP_WINDOW_H
