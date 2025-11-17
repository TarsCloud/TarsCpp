#ifndef __Common_Net_h__
#define __Common_Net_h__

#include <string>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <cstdint>

namespace Common
{
    class NetSender;
    class NetReceiver;
    class StreamReceiver;
    class NetStream;
    class SslReceiver;
    class NetSsl;
    class NetDriver;
   // class BindChannel_Result;

    typedef std::shared_ptr<NetSender>               NetSenderPtr;
    typedef std::shared_ptr<NetReceiver>             NetReceiverPtr;
    typedef std::shared_ptr<StreamReceiver>          StreamReceiverPtr;
    typedef std::shared_ptr<NetStream>               NetStreamPtr;
    typedef std::shared_ptr<SslReceiver>             SslReceiverPtr;
    typedef std::shared_ptr<NetSsl>                  NetSslPtr;
    typedef std::shared_ptr<NetDriver>               NetDriverPtr;
    //typedef std::shared_ptr<BindChannel_Result>      BindChannel_ResultPtr;

    /** 网络流量类型 */
    enum NetFlowType {
        /** IPv4 流量 */
        NetFlowIpv4 = 0,
        /** IPv6 流量 */
        NetFlowIpv6,
        /** @cond */
        NetFlowAddr,
        /** @endcond */
        /** UDP 流量 */
        NetFlowUdp = NetFlowAddr,
        /** TCP 流量 */
        NetFlowTcp,
        /** HTTP 流量 */
        NetFlowHttp,
        /** TLS 流量 */
        NetFlowTls,
        /** @cond */
        NetFlowSize
        /** @endcond */
    };

    /** 加密连接类型 */
    enum SslType {
        /** 标准 SSL/TLS */
        SslStandard = 0,
        /** 国密标准 */
        SslGm       = 1,
        /** @cond */
        SslMax      = SslGm,
        /** @endcond */
    };

    // /** 网络数据包 */
    // class NetPacket
    // {
    // public:
    //     /** 构造函数
    //      *
    //      * @param[in]   data        数据内容
    //      */
    //     explicit NetPacket(const Stream& data);
    //     /** 构造函数
    //      *
    //      * @param[in]   data        数据内容
    //      * @param[in]   dataLen     数据内容长度，单位字节
    //      */
    //     NetPacket(const unsigned char *data,int dataLen);
    // public:
    //     /** @cond */
    //     Stream _data;
    //     def_link_node(class NetPacket) _link;
    //     unsigned int _ticks;
    //     /** @endcond */
    // };

    // /** 网络数据包数组 */
    // class NetPacketArray
    // {
    // public:
    //     /** 构造函数 */
    //     NetPacketArray();
    //     /** 析构函数 */
    //     ~NetPacketArray();

    //     /** 数据包数量
    //      *
    //      * @return      数据包数量
    //      */
    //     int totalSize() { return _totalSize; }
    //     /** 顺序添加数据包
    //      *
    //      * @param[in]   pkt     数据包对象
    //      */
    //     void addPacket(NetPacket *pkt);
    //     /** 从头部获取数据包
    //      *
    //      * @param[in]   remove  是否从数组中删除
    //      *
    //      * @return      数据包对象
    //      */
    //     NetPacket *getPacket(bool remove);
    //     /** 从数组中移除指定数量的数据
    //      *
    //      * @param[in]   size    移除数据大小，单位字节
    //      */
    //     void commitSize(int size);

    //     /** 追加数据包数组
    //      *
    //      * @param[in]   from    追加的数组对象
    //      */
    //     void insertPackets(NetPacketArray& from);
    //     /** 获取数据最长保存时间
    //      *
    //      * @return      保存时间，单位毫秒
    //      */
    //     int  getDelay();
    // private:
    //     def_link_entity(class NetPacket) _entityPkts;
    //     int _totalSize;
    // };

    // class BindChannel_Result
    // {
    // public:
    //     virtual ~BindChannel_Result() = default;
    //     virtual void onBindChannel_Result(bool result,const std::string& reason,unsigned int channel) = 0;
    //     virtual void onChannelClosed(unsigned int channel) {}
    // };

