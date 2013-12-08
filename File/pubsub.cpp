#include "pubsub.h"
void Publisher::notify(Event& what)
{
   if (notifyEnabled)
   {
      list<Subscriber*>::iterator p;
      for(p = subscribers.begin(); p != subscribers.end(); p++)
          (*p)->update(what);
   }
   notifyEnabled = true;
}
