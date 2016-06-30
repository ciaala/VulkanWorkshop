//
// Created by crypt on 29/06/16.
//

#ifndef VULKANWORKSHOP_SDKUT_H
#define VULKANWORKSHOP_SDKUT_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <vulkan/vulkan.h>
#include <cstdlib>
#include <vector>
namespace vksdk {

/*
 * Keep each of our swap chain buffers' image, command buffer and view in one
 * spot
 */
    typedef struct _swap_chain_buffers {
        VkImage image;
        VkImageView view;
    } swap_chain_buffer;


    /*
 * A layer can expose extensions, keep track of those
 * extensions here.
 */
    typedef struct {
        VkLayerProperties properties;
        std::vector<VkExtensionProperties> extensions;
    } layer_properties;



    /*
 * Structure for tracking information used / created / modified
 * by utility functions.
 */
    struct sample_info {
#ifdef _WIN32
        #define APP_NAME_STR_LEN 80
    HINSTANCE connection;        // hInstance - Windows Instance
    char name[APP_NAME_STR_LEN]; // Name to put on the window/icon
    HWND window;                 // hWnd - window handle
#elif defined(__ANDROID__)
        PFN_vkCreateAndroidSurfaceKHR fpCreateAndroidSurfaceKHR;
#else  // _WIN32
        xcb_connection_t *connection;
        xcb_screen_t *screen;
        xcb_window_t window;
        xcb_intern_atom_reply_t *atom_wm_delete_window;
#endif // _WIN32
        VkSurfaceKHR surface;
        bool prepared;
        bool use_staging_buffer;
        bool save_images;

        std::vector<const char *> instance_layer_names;
        std::vector<const char *> instance_extension_names;
        std::vector<layer_properties> instance_layer_properties;
        std::vector<VkExtensionProperties> instance_extension_properties;
        VkInstance inst;

        std::vector<const char *> device_extension_names;
        std::vector<VkExtensionProperties> device_extension_properties;
        std::vector<VkPhysicalDevice> gpus;
        VkDevice device;
        VkQueue queue;
        uint32_t graphics_queue_family_index;
        VkPhysicalDeviceProperties gpu_props;
        std::vector<VkQueueFamilyProperties> queue_props;
        VkPhysicalDeviceMemoryProperties memory_properties;

        VkFramebuffer *framebuffers;
        int width, height;
        VkFormat format;

        uint32_t swapchainImageCount;
        VkSwapchainKHR swap_chain;
        std::vector<swap_chain_buffer> buffers;
        VkSemaphore presentCompleteSemaphore;

        VkCommandPool cmd_pool;

        struct {
            VkFormat format;

            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
        } depth;
/*
        std::vector<struct texture_object> textures;
*/
        struct {
            VkBuffer buf;
            VkDeviceMemory mem;
            VkDescriptorBufferInfo buffer_info;
        } uniform_data;

        struct {
            VkDescriptorImageInfo image_info;
        } texture_data;
        VkDeviceMemory stagingMemory;
        VkImage stagingImage;

        struct {
            VkBuffer buf;
            VkDeviceMemory mem;
            VkDescriptorBufferInfo buffer_info;
        } vertex_buffer;
        VkVertexInputBindingDescription vi_binding;
        VkVertexInputAttributeDescription vi_attribs[2];
/*
        glm::mat4 Projection;
        glm::mat4 View;
        glm::mat4 Model;
        glm::mat4 Clip;
        glm::mat4 MVP;
*/
        VkCommandBuffer cmd; // Buffer for initialization commands
        VkPipelineLayout pipeline_layout;
        std::vector<VkDescriptorSetLayout> desc_layout;
        VkPipelineCache pipelineCache;
        VkRenderPass render_pass;
        VkPipeline pipeline;

        VkPipelineShaderStageCreateInfo shaderStages[2];

        VkDescriptorPool desc_pool;
        std::vector<VkDescriptorSet> desc_set;

        PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
        PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
        PFN_vkDebugReportMessageEXT dbgBreakCallback;
        std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

        uint32_t current_buffer;
        uint32_t queue_count;

        VkViewport viewport;
        VkRect2D scissor;
    };


    /*
 * TODO: function description here
 */
    VkResult init_global_extension_properties(layer_properties &layer_props);
    VkResult init_global_layer_properties(struct sample_info &info);
}

#endif //VULKANWORKSHOP_SDKUT_H
