#include "..//include/cModel.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../include/cDeviceContext.h"
#include "../include/cConstBuffer.h"
#include "../include/utiliy/Grafics_libs.h"
#include <cassert>

bool cModel::LoadModelFromFile(const char * filePath, cDevice &device)
{
  /**************REMOVED BECUASE OF LINKER ERRORS ***************/
  //Assimp::Importer importer;
  //const aiScene* TheScene = importer.ReadFile(filePath,
  //                                            aiProcess_Triangulate |
  //                                            aiProcessPreset_TargetRealtime_MaxQuality |
  //                                            aiProcess_ConvertToLeftHanded);
  const aiScene* Scene = aiImportFile(filePath,
                                      aiProcess_Triangulate |
                                      aiProcessPreset_TargetRealtime_MaxQuality |
                                      aiProcess_ConvertToLeftHanded);

  if (Scene == nullptr)
  {
    return false;
  }
  else
  {
    this->TraversTree(Scene, Scene->mRootNode, device);

    return true;
  }

}

void cModel::DrawMeshes(cDeviceContext & deviceContext,std::vector<cConstBuffer *> &buffers)
{
  CBChangesEveryFrame cb;
  for (cMesh &mesh : m_meshes)
  {
    deviceContext.IASetIndexBuffer(mesh.getIndexBuffer(), 57);
    deviceContext.IASetVertexBuffers(&mesh.getVertexBuffer(), 1);
    deviceContext.DrawIndexed(mesh.getIndexBuffer().getElementCount(), 0); 
  }
  cb.mWorld = XMMatrixIdentity();
  // red 
  cb.vMeshColor = {0.6f,0.0f,0.0f,0.0f};
  deviceContext.UpdateSubresource(buffers[0],&cb);
}


void cModel::TraversTree(const aiScene * scene, aiNode * node, cDevice & device)
{
  // extract the current mesh
  for (uint32_t i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh *meshes = scene->mMeshes[node->mMeshes[i]];
    this->ExtractMesh(meshes, device);
  }
  // get the children in order to the rest of the meshes 
  for (uint32_t j = 0; j < node->mNumChildren; j++)
  {
    this->TraversTree(scene, node->mChildren[j], device);
  }
}

void cModel::ExtractMesh(const aiMesh * assimpMesh, cDevice &device)
{
  std::vector<WORD> indices;
  std::vector<SimpleVertex> vertices;

  // get the indices of a the model 
  for (uint32_t i = 0; i < assimpMesh->mNumFaces; ++i)
  {
    WORD tri[3];
    for (uint32_t j = 0; j < assimpMesh->mFaces[i].mNumIndices; ++j)
    {
      tri[j] = assimpMesh->mFaces[i].mIndices[j];
    }
    indices.emplace_back(tri[0]);
    indices.emplace_back(tri[1]);
    indices.emplace_back(tri[2]);
  }

  // get the vertices of the model
  for (uint32_t i = 0; i < assimpMesh->mNumVertices; ++i)
  {
    SimpleVertex vertex;
    vertex.Pos.x = assimpMesh->mVertices[i].x;
    vertex.Pos.y = assimpMesh->mVertices[i].y;
    vertex.Pos.z = assimpMesh->mVertices[i].z;
    vertices.emplace_back(vertex);
  }

  cMesh result;
  result.initIndexBuffer(indices);
  result.initVertexBuffer(vertices);
  bool IndexSuccessful = result.createIndexBuffer(device);
  bool VertexSuccessful = result.createVertexBuffer(device);

  assert((IndexSuccessful & VertexSuccessful, "error with index or vertex buffer creation"));
  m_meshes.emplace_back(std::move(result));
}
