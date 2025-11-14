#include "SipRegImp.h"
#include "servant/Application.h"

//////////////////////////////////////////////////////
void SipRegImp::initialize()
{
    m_running = true;
    
    // 启动定时清理线程
    m_cleanupThread = std::thread([this]() {
        while (m_running) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            cleanupExpiredServices();
        }
    });
    m_cleanupThread.detach();
    
    TLOGINFO("SipRegImp initialized" << endl);
}

//////////////////////////////////////////////////////
void SipRegImp::destroy()
{
    m_running = false;
    TLOGINFO("SipRegImp destroyed" << endl);
}

tars::Int32 SipRegImp::registerService(const VoipApp::ServiceInfo & serviceInfo, tars::TarsCurrentPtr current)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // 更新服务信息
    VoipApp::ServiceInfo info = serviceInfo;
    info.updateTime = TNOW;
    
    m_services[serviceInfo.serviceId] = info;
    
    TLOGINFO("Register service: " << serviceInfo.serviceId 
            << ", type:" << serviceInfo.serviceType << endl);
    
    return 0;
}

tars::Int32 SipRegImp::queryServices( const VoipApp::QueryCondition & condition,
    vector<VoipApp::ServiceInfo> &services, tars::TarsCurrentPtr current)
{
    services.clear();

    std::lock_guard<std::mutex> lock(m_mutex);
    // 遍历所有服务，根据条件过滤
    for (const auto& pair : m_services) {
        const auto& service = pair.second;
        
        // 过滤serviceType
        if (!condition.serviceType.empty() && 
            service.serviceType != condition.serviceType) {
            continue;
        }

        // 过滤attributes（可选）
        // if (!condition.attributes.empty()) {
        //     // 实现属性过滤逻辑
        // }

        services.push_back(service);
    }

    TLOGDEBUG("Query services, type:" << condition.serviceType 
                << ", found:" << services.size() << endl);

    return 0;
}

tars::Int32 SipRegImp::unregisterService(const string & serviceId, tars::TarsCurrentPtr current)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_services.erase(serviceId);
    
    TLOGINFO("Unregister service: " << serviceId << endl);
    return 0;
}

tars::Int32 SipRegImp::heartbeat(const string & serviceId, tars::TarsCurrentPtr current)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_services.find(serviceId);
    if (it != m_services.end()) {
        it->second.updateTime = TNOW;
        return 0;
    } else {
        TLOGWARN("Heartbeat for unknown service: " << serviceId << endl);
        return -2;  // 服务不存在，客户端需要重新注册
    }
}

void SipRegImp::cleanupExpiredServices()
{
    long now = TNOW;
    vector<string> expiredServices;

    std::lock_guard<std::mutex> lock(m_mutex);
    for (const auto& pair : m_services) {
        const auto& service = pair.second;
        
        // 检查是否超时（ttl秒没有心跳）
        if (now - service.updateTime > service.ttl) {
            expiredServices.push_back(service.serviceId);
        }
    }

    // 删除过期服务
    for (const auto& serviceId : expiredServices) {
        m_services.erase(serviceId);
        TLOGWARN("Remove expired service: " << serviceId << endl);
    }
}

