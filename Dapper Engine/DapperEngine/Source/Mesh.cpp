//-----------------------------------------------------------------------------
//
// File Name:	Mesh.cpp
// Author(s):	Luke Mostert
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------
#include "Mesh.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "rapidjson/istreamwrapper.h"
#include "Utility.h"

//start of dapper namespace
namespace Dapper
{

  Mesh::Mesh(int the_ID, Engine& theEngine) : 
    ID(the_ID), 
    engine(theEngine)
  {
    faceVAO = 0;
    edgeVAO = 0;
    VBO = 0;
    faceIBO = 0;
    edgeIBO = 0;
    projection[0];
    model[0];
    AllocateMesh();
  }

  Mesh::Mesh(Mesh&& mesh) noexcept : 
    faceVAO(mesh.faceVAO), 
    edgeVAO(mesh.edgeVAO), 
    VBO(mesh.VBO), 
    faceIBO(mesh.faceIBO), 
    edgeIBO(mesh.edgeIBO), 
    shaderID((mesh.shaderID)),
    vertices(std::move(mesh.vertices)),
    indices(std::move(mesh.indices)), 
    edges(std::move(mesh.edges)),
    engine(mesh.engine), 
    the_name(std::move(mesh.the_name)), 
    ID(mesh.ID)
  {
    mesh.VBO = 0;
    mesh.faceVAO = 0;
    mesh.edgeVAO = 0;
    mesh.faceIBO = 0;
    mesh.edgeIBO = 0;
  }

  Mesh::~Mesh()
  {
    ClearMesh();
  }

  void Mesh::CreateMesh(GLfloat* thevertices, unsigned int* theindices, unsigned int* theEdges, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfEdges, std::string name)
  {
    the_name = name;

    for (unsigned i = 0; i < numOfVertices; ++i)
    {
        vertices.push_back(thevertices[i]);
    }

    for (unsigned i = 0; i < numOfIndices; ++i)
    {
        indices.push_back(theindices[i]);
    }

    for (unsigned i = 0; i < numOfEdges; ++i)
    {
      edges.push_back(theEdges[i]);
    }

    //vbo data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //put vertices into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(thevertices[0]) * numOfVertices,
                                  thevertices, GL_DYNAMIC_DRAW);

    //edge data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(theEdges[0]) * numOfEdges,
                                          theEdges, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //FACE STUFF
    //face data
    glGenBuffers(1, &faceIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(theindices[0]) * numOfIndices,
                                          theindices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //generate and bind the vao
    glGenVertexArrays(1, &faceVAO);
    glBindVertexArray(faceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//&&&Adam 
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
      (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);///&&&Adam
    //generate and bind the ibo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);
    //puts indices data into buffer
    glBindVertexArray(0);


    //EDGE STUFF
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &edgeIBO);
    
    glGenVertexArrays(1, &edgeVAO);
    glBindVertexArray(edgeVAO);  
    //specify index, size, type, if it should be normalized,byte offset(stride),
  //offset of first component of first generic attrib in array (pointer)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//&&&Adam 
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
      (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);///&&&Adam
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    glBindVertexArray(0);

  }

  //For Testing Mesh's
  void Mesh::SquareMesh()
  {
    float vertices1[] = {
     0.5f,  0.5f, 0.0f,  // top right
    -0.5f,  0.5f, 0.0f, // top left 
    -0.5f, -0.5f, 0.0f,  // bottom left
     0.5f, -0.5f, 0.0f  // bottom right
    };
    unsigned int indices1[] = {  // note that we start from 0!
        0, 1, 2,   // first triangle
        0, 2, 3    // second triangle
    };
    //Dapper::Mesh* square = new Dapper::Mesh(0);
    //square->CreateMesh(vertices, indices, 12, 6, "SquareMesh");
  }