    /** 网络发送对象接口类 */
    class NetSender
    {
    public:
        virtual ~NetSender() = default;
    public:
        /** 绑定ARC传输通道，减小ARC头开销; 非ARC传输的通道，没有效果
         *
         * @param[in]   did         域ID
         * @param[in]   level       数据级别
         * @param[in]   result      结果通过result返回
         */
        //virtual void bindChannel(int did,int level,const BindChannel_ResultPtr& result) { if (result) result->onBindChannel_Result(false,"not-implemented",0); }

        // /** 通过channel发送数据
        //  *
        //  * @param[in]   channel     通过bindChannel获取到的channel
        //  * @param[in]   did         域ID, 必须使用绑定channel时的did
        //  * @param[in]   level       数据级别，必须使用绑定channel时的level
        //  * @param[in]   data        数据内容
        //  *
        //  * @return      已发送内容大小，单位字节
        //  */
        // virtual int send(unsigned int channel,int did,int level,const Stream& data) { return -1; }

        // /** 发送数据
        //  *
        //  * @param[in]   data        数据内容
        //  *
        //  * @return      已发送内容大小，单位字节
        //  */
        // virtual int  send(const Stream& data) = 0;
        /** 发送数据
         *
         * @param[in]   data        数据内容
         * @param[in]   dataLen     数据内容长度，单位字节
         *
         * @return      已发送内容大小，单位字节
         */
        virtual int  send(const unsigned char *data,int dataLen) = 0;
        // /** 发送数据
        //  *
        //  * @param[in]   did         域ID
        //  * @param[in]   level       数据级别
        //  * @param[in]   data        数据内容
        //  *
        //  * @return      已发送内容大小，单位字节
        //  */
        // virtual int  send(int did,int level,const Stream& data) = 0;
        /** 发送数据
         *
         * @param[in]   did         域ID
         * @param[in]   level       数据级别
         * @param[in]   data        数据内容
         * @param[in]   dataLen     数据内容长度，单位字节
         *
         * @return      已发送内容大小，单位字节
         */
        virtual int  send(int did,int level,const unsigned char *data,int dataLen) = 0;
        /** 关闭对象 */
        virtual void close() = 0;

        /** 是否可以发送数据
         *
         * @retval      true        可以发送数据
         * @retval      false       不能发送数据
         */
        virtual bool isSendReady() = 0;
        /** 是否已经关闭
         *
         * @retval      true        关闭
         * @retval      false       未关闭
         */
        virtual bool isClosed() = 0;
        /** 设置缓冲期大小
         *
         * @param[in]   sendBytes   发送缓冲区大小，单位字节
         * @param[in]   recvBytes   接收缓冲区大小，单位字节
         */
        virtual void setBufSize(int sendBytes,int recvBytes) = 0;
        /** 参数设置
         *
         * @param[in]   params      参数配置内容
         */
        virtual void setParams(const std::map<std::string,std::string>& params) = 0;
        /** 获取参数
         *
         * @param[in]   name        参数名称
         *
         * @return      参数内容
         */
        virtual std::string getParam(const std::string& name) = 0;

        /** 协议类型
         *
         * @return      协议类型，例如 "udp" "tcp" "tls" "arc" "varc" 等
         */
        virtual std::string protocol() = 0;
        /** 获取 socket 对象
         *
         * @return      Socket 对象句柄
         */
        virtual int  getSocket() = 0;
        /** 获取本地地址
         *
         * @param[out]  host        地址信息
         * @param[out]  port        端口信息
         *
         * @retval      true        获取成功
         * @retval      true        获取失败
         */
        virtual bool getLocal(std::string& host,int &port) = 0;
        /** 获取远端地址
         *
         * @param[out]  host        地址信息
         * @param[out]  port        端口信息
         *
         * @retval      true        获取成功
         * @retval      true        获取失败
         */
        virtual bool getRemote(std::string& host,int &port) = 0;
    };

