#pragma once

#include <boost/asio.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/thread.hpp>  
#include <boost/thread/mutex.hpp>  
#include <boost/bind.hpp>  
#include <iostream>  

namespace fsutility
{
	class CAsyncTimer;
	typedef boost::shared_ptr< CAsyncTimer> CAsyncTimerPtr;  

	class AFX_EXT_CLASS CAsyncTimer  
	{  
	public:
		CAsyncTimer(boost::shared_ptr<boost::asio::io_service> ios)   
			: m_sp_ios(ios) 
		{
		}  
		~CAsyncTimer() ;

	public:
		typedef   boost::function<void (int iTimerID, const boost::system::error_code& error)>   TimerHandlerEvent;

		virtual bool SetTimer(int iTimerID, int iElapse, TimerHandlerEvent Event);
		virtual bool ChangeTimerElapse(int iTimerID, int iElapse);
		virtual bool KillTimer(int iTimerID);
		virtual bool PauseTimer(int iTimerID);
		virtual bool ResumeTimer(int iTimerID);

	private:  
		void OnTimerInternal(int iTimerID, const boost::system::error_code& error);
		 
	protected:  
		typedef struct tagTIMER_INFO {  
			int iTimerID;  
			volatile int iElapse;  
			volatile bool bPause;
			volatile bool bCancel;
			TimerHandlerEvent CallProcess;  
			boost::shared_ptr< boost::asio::deadline_timer> timer;  
		} TIMER_INFO, *PTIMER_INFO;  
		typedef std::map<int, PTIMER_INFO> MapTimer;  

		MapTimer                  m_mapTimer;  
		boost::shared_ptr<boost::asio::io_service> m_sp_ios;  
	};  

}