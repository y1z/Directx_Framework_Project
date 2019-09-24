#include "../include/utiliy/Grafics_libs.h"
#include "../include/utiliy/CustomStructs.h"

class cCameraBase
{
public:
  cCameraBase();

  virtual ~cCameraBase();
public:

  virtual void moveRight(float unit, float deltaTime) = 0;

  virtual void moveUp(float unit, float deltaTime) = 0;

  virtual void moveFront(float unit, float deltaTime) = 0;
protected:
  virtual void updateCamera() = 0;
public:
  /*! returns the eye of the camera (position)*/
  Vector4
    getEye() const;
  /*! returns the At of the camera (where the camera is looking AT ,position)*/
  Vector4
    getAt() const;
  /*! returns the direction */
  Vector4
    getUp() const;

  /*! set's a value for the 'm_eye' variable */
  void setEye(float x, float y, float z, float w = 1.0f);
  /*! set's a value for the 'm_at' variable */
  void setAt(float x, float y, float z, float w = 1.0f);
  /*! set's a value for the 'm_up' variable */
  void setUp(float x, float y, float z, float w = 1.0f);
private:

  Vector4    m_eye;
  Vector4    m_at;
  Vector4    m_up;
};
