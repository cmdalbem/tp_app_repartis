#ifndef PUBSUB_H
#define PUBSUB_H
#include <list>  // for our subscriber list
#include <Event.h>
using namespace std;

class Subscriber
{
	public:
		virtual ~Subscriber() {}
		virtual void update(Event& what) = 0;

};

#endif
