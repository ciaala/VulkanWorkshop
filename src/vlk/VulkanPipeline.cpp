//
// Created by crypt on 12/2/2016.
//

#include "VulkanPipeline.h"
#include <cassert>

namespace vlk {
    void VulkanPipeline::init() {
        this->initDynamicState();
        this->initVertexState();
        this->initAssemblyState();
        this->initRasterizationState();
        this->initColorBlendState();
        this->initViewportState();
        this->initDepthStencil();
        this->initMultisampleState();
        this->createPipeline();
    }

    void VulkanPipeline::initDynamicState() {
        memset(dynamicStateEnables, 0, sizeof(dynamicStateEnables));
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = nullptr;
        dynamicState.pDynamicStates = dynamicStateEnables;
        dynamicState.dynamicStateCount = 0;
    }

    void VulkanPipeline::initVertexState() {
        vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vi.pNext = nullptr;
        vi.flags = 0;
        vi.vertexBindingDescriptionCount = (uint32_t) this->vertexBinding.size();
        vi.pVertexBindingDescriptions = this->vertexBinding.size() ? this->vertexBinding.data() : nullptr;
        vi.vertexAttributeDescriptionCount = (uint32_t) this->vertexAttributes.size();
        vi.pVertexAttributeDescriptions = this->vertexAttributes.size() > 0 ? this->vertexAttributes.data() : nullptr;

    }

    void VulkanPipeline::initAssemblyState() {
        ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ia.pNext = nullptr;
        ia.flags = 0;
        ia.primitiveRestartEnable = VK_FALSE;
        ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }

    void VulkanPipeline::initRasterizationState() {
        rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rs.pNext = nullptr;
        rs.flags = 0;
        rs.polygonMode = VK_POLYGON_MODE_FILL;
        rs.cullMode = VK_CULL_MODE_BACK_BIT;
        rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rs.depthClampEnable = VK_TRUE;
        rs.rasterizerDiscardEnable = VK_FALSE;
        rs.depthBiasEnable = VK_FALSE;
        rs.depthBiasConstantFactor = 0;
        rs.depthBiasClamp = 0;
        rs.depthBiasSlopeFactor = 0;
        rs.lineWidth = 1.0f;
    }

    void VulkanPipeline::initColorBlendState() {
        this->cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        this->cb.pNext = nullptr;
        this->cb.flags = 0;
        att_state[0].colorWriteMask = 0xf;
        att_state[0].blendEnable = VK_FALSE;
        att_state[0].alphaBlendOp = VK_BLEND_OP_ADD;
        att_state[0].colorBlendOp = VK_BLEND_OP_ADD;
        att_state[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        att_state[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        att_state[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        att_state[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        this->cb.attachmentCount = 1;
        this->cb.pAttachments = att_state;
        this->cb.logicOpEnable = VK_FALSE;
        this->cb.logicOp = VK_LOGIC_OP_NO_OP;
        this->cb.blendConstants[0] = 1.0f;
        this->cb.blendConstants[1] = 1.0f;
        this->cb.blendConstants[2] = 1.0f;
        this->cb.blendConstants[3] = 1.0f;
    }

    void VulkanPipeline::initViewportState() {
        vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vp.pNext = nullptr;
        vp.flags = 0;
        vp.viewportCount = NUM_VIEWPORTS;
        dynamicStateEnables[dynamicState.dynamicStateCount++] =
                VK_DYNAMIC_STATE_VIEWPORT;
        vp.scissorCount = NUM_SCISSORS;
        dynamicStateEnables[dynamicState.dynamicStateCount++] =
                VK_DYNAMIC_STATE_SCISSOR;
        vp.pScissors = nullptr;
        vp.pViewports = nullptr;

    }

    void VulkanPipeline::initDepthStencil() {
        ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        ds.pNext = nullptr;
        ds.flags = 0;
        ds.depthTestEnable = VK_TRUE;
        ds.depthWriteEnable = VK_TRUE;
        ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        ds.depthBoundsTestEnable = VK_FALSE;
        ds.minDepthBounds = 0;
        ds.maxDepthBounds = 0;
        ds.stencilTestEnable = VK_FALSE;
        ds.back.failOp = VK_STENCIL_OP_KEEP;
        ds.back.passOp = VK_STENCIL_OP_KEEP;
        ds.back.compareOp = VK_COMPARE_OP_ALWAYS;
        ds.back.compareMask = 0;
        ds.back.reference = 0;
        ds.back.depthFailOp = VK_STENCIL_OP_KEEP;
        ds.back.writeMask = 0;
        ds.front = ds.back;
    }

    void VulkanPipeline::initMultisampleState() {
        ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        ms.pNext = nullptr;
        ms.flags = 0;
        ms.pSampleMask = nullptr;
        ms.rasterizationSamples = NUM_SAMPLES;
        ms.sampleShadingEnable = VK_FALSE;
        ms.alphaToCoverageEnable = VK_FALSE;
        ms.alphaToOneEnable = VK_FALSE;
        ms.minSampleShading = 0.0;
    }

    void VulkanPipeline::createPipeline() {
        VkGraphicsPipelineCreateInfo pipelineCreateInfo;
        pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.pNext = nullptr;
        pipelineCreateInfo.layout = this->pipelineInfo.pipelineLayout;
        pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineCreateInfo.basePipelineIndex = 0;
        pipelineCreateInfo.flags = 0;
        pipelineCreateInfo.pVertexInputState = &this->vi;
        pipelineCreateInfo.pInputAssemblyState = &this->ia;
        pipelineCreateInfo.pRasterizationState = &this->rs;
        pipelineCreateInfo.pColorBlendState = &this->cb;
        pipelineCreateInfo.pTessellationState = nullptr;
        pipelineCreateInfo.pMultisampleState = &this->ms;
        pipelineCreateInfo.pDynamicState = &this->dynamicState;
        pipelineCreateInfo.pViewportState = &this->vp;
        pipelineCreateInfo.pDepthStencilState = &this->ds;
        pipelineCreateInfo.pStages = this->shaderStages.size() > 0 ? this->shaderStages.data() : nullptr;
        pipelineCreateInfo.stageCount = (uint32_t) this->shaderStages.size();
        pipelineCreateInfo.renderPass = this->renderPass;
        pipelineCreateInfo.subpass = 0;

        VkResult res = vkCreateGraphicsPipelines(this->virtualDevice, VK_NULL_HANDLE, 1,
                                                 &pipelineCreateInfo, nullptr, &this->pipeline);
        assert(res == VK_SUCCESS);

    }

    VulkanPipeline::~VulkanPipeline() {
        vkDestroyPipeline(this->virtualDevice, this->pipeline, nullptr);
    }

    VulkanPipeline::VulkanPipeline(VkDevice &virtualDevice,
                                   VkRenderPass &renderPass,
                                   vector<VkVertexInputBindingDescription> &vertexBinding,
                                   vector<VkVertexInputAttributeDescription> &vertexAttributes,
                                   vector<VkPipelineShaderStageCreateInfo> &shaderStages,
                                   pipeline_info &pipelineInfo) :
            virtualDevice(virtualDevice),
            renderPass(renderPass),
            vertexBinding(vertexBinding),
            vertexAttributes(vertexAttributes),
            shaderStages(shaderStages),
            pipelineInfo(pipelineInfo) {


    }
}
