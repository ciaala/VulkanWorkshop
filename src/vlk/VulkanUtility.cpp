//
// Created by crypt on 11/13/2016.
//

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
            res = vkEnumerateInstanceExtensionProperties(
                    layer_name, &instance_extension_count, instance_extensions);
        } while (res == VK_INCOMPLETE);

        return res;
    }

    void VulkanUtility::init_instance_extension_names(vector<const char *> &instance_extension_names) {

        instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
        instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    }

    void VulkanUtility::init_device_extension_names(vector<const char *> & device_extension_names) {
        device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
}