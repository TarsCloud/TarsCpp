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

#ifndef __TC_LOGGER_H
#define __TC_LOGGER_H

#include "util/tc_autoptr.h"
#include "util/tc_common.h"
#include "util/tc_ex.h"
#include "util/tc_file.h"
#include "util/tc_thread_queue.h"
#include "util/tc_cas_queue.h"
#include "util/tc_timeprovider.h"
#include "util/tc_spin_lock.h"
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <list>
#include <sstream>
#include <streambuf>
#include <string>
#include <time.h>
#include <vector>
#include <set>
#include <thread>
#include <stdlib.h>

#if TARGET_PLATFORM_LINUX||TARGET_PLATFORM_IOS
#include <sys/syscall.h>
#endif

using namespace std;
namespace tars
{
	/////////////////////////////////////////////////
	/**
	 * @file tc_logger.h
	 * @brief 日志类.
	 * @brief Log Class.
	 *
	 * 说明:可扩展的日志类,可以自定义滚动策略和写方法.
	 * Description: An extensible log class that allows you to customize scrolling policies and writing methods.
	 *
	 *  提供了两种方式的滚动方式:
	 *  There are two ways to scroll:
	 *
	 *  1 按文件大小,文件数目滚动;
	 *  1 Scroll by file size and number of files;
	 *
	 *  2 按时间滚动(按天/小时/分钟等)
	 *  2 Scroll by time (by day/hour/minute, etc.)
	 *
	 *   上述两种默认的策略中,都是默认写入文件的,当没有初始化,则默认写入到cout
	 *   Of the two default strategies described above, both are written to files by default, and when not initialized, to cout by default
	 *
	 *  自定义滚动方式的步骤如下:
	 *  The steps to customize scrolling are as follows:
	 *
	 *  1 实现自定义模板类,继承于TC_LoggerRoll,实现roll函数,该函数实现写逻辑,
	 *  1 Implement a custom template class that inherits from TC_LoggerRoll, which implements the roll function, which implements write logic
	 *
	 *  例如:template<typename WriteT>
	 *  for example: template<typename WriteT>
	 *
	 *  class MyRoll : public TC_LoggerRoll, public TC_ThreadMutex
	 *
	 *  2 模板中的WriteT是具体的写入逻辑,提供缺省的:TC_DefaultWriteT
	 *  2 WriteT in the template is the specific write logic that provides the default: TC_DefaultWriteT
	 *
	 *  3 可以自定义实现WriteT, 例如:实现发送到网络的逻辑.
	 *  3 You can customize the implementation of WriteT, for example, to implement logic sent to the network.
	 *
	 *  4 在MyRoll中实现内部类RollWrapperI, 并继承RollWrapperBase<MyRoll<WriteT> >
	 *  4 Implement the internal class RollWrapperI in MyRoll and inherit RollWrapperBase<MyRoll<WriteT>>
	 *
	 *  5 RollWrapperI主要就是提供MyRoll的初始化接口, 这些接口最后会直接反应到TC_Logger中.
	 *  5 RollWrapperI mainly provides MyRoll's initialization interfaces, which will eventually be reflected directly in TC_Logger.
	 *
	 *  6 RollWrapperI构造时, 构造MyRoll对象(采用智能指针),并且接口的操作直接转发到MyRoll对象中.
	 *  6 When RollWrapperI is constructed, MyRoll objects are constructed (using smart pointers), and the operations of the interface are forwarded directly to the MyRoll objects.
	 *
	 *  实现这些逻辑后, 采用模板方式定义出类型:
	 *  Once these logic are implemented, the types are defined as templates:
	 *
	 *  typedef TC_Logger<MyWriteT, MyRoll> MyLogger;
	 *
	 * @author : jarodruan@tencent.com,devinchen@tencent.com,skingfan@tencent.com
	 */
	template<class T>
	inline std::ostream& operator<<(std::ostream& oss, const std::vector<T>& v)
	{
		bool firstFlag = true;
		oss << "[ ";
		std::for_each(begin(v), end(v), [&oss, &firstFlag](const T& t){
			if (firstFlag) firstFlag = false;
			else
				oss << ", ";
			oss << t.writeToJsonString();
		});

		oss << "]";

		return oss;
	}
	/**
	* @brief 日志异常类
	* @brief Log Exception Class
	*/
	struct TC_Logger_Exception : public TC_Exception
	{
		TC_Logger_Exception(const string &buffer) : TC_Exception(buffer){};
		TC_Logger_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
		~TC_Logger_Exception() throw(){};
	};

	/**
	 * @brief 写操作
	 * @brief Write operation
	 */
	class TC_DefaultWriteT
	{
	public:
		void operator()(ostream &of, const deque<pair<size_t, string> > &ds)
		{
			deque<pair<size_t, string> >::const_iterator it = ds.begin();
			while (it != ds.end())
			{
				of << it->second;
				++it;
			}
			of.flush();
		}
	};

	class TC_LoggerThreadGroup;

	//////////////////////////////////////////////////////////////////////////////
	/**
	 * @brief 具体写日志基类
	 * @brief Specific Write Log Base Class
	 */

	class UTIL_DLL_API TC_LoggerRoll : public TC_HandleBase
	{
	public:
		/**
		 * @brief 构造函数
		 * @brief Constructor
		 */
		TC_LoggerRoll() : _pThreadGroup(NULL)
		{
		}

		/**
		 * @brief 实时记日志, 并且滚动.
		 * @brief Keep a real-time log and scroll.
		 *
		 * 不同的策略日志类,实现不同的逻辑
		 * Different policy log classes, implementing different logic
		 * @param buffer
		 */
		virtual void roll(const deque<pair<size_t, string> > &ds) = 0;

		/**
		 * @brief 安装线程.
		 * @brief Install Threads.
		 *
		 * @param ltg
		 */
		void setupThread(TC_LoggerThreadGroup *pThreadGroup);

		/**
		 * @brief 取消线程.
		 * @brief Cancel Thread.
		 *
		 */
		void unSetupThread();

		/**
		 * @brief 写到日志.
		 * @brief Write to Log
		 *
		 * @param 日志内容
		 */
		void write(const pair<size_t, string> &buffer);

		/**
		 * @brief 刷新缓存到文件
		 * @brief Refresh Cache to File
		 */
		void flush();

