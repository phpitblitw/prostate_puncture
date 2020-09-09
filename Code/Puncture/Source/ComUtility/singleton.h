#ifndef SINGLETON_H
#define SINGLETON_H

#include "synobj.h"

namespace fsutility
{
	template<class T>
	class  Singleton
	{
		CLASS_UNCOPYABLE(Singleton)
	public:
		static T& Instance() { // Unique point of access
			if (0 == _instance) {
				Lock lock(_mutex);
				if (0 == _instance) {
					_instance = new T();
					atexit(Destroy);
				}
			}
			return *_instance;
		}
	protected:
		Singleton(){}
		~Singleton(){}
	private:
		static void Destroy() { // Destroy the only instance
			if ( _instance != 0 ) {
				delete _instance;
				_instance = 0;
			}
		}
		static Mutex _mutex;
		static T * volatile _instance; // The one and only instance
	};

	template<class T>
	Mutex Singleton<T>::_mutex;

	template<class T>
	T * volatile Singleton<T>::_instance = 0;
}


#endif/*SINGLETON_H*/