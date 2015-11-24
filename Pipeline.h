#ifndef PIPELINE_H
#define PIPELINE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VAO.h"
#include "Framebuffer.h"

class Pipeline
{
private:
    void  drawLine(const glm::vec2 &p1, const glm::vec2 &p2, Framebuffer &framebuffer) const;
    void  drawTriangle(const glm::vec3 &v0_3, const glm::vec3 &v1_3, const glm::vec3 &v2_3, Framebuffer &framebuffer) const;

    static float edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c);

public:

    Pipeline();

    void drawVAO(const VAO &vao, Framebuffer &framebuffer) const;
};

#endif // PIPELINE_H
