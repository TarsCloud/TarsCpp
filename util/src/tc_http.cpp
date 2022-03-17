#include "util/tc_http.h"
#include "util/tc_port.h"
#include "util/tc_common.h"
#include "util/tc_clientsocket.h"
#include "util/tc_network_buffer.h"
#include <string.h>

namespace tars
{

	const char* strnstr(const char* s1, const char* s2, int pos1)
	{
		int l1, l2;

		l2 = strlen(s2);
		if (!l2)
			return (char *)s1;
		l1 = strlen(s1);

		pos1 = (pos1 > l1)?l1:pos1;

		while (pos1 >= l2) {
			pos1--;
			if (!memcmp(s1, s2, l2))
				return (char *)s1;
			s1++;
		}
		return NULL;
	}

	unordered_map<string, int> TC_Http::HEADER = {
			{"GET", TC_HttpRequest::REQUEST_GET},
			{"POST", TC_HttpRequest::REQUEST_POST},
			{"PUT", TC_HttpRequest::REQUEST_PUT},
			{"PATCH", TC_HttpRequest::REQUEST_PATCH},
			{"OPTIONS", TC_HttpRequest::REQUEST_OPTIONS},
			{"PRI", TC_HttpRequest::REQUEST_PRI},
			{"DELETE", TC_HttpRequest::REQUEST_DELETE},
			{"HEAD", TC_HttpRequest::REQUEST_HEAD},
	};

	unordered_map<int, string> TC_Http::HEADER_REVERSE = {
			{TC_HttpRequest::REQUEST_GET, "GET"},
			{TC_HttpRequest::REQUEST_POST, "POST"},
			{TC_HttpRequest::REQUEST_PUT, "PUT"},
			{TC_HttpRequest::REQUEST_PATCH, "PATCH"},
			{TC_HttpRequest::REQUEST_OPTIONS, "OPTIONS"},
			{TC_HttpRequest::REQUEST_PRI, "PRI"},
			{TC_HttpRequest::REQUEST_DELETE, "DELETE"},
			{TC_HttpRequest::REQUEST_HEAD, "HEAD"},
	};


