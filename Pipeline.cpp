#include "Pipeline.h"

using namespace agl;

Pipeline::Pipeline() : wireframe(false)
{
}

void Pipeline::drawLine(const glm::vec3 &v0_3,
                        const glm::vec3 &v1_3,
                        const std::vector<GenericMap> &lineVertexAttributes,
                        Framebuffer &framebuffer) const
{
    float length = glm::distance(v0_3, v1_3);

    int x0 = round(v0_3.x), y0 = round(v0_3.y);
    int x1 = round(v1_3.x), y1 = round(v1_3.y);

    int delta_x(x1 - x0);
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) / 2;

    int delta_y(y1 - y0);
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) / 2;

    glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,1.0f,v0_3);
    framebuffer.setPixel(v0_3, color);

    if (delta_x >= delta_y)
    {
        int error(delta_y - (delta_x >> 1));
        while (x0 != x1)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y0 += iy;
            }
            error += delta_y;
            x0 += ix;
            float w = glm::clamp(glm::distance(glm::vec2(x0,y0), glm::vec2(v0_3.x,v0_3.y))/(length+1e-20f),0.0f,1.0f);

            //std::cout << x0 << "," << y0 << " -> " << v0_3.x << "," << v0_3.y << "//" << v1_3.x << "," << v1_3.y << "(" << length << ") => " << w << std::endl;
            float z = v0_3.z*(1.0f-w) + v1_3.z * w;
            glm::vec3 pos(x0, y0, z);
            glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,w,pos);
            framebuffer.setPixel(pos, color);
        }
    }
    else
    {
        int error(delta_x - (delta_y >> 1));
        while (y0 != y1)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x0 += ix;
            }
            error += delta_x;
            y0 += iy;
            float w = glm::distance(glm::vec2(x0,y0), glm::vec2(v0_3))/length;
            float z = v0_3.z*(1.0f-w) + v1_3.z * w;
            glm::vec3 pos(x0, y0, z);
            glm::vec4 color = applyLineFragmentShader(lineVertexAttributes,w,pos);
            framebuffer.setPixel(pos, color);
        }
    }
}

void Pipeline::drawTriangle(const glm::vec3 &v0_3,const
    glm::vec3 &v1_3,
    const glm::vec3 &v2_3,
    const std::vector<GenericMap> &triangleFragmentAttributes,
    Framebuffer &framebuffer) const
{
    glm::vec2 v0(v0_3.x, v0_3.y);
    glm::vec2 v1(v1_3.x, v1_3.y);
    glm::vec2 v2(v2_3.x, v2_3.y);

    int fbWidth = framebuffer.getWidth();
    int fbHeight = framebuffer.getHeight();

    float area = edgeFunction(v0, v1, v2);
    float minX = std::max(0.0f,std::min(v0.x, std::min(v1.x, v2.x)));
    float maxX = std::min(float(fbWidth-1),std::max(v0.x, std::max(v1.x, v2.x)));
    float minY = std::max(0.0f,std::min(v0.y, std::min(v1.y, v2.y)));
    float maxY = std::min(float(fbHeight-1),std::max(v0.y, std::max(v1.y, v2.y)));

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
                w0 /= area; w1 /= area; w2 /= area;
                float z = w0 * v0_3.z + w1 * v1_3.z + w2 * v2_3.z;
                glm::vec3 fragmentPos(x,y,z);
                if (fragmentPos.z <= 1.0f && fragmentPos.z >= 0.0f &&
                    fragmentPos.x <= fbWidth && fragmentPos.x >= 0.0f &&
                    fragmentPos.y <= fbHeight && fragmentPos.y >= 0.0f)
                {
                    glm::vec4 color = applyTriangleFragmentShader(triangleFragmentAttributes, glm::vec3(w0, w1, w2),  fragmentPos);
                    framebuffer.setPixel(fragmentPos, color);
                }
            }
        }
    }
}

float Pipeline::edgeFunction(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{ return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x); }



glm::vec4 Pipeline::applyVertexShader(const GenericMap &vertexAttributes,
                                      GenericMap &fragmentAttributes) const //To fill the fragmentAttributes for each vertex
{
    glm::vec4 vertex = program.vertexShader(vertexAttributes, program.uniforms, fragmentAttributes);
    vertex.y *= -1.0;
    return vertex;
}

