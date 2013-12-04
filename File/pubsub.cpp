#include "pubsub.h"
void Publisher::notify(void* what, Subscriber* s)
{
   if (notifyEnabled)
   {
      list<Subscriber*>::iterator p;
      for(p = subscribers.begin(); p != subscribers.end(); p++)
         if (*p != s) (*p)->update(this, what);
   }
   notifyEnabled = true;
}