	bool TC_Http::CmpCase::operator()(const string &s1, const string &s2) const
	{
		if (TC_Port::strcasecmp(s1.c_str(), s2.c_str()) < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool TC_URL::isValid() const
	{
		return !_sURL.empty();
	}

	string TC_URL::getURL() const
	{
		return _sURL;
	}

	string TC_URL::type2String() const
	{
		switch (_iURLType)
		{
		case HTTP:
			return "http";
		case HTTPS:
			return "https";
		case FTP:
			return "ftp";
		}

		return "http";
	}

	string TC_URL::getDefaultPort() const
	{
		switch (_iURLType)
		{
		case HTTP:
			return "80";
		case HTTPS:
			return "443";
		case FTP:
			return "21";
		}

		return "80";
	}

	bool TC_URL::isDefaultPort() const
	{
		return _sPort == getDefaultPort();
	}

	string TC_URL::toURL()
	{
		_sURL.clear();

		_sURL = _sScheme;
		_sURL += "://";

		if (!_sUser.empty())
			_sURL += _sUser;

		if (!_sUser.empty() && !_sPass.empty())
		{
			_sURL += ":";
			_sURL += _sPass;
		}

		if (!_sUser.empty())
			_sURL += "@";

		_sURL += _sDomain;

		if (!isDefaultPort())
		{
			_sURL += ":";
			_sURL += _sPort;
		}

		_sURL += getRequest();

		return _sURL;
	}

	string TC_URL::getRequest() const
	{
		string sURL;

		if (!_sPath.empty())
			sURL += _sPath;

		if (!_sQuery.empty())
			sURL += "?" + _sQuery;

		if (!_sRef.empty())
			sURL += "#" + _sRef;

		return sURL;
	}

	bool TC_URL::parseURL(const string &originRequest)
	{
		if (originRequest.empty())
		{
			return false;
		}

		clear();

		int iPos = 0;

		if (TC_Port::strncasecmp(originRequest.c_str(), "http://" , 7) == 0)
		{
			//http开头
			_iURLType  = HTTP;
			iPos = 7;
			_sScheme = "http";
		}
		else if (TC_Port::strncasecmp(originRequest.c_str(), "https://" , 8) == 0)
		{
			//https开头
			_iURLType  = HTTPS;
			iPos = 8;
			_sScheme = "https";
		}
		else if (TC_Port::strncasecmp(originRequest.c_str(), "ftp://", 6) == 0)
		{
			//ftps开头
			_iURLType  = FTP;
			iPos = 6;
			_sScheme = "ftp";
		}
		else
		{
			//默认用http
			_iURLType  = HTTP;
			iPos = 0;
			_sScheme = "http";
		}

		string::size_type index            = originRequest.find("/", iPos);
		string::size_type nQuestionIndex   = originRequest.find("?", iPos);
		string::size_type nNumbersignIndex = originRequest.find("#", iPos);

		string  sUrlAuthority;
		string  sUrlPath;

		if (nQuestionIndex < index)
		{
			sUrlAuthority = originRequest.substr(iPos, nQuestionIndex - iPos);

			sUrlPath += '/';
			sUrlPath += originRequest.substr(nQuestionIndex);
		}
		else if (nNumbersignIndex < index)
		{
			sUrlAuthority = originRequest.substr(iPos, nNumbersignIndex - iPos);

			sUrlPath += '/';
			sUrlPath += originRequest.substr(nNumbersignIndex);
		}
		else
		{
			if (index == string::npos)
			{
				sUrlAuthority = originRequest.substr(iPos, index);
				sUrlPath = "";
			}
			else
			{
				sUrlAuthority = originRequest.substr(iPos, index - iPos);
				sUrlPath = originRequest.substr(index);
			}
		}

		//////解析Authority
		index = sUrlAuthority.find("@");
		if (index != string::npos)
		{
			_sUser = sUrlAuthority.substr(0, index);
			_sDomain = sUrlAuthority.substr(index + 1);
		}
		else
		{
			_sDomain    = sUrlAuthority;
		}

		//////解析User:Pass
		index = _sUser.find(":");
		if (index != string::npos)
		{
			_sPass = _sUser.substr(index + 1);
			_sUser = _sUser.substr(0, index);
		}

		//////解析Host:Port
		index = _sDomain.find(":");
		if (index != string::npos)
		{
			_sPort = _sDomain.substr(index + 1);

			_sDomain = _sDomain.substr(0, index);
		}
		else
		{
			_sPort      = getDefaultPort();
		}

		//////解析Path Query Ref
		index = sUrlPath.find("?");
		if (index != string::npos)
		{
			_sPath = sUrlPath.substr(0, index);
			_sQuery = sUrlPath.substr(index + 1);

			index = _sQuery.rfind("#");
			if (index != string::npos)
			{
				_sRef = _sQuery.substr(index + 1);
				_sQuery = _sQuery.substr(0, index);
			}
		}
		else
		{
			_sPath      = sUrlPath;
			_sQuery     = "";

			index = _sPath.rfind("#");
			if (index != string::npos)
			{
				_sRef = _sPath.substr(index + 1);
				_sPath = _sPath.substr(0, index);
			}
		}

		if (_sPath.empty())
		{
			_sPath = "/";
		}

		toURL();

		if(_sPort == "0")
		{
			//unix local套接字模式
			_sDomain = TC_Common::replace(_sDomain, "$", FILE_SEP);
			return true;
		}

		//域名或者IP必须包含一个点
		if (_sDomain.find(".") == string::npos)
		{
			return false;
		}

		return true;
	}

	void TC_URL::specialize()
	{
		//规整化路径
		/*string sUrlPath = simplePath(getPath());

		_fragment["path"]   = sUrlPath;*/
		_sPath = simplePath(getPath());

		_sURL = toURL();
	}

	void TC_URL::clear()
	{
		_sScheme = "";
		_sUser = "";
		_sPass = "";
		_sDomain = "";
		_sPort = "";
		_sPath = "";
		_sQuery = "";
		_sRef = "";

		_sURL.clear();
	}

	string TC_URL::getScheme() const
	{
		return _sScheme;
	}

	string TC_URL::getUserName() const
	{
		return _sUser;
	}

	string TC_URL::getPassword() const
	{
		return _sPass;
	}

	string TC_URL::getDomain() const
	{
		return _sDomain;
	}

	string TC_URL::getPort() const
	{
		return _sPort;
	}

	string TC_URL::getPath() const
	{
		return _sPath;
	}

	string TC_URL::getQuery() const
	{
		return _sQuery;
	}

	string TC_URL::getRef() const
	{
		return _sRef;
	}

	string TC_URL::getRelativePath() const
	{
		string sURL = getPath();

		string::size_type pos;


		pos   = sURL.rfind("/");

		if (pos == string::npos)
		{
			return "/";
		}
		else
		{
			return sURL.substr(0, pos + 1);
		}
	}

	string TC_URL::getRootPath() const
	{
		string sURL = _sScheme;
		sURL += "://";

		if (!_sUser.empty())
			sURL += _sUser;

		if (!_sUser.empty() && !_sPass.empty())
		{
			sURL += ":";
			sURL += _sPass;
		}

		if (!_sUser.empty())
			sURL += "@";

		sURL += _sDomain;

		if (!isDefaultPort())
		{
			sURL += ":";
			sURL += _sPort;
		}

		sURL += "/";

		return sURL;
	}

	TC_URL TC_URL::buildWithRelativePath(const string &sRelativePath) const
	{
		string sURL;

		if (!sRelativePath.empty() && sRelativePath[0] == '/')
		{
			//如果链接是用"/"开头的相对地址，那么应该用Host+相对地址
			sURL = sRelativePath.substr(1);
		}
		else if (sRelativePath[0] == '#')
		{
			//#
			sURL = getPath().substr(1);

			if (!getQuery().empty())
				sURL += "?" + getQuery();

			sURL += sRelativePath;

		}
		else
		{
			//相对地址
			sURL = getRelativePath().substr(1) + sRelativePath;
		}

		sURL = getRootPath() + simplePath("/" + sURL).substr(1);

		TC_URL url;

		url.parseURL(sURL);

		return url;
	}

	string TC_URL::simplePath(const string &sPath) const
	{
		//所有./都去掉
		size_t pos      = 0;
		string sNewPath = sPath;

		while (true)
		{
			size_t dotPos = sNewPath.find("./", pos);

			if (dotPos != string::npos)
			{
				if ((dotPos == 0) || (sNewPath.at(dotPos - 1) == '/'))
				{
					sNewPath.erase(dotPos, 2);
				}
				else
				{
					pos = dotPos + 2;
				}
			}
			else
			{
				break;
			}
		}

		//如果路径是以.结尾的, 则.去掉
		if (((sNewPath.length() >= 2) && (sNewPath.substr(sNewPath.length() - 2) == "/.")) || (sNewPath == "."))
		{
			sNewPath.erase(sNewPath.length() - 1);
		}

		//处理/../的形式
		pos = 0;
		size_t startPos = 0;

		while (1)
		{
			size_t slashDot = sNewPath.find("/../", pos);

			if (slashDot != string::npos)
			{
				if (0 == slashDot)
				{
					sNewPath.erase(0, 3);
					continue;
				}

				if ( (slashDot > 1) && (sNewPath.substr(slashDot - 2, 2) == "..") )
				{
					pos = slashDot + 4;
					continue;
				}

				startPos = sNewPath.rfind('/', slashDot - 1);

				if (startPos == string::npos) startPos = 0;

				sNewPath.erase(startPos, slashDot + 4 - startPos - 1);
			}
			else
			{
				break;
			}
		}

		//处理/..结尾的情况
		if ((sNewPath.size() >= 3) && (sNewPath.substr(sNewPath.size() - 3, 3) == "/.."))
		{
			size_t slashDot = sNewPath.size() - 3;
			if (!((slashDot > 1) && (sNewPath.substr(slashDot - 2, 2) == "..")))
			{
				startPos = sNewPath.rfind ('/', slashDot - 1);
				if (startPos == string::npos) startPos = 0;
				sNewPath.erase (startPos + 1);
			}
		}

		return sNewPath;
	}

////////////////////////////////////////////////////////////////////

	void TC_Http::setHeader(const string &sHeadName, const string &sHeadValue)
	{
		//Set-Cookie和Cookie可以有多个头
		const char * pStr1 = "SET-COOKIE";
		const char * pStr2 = "COOKIE";//原则上COOKIE只有一个，担心有兼容性问题，保留
		if ((TC_Port::strcasecmp(sHeadName.c_str(), pStr1) != 0) && (TC_Port::strcasecmp(sHeadName.c_str(), pStr2) != 0))
		{
			_headers.erase(sHeadName);
		}

		_headers.insert(multimap<string, string>::value_type(sHeadName, sHeadValue));
	}

	bool TC_Http::hasHeader(const char *sHeader) const
	{
		http_header_type::const_iterator it = _headers.find(sHeader);
		if (it == _headers.end())
		{
			return false;
		}

		return true;
	}

	bool TC_Http::checkHeader(const char *sHeader, const char *value) const
	{
		http_header_type::const_iterator it = _headers.find(sHeader);
		if (it == _headers.end())
		{
			return false;
		}

		return TC_Port::strcasecmp(it->second.c_str(), value) == 0;
	}

	string TC_Http::getHeader(const string& sHeader) const
	{
		http_header_type::const_iterator it = _headers.find(sHeader);
		if (it == _headers.end())
		{
			return "";
		}

		return it->second;
	}

	string TC_Http::getContentType() const
	{
		return getHeader("Content-Type");
	}

	string TC_Http::getHost() const
	{
		return getHeader("Host");
	}

	size_t TC_Http::getContentLength() const
	{
		string s = getHeader("Content-Length");
		if (s.empty())
		{
			return 0;
		}

		return TC_Common::strto<size_t>(s);
	}

	string TC_Http::genHeader() const
	{
		string sHttpHeader;

		for (http_header_type::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		{
			if (it->second != "")
			{
				sHttpHeader += it->first;
				sHttpHeader += ": ";
				sHttpHeader += it->second;
				sHttpHeader += "\r\n";
			}
		}

		return sHttpHeader;
	}

	void TC_Http::genHeader(string &sHttpHeader) const
	{
		for (http_header_type::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		{
			if (it->second != "")
			{
				sHttpHeader += it->first;
				sHttpHeader += ": ";
				sHttpHeader += it->second;
				sHttpHeader += "\r\n";
			}
		}
	}

	vector<string> TC_Http::getHeaderMulti(const string &sHeadName) const
	{
		vector<string> v;

		http_header_type::const_iterator itEnd = _headers.end();

		for ( http_header_type::const_iterator it = _headers.begin(); it != itEnd; ++it)
		{
			if (TC_Port::strcasecmp(it->first.c_str(), sHeadName.c_str()) == 0)
			{
				v.push_back(it->second);
			}
		}

		return v;
	}

	void TC_Http::reset()
	{
		_headers.clear();
		_version.clear();
		_headLength = 0;
		_content.clear();
		_bIsChunked = false;
		_headComplete = false;
	}

	void TC_Http::getHeaders(map<string, string> &header) const
	{
		for(auto it = _headers.begin(); it != _headers.end(); ++it)
		{
			header.insert(map<string, string>::value_type(it->first, it->second));
		}
	}

/********************* TC_HttpCookie ***********************/

	bool TC_HttpCookie::matchDomain(const string &sCookieDomain, const string &sDomain)
	{
		string sCookieDomainNew = TC_Common::lower(sCookieDomain);

		//没有点的自动加点
		if (sCookieDomainNew.find(".") != 0)
		{
			sCookieDomainNew = "." + sCookieDomainNew;
		}

		string::size_type pos = sCookieDomainNew.find(".");

		//sCookieDomain串至少有两个点
		if (pos == string::npos || (pos == 0 && sCookieDomainNew.rfind(".") == 0))
		{
			return false;
		}

		string sLowerDomain = TC_Common::lower(sDomain);

		//后边是子域名
		if (sDomain.length() >= sCookieDomainNew.length() &&
			sLowerDomain.compare(sDomain.length() - sCookieDomainNew.length(), sCookieDomainNew.length(), sCookieDomainNew) == 0)
		{
			return true;
		}

		//去掉.相等
		if (sLowerDomain == sCookieDomainNew.substr(1))
		{
			return true;
		}

		return false;
	}

	size_t TC_HttpCookie::matchPath(const string &sCookiePath, const string &sPath)
	{
		if (sCookiePath.empty() || sPath.empty()) return 0;

		//都在最后加/再匹配
		string sCookiePath1 = (sCookiePath.at(sCookiePath.length() - 1) == '/') ? sCookiePath : sCookiePath + "/";

		string sPath1 = (sPath.at(sPath.length() - 1) == '/') ? sPath : sPath + "/";

		if (sPath1.find(sCookiePath1) == 0)
		{
			return sCookiePath1.length();
		}

		return 0;
	}

	void TC_HttpCookie::clear()
	{
		_cookies.clear();
	}

	bool TC_HttpCookie::fixDomain(const string &sDomain, string &sFixDomain)
	{
		if (sDomain.empty())
		{
			return false;
		}

		sFixDomain = sDomain;

		//自动加.
		if (sDomain.at(0) != '.')
		{
			sFixDomain = "." + sDomain;
		}

		//domain至少两段
		if (sFixDomain.find(".") == sFixDomain.rfind("."))
			return false;

		return true;
	}

	void TC_HttpCookie::addCookie(const Cookie &cookie, list<Cookie> &cookies)
	{
		string sDomain;

		Cookie cookieNew = cookie;

		if (!fixDomain(cookieNew._domain, sDomain))
			return;

		cookieNew._domain = sDomain;

		if (cookieNew._path.empty())
			return;

		list<Cookie>::iterator it = cookies.begin();

		string sName;
		if (cookieNew._data.size() >= 1)
		{
			sName = cookieNew._data.begin()->first;
		}

		while (it != cookies.end())
		{
			//检查Cookie是否过期
			if (isCookieExpires(*it))
			{
				cookies.erase(it++);
			}
			else if (TC_Port::strcasecmp(it->_domain.c_str(), cookieNew._domain.c_str()) == 0
					 && strcmp(it->_path.c_str(), cookieNew._path.c_str()) == 0
					 && it->_isSecure == cookieNew._isSecure)
			{
				if (it->_expires == cookieNew._expires)
				{
					//domain/path/expires都匹配的情况下, 覆盖老cookie的数据
					cookieNew._data.insert(it->_data.begin(), it->_data.end());

					cookies.erase(it++);
				}
				else
				{
					//超时间不一样,但存在同样的key，需删除
					if (it->_data.find(sName) != it->_data.end())
					{
						it->_data.erase(sName);
					}
					++it;
				}
			}
			else
			{
				++it;
			}
		}

		cookies.push_back(cookieNew);
	}

	void TC_HttpCookie::addCookie(const Cookie &cookie)
	{
		addCookie(cookie, _cookies);
	}

	void TC_HttpCookie::addCookie(const list<Cookie> &cookie)
	{
		list<Cookie>::const_iterator it = cookie.begin();

		while (it != cookie.end())
		{
			addCookie(*it);

			++it;
		}
	}

	void TC_HttpCookie::addCookie(const string &sRspURL, const vector<string> &vCookies)
	{
		TC_URL cURL;

		cURL.parseURL(sRspURL);

		string sRspURLDomain = TC_Common::lower(cURL.getDomain());

		string sRspURLPath  = cURL.getPath();

		for (size_t i = 0; i < vCookies.size(); i++)
		{
			//处理一行SetCookie
			vector<string> v = TC_Common::sepstr<string>(vCookies[i], ";");

			Cookie cookie;

			cookie._isSecure = false;
			cookie._expires  = 0;

			//解析Cookie数据
			for (size_t j = 0; j < v.size(); ++j)
			{
				string::size_type index = v[j].find("=");

				string name;

				string value;

				if (index != string::npos)
				{
					name  = TC_Common::trim(v[j].substr(0, index), " ");

					value = TC_Common::trim(v[j].substr(index + 1));
				}
				else
				{
					name  = TC_Common::trim(v[j]);
				}
				if (TC_Port::strcasecmp(name.c_str(), "secure") == 0)
				{
					cookie._isSecure = true;
				}
				else if (TC_Port::strcasecmp(name.c_str(), "expires") == 0)
				{
					struct tm stTm;
					//兼容时间格式
					//expires=Mon, 09-May-41 08:39:32 GMT
					//expires=Thu, 01-Jan-1970 01:00:00 GMT
					value = TC_Common::replace(value, "-", " ");
					if (value.length() == 27  && value.at(11) == ' ' && value.at(14) == ' ')
					{
						int year = TC_Common::strto<int> (value.substr(12, 2));
						if (year >= 69 && year <= 99)
							value = value.substr(0, 12) + "19" + value.substr(12);
						else
							value = value.substr(0, 12) + "20" + value.substr(12);
					}

					TC_Common::strgmt2tm(value, stTm);

					cookie._expires = TC_Port::timegm(&stTm);
				}
				else if (TC_Port::strcasecmp(name.c_str(), "path") == 0)
				{
					cookie._path = value;
				}
				else if (TC_Port::strcasecmp(name.c_str(), "domain") == 0)
				{
					cookie._domain = value;
				}
				else if (TC_Port::strcasecmp(name.c_str(), "httponly") == 0)
				{
					//TODO
					//cookie._isHttpOnly = true;
				}
				else
					cookie._data.insert(http_cookie_data::value_type(name, value));
			}

			///修正和匹配domain/////////////////////////////////////////
			if (cookie._domain.empty())
				cookie._domain = sRspURLDomain;

			if (!fixDomain(cookie._domain, cookie._domain))
				continue;

			//匹配域名
			if (!matchDomain(cookie._domain, sRspURLDomain))
				continue;

			//修改和匹配path/////////////////////////////
			if (cookie._path.empty())
			{
				string sCookiePath;

				//缺省是全路径
				string sRequest = sRspURLPath;

				string::size_type pos = sRequest.rfind("/");

				if (pos == string::npos)
					sCookiePath = "/";
				else
					sCookiePath = sRequest.substr(0, pos + 1);

				cookie._path = sCookiePath;
			}

			//URL在Path范围内,Cookie 有效
			if (!matchPath(cookie._path, sRspURLPath))
				continue;

			//添加Cookie
			addCookie(cookie);
		}
	}

	bool TC_HttpCookie::isCookieExpires(const Cookie &cookie) const
	{
		//过期了
		if (cookie._expires != 0 && cookie._expires < time(NULL))
			return true;

		return false;
	}

	size_t TC_HttpCookie::isCookieMatch(const Cookie &cookie, const TC_URL &tURL) const
	{
		//域名没有匹配
		if (!matchDomain(cookie._domain, tURL.getDomain()))
			return 0;

		//路径没有匹配
		size_t len = matchPath(cookie._path, tURL.getPath());
		if (len == 0)
			return 0;

		//安全的cookie,不安全的URL
		if (cookie._isSecure && (tURL.getType() != TC_URL::HTTPS))
			return 0;

		return len;
	}

	void TC_HttpCookie::getCookieForURL(const string &sReqURL, list<TC_HttpCookie::Cookie> &cookies)
	{
		TC_URL tURL;

		tURL.parseURL(sReqURL);

		cookies.clear();

		list<Cookie>::iterator it = _cookies.begin();

		while (it != _cookies.end())
		{
			//检查Cookie是否过期
			if (isCookieExpires(*it))
			{
				_cookies.erase(it++);
				continue;
			}

			size_t len = isCookieMatch(*it, tURL);
			if (len == 0)
			{
				++it;
				continue;
			}

			cookies.push_back(*it);

			++it;
		}
	}

	void TC_HttpCookie::getCookieForURL(const string &sReqURL, string &sCookie)
	{
		list<Cookie> cookies;

		sCookie.clear();

		getCookieForURL(sReqURL, cookies);

		list<Cookie>::iterator it = cookies.begin();
		while (it != cookies.end())
		{
			http_cookie_data::iterator itd = it->_data.begin();

			while (itd != it->_data.end())
			{
				//被删除的cookie不输出
				if (itd->first != "" && itd->second != "" && TC_Common::lower(itd->second) != "null"
					&& TC_Common::lower(itd->second) != "deleted")
					sCookie += itd->first + "=" + itd->second + "; ";

				++itd;
			}

			++it;
		}

		//去掉末尾的 "; "
		if (sCookie.length() >= 2)
			sCookie = sCookie.substr(0, sCookie.length() - 2);
	}

	list<TC_HttpCookie::Cookie> TC_HttpCookie::getSerializeCookie(time_t t)
	{
		list<Cookie> cookies;

		list<Cookie>::iterator it = _cookies.begin();

		while (it != _cookies.end())
		{
			if (isCookieExpires(*it))
			{
				_cookies.erase(it++);
				continue;
			}
			else if (it->_expires != 0) //非当前会话的
			{
				cookies.push_back(*it);

				++it;
			}
			else
				++it;
		}

		return cookies;
	}

	const list<TC_HttpCookie::Cookie> & TC_HttpCookie::getAllCookie()
	{
		deleteExpires(time(NULL));

		return _cookies;
	}

	void TC_HttpCookie::deleteExpires(time_t t, bool bErase)
	{
		list<Cookie>::iterator it = _cookies.begin();

		while (it != _cookies.end())
		{
			if (bErase && it->_expires == 0)
			{
				_cookies.erase(it++);
				continue;
			}
			else if (isCookieExpires(*it))
			{
				_cookies.erase(it++);
				continue;
			}
			else
				++it;
		}
	}

/********************* TC_HttpResponse ***********************/
	void TC_HttpResponse::reset()
	{
		TC_Http::reset();

		_status = 200;
		_about  = "OK";
		_version = "HTTP/1.1";

		_iTmpContentLength = 0;

		_iRecvContentLength = 0;
	}

	vector<string> TC_HttpResponse::getSetCookie() const
	{
		return getHeaderMulti("Set-Cookie");
	}

	void TC_HttpResponse::addContent(const string &sBuffer)
	{
		_content += sBuffer;

		_iRecvContentLength += sBuffer.length();
	}

	void TC_HttpResponse::addContent(const char *buffer, size_t length)
	{
		_content.append(buffer, length);

		_iRecvContentLength += length;
	}

	size_t TC_HttpResponse::parseResponseHeaderString(const char *beginIt, const char *headerIt)
	{
		auto it = strnstr(beginIt, "\r\n", headerIt - beginIt);

		assert(it != NULL);

		auto f1 = strnstr(beginIt, " ", it - beginIt);
		if(f1 == NULL)
		{
			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response parse version format error : " + string(beginIt, headerIt - beginIt));
		}

		auto f2 = strnstr(f1 + 1, " ", it - (f1 + 1));
		if(f1 == NULL)
		{
			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response parse status format error : " + string(beginIt, headerIt - beginIt));
		}

		_headerLine = string(beginIt, it - beginIt);

		if(TC_Port::strncasecmp(_headerLine.c_str(), "HTTP/", 5) != 0)
		{
			throw TC_HttpResponse_Exception("[TC_HttpResponse_Exception::parseResponeHeader] http response version is not start with 'HTTP/' : " + _headerLine);
		}

		_version    = string(beginIt, f1 - beginIt);

		_status     = TC_Common::strto<int>(string(f1 + 1, f2));

		_about      = string(f2 + 1, it);

		_headComplete = true;

		return parseHeaderString(beginIt, headerIt, _headers);
	}

	bool TC_HttpResponse::incrementDecode(TC_NetWorkBuffer &buff)
	{
		if(buff.empty())
			return false;

		buff.mergeBuffers();

		size_t length = buff.getBufferLength();

		auto sBuf = buff.getBuffer();

		bool flag = incrementDecode(*sBuf.get());

		buff.subLength(length - sBuf->length());

		return flag;
	}

	bool TC_HttpResponse::incrementDecode(TC_NetWorkBuffer::Buffer &data)
	{
		if(data.empty())
			return false;
		//解析头部
		if (_headLength == 0)
		{
			const char * p = strnstr(data.buffer(), "\r\n\r\n", data.length());
			if(p == NULL)
			{
				return false;
			}

			_headLength = p - data.buffer() + 4;

			_iTmpContentLength = parseResponseHeaderString(data.buffer(), data.buffer() + _headLength);

			//304的返回码中头里本来就没有Content-Length，也不会有数据体，头收全了就是真正的收全了
			if ( (204 == _status) || (304 == _status) )
			{
				return true;
			}

			data.addReadIdx(_headLength);
//		buff.moveHeader(_headLength);

			//重定向就认为成功了
			if ((_status == 301 || _status == 302) && hasHeader("Location"))
			{
				return true;
			}

			if(_iTmpContentLength > 0)
			{
				data.expansion(_headLength + _iTmpContentLength);
			}

			//是否是chunk编码
			_bIsChunked = checkHeader("Transfer-Encoding", "chunked");
			if(_bIsChunked) {
				//删除头部里面
				eraseHeader("Transfer-Encoding");
			}
		}

		if (_bIsChunked)
		{
			while (true)
			{
				const static string sep = "\r\n";

				char* sit = std::search(data.buffer(), data.buffer() + data.length(), sep.c_str(), sep.c_str() + sep.size());

				if (sit == data.buffer() + data.length())
				{
					return false;
				}

//			string header = buff.iteratorToIterator<string>(buff.begin(), sit);

				int iChunkSize    = strtol(data.buffer(), &sit, 16);

				if (iChunkSize <= 0)
				{
					break;     //所有chunk都接收完毕
				}

				if (data.length() < sit - data.buffer() + 2 + (size_t)iChunkSize + 2)
				{
					//没有接收完整的chunk
					return false;
				}

				//接收到一个完整的chunk了
				data.addReadIdx(sit - data.buffer() + 2);
				addContent(data.buffer(), iChunkSize);
//			buff.moveHeader(header.size() + 2);
//			addContent(buff.getHeader<string>(iChunkSize));

				//删除一个chunk
				data.addReadIdx(iChunkSize+2);
//			buff.moveHeader(iChunkSize + 2);
			}

			data.clear();
//		buff.clearBuffers();

			//接收到buffer长度设置好
			setContentLength(_iRecvContentLength);

			return true;
		}
		else
		{
			if (_iTmpContentLength == 0)
			{
				//header长度为0, 但是有body数据
//			addContent(buff.getBuffersString());
				addContent(data.buffer(), data.length());

//			buff.clearBuffers();
				data.clear();

				if(_iRecvContentLength > 0) {
					setContentLength(_iRecvContentLength);
				}

				return true;
			}
			else if (_iTmpContentLength == (size_t) - 1)
			{
				//304的返回码中头没有Content-Length，不会有数据体
				if (_status == 304 || _status == 302)
				{
					return true;
				}

				//header中没长度, 但是有body数据
//			addContent(buff.getBuffersString());
				addContent(data.buffer(), data.length());

//			buff.clearBuffers();

				data.clear();
				if(_iRecvContentLength > 0) {
					setContentLength(_iRecvContentLength);
				}

//			http 收包这里收包并没有收完应该return false
				return false;
			}
			else
			{
				//头部有长度, 接收到长度大于头部为止
//			addContent(buff.getBuffersString());
//			buff.clearBuffers();
				addContent(data.buffer(), data.length());
				data.clear();

				//头部的长度小于接收的内容, 还需要继续增加解析后续的buffer
				if (_iTmpContentLength > _iRecvContentLength)
					return false;

				return true;
			}
		}

		return true;
	}

	bool TC_HttpResponse::decode(const string &sBuffer)
	{
		return decode(sBuffer.c_str(), sBuffer.length());
	}

	bool TC_HttpResponse::decode(const char *sBuffer, size_t iLength)
	{
		assert(sBuffer != NULL);

		const char *pHeader = sBuffer;

		const char *p = strnstr(sBuffer, "\r\n\r\n", iLength);
		if ( p == NULL)
		{
			return false;
		}

		//解析头部
		_iTmpContentLength = parseResponseHeaderString(sBuffer, p + 2);

		//304的返回码中头里本来就没有Content-Length，也不会有数据体，头收全了就是真正的收全了
		if ( (204 == _status) || (304 == _status) )
		{
			return true;
		}

		_headLength = p - sBuffer + 4;

		sBuffer += _headLength;

		//重定向就认为成功了
		if ((_status == 301 || _status == 302) && hasHeader("Location"))
		{
			return true;
		}

		//是否是chunk编码
		_bIsChunked = checkHeader("Transfer-Encoding", "chunked");
		if(_bIsChunked) {
			//删除头部里面
			eraseHeader("Transfer-Encoding");
		}

//	_bIsChunked = (getHeader("Transfer-Encoding") == "chunked");

		//删除头部里面
//    eraseHeader("Transfer-Encoding");

		if (_bIsChunked)
		{
			while (true)
			{
				p = strnstr(sBuffer, "\r\n", iLength - (sBuffer - pHeader));
				if (p == NULL)
					return false;

				size_t pos = p - sBuffer;

				//查找当前chunk的大小
				string sChunkSize;
				sChunkSize.insert(sChunkSize.end(), sBuffer, p);
				int iChunkSize    = strtol(sChunkSize.c_str(), NULL, 16);

				if (iChunkSize <= 0)     break;     //所有chunk都接收完毕

				if (strlen(sBuffer) < pos + 2 + (size_t)iChunkSize + 2)
				{
					//没有接收完整的chunk
					return false;
				}

				//接收到一个完整的chunk了
				addContent(p + 2, iChunkSize);

				//删除一个chunk
				sBuffer += pos + 2 + iChunkSize + 2;
			}

			//接收到buffer长度设置好
			setContentLength(_iRecvContentLength);

			return true;
		}
		else
		{
			if (_iTmpContentLength == 0)
			{
				//header长度为0, 但是有body数据
				addContent(sBuffer);

				setContentLength(_iRecvContentLength);

				return true;
			}
			else if (_iTmpContentLength == (size_t) - 1)
			{
				//304的返回码中头没有Content-Length，不会有数据体
				if (_status == 304 || _status == 302)
				{
					return true;
				}

				//header中没长度, 但是有body数据
				addContent(sBuffer);

				setContentLength(_iRecvContentLength);
			}
			else
			{
				//头部有长度, 接收到长度大于头部为止
				addContent(sBuffer);

				//头部的长度小于接收的内容, 还需要继续增加解析后续的buffer
				if (_iTmpContentLength <= _iRecvContentLength)
					return true;
			}
		}

		//body内容长度为0或者没有content-length  且 非chunk模式, 则认为包收全了, 直接返回
		if ((_iTmpContentLength == 0 || _iTmpContentLength == (size_t) - 1) && !_bIsChunked)
			return true;

		return (getContentLength() + getHeadLength() <= iLength);
	}

	string TC_HttpResponse::encode() const
	{
		string sRet;
		sRet.reserve(128);

		sRet += _version;
		sRet += " ";
		sRet += TC_Common::tostr(_status);
		sRet += " ";
		sRet += _about;
		sRet += "\r\n";
		genHeader(sRet);
		sRet += "\r\n";
		sRet += _content;

		return sRet;
	}

	void TC_HttpResponse::encode(vector<char> &buffer) const
	{
		buffer.clear();

		string s = encode();

		buffer.resize(s.length());
		memcpy(buffer.data(), s.c_str(), s.length());
	}

	void TC_HttpResponse::setResponse(int status, const string& about, const string& body)
	{
		_status = status;
		_about  = about;
		_content = body;

		/*stringstream os;

		os << _version << " " << _status << " " << _about;

		_headerLine = os.str();*/
		_headerLine = "";
		_headerLine += _version;
		_headerLine += " ";
		_headerLine += TC_Common::tostr(_status);
		_headerLine += " ";
		_headerLine += _about;

		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}

	void TC_HttpResponse::setResponse(int status, const string& about, const char *sBuffer, size_t iLength)
	{
		_status = status;
		_about  = about;

		/*stringstream os;

		os << _version << " " << _status << " " << _about;

		_headerLine = os.str();*/
		_headerLine = "";
		_headerLine += _version;
		_headerLine += " ";
		_headerLine += TC_Common::tostr(_status);
		_headerLine += " ";
		_headerLine += _about;

		if (sBuffer != NULL && iLength > 0)
		{
			_content.assign(sBuffer, iLength);
		}
		else
		{
			_content.clear();
		}


		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}

	void TC_HttpResponse::setResponse(const char *sBuffer, size_t iLength)
	{
		setResponse(200, "OK", sBuffer, iLength);
	}

/********************* TC_HttpRequest ***********************/

	void TC_HttpRequest::reset()
	{
		TC_Http::reset();

		_httpURL.clear();
	}

	vector<string> TC_HttpRequest::getCookie()
	{
		vector<string> v;

		http_header_type::const_iterator itEnd = _headers.end();

		for ( http_header_type::const_iterator it = _headers.begin(); it != itEnd; ++it)
		{
			if (it->first == "Cookie")
			{
				v.push_back(it->second);
			}
		}

		return v;
	}

	void TC_HttpRequest::parseURL(const string& sUrl)
	{
		_httpURL.parseURL(sUrl);

		//设置Host
		if (!hasHeader("Host"))
		{
			string sPort = _httpURL.isDefaultPort() ? "" : ":" + _httpURL.getPort();

			//缺省端口可以不用放进去
			setHost(_httpURL.getDomain() + sPort);
		}
	}

	void TC_HttpRequest::setRequest(const string& method, const string &sUrl, const std::string& body, bool bNewCreateHost)
	{
		if (TC_Port::strncasecmp(method.c_str(), "GET", 3) == 0)
			setGetRequest(sUrl, bNewCreateHost);
		else if (TC_Port::strncasecmp(method.c_str(), "HEAD", 4) == 0)
			setHeadRequest(sUrl, bNewCreateHost);
		else if (TC_Port::strncasecmp(method.c_str(),  "POST", 4) == 0)
			setPostRequest(sUrl, body, bNewCreateHost);
		else if (TC_Port::strncasecmp(method.c_str(),  "PUT", 3) == 0)
			setPutRequest(sUrl, body, bNewCreateHost);
		else if (TC_Port::strncasecmp(method.c_str(),  "DELETE", 6) == 0)
			setDeleteRequest(sUrl, body, bNewCreateHost);
		else if (TC_Port::strncasecmp(method.c_str(),  "PATH", 5) == 0)
			setPatchRequest(sUrl, body, bNewCreateHost);
	}

	void TC_HttpRequest::setGetRequest(const string &sUrl, bool bNewCreateHost)
	{
		if (bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_GET;

		_content        = "";

		eraseHeader("Content-Length");
	}

	void TC_HttpRequest::setHeadRequest(const string &sUrl, bool bNewCreateHost)
	{
		if (bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_HEAD;

		_content        = "";

		eraseHeader("Content-Length");
	}

	void TC_HttpRequest::setPostRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost)
	{
		if (bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_POST;

		_content        = sPostBody;

		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}

	void TC_HttpRequest::setOptionsRequest(const string &sUrl, bool bNewCreateHost)
	{
		if (bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_OPTIONS;

		_content        = "";

		eraseHeader("Content-Length");
	}

	void TC_HttpRequest::setPostRequest(const string &sUrl, const char *sBuffer, size_t iLength, bool bNewCreateHost)
	{
		assert(sBuffer != NULL);

		if (bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_POST;

		if (iLength > 0)
		{
			_content.assign(sBuffer, iLength);
		}
		else
		{
			_content.clear();
		}

		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}


	void TC_HttpRequest::setPutRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost )
	{
		if(bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_PUT;

		_content        = sPostBody;

		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}

	void TC_HttpRequest::setPatchRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost )
	{
		if(bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_PATCH;

		_content        = sPostBody;

		setHeader("Content-Length", TC_Common::tostr(_content.length()));
	}


	void TC_HttpRequest::setDeleteRequest(const string &sUrl, const string &sPostBody, bool bNewCreateHost )
	{
		if(bNewCreateHost)
		{
			eraseHeader("Host");
		}

		parseURL(sUrl);

		_requestType    = REQUEST_DELETE;

		_content        = sPostBody;

		setHeader("Content-Length", TC_Common::tostr(_content.length()));

	}

	string TC_HttpRequest::encode()
	{
		string sRet;
		sRet.reserve(128);
		if(_requestType == REQUEST_GET || _requestType == REQUEST_OPTIONS || _requestType == REQUEST_HEAD)
		{
			sRet += getMethod();//(_requestType);
			sRet += " ";
			sRet += _httpURL.getRequest();
			sRet += " HTTP/1.1\r\n";
			genHeader(sRet);
			sRet += "\r\n";
		}
		else if(_requestType == REQUEST_POST || _requestType == REQUEST_PUT || _requestType == REQUEST_PATCH || _requestType == REQUEST_DELETE)
		{
			setContentLength(_content.length());
			sRet += getMethod();//requestType2str(_requestType);
			sRet += " ";
			sRet += _httpURL.getRequest();
			sRet += " HTTP/1.1\r\n";
			genHeader(sRet);
			sRet += "\r\n";
			sRet += _content;
		}
		return sRet;
	}

	void TC_HttpRequest::encode(vector<char> &sRet)
	{
		string s = encode();
		sRet.resize(s.length());
		memcpy(sRet.data(), s.c_str(), s.length());
	}

	void TC_HttpRequest::encode(TC_NetWorkBuffer &buff)
	{
		buff.addBuffer(std::move(encode()));
	}

	void TC_HttpRequest::encode(shared_ptr<TC_NetWorkBuffer::Buffer>& buff)
	{
		buff->addBuffer(std::move(encode()));
	}

	bool TC_HttpRequest::decode(const string &sBuffer)
	{
		return decode(sBuffer.c_str(), sBuffer.length());
	}

	bool TC_HttpRequest::decode(const vector<char> &sBuffer)
	{
		return decode(sBuffer.data(), sBuffer.size());
	}

	bool TC_HttpRequest::decode(const char *sBuffer, size_t iLength)
	{
		assert(sBuffer != NULL);

		const char *p = strnstr(sBuffer, " ", 10);
		if(p == NULL)
		{
			throw runtime_error("[TC_HttpRequest::decode] http protocol parse error");
		}

		auto it = TC_Http::HEADER.find(string(sBuffer, p - sBuffer));
		if(it == TC_Http::HEADER.end())
		{
			throw runtime_error("[TC_HttpRequest::decode] protocol not support ");
		}

		p = strnstr(sBuffer, "\r\n\r\n", iLength);
		if (p == NULL)
		{
			return false;
		}

		_requestType = it->second;

		parseRequestHeader(sBuffer, p);

		_headLength = p - sBuffer + 4;

		bool bChunk = checkHeader("Transfer-Encoding", "chunked"); //(getHeader("Transfer-Encoding") == "chunked");

		int iChunkSuffixLen = 0;

		if (bChunk)
		{
			p = sBuffer + _headLength;
			while (true)
			{
				const char *pos  = strnstr(p, "\r\n", iLength - (p - sBuffer));

//	        const char *pos = strstr(p, "\r\n");
				if (pos == NULL)
					return false;

				//查找当前chunk的大小
				string sChunkSize(p, pos - p);
				int iChunkSize    = strtol(sChunkSize.c_str(), NULL, 16);

				iChunkSuffixLen = iChunkSuffixLen + sChunkSize.length();
				if (iChunkSize <= 0)
				{
					iChunkSuffixLen = iChunkSuffixLen + 4;
					break;      //所有chunk都接收完毕
				}
				if ((sBuffer+iLength-p) >= (pos - p) + 2 + iChunkSize + 2)  //接收到一个完整的chunk了
				{
					//获取一个chunk的内容
					_content.append(pos + 2, iChunkSize);

					//删除一个chunk
					p = pos + 2 + iChunkSize + 2;
					iChunkSuffixLen = iChunkSuffixLen + 4;
				}
				else
				{
					//没有接收完整的chunk
					return false;
				}

				setContentLength(getContent().length());
			}
		}
		else
		{
			_content.assign((sBuffer + _headLength), iLength - _headLength);
		}

		return (getContentLength() + getHeadLength() + iChunkSuffixLen == iLength);
	}

	bool TC_HttpRequest::checkRequest(TC_NetWorkBuffer &buff)
	{
		buff.mergeBuffers();

		return checkRequest(buff.getBufferPointer().first, buff.getBufferLength());
	}

	bool TC_HttpRequest::checkRequest(const char* sBuffer, size_t iLen)
	{
		if(iLen < 10)
			return false;

		const char *p = strnstr(sBuffer, " ", 10);
		if(p == NULL)
		{
			throw runtime_error("[TC_HttpRequest::checkRequest] http protocol parse error");
		}

		auto it = TC_Http::HEADER.find(string(sBuffer, p - sBuffer));
		if(it == TC_Http::HEADER.end())
		{
			throw runtime_error("[TC_HttpRequest::checkRequest] protocol not support: " + string(sBuffer, 8));
		}

		const char *header = strnstr(sBuffer, "\r\n\r\n", iLen);
		if ( header == NULL)
		{
			return false;
		}

		size_t pos = header - sBuffer;

		size_t iHeadLen = pos + 4;

		p =  strnstr(sBuffer, "\r\n", iHeadLen);
		if(p == NULL)
		{
			//first line
			return false;
		}

		p += 2;

		bool bChunk = false;

		size_t len  = 0;

		//找到\r\n\r\n之前的长度表示
		while (true)
		{
			size_t iMoveLen = p - sBuffer;
			if ( iMoveLen >= iHeadLen )
			{
				break;
			}

			const char *line    = p;
//	    const char *lineEnd  = strnstr(line, "\r\n", iLen - (line - sBuffer));

			const char *lineEnd = strstr(line, "\r\n");
			if (lineEnd != NULL)
			{
				p = lineEnd + 2;

				if (TC_Port::strncasecmp(line, "Transfer-Encoding:", 18) == 0)
				{
					string chunked = TC_Common::trim(string(line + 18, lineEnd));
					bChunk = (chunked == "chunked");
					if (bChunk) break;
				}
				else if (TC_Port::strncasecmp(line, "Content-Length:", 15) != 0)
				{
					continue;
				}
				else
				{
					len = TC_Common::strto<size_t>(TC_Common::trim(string(line + 15, lineEnd), " "));
				}
			}
		}

		if (bChunk)
		{
			int remain_len = iLen - iHeadLen;
			int move_len = 0;
			const char * pCur = header + 4;
			while (true)
			{
				p  = strnstr(pCur, "\r\n", remain_len);

//	        p = strstr(pCur , "\r\n");
				if ( p == NULL )
				{
					return false;
				}

				//查找当前chunk的大小
				int iChunkSize = strtol(string(pCur, p - pCur).c_str(), NULL, 16);
				if (iChunkSize <= 0)
				{
					return true; //所有chunk都接收完毕
				}

				move_len = (p - pCur) + 2 + iChunkSize + 2;
				if ( remain_len >= move_len )  //接收到一个完整的chunk了
				{
					//移动到下一个chunk
					remain_len -= move_len;
					pCur = p + 2 + iChunkSize + 2;
				}
				else
				{
					return false;
				}
			}
		}
		else  if (len + pos + 4 <= iLen)
		{
			return true;
		}

		return false;
	}

	size_t TC_Http::parseHeaderString(const char *beginIt, const char *headerIt, TC_Http::http_header_type &sHeader)
	{
		size_t contentLength = -1;

		sHeader.clear();

		bool first      = true;
		auto lineStartIt= beginIt;
		bool isLastLine = false;

		while (true)
		{
			auto it = strnstr(lineStartIt, "\r\n", headerIt - lineStartIt);
			if(it == NULL)
			{
				it = headerIt;
				isLastLine = true;
			}

			//first line ignore
			if(!first)
			{
				auto itF = strnstr(lineStartIt, ":", it - lineStartIt);
				if (itF != NULL)
				{
					while(*lineStartIt == ' ')
						++lineStartIt;

					const char *s = itF;
					while(*s == ' ')
						--s;

					string name(lineStartIt, s - lineStartIt);

					++itF;
					while(*itF == ' ')
						++itF;

					s = it;
					while(*s == ' ')
						--s;

					string value(itF, s-itF);

					if (TC_Port::strncasecmp(name.c_str(), "Content-Length", 14) == 0)
					{
						contentLength = TC_Common::strto<size_t>(value);
					}


					sHeader.insert(multimap<string, string>::value_type(std::move(name), std::move(value)));
				}
			}
			else
			{
				first = false;
			}

			if (isLastLine)
			{
				break;
			}

			lineStartIt = it + 2;//sep.size();
		}

		return contentLength;
	}

	void TC_HttpRequest::parseRequestHeader(const char* szBuffer, const char *header)
	{
		const char *p = strstr(szBuffer, "\r\n");

		assert(p != NULL);

		auto f1 = strnstr(szBuffer, " ", p - szBuffer);
		if (f1 == p)
		{
			throw TC_HttpRequest_Exception("[TC_HttpRequest::parseRequestHeader] http request format error: " + string(szBuffer, p - szBuffer));
		}

		auto f2 = strnstr(f1 + 1, " ", p - f1 + 1);//std::search(f1 + 1, p, sep.c_str(), sep.c_str() + sep.size());
		if (f2 == p || f1 >= f2)
		{
			throw TC_HttpRequest_Exception("[TC_HttpRequest::parseRequestHeader] http request format error: " + string(szBuffer, p - szBuffer));
		}

		//URL地址
		string sURL(f1 + 1, f2 - f1 - 1 );

		//HTTP协议版本
		_version.assign(f2 + 1, p - f2 - 1);

		parseHeaderString(szBuffer, header, _headers);

		_headComplete = true;

		if (TC_Port::strncasecmp(f1 + 1, "https://", 8) != 0 )
		{
			if (TC_Port::strncasecmp(f1 + 1, "http://", 7) != 0 )
			{
				sURL = "http://" + getHost() + sURL;
			}
		}

		parseURL(sURL);
	}

	void TC_HttpRequest::getHostPort(string &sDomain, uint32_t &iPort)
	{
		sDomain = _httpURL.getDomain();

		iPort   = TC_Common::strto<uint32_t>(_httpURL.getPort());
	}

	const string &TC_HttpRequest::getMethod() const
	{
		auto it = TC_Http::HEADER_REVERSE.find(_requestType);
		if(it != TC_Http::HEADER_REVERSE.end())
		{
			return it->second;
		}
		else
		{
			throw TC_HttpRequest_Exception("[TC_HttpRequest::setMethod] http request command error: " + TC_Common::tostr(_requestType));
		}
	}

	void TC_HttpRequest::setMethod(const char * sMethod)
	{
		auto it = TC_Http::HEADER.find(sMethod);
		if(it != TC_Http::HEADER.end())
		{
			_requestType = it->second;
		}
		else
		{
			throw TC_HttpRequest_Exception("[TC_HttpRequest::setMethod] http request command error: " + string(sMethod));
		}
	}

	void TC_HttpRequest::setPath(const char *path)
	{
		_httpURL._sPath = path;
	}

	void TC_HttpRequest::setDomain(const char * sDomain)
	{
		_httpURL._sDomain = sDomain;
	}

	void TC_HttpRequest::setScheme(const char * sScheme)
	{
		_httpURL._sScheme = sScheme;
	}

	int TC_HttpRequest::doRequest(TC_TCPClient& tcpClient, TC_HttpResponse& stHttpRsp)
	{
		//只支持短连接模式
		setConnection("close");

		string sSendBuffer = encode();

		int iRet = tcpClient.send(sSendBuffer.c_str(), sSendBuffer.length());
		if (iRet != TC_ClientSocket::EM_SUCCESS)
		{
			return iRet;
		}

		stHttpRsp.reset();

		TC_NetWorkBuffer::Buffer recvBuffer;

		while (true)
		{
			recvBuffer.expansion(recvBuffer.length() + 8*1024);

			size_t iRecvLen = recvBuffer.left();

			iRet = tcpClient.recv(recvBuffer.free(), iRecvLen);

			if (iRet == TC_ClientSocket::EM_SUCCESS)
			{
				recvBuffer.addWriteIdx(iRecvLen);
			}

			switch (iRet)
			{
			case TC_ClientSocket::EM_SUCCESS:
				if (stHttpRsp.incrementDecode(recvBuffer))
				{
					return TC_ClientSocket::EM_SUCCESS;
				}
				continue;
			case TC_ClientSocket::EM_CLOSE:
				stHttpRsp.incrementDecode(recvBuffer);
				return TC_ClientSocket::EM_SUCCESS;
			default:
				return iRet;
			}
		}

		assert(true);

		return 0;

	}

	int TC_HttpRequest::doRequest(TC_HttpResponse &stHttpRsp, int iTimeout)
	{
		//只支持短连接模式
		setConnection("close");

		string sSendBuffer = encode();

		string sHost;
		uint32_t iPort;

		getHostPort(sHost, iPort);

		TC_TCPClient tcpClient;
		tcpClient.init(sHost, iPort, iTimeout);

		int iRet = tcpClient.send(sSendBuffer.c_str(), sSendBuffer.length());
		if (iRet != TC_ClientSocket::EM_SUCCESS)
		{
			return iRet;
		}

		stHttpRsp.reset();

		TC_NetWorkBuffer::Buffer recvBuffer;

		while (true)
		{
			recvBuffer.expansion(recvBuffer.length() + 8*1024);

			size_t iRecvLen = recvBuffer.left();

			iRet = tcpClient.recv(recvBuffer.free(), iRecvLen);

			if (iRet == TC_ClientSocket::EM_SUCCESS)
			{
				recvBuffer.addWriteIdx(iRecvLen);
			}

			switch (iRet)
			{
			case TC_ClientSocket::EM_SUCCESS:
				if (stHttpRsp.incrementDecode(recvBuffer))
				{
					return TC_ClientSocket::EM_SUCCESS;
				}
				continue;
			case TC_ClientSocket::EM_CLOSE:
				stHttpRsp.incrementDecode(recvBuffer);
				return TC_ClientSocket::EM_SUCCESS;
			default:
				return iRet;
			}
		}

		assert(true);

		return 0;
	}

}