glm::vec4 Pipeline::applyTriangleFragmentShader(const std::vector<GenericMap> &vertexAttributes,
    const glm::vec3 &ws,
    const glm::vec3 fragmentPos) const
{
    GenericMap fragmentAttributes;
    GenericMap::interpolateTriangle(vertexAttributes, ws, fragmentAttributes);
    fragmentAttributes.set("fragmentPos", fragmentPos);
    return program.fragmentShader(fragmentAttributes, program.uniforms);
}

glm::vec4 Pipeline::applyLineFragmentShader(const std::vector<GenericMap> &vertexAttributes, float w, const glm::vec3 fragmentPos) const
{
    GenericMap fragmentAttributes;
    GenericMap::interpolateLine(vertexAttributes, w, fragmentAttributes);

    fragmentAttributes.set("fragmentPos", fragmentPos);
    return program.fragmentShader(fragmentAttributes, program.uniforms);
}


inline bool Pipeline::vertexOutOfViewCube(const glm::vec4 &v)
{
    return !(v.x >= -v.w && v.x <= v.w &&
       v.y >= -v.w && v.y <= v.w &&
       v.z >= -v.w && v.z <= v.w);
}


inline void Pipeline::vertexToDeviceCoords(glm::vec4 &v, int w, int h)
{
    //Perspective division it
    v /= v.w;

    //Map it
    v *= 0.5f;
    v += 0.5f;
    v.x *= w;
    v.y *= h;
}

void Pipeline::drawVAO(VAO &vao, Framebuffer &framebuffer) const
{
    int nThreads = 4;
    std::thread threads[nThreads];
    for (int i = 0; i < nThreads; ++i)
    {
        threads[i] = std::thread(&Pipeline::drawVAOthread, this, std::ref(vao), std::ref(framebuffer), nThreads, i);
    }
    for (int i = 0; i < nThreads; ++i) threads[i].join();
}

void Pipeline::drawVAOthread(VAO &vao, Framebuffer &framebuffer, int nThreads, int m) const
{
    int fbWidth = framebuffer.getWidth();
    int fbHeight = framebuffer.getHeight();

    for (unsigned int i = m*3; i < vao.vertexAttributes.size(); i += 3*nThreads)
    {
        GenericMap fragmentAttributes0, fragmentAttributes1, fragmentAttributes2;
        glm::vec4 v0 = applyVertexShader(vao.vertexAttributes[i],   fragmentAttributes0);
        glm::vec4 v1 = applyVertexShader(vao.vertexAttributes[i+1], fragmentAttributes1);
        glm::vec4 v2 = applyVertexShader(vao.vertexAttributes[i+2], fragmentAttributes2);

        if (vertexOutOfViewCube(v0) && vertexOutOfViewCube(v1) && vertexOutOfViewCube(v2)) continue;

        vertexToDeviceCoords(v0, fbWidth, fbHeight);
        vertexToDeviceCoords(v1, fbWidth, fbHeight);
        vertexToDeviceCoords(v2, fbWidth, fbHeight);


        if (this->wireframe) {
            std::vector<GenericMap> triangleFragmentAttributes; //The fragment attributes in each of the 3 tri vertices
            triangleFragmentAttributes = {
                fragmentAttributes0,
                fragmentAttributes1
            };
            drawLine(v0.xyz(),v1.xyz(), triangleFragmentAttributes, framebuffer);

            triangleFragmentAttributes = {
                fragmentAttributes1,
                fragmentAttributes2
            };
            drawLine(v1.xyz(),v2.xyz(), triangleFragmentAttributes, framebuffer);

            triangleFragmentAttributes = {
                fragmentAttributes2,
                fragmentAttributes0
            };
            drawLine(v2.xyz(),v0.xyz(), triangleFragmentAttributes, framebuffer);
        } else {
            std::vector<GenericMap> triangleFragmentAttributes; //The fragment attributes in each of the 3 tri vertices
            triangleFragmentAttributes = {
                fragmentAttributes0,
                fragmentAttributes1,
                fragmentAttributes2
            };
            drawTriangle(v0.xyz(), v1.xyz(), v2.xyz(), triangleFragmentAttributes, framebuffer);
        }
    }
}