    /** 网络接收对象接口类 */
    class NetReceiver
    {
    public:
        virtual ~NetReceiver() = default;
    public:
        /** 收到数据回调接口
         *
         * @param[in]   data        数据内容
         * @param[in]   dataLen     数据内容长度，单位字节
         */
        virtual void recv(const unsigned char *data,int dataLen) {}
        /** 收到数据回调接口
         *
         * @param[in]   did         域ID
         * @param[in]   level       数据级别
         * @param[in]   data        数据内容
         * @param[in]   dataLen     数据内容长度，单位字节
         */
        virtual void recv(int did,int level,const unsigned char *data,int dataLen) { recv(data,dataLen); }
        /** 建立新连接回调
         *
         * 当收到来自新的远端地址数据的时候，会回调该接口，以创建针对新连接的数据接收处理对象。
         *
         * @param[in]   sender      新连接的发送对象
         *
         * @return      新连接的接收处理对象。如果返回 0 则不会新建连接
         */
        virtual NetReceiverPtr recvConnection(const NetSenderPtr& sender) { return 0; }

        /** 已连接状态回调 */
        virtual void onConnReady() {}
        /** 处于可发送状态回调 */
        virtual void onSendReady() {}
        /** 连接关闭状态回调 */
        virtual void onConnClose() {}
        /** TLS 验证回调
         *
         * 如果当前接收对象网络协议是 TLS，则在收到 TLS 连接的时候会回调该接口
         *
         * @return      验证结果
         * - 0      验证失败，TLS 连接将关闭
         * - 1      验证成功
         */
        virtual int  onTlsVerify(int err) { return 0; }

        /** 获取连接名称
         *
         * @return      连接名称
         */
        virtual std::string getName() { return std::string(); }
    };

    /** 可靠传输连接 接收数据处理接口 */
    // class StreamReceiver
    // {
    // public:
    //     virtual ~StreamReceiver() = default;
    // public:
    //     // /** 可靠传输连接 需要输入数据
    //     //  *
    //     //  * @param[in]   offset      数据偏移量
    //     //  * @param[out]  data        数据内容
    //     //  */
    //     // virtual void onIputData(int64_t offset,Stream& data) = 0;
    //     // /** 可靠传输连接 输出数据
    //     //  *
    //     //  * @param[in]   offset      数据偏移量
    //     //  * @param[in]   data        数据内容
    //     //  */
    //     // virtual void onOputData(int64_t offset,Stream& data) = 0;
    //     // /** 可靠传输连接 发送数据
    //     //  *
    //     //  * @param[in]   pkt         需要发送的数据
    //     //  */
    //     // virtual void onSendPkt(Stream& pkt) = 0;

    //     /** 可靠传输连接 连接重置
    //      *
    //      * @param[in]   wantKbps    期望的带宽，单位 Kbps
    //      */
    //     virtual void onStreamReset(int wantKbps) {}
    //     /** 可靠传输连接 期望带宽变化回调
    //      *
    //      * @param[in]   wantKbps    期望的带宽，单位 Kbps
    //      */
    //     virtual void onSendWantKbps(int wantKbps) {}
    // };

    // /** 可靠传输对象 */
    // class NetStream
    // {
    // public:
    //     virtual ~NetStream() = default;
    // public:
    //     /** 关闭连接 */
    //     virtual void close() = 0;
    //     /** 判断连接是否断开
    //      *
    //      * @param[in]   ackTimeout      应答响应超时判断标准，单位毫秒
    //      * @param[in]   dataTimeout     数据传输超时判断标准，单位毫秒
    //      *
    //      * @retval      true            连接断开
    //      * @retval      false           连接未断开
    //      */
    //     virtual bool isDisconnect(int ackTimeout = 4000,int dataTimeout = 12000) = 0;

