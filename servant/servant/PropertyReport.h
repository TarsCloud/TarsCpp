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

#ifndef __TARS_PROPERTY_REPORT_H_
#define __TARS_PROPERTY_REPORT_H_

 #include "util/tc_lock.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_spin_lock.h"
#include <tuple>
#include <vector>
#include <string>

using namespace std;

#undef max
#undef min

namespace tars
{

///////////////////////////////////////////////////////////////////////////////////
//
/**
 * 用户自定义属性统计基类
 */
class PropertyReport : public TC_HandleBase
{
public:
    /**
     * 设置该属性的服务名称，如果不设置则为当前服务
     */
    void setMasterName(const std::string & sMasterName) { _sMasterName = sMasterName; }
    
    /**
     * 获取该属性的服务名称
     */
    const std::string &getMasterName() const { return _sMasterName; }

public:

	class base
	{
	public:
		virtual ~base(){}
		virtual string get() = 0;
		virtual const string &desc() = 0;
		virtual void set(int o) = 0;
	};

	static const string Sum;
	static const string Distr;
	static const string Avg;
	static const string Min;
	static const string Max;
	static const string Count;

    /**
     * 求和
     */
    class sumProperty : public base
    {
    public:
		sumProperty() :_d(0)                { }
		virtual string get();
		virtual const string & desc()               { return Sum; }
		virtual void   set(int o)           { _d += o; }
    protected:
        void   clear()              { _d  = 0; }
    private:
        int   _d;
    };

    /**
     * 求平均
     */
    class avgProperty : public base
    {
    public:
        avgProperty():_sum(0), _count(0)    { }
		virtual const string & desc()               { return Avg; }
		virtual string get();
		virtual void   set(int o)           { _sum += o;++_count; }
    protected:
        void clear()                { _sum = 0; _count = 0; }
    private:
        int _sum;
        int _count;
    };

    /**
     * 分布
     */
    class distrProperty : public base
    {
    public:
        distrProperty(){};
        distrProperty(const vector<int>& range);
		virtual const string & desc()               { return Distr; }
		virtual void   set(int o);
		virtual string get();
    protected:
        void clear()                { _result.clear();}
    private:
        vector<int>     _range;
        vector<size_t>  _result;
    };

    /**
     * 求最大值
     */
    class maxProperty : public base
    {
    public:
        maxProperty() : _d(-9999999)        { }
		virtual const string & desc()               { return Max; }
		virtual string get();
		virtual void   set(int o)           { _d < o?_d = o:1; }
    protected:
		virtual void   clear()              { _d = 0; }
    private:
        int   _d;
    };

    /**
     * 求最小值
     */
    class minProperty : public base
    {
    public:
        minProperty():_d(0)                 { }
		virtual const string &desc()               { return Min; }
        virtual string get();
		virtual void   set(int o);
    protected:
        void   clear()              { _d = 0; }
    private:
        int   _d;
    };

    /**
     * 计数
     */
    class countProperty : public base
    {
    public:
        countProperty():_d(0)               { }
		virtual const string &desc()               { return Count; }
		virtual string get();
		virtual void   set(int o)           { _d++; }
    protected:
        void   clear()              { _d = 0; }
    private:
        int   _d;
    };

	static shared_ptr<base> sum() { return std::make_shared<sumProperty>(); }
	static shared_ptr<base> distr() { return std::make_shared<distrProperty>(); }
	static shared_ptr<base> avg() { return std::make_shared<avgProperty>(); }
	static shared_ptr<base> min() { return std::make_shared<minProperty>(); }
	static shared_ptr<base> max() { return std::make_shared<maxProperty>(); }
	static shared_ptr<base> count() { return std::make_shared<countProperty>(); }

public:

    virtual void report(int iValue)             = 0;
    virtual vector<pair<string, string> > get() = 0;

protected:
    std::string _sMasterName;   //属性所属服务名称
};

typedef TC_AutoPtr<PropertyReport> PropertyReportPtr;

///////////////////////////////////////////////////////////////////////////////////
//
/**
 * 具体的属性策略管理
 */

class PropertyReportImp : public PropertyReport, public TC_ThreadMutex
{
private:
	template<class Tuple, std::size_t N>
	struct TupleTrans
	{
		static void trans(const Tuple& t, vector<shared_ptr<PropertyReport::base>> &properties)
		{
			TupleTrans<Tuple, N-1>::trans(t, properties);

			properties.push_back(std::get<N-1>(t));
//			pp.template SetResult<N - 1>(iValue);
		}
	};

