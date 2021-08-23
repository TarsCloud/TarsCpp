#ifndef _HTTP_IMP_H_
#define _HTTP_IMP_H_

#include <map>
#include <string>
#include <vector>
#include "servant/Application.h"

using namespace std;
using namespace tars;

/////////////////////////////////////////////////////////////////////////
class HttpImp : public Servant {

public:
    /**
     * 构造函数
     */
    HttpImp();

public:
    /**
     * 对象初始化
     */
    virtual void initialize();

    /**
     * 处理客户端的主动请求
     * @param current 
     * @param response 
     * @return int 
     */
    virtual int doRequest(tars::CurrentPtr current, vector<char> &response);

    /**
     * 对象销毁
     */
    virtual void destroy();
};
///////////////////////////////////////////////////////////////////////////////
#endif
