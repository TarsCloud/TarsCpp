#include "PenglCommon.h"
#include <mutex>
#include <vector>
#include "util/tc_timeprovider.h"

namespace Common
{
    Tackler::Tackler(int64_t firstInterval, int64_t intervalStep, int64_t maxInterval)
        : _firstInterval(firstInterval > 0 ? firstInterval : 0), _intervalStep(intervalStep > 0 ? intervalStep : 0), _maxInterval(maxInterval > firstInterval ? maxInterval : firstInterval)
    {
        reset();
    }

    bool Tackler::failOnce()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_lastFailTime == 0)
        {
            _curInterval = _firstInterval;
        }
        else
        {
            _curInterval += _intervalStep;
            if (_curInterval > _maxInterval)
                _curInterval = _maxInterval;
        }
        _lastFailTime = TNOWMS;
        return _curInterval > 0;
    }

    bool Tackler::needTackle()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _lastFailTime != 0 && TNOWMS - _lastFailTime < _curInterval;
    }

    void Tackler::reset()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _curInterval = _firstInterval - _intervalStep;
        _lastFailTime = 0;
    }


    void CacheServerCall::execute()
    {
        __call->sendResponse(-1, std::vector<char>(), {}, std::string(INNER_ERROR ":cache_call_execute_undefined"));
    }

    void CacheServerCall::fail(const std::string& reason)
    {
        __call->sendResponse(-1, std::vector<char>(), {}, reason);
    }


    ObjectRequisiteLoader::ObjectRequisiteLoader(const TacklerPtr& initTackler, const TacklerPtr& reloadTackler, int64_t reloadInterval, unsigned int maxCacheSize)
        : _initTackler(initTackler),
        _reloadTackler(reloadTackler),
        _reloadInterval(reloadInterval != -1 && reloadInterval < 180000 ? 180000 : reloadInterval),
        _maxCacheSize(maxCacheSize == 0 ? 1 : (maxCacheSize > 8192 ? 8192 : maxCacheSize))
    {
        _status = UNINIT;
    }

    ObjectRequisiteLoader::~ObjectRequisiteLoader()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        std::list<CacheServerCallPtr>::const_iterator it = _calls.begin();
        for (; it != _calls.end(); ++it)
            (*it)->fail(SERVER_ERROR ":obj_uninit");
    }

    void ObjectRequisiteLoader::load_end(const std::string& reason)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_status == INITING)
        {
            std::string everMaxCacheSize;
            //_application->getStatistic("ObjectRequisiteLoader.EverMaxCacheSize", everMaxCacheSize);
            int cacheSize = (int)_calls.size();
            //if (cacheSize > (everMaxCacheSize.empty() ? -1 : std::stoi(everMaxCacheSize)))
            //    _application->setStatistics("ObjectRequisiteLoader.EverMaxCacheSize", std::to_string(cacheSize));
            std::list<CacheServerCallPtr>::const_iterator it = _calls.begin();
            if (reason.empty())
            {
                _status = INITED;
                _loadTime = TNOWMS;
                lock.unlock();
                for (; it != _calls.end(); ++it)
                    (*it)->execute();
                lock.lock();
            }
            else
            {
                if (_initTackler && !_initTackler->failOnce())
                    load_begin(false);
                else
                    _status = INIT_FAIL;
                for (; it != _calls.end(); ++it)
                    (*it)->fail(reason);
            }
            _calls.clear();
        }
        else if (_status == RELOADING)
        {
            if (reason.empty())
            {
                _status = INITED;
                _loadTime = TNOWMS;
                if (_reloadTackler)
                    _reloadTackler->reset();
            }
            else
            {
                //_application->addStatisticsLong("ObjectRequisiteLoader.ReloadFail", 1);
                if (_reloadTackler && !_reloadTackler->failOnce())
                    load_begin(true);
                else
                    _status = RELOADING_FAIL;
            }
        }
        else
        {
            //_application->addStatisticsLong("ObjectRequisiteLoader.StatusConfused." + std::to_string((int)_status), 1);
        }
    }

    void ObjectRequisiteLoader::checkLoad(const CacheServerCallPtr& call)
    {
        ObjStatus oldStatus, status;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            oldStatus = _status;
            switch (_status)
            {
            case INIT_FAIL: 
                if (_initTackler && _initTackler->needTackle())
                    break;
            case UNINIT:
                _status = INITING;
            case INITING:
                if (_calls.size() >= _maxCacheSize) // ���캯����֤_maxCacheSize >= 1.
                {
                    (*_calls.begin())->fail(SERVER_ERROR ":cache_too_much_call");
                    _calls.pop_front();
                }
                _calls.push_back(call);
                break;
            case INITED:
                if (_reloadInterval != -1 && TNOWMS > _loadTime + _reloadInterval)
                    _status = RELOADING;
                break;
            case RELOADING_FAIL:
                if (_reloadTackler && _reloadTackler->needTackle())
                    break;
                _status = RELOADING;
                break;
            default:
                break;
            }
            status = _status;
        }
        switch (status)
        {
        case INIT_FAIL:
            call->fail(SERVER_ERROR ":request_tackled");
            break;
        case INITING:
            if (oldStatus != status)
                load_begin(false);
            break;
        case RELOADING:
            if (oldStatus != status)
                load_begin(true);
        default:
            call->execute();
            break;
        }
    }

    void ObjectRequisiteLoader::deinit()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_status == INITED)
        {
            //->addStatisticsLong("ObjectRequisiteLoader.Deinit", 1);
            _status = UNINIT;
        }
    }
}