		/**
		 * @brief 设置染色是否生效.
		 * @brief Set whether the dye works.
		 *
		 * @param bEnable
		 */
		void enableDyeing(bool bEnable, const string &sKey)
		{
			TC_LockT<TC_SpinLock> lock(_mutexDyeing);

			if (bEnable)
			{
				_mapThreadID.insert(std::make_pair(TC_Thread::CURRENT_THREADID(), sKey));
				//_setThreadID.insert(pthread_self());
			}
			else
			{
				_mapThreadID.erase(TC_Thread::CURRENT_THREADID());
				//_setThreadID.erase(pthread_self());
			}

			_bDyeingFlag = !_mapThreadID.empty();
		}

	protected:
		/**
		 * buffer
		 */
		TC_CasQueue<pair<std::size_t, string> > _buffer;

		/**
		 * 锁
		 * Lock
		 */
		std::mutex _mutex;

		/**
		 * 线程组
		 * Thread Group
		 */
		TC_LoggerThreadGroup *_pThreadGroup;

		/**
		 * 是否已经染色的标志
		 * A mark that has been stained or not
		 */
		static bool _bDyeingFlag;

		/**
		 * 染色用锁
		 * Dyeing lock
		 *
		 */
//		static std::mutex _mutexDyeing;
		static TC_SpinLock _mutexDyeing;

		/**
		 * 染色的线程ID集合
		 * Stained thread ID collection
		 *
		 */
		static unordered_map<size_t, string> _mapThreadID;
	};

	typedef TC_AutoPtr<TC_LoggerRoll> TC_LoggerRollPtr;

	//////////////////////////////////////////////////////////////////////////////
	//
	/**
	 * @brief 写日志线程组.
	 * @brief Write Log Thread Group
	 *
	 * 关键点:注册日志后,会保存职能指针,保证日志对象一直存在
	 * Key point: After registering the log, the function pointer is saved to ensure that the log object always exists
	 */
	class TC_LoggerThreadGroup
	{
	public:
		/**
		 * @brief 构造函数
		 * @brief Constructor
		 */
		TC_LoggerThreadGroup();

		/**
		 * @brief 析够函数
		 * @brief Destructor
		 */
		~TC_LoggerThreadGroup();

		/**
		 * @brief 启动线程.
		 * @brief Start Thread
		 *
		 * @param iThreadNum线程数
		 */
		void start(size_t iThreadNum);

		/**
		 * @brief 注册logger对象.
		 * @brief Register logger object.
		 *
		 * @param l TC_LoggerRollPtr对象
		 * @param l TC_LoggerRollPtr Object
		 */
		void registerLogger(TC_LoggerRollPtr &l);

		/**
		 * @brief 卸载logger对象.
		 * @brief Unload logger object.
		 *
		 * @param l TC_LoggerRollPtr对象
		 * @param l TC_LoggerRollPtr Object
		 */
		void unRegisterLogger(TC_LoggerRollPtr &l);

		/**
		 * @brief 刷新所有的数据
		 * @brief Refresh all data
		 */
		void flush();

        void terminate();

	protected:
		/**
		 * @brief 写日志
		 * @brief Write a log
		 */
		void run();

		/**
		 * @brief 指针比较
		 * @brief Pointer comparison
		 */
		struct KeyComp
		{
			bool operator()(const TC_LoggerRollPtr &p1, const TC_LoggerRollPtr &p2) const
			{
				return p1.get() < p2.get();
			}
		};

		typedef set<TC_LoggerRollPtr, KeyComp> logger_set;

	protected:
		/**
		 * 结束
		 * End
		 */
		bool _bTerminate;

		//锁
		//Lock
		std::mutex _mutex;

		//条件变量
		//Conditional variable
		std::condition_variable _cond;

		/**
		 * logger对象
		 * logger object
		 */
		logger_set _logger;

		//线程
		//Thread
		std::thread *_thread;
	};

	/**
	 * @brief 自定义logger buffer
	 * @brief Custom logger buffer
	 */
	class LoggerBuffer : public std::basic_streambuf<char>
	{
	public:
		/**
		 * @brief 定义最大的buffer大小(10M)
		 * @brief Define the maximum buffer size (10M)
		 */
		enum
		{
			MAX_BUFFER_LENGTH = 1024 * 1024 * 10,
		};

		/**
		 * @brief 构造函数
		 * @brief Constructor
		 */
		LoggerBuffer();

		/**
		 * @brief 构造函数.
		 * @brief Constructor.
		 *
		 * @param roll        TC_LoggerRollPtr对象
		 * @param roll        TC_LoggerRollPtr object
		 * @param buffer_len  buffer大小
		 * @param buffer_len  buffer size
		 */
		LoggerBuffer(TC_LoggerRollPtr roll, size_t buffer_len);

		/**
		 * @brief 析构函数
		 * @brief Destructor
		 */
		~LoggerBuffer();

	protected:
		/**
		 * @brief 分配空间.
		 * @brief Allocate space.
		 *
		 * @param n
		 */
		void reserve(std::streamsize n);

		/**
		 * @brief 放数据.
		 * @brief Place data.
		 *
		 * @param s
		 * @param n
		 *
		 * @return streamsize
		 */
		virtual streamsize xsputn(const char_type *s, streamsize n);

		/**
		 * @brief buffer满了, 具体写数据.
		 * @brief Buffer is full, and data should be written specifically.
		 *
		 * @param c
		 * @return int_type
		 */
		virtual int_type overflow(int_type c);

		/**
		 * @brief 读空间empty了(不实现).
		 * @brief Empty read space (not implemented)
		 *
		 * @return int_type
		 */
		virtual int_type underflow() { return std::char_traits<char_type>::eof(); }

		/**
		 * @brief 具体写逻辑.
		 * @brief Write logic concretely.
		 *
		 * @return int
		 */
		virtual int sync();

	protected:
		LoggerBuffer(const LoggerBuffer &);
		LoggerBuffer &operator=(const LoggerBuffer &);

	protected:
		/**
		 * @brief 写日志
		 * @brief Write a journal
		 */
		TC_LoggerRollPtr _roll;

		/**
		 * 缓冲区
		 * buffer
		 */
		char *_buffer;

		/**
		 * 缓冲区大小
		 * buffer size
		 */
		std::streamsize _buffer_len;
	};

