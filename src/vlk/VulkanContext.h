//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_VULKANSETUP_H
#define VULKANWORKSHOP_VULKANSETUP_H

#include "vlk.h"

#include <string>

#define APP_SHORT_NAME "Terrific"
#define ENGINE_SHORT_NAME "TerrificEngine"
using namespace std;

namespace vlk {


    class VulkanContext {
    public:
        const VkSampleCountFlagBits NUM_SAMPLES = VK_SAMPLE_COUNT_1_BIT;


        /** struct  extensions */
        vector<const char *> deviceExtensionNames;
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
        uint32_t present_queue_family_index;
        uint32_t graphic_queue_family_index;
        /* }; */

        /* struct PhysicalDevice {*/
        VkApplicationInfo app_info = {};
        VkInstanceCreateInfo inst_info = {};
        vector<VkPhysicalDevice> gpus;
        VkDeviceCreateInfo physical_device_info;
        VkPhysicalDeviceProperties physicalDeviceProperties;
        VkInstance inst;
        vector<VkVertexInputBindingDescription> vertexBindings;
        vector<VkVertexInputAttributeDescription> vertexAttributes;
        vector<vlk_layer_properties> instanceLayerProperties;

        /*   }; */
        void init();

    public:
        PFN_vkCreateDebugReportCallbackEXT pfnCreateDebugReportCallbackEXT;
        PFN_vkDestroyDebugReportCallbackEXT pfnDestroyDebugReportCallbackEXT;

        bool isVulkanReady() const;

        VulkanContext(const char *applicationName,
                      const char *engineName,
                      const uint32_t width,
                      const uint32_t height);

        ~VulkanContext();

        int countGPUs() const;


        VkCommandBuffer commandBuffer;
        VkCommandPool cmd_pool;
        VkCommandPoolCreateInfo cmd_pool_info;

        VkCommandBufferAllocateInfo commandBufferAllocateInfo;

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


        void createCommandBuffer();

        void destroyCommandBuffers();

        void createSwapChain();

        string application_name;
        string engine_name;


        VkPhysicalDeviceMemoryProperties memory_properties;

        VkResult init_instance();

        //void initExtensions();

        void init_debug_callback();

        VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo;
        VkDebugReportCallbackEXT debugReportCallback;

        void destroy_debug_report();

        vector<const char *> enabledInstanceLayers;
        vector<const char *> enabledInstanceExtensions;

        void destroy_surfaceKHR();

        void create_swapChain();

        uint32_t queueFamilyIndices[2];

        VkResult swapChainCreation_res;
        VkSwapchainKHR swapChain;


        VkSurfaceKHR surfaceKHR = nullptr;

        void createImage();

        uint32_t swapchainImageCount;

        vector<swap_chain_buffer> buffers;
        uint32_t currentBuffer = 0;

        VkFormat imageFormat;


        void selectImageFormat();

        void selectSurfaceCapabilities();

        VkSurfaceCapabilitiesKHR surfCapabilities;
        VkExtent2D swapchainExtent;

        // The FIFO present mode is guaranteed by the spec to be supported
        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;


        void setupTransform();

        void selectGraphicPresenterQueue();

        void setupSwapChainExtent(int width, int height);

        VkSurfaceTransformFlagBitsKHR preTransform;
        uint32_t desiredNumberOfSwapChainImages;
        VkSwapchainCreateInfoKHR swapchain_ci;

        void initWithWindow();

        void createDepthBuffer();

        void destroyMemoryBuffer();

        void createUniformBuffer();

        uniform_data uniformData;

        VkMemoryRequirements memoryRequirements;
        struct {
            uint32_t stuff;
            uint32_t moreStuff;
            uint32_t realStuff;
        } myData;
        depth_buffer depthBuffer;

        void destroyDepthBuffer();

        void destroyPipeline();

        pipeline_info pipelineInfo;
        const uint32_t NUM_DESCRIPTOR_SET = 1;

        void createPipeline();

        void createDescriptorSet();

        void destroyDescriptorSet();

        void updateDescriptorSet();

        void initRenderPass();

        vector<VkDescriptorSet> descriptorSetList;
        VkDescriptorPool descriptorPool;

        VkSemaphore imageAcquiredSemaphore;


        void runRenderPass();

        void acquireNextImage();

        VkQueue graphicQueue;

        void prepareAttachments(vector<VkAttachmentDescription> &attachmentDescriptions);
        VkRenderPass renderPass;
        void destroyRenderPass();

        void destroyFrameBuffers();
        vector<VkFramebuffer> frameBuffers;

        void initFrameBuffers();
        const uint32_t windowWidth;
        //vector<VkImageView> attachments;
        const uint32_t windowHeight;

        void beginCommandBuffer();


        void initVertexBuffers();

        vertex_buffer cubeVertexBuffer;

        void destroyVertexBuffers();
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
