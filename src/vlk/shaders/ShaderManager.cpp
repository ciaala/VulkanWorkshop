//
// Created by crypt on 11/20/2016.
//

#include "ShaderManager.h"
#include "GLSLUtility.h"


namespace vlk {


    ShaderManager::ShaderManager(VkDevice &virtualDevice) :
            virtualDevice(virtualDevice) {
    }

    void ShaderManager::loadVertexGLSLFromFile(string identifier, string filename) {

    }

    void ShaderManager::loadVertexGLSLFromString(string identifier, string content) {
        VkPipelineShaderStageCreateInfo shaderStage;
        shaderStage.sType =
                VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStage.pNext = NULL;
        shaderStage.pSpecializationInfo = NULL;
        shaderStage.flags = 0;
        shaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStage.pName = "main";


        bool retVal = this->glslUtility.GLSLtoSPV(VK_SHADER_STAGE_VERTEX_BIT, content.data(), vtx_spv);

    }

    ShaderManager::~ShaderManager() {

    }
}