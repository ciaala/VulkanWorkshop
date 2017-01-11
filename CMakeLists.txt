cmake_minimum_required(VERSION 3.6)
project(VulkanWorkshop)
set (CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(COMMON_COMPILE_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers")

set(CMAKE_VERBOSE_MAKEFILE ON)
set(VULKAN_VERSION 1.0.30.0)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        src/main.cpp
        src/engine/vlk/vlk.h
        src/engine/vlk/VulkanContext.cpp
        src/engine/vlk/VulkanContext.h
        src/engine/vlk/ApplicationContext.cpp
        src/engine/vlk/ApplicationContext.h
        src/engine/vlk/Window.cpp
        src/engine/vlk/Window.h
        src/engine/vlk/VulkanUtility.cpp
        src/engine/vlk/VulkanUtility.h
        src/engine/vlk/shaders/GLSLShader.cpp
        src/engine/vlk/shaders/GLSLShader.h
        src/engine/vlk/shaders/ShaderManager.cpp
        src/engine/vlk/shaders/ShaderManager.h
        src/engine/vlk/shaders/Shader.cpp
        src/engine/vlk/shaders/Shader.h
        src/engine/vlk/shaders/GLSLUtility.cpp
        src/engine/vlk/shaders/GLSLUtility.h
        src/engine/vlk/VulkanBufferUtility.cpp
        src/engine/vlk/VulkanBufferUtility.h
        src/engine/vlk/VulkanPipeline.cpp
        src/engine/vlk/VulkanPipeline.h


        src/engine/vlk/data/cube_data.h src/engine/vlk/VertexBufferManager.cpp src/engine/vlk/VertexBufferManager.h)


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(XCB REQUIRED)
    set(VULKAN_SDK /home/crypt/Developer/VulkanSDK/${VULKAN_VERSION})
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
    set(VULKAN_SDK C:/VulkanSDK/${VULKAN_VERSION})
    set(VULKAN_INCLUDES ${VULKAN_SDK}/Include)
    set(VULKAN_GLSDK_INCLUDES ${VULKAN_SDK}/glslang)
    set(VULKAN_SAMPLES ${VULKAN_SDK}/Samples/utils)
    set(VULKAN_LIBRARIES "${VULKAN_SDK}/Source/lib/vulkan-1.dll")

    add_library(glslang     STATIC IMPORTED)
    add_library(OGLCompiler STATIC IMPORTED)
    add_library(OSDependent STATIC IMPORTED)
    add_library(HLSL        STATIC IMPORTED)
    add_library(SPIRV       STATIC IMPORTED)
    add_library(Loader      STATIC IMPORTED)
    add_library(SPIRV-Tools STATIC IMPORTED)

    set (GLSLANG_BUILD_DIR ${VULKAN_SDK}/glslang/build-mingw)
    set (GLSLANG_DEBUG_SEARCH_PATH
            "${GLSLANG_BUILD_DIR}/glslang/Debug"
            "${GLSLANG_BUILD_DIR}/glslang/OSDependent/Windows/Debug"
            "${GLSLANG_BUILD_DIR}/hlsl/Debug"
            "${GLSLANG_BUILD_DIR}/OGLCompilersDLL/Debug"
            "${GLSLANG_BUILD_DIR}/SPIRV/Debug")

    #find_library(GLSLANG_LIB NAMES glslang        HINTS ${VULKAN_SDK}/glslang/build/glslang/Release )

    find_library(GLSLANG_DLIB NAMES glslang libglslang.a HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(OGLCompiler_DLIB NAMES OGLCompiler HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(OSDependent_DLIB NAMES OSDependent HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(HLSL_DLIB NAMES HLSL HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )
    find_library(SPIRV_DLIB NAMES SPIRV HINTS ${GLSLANG_DEBUG_SEARCH_PATH} )

    set_target_properties(glslang PROPERTIES
            IMPORTED_LOCATION       "${GLSLANG_LIB}"
            IMPORTED_LOCATION_DEBUG "${GLSLANG_DLIB}")
    set_target_properties(OGLCompiler PROPERTIES
            IMPORTED_LOCATION       "${OGLCompiler_LIB}"
            IMPORTED_LOCATION_DEBUG "${OGLCompiler_DLIB}")
    set_target_properties(OSDependent PROPERTIES
            IMPORTED_LOCATION       "${OSDependent_LIB}"
            IMPORTED_LOCATION_DEBUG "${OSDependent_DLIB}")
    set_target_properties(HLSL PROPERTIES
            IMPORTED_LOCATION       "${HLSL_LIB}"
            IMPORTED_LOCATION_DEBUG "${HLSL_DLIB}")
    set_target_properties(SPIRV PROPERTIES
            IMPORTED_LOCATION       "${SPIRV_LIB}"
            IMPORTED_LOCATION_DEBUG "${SPIRV_DLIB}")
    #set (GLSLANG_LIBRARIES glslang OGLCompiler OSDependent HLSL SPIRV)
    set (GLSLANG_LIBRARIES
            ${GLSLANG_BUILD_DIR}/glslang/libglslang.a
            ${GLSLANG_BUILD_DIR}/OGLCompilersDLL/libOGLCompiler.a
            ${GLSLANG_BUILD_DIR}/glslang/OSDependent/windows/libOSDependent.a
            ${GLSLANG_BUILD_DIR}/SPIRV/libSPIRV.a
            ${GLSLANG_BUILD_DIR}/SPIRV/libSPVRemapper.a
            ${GLSLANG_BUILD_DIR}/hlsl/libHLSL.a
            )
    # "${VULKAN_SDK}/glslang/build/glslang/Debug/glslangd.lib" )
else ()
    message(FATAL_ERROR "Operating System ${CMAKE_SYSTEM_NAME} is not managed by this build software")
endif ()

include_directories(${VULKAN_INCLUDES} src/engine/vlk ${PROJECT_SOURCE_DIR}/src/data ${PLATFORM_INCLUDES} ${VULKAN_GLSDK_INCLUDES})
# ${VULKAN_SAMPLES}

add_executable(VulkanWorkshop ${SOURCE_FILES})
target_link_libraries(
        VulkanWorkshop

       PUBLIC ${GLSLANG_LIBRARIES}
        PUBLIC ${PLATFORM_LIBRARIES}
        PUBLIC ${VULKAN_LIBRARIES}
)
message(target_link_libraries
        ${GLSLANG_LIBRARIES}
        ${PLATFORM_LIBRARIES}
        ${VULKAN_LIBRARIES}
)
#SET( SAMPLE_FILES src/sample/15-draw_cube.cpp )
#
#add_executable(draw-cube ${SAMPLE_FILES})
