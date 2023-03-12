//-----------------------------------------------------------------------------
//
// File Name:	Mesh.h
// Author(s):	Adam Tackett (Luke Mostert)
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Shader.h"
#include "Engine.h"

#include "Camera.h"
#include "Deserializer.h"
//start of dapper namespace
namespace Dapper {

  class Mesh
  {
  public:
    Mesh(int the_ID, Engine& theEngine);
    Mesh(Mesh&& mesh) noexcept;
    Mesh(const Mesh& mesh) = default;
    ~Mesh();

    void CreateMesh(GLfloat* thevertices, unsigned int* theindices, unsigned int* theEdges, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfEdges, std::string name);
    void RenderMesh();
    void ClearMesh();
    void SquareMesh();
    void DrawMesh(GLuint uniform_model, GLuint uniform_projection, GLuint uniform_view, Shader shader, float x_pos, float y_pos, float x_scale, float y_scale, glm::mat4 theprojection, Camera* camera);
    void AllocateMesh();

   // void CreateSpineMesh( std::vector<float> theVertices, std::vector<unsigned> theFaces, std::vector<unsigned> theEdges);
    void SetVertices(std::vector<float>& new_vertices);
    void SetFaces(std::vector<unsigned>& new_indices);
    void SetEdges(std::vector<unsigned>& new_indices);

    inline int GetShader() { return shaderID; }

    inline const std::string& GetName() { return the_name; }

    inline std::string_view GetTypeName() { return type_name; }

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);

    void ReadFromFile(Deserializer& deserial, std::string fileName);

  private:
    std::string the_name;
    static const inline std::string type_name = "Mesh";
    GLuint faceVAO, edgeVAO, VBO, faceIBO, edgeIBO;

    int shaderID;
    glm::mat4 projection;
    glm::mat4 model;

    std::vector <float> vertices;
    std::vector <unsigned int> indices;
    std::vector <unsigned int> edges;

    Engine& engine;

    // The name of the Mesh

    int ID;
  };

}//end of dapper namespace

