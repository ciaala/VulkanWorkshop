//
// Created by crypt on 29/06/16.
//

#include "VulkanSetup.h"
#include <assert.h>
#include <iostream>
#include <malloc.h>

using namespace std;

VulkanSetup::VulkanSetup(const char *application_name, const char *engine_name) {
    this->application_name = application_name;
    this->engine_name = engine_name;
    instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
}

void VulkanSetup::init() {
    //struct sample_info info = {};
//    init_global_layer_properties(info);
    //VkExtensionProperties *instance_extensions;
    uint32_t instance_extension_count;
    VkResult instanceExtension_res;
    // char *layer_name = NULL;

    //layer_name = properties.layerName;

    do {
        instanceExtension_res = vkEnumerateInstanceExtensionProperties(
                properties.layerName, &instance_extension_count, NULL);
        extensions.resize(instance_extension_count);
        //instance_extensions = extensions.data();
        instanceExtension_res = vkEnumerateInstanceExtensionProperties(
                properties.layerName, &instance_extension_count, extensions.data());
    } while (instanceExtension_res == VK_INCOMPLETE);



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
        std::cout << "cannot find a compatible Vulkan ICD" << endl;
        exit(-1);
    } else if (res) {
        std::cout << "unknown error" << endl;
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
    this->enumerated_physical_device_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, NULL);
    assert(this->enumerated_physical_device_res == VK_SUCCESS);
    this->gpus.resize(gpu_count);
    this->enumerateGPU_res = vkEnumeratePhysicalDevices(this->inst, &gpu_count, this->gpus.data());
    assert(this->enumerateGPU_res == VK_SUCCESS);
    if (this->gpus.size() > 0) {
        vkGetPhysicalDeviceProperties(this->gpus[0], &(this->physicalDeviceProperties));
        if (get_queue_families()) {

            createVirtualDevice();
            createCommandPool();
            createCommandBuffer();
            createSwapChain();
        } else {

            cerr << "No queue found" << endl;
        }
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


    this->queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    this->queue_info.pNext = NULL;
    this->queue_info.queueCount = 1;
    this->queue_info.pQueuePriorities = queue_priorities;


    this->physical_device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    this->physical_device_info.pNext = NULL;
    this->physical_device_info.queueCreateInfoCount = 1;
    this->physical_device_info.pQueueCreateInfos = &queue_info;
    this->physical_device_info.enabledExtensionCount = 0;
    this->physical_device_info.ppEnabledExtensionNames = NULL;
    this->physical_device_info.enabledLayerCount = 0;
    this->physical_device_info.ppEnabledLayerNames = NULL;
    this->physical_device_info.pEnabledFeatures = NULL;


    this->createDevice_res = vkCreateDevice(this->gpus[0], &(this->physical_device_info), NULL, &(this->virtualDevice));
    assert(createDevice_res == VK_SUCCESS);
}

