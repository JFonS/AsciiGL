#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    vertexShader   = [](){ return glm::vec4(0.0f); };
    fragmentShader = [](){ return glm::vec4(0.0f); };
}
