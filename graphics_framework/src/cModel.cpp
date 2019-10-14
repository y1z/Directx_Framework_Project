#include "..//include/cModel.h"
#include "assimp/cimport.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/material.h"
#include "../include/cDevice.h"
#include "../include/cDeviceContext.h"
#include "../include/cConstBuffer.h"
#include "../include/utility/Grafics_libs.h"
#include "enum_headers/enFormatEnums.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "utility/CustomStructs.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <WICTextureLoader.h>

namespace fs = std::filesystem;

auto comparePaths = [](std::vector<std::string>& alreadyFoundPaths, const char* possibleNewPath) {
  for (const std::string &Path : alreadyFoundPaths)
  {
    if (!Path.compare(possibleNewPath))
    {
      return false;
    }
  }
  return true;
};

cModel::cModel()
// init the identity matrix 
{
  m_transform.matrix = glm::identity<glm::mat4>();
  setComponentType(componentTypes::Model);
}

cModel::cModel(std::string_view strView)
  : cModel()
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

void
cModel::DrawMeshes(cDeviceContext & deviceContext, std::vector<cConstBuffer *> &buffers, const sColorf &color)
{
  GlChangeEveryFrame Cb;
  //m_meshes[2].setTopology(Topology::LineList);
  const glm::mat4 Identidad(1.0f);

  for (cMesh &mesh : m_meshes)
  {
    glm::mat4 Transform(Identidad * this->m_transform.matrix);
    mesh.setTransform(Transform);
    deviceContext.IASetIndexBuffer(mesh.getIndexBuffer(), Formats::R16);
    deviceContext.IASetVertexBuffers(&mesh.getVertexBuffer(), 1);
    deviceContext.IASetPrimitiveTopology(static_cast<int>(mesh.getTopology()));
    if (mesh.getResource() != nullptr)
    { deviceContext.PSSetShaderResources(mesh); }

    deviceContext.DrawIndexed(mesh.getIndexBuffer().getElementCount(), 0);

    Cb.world = glm::transpose(Transform);
    Cb.color = color;
    deviceContext.UpdateSubresource(buffers[0], &Cb);
  }
}

void
cModel::setModelPath(const std::string_view modelPath)
{
  this->m_modelPath = modelPath;
  setReady(true);
}

void
cModel::setMaterialPath(const std::string_view MaterialPath)
{
  m_materialPaths.push_back(std::string(MaterialPath));
}

std::size_t
cModel::getMeshCount() const
{
  return m_meshes.size();
}

std::size_t 
cModel::getVertexCount() const
{
  std::size_t Result(0);
  for (const cMesh & mesh : m_meshes)
  {
    Result += mesh.getVertexData()->size();
  }

  return  Result;
}

const cMesh *
cModel::getMesh(std::size_t index) const
{
  if (m_meshes.size() - 1 >= index)
  {
    return &m_meshes[index];
  }
  return nullptr;
}

bool
cModel::isReady() const
{
  return this->m_Ready;
}

void
cModel::Init(cDevice & device, [[maybe_unused]] cDeviceContext & deviceContext)
{
  this->LoadModelFromFile(device);
}

void
cModel::Draw(cDeviceContext & devContext, std::vector<cConstBuffer*> &buffers)
{
  DrawMeshes(devContext, buffers);
}

void cModel::update(cDeviceContext & deviceContext, const sMatrix4x4 &Transform )
{
  m_transform = Transform;
}

void
cModel::Destroy()
{
  m_meshes.clear();
  m_modelPath.clear();
  m_materialPaths.clear();
  setReady(false);
}


void
cModel::TraversTree(const aiScene * scene, aiNode * node, cDevice & device, std::vector<std::string> &texturePaths)
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

