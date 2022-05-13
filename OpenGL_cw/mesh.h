#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include "texture.h"
#include "shader.h"
#include <vector>

class Mesh {
public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    
    // draw function
    void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    // set up
    void setupMesh();
};

#endif // !MESH_H
