#include "servant/AuthF.h"
#include "util/tc_epoll_server.h"

namespace tars
{
/**
 * server:默认生成鉴权请求方法
 */
pair<TC_NetWorkBuffer::PACKET_TYPE, shared_ptr<TC_NetWorkBuffer::Buffer>> serverVerifyAuthCallback(TC_NetWorkBuffer &, TC_Transceiver*, weak_ptr<TC_EpollServer::BindAdapter> adapter, const string &expectObj);

/**
 * client:默认生成鉴权请求方法
 */
vector<char> defaultCreateAuthReq(const BasicAuthInfo& info);

} // end namespace tars

