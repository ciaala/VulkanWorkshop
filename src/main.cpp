#include <iostream>
#include "vulkan/VulkanSetup.h"
#include <iostream>

using namespace std;

int main(int argc, char ** argv) {

    VulkanSetup vulkanSetup;
    cout << "ApplicationName: " << vulkanSetup.app_info.pApplicationName << endl;
    cout << "EngineName: " << vulkanSetup.app_info.pEngineName << endl;
    cout << "Vulkan Initialised: " << std::boolalpha << vulkanSetup.isVulkanReady() << endl;
    cout << "Number of Valid GPUs: " << vulkanSetup.countGPUs() << endl;
    cout << "Device Name: " << vulkanSetup.physicalDeviceProperties.deviceName << endl;
    cout << "Queues: " << vulkanSetup.queue_family_count << endl;
    cout << "Queue selected: " << vulkanSetup.queue_info.queueFamilyIndex << endl;

    cout << "Command Queue: " << vulkanSetup.cmd_pool_info.queueFamilyIndex << endl;
    cout << "Command Queue flags: " << vulkanSetup.cmd_pool_info.flags << endl;
    return 0;
}