#ifndef VAO_H
#define VAO_H

#include <vector>
#include "glm/glm.hpp"

class VAO
{
public:
    VAO();
    std::vector<glm::vec3> vertices;
};

#endif // VAO_H
