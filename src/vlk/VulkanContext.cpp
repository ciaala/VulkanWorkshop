//
// Created by crypt on 29/06/16.
//

#include "VulkanContext.h"
#include <assert.h>
#include <iostream>

using namespace std;
namespace vlk {
    VulkanContext::VulkanContext(const char *application_name, const char *engine_name) {
        this->application_name = application_name;
        this->engine_name = engine_name;

    }

    VkResult VulkanContext::init_instance() {

        /* VULKAN_KEY_START */

        // initialize the VkApplicationInfo structure

        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = NULL;
        app_info.pApplicationName = APP_SHORT_NAME;
        app_info.applicationVersion = 1;
        app_info.pEngineName = ENGINE_SHORT_NAME;
        app_info.engineVersion = 1;
        app_info.apiVersion = VK_API_VERSION_1_0;

        // initialize the VkInstanceCreateInfo structure

        enabledInstanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
        enabledInstanceExtensions.push_back("VK_KHR_surface");
        enabledInstanceExtensions.push_back("VK_KHR_win32_surface");
        enabledInstanceExtensions.push_back("VK_EXT_debug_report");



        inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        inst_info.pNext = NULL;
        inst_info.flags = 0;
        inst_info.pApplicationInfo = &app_info;
        inst_info.enabledExtensionCount = static_cast<uint32_t>(this->enabledInstanceExtensions.size());
        inst_info.ppEnabledExtensionNames = this->enabledInstanceExtensions.data();
        inst_info.enabledLayerCount = static_cast<uint32_t>(this->enabledInstanceLayers.size());
        inst_info.ppEnabledLayerNames = this->enabledInstanceLayers.data();


        res = vkCreateInstance(&inst_info, NULL, &inst);
        if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
            std::cout << "cannot find a compatible Vulkan ICD" << endl;
            exit(-1);
        } else if (res) {
            std::cout << "unknown error" << endl;
            exit(-1);
        } else {
            return VK_SUCCESS;
        }

    }

    void VulkanContext::init_debug_callback() {

        this->pfnCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
                vkGetInstanceProcAddr(this->inst, "vkCreateDebugReportCallbackEXT"));
        this->pfnDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
                vkGetInstanceProcAddr(this->inst, "vkDestroyDebugReportCallbackEXT"));
        if (this->pfnCreateDebugReportCallbackEXT && this->pfnDestroyDebugReportCallbackEXT) {
            cout << "Initialization completed !" << endl;
        } else {
            cerr << "Initialization failed !" << endl;
            exit(0);
        }
        debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debugReportCreateInfo.flags =
                VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT |
                VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
        debugReportCreateInfo.pfnCallback = vlkDebugCallback;
        debugReportCreateInfo.pUserData = this;
        pfnCreateDebugReportCallbackEXT(this->inst, &this->debugReportCreateInfo, nullptr, &this->debugReportCallback);
    }

    void VulkanContext::init() {
        if (VK_SUCCESS != VulkanUtility::init_global_layer_properties(this->instanceLayerProperties)) {
            cerr << "Unable to setup Vulkan !!!" << endl;
            return;
        }
        VulkanUtility::init_instance_extension_names(this->instanceExtensionNames);
        VulkanUtility::init_device_extension_names(this->instanceExtensionNames);

        if (init_instance() == VK_SUCCESS) {
            init_debug_callback();
            init_enumerate_device();
        }

        if (1 == 3 && get_queue_families()) {
            createVirtualDevice();
            createCommandPool();
            createCommandBuffer();
//            createSwapChain();
        } else {
            cerr << "No queue found" << endl;
        }

    }


    VulkanContext::~VulkanContext() {
        this->destroyCommandBuffers();
        this->destroyVirtualDevice();
        vkDestroyInstance(inst, NULL);
    }

    bool VulkanContext::isVulkanReady() const {
        return this->res == VK_SUCCESS;
    }

    VkResult VulkanContext::init_enumerate_device() {
        uint32_t gpu_count = 0;
        this->enumerated_physical_device_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, NULL);
        assert(this->enumerated_physical_device_res == VK_SUCCESS);
        this->gpus.resize(gpu_count);

        this->enumerateGPU_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, this->gpus.data());
        assert(this->enumerateGPU_res == VK_SUCCESS);


        if (this->gpus.size() > 0) {
            vkGetPhysicalDeviceQueueFamilyProperties(this->gpus[0],
                                                     &this->queue_family_count, NULL);
            vkGetPhysicalDeviceMemoryProperties(this->gpus[0], &this->memory_properties);
            vkGetPhysicalDeviceProperties(this->gpus[0], &(this->physicalDeviceProperties));
        }
        return enumerateGPU_res;
    }

    bool VulkanContext::get_queue_families() {
        bool found = false;

        vkGetPhysicalDeviceQueueFamilyProperties(
                this->gpus[0],
                &this->queue_family_count,
                nullptr);
        assert(this->queue_family_count >= 0);
        this->queue_properties.resize(this->queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(
                this->gpus[0],
                &this->queue_family_count,
                this->queue_properties.data());

        for (unsigned int i = 0; i < this->queue_family_count; i++) {
            if (this->queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                this->queue_info.queueFamilyIndex = i;
                found = true;
                break;
            }
        }
        assert(found);
        assert(this->queue_family_count >= 1);
        return found;
    }

    void VulkanContext::createVirtualDevice() {


        this->queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        this->queue_info.pNext = NULL;
        this->queue_info.queueCount = 1;
        this->queue_info.pQueuePriorities = queue_priorities;


        this->physical_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        this->physical_device_info.pNext = NULL;
        this->physical_device_info.queueCreateInfoCount = 1;
        this->physical_device_info.pQueueCreateInfos = &queue_info;
        this->physical_device_info.enabledExtensionCount = 0;
        this->physical_device_info.ppEnabledExtensionNames = NULL;
        this->physical_device_info.enabledLayerCount = 0;
        this->physical_device_info.ppEnabledLayerNames = NULL;
        this->physical_device_info.pEnabledFeatures = NULL;


        this->createDevice_res = vkCreateDevice(this->gpus[0], &(this->physical_device_info), NULL, &(this->virtualDevice));
        assert(createDevice_res == VK_SUCCESS);
    }

    void VulkanContext::destroyVirtualDevice() {
        if (this->createDevice_res == VK_SUCCESS) {
            cout << "VirtualDevice destroyed " << endl;
            vkDestroyDevice(this->virtualDevice, NULL);
        }
    }

    int VulkanContext::countGPUs() const {
        return (int) this->gpus.size();
    }

    void VulkanContext::createCommandPool() {


        this->cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        this->cmd_pool_info.pNext = NULL;
        this->cmd_pool_info.queueFamilyIndex = this->queue_info.queueFamilyIndex;
        this->cmd_pool_info.flags = 0;

        this->command_pool_res = vkCreateCommandPool(this->virtualDevice, &this->cmd_pool_info, NULL, &this->cmd_pool);
        assert(command_pool_res == VK_SUCCESS);
    }

    void VulkanContext::createCommandBuffer() {
        this->cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        this->cmd.pNext = NULL;
        this->cmd.commandPool = this->cmd_pool;
        this->cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        this->cmd.commandBufferCount = 1;

        this->command_buffer_res = vkAllocateCommandBuffers(this->virtualDevice, &this->cmd, &this->commandBuffer);
        assert(command_buffer_res == VK_SUCCESS);
    }

    void VulkanContext::destroyCommandBuffers() {

        if (this->command_buffer_res == VK_SUCCESS) {
            VkCommandBuffer cmd_bufs[1] = {this->commandBuffer};
            vkFreeCommandBuffers(this->virtualDevice, this->cmd_pool, 1, cmd_bufs);
        }
        if (this->command_pool_res == VK_SUCCESS) {
            vkDestroyCommandPool(this->virtualDevice, this->cmd_pool, NULL);
        }
    }


    unsigned VulkanContext::getSwapChainSize() {
        return 0;
    }


    void VulkanContext::initExtensions() {
        uint32_t instance_extension_count;
        VkResult instanceExtension_res;
        // char *layer_name = NULL;

        //layer_name = properties.layerName;

        do {
            instanceExtension_res = vkEnumerateInstanceExtensionProperties(
                    properties.layerName, &instance_extension_count, NULL);
            extensions.resize(instance_extension_count);
            //instance_extensions = extensions.data();
            instanceExtension_res = vkEnumerateInstanceExtensionProperties(
                    properties.layerName, &instance_extension_count, extensions.data());
        } while (instanceExtension_res == VK_INCOMPLETE);

        //struct sample_info info = {};
//    init_global_layer_properties(info);
        //VkExtensionProperties *instance_extensions;


    }

    void VulkanContext::destroy_debug_report() {
        this->pfnDestroyDebugReportCallbackEXT(this->inst, this->debugReportCallback, nullptr);
        debugReportCallback = VK_NULL_HANDLE;
    }


    VkBool32 vlkDebugCallback(VkDebugReportFlagsEXT flags,
                              VkDebugReportObjectTypeEXT objType,
                              uint64_t srcObj,
                              size_t location,
                              int32_t msgCode,
                              const char *layerPrefix,
                              const char *msg,
                              void *userData) {
        if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
            std::cout << "[INFO | ";
        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
            std::cout << "[WARNING | ";
        } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            std::cout << "[PERFORMANCE | ";
        } else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            std::cout << "[ERROR | ";
        } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
            std::cout << "[DEBUG | ";
        }
        std::cout << layerPrefix << "] ";
        std::cout << msg << std::endl;
        return false;
    }


}