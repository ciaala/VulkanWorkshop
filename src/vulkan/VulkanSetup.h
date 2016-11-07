//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_VULKANSETUP_H
#define VULKANWORKSHOP_VULKANSETUP_H

#include "sdk/sdkut.h"
#include <vector>

#define APP_SHORT_NAME "VulkanWorkshop"
#define ENGINE_SHORT_NAME "TerrificEngine"
using namespace vksdk;

class VulkanSetup {
public:
    VkApplicationInfo app_info = {};
    VkInstanceCreateInfo inst_info = {};
    std::vector<VkPhysicalDevice> gpus;

    std::vector<VkQueueFamilyProperties> queue_properties;

    VkResult enumerateGPU_res;
    uint32_t queue_family_count;
    VkInstance inst;
    VkResult res;

    VulkanSetup();

    bool isVulkanReady() const;

    ~VulkanSetup();

    int countGPUs() const;

    VkDeviceQueueCreateInfo queue_info;
    VkPhysicalDeviceProperties physicalDeviceProperties;

    VkDeviceCreateInfo device_info;
    VkDevice virtualDevice;

    VkCommandBuffer commandBuffer;
    VkCommandPool cmd_pool;
    VkCommandPoolCreateInfo cmd_pool_info;
private:
    void post_init_setup();


    bool get_queue_families();

    void createVirtualDevice();

    VkResult createDevice_res;

    void destroyVirtualDevice();

    void createCommandPool();


    // FIELDS
private:
    VkCommandBufferAllocateInfo cmd;

    void createCommandBuffer();

    void destroyCommandBuffers();
};


#endif //VULKANWORKSHOP_VULKANSETUP_H
