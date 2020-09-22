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

#ifndef __TC_HTTP_H_
#define __TC_HTTP_H_

#include "util/tc_ex.h"
#include "util/tc_port.h"
#include "util/tc_common.h"
#include "util/tc_autoptr.h"
#include "util/tc_thread.h"
#include "util/tc_socket.h"
#include "util/tc_epoller.h"
#include "util/tc_timeout_queue.h"
#include "util/tc_network_buffer.h"
#include <map>
#include <sstream>
#include <cassert>
#include <vector>

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file tc_http.h 
 * @brief  http类.
 * @brief  http class
 *  
 * 包括TC_HttpRequest、TC_HttpResponse两个类；
 * Including TC_ HttpRequest、TC_ Two classes of httpresponse;
 *  
 * 支持GET HEAD POST OPTIONS，其他HTTP方法不支持；
 * GET HEAD POST OPTIONS is supported, not supported by other HTTP methods;
 *  
 * 通过TC_HttpRequest::checkRequest判断http请求是否收完；
 * Via TC_HttpRequest:: checkRequest determines whether an HTTP request has been received or not;
 *  
 * 与TC_ClientSocket配合，支持同步发送http请求，且支持http的chunk编码；
 * And TC_ClientSocket supports synchronous sending of HTTP requests and chunk encoding of http.
 *  
 * 发送http请求时，非线程安全
 * Non-thread-safe when sending HTTP requests
 *  
 * 1 支持http协议的解析,支持GET/POST模式
 * 1 Supports HTTP protocol resolution and GET/POST mode
 *  
 * 2 内嵌支持对chunk编码的解析
 * 2 Embedded support for chunk encoding parsing
 *  
 * 3 支持同步http请求和响应(只支持短连接,不支持keep-alive)
 * 3 Support synchronization of HTTP requests and responses (short connections only, keep-alive not supported)
 *  
 * 4 支持http响应包体的增量解析
 * 4 Supports incremental resolution of HTTP response packages
 *  
 * 5 增加Cookie管理类
 * 5 Add Cookie Management Class
 *  
 */             
/////////////////////////////////////////////////

class TC_NetWorkBuffer;

/**
* @brief  http协议解析异常类
* @brief  HTTP protocol resolution exception class
*/
struct TC_Http_Exception : public TC_Exception
{
    TC_Http_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
    ~TC_Http_Exception() {};
};

/**
 * @brief  http应答协议解析异常类
 * @brief  Http Answer Protocol Resolution Exception Class
 */
struct TC_HttpResponse_Exception : public TC_Http_Exception
{
    TC_HttpResponse_Exception(const string &sBuffer) : TC_Http_Exception(sBuffer){};
    ~TC_HttpResponse_Exception() {};
};

/**
 * @brief  http请求协议解析异常类
 * @brief  HTTP request protocol resolution exception class
 */
struct TC_HttpRequest_Exception : public TC_Http_Exception
{
    TC_HttpRequest_Exception(const string &sBuffer) : TC_Http_Exception(sBuffer){};
    ~TC_HttpRequest_Exception() {};
};

class TC_TCPClient;
class TC_HttpRequest;
class TC_HttpResponse;

/**   
 * @brief  简单的URL解析类.
 * @brief  Simple URL parsing class
 *  
 * eg：
 * ftp://user:password@www.qq.com:8080/abc/dev/query?a=b&c=3#ref.
 *  
 * scheme: ftp.
 *  
 * user:user.
 *  
 * pass:password.
 *  
 * domain:www.qq.com.
 *  
 * port:8080.
 *  
 * path:/abc/dev/query.
 *  
 * query:a=b&c=3.
 *  
 * ref:ref.
 *  
 * request:/abc/dev/query?a=b&c=3#ref.
 *  
 * relativePath:/abc/dev/.
 *  
 * rootPath:ftp://user:password@www.qq.com:8080/.
 */

class TC_URL
{
public:

    /**
     *  @brief  URL类型
     *  @brief  URL type
     */
    enum URL_TYPE
    {
        HTTP,
        HTTPS,
        FTP
    };

    /**
     *  @brief  构造函数
     *  @brief  Constructor
     */
    TC_URL() : _iURLType(HTTP)
    {
    }

    /**
     * @brief  解析URL, url必须是绝对路径.
     * @brief  Resolve URL, URL must be absolute path.
     *  
     * @throws       TC_URL_Exception
     * @param sURL  具体的URL串
     * @param sURL  Specific URL string
     * @return      解析成功返回true，否则返回false
     * @return      Resolution successfully returned true, otherwise false
     */
    bool parseURL(const string &sURL);

    /**
     * @brief   清空
     * @brief   Clean Up
     */
    void clear();

    /**
     * @brief  获取scheme.
     * @brief  Get scheme.
     * 
     * @return const string&类型的scheme
     * @return scheme of const string& type 
     */
    string getScheme() const;

    /**
     * @brief  获取用户名.
     * @brief  Get User Name
     * 
     * @return const string& ：用户名
     * @return const string@ : user name
     */
    string getUserName() const;

    /**
     * @brief  获取密码.
     * @brief  Get password
     * 
     * @return const string& ：密码
     * @return const string& : password
     */
    string getPassword() const;

    /**
     * @brief  获取域名.
     * @brief  Get domain name.
     * 
     * @return const string& ：域名
     * @return const string& : domain name
     */
    string getDomain() const;

    /**
     * @brief   获取端口.
     * @brief   Get port.
     * 
     * @return const string& ：端口
     * @return const string& : port
     */
    string getPort() const;

    /**
     * @brief   是否是缺省的端口.
     * @brief   Is it the default port?
     * 
     * @return bool型，类缺省的端口返回true，否则返回false
     * @return bool type, Class default port returns true, otherwise returns false
     */
    bool isDefaultPort() const;

    /**
     * @brief   获取路径.
     * @brief   Get path.
     * 
     * @return const string& ：路径值
     * @return const string& : path
     */
    string getPath() const;

    /**
     * @brief   获取查询部分.
     * @brief   Get the query section.
     * 
     * @return string ：查询结果
     * @return string : query results
     */
    string getQuery() const;

