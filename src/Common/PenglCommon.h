#ifndef __PenglCommon_h
#define __PenglCommon_h

#include <string>
#include <map>
#include <set>
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include <thread>
#include <cstdint>
#include <stdexcept>
#include "servant/Application.h"
#include "servant/Current.h"
#include "util/tc_timeprovider.h"
#include "Reason.h"

namespace Common
{
#define ERR_BREAK_IF(condition, r) \
    { \
        if (condition) \
        { \
            reason = r; \
            break; \
        } \
    }

#define ERR_RET_IF(condition, r) \
    { \
        if (condition) \
        { \
            reason = r; \
            return false; \
        } \
    }

#define ERR_CONTINUE_IF(condition, r) \
    { \
        if (condition) \
        { \
            reason = r; \
            continue; \
        } \
    }

// #define ALERT(application) \
//     application->addStatisticsLong(INNER_ERROR ":" __FILE__ "_" + std::to_string(__LINE__), 1);

// #define ALERT_REASON(application, reason) \
//     { \
//         reason = std::string(REASON_INNER_ERROR) + ":" __FILE__ "_" + std::to_string(__LINE__); \
//         application->addStatisticsLong(reason, 1); \
//     }

// #define ALERT_REASON_PRO(debugStr, application, reason) \
//     { \
//         reason = std::string(REASON_INNER_ERROR) + ":" __FILE__ "_" + std::to_string(__LINE__); \
//         application->addStatisticsLong(reason, 1); \
//         std::string debugKey = "debug:" __FILE__ "_" + std::to_string(__LINE__); \
//         std::string debugStrs = application->getStatistic(debugKey); \
//         if (debugStrs.size() < 128 && debugStrs.find("|" + debugStr + "|") == std::string::npos) \
//         { \
//             if (debugStrs.empty()) \
//                 debugStrs = "|"; \
//             application->setStatistics(debugKey, debugStrs + debugStr + "|"); \
//         } \
//     }

// #define ASSERT_REASON_BREAK(cond, application, reason) \
//     { \
//         if (!(cond)) \
//         { \
//             ALERT_REASON(application, reason) \
//             break; \
//         } \
//     }

// #define ASSERT_REASON_PRO_BREAK(cond, debugStr, application, reason) \
//     { \
//         if (!(cond)) \
//         { \
//             ALERT_REASON_PRO(debugStr, application, reason) \
//             break; \
//         } \
//     }

// #define ASSERT_REASON_CONTINUE(cond, application, reason) \
//     { \
//         if (!(cond)) \
//         { \
//             ALERT_REASON(application, reason) \
//             continue; \
//         } \
//     }

// #define ASSERT_REASON_RET(cond, application, reason) \
//     { \
//         if (!(cond)) \
//         { \
//             ALERT_REASON(application, reason) \
//             return false; \
//         } \
//     }

// #define ASSERT_RET(cond, application) \
//     { \
//         if (!(cond)) \
//         { \
//             std::string reason; \
//             ALERT_REASON(application, reason) \
//             return; \
//         } \
//     }

// #define ASSERT_EXCEPTION(cond, application, reason) \
//     { \
//         if (!(cond)) \
//         { \
//             ALERT_REASON(application, reason) \
//             throw std::runtime_error(reason); \
//         } \
//     }

    template<typename K, typename V>
    void applyDiff(map<K, V>& base, const map<K, V>& diff, bool eraseEmpty = false) // If accumulate diffs, let eraseEmpty be false.
    {
        typename map<K, V>::const_iterator it = diff.begin();
        for (; it != diff.end(); ++it)
        {
            if (eraseEmpty && it->second == V())
                base.erase(it->first);
            else
                base[it->first] = it->second;
        }
    }

    template<typename T>
    void mergeMap(map<std::string, T>& src, const map<std::string, T>& diff)
    {
        typename map<std::string, T>::const_iterator it = diff.begin();
        for (; it != diff.end(); ++it)
            src[it->first] = it->second;
    }

    template<typename T>
    void filterMap(const std::set<std::string>& prefixs, map<std::string, T>& props)
    {
        if (prefixs.find("") != prefixs.end())
            return;

        typename map<std::string, T>::const_iterator propIt = props.begin();
        for (; propIt != props.end();)
        {
            std::set<std::string>::const_iterator prefixIt = prefixs.begin();
            for (; prefixIt != prefixs.end(); ++prefixIt)
            {
                if (propIt->first.substr(0, prefixIt->size()) == *prefixIt)
                {
                    ++propIt;
                    break;
                }
            }
            if (prefixIt == prefixs.end())
                props.erase((propIt++)->first);
        }
    }

