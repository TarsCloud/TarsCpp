#include "PushThread.h"
// #include <arpa/inet.h>

map<string, CurrentPtr> PushUser::pushUser;
TC_ThreadMutex PushUser::mapMutex;

PushInfoThread::~PushInfoThread()
{
	PushUser::pushUser.clear();
}

void PushInfoThread::terminate(void)
{
	tars::TC_ThreadLock::Lock sync(*this);
	_bTerminate = true;

	notifyAll();
}

void PushInfoThread::setPushInfo(const string &sInfo)
{
    unsigned int iBuffLength = htonl(sInfo.size()+8);
    unsigned char * pBuff = (unsigned char*)(&iBuffLength);

    _sPushInfo = "";
    for (int i = 0; i<4; ++i)
    {
        _sPushInfo += *pBuff++;
    }

    unsigned int iRequestId = htonl(_iId);
    unsigned char * pRequestId = (unsigned char*)(&iRequestId);

    for (int i = 0; i<4; ++i)
    {
        _sPushInfo += *pRequestId++;
    }

    _sPushInfo += sInfo;
}
//定期向客户push消息
void PushInfoThread::run(void)
{
	setPushInfo("hello world");

	int count = 0;
	while (!_bTerminate)
	{
		++count;

		if(count % 10 == 0)
		{
			(PushUser::mapMutex).lock();

			for (map<string, CurrentPtr>::iterator it = (PushUser::pushUser).begin();
			     it != (PushUser::pushUser).end(); ++it) {
				(it->second)->close();
				LOG->debug() << "close" << endl;
			}
			(PushUser::mapMutex).unlock();
		}
		else
		{
			(PushUser::mapMutex).lock();

			for (map<string, CurrentPtr>::iterator it = (PushUser::pushUser).begin();
			     it != (PushUser::pushUser).end(); ++it) {
				(it->second)->sendResponse(_sPushInfo.c_str(), _sPushInfo.size());
				LOG->debug() << "sendResponse: " << _sPushInfo.size() << endl;
			}
			(PushUser::mapMutex).unlock();
		}

		{
            TC_ThreadLock::Lock sync(*this);
            timedWait(1000);
		}
	}
}