	/**
	 * @brief 临时类, 析够的时候写日志
	 * @brief Temporary class, log when enough analysis
	 */
	class LoggerStream
	{
	public:
		/**
		 * @brief 构造.
		 * @brief Constructor.
		 *
		 * @param stream
		 * @param mutex
		 */
		LoggerStream(const char *header, ostream *stream, ostream *estream, TC_SpinLock &mutex) : _stream(stream), _estream(estream), _mutex(mutex)
		{
			if (stream)
			{
				_buffer << header;
			}
		}

		/**
		 * @brief 析构
		 * @brief Destructor
		 */
		~LoggerStream()
		{
			if (_stream)
			{
				TC_LockT<TC_SpinLock> lock(_mutex);
				_stream->clear();
				(*_stream) << _buffer.str();
				_stream->flush();
			}
		}

		/**
		* @brief 重载<<
		* @brief Reload<<
		*/
		template <typename P>
		LoggerStream &operator<<(const P &t)
		{
			if (_stream)
			{
				_buffer << t;
				// *_stream << t;
			}
			return *this;
		}

		/**
		 * @brief endl,flush等函数
		 * @brief Endl, flush and other functions
		 */
		typedef ostream &(*F)(ostream &os);
		LoggerStream &operator<<(F f)
		{
			if (_stream)
			{
				(f)(_buffer);
			}
			return *this;
		}

		/**
		 * @brief  hex等系列函数
		 * @brief  Series of functions such as hex
		 */
		typedef ios_base &(*I)(ios_base &os);
		LoggerStream &operator<<(I f)
		{
			if (_stream)
			{
				(f)(_buffer);
			}
			return *this;
		}

		/**
		 * @brief 字段转换成ostream类型.
		 * @brief Field is converted to ostream type.
		 *
		 * @return ostream&
		 */
		operator ostream &()
		{
			if (_stream)
			{
				return _buffer;
			}

			return *_estream;
		}

		//不实现
		//Not implemented
		LoggerStream(const LoggerStream &lt);
		LoggerStream &operator=(const LoggerStream &lt);

	protected:
		/**
		* 缓冲区
		* Buffer
		*/
		std::stringstream _buffer;

		/**
		 * 输出流
		 * Output stream
		 */
		std::ostream *_stream;

		/**
		 *
		 */
		std::ostream *_estream;

		/**
		 * 锁
		 * Lock
		 */
		TC_SpinLock &_mutex;
	};

	/**
	 * @brief 日志基类
	 * @brief Log Base Class
	 */
	template <typename WriteT, template <class> class RollPolicy>
	class TC_Logger : public RollPolicy<WriteT>::RollWrapperI
	{
	public:
		/**
		 * @brief 设置显示标题
		 * @brief Set Display Title
		 */
		enum
		{
			//秒级别时间
			//Seconds level time
			HAS_TIME = 0x01, 
			//进程ID
			//Process ID
			HAS_PID = 0x02, 
			//日志等级
			//Log Level
			HAS_LEVEL = 0x04, 
			//毫秒级别时间(不建议使用, 会影响性能)
			//Millisecond level time (not recommended, performance will be affected)
			HAS_MTIME = 0x08,
		};

		/**
		* @brief 枚举类型,定义日志的四种等级 . 此处级别被修改了，与taf标准不一样
		* @brief Enumeration type, defines four levels of logs. This level has been modified and is different from the TAF standard
		*/
		enum
		{
			//所有的log都不写
			//All logs are not written
			NONE_LOG_LEVEL = 1, 
			//写错误log
			//Writing-error log
			ERROR_LOG_LEVEL = 2,
			//写错误,警告log
			//Write error, warning log
			WARN_LOG_LEVEL = 3, 
			//写错误,警告,调试,Info log
			//Write errors, warnings, debugging, Info log
			INFO_LOG_LEVEL = 4,			
			//写错误,警告,调试log
			//Write error, warning, debug log
			DEBUG_LOG_LEVEL = 5,
			//给TARS框架打日志用
			//Logging TARS Framework
			TARS_LOG_LEVEL = 6, 
		};

		/**
		 * @brief 日志级别名称
		 * @brief Log Level Name
		 */
		static const string LN[];

		/**
		 * @brief 构造函数
		 * @brief Constructor
		 */
		TC_Logger()
			: _flag(HAS_TIME), _level(DEBUG_LOG_LEVEL), _buffer(TC_LoggerRollPtr::dynamicCast(this->_roll), 1024), _stream(&_buffer), _ebuffer(NULL, 0), _estream(&_ebuffer), _sSepar("|"), _bHasSquareBracket(false)
		{
		}

		/**
		 * @brief 析够函数
		 * @brief Disjunctive Function
		 */
		~TC_Logger()
		{
		}

		/**
		 * @brief 修改标题.
		 * @brief Modify Title
		 *
		 * @param flag
		 * @param add
		 */
		void modFlag(int flag, bool add = true)
		{
			if (add)
			{
				_flag |= flag;
			}
			else
			{
				_flag &= ~flag;
			}
		}

		/**
		 * @brief 是否有某标示.
		 * @brief Is there an indication.
		 *
		 * @return bool
		 */
		bool hasFlag(int flag) const { return (_flag & flag) != 0; }

		/**
		 * @brief 获取flag
		 * @brief Get flag
		 * @return flag的值
		 * @return Value of flag
		 */
		int getFlag() const { return _flag; }

		/**
		* @brief 获取日志等级.
		* @brief Get Log Level.
		*
		* @return int 等级
		* @return int level
		*/
		int getLogLevel() const { return _level; }

		/**
		* @brief 设置日志等级.
		* @brief Set log level.
		*
		* @param level 等级
		* @param level level
		* @return      成功设置返回0，否则返回-1
		* @return      Successfully set to return 0, otherwise return -1
		*/
		int setLogLevel(int level)
		{
			if (!isLogLevelValid(level))
			{
				return -1;
			}

			_level = level;
			return 0;
		}

		/**
		 * @brief 设置等级.
		 * @brief Set level.
		 *
		 * @param level
		 * @param int
		 */
		int setLogLevel(const string &level)
		{
			if (level == "ERROR")
			{
				return setLogLevel(ERROR_LOG_LEVEL);
			}
			else if (level == "WARN")
			{
				return setLogLevel(WARN_LOG_LEVEL);
			}
			else if (level == "DEBUG")
			{
				return setLogLevel(DEBUG_LOG_LEVEL);
			}
			else if (level == "NONE")
			{
				return setLogLevel(NONE_LOG_LEVEL);
			}
			else if (level == "INFO")
			{
				return setLogLevel(INFO_LOG_LEVEL);
			}
			else if (level == "TARS")
			{
				return setLogLevel(TARS_LOG_LEVEL);
			}	
			return -1;
		}

