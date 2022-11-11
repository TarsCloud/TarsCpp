#include "util/tc_split.h"

namespace tars
{
    bool TC_Split::hasNext() const
    {
        return _p != NULL && (*_p != '\0');
    }
    string TC_Split::getNextAsString()
    {
        return getNextSection();
    }

    double TC_Split::getNextAsDouble()
    {
        return atof(getNextSection().c_str());
    }

    int64_t TC_Split::getNextAsLong()
    {

        return atoll(getNextSection().c_str());
    }

    int TC_Split::getNextAsInt()
    {
        return atoi(getNextSection().c_str());

    }

    string TC_Split::getNextSection()
    {
        const char * begin = _p;
        const char * end = _p;

        while (*_p != '\0' && !isSepFlag(*_p))
        {
            _p++;
        }

        end = _p;
        //跳过 
        if (*_p != '\0')
        {
            _p++;
        }

        return string(begin, end);
    }

    string TC_Split::getValue(size_t pos) const
    {
        const char * tmp = _p;
        const char * begin = tmp;
        const char * end = tmp;

        size_t i = 0;
        while (*tmp != '\0' && (i < pos))
        {
            if (isSepFlag(*tmp))
            {
                i++;
            }
            tmp++;
        }
        begin = tmp;
        while (*tmp != '\0' && !isSepFlag(*tmp))
        {
            tmp++;
        }
        end = tmp;

        return string(begin, end);
    }

    string TC_Split::getValueAsString(size_t pos)
    {
        return getValue(pos);
    }

    double TC_Split::getValueAsDouble(size_t pos)
    {
        return atof(getValue(pos).c_str());
    }

    int64_t TC_Split::getValueAsLong(size_t pos)
    {
        return atoll(getValue(pos).c_str() );
    }

    int TC_Split::getValueAsInt(size_t pos)
    {
        return atoi(getValue(pos).c_str());
    }
}
