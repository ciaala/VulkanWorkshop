//
// Created by crypt on 11/13/2016.
//

#include <cassert>
#include <iostream>
#include "VulkanUtility.h"

namespace vlk {
    VkResult VulkanUtility::init_global_layer_properties(
            vector<vlk_layer_properties> &instance_layer_properties) {
        uint32_t instance_layer_count;
        VkLayerProperties *vk_props = NULL;
        VkResult res;

        do {
            res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
            if (res)
                return res;

            if (instance_layer_count == 0) {
                return VK_SUCCESS;
            }

            vk_props = (VkLayerProperties *) realloc(
                    vk_props, instance_layer_count * sizeof(VkLayerProperties));

            res =
                    vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
        } while (res == VK_INCOMPLETE);
        for (uint32_t i = 0; i < instance_layer_count; i++) {
            vlk_layer_properties layer_props;
            layer_props.properties = vk_props[i];
            res = VulkanUtility::init_global_extension_properties(layer_props);
            if (res)
                return res;
            instance_layer_properties.push_back(layer_props);
        }
        free(vk_props);

        return res;
    }

    VkResult VulkanUtility::init_global_extension_properties(
            vlk_layer_properties &layer_props) {
        VkExtensionProperties *instance_extensions;
        uint32_t instance_extension_count;
        VkResult res;
        char *layer_name = NULL;

        layer_name = layer_props.properties.layerName;

        do {
            res = vkEnumerateInstanceExtensionProperties(
                    layer_name, &instance_extension_count, NULL);
            if (res)
                return res;

            if (instance_extension_count == 0) {
                return VK_SUCCESS;
            }

            layer_props.extensions.resize(instance_extension_count);
            instance_extensions = layer_props.extensions.data();
            cout << "Extension found: " << layer_name << endl;
            res = vkEnumerateInstanceExtensionProperties(
                    layer_name, &instance_extension_count, instance_extensions);
        } while (res == VK_INCOMPLETE);

        return res;
    }

    void VulkanUtility::init_instance_extension_names(
            vector<const char *> &instance_extension_names) {

        instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }

    void VulkanUtility::init_device_extension_names(
            vector<const char *> &device_extension_names) {
        device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    void VulkanUtility::createImageViewInfo(
            VkDevice &virtualDevice, VkImage &image,
            VkImageView &imageView) {
        const VkFormat depth_format = VK_FORMAT_D16_UNORM;

        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.pNext = NULL;
        view_info.image = VK_NULL_HANDLE;
        view_info.format = depth_format;
        view_info.components.r = VK_COMPONENT_SWIZZLE_R;
        view_info.components.g = VK_COMPONENT_SWIZZLE_G;
        view_info.components.b = VK_COMPONENT_SWIZZLE_B;
        view_info.components.a = VK_COMPONENT_SWIZZLE_A;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.flags = 0;

        view_info.image = image;
        VkResult res = vkCreateImageView(virtualDevice, &view_info, NULL, &imageView);
        assert(res == VK_SUCCESS);
    }

    void VulkanUtility::init_swapchain_ci(VkSwapchainCreateInfoKHR &swapchain_ci,
                                          VkSurfaceKHR &surfaceKHR,
                                          uint32_t desiredNumberOfSwapChainImages,
                                          VkFormat &imageFormat,
                                          VkExtent2D &swapchainExtent,
                                          VkSurfaceTransformFlagBitsKHR &preTransform,
                                          VkPresentModeKHR &swapchainPresentMode) {

        swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_ci.pNext = NULL;
        swapchain_ci.flags = 0;
        swapchain_ci.surface = surfaceKHR;
        swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
        swapchain_ci.imageFormat = imageFormat;
        swapchain_ci.imageExtent.width = swapchainExtent.width;
        swapchain_ci.imageExtent.height = swapchainExtent.height;
        swapchain_ci.preTransform = preTransform;
        swapchain_ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_ci.imageArrayLayers = 1;
        swapchain_ci.presentMode = swapchainPresentMode;
        swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
        swapchain_ci.clipped = (VkBool32) true;
        swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_ci.queueFamilyIndexCount = 0;
        swapchain_ci.pQueueFamilyIndices = NULL;

    }

    bool VulkanUtility::memory_type_from_properties(
            VkPhysicalDeviceMemoryProperties &memory_properties,
            uint32_t typeBits,
            VkFlags requirements_mask,
            uint32_t *typeIndex) {
        // Search memtypes to find first index with those properties
        for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
            if ((typeBits & 1) == 1) {
                // Type is available, does it match user properties?
                if ((memory_properties.memoryTypes[i].propertyFlags &
                     requirements_mask) == requirements_mask) {
                    *typeIndex = i;
                    return true;
                }
            }
            typeBits >>= 1;
        }
        // No memory types matched, return failure
        return false;
    }

