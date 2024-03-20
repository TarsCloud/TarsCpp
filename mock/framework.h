#pragma once

#include <string>

const static std::string &MOCK_FRAMEWORK_CONFIG = R"(
<tars>
  <application>
    #proxy需要的配置
    <client>
        #地址
        locator                     = tars.tarsmock.QueryObj@tcp -h 127.0.0.1 -p 17890
        #最大超时时间(毫秒)
        sync-invoke-timeout         = 5000
        async-invoke-timeout        = 60000
        #刷新端口时间间隔(毫秒)
        refresh-endpoint-interval   = 5000
        #模块间调用[可选]
        stat                        = tars.tarsmock.StatObj
        #发送队列长度
        sendqueuelimit              = 100000
        #异步回调队列个数限制
        asyncqueuecap               = 100000
        #网络异步回调线程个数
        asyncthread                 = 3
        #网络线程个数
        netthread                   = 2
        #合并回调线程和网络线程(以网络线程个数为准)
        mergenetasync               = 0
        #模块名称
        modulename                  = tars.tarsmock

    </client>

    #定义所有绑定的IP
    <server>
        start_output = ERROR
        closecout = 0
        #应用名称
        app      = tars
        #服务名称
        server   = tarsmock
        #服务的数据目录,可执行文件,配置文件等
        basepath = .
        datapath = .
        #日志路径
        logpath  = .
        #网络线程个数
        netthread = 1
        #合并网络和业务线程(以网络线程个数为准)
        mergenetimp = 0
        opencoroutine = 0
        loglevel=TARS

        #本地管理套接字[可选]
        local   = tcp -h 127.0.0.1 -p 15791 -t 10000

        #本地node的ip:port:timeout[可选]
#        node    = ServerObj@tcp -h 127.0.0.1 -p 2345 -t 10000
        #配置中心的地址[可选]
        config  = tars.tarsmock.ConfigObj
        #配置中心的地址[可选]
#		notify  = tars.tarsconfig.NotifyObj
        #远程LogServer[可选]
        log     = tars.tarsmock.LogObj

        <ConfigAdapter>
            #ip:port:timeout
            endpoint = tcp -h 127.0.0.1 -p 11003 -t 60000
            #allow ip
            allow	 =
            #max connection num
            maxconns = 4096
            #imp thread num
            threads	 = 5
            #servant
            servant = tars.tarsmock.ConfigObj
            #queue capacity
            queuecap = 1000000
        </ConfigAdapter>

        <LogAdapter>
            #ip:port:timeout
            endpoint = tcp -h 127.0.0.1 -p 11005 -t 60000
            #allow ip
            allow	 =
            #max connection num
            maxconns = 4096
            #imp thread num
            threads	 = 5
            #servant
            servant = tars.tarsmock.LogObj
            #queue capacity
            queuecap = 1000000
        </LogAdapter>

        <RegistryAdapter>
            #ip:port:timeout
            endpoint = tcp -h 127.0.0.1 -p 17890 -t 60000
            #allow ip
            allow	 =
            #max connection num
            maxconns = 4096
            #imp thread num
            threads	 = 5
            #servant
            servant = tars.tarsmock.QueryObj
            #queue capacity
            queuecap = 1000000
        </RegistryAdapter>

        <StatAdapter>
            #ip:port:timeout
            endpoint = tcp -h 127.0.0.1 -p 12004 -t 60000
            #allow ip
            allow	 =
            #max connection num
            maxconns = 4096
            #imp thread num
            threads	 = 5
            #servant
            servant = tars.tarsmock.StatObj
            #queue capacity
            queuecap = 1000000
        </StatAdapter>
    </server>
  </application>

  <log>
    logpath = .
    logthread = 1
    ipfix = true
    <format>
        hour=app.server.file|app2.server2.file2
    </format>
    <logtype>
        app.server.file = hour
    </logtype>

  </log>
</tars>

)";


