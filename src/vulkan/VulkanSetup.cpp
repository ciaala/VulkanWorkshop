//
// Created by crypt on 29/06/16.
//

#include <assert.h>
#include "VulkanSetup.h"

VulkanSetup::VulkanSetup() {
    struct sample_info info = {};
    init_global_layer_properties(info);

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

    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;


    res = vkCreateInstance(&inst_info, NULL, &inst);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    } else {
        post_init_setup();
    }



    /* VULKAN_KEY_END */

}

VulkanSetup::~VulkanSetup() {
    vkDestroyInstance(inst, NULL);
}

bool VulkanSetup::isVulkanReady() const {
    return this->res == VK_SUCCESS ;
}

void VulkanSetup::post_init_setup() {
    uint32_t gpu_count = 0;
    VkResult res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, NULL);
    this->gpus.resize(gpu_count);
    this->enumerateGPU_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, this->gpus.data());
}

int VulkanSetup::countGPUs() const {
    return (int) this->gpus.size();
}

