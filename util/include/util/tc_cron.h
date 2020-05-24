#pragma once

#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <bitset>
#include "util/tc_common.h"
#include "util/tc_ex.h"
#include <ctime>

namespace tars
{

struct TC_CronException : public TC_Exception
{
public:
	TC_CronException(const string& buffer) : TC_Exception(buffer) {};
//	TC_CronException(const string& buffer, bool err) : TC_Exception(buffer, err) {};
    ~TC_CronException() throw() {};
};

typedef uint8_t cron_int;

class TC_Cron
{
public:
    enum class CronField
    {
        second,
        minute,
        hour_of_day,
        day_of_week,
        day_of_month,
        month,
        year
    };

    static const std::time_t INVALID_TIME = static_cast<std::time_t>(-1);
    static const size_t INVALID_INDEX = static_cast<size_t>(-1);

public: 
    /**
     * @brief  创建一个cron对象
     * @brief  Create a cron object
     * @return  cron 对象
     * @return  cron object
     */
     // 字段分别为  <seconds> <minutes> <hours> <days of month> <months> <days of week> 
     // The fields are: <seconds> <minutes> <hours> <days of month> <months> <days of week>
     // 通配符以及含义
     // Wildcards and their meanings:
     // *	all values	selects all values within a field
     // -	range	specify ranges
     // ,	comma	specify additional values
     // /	slash	speficy increments
     // DAYS = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
     // MONTHS = { "NIL", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
     // 各类例子
     // Examples:
     // CRON	Description
     // * * * * * *	Every second
     // */5 * * * * *	Every 5 seconds
     // 0 */5 */2 * * *	Every 5 minutes, every 2 hours
     // 0 */2 */2 * */2 */2	Every 2 minutes, every 2 hours, every 2 days of the week, every 2 months
     // 0 15 10 * * * *	10:15 AM every day
     // 0 0/5 14 * * *	Every 5 minutes starting at 2 PM and ending at 2:55 PM, every day
     // 0 10,44 14 * 3 WED	2:10 PM and at 2:44 PM every Wednesday of March
     // 0 15 10 * * MON-FRI	10:15 AM every Monday, Tuesday, Wednesday, Thursday and Friday
     // 0 0 12 1/5 * *	12 PM every 5 days every month, starting on the first day of the month
     // 0 11 11 11 11 *	Every November 11th at 11:11 AM
    static TC_Cron makecron(const string & expr);

    /**
     * @brief 获取cron对象的下一个时间点
     * @brief Get the next point in time for the cron object
     * @param  cron 对象
     * @param  cron object
     * @return 如果返回 INVALID_TIME，则是错误时间，否则返回正确的时间戳
     * @return If INVALID_TIME is returned, it is the wrong time. Otherwise the correct timestamp is returned.
     */
    static std::time_t nextcron( const TC_Cron& cron, std::time_t timestamp);
   
    /**
     * @brief 获取cron对象的下一个时间点
     * @brief Get the next point in time for the cron object.
     * @param  cron 对象
     * @param  cron object
     * @return 如果返回 INVALID_TIME，则是错误时间，否则返回正确的时间戳
     * @return If INVALID_TIME is returned, it is the wrong time. Otherwise the correct timestamp is returned.
     */
    static std::time_t nextcron(const TC_Cron& cron);

public:
    std::bitset<60> seconds;
    std::bitset<60> minutes;
    std::bitset<24> hours;
    std::bitset<7>  days_of_week;
    std::bitset<31> days_of_month;
    std::bitset<12> months;
    bool isset = false;

public:
    static const cron_int CRON_MIN_SECONDS = 0;
    static const cron_int CRON_MAX_SECONDS = 59;

    static const cron_int CRON_MIN_MINUTES = 0;
    static const cron_int CRON_MAX_MINUTES = 59;

    static const cron_int CRON_MIN_HOURS = 0;
    static const cron_int CRON_MAX_HOURS = 23;

    static const cron_int CRON_MIN_DAYS_OF_WEEK = 0;
    static const cron_int CRON_MAX_DAYS_OF_WEEK = 6;

    static const cron_int CRON_MIN_DAYS_OF_MONTH = 1;
    static const cron_int CRON_MAX_DAYS_OF_MONTH = 31;

    static const cron_int CRON_MIN_MONTHS = 1;
    static const cron_int CRON_MAX_MONTHS = 12;

    static const cron_int CRON_MAX_YEARS_DIFF = 4;

    static const std::vector<std::string> DAYS;
    static const std::vector<std::string> MONTHS;

protected:
    /**
     * @brief 判断是否包含字符
     * @brief Determine whether it includes characters.
     * @return true 包含;false 不包含
     * @return true include; false not include
     */
    static bool contains(const std::string &text, char ch) ;

    /**
     * @brief 转换成crontab合法数字
     * @brief Convert to crontab legal number
     * @return cron_int
     */
    static cron_int to_cron_int(const std::string& text);

