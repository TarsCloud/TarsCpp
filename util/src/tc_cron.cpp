#include "util/tc_cron.h"
#include "util/tc_platform.h"


namespace tars
{

const std::vector<std::string> TC_Cron::DAYS = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
const std::vector<std::string> TC_Cron::MONTHS = { "NIL", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

TC_Cron TC_Cron::makecron(const string& expr)
{
    TC_Cron cron;

    if (expr.empty())
    {
        throw TC_CronException("[TC_Cron makecron] invalid empty cron expression");
    }
    vector<string> fields = TC_Common::sepstr<string>(expr, " ");

    if (fields.size() != 6)
        throw TC_CronException("[TC_Cron makecron] cron expression must have six fields");

    setCronField(fields[0], cron.seconds, CRON_MIN_SECONDS, CRON_MAX_SECONDS);
    setCronField(fields[1], cron.minutes, CRON_MIN_MINUTES, CRON_MAX_MINUTES);
    setCronField(fields[2], cron.hours, CRON_MIN_HOURS, CRON_MAX_HOURS);

    setCronDaysOfWeek(fields[5], cron.days_of_week);
    setCronDaysOfMonth(fields[3], cron.days_of_month);
    setCronMonth(fields[4], cron.months);

    cron.isset = true;
    return cron;
}

std::time_t TC_Cron::nextcron(const TC_Cron& cron, std::time_t date)
{
    std::tm val;
    std::tm* dt = time_to_tm(&date, &val);
    if (dt == nullptr) return INVALID_TIME;

    time_t original = tm_to_time(*dt);
    if (INVALID_TIME == original) return INVALID_TIME;

    if (!findNext(cron, *dt, dt->tm_year))
        return INVALID_TIME;

    time_t calculated = tm_to_time(*dt);
    if (INVALID_TIME == calculated) return calculated;

    if (calculated == original)
    {
        addToField(*dt, CronField::second, 1);
        if (!findNext(cron, *dt, dt->tm_year))
            return INVALID_TIME;
    }

    return tm_to_time(*dt);
}

std::time_t TC_Cron::nextcron(const TC_Cron& cron)
{
    std::time_t now = std::time(0);
    return nextcron(cron, now);
}

std::tm* TC_Cron::time_to_tm(std::time_t const* date, std::tm* const out)
{
#if TARGET_PLATFORM_WINDOWS
    errno_t err = localtime_s(out, date);
    return 0 == err ? out : nullptr;
#else
    return localtime_r(date, out);
#endif   
}

std::time_t TC_Cron::tm_to_time(std::tm& tmt)
{
    return std::mktime(&tmt);
}

bool TC_Cron::contains(const std::string& text, char  ch) 
{
    return std::string::npos != text.find_first_of(ch);
}


cron_int TC_Cron::to_cron_int(const std::string& text)
{
    try
    {
        return static_cast<cron_int>(std::stoul(text.data()));
    }
    catch (std::exception const& ex)
    {
        throw TC_CronException(string("[TC_Cron to_cron_int] ") + ex.what());
    }
}

std::string TC_Cron::replaceOrdinals(std::string text, const std::vector<std::string> & replacement)
{
    for (size_t i = 0; i < replacement.size(); ++i)
    {
        auto pos = text.find(replacement[i]);
        if (std::string::npos != pos)
        {
            text.replace(pos, 3, std::to_string(i));
        }
    }
    return text;
}

std::pair<cron_int, cron_int> TC_Cron::makeRange(std::string field, cron_int minval, cron_int maxval)
{
    cron_int first = 0;
    cron_int last = 0;
    if (field.size() == 1 && field[0] == '*')
    {
        first = minval;
        last = maxval;
    }
    else if (!contains(field, '-'))
    {
        first = to_cron_int(field);
        last = first;
    }
    else
    {
        auto parts = TC_Common::sepstr<string>(field, "-");
        if (parts.size() != 2)
            throw TC_CronException("TC_Cron::makeRange] specified range requires two fields");

        first = to_cron_int(parts[0]);
        last = to_cron_int(parts[1]);
    }

    if (first > maxval || last > maxval)
    {
        throw TC_CronException("TC_Cron::makeRange] specified range exceeds maximum");
    }
    if (first < minval || last < minval)
    {
        throw TC_CronException("TC_Cron::makeRange] specified range is less than minimum");
    }
    if (first > last)
    {
        throw TC_CronException("TC_Cron::makeRange] specified range start exceeds range end");
    }

    return { first, last };
}


void TC_Cron::setCronDaysOfWeek(std::string value, std::bitset<7>& target)
{
    auto days = TC_Common::upper(value);
    auto days_replaced = replaceOrdinals(days, DAYS);

    if (days_replaced.size() == 1 && days_replaced[0] == '?')
        days_replaced[0] = '*';

    setCronField(days_replaced,target,CRON_MIN_DAYS_OF_WEEK,CRON_MAX_DAYS_OF_WEEK);
}


void TC_Cron::setCronDaysOfMonth(std::string value, std::bitset<31>& target)
{
    if (value.size() == 1 && value[0] == '?')
        value[0] = '*';

    setCronField(value, target, CRON_MIN_DAYS_OF_MONTH, CRON_MAX_DAYS_OF_MONTH);
}

void TC_Cron::setCronMonth(std::string value, std::bitset<12>& target)
{
    auto month = TC_Common::upper(value);
    auto month_replaced = replaceOrdinals(month, MONTHS);

    setCronField(month_replaced, target, CRON_MIN_MONTHS, CRON_MAX_MONTHS);
}

void TC_Cron::addToField(std::tm& tmt, CronField field, int const val)
{
    switch (field)
    {
    case CronField::second:
        tmt.tm_sec += val;
        break;
    case CronField::minute:
        tmt.tm_min += val;
        break;
    case CronField::hour_of_day:
        tmt.tm_hour += val;
        break;
    case CronField::day_of_week:
    case CronField::day_of_month:
        tmt.tm_mday += val;
        break;
    case CronField::month:
        tmt.tm_mon += val;
        break;
    case CronField::year:
        tmt.tm_year += val;
        break;
    }

    if (INVALID_TIME == tm_to_time(tmt))
        throw TC_CronException("TC_Cron::addToField] invalid time expression");
}

void TC_Cron::setField(std::tm& tmt, CronField field, int const val)
{
    switch (field)
    {
    case CronField::second:
        tmt.tm_sec = val;
        break;
    case CronField::minute:
        tmt.tm_min = val;
        break;
    case CronField::hour_of_day:
        tmt.tm_hour = val;
        break;
    case CronField::day_of_week:
        tmt.tm_wday = val;
        break;
    case CronField::day_of_month:
        tmt.tm_mday = val;
        break;
    case CronField::month:
        tmt.tm_mon = val;
        break;
    case CronField::year:
        tmt.tm_year = val;
        break;
    }

    if (INVALID_TIME == tm_to_time(tmt))
        throw TC_CronException("TC_Cron::setField] invalid time expression");
}

void TC_Cron::resetField(std::tm& tmt, CronField  field)
{
    switch (field)
    {
    case CronField::second:
        tmt.tm_sec = 0;
        break;
    case CronField::minute:
        tmt.tm_min = 0;
        break;
    case CronField::hour_of_day:
        tmt.tm_hour = 0;
        break;
    case CronField::day_of_week:
        tmt.tm_wday = 0;
        break;
    case CronField::day_of_month:
        tmt.tm_mday = 1;
        break;
    case CronField::month:
        tmt.tm_mon = 0;
        break;
    case CronField::year:
        tmt.tm_year = 0;
        break;
    }

    if (INVALID_TIME == tm_to_time(tmt))
        throw TC_CronException("TC_Cron::resetField] invalid time expression");
}

void TC_Cron::resetAllFields(std::tm& tmt, const std::bitset<7> & marked_fields)
{
    for (size_t i = 0; i < marked_fields.size(); ++i)
    {
        if (marked_fields.test(i))
            resetField(tmt, static_cast<CronField>(i));
    }
}

void TC_Cron::markField(std::bitset<7>& orders, CronField field)
{
    if (!orders.test(static_cast<size_t>(field)))
        orders.set(static_cast<size_t>(field));
}

size_t TC_Cron::findNextDay(
    std::tm& tmt,
    std::bitset<31> const& days_of_month,
    size_t day_of_month,
    const std::bitset<7> & days_of_week,
    size_t day_of_week,
    const std::bitset<7> & marked_fields)
{
    unsigned int count = 0;
    unsigned int maximum = 366;
    while (
        (!days_of_month.test(day_of_month - CRON_MIN_DAYS_OF_MONTH) ||
            !days_of_week.test(day_of_week - CRON_MIN_DAYS_OF_WEEK))
        && count++ < maximum)
    {
        addToField(tmt, CronField::day_of_month, 1);

        day_of_month = tmt.tm_mday;
        day_of_week = tmt.tm_wday;

        resetAllFields(tmt, marked_fields);
    }

    return day_of_month;
}

bool TC_Cron::findNext(const TC_Cron & cron, std::tm& tmt, size_t const dot)
{
    bool res = true;

    std::bitset<7> marked_fields{ 0 };
    std::bitset<7> empty_list{ 0 };

    unsigned int second = tmt.tm_sec;
    auto updated_second = findNext(cron.seconds,tmt,CRON_MIN_SECONDS,CRON_MAX_SECONDS,second,CronField::second,CronField::minute,empty_list);

    if (second == updated_second)
    {
        markField(marked_fields, CronField::second);
    }

    unsigned int minute = tmt.tm_min;
    auto update_minute = findNext(cron.minutes,tmt,CRON_MIN_MINUTES,CRON_MAX_MINUTES,minute,CronField::minute,CronField::hour_of_day,marked_fields);
    if (minute == update_minute)
    {
        markField(marked_fields, CronField::minute);
    }
    else
    {
        res = findNext(cron, tmt, dot);
        if (!res) return res;
    }

    unsigned int hour = tmt.tm_hour;
    auto updated_hour = findNext(cron.hours,tmt,CRON_MIN_HOURS,CRON_MAX_HOURS,hour,CronField::hour_of_day,CronField::day_of_week,marked_fields);
    if (hour == updated_hour)
    {
        markField(marked_fields, CronField::hour_of_day);
    }
    else
    {
        res = findNext(cron, tmt, dot);
        if (!res) return res;
    }

    unsigned int day_of_week = tmt.tm_wday;
    unsigned int day_of_month = tmt.tm_mday;
    auto updated_day_of_month = findNextDay(tmt,cron.days_of_month,day_of_month,cron.days_of_week,day_of_week,marked_fields);
    if (day_of_month == updated_day_of_month)
    {
        markField(marked_fields, CronField::day_of_month);
    }
    else
    {
        res = findNext(cron, tmt, dot);
        if (!res) return res;
    }

    unsigned int month = tmt.tm_mon;
    auto updated_month = findNext(cron.months,tmt,CRON_MIN_MONTHS,CRON_MAX_MONTHS,month,
                                  CronField::month,CronField::year,marked_fields);
    if (month != updated_month)
    {
        if (tmt.tm_year - dot > CRON_MAX_YEARS_DIFF)
            return false;

        res = findNext(cron, tmt, dot);
        if (!res) return res;
    }

    return res;
}

}