    /**
     * @brief 取获Request，不包括Host,
     *        例如http://www.qq.com/abc?a=b#ref则为:/abc?a=b#ref
     * @brief Get Request, excluding Host,
     *        for example:http://www.qq.com/abc?A=b#refIs:/abc?A=b#ref
     * @return 符合上述条件的request
     * @return Request that meets the above criteria
     */
    string getRequest() const;

    /**
     * @brief  获取Ref.
     * @brief  Get Ref.
     * 
     * @return Ref串
     * @return Ref string
     */
    string getRef() const;

    /**
     * @brief   是否有效.
     * @brief   Is it valid?
     * 
     * @return 有效返回true，否则返回false
     * @return is valid: return true; else: retrun false
     */
    bool isValid() const;

    /**
     * @brief   获取解析后URL.
     * @brief   Get the parsed URL.
     * 
     * @return 解析后的URL
     * @return Resolved URL
     */
    string getURL() const;

    /**
     * @brief   获取URL类型.
     * @brief   Get the URL type.
     * 
     * @return  URL类型
     * @return  url types
     */
    int getType() const { return _iURLType; }

    /**
     * @brief   获取相对路径.
     * @brief   Get the relative path.
     * 
     * @return 相对路径字符串
     * @return Relative path string
     */
    string getRelativePath() const;

    /**
     * @brief   获取根路径.
     * @brief   Get the root path.
     * 
     * @return  根路径字符串
     * @return  Root Path String
     */
    string getRootPath() const;

    /** 
     * @brief 调整路径.
     * @brief Adjust the path.
     *  
     * 以当前URL调整路径.
     * Adjust the path with the current URL.
     *  
     * 1 http://www.qq.com/a/b?a=b, /test/abc => http://www.qq.com/test/abc
     *  
     * 2 http://www.qq.com/a/b?a=b, test/abc => http://www.qq.com/a/b/test/abc
     *  
     * 3 http://www.qq.com/a/b?a=b, ../test/abc   => http://www.qq.com/a/test/abc 
     *  
     * @param  sRelativeURL: 相对当前URL的地址
     * @param  sRelativeURL Address relative to current URL
     * @return 返回调整后的URL
     * @return return adjusted URL
     */
    TC_URL buildWithRelativePath(const string &sRelativeURL) const;

    /**
     * @brief 规整化.
     * @brief regularization
     */
    void specialize();

protected:
	friend class TC_HttpRequest;

    /**
     * @brief  换成URL.
     * @brief  Change to URL
     * 
     * @return URL串
     * @return URL string
     */
    string toURL();

    /**
     * @brief  类型到字符串的转换
     * @brief  Type to String Conversion
     * 
     * @return string：转换后的字符串
     * @return string: Converted string
     */
    string type2String() const;

    /**
     * @brief  获取协议的缺省端口.
     * @brief  Get the default port for the protocol
     * 
     * @return string：缺省端口
     * @return string: default port
     */
    string getDefaultPort() const;

    /**
     * @brief  简化URL.
     * @brief  Simplify URL.
     *  
     * @param sPath 原路径
     * @param sPath Original Path
     * @return      简化后的URL
     * @return      Simplified URL
     */
    string simplePath(const string &sPath) const;

protected:

    /**
     * URL类型
     * URL type
     */
    int                 _iURLType;

    string                _sScheme;
    string                _sUser;
    string                _sPass;
    string                _sDomain;
    string                _sPort;
    string                _sPath;
    string                _sQuery;
    string                _sRef;
    /**
     * 解析后的URL
     * Resolved URL
     */
    string              _sURL;
};

/**
 * @brief  http协议解析类, 请求和响应都在该类中解析
 * @brief  HTTP protocol resolution class in which both requests and responses are resolved
 */
class TC_Http
{
public:
	static unordered_map<string, int> HEADER;
	static unordered_map<int, string> HEADER_REVERSE;

    /**
     * @brief  构造函数
     * @brief  Constructor
     */
    TC_Http()
    {
        TC_Http::reset();
        /*Short connection by default*/
        setConnection("close");     //默认就用短连接
    }

    /**
     * @brief  不区分头部大小写
     * @brief  Header case insensitive
     */
    struct CmpCase
    {
        bool operator()(const string &s1, const string &s2) const;
    };

    typedef multimap<string, string, CmpCase> http_header_type;

    /**
     * @brief  删除头部.
     * @brief  Remove Header
     *
     * @param sHeader:头部信息
     */
    void eraseHeader(const string &sHeader) { _headers.erase(sHeader); }

    /**
     * @brief  设置 Cache-Control.
     * @brief  Set Cache-Control
     *  
     * @param sCacheControl
     */
    void setCacheControl(const string &sCacheControl){setHeader("Cache-Control", sCacheControl);}

    /**
     * @brief  设置 Connection.
     * @brief  Set Connection
     *  
     * @param sConnection：Connection信息
     */
    void setConnection(const string &sConnection){setHeader("Connection", sConnection);}

    /**
     * @brief  设置 Content-Type.
     * @brief  Set Content-Type
     *  
     * @param sContentType：Content信息
     * @param sContentType：Content Info
     */
    void setContentType(const string &sContentType){setHeader("Content-Type", sContentType);}

    /**
     * @brief  设置内容长度.
     * @brief  Set content length
     *  
     * @param iContentLength：要设置的长度值
     * @param iContentLength：the length to set
     */
    void setContentLength(size_t iContentLength)
    {
        setHeader("Content-Length", TC_Common::tostr(iContentLength));
    }

    /**
     * @brief  设置 Referer.
     * @brief  Set Referer.
     *  
     * @param sReferer：Referer信息
     * @param sReferer：Referer Info
     */
    void setReferer(const string &sReferer){setHeader("Referer", sReferer);}

    /**
     * @brief  设置 Host.
     * @brief  Set Host.
     *  
     * @param sHost 例如 www.qq.com:80
     * @param sHost for example: www.qq.com:80
     */
    void setHost(const string &sHost){setHeader("Host", sHost);}

    /**
     * @brief  设置 Accept-Encoding.
     * @brief  Set Accept-Encoding.
     *  
     * @param sAcceptEncoding, gzip, compress, deflate, identity
     */
    void setAcceptEncoding(const string &sAcceptEncoding){setHeader("Accept-Encoding", sAcceptEncoding);}