    //     /** 设置发送带宽控制参数
    //      *
    //      * @param[in]   initKbps        初始码率，单位 Kbps
    //      * @param[in]   baseKbps        最低码率，单位 Kbps
    //      * @param[in]   maxKbps         最大码率，单位 Kbps
    //      *
    //      * @retval      true            设置成功
    //      * @retval      false           设置失败
    //      */
    //     virtual bool setKbps(int initKbps,int baseKbps,int maxKbps) = 0;
    //     /** 设置分片数据传输超时时间
    //      *
    //      * @param[in]   timeout         超时时长，单位毫秒
    //      *
    //      * @retval      true            设置成功
    //      * @retval      false           设置失败
    //      */
    //     virtual bool setFragTimeout(int timeout) = 0;
    //     /** 设置分片数据 MTU 大小
    //      *
    //      * @param[in]   mtu             MTU 大小，单位字节
    //      *
    //      * @retval      true            设置成功
    //      * @retval      false           设置失败
    //      */
    //     virtual bool setFragMtu(int mtu) = 0;
    //     /** 重置发送状态
    //      *
    //      * @retval      true            设置成功
    //      * @retval      false           设置失败
    //      */
    //     virtual bool setSendReset() = 0;
    //     // /** 发送数据
    //     //  *
    //     //  * @param[in]   data        数据内容
    //     //  *
    //     //  * @retval      true            发送成功
    //     //  * @retval      false           发送失败
    //     //  */
    //     // virtual bool sendData(const Stream& data) = 0;
    //     /** 发送数据
    //      *
    //      * @param[in]   data        数据内容
    //      * @param[in]   dataLen     数据内容大小，单位字节
    //      *
    //      * @retval      true            发送成功
    //      * @retval      false           发送失败
    //      */
    //     virtual bool sendData(const unsigned char *data,int dataLen) = 0;
    //     // /** 将接收数据放入可靠传输对象处理
    //     //  *
    //     //  * @param[in]   pkt         数据内容
    //     //  *
    //     //  * @retval      true            调用成功
    //     //  * @retval      false           调用失败
    //     //  */
    //     // virtual bool recvPkt(const Stream& pkt) = 0;
    //     /** 将接收数据放入可靠传输对象处理
    //      *
    //      * @param[in]   pkt         数据内容
    //      * @param[in]   pktLen     数据内容大小，单位字节
    //      *
    //      * @retval      true            调用成功
    //      * @retval      false           调用失败
    //      */
    //     virtual bool recvPkt(const unsigned char *pkt,int pktLen) = 0;

    //     /* remote acked offset */
    //     /** 获取对端已经确认收到的数据偏移
    //      *
    //      * @return      数据偏移量
    //      */
    //     virtual int64_t getSendOffset() = 0;
    //     /* receive data offset */
    //     /** 获取本地已经收到数据的偏移量
    //      *
    //      * @return      数据偏移量
    //      */
    //     virtual int64_t getRecvOffset() = 0;
    //     /* all send data remote acked */
    //     /** 判断是否数据已经发送完成
    //      *
    //      * @retval      true        已经发送完成
    //      * @retval      true        尚未发送完成
    //      */
    //     virtual bool sendCompleted() = 0;

    //     /** 设置参数
    //      *
    //      * @param[in]   name        参数名称
    //      * @param[in]   value        参数值
    //      */
    //     virtual void setParam(const std::string& name,const std::string& value) = 0;
    //     /** 获取参数
    //      *
    //      * @param[in]   name        参数名称
    //      *
    //      * @return      参数值
    //      */
    //     virtual std::string getParam(const std::string& name) = 0;
    //     /** @cond */
    //     virtual bool setSecurityParam(unsigned int sentKey, unsigned int recvKey, int passThroughType) = 0;
    //     /** @endcond */
    // };

    /** 安全连接数据接收接口 */
    class SslReceiver
    {
    public:
        virtual ~SslReceiver() = default;
    public:
        /** 安全连接 输出数据
         *
         * @param[in]   data        数据内容
         * @param[in]   dataLen        数据内容大小，单位字节
         */
        virtual void onOputData(const unsigned char* data,int dataLen) = 0;
        /** 安全连接 发送数据
         *
         * @param[in]   data        数据内容
         * @param[in]   dataLen        数据内容大小，单位字节
         */
        virtual int  onSendPkt(const unsigned char* data,int dataLen) = 0;
        /** 安全连接 已经就绪状态回调 */
        virtual bool onSslReady() { return true; }
        /** 安全连接 验证新建连接回调 */
        virtual int  onSslVerify(int err) { return 0; }
    };