		bool isNeedLog(const string &level)
		{
			if (level == "ERROR")
			{
				return isNeedLog(ERROR_LOG_LEVEL);
			}
			else if (level == "WARN")
			{
				return isNeedLog(WARN_LOG_LEVEL);
			}
			else if (level == "DEBUG")
			{
				return isNeedLog(DEBUG_LOG_LEVEL);
			}
			else if (level == "NONE")
			{
				return isNeedLog(NONE_LOG_LEVEL);
			}
			else if (level == "INFO")
			{
				return isNeedLog(INFO_LOG_LEVEL);
			}
			else if (level == "TARS")
			{
				return isNeedLog(TARS_LOG_LEVEL);
			}			
			else
			{
				return true;
			}
		}
		bool isNeedLog(int level)
		{
			return level <= _level;
		}
		/**
		 * @brief 框架中增加的日志内容之间的分割符，默认是"|"
		 * @brief Separator between added log contents in the framework, default is'|'
		 * @param str
		 */
		void setSeparator(const string &str) { _sSepar = str; }

		/**
		 * @brief 框架中日期和时间之间是否需要加中括号[],有些统计有特殊需求；默认不加
		 * @brief Whether brackets [] are required between dates and times in the framework, some statistics have special requirements; no brackets are added by default
		 * @param bEnable
		 */
		void enableSqareWrapper(bool bEnable) { _bHasSquareBracket = bEnable; }

		/**
		* @brief TARS记日志
		* @brief TARS Log
		*/
		LoggerStream tars() { return stream(TARS_LOG_LEVEL); }

		/**
		* @brief DEBUG记日志
		* @brief DEBUG Log
		*/
		LoggerStream info() { return stream(INFO_LOG_LEVEL); }

		/**
		* @brief DEBUG记日志
		* @brief TARS Log
		*/
		LoggerStream debug() { return stream(DEBUG_LOG_LEVEL); }

		/**
		* @brief WARNING记日志
		* @brief WARNING Log
		*/
		LoggerStream warn() { return stream(WARN_LOG_LEVEL); }

		/**
		* @brief ERROR记日志
		* @brief ERROR Log
		*/
		LoggerStream error() { return stream(ERROR_LOG_LEVEL); }

		/**
		* @brief 记所有日志, 与等级无关
		* @brief Keep all logs, regardless of rank
		*/
		LoggerStream any() { return stream(0); }

		LoggerStream log(int level) { return stream(level); }
	protected:
		/**
		 * @brief 获取头部信息.
		 * @brief Get header information.
		 *
		 * @param c
		 * @param len
		 * @param level
		 */
		void head(char *c, int len, int level)
		{
			size_t n = 0;

			if (hasFlag(TC_Logger::HAS_MTIME))
			{
				// auto time_now = chrono::system_clock::now();
				time_t t = TNOW;//std::chrono::system_clock::to_time_t(time_now);

				auto duration_in_ms = TNOWMS; //chrono::duration_cast<chrono::milliseconds>(time_now.time_since_epoch());

				tm tt;

				TC_Port::localtime_r(&t, &tt);

				const char *szFormat = (_bHasSquareBracket) ? ("[%04d-%02d-%02d %02d:%02d:%02d.%03ld]%s") : ("%04d-%02d-%02d %02d:%02d:%02d.%03ld%s");
				n += snprintf(c + n, len - n, szFormat,
					tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, duration_in_ms % 1000, _sSepar.c_str());
			}
			else if (hasFlag(TC_Logger::HAS_TIME))
			{
				time_t t = TNOW;//std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

				tm tt;
				TC_Port::localtime_r(&t, &tt);
// #if TARGET_PLATFORM_LINUX||TARGET_PLATFORM_IOS
// 				localtime_r(&t, &tt);
// #else
// 				localtime_s(&tt, &t);
// #endif
				const char *szFormat = (_bHasSquareBracket) ? ("[%04d-%02d-%02d %02d:%02d:%02d]%s") : ("%04d-%02d-%02d %02d:%02d:%02d%s");
				n += snprintf(c + n, len - n, szFormat,
					tt.tm_year + 1900, tt.tm_mon + 1, tt.tm_mday, tt.tm_hour, tt.tm_min, tt.tm_sec, _sSepar.c_str());
			}

			if (hasFlag(TC_Logger::HAS_PID))
			{
				n += snprintf(c + n, len - n, "%zd%s", TC_Thread::CURRENT_THREADID(), _sSepar.c_str());               
			}

			if (hasFlag(TC_Logger::HAS_LEVEL))
			{
				n += snprintf(c + n, len - n, "%s%s", TC_Logger::LN[level].c_str(), _sSepar.c_str());
			}
		}

		LoggerStream stream(int level)
		{
			ostream *ost = NULL;

			if (level <= _level)
			{
				char c[128] = "\0";
				head(c, sizeof(c)-1, level);

				ost = &_stream;

				return LoggerStream(c, ost, &_estream, _spinMutex);
			}

			return LoggerStream(NULL, ost, &_estream, _spinMutex);
		}

		/**
		* @brief 进程等级是否有效.
		* @brief Is the process level valid.
		*
		* @param level : 进程等级
		* @param level : process level
		* @return bool,等级是否有效
		* @return bool, Is the rating valid
		*/
		bool isLogLevelValid(int level)
		{
			switch (level)
			{
			case NONE_LOG_LEVEL:
			case ERROR_LOG_LEVEL:
			case WARN_LOG_LEVEL:
			case DEBUG_LOG_LEVEL:
			case INFO_LOG_LEVEL:
			case TARS_LOG_LEVEL:
				return true;
				break;
			}

			return false;
		}

	protected:
		/**
		 * 显示头
		 * Display Head
		 */
		int _flag;

		/**
		* 日志最高等级
		* Log Highest Level
		*/
		int _level;

		/**
		 * buffer
		 */
		LoggerBuffer _buffer;

