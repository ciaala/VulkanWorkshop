//
// Created by crypt on 11/13/2016.
//

#include <cassert>
#include <iostream>
#include "VulkanUtility.h"

namespace vlk {
    VkResult VulkanUtility::init_global_layer_properties(vector<vlk_layer_properties> &instance_layer_properties) {
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

    VkResult VulkanUtility::init_global_extension_properties(vlk_layer_properties &layer_props) {
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
}
