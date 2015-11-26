#ifndef GENERICMAP_H
#define GENERICMAP_H

#include <map>

#include "glm/glm.hpp"

class GenericMap
{
private:
    std::map<std::string, bool>      map_bool;
    std::map<std::string, int>       map_int;
    std::map<std::string, float>     map_float;
    std::map<std::string, double>    map_double;
    std::map<std::string, glm::vec2> map_vec2;
    std::map<std::string, glm::vec3> map_vec3;
    std::map<std::string, glm::vec4> map_vec4;
    std::map<std::string, glm::mat2> map_mat2;
    std::map<std::string, glm::mat3> map_mat3;
    std::map<std::string, glm::mat4> map_mat4;

public:
    GenericMap();

    void set(std::string name, bool value);
    void set(std::string name, int value);
    void set(std::string name, float value);
    void set(std::string name, double value);
    void set(std::string name, glm::vec2 value);
    void set(std::string name, glm::vec3 value);
    void set(std::string name, glm::vec4 value);
    void set(std::string name, glm::mat2 value);
    void set(std::string name, glm::mat3 value);
    void set(std::string name, glm::mat4 value);

    //TO DO: warnings if value not found
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
};

#endif // GENERICMAP_H