    /**
     * @brief  设置 Accept-Language.
     * @brief  Set Accept-Language.
     *  
     * @param sAcceptLanguage：Accept-Language信息
     * @param sAcceptLanguage：Accept-Language Info
     */
    void setAcceptLanguage(const string &sAcceptLanguage){setHeader("Accept-Language", sAcceptLanguage);}

    /**
     * @brief  设置 Accept.
     * @brief  Set Accept.
     *  
     * @param sAccept Accept信息
     * @param sAccept Accept Info
     */
    void setAccept(const string &sAccept){setHeader("Accept", sAccept);}

    /**
     * @brief  设置 Transfer-Encoding.
     * @brief  Set Transfer-Encoding.
     *  
     * @param sTransferEncoding：Transfer-Encoding信息
     * @param sTransferEncoding：Transfer-Encoding Info
     */
    void setTransferEncoding(const string& sTransferEncoding) {setHeader("Transfer-Encoding", sTransferEncoding); }

    /**
     * @brief  设置header，
     * 常用的值请使用已经有的get/set方法设置
     * (除Set-Cookie&Cookie外,头部字段不能重复)
     * @brief  Set header,
     * Use existing get/set method settings for commonly used values
     * (Header fields cannot be repeated except Set-Cookie&Cookie)
     * @param sHeadName  header的名字
     * @param sHeadName  header name
     * @param sHeadValue header的值
     * @param sHeadValue header value
     */
    void setHeader(const string &sHeadName, const string &sHeadValue);

    /**
     * @brief  设置header，常用的值请使用已经有的get/set方法设
     *         (头部字段可以重复)
     * @brief  Set header, use get/set method to set common values
     *         (Header fields can be repeated)
     * @param sHeadName   header的名字
     * @param sHeadName   header name
     * @param sHeadValue  header的值
     * @param sHeadValue  header value
     */
    void setHeaderMulti(const string &sHeadName, const string &sHeadValue) 
    {
        _headers.insert(multimap<string, string>::value_type(sHeadName, sHeadValue));
    }

    /**
     * @brief  获取头(重复的也提取).
     * @brief  Get header(duplicate also extracts).
     *
     * @param sHeadName  header的名字
     * @param sHeadName  header value
     * @return           vector<string>header的值
     * @return           vector<string>header value
     */
    vector<string> getHeaderMulti(const string &sHeadName) const;

    /**
     * @brief  获取http内容长度.
     * @brief  Get HTTP content length.
     *
     * @return http内容长度.
     * @return http content length.
     */
    size_t getContentLength() const;

    /**
     * @brief  获取http请求Host.
     * @brief  Get HTTP request Host.
     *
     * @return host请求信息
     * @return Host request information
     */
    string getHost() const;

    /**
     * @brief 获取http头部长度.
     * @brief Get HTTP header length
     *
     * @return 头部长度
     * @return Head length
     */
    size_t getHeadLength() const { return _headLength; }

    /**
     * @brief 获取http内容.
     * @brief Get HTTP content.
     *
     * @return http内容串
     * @return HTTP content string
     */
    const string &getContent() const { return _content; }

    /**
     * @brief get body
     * @return http body
     */
    string &getContent() { return _content; }

    /**
     * @brief 请求body内容, 其他都不变
     * @brief Request body content, nothing else changes
     *
     * @return
     */
    void clearContent() { _content.clear(); }

    /**
     * append content
     * @param append
     * @param bUpdateContentLength
     */
    void appendContent(const char *buff, size_t len, bool bUpdateContentLength = false)
    {
	    _content.append(buff, len);

	    if(bUpdateContentLength)
	    {
		    eraseHeader("Content-Length");
		    if(_content.length() > 0)
			    setContentLength(_content.length());
	    }
    }

	/**
     * @brief 设置http body(默认不修改content-length).
     * @brief Set HTTP body (content-length is not modified by default).
     *  
     * @param content               http body内容
     * @param content               http body content
     * @param bUpdateContentLength  是否更新ContentLength
     * @param bUpdateContentLength  Whether to update ContentLength
     */
    void setContent(const string &content, bool bUpdateContentLength = false) 
    {
        _content = content; 

        if(bUpdateContentLength)
        {
            eraseHeader("Content-Length");
            if(_content.length() > 0)
                setContentLength(_content.length());
        }
    }

    /**
     * @brief 获取内容类型.
     * @brief Get Content Type.
     *
     * @return string
     */
    string getContentType() const;

    /**
     * @brief 获取http头部参数，(Set-Cookie和Cookie不要用这个获取,
     *        单独有函数获取)
     * @brief Get HTTP header parameters, (Set-Cookie and Cookie do not use this to get,
     *        other function feeches)
     * @param sHeader header名字
     * @param sHeader header name
     * @return        header的相关信息
     * @return        header related information
     */
    string getHeader(const string& sHeader) const;

    /**
     * @brief 是否有header
     * @brief Whether there is a header or not.
     * @param sHeader
     * @return
     */
    bool hasHeader(const char *sHeader) const;

    /**
     * @brief 检查头部
     * @brief check header
     * @param sHeader
     * @param value
     * @return
     */
	bool checkHeader(const char *sHeader, const char *value) const;

	/**
	 * @brief 获取http头部map.
     * @brief Get HTTP header map
	 *
	 * @return http_header_type&
	 */
    const http_header_type& getHeaders() const{return _headers;}

	/**
	 * get headers
	 * @param header
	 */
	void getHeaders(map<string, string> &header);

     /**
      * @brief 重置
      * @brief Reset
      */
    void reset();

    /**
     * @brief 生成头部字符串(不包含第一行), 直接累加到sHttpHeader后
     * @brief Generate a header string (excluding the first line) and add it directly to the  back of sHttpHeader
     *
     * @return string：头部字符串
     * @return string: header string
     */
    void genHeader(string &sHttpHeader) const;

    /**
     * @brief 生成头部字符串(不包含第一行)
     * @brief Generate header string (excluding first line)
     * @return
     */
	string genHeader() const;

	/**
	 * @brief 该http原始数据包是否是chunk编码格式.
     * @brief Whether the original HTTP packet is in chunk encoding format.
	 *
	 * @return bool：包含返回true，否则返回false
     * @return bool: Contains returns true, otherwise returns false
	 */
    bool isChunked() const { return _bIsChunked; }

