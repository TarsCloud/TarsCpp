/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */

#include "servant/PropertyReport.h"
#include "util/tc_common.h"

namespace tars
{

const string PropertyReport::Sum = "Sum";
const string PropertyReport::Distr = "Distr";
const string PropertyReport::Avg = "Avg";
const string PropertyReport::Min = "Min";
const string PropertyReport::Max = "Max";
const string PropertyReport::Count = "Count";

string PropertyReport::sumProperty::get()
{
    string s = TC_Common::tostr(_d);

    clear();

    return s;
}

string PropertyReport::avgProperty::get()
{
    if(_count == 0)
    {
        return "0";
    }
    string s = TC_Common::tostr(static_cast<double>(_sum)/_count);

    clear();

    return s;
}

PropertyReport::distrProperty::distrProperty(const vector<int>& range)
{
    _range = range;

    std::sort(_range.begin(), _range.end());

    _range.erase(unique(_range.begin(), _range.end()),_range.end());

    _result.resize(_range.size());
}

void PropertyReport::distrProperty::set(int o)
{
    vector<int>::iterator it = std::upper_bound(_range.begin(), _range.end(), o);

    if (it != _range.end())
    {
        size_t n = it - _range.begin();

        ++_result[n];
    }
}

string PropertyReport::distrProperty::get()
{
    string s = "";

    for(unsigned i = 0; i < _range.size(); ++i)
    {
        if (i != 0)
        {
            s += ",";
        }
        s = s + TC_Common::tostr(_range[i]) + "|" + TC_Common::tostr(_result[i]);
    }
    for(unsigned i = 0; i < _result.size(); ++i)
    {
        _result[i] = 0;
    }
    return s;
}

string PropertyReport::maxProperty::get()
{
    string s = TC_Common::tostr(_d);

    clear();

    return s;
}

string PropertyReport::minProperty::get()
{
    string s = TC_Common::tostr(_d);

    clear();

    return s;
}

void PropertyReport::minProperty::set(int o)
{
    //非0最小值
    if(_d == 0 ||(_d > o && o != 0))
    {
         _d = o;
    }
}

string PropertyReport::countProperty::get()
{
    string s = TC_Common::tostr(_d);

    clear();

    return s;
}
///////////////////////////////////////////////
}
