#pragma once 
#include "utility/enGraphics.h"
#include "utility/enDefs.h"

#include <string>
#include <vector>

/*!\brief this is the base class for all shader classes */
class cShaderBase
{
public:
  cShaderBase();
  cShaderBase(cShaderBase &&other);
  cShaderBase(const cShaderBase &other) = delete;

  virtual ~cShaderBase();
public:// functions 
#if DIRECTX
/*! used in directX to receive information about
shader/error etc..*/
  ID3DBlob *
    getInfo();

  ID3DBlob **
    getInfoRef();

  void
    setDxPointer(void* ptr);
#elif OPEN_GL

  uint32_t
    getID() const;

  void
    setID(uint32_t  newID);
#endif // DIRECTX

  /*! @return the first version of a shader */
  const std::string *
    getOriginalShaderPtr();

  const std::string &
    getOriginalShader();

    /*! @returns a container of all the shaders created*/
  const std::vector<std::string > &
    getAlteredShaders();

    /*! used to store the new shader */
  void
    setShader(std::string_view newShader);

   /*!compiles a shader from a file.*/
  bool
    compileShader(std::string_view shaderPath,
                  std::string_view EntryPoint,
                  std::string_view vertexShaderVersion);

  bool
    compileShaderFromMemory(std::string_view shader,
                            std::string_view EntryPoint,
                            std::string_view vertexShaderVersion);



               /*!add define statements to the shader */
  void
    addDefines(std::vector<std::string> &defines);

  void
    printOriginalShader()const;

  void
    printAlteredShader()const;

private://functions 

  //! get the index of the first new line in the shader 
  size_t
    getIndexAfterFirstNewLine() const;

  void
    resizeVector(size_t newSize);

private:
#if DIRECTX
  /*! this contains information ,errors , type of shader etc..*/
  ID3DBlob *mptr_infoBlob{ nullptr };

#elif OPEN_GL
  uint32_t m_shaderID;
#endif // DIRECTX

protected:


  //! contains the shader 
  std::string m_originalShader;

  //! keeps track of the entry point of the shader  
  std::string m_entryPoint;

  //! keeps track of which version of the shader I'm using 
  std::string m_shaderModel;

  //! holds alternate versions of a shader 
  std::vector<std::string> m_alteredShaders;

  enShaderTypes m_shaderType = enShaderTypes::NONE;
};


