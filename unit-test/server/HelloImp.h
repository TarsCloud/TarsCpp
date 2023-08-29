#ifndef _HELLO_IMP_H_
#define _HELLO_IMP_H_

#include "Hello.h"
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace tars;
using namespace Test;

/////////////////////////////////////////////////////////////////////////
class HelloImp : public Hello {

public:
    /**
     * 构造函数
     */
    HelloImp();

    static set<int32_t> _current;

public:
    /**
     * 对象初始化
     */
    virtual void initialize();

    /**
     * 对象销毁
     */
    virtual void destroy();

    /**
     * 关闭连接
     * @param current
     * @return
     */
    virtual int doClose(CurrentPtr current);

public:
    int testTrans(int index, const string &s, string &r, CurrentPtr current);

	int testSyncTrans(int index, const string &s, string &r, CurrentPtr current);

    int testHello(int index, const string &s, string &r, CurrentPtr current);

    int testPid(string &r, CurrentPtr current);

	int testTimeout(int timeout, CurrentPtr current);

	virtual tars::Int32 testDyeing(const std::string & strIn,std::string &strOut, CurrentPtr current);

	virtual tars::Int32 testDyeingTrans(CurrentPtr current);

	virtual bool testCoro(const std::string& sIn, std::string &sOut, CurrentPtr current);

    virtual int testConHash(std::string &sOut, CurrentPtr current);

	virtual int testPushRegister(const string &msg, CurrentPtr current);

    virtual int testClose(CurrentPtr current);
protected:
    HelloPrx _helloPrx;

};
///////////////////////////////////////////////////////////////////////////////
#endif