  void Mesh::DrawMesh(GLuint uniform_model, GLuint uniform_projection, GLuint uniform_view, Shader shader, float x_pos, float y_pos, float x_scale, float y_scale, glm::mat4 theprojection, Camera* camera)
  {
    uniform_model = shader.GetModelLocation();
    uniform_projection = shader.GetProjectionLocation();
    uniform_view = shader.GetViewLocation();
 
    glm::mat4 aModel = camera->calculateViewMatrix(); // View Matrix (worldspace -> screenspace)

    aModel = glm::translate(aModel, glm::vec3(x_pos, y_pos, -5.0f));
    aModel = glm::scale(aModel, glm::vec3(x_scale, y_scale , 1.0f));
    (glUniformMatrix4fv(uniform_model, 1, GL_FALSE, &aModel[0][0]));
    (glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, &theprojection[0][0]));
    RenderMesh();
  }

  void Mesh::AllocateMesh()
  {

      //generate and bind the vbo
      glGenBuffers(1, &VBO);

      //generate and bind the vao
      glGenVertexArrays(1, &faceVAO);
      glBindVertexArray(faceVAO);

      // bind vertex buffer to vertex array
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      //generate and bind the ibo
      glGenBuffers(1, &faceIBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);

      glGenVertexArrays(1, &edgeVAO);
      glBindVertexArray(edgeVAO);

      // bind vertex buffer to vertex array
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      //generate and bind the ibo
      glGenBuffers(1, &edgeIBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);
  }

  void Mesh::SetVertices(std::vector<float>& new_vertices)
  {
    // Set the new vertices in the mesh class data
    vertices = new_vertices;

    glBindVertexArray(faceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
      &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//&&&Adam 

    glEnableVertexAttribArray(1);///&&&Adam
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
      (void*)(3 * sizeof(float)));//&&&Adam trying to get texture working


    glBindVertexArray(0);
  }

  void Mesh::SetFaces(std::vector<unsigned>& new_indices)
  {
    // Set the new indices in the mesh class data
    indices = new_indices;

    // Allow OpenGL to set the new indices
    glBindVertexArray(faceVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(),
      &indices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
  }

  void Mesh::SetEdges(std::vector<unsigned>& new_indices)
  {
    new_indices;
    ////edge data
    //int num_edges = new
    //for (unsigned i = 0; i < numOfEdges; ++i)
    //{
    //  edges.push_back(theEdges[i]);
    //}
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(theEdges[0]) * numOfEdges,
    //  theEdges, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void Mesh::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    //auto object = jsonVal.GetObject();
    deserial;
    the_name = jsonVal["Name"].GetString();
    rapidjson::GenericArray vertices_array = jsonVal["Vertices"].GetArray();
    rapidjson::GenericArray edges_array = jsonVal["Edges"].GetArray();
    rapidjson::GenericArray indices_array = jsonVal["Indices"].GetArray();

    //get the shader from shader manager
    auto shaderInfo =  deserial.Deserialize("Shader", jsonVal["Shader"]);
    auto shaderName = jsonVal["Shader"].GetString();

    ShaderManager* theShaderManager = dynamic_cast<ShaderManager*>
        (deserial.GetEngine().Get("ShaderManager"));
    auto theShader = theShaderManager->HasShader(shaderName);

    shaderID = theShader.second;

    int size = vertices_array.Size();
    for (int i = 0; i < size; i++)
    {
      vertices.push_back((vertices_array[i].GetFloat()));

    }

    size = indices_array.Size();
    for (int i = 0; i < size; i++)
    {
      indices.push_back((indices_array[i].GetUint()));
    }

    size = edges_array.Size();
    for (int i = 0; i < size; i++)
    {
      edges.push_back((edges_array[i].GetUint()));
    }

    //AllocateMesh();

    //glBindVertexArray(faceVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(),
        indices.data(), GL_STATIC_DRAW);

    //glBindVertexArray(edgeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(edges[0]) * edges.size(),
       edges.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //put vertices into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(faceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //specify index, size, type, if it should be normalized,byte offset(stride), 
    //offset of first component of first generic attrib in array (pointer)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//&&&Adam 

    glEnableVertexAttribArray(1);///&&&Adam
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        (void*)(3 * sizeof(float)));//&&&Adam trying to get texture working
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);
    glBindVertexArray(0);

    //EDGES
    glBindVertexArray(edgeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //specify index, size, type, if it should be normalized,byte offset(stride), 
    //offset of first component of first generic attrib in array (pointer)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);//&&&Adam 

    glEnableVertexAttribArray(1);///&&&Adam
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
      (void*)(3 * sizeof(float)));//&&&Adam trying to get texture working
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);
    glBindVertexArray(0);

    //Clean up after yo self
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    /*glBindVertexArray(0);*/
  }

  //Reads and parses a json file
  void Mesh::ReadFromFile(Deserializer& deserial, std::string fileName)
  {
      rapidjson::Document theDoc;
      std::ifstream fileRead(fileName);
      if (fileRead)
      {
          rapidjson::IStreamWrapper theWrapper(fileRead);
          theDoc.ParseStream(theWrapper);
          Read(deserial, theDoc["Mesh"].GetObject());
      }
  }
  
  void Mesh::RenderMesh()
  {

    //draws the mesh

    //draw faces
    //binds to gpu for use
    glBindVertexArray(faceVAO);
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIBO);*/
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //draw edges
    glBindVertexArray(edgeVAO);
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeIBO);*/
    glDrawElements(GL_LINES, (GLsizei)edges.size(), GL_UNSIGNED_INT, 0);
   /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    glBindVertexArray(0);
  }

  void Mesh::ClearMesh()
  {
    //clearning the mesh so any objects used are deleted and set to 0
    if (faceIBO != 0)
    {
      glDeleteBuffers(1, &faceIBO);
      faceIBO = 0;
    }

    if (edgeIBO != 0)
    {
      glDeleteBuffers(1, &edgeIBO);
      edgeIBO = 0;
    }

    if (VBO != 0)
    {
      glDeleteBuffers(1, &VBO);
      VBO = 0;
    }

    if (faceVAO != 0)
    {
      glDeleteVertexArrays(1, &faceVAO);
      faceVAO = 0;
    }

    if (edgeVAO != 0)
    {
      glDeleteVertexArrays(1, &edgeVAO);
      edgeVAO = 0;
    }

  }

}//end of dapper namesapce
