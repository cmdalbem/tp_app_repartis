#ifndef PUBSUB_H
#define PUBSUB_H
#include <list>  // for our subscriber list
#include <Event.h>
using namespace std;
class Publisher; // forward reference

class Subscriber
{
	public:
		virtual ~Subscriber() {}
		virtual void update(Publisher* who, Event& what) = 0;

};

class Publisher
{
	public:
		Publisher() { notifyEnabled = true; }
		virtual ~Publisher() {}
		void subscribe(Subscriber* s) { subscribers.push_back(s); }
		void unsubscribe(Subscriber* s) { subscribers.remove(s); }
		void notify(Event& what);
		void setNotifyEnabled(bool flag) { notifyEnabled = flag; }
		bool getNotifyEnabled() const { return notifyEnabled; }
	private:
		list<Subscriber*> subscribers;
		bool notifyEnabled;
};

#endif
