#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/glm.hpp"

class FileReader
{
private:
    static void GetOBJFormat(const char *filepath, bool &uvs, bool &normals, bool &triangles);

public:

    FileReader();

    static bool ReadOBJ(const char *filepath, std::vector<glm::vec3> &vertexPos,
                                              std::vector<glm::vec2> &vertexUvs,
                                              std::vector<glm::vec3> &vertexNormals,
                                              bool &triangles);
};

#endif // FILEREADER_H
