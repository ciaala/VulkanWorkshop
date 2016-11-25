//
// Created by crypt on 11/20/2016.
//

#ifndef VULKANWORKSHOP_SHADERMANAGER_H
#define VULKANWORKSHOP_SHADERMANAGER_H

#include "vlk.h"
#include "Shader.h"
#include "GLSLUtility.h"
#include <string>
#include <unordered_map>

namespace vlk {
    class ShaderManager {

        //unordered_map<string, Shader> vertex_shaders;
        //unordered_map<string, Shader> fragment_shaders;
        unordered_map<string, Shader> shaders;
        vector<VkPipelineShaderStageCreateInfo> shaderStages;
        vector<unsigned int> vtx_spv;
        VkDevice &virtualDevice;
        GLSLUtility glslUtility;
    public:
        ShaderManager(VkDevice &virtualDevice);

        ~ShaderManager();

        void loadVertexGLSLFromString(string identifier, string content);

        void loadVertexGLSLFromFile(string identifier, string filename);
    };
}


#endif //VULKANWORKSHOP_SHADERMANAGER_H