    /**
     * @brief 解析请求head，不解析第一行,
     *        第一行请求包和响应包不一样， 后面的数据解析为map格式
     * @brief Resolve the request head without resolving the first line,
     *        The first row of request and response packages is different, and the subsequent data is parsed into map format
     * @param szBuffer
     * @return const char*, 偏移的指针
     * @return const char*, Offset pointer
     */
//    static const char* parseHeader(const char* szBuffer, http_header_type &sHeader);
//
//    template<typename ForwardIterator1, typename ForwardIterator2>
//	static void parseHeader(const ForwardIterator1 &beginIt, const ForwardIterator2 &headerIt, http_header_type &sHeader)
//	{
//		sHeader.clear();
//
//		string sep      = "\r\n";
//		string colon    = ":";
//
//		bool first      = true;
//		auto lineStartIt= beginIt;
//
//		while (true)
//		{
//			auto it = std::search(lineStartIt, headerIt, sep.c_str(), sep.c_str() + sep.size());
//			if(it == headerIt)
//			{
//				break;
//			}
//
//			//first line ignore
//			if(!first)
//			{
//				auto itF = std::search(lineStartIt, it, colon.c_str(), colon.c_str() + colon.size());
//				if (itF != it)
//				{
//					string name;
//					name.resize(itF - lineStartIt);
//					std::copy(lineStartIt, itF, name.begin());
//
//					string value;
//					value.resize(it - (itF + 1));
//					std::copy(itF + 1, it, value.begin());
//
//					sHeader.insert(multimap<string, string>::value_type(TC_Common::trim(name, " "),
//					                                                    TC_Common::trim(value, " ")));
//
//				}
//			}
//			else
//			{
//				first = false;
//			}
//
//			lineStartIt = it + sep.size();
//		}
//	}

	/**
	 * 解析, 尽量避免内存copy, 提升效率
     * Resolve, avoid memory copy as much as possible, improve efficiency
	 * @param beginIt
	 * @param headerIt
	 * @param sHeader
	 */
	static size_t parseHeaderString(const char *beginIt, const char *headerIt, http_header_type &sHeader);

protected:

    /**
     * http头部所有变量
     * HTTP header all variables
     */
    http_header_type    _headers;

    /**
     * http头部长度
     * HTTP header length
     */
    size_t              _headLength;

	/**
	 * 获取版本
     * get version
	 */
	string  _version;

    /**
     * http头部内容
     * HTTP header content
     */
    string              _content;

    /**
     * 是否是chunk模式
     * Is it a chunk mode
     */
    bool                _bIsChunked;
};

/********************* TC_HttpCookie***********************/

/**
 * @brief 简单的Cookie管理类.
 * @brief Simple Cookie Management Class
 */
class TC_HttpCookie
{
public:
    typedef map<string, string, TC_Http::CmpCase> http_cookie_data;

    struct Cookie
    {
        http_cookie_data    _data;
        string              _domain;
        string              _path;
        /*Equal to 0 means valid only for the current call*/
        time_t              _expires;       //等于0表示只在当前回话有效
        bool                _isSecure;
    };

    /**
     * @brief 清空数据
     * @brief Clear data
     */
    void clear();

    /**
     * @brief 增加Cookie到管理器中.
     * @brief Add cookies to the manager.
     *  
     * @param sCookieRspURL 产生该Cookie HTTP响应的URL
     * @param sCookieRspURL The URL that generated the cookie HTTP response
     * @param vCookies      set-Cookie字符串
     * @param vCookies      Set-Cookie string
     */
    void addCookie(const string &sCookieRspURL, const vector<string> &vCookies);

    /**
     * @brief 增加Cookie到管理器.
     * @brief Add Cookies to Manager.
     *  
     * @param cookie
     */
    void addCookie(const Cookie &cookie);

    /**
     * @brief 增加Cookie到管理器.
     * @brief Add Cookies to Manager.
     *  
     * @param cookie
     */
    void addCookie(const list<Cookie> &cookie);

    /**
     * @brief 获取某个url的cookie域值对，去掉了Domain,Path等字段
     * @brief Gets a cookie field value pair for a url, removes fields such as Domain, Path, and so on
     * @param sReqURL 需要匹配的URL
     * @param sReqURL URL required matching
     * @param cookies
     */
    void getCookieForURL(const string &sReqURL, list<Cookie> &cookies);

    /**
     * @brief 获取某个url的cookie域值对，去掉了Domain,Path等字段
     * @brief Gets a cookie field value pair for a url, removes fields such as Domain, Path, and so on
     * @param sReqURL  需要匹配的URL
     * @param sReqURL URL required matching
     * @param sCookie
     * @param string  
     */
    void getCookieForURL(const string &sReqURL, string &sCookie);

    /**
     * @brief 匹配域名，sCookieDomain串有多个域
     * @brief Match domain name, sCookieDomain string has multiple domains
     * sCookieDomain未以.开头, 则sCookieDomain=sDomain
     * SCookieDomain does not start with. then sCookieDomain=sDomain
     * sCookieDomain以.开头,则sDomain右边匹配sDomain,sDomain比sCookieDomain以最多多一个.
     * The sCookieDomain starts with.And the right side of the sDomain matches the sDomain, which has a maximum of one compared to the sCookieDomain.
     * (x.y.z.com 匹配 .y.z.com, x.y.z.com 不匹配.z.com )
     * (x.y.z.com matches.y.z.com, x.y.z.com does not match.z.com)
     * @param sCookieDomain
     * @param sDomain       域名
     * @param sDomain       domain
     * @return bool         匹配成功返回true，否则返回false
     * @return bool         Match successfully returns true, otherwise returns false
     */
    static bool matchDomain(const string &sCookieDomain, const string &sDomain);

    /**
     * @brief 匹配路径.
     * @brief Match paths.
     * 
     * @param sCookiePath sPath的前缀
     * @param sCookiePath Prefix for sPath
     * @param sPath       路径
     * @param sPath       path
     * @return            匹配路径的长度
     * @return            The length of the matched path
     */
    static size_t matchPath(const string &sCookiePath, const string &sPath);

    /**
     * @brief 获取所有的Cookie.
     * @brief Get all cookies.
     * 
     * @return list<Cookie>&
     */
    const list<Cookie>& getAllCookie();

