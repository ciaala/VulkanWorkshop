//
// Created by crypt on 29/06/16.
//

#include "VulkanContext.h"
#include <assert.h>
#include <iostream>
#include <malloc.h>

using namespace std;
namespace vlk {
    VulkanContext::VulkanContext(const char *application_name, const char *engine_name) {
        this->application_name = application_name;
        this->engine_name = engine_name;

    }

    void VulkanContext::init() {

        if (VK_SUCCESS != VulkanUtility::init_global_layer_properties(this->instanceLayerProperties)) {
            cerr << "Unable to setup Vulkan !!!" << endl;
            return;
        }
        VulkanUtility::init_instance_extension_names(this->instanceExtensionNames);
        VulkanUtility::init_device_extension_names(this->deviceExtensionNames);

        if (init_instance() == VK_SUCCESS) {
            init_debug_callback();
            init_enumerate_device();
            get_queue_families();
        } else {
            cerr << "Instance not created" << endl;
            exit(-1);
        }
    }

    void VulkanContext::initWithWindow() {

        selectGraphicPresenterQueue();
        createVirtualDevice();
        //createCommandPool();
        //createCommandBuffer();
        selectImageFormat();
        selectSurfaceCapabilities();
        setupSwapChainExtent(1280, 720);
        setupTransform();
        create_swapChain();
        createImage();
        createDepthBuffer();
        createUniformBuffer();
        createPipeline();
    }

