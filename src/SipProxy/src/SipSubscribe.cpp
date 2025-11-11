#include "SipSubscribe.h"
#include "servant/Application.h"

namespace SipProxy
{

	SipSubscribe::SipSubscribe(const std::string & subscribeId, const std::string & sipProxyId)
	{
		m_strSubscribeId = subscribeId;
		m_strSipProxyId = sipProxyId;
	}

	void SipSubscribe::setExpire(int expire)
	{
		m_iExpire = expire;
	}
	int SipSubscribe::getExpire()
	{
		return m_iExpire;
	}

	void SipSubscribe::setLastSubscribeTime(int64_t lastTime)
	{
		m_lLastSubscribeTime = lastTime;
	}



	//map<std::string, SipSubscribePtr>			m_mapSipSubscribes;
	//Common::RecMutex								m_mutexSipSubscribes;

	//=======================================================================

	bool SipSubscribeManager::InsertSipSubscribe(const std::string & subscribeId, const SipSubscribePtr & sipSubscribePtr)
	{
		std::lock_guard<std::mutex> lock(m_mutexSipSubscribes);

		std::map<std::string, SipSubscribePtr>::iterator iter;
		iter = m_mapSipSubscribes.find(subscribeId);
		if (iter != m_mapSipSubscribes.end())
		{
			TLOGWARN("[SipProxy] InsertSipSubscribe() called : failed to insert a SipSubscribePtr for that the call id exists in the map, subscribe id:" << subscribeId << endl);
			return false;
		}

		m_mapSipSubscribes.insert(std::make_pair(subscribeId, sipSubscribePtr));

		TLOGDEBUG("[SipProxy] InsertSipSubscribe() called, insert a SipSubscribePtr, subscribeId: " << subscribeId << ", now the total is :" << m_mapSipSubscribes.size() << endl);
		return true;



		return true;
	}

	bool SipSubscribeManager::removeSipSubscribe(const std::string & subscribeId)
	{


		std::lock_guard<std::mutex> lock(m_mutexSipSubscribes);

		std::map<std::string, SipSubscribePtr>::iterator iter;
		iter = m_mapSipSubscribes.find(subscribeId);
		if (iter == m_mapSipSubscribes.end())
		{
			TLOGWARN("[SipProxy] removeSipSubscribe() called : failed to Remove the SipSubscribePtr that not exist in the map,  subscribe  id:" << subscribeId << endl);
			return false;
		}

		m_mapSipSubscribes.erase(iter);

		TLOGDEBUG("[SipProxy] removeSipSubscribe() called, insert a SipSubscribePtr, subscribeId : " << subscribeId << ", now the total is :" << m_mapSipSubscribes.size() << endl);

		return true;

	}





	SipSubscribePtr SipSubscribeManager::getSipSubscribe(const std::string & subscribeId)
	{


		return nullptr;
	}
	//应当在REGISTER的 200 OK 返回之后将注册对象从集合中删除

	SipSubscribePtr SipSubscribeManager::createSipSubscribe(const std::string & subscribeId, const std::string & sipProxyId)
	{

		return nullptr;
	}
















































};