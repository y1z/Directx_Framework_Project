
#include "..//include/cModel.h"
#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "../include/cDevice.h"
#include "../include/cDeviceContext.h"
#include "../include/cConstBuffer.h"
#include "../include/utility/Grafics_libs.h"
#include "enum_headers/enFormatEnums.h"

#include <cassert>
#include <filesystem>

auto comparePaths = [](std::vector<std::string>& alreadyFoundPaths, const char* possibleNewPath) {
  for (const std::string &Path : alreadyFoundPaths)
  {
    if (!Path.compare(possibleNewPath))
    {
      return true;
    }
  }
  return false;
};

cModel::cModel()
  :m_modelPath(""),
  m_materialPath("")
{
  setComponentType(componentTypes::Model);
}


cModel::cModel(std::string_view strView)
  :cModel() 
{
  m_modelPath = strView;
  setReady(true);
}

bool
cModel::LoadModelFromFile(cDevice &device) 
{
  /************** REMOVED BECUASE OF LINKER ERRORS ***************/
  //Assimp::Importer importer;
  //
  //const aiScene* TheScene = importer.ReadFile(filePath,
  //                                            aiProcess_Triangulate |
  //                                            aiProcessPreset_TargetRealtime_MaxQuality |
  //                                            aiProcess_ConvertToLeftHanded);

  const aiScene* Scene = aiImportFile(m_modelPath.c_str(),
                                      aiProcessPreset_TargetRealtime_MaxQuality |
                                      aiProcess_ConvertToLeftHanded);


  std::vector<std::string> texturePaths;

  if (Scene == nullptr)
  {
    aiReleaseImport(Scene);
    return false;
  }
  else
  {
    this->TraversTree(Scene, Scene->mRootNode, device, texturePaths);
    aiReleaseImport(Scene);
    return true;
  }

}

void cModel::DrawMeshes(cDeviceContext & deviceContext, std::vector<cConstBuffer *> &buffers)
{
  CBChangesEveryFrame cb;
  //m_meshes[2].setTopology(Topology::LineList);
  for (cMesh &mesh : m_meshes)
  {
    deviceContext.IASetIndexBuffer(mesh.getIndexBuffer(), Formats::R16);
    deviceContext.IASetVertexBuffers(&mesh.getVertexBuffer(), 1);
    deviceContext.IASetPrimitiveTopology(static_cast<int>(mesh.getTopology()));
    if (mesh.getResource() != nullptr)
    { deviceContext.PSSetShaderResources(mesh); }

    deviceContext.DrawIndexed(mesh.getIndexBuffer().getElementCount(), 0);
  }
#if DIRECTX
  cb.mWorld = dx::XMMatrixIdentity();

#endif // DIRECTX
  // red 
  cb.vMeshColor = {0.6f,0.0f,0.0f,0.0f};
  deviceContext.UpdateSubresource(buffers[0], &cb);
}

void cModel::setModelPath(const std::string_view modelPath) 
{
  this->m_modelPath = modelPath;
  setReady(true);
}

bool cModel::isReady() const
{
  return this->m_Ready;
}

void cModel::Init(cDevice & device, [[maybe_unused]] cDeviceContext & deviceContext)
{
  this->LoadModelFromFile(device);
}

void cModel::Draw(cDeviceContext & devContext,std::vector<cConstBuffer*> &buffers)
{
  DrawMeshes(devContext, buffers);
}

void cModel::Destroy()
{

}


void cModel::TraversTree(const aiScene * scene, aiNode * node, cDevice & device, std::vector<std::string> &texturePaths)
{
  // extract the current mesh
  for (uint32_t i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh *meshes = scene->mMeshes[node->mMeshes[i]];
    this->ExtractMesh(meshes, device, scene, texturePaths);
  }

  // get the children in order to the rest of the meshes 
  for (uint32_t j = 0; j < node->mNumChildren; j++)
  {
    this->TraversTree(scene, node->mChildren[j], device, texturePaths);
  }
}

void cModel::ExtractMesh(const aiMesh * assimpMesh, cDevice &device, const aiScene*scene, std::vector<std::string> &texturePaths)
{
  std::vector<WORD> indices;
#if DIRECTX
  std::vector<SimpleVertex> vertices;
#else
#endif // DIRECTX

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
    vertex.Pos.w  = 1.0f;
    if (assimpMesh->HasTextureCoords(0))
    {
      vertex.Tex.x = static_cast<float>(assimpMesh->mTextureCoords[0][i].x);
      vertex.Tex.y = static_cast<float>(assimpMesh->mTextureCoords[0][i].y);
    }
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

void cModel::CheckTexturePaths(const aiScene * scene, const aiMesh *assimpMesh, std::vector<std::string> &texturePaths)
{

  aiMaterial *material = scene->mMaterials[assimpMesh->mNumAnimMeshes];
  if (material->GetTextureCount(aiTextureType_DIFFUSE));
  {
    aiString FilePath;
    material->GetTexture(aiTextureType_DIFFUSE, 0, &FilePath);
    FilePath.C_Str();
  }

}

void cModel::ExtractTexture(const char * texturePath, cMesh & AfectedMesh, cDevice &device)
{
#if DIRECTX
  HRESULT hr = S_FALSE;

#if UNICODE
  std::int32_t PathLength = MultiByteToWideChar(CP_UTF8, 0, texturePath, -1, NULL, 0);
  wchar_t *Temp = new wchar_t[PathLength];
  MultiByteToWideChar(CP_UTF8, 0, texturePath, -1, Temp, PathLength);

//  DirectX::CreateWICTextreFromFile();
  
  //hr =dx::D3DX11CreateShaderResourceViewFromFile(device.getDevice(), Temp,
  //                                            NULL, NULL,
  //                                            AfectedMesh.getResourceRef(), NULL);
  //hr = dx::CreateWICTextreFromFile() 
  //  HRESULT CreateWICTextureFromFile(

  delete Temp;
#ifndef UNICODE

  hr = D3DX11CreateShaderResourceViewFromFile(device.getDevice(), texturePath,
                                              NULL, NULL,
                                              AfectedMesh.getResourceRef(), NULL);
#endif // !UNICODE

#endif // UNICODE

  if (!SUCCEEDED(hr))
  {
    assert((SUCCEEDED(hr) && "error with creating textures "));
  }
#endif // DIRECTX

}