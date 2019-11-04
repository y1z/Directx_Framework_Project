#pragma once 
#include "utility/enGraphics.h"
#include <string>

/*!\brief this is the base class for all shader classes */
class cShaderBase
{
public:
  cShaderBase();
  virtual ~cShaderBase();
public:// functions 
#if DIRECTX
   
/*! used in directX to receive information about 
shader/error etc..*/
  ID3DBlob *
    getInfo();

  ID3DBlob **
    getInfoRef();
#elif OPEN_GL

  uint32_t
    getID() const;

  void
    setID(uint32_t  newID);
#endif // DIRECTX

  /*! returns the shader currently being stored*/
 const std::string 
    *getShader();
 /*! used to store the new shader */
 void
   setShader(std::string_view newShader);

private:
#if DIRECTX
  /*! this contains information ,errors , type of shader etc..*/
 ID3DBlob *mptr_infoBlob;
#elif OPEN_GL
  uint32_t m_shaderID;
#endif // DIRECTX
protected:
  //! contains the shader 
  std::string m_shader;

};