		/**
		 * logger临时流
		 * Logger temporary stream
		 */
		std::ostream _stream;

		/**
		 * 空buffer
		 * empty buffer
		 */
		LoggerBuffer _ebuffer;

		/**
		 * 空流
		 * empty flow
		 */
		std::ostream _estream;

		/**
		 * 锁
		 * Lock
		 */
//		std::mutex _mutex;
		TC_SpinLock _spinMutex;

		/**
		 * 分隔符
		 * Separator
		 */
		string _sSepar;
		/**
		 * 日期部分是否加上[]
		 * Is [] added to the date part
		 */
		bool _bHasSquareBracket;
	};

	template <typename WriteT, template <class> class RollPolicy>
	//const string TC_Logger<WriteT, RollPolicy>::LN[6] = { "ANY", "NONE_LOG", "ERROR", "WARN", "DEBUG", "INFO" };
    const string TC_Logger<WriteT, RollPolicy>::LN[] = { "ANY", "NONE_LOG", "ERROR", "WARN", "INFO", "DEBUG", "TARS" };
	////////////////////////////////////////////////////////////////////////////////

	class RollWrapperInterface
	{
	public:
		virtual ~RollWrapperInterface() {}

		/**
		* @brief TARS记日志
		* @brief TARS Log
		*/
		virtual LoggerStream tars() = 0;

		/**
		* @brief INFO记日志
		* @brief INFO Log
		*/
		virtual LoggerStream info() = 0;

		/**
		* @brief DEBUG记日志
		* @brief DEBUG Log
		*/
		virtual LoggerStream debug() = 0;

		/**
		* @brief WARNING记日志
		* @brief WARNING Log
		*/
		virtual LoggerStream warn() = 0;

		/**
		* @brief ERROR记日志
		* @brief ERROR Log
		*/
		virtual LoggerStream error() = 0;

		/**
		* @brief 记所有日志, 与等级无关
		* @brief Keep all logs, regardless of rank
		*/
		virtual LoggerStream any() = 0;

		/**
		 *@brief 按照等级来输出日志
		 *@brief Output log by level
		 */
		virtual LoggerStream log(int level) = 0;
		/**
		 * @brief 如果是异步调用，则马上进行刷新
		 * @brief If it is an asynchronous call, refresh immediately.
		 */
		virtual void flush() = 0;
	};

	template <typename RollPolicyWriteT>
	class RollWrapperBase : public RollWrapperInterface
	{
	public:
		typedef TC_AutoPtr<RollPolicyWriteT> RollPolicyWriteTPtr;

		/**
		 * @brief 构造
		 * @brief Constructor
		 */
		RollWrapperBase()
		{
			_roll = new RollPolicyWriteT;
		}

		/**
		 * @brief 安装线程.
		 * @brief Install Threads
		 *
		 * @param ltg
		 */
		void setupThread(TC_LoggerThreadGroup *ltg) { _roll->setupThread(ltg); }

		/**
		 * @brief 取消线程
		 * @brief Cancel Thread
		 */
		void unSetupThread() { _roll->unSetupThread(); }

		/**
		 * @brief 获取写对象.
		 * @brief Get Write Object
		 *
		 * @return WriteT&
		 */
		typename RollPolicyWriteT::T &getWriteT() { return _roll->getWriteT(); }

		/**
		 * @brief 写日志.
		 * @brief Write Log
		 *
		 * @param buffer
		 */
		void roll(const pair<size_t, string> &buffer) { _roll->write(buffer); }

		/**
		* @brief 获取roll实例.
		* @brief Get roll instance
		*
		* @return RollPolicyWriteTPtr&
		*/
		RollPolicyWriteTPtr &getRoll() { return _roll; }

		/**
		 * @brief 异步刷新
		 * @brief Asynchronous refresh
		 */
		void flush() { _roll->flush(); }

	protected:
		/**
		 * @brief 具体写日志操作类
		 * @brief Specific Write Log Action Class
		 */
		RollPolicyWriteTPtr _roll;
	};

	////////////////////////////////////////////////////////////////////////////////
	/**
	 * @brief 日志滚动方法, 根据日志大小滚动
	 * @brief Log scrolling method, scrolling according to log size.
	 */
	template <typename WriteT>
	class TC_RollBySize : public TC_LoggerRoll, public std::mutex
	{
	public:
		typedef WriteT T;

		/**
		 * @brief 封装类(提供接口)
		 * @brief Encapsulation classes (providing interfaces)
		 */
		class RollWrapperI : public RollWrapperBase<TC_RollBySize<WriteT> >
		{
		public:
			/**
			 * @brief 初始化.
			 * @brief Initialize
			 *
			 * @param path
			 * @param iMaxSize, 字节
			 * @param iMaxSize, byte
			 * @param iMaxNum
			 */
			void init(const string &path, int iMaxSize = 5000000, int iMaxNum = 10)
			{
				this->_roll->init(path, iMaxSize, iMaxNum);
			}

			/**
			 * @brief 获取日志路径.
			 * @brief Get Log Path
			 *
			 * @return string
			 */
			string getPath() { return this->_roll->getPath(); }

			/**
			 * @brief 设置文件路径
			 * @brief Set File Path
			 */
			void setPath(const string &path) { this->_roll->setPath(path); }

			/**
			 * @brief 获取最大大小.
			 * @brief Get maximum size
			 *
			 * @return int
			 */
			int getMaxSize() { return this->_roll->getMaxSize(); }

			/**
			 * @brief 设置最大大小.
			 * @brief Set maximum size
			 *
			 * @param maxSize
			 */
			void setMaxSize(int maxSize) { this->_roll->setMaxSize(maxSize); }

			/**
			 * @brief 获取最大个数.
			 * @brief Get the maximum number
			 *
			 * @return int
			 */
			int getMaxNum() { return this->_roll->getMaxNum(); }

			/**
			 * @brief 设置文件个数.
			 * @brief Set the number of files.
			 *
			 * @param maxNum
			 */
			void setMaxNum(int maxNum) { this->_roll->setMaxNum(maxNum); }
		};

		/**
		 * @brief 构造函数
		 * @brief Constructor
		 */
		TC_RollBySize() : _maxSize(5000000), _maxNum(10), _iUpdateCount(0), _lt(time(NULL))
		{
		}

		/**
		 * @brief 析够
		 * @brief Destructor
		 */
		~TC_RollBySize()
		{
			if (_of.is_open())
			{
				_of.close();
			}
		}