    VkResult VulkanContext::init_instance() {
        //
        // STEP 1
        //
        cout << "- STEP#0001 " << "init_instance" << endl << flush;

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
        //
        // STEP 2
        //
        cout << "- STEP#0002 " << "init_debug_callback" << endl << flush;

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

/*
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
*/
    VkResult VulkanContext::init_enumerate_device() {

        //
        // STEP 15
        //
        cout << "- STEP#0015 " << "init_enumerate_device" << endl << flush;

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
        } else {
            cerr << "No GPU recognized";
        }
        return enumerateGPU_res;
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

    /**
     * Identify the queues to be used
     * GRAPHIC, PRESENT
     * @return true if founds
     */
    void VulkanContext::selectGraphicPresenterQueue() {
        //
        // STEP 25
        //
        // Search for a graphics and a present queue in the array of queue
        // families, try to find one that supports both
        cout << "- STEP#0025 " << "selectGraphicPresenterQueue" << endl << flush;


        VkBool32 *pSupportsPresent =
                (VkBool32 *) malloc(this->queue_properties.size() * sizeof(VkBool32));
        for (uint32_t i = 0; i < this->queue_properties.size(); i++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(this->gpus[0], i, this->surfaceKHR,
                                                 &pSupportsPresent[i]);
        }
        this->graphic_queue_family_index = UINT32_MAX;
        this->present_queue_family_index = UINT32_MAX;
        for (uint32_t i = 0; i < this->queue_properties.size(); ++i) {
            if ((this->queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                if (this->graphic_queue_family_index == UINT32_MAX)
                    this->graphic_queue_family_index = i;

                if (pSupportsPresent[i] == VK_TRUE) {
                    this->graphic_queue_family_index = i;
                    this->present_queue_family_index = i;
                    break;
                }
            }
        }
        if (this->present_queue_family_index == UINT32_MAX) {
            // If didn't find a queue that supports both graphics and present, then
            // find a separate present queue.
            for (size_t i = 0; i < this->queue_properties.size(); ++i)
                if (pSupportsPresent[i] == VK_TRUE) {
                    this->present_queue_family_index = (uint32_t) i;
                    break;
                }
        }
        free(pSupportsPresent);
    }

    void VulkanContext::createVirtualDevice() {
        //
        // STEP 26
        //
        cout << "- STEP#0026 " << "createVirtualDevice" << endl << flush;

        this->queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        this->queue_info.flags = 0;
        this->queue_info.pNext = NULL;
        this->queue_info.queueCount = 1;
        this->queue_info.pQueuePriorities = this->queue_priorities;
        this->queue_info.queueFamilyIndex = this->graphic_queue_family_index;


        this->physical_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        this->physical_device_info.flags = 0;
        this->physical_device_info.pNext = NULL;
        this->physical_device_info.queueCreateInfoCount = 1;
        this->physical_device_info.pQueueCreateInfos = &this->queue_info;
        this->physical_device_info.enabledExtensionCount = (uint32_t) this->deviceExtensionNames.size();
        this->physical_device_info.ppEnabledExtensionNames = this->deviceExtensionNames.size() > 0 ? this->deviceExtensionNames.data() : nullptr;
        this->physical_device_info.enabledLayerCount = (uint32_t) this->enabledInstanceLayers.size();

        this->physical_device_info.ppEnabledLayerNames =
                this->enabledInstanceLayers.size() > 0 ? this->enabledInstanceLayers.data() : nullptr;

        this->physical_device_info.pEnabledFeatures = NULL;
        this->createDevice_res = vkCreateDevice(this->gpus[0], &(this->physical_device_info), NULL, &(this->virtualDevice));
        assert(createDevice_res == VK_SUCCESS);
    }

    /**
     *  Get the list of VkFormats that are supported:
     */
    void VulkanContext::selectImageFormat() {
        //
        // STEP 30
        //
        cout << "- STEP#0030 " << "selectImageFormat" << endl << flush;

        uint32_t formatCount;
        VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(this->gpus[0], this->surfaceKHR,
                                                            &formatCount, NULL);
        assert(res == VK_SUCCESS);
        VkSurfaceFormatKHR *surfFormats =
                (VkSurfaceFormatKHR *) malloc(formatCount * sizeof(VkSurfaceFormatKHR));
        res = vkGetPhysicalDeviceSurfaceFormatsKHR(this->gpus[0], this->surfaceKHR,
                                                   &formatCount, surfFormats);
        assert(res == VK_SUCCESS);
        // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
        // the surface has no preferred format.  Otherwise, at least one
        // supported format will be returned.
        if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
            this->imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
        } else {
            assert(formatCount >= 1);
            this->imageFormat = surfFormats[0].format;
        }
        free(surfFormats);
    }

    void VulkanContext::selectSurfaceCapabilities() {
        //
        // STEP 31
        //
        cout << "- STEP#0031 " << "selectSurfaceCapabilities" << endl << flush;

        VkResult res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->gpus[0], this->surfaceKHR,
                                                                 &this->surfCapabilities);
        assert(res == VK_SUCCESS);

        uint32_t presentModeCount;
        res = vkGetPhysicalDeviceSurfacePresentModesKHR(this->gpus[0], this->surfaceKHR,
                                                        &presentModeCount, NULL);
        assert(res == VK_SUCCESS);
        VkPresentModeKHR *presentModes =
                (VkPresentModeKHR *) malloc(presentModeCount * sizeof(VkPresentModeKHR));

        res = vkGetPhysicalDeviceSurfacePresentModesKHR(
                this->gpus[0], this->surfaceKHR, &presentModeCount, presentModes);
        assert(res == VK_SUCCESS);
    }

    void VulkanContext::setupSwapChainExtent(int width, int height) {
        //
        // STEP 32
        //
        cout << "- STEP#0032 " << "setupSwapChainExtent" << endl << flush;

        // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
        if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
            // If the surface size is undefined, the size is set to
            // the size of the images requested.
            swapchainExtent.width = width;
            swapchainExtent.height = height;
            if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
                swapchainExtent.width = surfCapabilities.minImageExtent.width;
            } else if (swapchainExtent.width >
                       surfCapabilities.maxImageExtent.width) {
                swapchainExtent.width = surfCapabilities.maxImageExtent.width;
            }