    void VulkanUtility::mapMemory(
            VkDevice &virtualDevice,
            uniform_data &uniformData,
            VkDeviceSize memorySize,
            void *memory,
            VkMemoryRequirements &mem_reqs) {

        uint8_t *pData;
        VkResult res = vkMapMemory(virtualDevice, uniformData.mem, 0, mem_reqs.size, 0, (void **) &pData);
        assert(res == VK_SUCCESS);

        memcpy(pData, memory, memorySize);

        vkUnmapMemory(virtualDevice, uniformData.mem);

        res = vkBindBufferMemory(virtualDevice, uniformData.buf, uniformData.mem, 0);
        assert(res == VK_SUCCESS);

        uniformData.buffer_info.buffer = uniformData.buf;
        uniformData.buffer_info.offset = 0;
        uniformData.buffer_info.range = memorySize;
    }

    void VulkanUtility::create_image_info(
            VkDevice &virtualDevice,
            VkFormat &format,
            swap_chain_buffer &buffer) {

        VkResult res;
        VkImageViewCreateInfo color_image_view = {};
        color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        color_image_view.pNext = NULL;
        color_image_view.flags = 0;
        color_image_view.image = buffer.image;
        color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
        color_image_view.format = format;
        color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
        color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
        color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
        color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
        color_image_view.subresourceRange.aspectMask =
                VK_IMAGE_ASPECT_COLOR_BIT;
        color_image_view.subresourceRange.baseMipLevel = 0;
        color_image_view.subresourceRange.levelCount = 1;
        color_image_view.subresourceRange.baseArrayLayer = 0;
        color_image_view.subresourceRange.layerCount = 1;

        res = vkCreateImageView(virtualDevice, &color_image_view, NULL,
                                &buffer.view);
        assert(res == VK_SUCCESS);
    }

