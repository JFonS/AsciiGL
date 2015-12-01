#ifndef PIPELINE_H
#define PIPELINE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Framebuffer.h"
#include "ShaderProgram.h"

class Pipeline
{
private:
    mutable std::vector<GenericMap> fragmentAttributes;

    void  drawLine(const glm::vec3 &v0_3,const glm::vec3 &v1_3,
                   const std::vector<GenericMap> &lineVertexAttributes,
                   Framebuffer &framebuffer) const;

    void  drawTriangle(const glm::vec3 &v0_3, const glm::vec3 &v1_3, const glm::vec3 &v2_3,
                       const std::vector<GenericMap> &triangleVertexAttributes,
                       Framebuffer &framebuffer) const;

    static float edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c);

    glm::vec4 applyVertexShader(const GenericMap &vertexAttributes, int vertex_index) const;
    glm::vec4 applyTriangleFragmentShader(const std::vector<GenericMap> &vertexAttributes, const glm::vec3 &ws,  const glm::vec3 fragmentPos) const;
    glm::vec4 applyLineFragmentShader(const std::vector<GenericMap> &vertexAttributes, float w,  const glm::vec3 fragmentPos) const;

public:

    ShaderProgram program;

    Pipeline();

    void drawVAO(VAO &vao, Framebuffer &framebuffer) const;
};

#endif // PIPELINE_H