    /**
     * @brief  删除过期的Cookie，仅仅存在与当前回话的Cookie不删除
     * @brief  Delete expired cookies, only those that exist with the current call do not delete
     * @param t       超过当前时间的cookie都删除
     * @param t       Cookies older than the current time are deleted
     * @param bErase true:存在于当前回话的Cookie也删除
     *               false:存在于当前回话的Cookie不删除
     * @param bErase true: Cookies that exist in the current call are also deleted
     *               false:Cookies that exist in the current call are not deleted
     */
    void deleteExpires(time_t t, bool bErase = false);

    /**
     * @brief  返回所有没有过期的Cookie且有Expires字段的Cookie，
     * 即仅仅存在与当前回话的Cookie是不返回的通常用于永久序列化
     * @brief  Returns all Cookies that are not expired and have an Expires field, 
     * that is, only Cookies that exist with the current call are not returned and are usually used for permanent serialization
     * @param t 超过当前时间的cookie都删除
     * @param t Cookies older than the current time are deleted
     * @return  list<Cookie>:所有符合条件的cookie
     * @return  List <Cookie>: All eligible cookies
     */
    list<Cookie> getSerializeCookie(time_t t);

protected:
    /**
     * @brief  判断Cookie是有效.
     * @brief  Judge that the cookie is valid.
     *  
     * @param tURL 
     * @return size_t
     */
    size_t isCookieMatch(const Cookie &cookie, const TC_URL &tURL) const;

    /**
     * @brief  检查Cookie是否过期，当前回话的不算过期(expires=0)
     * @brief  Check if the cookie is expired and the current call is not expired (expires=0)
     * @param cookie 被检查的cookie对象
     * @param cookie Checked cookie object
     * @return bool  过期返回true，否则返回false
     * @return bool  Expiration returns true, otherwise false
     */
    bool isCookieExpires(const Cookie &cookie) const;

    /**
     * @brief  添加Cookie.
     * @brief  Add cookies.
     *  
     * @param cookie  要添加的cookie对象
     * @param cookie  The cookie object to be added
     * @param cookies 要被添加的list对象
     * @param cookies List object to be added
     */
    void addCookie(const Cookie &cookie, list<Cookie> &cookies);

    /**
     * @brief 修正Domain.
     * @brief Fix Domain.
     *  
     * @param sDomain  修正前的Domain
     * @param sDomain  Domain before correction
     * @return string 修正后的Domain
     * @return string Modified Domain
     */
    bool fixDomain(const string &sDomain, string &sFixDomain);

protected:

    /**
     * @brief  保存Cookie.
     * @brief  Save the cookie.
     *  
     * key:domain+path
     */
    list<Cookie> _cookies;
};

/********************* TC_HttpResponse ***********************/

class TC_HttpResponse : public TC_Http
{
public:

    /**
     * @brief 构造
     * @brief Construct
     */
    TC_HttpResponse()
    {
        TC_HttpResponse::reset();
    }

    /**
     * @brief reset响应包
     * @brief Reset response package
     */
    void reset();

    /**
     * @brief 增量decode,输入的buffer会自动在解析过程中被清除掉，
     * 增量decode之前必须reset，
     * (网络接收的buffer直接添加到sBuffer里面即可, 然后增量解析)
     * (能够解析的数据TC_HttpResponse会自动从sBuffer里面消除，直到网络接收完毕或者解析返回true)
     * @brief Incremental decode, input buffers are automatically cleared during parsing.
     * Must reset before incremental decode,
     * (The buffers received by the network are added directly to the sBuffer and then incrementally resolved)
     * (The resolvable data TC_HttpResponse is automatically eliminated from the sBuffer until the network has received it or the resolving returns true)
     * @param buffer
     * @throws TC_HttpResponse_Exception, 不支持的http协议, 抛出异常
     * @throws TC_HttpResponse_Exception, unsupported http protocol, throwing exception
     * @return true:解析出一个完整的buffer
     *        false:还需要继续解析，如果服务器主动关闭连接的模式下
     *        , 也可能不需要再解析了
     * @return true: resolves a complete buffer
     *         false: You also need to continue parsing if the server actively closes the connection in mode
     *         Or you may not need to parse any more
     */
	bool incrementDecode(TC_NetWorkBuffer &buff);

    /**
     * @brief 解析http应答(采用string方式) ，
     * 注意:如果http头部没有Content-Length且非chunk模式, 则返回true
     * 需要网络层判断(http服务端主动关闭连接也算是发送http响应完整了)
     * @brief Resolve the HTTP response (in string mode),
     * Note: Returns true if the HTTP header has no Content-Length and is not in chunk mode
     * Network layer judgment is required (proactive closure of the connection by the HTTP server is equivalent to sending a complete http response)
     * @param sBuffer
     * @return bool, sBuffer是否是完整的http请求
     * @return bool, is sBuffer a complete HTTP request
     */
    bool decode(const string &sBuffer);

    /**
     *  @brief 解析http应答，
     * 注意:如果http头部没有Content-Length且非chunk模式, 则返回true
     * 需要网络层判断(http服务端主动关闭连接也算是发送http响应完整了)
     * @brief Resolve the HTTP response,
     * Note: Returns true if the HTTP header has no Content-Length and is not in chunk mode
     * Network layer judgment is required (proactive closure of the connection by the HTTP server is equivalent to sending a complete http response)
     * @param sBuffer
     * @return bool, sBuffer是否是完整的http请求
     * @return bool, is sBuffer a complete HTTP request
     */
    bool decode(const char *sBuffer, size_t iLength);

    /**
     * @brief 编码应答包(采用string方式).
     * @brief Encode the answer pack (in string mode).
     *  
     * @param sBuffer 
     * @return string 编码后的应答包
     * @return string Encoded Answer Pack
     */
    string encode() const;

    /**
     * @brief 编码应答包(采用vector<char>方式).
     * @brief Encode the response pack (using vector <char>mode).
     *  
     * @param sBuffer 
     * @return string 编码后的应答包(vector<char>形式的)
     * @return string Encoded Response Pack (in the form of vector<char>
     */
    void encode(vector<char> &sBuffer) const;

