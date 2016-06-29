cmake_minimum_required(VERSION 3.5)
project(VulkanWorkshop)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++14 -DVK_USE_PLATFORM_XCB_KHR -DVULKAN_SAMPLES_BASE_DIR")
set(VULKAN_SDK /home/crypt/Developer/VulkanSDK/1.0.17.0)

set(SOURCE_FILES
        src/main.cpp
        src/vulkan/VulkanSetup.cpp
        src/vulkan/VulkanSetup.h
      #  src/vulkan/sdk/util.hpp
      #  src/vulkan/sdk/util.cpp
      #  src/vulkan/sdk/util_init.hpp
      #  src/vulkan/sdk/util_init.cpp

        src/vulkan/sdk/sdkut.cpp
        src/vulkan/sdk/sdkut.h)

find_package(XCB REQUIRED)
message(ERROR ${XCB_INCLUDE_DIRS})
include_directories(
        ${VULKAN_SDK}/x86_64/include
        ${VULKAN_SDK}/samples/API-Samples/utils/
        ${XCB_INCLUDE_DIRS})
link_libraries(
        ${XCB_LIBRARIES}
        ${VULKAN_SDK}/x86_64/lib/libvulkan.so
)
add_executable(VulkanWorkshop ${SOURCE_FILES})