#include "include/cViewport.h"
//std includes
#include <memory>
cViewport::cViewport()
{
#if DIRECTX
  std::memset(&m_directxViewport, 0, sizeof(m_directxViewport));

#endif // DIRECTX
}

cViewport::~cViewport()
{}

