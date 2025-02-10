#pragma once
#include <map>
#include <string>
#include <iostream>
#include "servant/RemoteLogger.h"

namespace tars
{
template< typename Interface >
class TC_ObjectFactoryMgr
{
public:
    typedef Interface* (*CreateObjectFun)();
    typedef Interface* (*CreateObjectFunWithParam)(const string &param);

    // 注册类名以及创建的函数指针
    Interface* reg( const std::string& className, CreateObjectFun fun )
    {
        m_mapClass[className] = fun;
        return NULL;
    }

    Interface* regWithParam(const std::string& className, CreateObjectFunWithParam fun)
    {
        m_mapClassWithParam[className] = fun;
        return NULL;
    }
    // 通过类名创建对象，无参数
    Interface* createObject( const std::string& className )
    {
        typename std::map< std::string, CreateObjectFun >::iterator it = m_mapClass.find( className );
        if ( it != m_mapClass.end() )
        {
            return (it->second)();
        }
        else
        {
            LOG_ERROR << " TC_ObjectFactoryMgr::createObject className " << className << " empty! " << std::endl;
            return NULL;
        }
    }

    //一个参数string,需要的话，自己根据string格式化
    Interface* createObjectWithParam(const std::string& className,const string &param)
    {
        typename std::map< std::string, CreateObjectFunWithParam >::iterator it = m_mapClassWithParam.find(className);
        if (it != m_mapClassWithParam.end())
        {
            return (it->second)(param);
        }
        else
        {
            LOG_ERROR << " TC_ObjectFactoryMgr::createObject className " << className << " empty! " << std::endl;
            return NULL;
        }
    }

    static TC_ObjectFactoryMgr< Interface >& instance()
    {
        static TC_ObjectFactoryMgr< Interface > gObjectFactoryMgr;
        return gObjectFactoryMgr;
    }
private:

    TC_ObjectFactoryMgr()
    {
    }
public:
    std::map< std::string, CreateObjectFun > m_mapClass;
    std::map< std::string, CreateObjectFunWithParam > m_mapClassWithParam;
};

//构造函数不带参数的反射
#define CREATE_REFLECTION_OBJECT(BaseClass, name) TC_ObjectFactoryMgr<BaseClass>::instance().createObject((name))

#define DECLAR_REFLECTION_CLASS(BaseClass, ConcreteClass) \
	static BaseClass* ConcreteClass##createObj(){ return new ConcreteClass;}\
	static BaseClass* ConcreteClass##Tmp = TC_ObjectFactoryMgr<BaseClass>::instance().reg(#ConcreteClass, ConcreteClass##createObj);

//构造函数带一个字符串带参数的反射
#define CREATE_REFLECTION_OBJECT_WITH_PARAM(BaseClass, name,param) TC_ObjectFactoryMgr<BaseClass>::instance().createObjectWithParam((name) ,param)
#define DECLAR_REFLECTION_CLASS_WITH_PARAM(BaseClass, ConcreteClass) \
	static BaseClass* ConcreteClass##createObj(const string &param){ return new ConcreteClass(param);}\
	static BaseClass* ConcreteClass##Tmp = TC_ObjectFactoryMgr<BaseClass>::instance().regWithParam(#ConcreteClass, ConcreteClass##createObj);

}