    /**
     * @brief 获取第一行(HTTP/1.1 200 OK).
     * @brief Get the first row (HTTP/1.1 200 OK).
     * 
     * @return string 获取的内容
     * @return string content obtained
     */
    string getResponseHeaderLine() const { return _headerLine; }

    /**
     * @brief 获取HTTP响应状态(例如200).
     * @brief Get the HTTP response status (for example, 200).
     *
     * @return int
     */
    int  getStatus() const {return _status; }

    /**
     * @brief 设置状态.
     * @brief Set state
     *  
     * @param status 状态值
     * @param status state value
     */
    void setStatus(int status) { _status = status; }

    /**
     * @brief 获取描述(例如OK).
     * @brief Get a description (for example, OK).
     * 
     * @return string 描述信息
     * @return string Description Information
     */
    string getAbout() const { return _about; }

    /**
     * @brief 设置描述.
     * @brief Set Description
     *  
     * @param about 描述信息
     * @param about Descriptive Information
     */
    void setAbout(const string &about) { _about = about; }

    /**
     * @brief 获取版本, 例如HTTP/1.1   .
     * @brief Get a version, such as HTTP/1.1
     * 
     * @return string 版本信息
     * @return string version info
     */
    string getVersion() const { return _version; }

    /**
     * @brief 另种模式:HTTP/1.1(默认)或者HTTP/1.0  .
     * @brief Another mode: HTTP/1.1 (default) or HTTP/1.0
     *  
     * @param sVersion
     */
    void setVersion(const string &sVersion) { _version = sVersion; }

    /**
     * @brief 设置应答状态(采用string方式).
     * @brief Set the response status (string mode).
     *  
     * @param status 状态码
     * @param status Status Code
     * @param about 描述信息
     * @param about Descriptive Information
     * @param sBody post协议body的内容
     * @param sBody Contents of post protocol body
     */
    void setResponse(int status = 200, const string& about = "OK", const string& sBody = "");

    /**
     * @brief 设置应答状态.
     * @brief Set the response status.
     *  
     * @param status 状态码
     * @param status status code
     * @param about  描述信息
     * @param about  Descriptive Information
     * @param sBuffer post协议body的内容
     * @param sBuffer Contents of post protocol body
     * @param iLength sBuffer长度
     * @param iLength sBuffer length
     */
    void setResponse(int status, const string& about, const char *sBuffer, size_t iLength);

    /**
     * @brief 设置应答, 缺省status=200, about=OK.
     * @brief Set the response, default status=200, about=OK.
     *  
     * @param sBuffer 应答内容
     * @param sBuffer Answer Content
     * @param iLength sBuffer长度
     * @param iLength sBuffer length
     */
    void setResponse(const char *sBuffer, size_t iLength);

    /**
     * @brief 设置服务.
     * @brief Set up server
     *  
     * @param sServer 服务的信息
     * @param sServer server info
     */
    void setServer(const string &sServer){setHeader("Server", sServer);}

    /**
     * @brief 设置Set-Cookie.
     * @brief set up Set-Cookie
     *  
     * @param sCookie Cookie信息
     * @param sCookie Cookie info
     */
    void setSetCookie(const string &sCookie){setHeader("Set-Cookie", sCookie);}

    /**
     * @brief 获取Set-Cookie.
     * @brief get Set-Cookie
     *
     * @return vector<string>
     */
    vector<string> getSetCookie() const;

    /**
     * @brief 解析应答头.
     * @brief Analytic answer header
     *
     * @param szBuffer 应答头信息
     * @param szBuffer Answer Header Information
     * @return
     */
//    /**
//     *
//     * @param szBuffer
//     */
//    template<typename ForwardIterator1, typename ForwardIterator2>
//	void parseResponseHeader(const ForwardIterator1 &beginIt, const ForwardIterator2 &headerIt)
//	{
//		string line = "\r\n";
//
//		auto it = std::search(beginIt, headerIt, line.c_str(), line.c_str() + line.size());
//
//		assert(it != headerIt);
//
//		string sep = " ";
//
//		auto f1 = std::search(beginIt, headerIt, sep.c_str(), sep.c_str() + sep.size());
//		if(f1 == headerIt)
//		{
//			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response parse version format error : " + string(beginIt, it));
//		}
//
//		auto f2 = std::search(f1 + 1, headerIt, sep.c_str(), sep.c_str() + sep.size());
//		if(f1 == headerIt)
//		{
//			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response parse status format error : " + string(beginIt, it));
//		}
//
//		_headerLine = string(beginIt, it);
//
//		if(TC_Port::strncasecmp(_headerLine.c_str(), "HTTP/", 5) != 0)
//		{
//			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response version is not start with 'HTTP/' : " + _headerLine);
//		}
//
//		_version    = string(beginIt, f1);
//
//		_status     = TC_Common::strto<int>(string(f1 + 1, f2));
//
//		_about      = TC_Common::trim(string(f2 + 1, it));
//
//		parseHeader(beginIt, headerIt, _headers);
//	}

	size_t parseResponseHeaderString(const char *beginIt, const char *headerIt);

protected:
    /**
     * 添加内容
     * Add content
     * @param sBuffer
     */
    void addContent(const string &sBuffer);

    /**
     * 添加内容
     * Add content
     * @param buffer
     * @param length
     */
	void addContent(const char *buffer, size_t length);

protected:

    /**
     * 应答状态
     * Response Status
     */
    int     _status;

    /**
     * 应答描述
     * Response Description
     */
    string  _about;

    /**
     * 获取第一行
     * Get the first row
     */
    string  _headerLine;

    /**
     * 临时的content length
     * temporary content length
     */
    size_t  _iTmpContentLength;

    /**
     * 接收到数据的长度
     * Length of received data
     */
    size_t  _iRecvContentLength;

};

/********************* TC_HttpRequest ***********************/

class TC_HttpRequest : public TC_Http
{
public:

    ///////////////////////////////////////////////////////////////////
    TC_HttpRequest()
    {
        TC_HttpRequest::reset();
        setUserAgent("Tars-Http");
    }

    /**
     * @brief 请求协议类型
     * @brief Request Agreement Type
     */
    enum
    {
        REQUEST_GET,
        REQUEST_POST,
        REQUEST_OPTIONS,
        REQUEST_HEAD,
        REQUEST_PUT,
        REQUEST_DELETE,
        REQUEST_PATCH,
	    REQUEST_PRI,
    };

