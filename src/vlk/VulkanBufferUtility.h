//
// Created by crypt on 11/29/2016.
//

#ifndef VULKANWORKSHOP_VULKANBUFFERUTILITY_H
#define VULKANWORKSHOP_VULKANBUFFERUTILITY_H

#include "vlk.h"

namespace vlk {
    class VulkanBufferUtility {
    public:
        static
        void createBuffer(VkDevice &virtualDevice,
                          VkBufferUsageFlags bufferUsageFlags,
                          void *data, uint32_t dataSize,
                          VkPhysicalDeviceMemoryProperties &memoryProperties,
                          vertex_buffer &vertexBuffer);
    };

}

#endif //VULKANWORKSHOP_VULKANBUFFERUTILITY_H
