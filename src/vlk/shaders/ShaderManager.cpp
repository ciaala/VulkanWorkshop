//
// Created by crypt on 11/20/2016.
//

#include "ShaderManager.h"
#include <iostream>
using namespace std;

namespace vlk {


    ShaderManager::ShaderManager(VkDevice &virtualDevice) :
            virtualDevice(virtualDevice) {
    }

/**
 * VK_SHADER_STAGE_VERTEX_BIT |
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

        std::vector<unsigned int> spv_content;
        cout << "Loading shader " << identifier << endl << flush;
        uint64_t index = shaderStages.size();
        shaderStages.resize(index + 1);
        shaderStages[index].sType =
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[index].pNext = nullptr;
        shaderStages[index].pSpecializationInfo = nullptr;
        shaderStages[index].flags = 0;
        shaderStages[index].stage = shaderStageFlagBits;
        shaderStages[index].pName = "main";


        bool retVal = this->glslUtility.GLSLtoSPV(shaderStageFlagBits, content.data(), spv_content);
        assert(retVal);
        VkShaderModuleCreateInfo moduleCreateInfo;

        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.pNext = nullptr;
        moduleCreateInfo.flags = 0;
        moduleCreateInfo.codeSize = spv_content.size() * sizeof(unsigned int);
        moduleCreateInfo.pCode = spv_content.data();
        VkResult res = vkCreateShaderModule(this->virtualDevice, &moduleCreateInfo, nullptr,
                                            &(shaderStages[index].module));
        assert(res == VK_SUCCESS);
        this->shaders[identifier] = index;
    }

    ShaderManager::~ShaderManager() {

    }

    vector<VkPipelineShaderStageCreateInfo> &ShaderManager::getShaderStages() {
        return this->shaderStages;
    }
}