#include "servant/AuthF.h"
#include "util/tc_epoll_server.h"

namespace tars
{

/**
 * server :默认鉴权逻辑
 */
bool processAuth(TC_EpollServer::Connection *c, const shared_ptr<TC_EpollServer::RecvContext>& data, const string &objName);

/**
 * server :默认鉴权逻辑
 */
int processAuthReqHelper(const BasicAuthPackage& pkg, const BasicAuthInfo& info);

/**
 * server :默认鉴权方法
 */
//int defaultProcessAuthReq(const char* request, size_t len, const string& expectObj);
//int defaultProcessAuthReq(const string& request, const string& expectObj);
int defaultProcessAuthReq(const char* request, size_t len, const TC_EpollServer::BindAdapterPtr &adapter, const string &objName);

/**
 * client:默认生成鉴权请求方法
 */
string defaultCreateAuthReq(const BasicAuthInfo& info /*, const string& hashMethod = "sha1" */ );

} // end namespace tars

