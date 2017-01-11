//
// Created by crypt on 11/29/2016.
//

#include <cassert>
#include "VulkanBufferUtility.h"
#include "VulkanUtility.h"

namespace vlk {
    /**
     *
     * @param bufferUsageFlags (VK_BUFFER_USAGE_VERTEX_BUFFER_BIT || )
     */
    void VulkanBufferUtility::createBuffer(
            VkDevice &virtualDevice,
            VkBufferUsageFlags bufferUsageFlags,
            void *data,
            uint32_t dataSize,
            VkPhysicalDeviceMemoryProperties &memoryProperties,
            vertex_buffer &vertexBuffer)
    {
        VkBufferCreateInfo buf_info = {};
        buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buf_info.pNext = NULL;
        buf_info.usage = bufferUsageFlags;
        buf_info.size = dataSize;
        buf_info.queueFamilyIndexCount = 0;
        buf_info.pQueueFamilyIndices = NULL;
        buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        buf_info.flags = 0;
        VkResult res = vkCreateBuffer(virtualDevice, &buf_info, NULL, &vertexBuffer.buf);
        assert(res == VK_SUCCESS);


        VkMemoryRequirements mem_reqs;
        vkGetBufferMemoryRequirements(virtualDevice, vertexBuffer.buf,
                                      &mem_reqs);

        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.pNext = NULL;
        alloc_info.memoryTypeIndex = 0;

        alloc_info.allocationSize = mem_reqs.size;
        bool pass = VulkanUtility::memory_type_from_properties(memoryProperties, mem_reqs.memoryTypeBits,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                           &alloc_info.memoryTypeIndex);
        assert(pass && "No mappable, coherent memory");

        res = vkAllocateMemory(virtualDevice, &alloc_info, NULL,
                               &(vertexBuffer.mem));
        assert(res == VK_SUCCESS);

        uint8_t *pData;
        res = vkMapMemory(virtualDevice, vertexBuffer.mem, 0, mem_reqs.size, 0,
                          (void **)&pData);
        assert(res == VK_SUCCESS);

        memcpy(pData, data,
               dataSize);

        vkUnmapMemory(virtualDevice, vertexBuffer.mem);

        res = vkBindBufferMemory(virtualDevice, vertexBuffer.buf,
                                 vertexBuffer.mem, 0);
        assert(res == VK_SUCCESS);

    }
}