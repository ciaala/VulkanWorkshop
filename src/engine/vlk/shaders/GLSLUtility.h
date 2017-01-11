//
// Created by crypt on 11/20/2016.
//

#ifndef VULKANWORKSHOP_GLSLUTILITY_H
#define VULKANWORKSHOP_GLSLUTILITY_H

#include "vlk.h"
#include <cstdio>
#include "SPIRV/GlslangToSpv.h"

namespace vlk {
    class GLSLUtility {
    public:

        GLSLUtility();

        ~GLSLUtility();

        bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, vector<unsigned int> &spirv);


        EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type);


        void init_resources(TBuiltInResource &Resources);
    };

}
#endif //VULKANWORKSHOP_GLSLUTILITY_H