		/**
		 * @brief 初始化.
		 * @brief Initialize
		 *
		 * @param path, 文件名
		 * @param path, file name
		 * @param iMaxSize, 最大大小
		 * @param iMaxSize, the maximun size
		 * @param iMaxNum, 最大个数
		 * @param iMaxNum. maximum number
		 */
		void init(const string &path, int iMaxSize = 5000000, int iMaxNum = 10)
		{
			std::lock_guard<std::mutex> lock(*this);

			_path = path;
			_maxSize = iMaxSize;
			_maxNum = iMaxNum;
		}

		/**
		 * @brief 获取日志路径.
		 * @brief Get log path.
		 *
		 * @return string
		 */
		string getPath()
		{
			std::lock_guard<std::mutex> lock(*this);
			return _path;
		}

		/**
		 * @brief 设置路径.
		 * @brief Set log path.
		 */
		void setPath(const string &path)
		{
			std::lock_guard<std::mutex> lock(*this);
			_path = path;
		}

		/**
		 * @brief 获取最大大小.
		 * @brief Get maximum size.
		 *
		 * @return int
		 */
		int getMaxSize()
		{
			std::lock_guard<std::mutex> lock(*this);
			return _maxSize;
		}

		/**
		 * @brief 设置最大大小.
		 * @brief Set maximum size.
		 *
		 * @param maxSize
		 * @return void
		 */
		void setMaxSize(int maxSize)
		{
			std::lock_guard<std::mutex> lock(*this);
			_maxSize = maxSize;
		}

		/**
		 * @brief 获取最大个数.
		 * @brief Get the maximum number.
		 *
		 * @return int
		 */
		int getMaxNum()
		{
			std::lock_guard<std::mutex> lock(*this);
			return _maxNum;
		}

		/**
		 * @brief 设置最大个数.
		 * @brief Set maximum number.
		 *
		 * @param maxNum
		 */
		void setMaxNum(int maxNum)
		{
			std::lock_guard<std::mutex> lock(*this);
			_maxNum = maxNum;
		}

		/**
		 * @brief 获取写示例.
		 * @brief Get Write Samples.
		 *
		 * @return T&
		 */
		WriteT &getWriteT() { return _t; }

		/**
		 * 函数对象
		 * Function object
		 *
		 * @param string
		 */
		void roll(const deque<pair<size_t, string> > &buffer)
		{
			std::lock_guard<std::mutex> lock(*this);

			if (_path.empty())
			{
				_t(cout, buffer);
				return;
			}

			time_t t = TNOW;
			time_t tt = t - _lt;
			//每隔5, 重新打开一次文件, 避免文件被删除后句柄不释放
			//Every 5, reopen the file to prevent the handle from being released after the file is deleted.
			if (tt > 5 || tt < 0)
			{
				_lt = t;
				_of.close();
			}

			//检查日志文件是否打开
			//Check that the log file is open
			if (!_of.is_open())
			{
				string sLogFileName = _path + ".log";

				_of.open(sLogFileName.c_str(), ios::app | ios::out);

				string sLogFilePath = TC_File::extractFilePath(_path);

				if (!TC_File::isFileExist(sLogFilePath))
				{
					TC_File::makeDirRecursive(sLogFilePath);
				}

				if (!_of.is_open())
				{
					// cout << "write roll4:" << sLogFileName << ", " << strerror(errno)<< endl;

					//抛异常前继续进入_t 以便打远程日志
					//Continue to _t for remote logging before throwing an exception
					_t(_of, buffer);
					THROW_EXCEPTION_SYSCODE(TC_Logger_Exception, "[TC_RollBySize::roll]:fopen fail: " + sLogFileName);
					// throw TC_Logger_Exception("[TC_RollBySize::roll]:fopen fail: " + sLogFileName, TC_Exception::getSystemCode());
				}
			}

			_t(_of, buffer);

			if (tt <= 5)
			{
				return;
			}

			//文件大小小于限制, 直接返回
			//File size less than limit, return directly
			if (_of.tellp() < _maxSize)
			{
				return;
			}

			//文件大小超出限制,删除最后一个文件
			//File size exceeds limit, delete last file
			string sLogFileName = _path + TC_Common::tostr(_maxNum - 1) + ".log";
			{
				if (TC_File::isFileExist(sLogFileName) && TC_File::removeFile(sLogFileName, true))
				{
					return;					
				}
			}

			//需要先关闭当前文件，再进行rename操作
			_of.close();

			//将log文件命名shift, xxx1.log=>xxx2.log,第一文件还是为xxx.log
			//Name the log file shift, xxx1.log=>xxx2.log, or the first file is xxx.log
			for (int i = _maxNum - 2; i >= 0; i--)
			{
				if (i == 0)
				{
					sLogFileName = _path + ".log";
				}
				else
				{
					sLogFileName = _path + TC_Common::tostr(i) + ".log";
				}

				if (TC_File::isFileExist(sLogFileName))
				{
					string sNewLogFileName = _path + TC_Common::tostr(i + 1) + ".log";
					rename(sLogFileName.c_str(), sNewLogFileName.c_str());
				}
			}

			_of.open(sLogFileName.c_str(), ios::app);
			if (!_of)
			{
				THROW_EXCEPTION_SYSCODE(TC_Logger_Exception, "[TC_RollBySize::roll]:fopen fail: " + sLogFileName);
				// throw TC_Logger_Exception("[TC_RollBySize::roll]:fopen fail: " + sLogFileName, TC_Exception::getSystemCode());
			}
		}

	protected:
		/**
		 * 文件路径
		 * File Path
		 */
		string _path;

		/**
		* 日志文件的最大大小
		* Maximum size of log file
		*/
		int _maxSize;

		/**
		* log文件最大数log文件命名为:xxx.log xxx1.log ...
		* Log file maximum log file name: xxx.log xxx1.log ...
		* xxx[_maxNum-1].log,_maxNum<=1 ?xxx.log
		*/
		int _maxNum;

		/**
		 * 日志文件
		 * log file
		 */
		ofstream _of;

		/**
		 * 具体写操作
		 * Write operation
		 */
		WriteT _t;

		/**
		 * 多长时间检查一次文件大小
		 * How often to check file size
		 */
		short _iUpdateCount;