    template<typename T>
    void filterMapEx(const std::set<std::string>& prefixs, const map<std::string, T>& props, map<std::string, T>& rslt)
    {
        if (prefixs.find("") != prefixs.end())
        {
            rslt = props;
            return;
        }

        typename map<std::string, T>::const_iterator propIt = props.begin();
        for (; propIt != props.end(); ++propIt)
        {
            std::set<std::string>::const_iterator prefixIt = prefixs.begin();
            for (; prefixIt != prefixs.end(); ++prefixIt)
            {
                if (propIt->first.substr(0, prefixIt->size()) == *prefixIt)
                {
                    rslt.insert(make_pair(propIt->first, propIt->second));
                    break;
                }
            }
        }
    }


    class RWMutex
    {
    public:
        RWMutex()
            : _lock(0) {};
        void rlock()
        {
            while (1)
            {
                if ((_lock += 2) % 2 == 0)
                    break;
                _lock -= 2;
                while (_lock.load() % 2 != 0)
                    std::this_thread::yield();
            }
        }
        void runlock()
        {
            _lock -= 2;
        }
        void wlock()
        {
            while (1)
            {
                if ((_lock += 1) == 0)
                    break;
                _lock -= 1;
                while (_lock.load() != 0)
                    std::this_thread::yield();
            }
        }
        void wunlock()
        {
            _lock -= 1;
        }
    private:
        std::atomic<int> _lock;
        friend class ReadLock;
        friend class WriteLock;
    };

    class ReadLock
    {
    public:
        ReadLock(RWMutex* mutex)
            : _mutex(mutex)
        {
            _mutex->rlock();
        }
        ReadLock(RWMutex& mutex)
            : _mutex(&mutex)
        {
            _mutex->rlock();
        }
        ~ReadLock()
        {
            _mutex->runlock();
        }
    private:
        RWMutex* _mutex;
    };

    class WriteLock
    {
    public:
        WriteLock(RWMutex* mutex)
            : _mutex(mutex)
        {
            _mutex->wlock();
        }
        WriteLock(RWMutex& mutex)
            : _mutex(&mutex)
        {
            _mutex->wlock();
        }
        ~WriteLock()
        {
            _mutex->wunlock();
        }
    private:
        RWMutex* _mutex;
    };

    class Tackler
    {
    public:
        Tackler(int64_t firstInterval, int64_t intervalStep, int64_t maxInterval);
        bool failOnce();
        bool needTackle();
        void reset();
    private:
        const int64_t _firstInterval, _intervalStep, _maxInterval;
        int64_t _curInterval, _lastFailTime;
        mutable std::mutex _mutex;
    };
    typedef std::shared_ptr<Tackler>             TacklerPtr;

    class CacheServerCall
    {
    public:
        CacheServerCall(const tars::TarsCurrentPtr& call)
            : __call(call) {};
        virtual void execute();
        virtual void fail(const std::string& reason);
    protected:
        const tars::TarsCurrentPtr __call;
    };
    typedef std::shared_ptr<CacheServerCall>     CacheServerCallPtr;

    class ObjectRequisiteLoader
    {
    public:
        enum ObjStatus
        {
            UNINIT = 0,
            INITING,
            INIT_FAIL,
            INITED,
            RELOADING,
            RELOADING_FAIL
        };
    public:
        ObjectRequisiteLoader(const TacklerPtr& initTackler = 0, const TacklerPtr& reloadTackler = 0, int64_t reloadInterval = -1/*never reload*/, unsigned int maxCacheSize = 1024);
        virtual ~ObjectRequisiteLoader();
        virtual void load_begin(bool reload) = 0;
        void load_end(const std::string& reason);
        void checkLoad(const CacheServerCallPtr& call);
        void deinit();
        ObjStatus getObjStatus() { return _status; }
    public:
        tars::Application* _application;
    private:
        TacklerPtr _initTackler, _reloadTackler;
        const int64_t _reloadInterval;
        const unsigned int _maxCacheSize;
        ObjStatus _status;
        int64_t _loadTime;
        std::list<CacheServerCallPtr> _calls;
        mutable std::mutex _mutex;
    };
}

#endif