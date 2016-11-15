//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_VULKANSETUP_H
#define VULKANWORKSHOP_VULKANSETUP_H

#include <vector>
#include <string>
#include "vlk.h"
#include "VulkanUtility.h"

#define APP_SHORT_NAME "Terrific"
#define ENGINE_SHORT_NAME "TerrificEngine"
using namespace std;
namespace vlk {


    class VulkanContext {
    public:
        /** struct  extensions */
        vector<const char *> device_extension_names;
        vector<const char *> instanceExtensionNames;

        /** layer properties */
        VkLayerProperties properties;
        vector<VkExtensionProperties> extensions;

        /*  struct VirtualDevice{ */
        VkDeviceQueueCreateInfo queue_info;
        uint32_t queue_family_count;
        vector<VkQueueFamilyProperties> queue_properties;
        VkDevice virtualDevice;
        VkResult enumerated_physical_device_res = VK_RESULT_MAX_ENUM;
        VkResult enumerateGPU_res = VK_RESULT_MAX_ENUM;
        unsigned present_queue_family_index;
        /* }; */

        /* struct PhysicalDevice {*/
        VkApplicationInfo app_info = {};
        VkInstanceCreateInfo inst_info = {};
        vector<VkPhysicalDevice> gpus;
        VkDeviceCreateInfo physical_device_info;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkInstance inst;

        vector<vlk_layer_properties> instanceLayerProperties;

        /*   }; */
        void init();

    public:
        PFN_vkCreateDebugReportCallbackEXT pfnCreateDebugReportCallbackEXT;
        PFN_vkDestroyDebugReportCallbackEXT pfnDestroyDebugReportCallbackEXT;

        bool isVulkanReady() const;

        VulkanContext(const char *applicationName, const char *engineName);

        ~VulkanContext();

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

        VkResult init_enumerate_device();

        bool get_queue_families();

        void createVirtualDevice();

        void destroyVirtualDevice();

        void createCommandPool();

        // FIELDS

        VkCommandBufferAllocateInfo cmd;

        void createCommandBuffer();

        void destroyCommandBuffers();

        void createSwapChain();

        string application_name;
        string engine_name;


        VkPhysicalDeviceMemoryProperties memory_properties;

        VkResult init_instance();

        void initExtensions();

        void init_debug_callback();

        VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo;
        VkDebugReportCallbackEXT debugReportCallback;

        void destroy_debug_report();

        vector<const char*> enabledInstanceLayers;
        vector<const char*> enabledInstanceExtensions;
    };

    VkBool32 vlkDebugCallback(VkDebugReportFlagsEXT flags,
                              VkDebugReportObjectTypeEXT objType,
                              uint64_t srcObj,
                              size_t location,
                              int32_t msgCode,
                              const char *layerPrefix,
                              const char *msg,
                              void *userData);
}
#endif //VULKANWORKSHOP_VULKANSETUP_H
