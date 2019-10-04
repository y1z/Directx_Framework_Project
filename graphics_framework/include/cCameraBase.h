#include "../include/utility/Grafics_libs.h"
#include "../include/utility/CustomStructs.h"

class cWindow;

/*! this is the base class for all camera instances */
class cCameraBase
{
public:
  cCameraBase();

  virtual ~cCameraBase();
public:

  virtual void moveRight(float unit, cWindow &window,float deltaTime) = 0;

  virtual void moveUp(float unit,cWindow &window, float deltaTime) = 0;

  virtual void moveFront(float unit,cWindow &window, float deltaTime) = 0;
protected:
  virtual void updateCamera(cWindow &window) = 0;
public:
  /*! returns the eye of the camera (position)*/
  sVector4
    getEye() const;
  /*! returns the At of the camera (where the camera is looking AT ,position)*/
  sVector4
    getAt() const;
  /*! returns the direction */
  sVector4
    getUp() const;
  /*! set's a value for the 'm_eye' variable */
  void setEye(float x, float y, float z, float w = 1.0f);
  /*! set's a value for the 'm_at' variable */
  void setAt(float x, float y, float z, float w = 1.0f);
  /*! set's a value for the 'm_up' variable */
  void setUp(float x, float y, float z, float w = 1.0f);
protected:
  //! the position of the camera 
  sVector4    m_eye;
  //!  the position of what the camera is looking 'at' 
  sVector4    m_at;
  //! the up direction for the camera 
  sVector4    m_up;
};
