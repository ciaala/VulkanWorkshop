//
// Created by crypt on 11/20/2016.
//

#include "ShaderManager.h"


namespace vlk {


    ShaderManager::ShaderManager(VkDevice &virtualDevice) :
            virtualDevice(virtualDevice) {
    }

/**
 *
 *
 * @param identifier
 * @param shaderStage
 * @param flagBits VK_SHADER_STAGE_VERTEX_BIT
 * @param content
 */
    void ShaderManager::loadGLSLFromString(
            string identifier,
            VkShaderStageFlagBits shaderStageFlagBits,
            string content) {
        uint64_t index = shaderStages.size();
        shaderStages.resize(index + 1);
        shaderStages[index].sType =
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[index].pNext = NULL;
        shaderStages[index].pSpecializationInfo = NULL;
        shaderStages[index].flags = 0;
        shaderStages[index].stage = shaderStageFlagBits;
        shaderStages[index].pName = "main";


        bool retVal = this->glslUtility.GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, content.data(), vtx_spv);
        assert(retVal);
        VkShaderModuleCreateInfo moduleCreateInfo;

        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.pNext = NULL;
        moduleCreateInfo.flags = 0;
        moduleCreateInfo.codeSize = vtx_spv.size() * sizeof(unsigned int);
        moduleCreateInfo.pCode = vtx_spv.data();
        VkResult res = vkCreateShaderModule(this->virtualDevice, &moduleCreateInfo, NULL,
                                            &(shaderStages[index].module));
        assert(res == VK_SUCCESS);

    }

    ShaderManager::~ShaderManager() {

    }

    vector<VkPipelineShaderStageCreateInfo> &ShaderManager::getShaderStages() {
        return this->shaderStages;
    }
}