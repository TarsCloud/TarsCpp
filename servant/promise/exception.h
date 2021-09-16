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

#ifndef _TARS_EXCEPTION_H_
#define _TARS_EXCEPTION_H_

#include <exception>
#include <memory>
#include <cstring>
#include <string>

namespace tars {
    
class ExceptionBase : public std::exception {
public:
    ExceptionBase() {}
    ~ExceptionBase() throw() override {}

    const char* what() const throw() override
    {
        return "exception_base";
    }

    virtual ExceptionBase* clone() const
    {
        return new ExceptionBase(*this);
    }

    virtual int code() const { return -1; }

    virtual void rethrow() const { throw *this; }
};

class Exception : public ExceptionBase {
public:
    Exception(const std::string& info, const int code = 0)
        : m_info(info)
        , m_code(code)
    {
        if (code != 0)
            m_info = m_info + ":" + std::strerror(code);
    }

    ~Exception() throw() override
    {
    }

    const char* what() const throw() override
    {
        return m_info.c_str();
    }

    ExceptionBase* clone() const override 
    {
        return new Exception(*this);
    }

    int code() const override { return m_code; }

    void rethrow() const override { throw *this; }

protected:
    std::string m_info;
    int m_code;
};

using ExceptionPtr = std::shared_ptr<ExceptionBase>;

inline ExceptionPtr currentException()
{
    try {
        throw;
    } catch (Exception& e) {
        return ExceptionPtr(e.clone());
    } catch (ExceptionBase& e) {
        return ExceptionPtr(e.clone());
    } catch (std::exception& e) {
        return ExceptionPtr(new Exception(std::string(e.what())));
    } catch (...) {
    }

    return ExceptionPtr(new Exception("unknow_exception"));
}

inline ExceptionPtr copyException(const std::string& e, int err)
{
    try {
        throw Exception(e, err);
    } catch (...) {
        return currentException();
    }    
}

} // end namespace tars

#endif
