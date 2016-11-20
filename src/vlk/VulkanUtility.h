//
// Created by crypt on 11/13/2016.
//

#ifndef VULKANWORKSHOP_VULKANUTILITY_H
#define VULKANWORKSHOP_VULKANUTILITY_H

#include "vlk.h"
#include <vector>

using namespace std;
namespace vlk {
    typedef struct {
        VkLayerProperties properties;
        vector<VkExtensionProperties> extensions;
    } vlk_layer_properties;


    class VulkanUtility {
    private:
        VulkanUtility();

    public:
        static VkResult init_global_layer_properties(vector<vlk_layer_properties> &instance_layer_properties);

        static VkResult init_global_extension_properties(vlk_layer_properties &layer_props);

        static
        void init_instance_extension_names(vector<const char *> &instance_extension_names);

        static
        void init_device_extension_names(vector<const char *> &device_extension_names);

        static
        void init_swapchain_ci(VkSwapchainCreateInfoKHR &swapchain_ci,
                               VkSurfaceKHR &surfaceKHR,
                               uint32_t desiredNumberOfSwapChainImages,
                               VkFormat &imageFormat,
                               VkExtent2D &swapchainExtent,
                               VkSurfaceTransformFlagBitsKHR &preTransform,
                               VkPresentModeKHR &swapchainPresentMode);

        static
        void create_image_info(
                VkDevice &virtualDevice,
                VkFormat &format,
                swap_chain_buffer &buffer);

        static
        void createImageViewInfo(VkDevice &virtualDevice, VkImage &image, VkImageView &imageView);

        static
        bool
        memory_type_from_properties(VkPhysicalDeviceMemoryProperties &memory_properties, uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex);

        static
        void setMemoryAllocation(VkDevice &virtualDevice, VkPhysicalDeviceMemoryProperties &memory_properties, VkImage &image, VkDeviceMemory &deviceMemory);

        static
        void depthBufferImage(VkPhysicalDevice &physicalDevice, VkDevice &virtualDevice, uint32_t width, uint32_t height, VkFormat &depthBufferFormat, VkImage &image, VkSampleCountFlagBits NUM_SAMPLES );

        static
        void createUniformBuffer(VkDevice &virtualDevice, uniform_data &uniformData, VkDeviceSize memorySize, VkMemoryRequirements &mem_reqs);

        static
        void mapMemory(VkDevice &virtualDevice, uniform_data &uniformData, VkDeviceSize memorySize, void *memory, VkMemoryRequirements &mem_reqs);

        static
        void setMemoryAllocation(VkDevice &virtualDevice, VkPhysicalDeviceMemoryProperties &memory_properties, uniform_data &uniformData,
                                 VkMemoryRequirements &mem_reqs);

        static
        void createPipeline(VkDevice &virtualDevice, pipeline_info &pipelineInfo, const uint32_t NUM_DESCRIPTOR_SETS);

        static
        void createDescriptorSet(
                VkDevice &virtualDevice,
                VkDescriptorPool &descriptorPool,
                std::vector<VkDescriptorSet> &descriptorSetList,
                const uint32_t NUM_DESCRIPTOR_SETS,
                pipeline_info &pipelineInfo);

        static
        void set_image_layout(
                VkCommandBuffer &cmd,
                VkQueue &graphic_queue,
                VkImage image,
                VkImageAspectFlags aspectMask,
                VkImageLayout old_image_layout,
                VkImageLayout new_image_layout);

        static
        void createRenderPass(VkDevice &virtualDevice,
                              vector<VkAttachmentDescription> &attachments,
                              vector<VkAttachmentReference> &colorReferences,
                              VkAttachmentReference &depthReference,
                              VkSubpassDescription &subpass, VkRenderPass &renderPass);
    };

}

#endif //VULKANWORKSHOP_VULKANUTILITY_H
