//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_VULKANSETUP_H
#define VULKANWORKSHOP_VULKANSETUP_H
#define VK_USE_PLATFORM_WIN32_KHR
//#include "sdk/sdkut.h"
#include <vector>
#include <string>
#include "vulkan/vulkan.h"


#define APP_SHORT_NAME "Terrific"
#define ENGINE_SHORT_NAME "TerrificEngine"

class ApplicationContext;
class VulkanSetup {
public:
    /** struct  extensions */
    std::vector<const char *> device_extension_names;
    std::vector<const char *> instance_extension_names;

    /** layer properties */
    VkLayerProperties properties;
    std::vector<VkExtensionProperties> extensions;

    /*  struct VirtualDevice{ */
    VkDeviceQueueCreateInfo queue_info;
    uint32_t queue_family_count;
    std::vector<VkQueueFamilyProperties> queue_properties;
    VkDevice virtualDevice;
    VkResult enumerated_physical_device_res = VK_RESULT_MAX_ENUM;
    VkResult enumerateGPU_res = VK_RESULT_MAX_ENUM;
    unsigned present_queue_family_index;
    /* }; */

    /* struct PhysicalDevice {*/
    VkApplicationInfo app_info = {};
    VkInstanceCreateInfo inst_info = {};
    std::vector<VkPhysicalDevice> gpus;
    VkDeviceCreateInfo physical_device_info;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkInstance inst;


    /*   }; */
    void init();

public:


    bool isVulkanReady() const;

    VulkanSetup(const char *applicationName, const char *engineName);

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

    unsigned getSwapChainSize();

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

    void createSwapChain();

    std::string application_name;
    std::string engine_name;

};


class Window {
public:
    HWND window;
    HINSTANCE connection;
    /** window / surface / */
    VkSurfaceKHR surface;
    Window(int width, int height, const char *name);
    void init(ApplicationContext *applicationContext);
private:
    std::string name;
    int height;
    int width;

    void createSurface(ApplicationContext *applicationContext);

    void checkQueueSupportPresenting(ApplicationContext *applicationContext);
};

class ApplicationContext {

    int counter = 0;
    Window window;
public:
    ApplicationContext();
    void wm_paint();
    void init();

    VulkanSetup vulkanSetup;
};

#endif //VULKANWORKSHOP_VULKANSETUP_H
