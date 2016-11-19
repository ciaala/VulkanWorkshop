//
// Created by crypt on 11/13/2016.
//

//#include <rpc.h>
#include <malloc.h>
#include <assert.h>
#include <iostream>
#include "Window.h"

namespace vlk {

// MS-Windows event handling function:
    LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        struct ApplicationContext *applicationContext = reinterpret_cast<struct ApplicationContext *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        switch (uMsg) {
            case WM_DESTROY:
                if (applicationContext != nullptr) {
                    PostQuitMessage(0);
                    applicationContext->quit();
                }
                break;
            case WM_CLOSE:
                DestroyWindow (hWnd);
                break;
            case WM_PAINT:
                if (applicationContext != nullptr) {
                    applicationContext->wm_paint();
                }
                break;
            default:
                return (DefWindowProc(hWnd, uMsg, wParam, lParam));

        }
        return 0;
    }

    Window::Window(int width, int height, const char *name) {
        this->width = width;
        this->height = height;
        this->name = name;
    }

    void Window::init(ApplicationContext *applicationContext) {

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
            cerr << "Unexpected error trying to start the application!" << endl << flush;
            exit(1);
        }

        wr.left = 0;
        wr.top = 0;
        wr.right = this->width;
        wr.bottom = this->height;
        // Create window with the registered class:
        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
        this->window = CreateWindowEx(0,
                                      this->name.data(),            // class name
                                      this->name.data(),            // app name
                                      WS_OVERLAPPEDWINDOW | // window style
                                      WS_VISIBLE | WS_SYSMENU,
                                      384, 216,           // x/y coords
                                      wr.right - wr.left, // width
                                      wr.bottom - wr.top, // height
                                      NULL,               // handle to parent
                                      NULL,               // handle to menu
                                      this->connection,    // hInstance
                                      NULL);              // no extra parameters

        if (!this->window) {
            cerr << "Cannot create a window in which to draw!" << endl << flush ;
            exit(1);
        }
        ShowWindow (window, SW_SHOWNORMAL);
        SetWindowLongPtr(this->window, GWLP_USERDATA, (LONG_PTR) applicationContext);

        if (this->init_surface(applicationContext)) {
            cerr << "Unable to setup the surface" << endl << flush;
            exit(1);
        }
//        this->checkQueueSupportPresenting(applicationContext);
    }

    VkResult Window::init_surface(ApplicationContext *applicationContext) {
        cout << "STEP W002 "<< "initSurface" << endl << flush;
        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.pNext = nullptr;
        createInfo.hinstance = this->connection;
        createInfo.hwnd = this->window;
        VkResult surfaceCreateInfo_res = vkCreateWin32SurfaceKHR(applicationContext->vulkanContext.inst, &createInfo, NULL,
                                                                 &applicationContext->vulkanContext.surfaceKHR);
        assert(surfaceCreateInfo_res == VK_SUCCESS);
        return surfaceCreateInfo_res;
    }
/*
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
*/

}