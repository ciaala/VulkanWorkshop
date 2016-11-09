cmake_minimum_required(VERSION 3.5)
project(VulkanWorkshop)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++14")

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


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(XCB REQUIRED)
    set(VULKAN_SDK /home/crypt/Developer/VulkanSDK/1.0.26.0)
    set(PLATFORM_DIRECTORIES
            ${XCB_INCLUDE_DIRS})
    set(PLATFORM_LIBRARIES
            ${XCB_LIBRARIES}
            ${VULKAN_SDK}/x86_64/lib/libvulkan.so)

    set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} \
            -DVK_USE_PLATFORM_XCB_KHR \
            -DVULKAN_SAMPLES_BASE_DIR")
    set(VULKAN_INCLUDES ${VULKAN_SDK}/x86_64/include)

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(VULKAN_SDK C:\\\\VulkanSDK\\\\1.0.26.0)
    set(VULKAN_INCLUDES ${VULKAN_SDK}\\Include)
    set(VULKAN_LIBRARIES "${VULKAN_SDK}\\\\Source\\\\lib\\\\vulkan-1.dll")
  #  message(FATAL_ERROR ${VULKAN_LIBRARIES})
else ()
    message(FATAL_ERROR "Operating System ${CMAKE_SYSTEM_NAME} is not managed by this build software")
endif ()

include_directories(${VULKAN_INCLUDES} ${PLATFORM_INCLUDES})

link_libraries(${PLATFORM_LIBRARIES} ${VULKAN_LIBRARIES})

add_executable(VulkanWorkshop ${SOURCE_FILES})