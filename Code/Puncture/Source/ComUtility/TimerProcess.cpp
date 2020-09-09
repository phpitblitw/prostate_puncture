#include "StdAfx.h"
#include "TimerProcess.h"

using namespace fsutility;

CAsyncTimer::~CAsyncTimer() 
{  
	MapTimer::iterator it = m_mapTimer.begin();  
	for( ; it != m_mapTimer.end(); it ++) 
	{  
		PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
		delete pInfo;  
		pInfo = NULL;  
	}  
	m_mapTimer.clear();  
}  

bool CAsyncTimer::SetTimer(int iTimerID, int iElapse, TimerHandlerEvent Event)
{
	MapTimer::iterator it = m_mapTimer.find(iTimerID);  
	if( it != m_mapTimer.end() ) {  
		return false;  
	}  

	PTIMER_INFO pInfo  = new TIMER_INFO;  
	pInfo->iTimerID    = iTimerID;  
	pInfo->iElapse     = iElapse;  
	pInfo->bPause      = false;
	pInfo->bCancel     = false;
	pInfo->CallProcess = Event;  
	pInfo->timer = boost::shared_ptr< boost::asio::deadline_timer >(new boost::asio::deadline_timer(*(m_sp_ios.get())));  

	pInfo->timer->expires_from_now(boost::posix_time::millisec(iElapse));  
	pInfo->timer->async_wait(boost::bind(&CAsyncTimer::OnTimerInternal, this, iTimerID, boost::asio::placeholders::error));  

	m_mapTimer.insert(std::pair<int, PTIMER_INFO>(iTimerID, pInfo));  

	return true;  
}

bool CAsyncTimer::KillTimer(int iTimerID) 
{  
	MapTimer::iterator it = m_mapTimer.find(iTimerID);  
	if( it == m_mapTimer.end() ) {  
		return false;  
	}  

	PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
	pInfo->bCancel = true;

	return true;  
}  


bool CAsyncTimer::ChangeTimerElapse(int iTimerID, int iElapse)
{
	MapTimer::iterator it = m_mapTimer.find(iTimerID);  
	if( it == m_mapTimer.end() ) {  
		return false;  
	}  

	PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
	pInfo->iElapse = iElapse;

	return true;  
}


bool CAsyncTimer::PauseTimer(int iTimerID)
{
	MapTimer::iterator it = m_mapTimer.find(iTimerID);  
	if( it == m_mapTimer.end() ) {  
		return false;  
	}  

	PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
	pInfo->bPause = true; 
	return true;  
}

bool CAsyncTimer::ResumeTimer(int iTimerID)
{
	MapTimer::iterator it = m_mapTimer.find(iTimerID);  
	if( it == m_mapTimer.end() ) {  
		return false;  
	}  

	PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
	pInfo->bPause = false; 
	return true;  
}

void CAsyncTimer::OnTimerInternal(int iTimerID, const boost::system::error_code& error) 
{  
	if(!error)
	{
		MapTimer::iterator it = m_mapTimer.find(iTimerID);  
		if( it == m_mapTimer.end() ) {  
			return;  
		}  

		PTIMER_INFO pInfo = (PTIMER_INFO)(it->second);  
		if(pInfo->bCancel)
		{
			pInfo->timer->cancel();
			delete pInfo;
			m_mapTimer.erase(it);
		}
		else
		{
			if(!pInfo->bPause && pInfo->CallProcess != NULL)
			{
				pInfo->CallProcess(iTimerID, error);
			}

			//继续处理下一个
			pInfo->timer->expires_from_now(boost::posix_time::millisec(pInfo->iElapse));  
			pInfo->timer->async_wait(boost::bind(&CAsyncTimer::OnTimerInternal, this, iTimerID, boost::asio::placeholders::error));  
		}
	}
}  

