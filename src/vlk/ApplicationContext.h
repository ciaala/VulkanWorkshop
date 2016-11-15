//
// Created by crypt on 11/13/2016.
//


#ifndef VULKANWORKSHOP_APPLICATIONCONTEXT_H
#define VULKANWORKSHOP_APPLICATIONCONTEXT_H

#include "vlk.h"
#include "Window.h"
#include "VulkanContext.h"

namespace vlk {
    class ApplicationContext {

        int counter = 0;
        Window window;
    public:
        VulkanContext vulkanContext;

        ApplicationContext();

        void wm_paint();

        void init();


    };

}
#endif //VULKANWORKSHOP_APPLICATIONCONTEXT_H
