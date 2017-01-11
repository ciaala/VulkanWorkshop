//
// Created by crypt on 11/20/2016.
//

#include "vlk.h"
#include "Shader.h"

#ifndef VULKANWORKSHOP_GLSLSHADER_H
#define VULKANWORKSHOP_GLSLSHADER_H

namespace vlk {

class GLSLShader : public Shader {

    VkDevice &virtualDevice;
    GLSLShader(VkDevice &virtualDevice, const char* shaderText);


};
}

#endif //VULKANWORKSHOP_GLSLSHADER_H
