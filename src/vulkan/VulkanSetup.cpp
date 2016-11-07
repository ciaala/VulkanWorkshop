//
// Created by crypt on 29/06/16.
//

#include <assert.h>
#include "VulkanSetup.h"

#include <iostream>
#include <lm.h>

using namespace std;

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
    this->destroyCommandBuffers();
    this->destroyVirtualDevice();
    vkDestroyInstance(inst, NULL);
}

bool VulkanSetup::isVulkanReady() const {
    return this->res == VK_SUCCESS;
}

void VulkanSetup::post_init_setup() {
    uint32_t gpu_count = 0;
    VkResult res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, NULL);
    this->gpus.resize(gpu_count);
    this->enumerateGPU_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, this->gpus.data());

    vkGetPhysicalDeviceProperties(this->gpus[0], &(this->physicalDeviceProperties));
    if (!get_queue_families()) {
        cerr << "No queue found" << endl;
    } else{
        createVirtualDevice();
        createCommandPool();
        createCommandBuffer();
    }
}

bool VulkanSetup::get_queue_families() {
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

void VulkanSetup::createVirtualDevice() {
    this->device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    this->device_info.pNext = NULL;
    this->device_info.queueCreateInfoCount = 1;
    this->device_info.pQueueCreateInfos = &queue_info;
    this->device_info.enabledExtensionCount = 0;
    this->device_info.ppEnabledExtensionNames = NULL;
    this->device_info.enabledLayerCount = 0;
    this->device_info.ppEnabledLayerNames = NULL;
    this->device_info.pEnabledFeatures = NULL;


    createDevice_res = vkCreateDevice(this->gpus[0],
                         &this->device_info,
                         NULL,
                         &this->virtualDevice);
    assert(res == VK_SUCCESS);

}

void VulkanSetup::destroyVirtualDevice() {
    if ( this->createDevice_res == VK_SUCCESS) {
        cout << "VirtualDevice destroyed " << endl;
        vkDestroyDevice(this->virtualDevice, NULL);
    }
}

int VulkanSetup::countGPUs() const {
    return (int) this->gpus.size();
}

void VulkanSetup::createCommandPool() {


    this->cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    this->cmd_pool_info.pNext = NULL;
    this->cmd_pool_info.queueFamilyIndex = this->queue_info.queueFamilyIndex;
    this->cmd_pool_info.flags = 0;

    res =
            vkCreateCommandPool(this->virtualDevice, &this->cmd_pool_info, NULL, &this->cmd_pool);
    assert(res == VK_SUCCESS);
}

void VulkanSetup::createCommandBuffer() {
    this->cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    this->cmd.pNext = NULL;
    this->cmd.commandPool = this->cmd_pool;
    this->cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    this->cmd.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(this->virtualDevice, &this->cmd, &this->commandBuffer);
    assert(res == VK_SUCCESS);
}

void VulkanSetup::destroyCommandBuffers() {

    VkCommandBuffer cmd_bufs[1] = {this->commandBuffer};
    vkFreeCommandBuffers(this->virtualDevice, this->cmd_pool, 1, cmd_bufs);
    vkDestroyCommandPool(this->virtualDevice, this->cmd_pool, NULL);
}

