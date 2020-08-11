#ifndef SYNOBJ_H
#define SYNOBJ_H

#include <windows.h>

namespace fsutility
{
	#define CLASS_UNCOPYABLE(classname) \
		 private: \
		 classname(const classname&); \
		 classname& operator=(const classname&);

	class AFX_EXT_CLASS Mutex {
		CLASS_UNCOPYABLE(Mutex)
	public:
		Mutex() :_cs() { InitializeCriticalSection(&_cs); }
		~Mutex() { DeleteCriticalSection(&_cs); }
		void lock() { EnterCriticalSection(&_cs); }
		void unlock() { LeaveCriticalSection(&_cs); }
	private:
		CRITICAL_SECTION _cs;
	};

	class AFX_EXT_CLASS Lock {
		CLASS_UNCOPYABLE(Lock)
	public:
		explicit Lock(Mutex& cs) :_cs(cs) { _cs.lock(); }
		~Lock() { _cs.unlock(); }
	private:
		Mutex& _cs;
	};

}
#endif/*SYNOBJ_H*/