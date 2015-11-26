#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "VAO.h"
#include "GenericMap.h"

class ShaderProgram
{
public:
    GenericMap uniforms;

    std::function<glm::vec4(GenericMap &vertexAttributes, GenericMap &uniforms)> vertexShader;
    std::function<glm::vec4(const GenericMap &fragmentAttributes, GenericMap &uniforms)> fragmentShader;

    ShaderProgram();
};

#endif // SHADERPROGRAM_H
