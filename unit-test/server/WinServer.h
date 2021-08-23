#ifndef _WinServer_H_
#define _WinServer_H_

#include <iostream>
#include "servant/Application.h"

using namespace tars;

class WinServer : public Application, public TC_Thread
{
public:
	/**
	 * 析构函数
	 **/
	virtual ~WinServer();

	/**
	 * 服务初始化
	 **/
	virtual void initialize();

	/**
	 * 服务销毁
	 **/
	virtual void destroyApp();

	bool cmdAdd(const string& command, const string& params, string& result);

	bool delTarsViewVersion(const string& command, const string& params, string& result);

protected:
	virtual void run();

};

////////////////////////////////////////////
#endif
