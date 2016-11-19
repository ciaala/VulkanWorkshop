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
    };
}

#endif //VULKANWORKSHOP_VULKANUTILITY_H