void VulkanSetup::destroyVirtualDevice() {
    if (this->createDevice_res == VK_SUCCESS) {
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

    this->command_pool_res = vkCreateCommandPool(this->virtualDevice, &this->cmd_pool_info, NULL, &this->cmd_pool);
    assert(command_pool_res == VK_SUCCESS);
}

void VulkanSetup::createCommandBuffer() {
    this->cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    this->cmd.pNext = NULL;
    this->cmd.commandPool = this->cmd_pool;
    this->cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    this->cmd.commandBufferCount = 1;

    this->command_buffer_res = vkAllocateCommandBuffers(this->virtualDevice, &this->cmd, &this->commandBuffer);
    assert(command_buffer_res == VK_SUCCESS);
}

void VulkanSetup::destroyCommandBuffers() {

    if (this->command_buffer_res == VK_SUCCESS) {
        VkCommandBuffer cmd_bufs[1] = {this->commandBuffer};
        vkFreeCommandBuffers(this->virtualDevice, this->cmd_pool, 1, cmd_bufs);
    }
    if (this->command_pool_res == VK_SUCCESS) {
        vkDestroyCommandPool(this->virtualDevice, this->cmd_pool, NULL);
    }
}

void VulkanSetup::createSwapChain() {

}

unsigned VulkanSetup::getSwapChainSize() {
    return 0;
}

Window::Window(int width, int height, const char *name) {
    this->width = width;
    this->height = height;
    this->name = name;
}

// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    struct ApplicationContext *applicationContext = reinterpret_cast<struct ApplicationContext *>(
            GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            if (applicationContext != nullptr) {
                applicationContext->wm_paint();
                return 0;
            }

        default:
            break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void Window::init(ApplicationContext *applicationContext) {
    WNDCLASSEX win_class;

    this->connection = GetModuleHandle(NULL);

    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = WndProc;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = this->connection; // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = this->name.data();
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        printf("Unexpected error trying to start the application!\n");
        fflush(stdout);
        exit(1);
    }
    // Create window with the registered class:
    RECT wr = {0, 0, this->width, this->height};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
    this->window = CreateWindowEx(0,
                                  this->name.data(),            // class name
                                  this->name.data(),            // app name
                                  WS_OVERLAPPEDWINDOW | // window style
                                  WS_VISIBLE | WS_SYSMENU,
                                  100, 100,           // x/y coords
                                  wr.right - wr.left, // width
                                  wr.bottom - wr.top, // height
                                  NULL,               // handle to parent
                                  NULL,               // handle to menu
                                  this->connection,    // hInstance
                                  NULL);              // no extra parameters
    if (!this->window) {
        // It didn't work, so try to give a useful error:
        printf("Cannot create a window in which to draw!\n");
        fflush(stdout);
        exit(1);
    }
    SetWindowLongPtr(this->window, GWLP_USERDATA, (LONG_PTR) applicationContext);

    this->createSurface(applicationContext);
}

void Window::createSurface(ApplicationContext *applicationContext) {
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.hinstance = this->connection;
    createInfo.hwnd = this->window;
    VkResult surfaceCreateInfo_res = VK_RESULT_MAX_ENUM;
    surfaceCreateInfo_res = vkCreateWin32SurfaceKHR(applicationContext->vulkanSetup.inst, &createInfo, NULL, &this->surface);
    assert(surfaceCreateInfo_res == VK_SUCCESS);

}

void Window::checkQueueSupportPresenting(ApplicationContext *applicationContext) {
    // Iterate over each queue to learn whether it supports presenting:
    VkBool32 *pSupportsPresent =
            (VkBool32 *) malloc(applicationContext->vulkanSetup.queue_family_count * sizeof(VkBool32));
    for (uint32_t i = 0; i < applicationContext->vulkanSetup.queue_family_count; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(applicationContext->vulkanSetup.gpus[0], i, this->surface, &pSupportsPresent[i]);
    }
    for (uint32_t i = 0; i < applicationContext->vulkanSetup.queue_family_count; i++) {
        if (pSupportsPresent[i] == VK_TRUE) {
            applicationContext->vulkanSetup.present_queue_family_index = i;
            break;
        }
    }
    free(pSupportsPresent);


}

void ApplicationContext::wm_paint() {
    if (this->counter == 1000) {
        cout << "ApplicationContext::wm_paint" << endl;
    } else {
        this->counter = 0;
    }
}

void ApplicationContext::init() {

    this->vulkanSetup.init();

    cout << "ApplicationName: " << vulkanSetup.app_info.pApplicationName << endl;
    cout << "EngineName: " << vulkanSetup.app_info.pEngineName << endl;
    cout << "Vulkan Initialised: " << std::boolalpha << vulkanSetup.isVulkanReady() << endl;
    cout << "Number of Valid GPUs: " << vulkanSetup.countGPUs() << endl;
    cout << "Device Name: " << vulkanSetup.physicalDeviceProperties.deviceName << endl;
    cout << "Queues: " << vulkanSetup.queue_family_count << endl;
    cout << "Queue selected: " << vulkanSetup.queue_info.queueFamilyIndex << endl;

    cout << "Command Queue: " << vulkanSetup.cmd_pool_info.queueFamilyIndex << endl;
    cout << "Command Queue flags: " << vulkanSetup.cmd_pool_info.flags << endl;

    cout << "SwapChain: " << vulkanSetup.getSwapChainSize() << endl;

    this->window.init(this);
}

ApplicationContext::ApplicationContext() :
        vulkanSetup(APP_SHORT_NAME, ENGINE_SHORT_NAME),
        window(1280, 720, APP_SHORT_NAME) {

}