		/**
		 * 多长时间检查一次文件大小
		 * How often to check file size
		 */
		time_t _lt;
	};

	typedef TC_Logger<TC_DefaultWriteT, TC_RollBySize> TC_RollLogger;

	/**
	 * @brief 根据时间滚动日志分隔类型
	 * @brief Separation type based on scrolling log
	 */
	class LogType : public TC_HandleBase
	{
	public:
		LogType() : _next_time_t(0), _format("%Y%m%d"), _frequency(1), _des("day")
		{
			_next_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			_next_cut_time = TC_Common::tm2str(_next_time_t, "%Y%m%d");
		}

		virtual ~LogType() {}
		//频率值
		//Frequency value
		virtual size_t frequence() = 0;

		virtual std::string get_init_time()
		{
			time_t tmp = _next_time_t - frequence();
			return TC_Common::tm2str(tmp, _format);
		}

		//返回空表示没有到下一个记录点
		//Returning null indicates that no next record point has been reached
		virtual std::string get_new_time(time_t t)
		{
			if (t >= _next_time_t && TC_Common::tm2str(t, _format) >= _next_cut_time)
			{
				//时间规整
				//Time regulation
				time_t new_nt = _next_time_t + ((t - _next_time_t) / frequence()) * frequence();
				_next_time_t = new_nt + frequence();
				_next_cut_time = TC_Common::tm2str(_next_time_t, _format);
				return TC_Common::tm2str(new_nt, _format);
			}
			else
			{
				return "";
			}
		}

		//转换成相应的字符串形式:1day,1hour,2minute
		//Convert to the corresponding string: 1day, 1hour, 2minute
		inline std::string &toString()
		{
			return _des;
		}

	protected:
		//计算下一个时间点
		//Calculate the next point in time
		void init(const string &format = "%Y%m%d", size_t frequency = 1)
		{
			_frequency = (frequency == 0 ? 1 : frequency);
			_format = format;

			_next_time_t = TNOW / frequence() + 1;
			_next_time_t *= frequence();

			_next_cut_time = TC_Common::tm2str(_next_time_t, _format);
		}

	protected:
		time_t _next_time_t;
		string _format;
		string _next_cut_time;
		size_t _frequency;
		string _des;
	};
	typedef TC_AutoPtr<LogType> LogTypePtr;

	class LogByDay : public LogType
	{
	public:
		static const string FORMAT;

	public:
		explicit LogByDay(const string &format = "%Y%m%d", size_t frequency = 1)
		{
			init(format, frequency);
			_des = TC_Common::tostr(_frequency) + "day";
		}
		size_t frequence()
		{
			const size_t DAY_IN_SECOND = 60 * 60 * 24;
			return _frequency * DAY_IN_SECOND;
		}
	};

	class LogByHour : public LogType
	{
	public:
		static const string FORMAT;

	public:
		explicit LogByHour(const string &format = "%Y%m%d%H", size_t frequency = 1)
		{
			init(format, frequency);
			_des = TC_Common::tostr(_frequency) + "hour";
		}

		size_t frequence()
		{
			const size_t HOUR_IN_SECOND = 60 * 60;
			return _frequency * HOUR_IN_SECOND;
		}
	};

	class LogByMinute : public LogType
	{
	public:
		static const string FORMAT;

	public:
		explicit LogByMinute(const string &format = "%Y%m%d%H%M", size_t frequency = 1)
		{
			init(format, frequency);
			_des = TC_Common::tostr(_frequency) + "minute";
		}
		size_t frequence()
		{
			const size_t MINUTE_IN_SECOND = 60;
			return _frequency * MINUTE_IN_SECOND;
		}
	};

	/**
	 * @brief 根据时间滚动日志
	 * @brief Scroll Log by Time
	 */
	template <typename WriteT>
	class TC_RollByTime : public TC_LoggerRoll, public std::mutex
	{
	public:
		typedef WriteT T;

		/**
		 * @brief 封装类(接口类)
		 * @brief Encapsulation class (interface class)
		 */
		class RollWrapperI : public RollWrapperBase<TC_RollByTime<WriteT> >
		{
		public:
			/**
			 * @brief 初始化.
			 * @brief Initialization
			 *
			 * @param path,日志路径
			 * @param path log path
			 * @param format，日志文件记录格式，按天，小时，分钟
			 * @param format，Log file record format, by day, hour, minute
			 * @param bHasSufix,日志文件是否添加".log"后缀
			 * @param bHasSufix, Does the log file add a '.log' suffix
			 * @param sConcatstr,日志路径和时间字串之间的连接符,例如：app_log/test_20121210.log
			 * @param sConcatstr,Connector between log path and time string, for example: app_Log/test_20121210.log
			 * @param logTypePtr,日志记录类型，详见LogType
			 * @param logTypePtr,Log record type, see LogType for details
			 * @param bIsRemote,是否是远程日志实例
			 * @param bIsRemote,Is it a remote log instance
			 */
			void init(const string &path, const string &format = "%Y%m%d", bool bHasSufix = true, const string &sConcatstr = "_", const LogTypePtr &logTypePtr = NULL, bool bIsRemote = false)
			{
				this->_roll->init(path, format, bHasSufix, sConcatstr, logTypePtr, bIsRemote);
			}

			/**
			 * @brief 获取日志路径.
			 * @brief Get log path.
			 *
			 * @return string
			 */
			string getPath() { return this->_roll->getPath(); }

			/**
			 * @brief 设置文件路径
			 * @brief Set file path
			 */
			void setPath(const string &path) { this->_roll->setPath(path); }

			/**
			 * @brief 获取格式.
			 * @brief Get  format.
			 *
			 * @return string
			 */
			string getFormat() { return this->_roll->getFormat(); }

			/**
			 * @brief 设置格式.
			 * @brief Set format.
			 *
			 * @param format,支持按天，按小时，按分钟格式
			 * @param format,Supports day-by-day, hour-by-minute formats
			 */
			void setFormat(const string &format) { this->_roll->setFormat(format); }

			void setRemote(bool en) { this->_roll->setRemote(en); }
		};

		/**
		 * @brief 构造
		 * @brief Construction
		 */
		TC_RollByTime() : _lt(0), _logTypePtr(NULL), _bRemoteType(false)
		{
			_lt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			_bHasSufix = true;
			_sConcatStr = "_";
		}