    void VulkanUtility::depthBufferImage(
            VkPhysicalDevice &physicalDevice, VkDevice &virtualDevice,
            uint32_t width, uint32_t height,
            VkImage &image) {
        VkImageCreateInfo image_info = {};

        const VkFormat depth_format = VK_FORMAT_D16_UNORM;
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, depth_format, &props);
        if (props.linearTilingFeatures &
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            image_info.tiling = VK_IMAGE_TILING_LINEAR;
        } else if (props.optimalTilingFeatures &
                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        } else {
            /* Try other depth formats? */
            std::cout << "VK_FORMAT_D16_UNORM Unsupported.\n";
            exit(-1);
        }

        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.pNext = NULL;
        image_info.flags = 0;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.format = VK_FORMAT_D16_UNORM;
        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.samples = NUM_SAMPLES;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        image_info.queueFamilyIndexCount = 0;
        image_info.pQueueFamilyIndices = NULL;
        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCreateImage(virtualDevice, &image_info, NULL, &image);
    }

    void VulkanUtility::setMemoryAllocation(
            VkDevice &virtualDevice,
            VkPhysicalDeviceMemoryProperties &memory_properties,
            VkImage &image,
            VkDeviceMemory &deviceMemory) {

        VkMemoryRequirements mem_reqs;

        VkMemoryAllocateInfo mem_alloc = {};
        mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        mem_alloc.pNext = NULL;
        mem_alloc.allocationSize = 0;
        mem_alloc.memoryTypeIndex = 0;
        vkGetImageMemoryRequirements(virtualDevice, image, &mem_reqs);


        mem_alloc.allocationSize = mem_reqs.size;
        /* Use the memory properties to determine the type of memory required */
        bool pass = VulkanUtility::memory_type_from_properties(memory_properties, mem_reqs.memoryTypeBits,
                                                               0, /* No Requirements */
                                                               &mem_alloc.memoryTypeIndex);
        assert(pass);

        /* Allocate memory */
        VkResult res = vkAllocateMemory(virtualDevice, &mem_alloc, NULL, &deviceMemory);
        assert(res == VK_SUCCESS);

        res = vkBindImageMemory(virtualDevice, image, deviceMemory, 0);
        assert(res == VK_SUCCESS);
    }

    void VulkanUtility::createUniformBuffer(
            VkDevice &virtualDevice,
            uniform_data &uniformData,
            VkDeviceSize memorySize,
            VkMemoryRequirements &mem_reqs) {
        VkBufferCreateInfo buf_info = {};
        buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buf_info.pNext = NULL;
        buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        buf_info.size = memorySize;
        buf_info.queueFamilyIndexCount = 0;
        buf_info.pQueueFamilyIndices = NULL;
        buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        buf_info.flags = 0;
        VkResult res = vkCreateBuffer(virtualDevice, &buf_info, NULL, &uniformData.buf);
        assert(res == VK_SUCCESS);

        vkGetBufferMemoryRequirements(virtualDevice, uniformData.buf, &mem_reqs);
    }

    void VulkanUtility::setMemoryAllocation(
            VkDevice &virtualDevice,
            VkPhysicalDeviceMemoryProperties &memory_properties,
            uniform_data &uniformData,
            VkMemoryRequirements &mem_reqs) {
        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.pNext = NULL;
        alloc_info.memoryTypeIndex = 0;

        alloc_info.allocationSize = mem_reqs.size;

        bool pass = memory_type_from_properties(memory_properties, mem_reqs.memoryTypeBits,
                                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                                &alloc_info.memoryTypeIndex);
        assert(pass && "No mappable, coherent memory");

        VkResult res = vkAllocateMemory(virtualDevice, &alloc_info, NULL,
                                        &(uniformData.mem));
        assert(res == VK_SUCCESS);
    }
    void VulkanUtility::createPipeline(
            VkDevice &virtualDevice,
            pipeline_info &pipelineInfo,
            const uint32_t NUM_DESCRIPTOR_SETS
    ) {
        VkDescriptorSetLayoutBinding layout_binding = {};
        layout_binding.binding = 0;
        layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layout_binding.descriptorCount = 1;
        layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        layout_binding.pImmutableSamplers = NULL;

        VkDescriptorSetLayoutCreateInfo descriptor_layout = {};
        descriptor_layout.sType =
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_layout.pNext = NULL;
        descriptor_layout.bindingCount = 1;
        descriptor_layout.pBindings = &layout_binding;

        pipelineInfo.descriptorLayoutList.resize(NUM_DESCRIPTOR_SETS);
        VkResult res = vkCreateDescriptorSetLayout(virtualDevice, &descriptor_layout, NULL,
                                          pipelineInfo.descriptorLayoutList.data());
        assert(res == VK_SUCCESS);

        VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
        pPipelineLayoutCreateInfo.sType =
                VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pPipelineLayoutCreateInfo.pNext = NULL;
        pPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pPipelineLayoutCreateInfo.pPushConstantRanges = NULL;
        pPipelineLayoutCreateInfo.setLayoutCount = (uint32_t) pipelineInfo.descriptorLayoutList.size();
        pPipelineLayoutCreateInfo.pSetLayouts = pipelineInfo.descriptorLayoutList.data();

        res = vkCreatePipelineLayout(virtualDevice, &pPipelineLayoutCreateInfo, NULL,
                                     &pipelineInfo.pipelineLayout);
        assert(res == VK_SUCCESS);
    }
}