    /** 安全连接对象 */
    class NetSsl
    {
    public:
        virtual ~NetSsl() = default;
    public:
        /** 关闭连接 */
        virtual void close() = 0;
        /** 发送数据
         *
         * @param[in]   data        数据内容
         * @param[in]   dataLen     数据内容大小，单位字节
         *
         * @return      已发送数据大小
         */
        virtual int sendData(const unsigned char* data,int dataLen) = 0;
        /** 将接收数据放入安全连接对象处理
         *
         * @param[in]   data         数据内容
         * @param[in]   dataLen     数据内容大小，单位字节
         *
         * @return      已处理数据大小
         */
        virtual int recvPkt(const unsigned char* data,int dataLen) = 0;
    };

    struct ResolveResult
    {
        ResolveResult() : defaultIpv6(false) {}

        bool defaultIpv6;
        std::vector<std::string> hosts4;
        std::vector<std::string> hosts6;
    };

    /** 网络接口对象 */
    class NetDriver
    {
    public:
        virtual ~NetDriver() = default;
    public:
        /** 创建网络接口对象
         *
         * @param[in]   maxFds      最大Socket数量
         * @param[in]   polls       监听线程数量
         *
         * @return      网络接口对象
         */
        static NetDriverPtr create(int maxFds, int polls = 1);
        /** 设置挂起状态
         *
         * @param[in]   suspend     是否停止处理网络数据收发
         */
        static void setNetSuspend(bool suspend);

        /** 设置后台状态
         *
         * @param[in]   background  是否处于后台; 网络活动在后台会减少
         */
        virtual void setBackground(bool background) = 0;

        /** 关闭接口 */
        virtual void shutdown() = 0;
        /** WatchDog 检查 */
        virtual void *watchDogCheck(int abortTimeout) = 0;

        /** 设置是否默认使用 IPv6 地址
         *
         * @param[in]   enable      是否默认使用 IPv6 地址
         */
        virtual void setDefaultIpv6(bool enable) = 0;
        /** 获取是否默认使用 IPv6 地址
         *
         * @return      是否默认使用 IPv6 地址
         */
        virtual bool getDefaultIpv6() = 0;
        /** 设置网络状态变化
         *
         * 当应用代码监听到网络状态变化，例如，接入网络类型变化，或者其它可能导致变化的事件时，
         * 应调用该接口。
         *
         * @param[in]   reload      是否需要重新加载所有配置信息
         * @param[in]   offline     系统网络是否离线
         */
        virtual void setNetworkChanged(bool reload = false, bool offline = false) = 0;

        /** 打开本地监听
         *
         * @param[in]   protocol    协议类型
         * @param[in]   localHost   监听地址
         * @param[in]   localPort   监听端口
         * @param[in]   receiver    数据接收处理对象
         * @param[in]   reuseAddr   是否重用地址
         *
         * @return      创建成功返回数据发送对象，否则返回 0
         */
        virtual NetSenderPtr listen(const std::string& protocol,const std::string& localHost,int localPort,const NetReceiverPtr& receiver,bool reuseAddr = false) = 0;
        /** 连接远端地址
         *
         * @param[in]   protocol    协议类型
         * @param[in]   localHost   监听地址
         * @param[in]   localPort   监听端口
         * @param[in]   remoteHost  远端地址
         * @param[in]   remotePort  远端端口
         * @param[in]   receiver    数据接收处理对象
         * @param[in]   reuseAddr   是否重用地址
         *
         * @return      创建成功返回数据发送对象，否则返回 0
         */
        virtual NetSenderPtr connect(const std::string& protocol,const std::string& localHost,int localPort,const std::string& remoteHost,int remotePort,const NetReceiverPtr& receiver,bool reuseAddr = false) = 0;