    /**
     * @brief 检查http请求是否收全.
     * @brief Check that the HTTP request is complete.
     *
     * @param sBuffer http请求
     * @param sBuffer HTTP request
     * @throws TC_HttpRequest_Exception, 不支持的http协议, 抛出异常
     * @throws TC_HttpRequest_Exception, unsupported http protocol, throw exception
     * @return  true: 收全, false:不全
     * @return  True:Collection, False:Incomplete
     */
    static bool checkRequest(const char* sBuffer, size_t len);

    /**
     * 检查http包是否收全
     * Check if the HTTP package is complete
     * @param buff
     * @return
     */
	static bool checkRequest(TC_NetWorkBuffer &buff);

	/**
     * @brief 重置
     * @brief Reset
     */
    void reset();

    /**
     * @brief 设置 User-Agent.
     * @brief Set up User-Agent
     *  
     * @param sUserAgent
     */
    void setUserAgent(const string &sUserAgent){setHeader("User-Agent", sUserAgent);}

    /**
     * @brief 设置 Cookie.
     * @brief Set up Cookie
     *  
     * @param sCookie
     */
    void setCookie(const string &sCookie){setHeader("Cookie", sCookie);}

    /**
     * @brief 获取原始Cookie行.
     * @brief Get the original Cookie line.
     * 
     * @return vector<string>
     */
    vector<string> getCookie();

    /**
     * @brief 解析http请求, 如果不是HTTP请求则抛出异常.
     * @brief Resolve the HTTP request and throw an exception if it is not an HTTP request.
     *  
     * @param sBuffer 要解析的http请求
     * @param sBuffer HTTP request to resolve
     * @return        sBuffer是否是完整的http请求
     * @return        Is sBuffer a complete HTTP request
     * @throw         TC_HttpRequest_Exception 
     */
    bool decode(const string &sBuffer);

	/**
	* @brief 解析http请求, 如果不是HTTP请求则抛出异常.
    * @brief Resolve the HTTP request and throw an exception if it is not an HTTP request.
	*
	* @param sBuffer 要解析的http请求
    * @param sBuffer HTTP request to resolve
	* @return        sBuffer是否是完整的http请求
    * @return        Is sBuffer a complete HTTP request
	* @throw         TC_HttpRequest_Exception
	*/
	bool decode(const vector<char> &sBuffer);

	/**
     * @brief 解析http请求,
     *        如果不是HTTP请求则抛出异常(采用vector<char>方式).
     * @brief Resolve HTTP requests,
     *        If it is not an HTTP request, an exception is thrown (in vector<char>mode).
     *  
     * @param sBuffer http请求
     * @param sBuffer HTTP request
     * @param iLength 长度
     * @param iLength length
     * @throw         TC_HttpRequest_Exception
     * @return        sBuffer是否是完整的http请求
     * @return        Is sBuffer a complete HTTP request
     */
    bool decode(const char *sBuffer, size_t iLength);

    /**
     * @brief 生成请求(采用string方式).
     * @brief Generate a request (in string mode).
     *  
     * @param sUrl 
     * @return string
     */
    string encode();

    /**
     * @brief 生成请求(采用vector<char>方式).
     * @brief Generate the request (using vector <char>).
     *  
     * @param buffer请求内容
     */
    void encode(vector<char> &buffer);

    /**
     * encode buffer to TC_NetWorkBuffer
     * @param buff
     */
	void encode(TC_NetWorkBuffer &buff);

	/**
     * @brief 设置请求包.
     * @brief Set the request package.
     *  
     * @param sUrl         例如:http://www.qq.com/query?a=b&c=d
     * @param sUrl         for example: http://www.qq.com/query?a=b&c=d
     * @param bCreateHost  是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     * @param bCreateHost  Whether to create new Host information for the header
     *                     (By default, if there is a Host message, it is not created)
     *                     (Note that it was created at encode time)
     */
    void setRequest(const string& method, const string &sUrl, const std::string& body = "", bool bNewCreateHost = false);
    
    /**
     * @brief 设置Get请求包.
     * @brief Set request package of Get.
     *  
     * @param sUrl         例如:http://www.qq.com/query?a=b&c=d
     * @param sUrl         for example: http://www.qq.com/query?a=b&c=d
     * @param bCreateHost  是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     * @param bCreateHost  Whether to create new Host information for the header
     *                     (By default, if there is a Host message, it is not created)
     *                     (Note that it was created at encode time)
     */
    void setGetRequest(const string &sUrl, bool bNewCreateHost = false);

    /**
     * @brief 设置Head请求包.
     * @brief Set Head Request Package
     *
        * @param sUrl         例如:http://www.qq.com/query?a=b&c=d
        * @param sUrl         for example: http://www.qq.com/query?a=b&c=d
        * @param bCreateHost  是否新创建头部的Host信息
        *                     (默认, 如果有Host信息了, 就不创建)
        *                     (注意, 是在encode的时候创建的)
        * @param bCreateHost  Whether to create new Host information for the header
        *                     (By default, if there is a Host message, it is not created)
        *                     (Note that it was created at encode time)
        */
   void setHeadRequest(const string &sUrl, bool bNewCreateHost = false);

