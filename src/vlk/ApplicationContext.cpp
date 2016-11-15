//
// Created by crypt on 11/13/2016.
//

#include "ApplicationContext.h"
#include <iostream>
using namespace std;
namespace vlk {
    void ApplicationContext::wm_paint() {
        if (this->counter == 1000) {
            cout << "ApplicationContext::wm_paint" << endl;
        } else {
            this->counter = 0;
        }
    }

    void ApplicationContext::init() {

        this->vulkanContext.init();

        cout << "ApplicationName: " << vulkanContext.app_info.pApplicationName << endl;
        cout << "EngineName: " << vulkanContext.app_info.pEngineName << endl;
        cout << "Vulkan Initialised: " << std::boolalpha << vulkanContext.isVulkanReady() << endl;
        cout << "Number of Valid GPUs: " << vulkanContext.countGPUs() << endl;
        cout << "Device Name: " << vulkanContext.physicalDeviceProperties.deviceName << endl;
        cout << "Device Memory: " << vulkanContext.memory_properties.memoryTypeCount << endl;

        cout << "Queues: " << vulkanContext.queue_family_count << endl;
        cout << "Queue selected: " << vulkanContext.queue_info.queueFamilyIndex << endl;

        cout << "Command Queue: " << vulkanContext.cmd_pool_info.queueFamilyIndex << endl;
        cout << "Command Queue flags: " << vulkanContext.cmd_pool_info.flags << endl;

        cout << "SwapChain: " << vulkanContext.getSwapChainSize() << endl;

        this->window.init(this);

    }

    ApplicationContext::ApplicationContext() :
            vulkanContext(APP_SHORT_NAME, ENGINE_SHORT_NAME),
            window(1280, 720, APP_SHORT_NAME) {

    }
}