	template<class Tuple>
	struct TupleTrans<Tuple, 1>
	{
		static void trans(const Tuple& t, vector<shared_ptr<PropertyReport::base>> &properties)
		{
			properties.push_back(std::get<0>(t));
		}
	};

public:
//    using PropertyReportData = std::tuple<Params...>;
	PropertyReportImp()
	{
	}

	template <typename... Params>
    PropertyReportImp(Params&&... args)
    {
		TupleTrans<std::tuple<Params...>, sizeof...(args)>::trans(std::forward<Params>(args)..., _properties);
    }

//	template<>
//	PropertyReportImp(const vector<shared_ptr<PropertyReport::base>> &properies) : _properties(properies)
//	{
//	}

//	// do NOT copy
//    PropertyReportImp(const PropertyReportImp& ) = delete;
//    void operator = (const PropertyReportImp& ) = delete;
//
//    // but CAN move
//    PropertyReportImp(PropertyReportImp&& ) = default;
//    PropertyReportImp& operator= (PropertyReportImp&& ) = default;


    /**
    * 设置调用数据
    * @param iValue,值
    */
    void report(int iValue) override
    {
		for(auto &property : _properties)
		{
			property->set(iValue);
		}

//        TC_LockT<TC_ThreadMutex> lock(*this);
//        Helper<std::tuple_size<decltype(_propertyReportData)>::value>::Report(*this, iValue);
    }


    /**
     * 获取属性信息
     *
     * @return vector<pair<string, string>>
     */
    vector<pair<string, string> > get() override
    {
		std::vector<std::pair<std::string, std::string> > vs;
		for(auto &property : _properties)
		{
			vs.push_back(std::make_pair(property->desc(), property->get()));
		}
		return vs;

//        TC_LockT<TC_ThreadMutex> lock(*this);
//        return Helper<std::tuple_size<decltype(_propertyReportData)>::value>::Get(*this);
    }

protected:

	void setProperties(const vector<shared_ptr<PropertyReport::base>> &properties)
	{
		_properties = properties;
	}
	friend class StatReport;

private:


//	template<typename... Args, std::enable_if_t<sizeof...(Args) == 0, int> = 0>
//	void print(const std::tuple<Args...>& t)
//	{
//		std::cout << "()\n";
//	}
//
//	template<typename... Args, std::enable_if_t<sizeof...(Args) != 0, int> = 0>
//	void print(const std::tuple<Args...>& t)
//	{
//		std::cout << "(";
//		TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
//		std::cout << ")\n";
//	}
//
//    // report helper
//    template <int N, typename DUMMY = void>
//    struct Helper
//    {
//        static void Report(PropertyReportImp<Params...>& pp, int iValue)
//        {
//            static_assert(N >= 1, "Obviously success");
//            Helper<N - 1, DUMMY>::Report(pp, iValue);
//            pp.template SetResult<N - 1>(iValue);
//        }
//
//        static std::vector<std::pair<std::string, std::string>> Get(PropertyReportImp<Params...>& pp)
//        {
//            static_assert(N >= 1, "Obviously success");
//
//            std::vector<std::pair<std::string, std::string> > vs = Helper<N-1, DUMMY>::Get(pp);
//
//            vs.push_back({std::get<N - 1>(pp._propertyReportData).desc(), std::get<N-1>(pp._propertyReportData).get()});
//            return vs;
//        }
//    };
//
//    template <typename DUMMY>
//    struct Helper<0, DUMMY>
//    {
//        // base template
//        static void Report(PropertyReportImp<Params...>&, int  )
//        {
//        }
//
//        static
//        std::vector<std::pair<std::string, std::string>> Get(PropertyReportImp<Params...>& )
//        {
//            return std::vector<std::pair<std::string, std::string> >();
//        }
//    };
//
//    template <int I>
//    void SetResult(int iValue)
//    {
//        std::get<I>(_propertyReportData).set(iValue);
//    }
    /**
     * 状态报告数据
     */
//    PropertyReportData  _propertyReportData;

	vector<shared_ptr<PropertyReport::base>>	_properties;
};

}

#endif