		/**
		 * @brief 析构
		 * @brief Destruction
		 */
		~TC_RollByTime()
		{
			if (_of.is_open())
			{
				_of.close();
			}
		}

		/**
		 * @brief 初始化.
		 * @brief Initialization
		 *
		 * @param path, 文件路径
		 * @param path, file path
		 * @param format, 格式
		 * @param format, format
		 * @param bHasSufix
		 * @param sConcatstr
		 * @param logTypePtr
		 * @param bIsRemote
		 */

		void init(const string &path, const string &format = "%Y%m%d", bool bHasSufix = true, const string &sConcatstr = "_", const LogTypePtr &logTypePtr = NULL, bool bIsRemote = false)
		{
			std::lock_guard<std::mutex> lock(*this);

			_logTypePtr = logTypePtr;
			_path = path;
			_format = format;
			_bHasSufix = bHasSufix;
			_sConcatStr = sConcatstr;
			_bRemoteType = bIsRemote;

			if (_logTypePtr)
			{
				_currentTime = logTypePtr->get_init_time();
			}
			else
			{
				_currentTime = TC_Common::now2str(_format);
			}
		}

		/**
		 * @brief 获取日志路径.
		 * @brief Get log path.
		 *
		 * @return string
		 */
		string getPath()
		{
			std::lock_guard<std::mutex> lock(*this);
			return _path;
		}

		/**
		 * @brief 设置文件路径
		 * @brief Set log path
		 */
		void setPath(const string &path)
		{
			std::lock_guard<std::mutex> lock(*this);
			_path = path;
		}

		/**
		 * @brief 获取格式.
		 * @brief Get  format
		 *
		 * @return string
		 */
		string getFormat()
		{
			std::lock_guard<std::mutex> lock(*this);
			return _format;
		}

		/**
		 * @brief 设置格式.
		 * @brief Set format.
		 *
		 * @param format,支持按天，按小时，按分钟格式
		 * @param format,Supports day-by-day, hour-by-minute formats
		 */
		void setFormat(const string &format)
		{
			std::lock_guard<std::mutex> lock(*this);
			_format = format;
		}

		/**
		  *
		  * 如果是关闭了本地日志，则不生成文件
		  * If the local log is closed, no files will be generated
		  * 如果是远程日志实例，本地不生成文件
		  * If it is a remote log instance, no files are generated locally
		  */
		void setRemote(bool en)
		{
			_bRemoteType = en;
		}
		/**
		 * @brief 获取写示例.
		 * @brief Get Write Samples
		 *
		 * @return WriteT&
		 */
		WriteT &getWriteT() { return _t; }

		/**
		 * @brief 函数对象.
		 * @brief Function object.
		 *
		 * @param path
		 * @param of
		 * @return string
		 */
		void roll(const deque<pair<size_t, string> > &buffer)
		{
			std::lock_guard<std::mutex> lock(*this);

			if (_path.empty())
			{
				_t(cout, buffer);
				return;
			}
			//远程日志在本地不用打开文件
			//Remote log does not need to open files locally
			if (_bRemoteType)
			{
				_t(_of, buffer);
				return;
			}

			time_t t = TNOW;//std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			string nowTime = "";
			//如果设置了记录类型,则使用记录类型来计算时间
			//If record type is set, use record type to calculate time
			if (_logTypePtr)
			{
				nowTime = _logTypePtr->get_new_time(t);
				//时间跨越了
				//Time has crossed
				if (nowTime != "")
				{
					_currentTime = nowTime;
					_of.close();
				}
				else
				{
					nowTime = _currentTime;
				}
			}
			else
			{
				nowTime = TC_Common::tm2str(t, _format);
				//检查时间是否跨时间了, 跨时间后关闭文件
				//Check if time is crossed and close the file after crossing time
				if (_currentTime != nowTime)
				{
					_currentTime = nowTime;
					_of.close();
				}
			}

			//每隔10s, 重新打开一次文件, 避免文件被删除后句柄不释放
			//Reopen the file every 10s to prevent the handle from being released after the file is deleted
			if (t - _lt > 10 || t - _lt < 0)
			{
				_lt = t;
				_of.close();
			}

			if (!_of.is_open())
			{
				string sLogFileName = _path + _sConcatStr + nowTime + (_bHasSufix ? ".log" : "");
				string sLogFilePath = TC_File::extractFilePath(_path);
				if (!TC_File::isFileExist(sLogFilePath))
				{
					TC_File::makeDirRecursive(sLogFilePath);
				}

				_of.open(sLogFileName.c_str(), ios::app);

				if (!_of)
				{
					//抛异常前继续进入_t 以便打远程日志
					//Continue to _t for remote logging before throwing an exception
					_t(_of, buffer);
					THROW_EXCEPTION_SYSCODE(TC_Logger_Exception, "[TC_RollByTime::roll]:fopen fail: " + sLogFileName);
					// throw TC_Logger_Exception("[TC_RollByTime::roll]:fopen fail: " + sLogFileName, TC_Exception::getSystemCode());
				}
			}

			//写
			//write
			_t(_of, buffer);
		}

	protected:
		/**
		 * 文件路径
		 * file path
		 */
		string _path;

		/**
		 * 时间格式
		 * time format
		 */
		string _format;

		/**
		 * 上次roll时的时间
		 * Last roll time
		 */
		string _currentTime;

		/**
		 * 日志文件
		 * log file
		 */
		ofstream _of;

		/**
		 * 具体写操作
		 * Write operation
		 */
		WriteT _t;

		/**
		 * 多长时间检查一次文件大小
		 * How often to check file size
		 */
		time_t _lt;
		/**
		 * 日志文件名是否带.log后缀
		 * Is the log file name suffixed with .log
		 */
		bool _bHasSufix;
		/**
		 * 日志文件名中用户自定义字符与日期字符间的连接符，默认是"_"
		 * Connector between user-defined character and date character in log file name, default is "_"
		 */
		string _sConcatStr;

		/**
		 * 按天/小时/分钟输出日志时的记录类型
		 * Record type when exporting log by day/hour/minute
		 */

		LogTypePtr _logTypePtr;
		/**
		 * 是否是远程日志实例
		 * Is it a remote log instance
		 */
		bool _bRemoteType;
	};

	typedef TC_Logger<TC_DefaultWriteT, TC_RollByTime> TC_DayLogger;
}

#endif