    /**
     * @brief 时间格式转换函数
     * @brief Time format conversion function
     */
    static std::time_t tm_to_time(std::tm& tmt);
    static std::tm* time_to_tm(std::time_t const* date, std::tm* const out);

 protected:
    /**
    * @brief 替换周/月的字符为下标
    * @brief Replace week/month characters as Subscripts
    */
    static std::string replaceOrdinals(std::string text, const std::vector<std::string> & replacement);

    /**
    * @brief 获取时间范围，用于计算通配符
    * @brief Gets the time range for calculating wildcards.
    */
    static std::pair<cron_int, cron_int> makeRange(std::string field, cron_int minval, cron_int maxval);

    /**
    * @brief 设置crontab不同的位置标记
    * @brief Set different crontab location tags
    */
    template <size_t N>
    static void setCronField(std::string value, std::bitset<N>& target, cron_int minval, cron_int maxval);
    static void setCronDaysOfWeek(std::string value, std::bitset<7>& target);
    static void setCronDaysOfMonth(std::string value, std::bitset<31>& target);
    static void setCronMonth(std::string value, std::bitset<12>& target);

    /**
    * @brief 计算下一个时间戳
    * @brief Calculate the next timestamp
    */
    template <size_t N>
    static size_t findNext(const std::bitset<N> & target, std::tm& tmt, unsigned int minimum, unsigned int maximum, unsigned int value,
                           CronField field, CronField next_field, const std::bitset<7> & marked_fields);
    static bool findNext(const TC_Cron & cron, std::tm& tmt, size_t dot);
    static size_t findNextDay(std::tm& tmt,std::bitset<31> const& days_of_month,size_t day_of_month,const std::bitset<7> & days_of_week,size_t day_of_week,const std::bitset<7> & marked_fields);

    /**
    * @brief 位图控制函数
    * @brief Bitmap control function
    */
    template <size_t N>
    static size_t nextSetbit(const std::bitset<N> & target, size_t minimum, size_t maximum, size_t offset);
    static void addToField(std::tm& tmt, CronField field, int val);
    static void setField(std::tm& tmt, CronField field, int val);
    static void resetField(std::tm& tmt, CronField field);
    static void resetAllFields(std::tm& tmt, const std::bitset<7> & marked_fields);
    static void markField(std::bitset<7>& orders, CronField  field);
};

template <size_t N>
void TC_Cron::setCronField(std::string value,std::bitset<N>& target,cron_int minval,cron_int maxval)
{
    if (value.length() > 0 && value[value.length() - 1] == ',')
        throw TC_CronException("[TC_Cron::setCronField] value cannot end with comma");

    auto fields = TC_Common::sepstr<string>(value, ",");
    if (fields.empty())
        throw TC_CronException("[TC_Cron::setCronField] expression parsing error");

    for (auto const& field : fields)
    {
        if (!contains(field, '/'))
        {
            auto item = makeRange(field, minval, maxval);
            auto first = item.first;
            auto last = item.second;
            for (cron_int i = first - minval; i <= last - minval; ++i)
            {
                target.set(i);
            }
        }
        else
        {
            auto parts = TC_Common::sepstr<string>(field, "/");
            if (parts.size() != 2)
                throw TC_CronException("[TC_Cron::setCronField] incrementer must have two fields");

            auto item = makeRange(parts[0], minval, maxval);
            auto first = item.first;
            auto last = item.second;

            if (!contains(parts[0], '-'))
            {
                last = maxval;
            }

            auto delta = to_cron_int(parts[1]);
            if (delta <= 0)
                throw TC_CronException("[TC_Cron::setCronField] incrementer must be a positive value");

            for (cron_int i = first - minval; i <= last - minval; i += delta)
            {
                target.set(i);
            }
        }
    }
}

template <size_t N>
size_t TC_Cron::nextSetbit(const std::bitset<N> & target,size_t minimum,size_t maximum,size_t offset)
{
    for (auto i = offset; i < N; ++i)
    {
        if (target.test(i)) return i;
    }

    return INVALID_INDEX;
}

template <size_t N>
size_t TC_Cron::findNext(const std::bitset<N> & target, std::tm& tmt, unsigned int minimum, unsigned int  maximum, unsigned int  value,
    CronField  field, CronField  next_field, const std::bitset<7> & marked_fields)
{
    auto next_value = nextSetbit(target, minimum, maximum, value);
    if (INVALID_INDEX == next_value)
    {
        addToField(tmt, next_field, 1);
        resetField(tmt, field);
        next_value = nextSetbit(target, minimum, maximum, 0);
    }

    if (INVALID_INDEX == next_value || next_value != value)
    {
        setField(tmt, field, static_cast<int>(next_value));
        resetAllFields(tmt, marked_fields);
    }

    return next_value;
}

}

