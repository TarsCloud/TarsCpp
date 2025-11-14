#ifndef _SIP_SUBSCRIBE_H_
#define _SIP_SUBSCRIBE_H_

#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "SipGateway/SipMsgDecodedPart.h"
#include "SipGateway/SipMsgCommon.h"

namespace SipProxy
{
	class SipSubscribe;
	typedef std::shared_ptr<SipSubscribe> SipSubscribePtr;

	class SipSubscribe
	{

		//construction
	public:

		SipSubscribe(const std::string & subscribeId, const std::string & sipProxyId);

		//date members
	private:
		std::string				m_strSubscribeId;
		std::string				m_strSipProxyId;

		int64_t					m_lLastSubscribeTime;			//the last time that receive the 200 REGISTER OK from the SBC
		int						m_iExpire;



	public:
		SipMsgDecodedPart			m_sipMsgDecodePart;				//decoded sip msg information


		//function member
	public:
		void setExpire(int expire);
		int getExpire();

		void setLastSubscribeTime(int64_t lastTime);


	};


	//SipSubscribe  类是否有存在的必要？ ==============================================



	class SipSubscribeManager
	{

	private:
		std::map<std::string, SipSubscribePtr>	m_mapSipSubscribes;
		std::mutex								m_mutexSipSubscribes;



	public:
		bool InsertSipSubscribe(const std::string & subscribeId, const SipSubscribePtr & sipSubscribePtr);
		bool removeSipSubscribe(const std::string & subscribeId);

		SipSubscribePtr getSipSubscribe(const std::string & subscribeId);
		SipSubscribePtr createSipSubscribe(const std::string & subscribeId, const std::string & sipProxyId);



	};






};



#endif











