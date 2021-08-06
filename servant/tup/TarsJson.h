#ifndef __TARS_JSON_H__
#define __TARS_JSON_H__

#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <string.h>
#include "tup/TarsType.h"
#include "util/tc_json.h"
#include "util/tc_common.h"

namespace tars
{
class JsonInput
{
public:
    static void readJson(tars::Bool& b, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeBoolean)
        {
            b = JsonValueBooleanPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Bool' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Char& c, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            c = (char)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Char' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(UInt8& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (UInt8)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Uint8' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Short& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (Short)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Short' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(UInt16& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (UInt16)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Uint16' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Int32& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (Int32)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Int32' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(UInt32& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (UInt32)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'UInt32' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Int64& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (Int64)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Int64' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Float& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = (Float)JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Float' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(Double& n, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeNum)
        {
            n = JsonValueNumPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Double' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(std::string& s, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeString)
        {
            s = JsonValueStringPtr::dynamicCast(p)->value;
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'string' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    static void readJson(char *buf, const UInt32 bufLen, UInt32 & readLen, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeString)
        {
            JsonValueStringPtr pString=JsonValueStringPtr::dynamicCast(p);
            if((UInt32)pString->value.size()>bufLen)
            {
                char s[128];
                snprintf(s, sizeof(s), "invalid char * size, size: %u", (UInt32)pString->value.size());
                throw TC_Json_Exception(s);
            }
            memcpy(buf,pString->value.c_str(),pString->value.size());
            readLen = (UInt32)pString->value.size();
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'char *' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }


    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<string, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<string, V> pr;
                pr.first=iter->first;
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<bool, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<bool, V> pr;
                pr.first=TC_Common::strto<bool>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Char, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Char, V> pr;
                pr.first=TC_Common::strto<Int32>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }
    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<UInt8, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<UInt8, V> pr;
                pr.first=TC_Common::strto<UInt8>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }
    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Short, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Short, V> pr;
                pr.first=TC_Common::strto<Short>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }
    
    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<UInt16, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<UInt16, V> pr;
                pr.first=TC_Common::strto<UInt16>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Int32, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Int32, V> pr;
                pr.first=TC_Common::strto<Int32>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<UInt32, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<UInt32, V> pr;
                pr.first=TC_Common::strto<UInt32>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Int64, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Int64, V> pr;
                pr.first=TC_Common::strto<Int64>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Float, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Float, V> pr;
                pr.first=TC_Common::strto<Float>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<Double, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            auto iter=pObj->value.begin();
            for(;iter!=pObj->value.end();++iter)
            {
                std::pair<Double, V> pr;
                pr.first=TC_Common::strto<Double>(iter->first);
                readJson(pr.second,iter->second);
                m.insert(pr);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'map' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename K, typename V, typename Cmp, typename Alloc>
    static void readJson(std::map<K, V, Cmp, Alloc>& m, const JsonValuePtr & p, bool isRequire = true)
    {
        char s[128];
        snprintf(s, sizeof(s), "map key is not Basic type. map key is only string|bool|num");
        throw TC_Json_Exception(s);
    }

    template<typename T, typename Alloc>
    static void readJson(std::vector<T, Alloc>& v, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeArray)
        {
            JsonValueArrayPtr pArray=JsonValueArrayPtr::dynamicCast(p);
            v.resize(pArray->value.size());
            for(size_t i=0;i<pArray->value.size();++i)
            {
                readJson(v[i],pArray->value[i]);
            }
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'vector' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    /// 读取结构数组
    template<typename T>
    static void readJson(T* v, const UInt32 len, UInt32 & readLen, const JsonValuePtr & p, bool isRequire = true)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeArray)
        {
            JsonValueArrayPtr pArray=JsonValueArrayPtr::dynamicCast(p);
            if(pArray->value.size()>len)
            {
                char s[128];
                snprintf(s, sizeof(s), "read 'T *' invalid size, size: %u", (uint32_t)pArray->value.size());
                throw TC_Json_Exception(s);
            }
            for(size_t i=0;i<pArray->value.size();++i)
            {
                readJson(v[i],pArray->value[i]);
            }
            readLen=pArray->value.size();
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'T *' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }

    template<typename T>
    static void readJson(T& v, const JsonValuePtr & p, bool isRequire = true, typename detail::disable_if<detail::is_convertible<T*, TarsStructBase*>, void ***>::type dummy = 0)
    {
        Int32 n = 0;
        readJson(n, p, isRequire);
        v = (T) n;
    }

    /// 读取结构
    template<typename T>
    static void readJson(T& v, const JsonValuePtr & p, bool isRequire = true, typename detail::enable_if<detail::is_convertible<T*, TarsStructBase*>, void ***>::type dummy = 0)
    {
        if(NULL != p.get() && p->getType() == eJsonTypeObj)
        {
            JsonValueObjPtr pObj=JsonValueObjPtr::dynamicCast(p);
            v.readFromJson(pObj);
        }
        else if (isRequire)
        {
            char s[128];
            snprintf(s, sizeof(s), "read 'Char' type mismatch, get type: %d.", (p.get() ? p->getType() : 0));
            throw TC_Json_Exception(s);
        }
    }
};

class JsonOutput
{
public:
    static JsonValueBooleanPtr writeJson(Bool b)
    {
        JsonValueBooleanPtr p = new JsonValueBoolean();
        p->value=b;
        return p;
    }

    static JsonValueNumPtr writeJson(Char n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(UInt8 n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(Short n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(UInt16 n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(Int32 n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(UInt32 n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(Int64 n)
    {
        return (new JsonValueNum(static_cast<int64_t>(n),true));
    }

    static JsonValueNumPtr writeJson(Float n)
    {
        return (new JsonValueNum(n));
    }

    static JsonValueNumPtr writeJson(Double n)
    {
        return (new JsonValueNum(static_cast<double>(n)));
    }

    static JsonValueStringPtr writeJson(const std::string& s)
    {
        return (new JsonValueString(s));
    }

    static JsonValueStringPtr writeJson(const char *buf, const UInt32 len)
    {
        return (new JsonValueString(string(buf,len)));
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<string, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<string, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[i->first]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Bool, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Bool, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Char, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Char, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr((Int32)i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<UInt8, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<UInt8, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Short, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Short, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }
    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<UInt16, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<UInt16, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }
    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Int32, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Int32, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }
    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<UInt32, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<UInt32, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Int64, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Int64, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Float, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Float, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<Double, V, Cmp, Alloc>& m)
    {
        JsonValueObjPtr pObj=new JsonValueObj();
        typedef typename std::map<Double, V, Cmp, Alloc>::const_iterator IT;
        for (IT i = m.begin(); i != m.end(); ++i)
        {
            pObj->value[TC_Common::tostr(i->first)]=writeJson(i->second);
        }
        return pObj;
    }

    template<typename K, typename V, typename Cmp, typename Alloc>
    static JsonValueObjPtr writeJson(const std::map<K, V, Cmp, Alloc>& m)
    {
        char s[128];
        snprintf(s, sizeof(s), "map key is not Basic type. map key is only string|bool|num");
        throw TC_Json_Exception(s);
    }

    template<typename T, typename Alloc>
    static JsonValueArrayPtr writeJson(const std::vector<T, Alloc>& v)
    {
        JsonValueArrayPtr pArray=new JsonValueArray();
        typedef typename std::vector<T, Alloc>::const_iterator IT;
        for (IT i = v.begin(); i != v.end(); ++i)
            pArray->value.push_back(writeJson(*i));
        return pArray;
    }

    template<typename T>
    static JsonValueArrayPtr writeJson(const T *v, const UInt32 len)
    {
        JsonValueArrayPtr pArray=new JsonValueArray();
        for (size_t i = 0; i < len; ++i)
        {
            pArray->value.push_back(writeJson(v[i]));
        }
        return pArray;
    }

    template<typename T>
    static JsonValueNumPtr writeJson(const T& v, typename detail::disable_if<detail::is_convertible<T*, TarsStructBase*>, void ***>::type dummy = 0)
    {
        return writeJson((Int32) v);
    }

    template<typename T>
    static JsonValueObjPtr writeJson(const T& v, typename detail::enable_if<detail::is_convertible<T*, TarsStructBase*>, void ***>::type dummy = 0)
    {
        return v.writeToJson();
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}


#endif
