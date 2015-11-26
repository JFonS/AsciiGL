#ifndef VAO_H
#define VAO_H

#include <vector>
#include <map>
#include <tuple>

#include "glm/glm.hpp"

class Pipeline;

class VAO
{

friend class Pipeline;

private:

    int VBOsize;

    std::map<std::string, std::vector<bool> >      vbos_bool;
    std::map<std::string, std::vector<int> >       vbos_int;
    std::map<std::string, std::vector<float> >     vbos_float;
    std::map<std::string, std::vector<double> >    vbos_double;
    std::map<std::string, std::vector<glm::vec2> > vbos_vec2;
    std::map<std::string, std::vector<glm::vec3> > vbos_vec3;
    std::map<std::string, std::vector<glm::vec4> > vbos_vec4;
    std::map<std::string, std::vector<glm::mat2> > vbos_mat2;
    std::map<std::string, std::vector<glm::mat3> > vbos_mat3;
    std::map<std::string, std::vector<glm::mat4> > vbos_mat4;

    int currentVertexIndex;

public:

    VAO();
    int size();

    //TO DO check if adding VBOs of different size
    void addVBO(const std::string &name, const std::vector<bool> &data);
    void addVBO(const std::string &name, const std::vector<int> &data);
    void addVBO(const std::string &name, const std::vector<float> &data);
    void addVBO(const std::string &name, const std::vector<double> &data);
    void addVBO(const std::string &name, const std::vector<glm::vec2> &data);
    void addVBO(const std::string &name, const std::vector<glm::vec3> &data);
    void addVBO(const std::string &name, const std::vector<glm::vec4> &data);
    void addVBO(const std::string &name, const std::vector<glm::mat2> &data);
    void addVBO(const std::string &name, const std::vector<glm::mat3> &data);
    void addVBO(const std::string &name, const std::vector<glm::mat4> &data);

    void setBool(std::string name, bool value);
    void setInt(std::string name,  int value);
    void setFloat(std::string name, float value);
    void setDouble(std::string name, double value);
    void setVec2(std::string name, glm::vec2 value);
    void setVec3(std::string name, glm::vec3 value);
    void setVec4(std::string name, glm::vec4 value);
    void setMat2(std::string name, glm::mat2 value);
    void setMat3(std::string name, glm::mat3 value);
    void setMat4(std::string name, glm::mat4 value);

    bool getBool(std::string name);
    int getInt(std::string name);
    float getFloat(std::string name);
    double getDouble(std::string name);
    glm::vec2 getVec2(std::string name);
    glm::vec3 getVec3(std::string name);
    glm::vec4 getVec4(std::string name);
    glm::mat2 getMat2(std::string name);
    glm::mat3 getMat3(std::string name);
    glm::mat4 getMat4(std::string name);

    //TO DO (V2.0) void removeVBO(std::string name);

    std::vector<glm::vec3> vertices;
};

#endif // VAO_H
