#ifndef __TEST_PUSH_THREAD_H
#define __TEST_PUSH_THREAD_H

#include "servant/Application.h"

class PushUser
{
public:
	static map<string, TarsCurrentPtr> pushUser;
	static TC_ThreadMutex mapMutex;
};

class PushInfoThread : public TC_Thread, public TC_ThreadLock
{
public:
	PushInfoThread():_bTerminate(false), _iId(0){}

	virtual void run();

	void terminate();

	void setPushInfo(const string &sInfo);

private:
	bool _bTerminate;
	unsigned int _iId;
	string _sPushInfo;
};
#endif
