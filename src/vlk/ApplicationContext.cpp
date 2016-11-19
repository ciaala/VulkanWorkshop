//
// Created by crypt on 11/13/2016.
//

#include "ApplicationContext.h"
#include <iostream>
using namespace std;
namespace vlk {
    void ApplicationContext::wm_paint() {
        if (this->counter == 1000) {
            cout << "ApplicationContext::wm_paint" << endl << flush;
            this->counter = 0;
        } else {
            this->counter ++;
        }
    }

    void ApplicationContext::init() {
        this->isRunning = true;
        this->vulkanContext.init();

        this->window.init(this);

        this->vulkanContext.initWithWindow();
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

    }

    ApplicationContext::ApplicationContext() :
            vulkanContext(APP_SHORT_NAME, ENGINE_SHORT_NAME),
            window(1280, 720, APP_SHORT_NAME) {

    }

    void ApplicationContext::join() {

        while(this->isRunning) {
            MSG msg;

            while (GetMessage (&msg, NULL, 0, 0) > 0)
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
                if (VK_ESCAPE == msg.wParam)
                    break;
            }
        }
    }

    void ApplicationContext::quit() {
        this->isRunning = false;
    }
}