//
// Created by crypt on 11/13/2016.
//

//#include <rpc.h>
#include <malloc.h>
#include <assert.h>
#include "Window.h"

namespace vlk {

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
                }
                return 0;

            default:
                break;
        }
        return (DefWindowProc(hWnd, uMsg, wParam, lParam));
    }

    Window::Window(int width, int height, const char *name) {
        this->width = width;
        this->height = height;
        this->name = name;
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

        if ( this->init_surface_extension(applicationContext) ) {

        }
//        this->checkQueueSupportPresenting(applicationContext);
    }

    VkResult Window::init_surface_extension(ApplicationContext *applicationContext) {
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.hinstance = this->connection;
        createInfo.hwnd = this->window;
        VkResult surfaceCreateInfo_res = vkCreateWin32SurfaceKHR(applicationContext->vulkanContext.inst, &createInfo, NULL, &this->surface);
        assert(surfaceCreateInfo_res == VK_SUCCESS);
        return surfaceCreateInfo_res;
    }

    void Window::checkQueueSupportPresenting(ApplicationContext *applicationContext) {
        // Iterate over each queue to learn whether it supports presenting:
        VkBool32 *pSupportsPresent =
                (VkBool32 *) malloc(applicationContext->vulkanContext.queue_family_count * sizeof(VkBool32));
        for (uint32_t i = 0; i < applicationContext->vulkanContext.queue_family_count; i++) {
            vkGetPhysicalDeviceSurfaceSupportKHR(applicationContext->vulkanContext.gpus[0], i, this->surface, &pSupportsPresent[i]);
        }
        for (uint32_t i = 0; i < applicationContext->vulkanContext.queue_family_count; i++) {
            if (pSupportsPresent[i] == VK_TRUE) {
                applicationContext->vulkanContext.present_queue_family_index = i;
                break;
            }
        }
        free(pSupportsPresent);


    }


}