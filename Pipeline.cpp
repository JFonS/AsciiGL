#include "Pipeline.h"

Pipeline::Pipeline()
{
}

void Pipeline::drawLine(const glm::vec2 &p1, const glm::vec2 &p2, VAO &vao, Framebuffer &framebuffer) const
{
    int x1 = round(p1.x), y1 = round(p1.y);
    int x2 = round(p2.x), y2 = round(p2.y);

    int delta_x(x2 - x1);
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) / 2;

    int delta_y(y2 - y1);
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) / 2;

    framebuffer.drawChar(glm::vec3(x1, y1, 0), '*');

    if (delta_x >= delta_y)
    {
        int error(delta_y - (delta_x >> 1));
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            error += delta_y;
            x1 += ix;
            framebuffer.drawChar(glm::vec3(x1, y1, 0), '*');
        }
    }
    else
    {
        int error(delta_x - (delta_y >> 1));
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            error += delta_x;
            y1 += iy;
            framebuffer.drawChar(glm::vec3(x1, y1, 0), '*');
        }
    }
}

void Pipeline::drawTriangle(const glm::vec3 &v0_3,const glm::vec3 &v1_3,const glm::vec3 &v2_3, VAO &vao, Framebuffer &framebuffer) const
{

    glm::vec2 v0(v0_3.x, v0_3.y);
    glm::vec2 v1(v1_3.x, v1_3.y);
    glm::vec2 v2(v2_3.x, v2_3.y);

    float area = edgeFunction(v0, v1, v2);
    float minX = std::min(v0.x, std::min(v1.x, v2.x));
    float maxX = std::max(v0.x, std::max(v1.x, v2.x));
    float minY = std::min(v0.y, std::min(v1.y, v2.y));
    float maxY = std::max(v0.y, std::max(v1.y, v2.y));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            glm::vec2 p(x + 0.5, y + 0.5);
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float z = w0 * v0_3.z + w1 * v1_3.z + w2 * v2_3.z;
                glm::vec3 fragment(x,y,z);
                //char c = applyCharShader(vao, fragment);
                framebuffer.drawChar(fragment, '*');
            }
        }
    }
}

float Pipeline::edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{ return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }

glm::vec4 Pipeline::applyVertexShader(VAO &vao, int vertex_index) const
{
    vao.currentVertexIndex = vertex_index;

    glm::mat4 P = vao.getMat4("project");
    glm::mat4 M = vao.getMat4("model");
    glm::vec4 vertex = glm::vec4(vao.getVec3("position"), 1.0);

    vertex.y *= -1.0;
    vertex = glm::scale(glm::mat4(1.0),glm::vec3(1,0.6,1)) * P * M * vertex;
    return vertex;
}

char Pipeline::applyCharShader(VAO &vao, const glm::vec3 &fragment) const
{
    return '*';
}


void Pipeline::drawVAO(VAO &vao, Framebuffer &framebuffer) const
{
    std::vector<glm::vec3> lines;
    static float rotation = 0.0f;
    rotation += 0.15;
    for (int i = 0; i < vao.size(); ++i)
    {
        glm::vec4 vertex = applyVertexShader(vao,i);

        float w = vertex.w;
        if (vertex.x >= -w && vertex.x <= w &&
            vertex.y >= -w && vertex.y <= w &&
            vertex.z >= -w && vertex.z <= w)
        {
            vertex /= vertex.w;
            vertex *= 0.5f;
            vertex += 0.5f;

            vertex.x *= framebuffer.getWidth();
            vertex.y *= framebuffer.getHeight();

            lines.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
        }
        else
        {
            if (lines.size() > 0) lines.push_back(lines[lines.size()-1]);
        }
    }

    if (lines.size() > 1)
    {
        for (int i = 0; i < lines.size(); i+=3)
        {
            //drawLine(glm::vec2(lines[i]),glm::vec2(lines[i+1]), framebuffer);
            //drawLine(glm::vec2(lines[i+1]),glm::vec2(lines[i+2]), framebuffer);
            //drawLine(glm::vec2(lines[i]),glm::vec2(lines[i+2]), framebuffer);
            drawTriangle(lines[i], lines[i+1], lines[i+2], vao, framebuffer);
        }
    }
}


