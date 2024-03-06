#pragma once

#include <iostream>
#include "servant/Application.h"

using namespace tars;

extern vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _clientStatData;
extern vector<map<tars::StatMicMsgHead, tars::StatMicMsgBody>> _serverStatData;

class FrameworkServer : public Application, public TC_Thread
{
public:
	/**
	 * 析构函数
	 **/
	virtual ~FrameworkServer();

	/**
	 * 服务初始化
	 **/
	virtual void initialize();

	/**
	 * 服务销毁
	 **/
	virtual void destroyApp();

protected:
	virtual void run();
};


