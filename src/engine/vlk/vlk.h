//
// Created by crypt on 11/13/2016.
//
#define VK_USE_PLATFORM_WIN32_KHR


#include <vector>
#include "vulkan/vulkan.h"
#include "vulkan/vk_platform.h"

#ifndef VULKANWORKSHOP_VLK_H
#define VULKANWORKSHOP_VLK_H

#define VK_NO_PROTOTYPES
#define _GLIBCXX_USE_C99_DYNAMIC 1


using namespace std;
namespace vlk {
    typedef struct {
        VkLayerProperties properties;
        vector<VkExtensionProperties> extensions;
    } vlk_layer_properties;

    typedef struct _depth_buffer {
        VkImage depthBufferImage;
        VkDeviceMemory deviceMemory;
        VkImageView imageView;
        VkFormat format;
    } depth_buffer;

    typedef struct _uniform_data {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo buffer_info;
    } uniform_data;

    typedef struct _pipeline_info {
        std::vector<VkDescriptorSetLayout> descriptorLayoutList;
        VkPipelineLayout pipelineLayout;
    } pipeline_info;
    /*
 * Keep each of our swap chain buffers' image, command buffer and view in one
 * spot
 */

    typedef struct _swap_chain_buffers {
        VkImage image;
        VkImageView view;
    } swap_chain_buffer;

    typedef struct _vertex_buffer {
        VkBuffer buf;
        VkDeviceMemory mem;
        VkDescriptorBufferInfo buffer_info;
    } vertex_buffer;

    class ApplicationContext;

    class VulkanBufferUtility;

    class VulkanContext;

    class Window;
}
#endif //VULKANWORKSHOP_VLK_H