        /** 创建可靠传输连接对象
         *
         * @param[in]   receiver    数据接收处理对象
         * @param[in]   realtime    是否适应实时数据传输处理
         * - true   实时数据，提供有限可靠性保障
         * - false  提供完整可靠性保障
         * @param[in]   sendOffset  初始数据偏移量
         * @param[in]   security    是否加密传输数据
         *
         * @return      创建成功返回可靠传输连接对象，否则返回 0
         */
        virtual NetStreamPtr stream(const StreamReceiverPtr& receiver,bool realtime,int64_t sendOffset = 0,bool security = false) = 0;
        /** 创建安全传输连接对象
         *
         * @param[in]   receiver    数据接收处理对象
         * @param[in]   client      是否是客户端
         * - true   客户端
         * - false  服务短
         * @param[in]   type        安全传输类型
         *
         * @return      创建成功返回安全连接对象，否则返回 0
         */
        virtual NetSslPtr createNetSsl(const SslReceiverPtr& receiver, bool client, SslType type) = 0;

        /** 设置配置
         *
         * @param[in]   name        配置名称
         * @param[in]   value       配置值
         */
        virtual void setConfig(const std::string& name,const std::string& value) = 0;
        /** 获取配置
         *
         * @param[in]   name        配置名称
         *
         * @return      配置值
         */
        virtual std::string getConfig(const std::string& name) = 0;

        /** 获取发送带宽统计
         *
         * @param[in,out]   avgKbps     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      平均发送带宽
         */
        virtual double sendAvgKbps(double *avgKbps = 0, int *count = 0) = 0;
        /** 获取接收带宽统计
         *
         * @param[in,out]   avgKbps     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      平均接收带宽
         */
        virtual double recvAvgKbps(double *avgKbps = 0, int *count = 0) = 0;
        /** 获取发送包速率统计
         *
         * @param[in,out]   avgPkts     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      平均发送包速率
         */
        virtual int sendAvgPkts(int *avgPkts = 0, int *count = 0) = 0;
        /** 获取接收包速率统计
         *
         * @param[in,out]   avgPkts     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      平均接收包速率
         */
        virtual int recvAvgPkts(int *avgPkts = 0, int *count = 0) = 0;

        /** 累计发送流量统计
         *
         * @param[in,out]   totalBytes     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      累计发送流量，单位字节
         */
        virtual int64_t totalSendBytes(int64_t *totalBytes = 0, int *count = 0) = 0;
        /** 累计接收流量统计
         *
         * @param[in,out]   totalBytes     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      累计接收流量，单位字节
         */
        virtual int64_t totalRecvBytes(int64_t *totalBytes = 0, int *count = 0) = 0;
        /** 累计发送包数量统计
         *
         * @param[in,out]   totalPkts     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      累计发送包数量
         */
        virtual int64_t totalSendPkts(int64_t *totalPkts = 0, int *count = 0) = 0;
        /** 累计接收包数量统计
         *
         * @param[in,out]   totalPkts     保存分类型统计内容的地址
         * @param[in,out]   count       分类型统计内容数量
         *
         * @return      累计接收包数量
         */
        virtual int64_t totalRecvPkts(int64_t *totalPkts = 0, int *count = 0) = 0;
        /** 重置累计统计 */
        virtual void totalReset() = 0;

        /** 解析域名地址
         *
         * @param[in]   domain      域名地址
         * @param[out]  hosts4      IPv4 地址
         * @param[out]  hosts6      IPv6 地址
         * @param[out]  defaultIpv6 是否默认使用 IPv6
         * @param[in]   timeout     等待解析超时时间长度，单位毫秒
         *
         * @retval      true        解析成功
         * @retval      false       解析失败
         */
        virtual bool resolveHosts(const std::string& domain,std::vector<std::string>& hosts4,std::vector<std::string>& hosts6,bool& defaultIpv6, int timeout = 6000) = 0;

        /**
         * @brief 批量解析域名地址
         *
         * @param domains 域名地址集合
         * @param results 解析结果集合，key 为域名地址，value 为域名解析结果
         * @param timeout 等待解析超时时长，单位毫秒
         *
         * @retval true 解析成功
         * @retval false 解析失败或者部分成功
         */
        virtual bool resolveHosts(const std::set<std::string> &domains, std::map<std::string, ResolveResult> &results, int timeout = 6000) = 0;
    };
};

#endif
