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
    /*  struct VirtualDevice{ */
    VkDeviceQueueCreateInfo queue_info;
    uint32_t queue_family_count;
    std::vector<VkQueueFamilyProperties> queue_properties;
    VkDevice virtualDevice;
    VkResult enumerated_physical_device_res = VK_RESULT_MAX_ENUM;
    VkResult enumerateGPU_res = VK_RESULT_MAX_ENUM;
    /* }; */

    /* struct PhysicalDevice {*/
    VkApplicationInfo app_info = {};
    VkInstanceCreateInfo inst_info = {};
    std::vector<VkPhysicalDevice> gpus;
    VkDeviceCreateInfo physical_device_info;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkInstance inst;
    /*   }; */
public:


    bool isVulkanReady() const;

    VulkanSetup();

    ~VulkanSetup();

    int countGPUs() const;


    VkCommandBuffer commandBuffer;
    VkCommandPool cmd_pool;
    VkCommandPoolCreateInfo cmd_pool_info;


    VkResult createDevice_res = VK_RESULT_MAX_ENUM;
    VkResult res = VK_RESULT_MAX_ENUM;
    VkResult command_pool_res = VK_RESULT_MAX_ENUM;
    VkResult command_buffer_res = VK_RESULT_MAX_ENUM;
    float queue_priorities[1] = {0.0};


private:
    void post_init_setup();

    bool get_queue_families();

    void createVirtualDevice();

    void destroyVirtualDevice();

    void createCommandPool();

    // FIELDS
private:
    VkCommandBufferAllocateInfo cmd;

    void createCommandBuffer();

    void destroyCommandBuffers();

};


#endif //VULKANWORKSHOP_VULKANSETUP_H
