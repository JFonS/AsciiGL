#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    vertexShader = [](const GenericMap &vertexAttributes, const GenericMap &uniforms, GenericMap &fragmentAttributes)
    {
        return glm::vec4(0.0f);
    };

    fragmentShader = [](const GenericMap &fragmentAttributes, const GenericMap &uniforms)
    {
        return glm::vec4(0.0f);
    };
}
