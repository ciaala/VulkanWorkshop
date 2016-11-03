#include <iostream>
#include "vulkan/VulkanSetup.h"
#include <iostream>

using namespace std;

int main() {

    VulkanSetup vulkanSetup;
    cout << "ApplicationName: " << vulkanSetup.app_info.pApplicationName << endl;
    cout << "EngineName: " << vulkanSetup.app_info.pEngineName << endl;
    cout << "Vulkan Initialised: " << std::boolalpha << vulkanSetup.isVulkanReady() << endl;
    cout << "Valid GPU: " << vulkanSetup.countGPUs() << endl;
    return 0;
}