    /**
     * @brief 设置POST请求包(采用string方式).
     * @brief Set the POST request package (string mode).
     *  
     * @param sUrl        例如:http://www.qq.com/query
     * @param sUrl        for example: http://www.qq.com/query
     * @param sPostBody   如果requestType是GET, 则sPostBody无效
     * @param sPostBody   sPostBody is invalid if requestType is GET
     * @param bCreateHost 是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     * @param bCreateHost Whether to create new Host information for the header
     *                    (By default, if there is a Host message, it is not created)
     *                    (Note that it was created at encode time)
     */
    void setPostRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost = false);

    /**
     * @brief 设置POST请求包(采用vector<char>方式).
     * @brief Set up the POST request package (in vector<char>mode).
     *  
     * @param sUrl        例如:http://www.qq.com/query
     * @param sUrl        for example: http://www.qq.com/query
     * @param sPostBody   如果requestType是GET, 则sPostBody无效
     * @param sPostBody   sPostBody is invalid if requestType is GET
     * @param bCreateHost 是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     * @param bCreateHost Whether to create new Host information for the header
     *                    (By default, if there is a Host message, it is not created)
     *                    (Note that it was created at encode time)
     */
    void setPostRequest(const string &sUrl, const char *sBuffer, size_t iLength, bool bNewCreateHost = false);

    void setPutRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost = false);
    void setPatchRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost = false);

    void setDeleteRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost = false);

    /**
     * @brief 设置Get请求包.
     * @brief Set Request Package of Get 
     *  
     * @param sUrl         例如:http://www.qq.com/query?a=b&c=d
     * @param sUrl         for example: http://www.qq.com/query?a=b&c=d
     * @param bCreateHost  是否新创建头部的Host信息
     *                     (默认, 如果有Host信息了, 就不创建)
     *                     (注意, 是在encode的时候创建的)
     * @param bCreateHost  Whether to create new Host information for the header
     *                     (By default, if there is a Host message, it is not created)
     *                     (Note that it was created at encode time)
     */
    void setOptionsRequest(const string &sUrl, bool bNewCreateHost = false);

    /**
     * @brief 获取url里面的地址和端口.
     * @brief Get the address and port inside the url.
     *  
     * @param sHost
     * @param iPort
     */
    void getHostPort(string &sDomain, uint32_t &iPort);

    /**
     * @brief 发送HTTP请求.
     * @brief Send HTTP Request
     *  
     * @param iTimeout 毫秒
     * @param iTimeout Millisecond
     * @return        0 成功；
     *               <0失败, 具体值参见TC_ClientSocket声明
     * @return        0 success；
     *               <0 failed, see TC_ClientSocket statement for value
     */
    int doRequest(TC_HttpResponse &stHttpRep, int iTimeout = 3000);

    /**
     * 复用socket
     * Multiplex socket
     * @param client
     * @param stHttpRsp
     * @return
     */
    int doRequest(TC_TCPClient &client, TC_HttpResponse& stHttpRsp);

    /**
     * @brief get request type
     */
    int requestType() const { return _requestType ; }

	/**
	 * @brief set request type
	 */
	void setRequestType(int requestType) {  _requestType = requestType ; }

	/**
	 * get method
	 * @return
	 */
	const string &getMethod() const;

	/**
	 * set method
	 * @param
	 * @return method invalid, throw exception
	 */
	void setMethod(const char * sMethod);

	/**
	 * set method
	 * @param
	 */
	void setPath(const char * sPath);

	/**
	 * set domain
	 * @param
	 */
	void setDomain(const char * sDomain);

	/**
	 * set schema
	 * @param
	 */
	void setScheme(const char * sScheme);

    /**
     * @brief 是否是GET请求.
     * @brief Is it a get request
     *
     * @return 是GET请求返回true，否则返回false
     * @return True is returned for get request, false is returned otherwise
     */
    bool isGET() const { return _requestType == REQUEST_GET; }

    /**
     * @brief 是否是HEAD请求.
     * @brief Is it a head request
     *
     * @return 是HEAD请求返回true，否则返回false
     * @return True is returned for head request, otherwise false
     */
    bool isHEAD() const { return _requestType == REQUEST_HEAD; }

    /**
     * @brief 是否是POST请求.
     * @brief Is it a post request
     *
     * @return 是GET请求返回true，否则返回false
     * @return True is returned for get request, false is returned otherwise
     */
    bool isPOST() const { return _requestType == REQUEST_POST; }

    /**
     * @brief 是否是OPTIONS请求.
     * @brief Is it an options request
     *
     * @return 是GET请求返回true，否则返回false
     * @return True is returned for get request, false is returned otherwise
     */
    bool isOPTIONS() const { return _requestType == REQUEST_OPTIONS; }

    /**
     * @brief 是否是Delete请求.
     * @brief Whether it is a delete request
     *
     * @return 是delete请求返回true，否则返回false
     * @return True is returned for delete request, false is returned otherwise
     */
    bool isDELETE() const { return _requestType == REQUEST_DELETE; }
    /**
     * @brief 获取请求的URL.
     * @brief Get the requested URL.
     *
     * @return const TC_URL&
     */
    const TC_URL &getURL() const { return _httpURL; }

    /**
     * @brief 获取完整的http请求.
     * @brief Get the complete HTTP request
     *
     * @return http请求串
     * @return HTTP request string
     */
    string getOriginRequest() const { return _httpURL.getURL(); }

    /**
     * @brief 获取http请求的url，不包括Host,
     *        例如http://www.qq.com/abc?a=b, 则为:/abc?a=b
     * @brief Get the URL of the HTTP request, excluding the host,
     *        for: http://www.qq.com/abc?a=b, it is :/abc?a=b
     * @return http请求的url
     * @return URL of HTTP request
     */
    string getRequest() const { return _httpURL.getRequest(); }

    /**
     * @brief 获取http请求的url部分, 即?前面，不包括Host,
     *        例如http://www.qq.com/abc?a=b#def, 则为:/abc
     * @brief Get the URL part of the HTTP request, that is, before, excluding the host,
     *        for 'http://www.qq.com/abc?a=b#def' it is :'/abc'
     * @return http请求的url部分
     * @return URL part of HTTP request
     * */
    string getRequestUrl() const { return _httpURL.getPath(); }

    /**
     * @brief 获取http请求url后的参数部分，即?后面#前面部分不
     *        包括Host, 例如http://www.qq.com/abc?a=b#def, 则为:a=b
     * @brief Get the parameter part after the HTTP request url, i.e. Host is not included before ? after #,
     *        for example: Http://www.qq.com/abc?A=b#def  Is: a=b
     * @return http请求url后的参数部分
     * @return Parameter section after HTTP request URL
     */
    string getRequestParam() const { return _httpURL.getQuery(); }

    /**
     * @brief 解析请求头部.
     * @brief Resolve Request Header.
     *
     * @param szBuffer 请求头部
     * @param szBuffer Request Header
     * @return
     */
    void parseRequestHeader(const char* szBuffer, const char *header);


protected:


    /**
     * @brief 解析URL
     * @brief Resolve URL
     */
    void parseURL(const string& sUrl);

protected:

    /**
     * 请求URL
     * Request URL
     */
    TC_URL             _httpURL;

    /**
     * 请求类型
     * Request Type
     */
    int                _requestType;
};


}
#endif