void
cModel::ExtractMesh(const aiMesh * assimpMesh, cDevice &device,
                    const aiScene*scene, std::vector<std::string> &texturePaths)
{
  std::unique_ptr< std::vector<uint16>> ptr_indices = std::make_unique<std::vector<uint16>>();
  std::unique_ptr<std::vector<sVertexPosTex>> ptr_vertices = std::make_unique<std::vector<sVertexPosTex>>();

  ptr_indices->reserve(assimpMesh->mNumFaces * 3);
  ptr_vertices->reserve(assimpMesh->mNumVertices);

  // get the indices of a the model 
  for (uint32_t i = 0; i < assimpMesh->mNumFaces; ++i)
  {
    uint16 tri[3];
    for (uint32_t j = 0; j < assimpMesh->mFaces[i].mNumIndices; ++j)
    {
      tri[j] = assimpMesh->mFaces[i].mIndices[j];
    }
    ptr_indices->emplace_back(tri[0]);
    ptr_indices->emplace_back(tri[1]);
    ptr_indices->emplace_back(tri[2]);
  }

  // get the vertices of the model
  for (uint32_t i = 0; i < assimpMesh->mNumVertices; ++i)
  {
    sVertexPosTex vertex;
    vertex.pos.x = assimpMesh->mVertices[i].x;
    vertex.pos.y = assimpMesh->mVertices[i].y;
    vertex.pos.z = assimpMesh->mVertices[i].z;
    vertex.pos.w = 1.0f;
    if (assimpMesh->HasTextureCoords(0))
    {
      vertex.tex.x = static_cast<float>(assimpMesh->mTextureCoords[0][i].x);
      vertex.tex.y = static_cast<float>(assimpMesh->mTextureCoords[0][i].y);
    }
    ptr_vertices->emplace_back(vertex);
  }

  this->CheckTexturePaths(scene, assimpMesh, texturePaths);

  cMesh result;
  result.initIndexBuffer(ptr_indices);
  result.initVertexBuffer(ptr_vertices);
  assert(ptr_indices == nullptr);
  assert(ptr_vertices == nullptr);
  bool IndexSuccessful = result.createIndexBuffer(device);
  bool VertexSuccessful = result.createVertexBuffer(device);

  assert((IndexSuccessful && VertexSuccessful, "error with index or vertex buffer creation"));
  m_meshes.emplace_back(std::move(result));
}

void
cModel::CheckTexturePaths(const aiScene * scene, const aiMesh *assimpMesh, std::vector<std::string> &texturePaths)
{
  aiMaterial *material = scene->mMaterials[assimpMesh->mMaterialIndex];
  aiString matName = material->GetName();
  std::filesystem::path Path = m_modelPath;
  Path = Path.parent_path();
  fs::directory_iterator dirIter(Path);
  static std::vector<std::string >  ExtensionFormast = {{".jpg"},{".png"}};

  //TODO : make this dynamic 
  //for (const fs::path p : dirIter)
  //{
  //  for (const std::string &Extensions : ExtensionFormast)
  //  {
  //    if (!Extensions.compare(p.extension().generic_string().c_str()))
  //    {
  //      if (comparePaths(m_materialPaths, p.generic_string().c_str()))
  //      {
  //        m_materialPaths.emplace_back(p.generic_string());
  //        std::cout << "added material from " << p << std::endl;
  //        break;
  //      }
  //    }
  //  }
  //}


  //Path += "/";
  //Path += matName.C_Str();
  //std::cout << Path.extension() << "\n";
  //if (comparePaths(m_materialPaths, Path.generic_string().c_str()))
  //{
  //  this->m_materialPaths.emplace_back(Path.generic_string().c_str());
  //  std::cout << "Successfully load material from " << Path.generic_string().c_str() << std::endl;
  //}

  //if (material->GetTextureCount(aiTextureType_DIFFUSE))
  //{
  //  aiString FilePath;
  //  aiReturn RetCheck = material->GetTexture(aiTextureType_DIFFUSE, assimpMesh->mMaterialIndex, &FilePath);
  //  //  = material->GetTexture(aiTextureType_DIFFUSE, assimpMesh->mMaterialIndex, &FilePath);
  //  if (RetCheck != aiReturn_SUCCESS)
  //  {
  //    std::cout << "did not load \'" << matName.C_Str() << "\' \n";
  //  }
  //  else
  //  {
  //    if (comparePaths(m_materialPaths, FilePath.C_Str()))
  //    {
  //      this->m_materialPaths.emplace_back(FilePath.C_Str());
  //      std::cout << "Successfully load material from " << FilePath.C_Str() << std::endl;
  //    }
  //  }
  //}
}

void cModel::ExtractTexture(const char * texturePath, cMesh & AfectedMesh, cDevice &device)
{
#if DIRECTX
  HRESULT hr = S_FALSE;

#if UNICODE
  std::int32_t PathLength = MultiByteToWideChar(CP_UTF8, 0, texturePath, -1, NULL, 0);
  wchar_t *Temp = new wchar_t[PathLength];
  MultiByteToWideChar(CP_UTF8, 0, texturePath, -1, Temp, PathLength);

  dx::CreateWICTextureFromFile(device.getDevice(),
                               Temp,
                               nullptr,
                               AfectedMesh.getResourceRef());

  delete Temp;
  Temp = nullptr;
#endif // UNICODE

  if (!SUCCEEDED(hr))
  {
    assert((SUCCEEDED(hr) && "error with creating textures "));
  }
#endif // DIRECTX

}

void
cModel::setTransform(glm::mat4 & matrix)
{
  m_transform.matrix = matrix;
}

glm::mat4
cModel::getTransform() const
{
  return this->m_transform.matrix;
}
