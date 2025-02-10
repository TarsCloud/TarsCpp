

#pragma once
#include "util/tc_common.h"
#include "util/tc_singleton.h"

namespace tars
{
//统计时间
class TC_ProgressTimer
{
public:
    TC_ProgressTimer()
    {
        start();
    }
    int64_t start()
    {
        _start = TC_Common::now2ms();
        return _start;
    }
    int64_t elapse()
    {
        return (TC_Common::now2ms() - _start);
    }
private:
    int64_t _start;
};

//统计时间
class TC_ProgressTimerU
{
public:
    TC_ProgressTimerU()
    {
        start();
    }
    int64_t start()
    {
        _start = TC_Common::now2us();
        return _start;
    }
    int64_t elapse()
    {
        return (TC_Common::now2us() - _start);
    }
private:
    int64_t _start;
};

//统计连续处理时间
class TC_ContinuousTimer : public TC_Singleton<TC_ContinuousTimer>
{
public:

	/**
	 * 是否开启统计
	 * @param open
	 */
	void enable(bool open);

	/**
	 * add static point
	 * 添加统计点
	 *
	 * @param name : 统计点名称
	 */
    void add(const string &name);

    /**
     * 输出统计信息
     */
    void output();

    /**
     * 输出平均耗时
     */
	void outputAvg();

protected:

	/**
	 * 统计信息
	 */
	list<pair<string, int64_t>> statics();

protected:
    struct RecordPoint
    {
        size_t seq = 0;
        string name;
        int64_t us;
    };

    bool _enable = false;

    static std::atomic<size_t> _seq;

    static std::mutex _mutex;

    static unordered_set<shared_ptr<list<shared_ptr<RecordPoint>>>> _recordPointsList;

    static thread_local shared_ptr<list<shared_ptr<RecordPoint>>> _recordPoints;
};

};
