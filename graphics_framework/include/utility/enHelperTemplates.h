#pragma once
#include <../include/actor/cActor.h>


namespace helper
{

  template<class ComponentClass>
  [[nodiscard]]ComponentClass*
    findComponent(cActor &actor)
  {
    for (auto iter = actor.getIteratorBegin(); iter != actor.getIteratorEnd(); ++iter)
    {
      baseComponent *ptr_component = *iter;
      ComponentClass * model = dynamic_cast< ComponentClass* >(ptr_component);
      if (model == nullptr) { continue; }
      else { return model; }
    }
    return nullptr;
  }

}
 
