//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_VULKANSETUP_H
#define VULKANWORKSHOP_VULKANSETUP_H

#include "sdk/sdkut.h"
#include <iostream>
#include <vector>

#define APP_SHORT_NAME "VulkanWorkshop"
#define ENGINE_SHORT_NAME "TerrificEngine"
using namespace vksdk;

class VulkanSetup {
public:
    VkApplicationInfo app_info = {};
    VkInstanceCreateInfo inst_info = {};
    std::vector<VkPhysicalDevice> gpus;
    VkResult enumerateGPU_res;

    VkInstance inst;
    VkResult res;

    VulkanSetup();
    bool isVulkanReady() const;
    ~VulkanSetup();

    int countGPUs() const;
private:
    void post_init_setup();


};


#endif //VULKANWORKSHOP_VULKANSETUP_H
