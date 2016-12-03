//
// Created by crypt on 12/2/2016.
//

#ifndef VULKANWORKSHOP_VULKANPIPELINE_H
#define VULKANWORKSHOP_VULKANPIPELINE_H

#include "vlk.h"

#define NUM_VIEWPORTS 1
#define NUM_SCISSORS NUM_VIEWPORTS
#define NUM_SAMPLES VK_SAMPLE_COUNT_1_BIT
namespace vlk {
    class VulkanPipeline {
    private:
        VkPipelineMultisampleStateCreateInfo ms;
        VkPipelineColorBlendAttachmentState att_state[1];
        VkPipelineColorBlendStateCreateInfo cb;
        VkPipelineDynamicStateCreateInfo dynamicState = {};
        VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
        VkPipelineDepthStencilStateCreateInfo ds;
        VkPipelineViewportStateCreateInfo vp = {};
        VkPipelineRasterizationStateCreateInfo rs;
        VkPipelineVertexInputStateCreateInfo vi;
        VkPipelineInputAssemblyStateCreateInfo ia;

        VkPipeline pipeline;
    private:
        void initDynamicState();

        void initVertexState();

        void initAssemblyState();

        void initRasterizationState();

        void initColorBlendState();

        void initViewportState();

        void initDepthStencil();

        void initMultisampleState();

        void createPipeline();


        VkRenderPass &renderPass;
        VkDevice &virtualDevice;
        pipeline_info &pipelineInfo;
        vector<VkVertexInputBindingDescription> &vertexBinding;
        vector<VkVertexInputAttributeDescription> &vertexAttributes;
        vector<VkPipelineShaderStageCreateInfo> &shaderStages;

    public :
        VulkanPipeline(VkDevice &virtualDevice,
                       VkRenderPass &renderPass,
                       vector<VkVertexInputBindingDescription> &vertexBinding,
                       vector<VkVertexInputAttributeDescription> &vertexAttributes,
                       vector<VkPipelineShaderStageCreateInfo> &shaderStages,
                       pipeline_info &pipelineInfo
        );

        ~VulkanPipeline();

        void init();
    };

}

#endif //VULKANWORKSHOP_VULKANPIPELINE_H