            if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
                swapchainExtent.height = surfCapabilities.minImageExtent.height;
            } else if (swapchainExtent.height >
                       surfCapabilities.maxImageExtent.height) {
                swapchainExtent.height = surfCapabilities.maxImageExtent.height;
            }
        } else {
            // If the surface size is defined, the swap chain size must match
            swapchainExtent = surfCapabilities.currentExtent;
        }
        // Determine the number of VkImage's to use in the swap chain.
        // We need to acquire only 1 presentable image at at time.
        // Asking for minImageCount images ensures that we can acquire
        // 1 presentable image as long as we present it before attempting
        // to acquire another.
        this->desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;
    }

    void VulkanContext::setupTransform() {
        //
        // STEP 33
        //
        cout << "- STEP#0033 " << "setupTransform" << endl;

        if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR
                ) {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            preTransform = surfCapabilities.currentTransform;
        }
    }

    void VulkanContext::create_swapChain() {
        //
        // STEP 34
        //
        cout << "- STEP#0034 " << "create_swapChain" << endl;

        VulkanUtility::init_swapchain_ci(
                swapchain_ci, this->surfaceKHR, this->desiredNumberOfSwapChainImages,
                this->imageFormat, this->swapchainExtent,
                this->preTransform, this->swapchainPresentMode);

        queueFamilyIndices[0] =
                (uint32_t) this->graphic_queue_family_index;

        queueFamilyIndices[1] =
                (uint32_t) this->present_queue_family_index;

        if (this->graphic_queue_family_index != this->present_queue_family_index) {
            // If the graphics and present queues are from different queue families,
            // we either have to explicitly transfer ownership of images between
            // the queues, or we have to create the swapchain with imageSharingMode
            // as VK_SHARING_MODE_CONCURRENT
            swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchain_ci.queueFamilyIndexCount = 2;
            swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
        }
        this->swapChainCreation_res = vkCreateSwapchainKHR(this->virtualDevice, &swapchain_ci, NULL,
                                                           &this->swapChain);
        assert(res == VK_SUCCESS);


    }

    void VulkanContext::createImage() {
        //
        // STEP 35
        //
        cout << "- STEP#0035 " << "createImage" << endl;

        VkResult res = vkGetSwapchainImagesKHR(this->virtualDevice, this->swapChain,
                                               &this->swapchainImageCount, NULL);
        assert(res == VK_SUCCESS);

        VkImage *swapchainImages =
                (VkImage *) malloc(this->swapchainImageCount * sizeof(VkImage));
        assert(swapchainImages);
        res = vkGetSwapchainImagesKHR(this->virtualDevice, this->swapChain,
                                      &this->swapchainImageCount, swapchainImages);
        assert(res == VK_SUCCESS);
        this->buffers.resize(this->swapchainImageCount);
        for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
            this->buffers[i].image = swapchainImages[i];
        }
        free(swapchainImages);
        for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
            VulkanUtility::create_image_info(
                    this->virtualDevice, this->imageFormat, this->buffers[i]);
        }
    }

    void VulkanContext::createDepthBuffer() {
        //
        // STEP 36
        //
        cout << "- STEP#0036 " << "createDepthBuffer" << endl << flush;

        VulkanUtility::depthBufferImage(this->gpus[0], this->virtualDevice, 1280, 720, this->depthBuffer.depthBufferImage);

        VulkanUtility::setMemoryAllocation(this->virtualDevice, this->memory_properties, this->depthBuffer.depthBufferImage, this->depthBuffer.deviceMemory);

        VulkanUtility::createImageViewInfo(this->virtualDevice, this->depthBuffer.depthBufferImage, this->depthBuffer.imageView);
    }

    void VulkanContext::createUniformBuffer() {
        //
        // STEP 37
        //
        cout << "- STEP#0037 " << "createUniformBuffer" << endl << flush;
        VulkanUtility::createUniformBuffer(this->virtualDevice, this->uniformData, sizeof(myData), this->memoryRequirements);

        VulkanUtility::setMemoryAllocation(this->virtualDevice, this->memory_properties, this->uniformData, this->memoryRequirements);
        VulkanUtility::mapMemory(this->virtualDevice, this->uniformData, sizeof(myData), (void *) &myData, this->memoryRequirements);

    }

    void VulkanContext::createPipeline() {
        //
        // STEP 38
        //
        cout << "- STEP#0038 " << "createPipeline" << endl << flush;
        VulkanUtility::createPipeline(this->virtualDevice, this->pipelineInfo, this->NUM_DESCRIPTOR_SET);
    }

    //
    // DESTRUCTORS
    //
    void VulkanContext::destroyDescriptorSet() {

    }
    void VulkanContext::destroyPipeline() {
        for (auto i = 0; i < this->pipelineInfo.descriptorLayoutList.size(); i++)
            vkDestroyDescriptorSetLayout(this->virtualDevice, this->pipelineInfo.descriptorLayoutList[i], nullptr);
        vkDestroyPipelineLayout(this->virtualDevice, this->pipelineInfo.pipelineLayout, nullptr);
    }
    void VulkanContext::destroyDepthBuffer() {
        vkDestroyImageView(this->virtualDevice, this->depthBuffer.imageView, NULL);
        vkDestroyImage(this->virtualDevice, this->depthBuffer.depthBufferImage, NULL);
        vkFreeMemory(this->virtualDevice, this->depthBuffer.deviceMemory, NULL);
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

    void VulkanContext::destroy_debug_report() {
        this->pfnDestroyDebugReportCallbackEXT(this->inst, this->debugReportCallback, nullptr);
        debugReportCallback = VK_NULL_HANDLE;
    }

    void VulkanContext::destroy_surfaceKHR() {
        for (uint32_t i = 0; i < this->swapchainImageCount; i++) {
            vkDestroyImageView(this->virtualDevice, this->buffers[i].view, nullptr);
        }
        vkDestroySwapchainKHR(this->virtualDevice, this->swapChain, nullptr);
        vkDestroySurfaceKHR(this->inst, this->surfaceKHR, NULL);
    }

    void VulkanContext::destroyVirtualDevice() {
        if (this->createDevice_res == VK_SUCCESS) {
            cout << "VirtualDevice destroyed " << endl;
            vkDestroyDevice(this->virtualDevice, NULL);
        }
    }

    void VulkanContext::destroyMemoryBuffer() {
        vkDestroyBuffer(this->virtualDevice, this->uniformData.buf, NULL);
        vkFreeMemory(this->virtualDevice, this->uniformData.mem, NULL);
    }

    VulkanContext::~VulkanContext() {
        this->destroyPipeline();
        this->destroyDepthBuffer();
        this->destroyMemoryBuffer();
        this->destroyCommandBuffers();
        this->destroy_surfaceKHR();
        this->destroyVirtualDevice();
        this->destroy_debug_report();

        vkDestroyInstance(inst, NULL);
    }

    //
    // GETTERS - SETTERS
    //
    unsigned VulkanContext::getSwapChainSize() {
        return 0;
    }

    int VulkanContext::countGPUs() const {
        return (int) this->gpus.size();
    }

    bool VulkanContext::isVulkanReady() const {
        return this->res == VK_SUCCESS;
    }

    // CALLBACKS

    VkBool32 vlkDebugCallback(VkDebugReportFlagsEXT flags,
                              VkDebugReportObjectTypeEXT objType,
                              uint64_t srcObj,
                              size_t location,
                              int32_t msgCode,
                              const char *layerPrefix,
                              const char *msg,
                              void *userData) {

        // TODO Select and use some logging libraries. Evaluate logging to a small service through some network
        // TODO Identify way to add colors

        if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
            std::cout << "[INFO        | ";
        } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
            std::cout << "[WARNING *** | ";
        } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
            std::cout << "[PERFORMANCE | ";
        } else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
            //std::cout << "\033[1;31mbold [ERROR ***** | ";
            std::cout << "[ERROR ***** | ";
        } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
            std::cout << "[DEBUG       | ";
        }
        std::cout << layerPrefix << "] ";
        std::cout << msg;
        if (0 && (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)) {
            std::cout << "\033[0m";
        }
        std::cout << std::endl;
        return false;
    }
}

