#ifndef __HttpParser_h
#define __HttpParser_h

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <map>
#include <iterator>

using namespace std;

namespace Http
{
    static string str(long long value)
    {
        std::stringstream ss;

        ss<<value;
        return ss.str();
    }

    static unsigned char hex(unsigned char c)
    {
        return c > 9 ? c + 55 : c  +48;
    }

    static unsigned char reHex(unsigned char c)
    {
        unsigned char r;

        if(c >= 'A' && c <= 'Z')
            r = c - 'A' + 10;
        else if(c >= 'a' && c <= 'z')
            r = c-'a'+10;
        else if(c >= '0' && c <= '9')
            r = c-'0';
        else
            r = ' ';

        return r;
    }

    static bool parseUrl(const string& url, string& host, string& port, string& root)
    {
        string u = url;
        bool ssl = false;
        size_t i;

        if(u.substr(0, 7) == "http://")
            u = u.substr(7);
        else if(u.substr(0, 8) == "https://")
        {
            ssl = true;
            u = u.substr(8);
        }

        i = u.find(":");
        if(i == string::npos)
        {
            if(!ssl)
                port = "80";
            else
                port = "443";
            i = u.find("/");
            if(i != string::npos)
            {
                host = u.substr(0, i);
                root = u.substr(i);
            }
            else
            {
                host = u;
                root = "/";
            }
        }
        else if(i+1 < u.length())
        {
            size_t j = i;

            host = u.substr(0, i);
            i = u.find("/");
            port = u.substr(j+1, i-j-1);
            if(i != string::npos)
                root = u.substr(i);
            else
                root = "/";
        }

        return ssl;
    }

    static string urlEncode(const string& str)
    {
        string r;

        for(size_t i = 0; i < str.length(); i++)
        {
            if(isalnum((unsigned char)str[i])
                || str[i] == '-'
                || str[i] == '_'
                || str[i] == '.'
                || str[i] == '~')
                r += str[i];
            else if(str[i] == ' ')
                r += '+';
            else
            {
                r += '%';
                r += hex((unsigned char)str[i] >> 4);
                r += hex((unsigned char)str[i] % 16);
            }
        }

        return r;
    }

    static string urlDecode(const string& str)
    {
        string r;

        for(size_t i=0; i<str.length(); i++)
        {
            if(str[i] == '+')
                r += ' ';
            else if(str[i] == '%')
            {
                if(i+2 < str.length())
                {
                    unsigned char high = static_cast<unsigned char>(str[i + 1]);
                    unsigned char low = static_cast<unsigned char>(str[i + 2]);
                    r += reHex(high) * 16 + reHex(low);
                    i += 2;
                }
                else
                    break;
            }
            else
                r += str[i];
        }

        return r;
    }

    static string upper(const string &str)
    {
        string r = str;

        transform(r.begin(), r.end(), r.begin(), ::toupper);
        return r;
    }

    class Headers
    {
        class Header
        {
        public:
            Header() {}
            Header( const string& name, const string& value)
            {
                _name = name;
                _value = value;
            }

        public:
            string _name;
            string _value;
        };

    public:
        void set(const string& name, const string& value)
        {
            if(value.length())
                _headers[upper(name)] = Header(name, value);
            else
                _headers.erase(upper(name));
        }

        bool has(const string& name) const
        {
            return _headers.find(upper(name)) != _headers.end();
        }

        string value(const string& name) const
        {
            std::map<std::string, Header>::const_iterator it = _headers.find(upper(name));

            if(it != _headers.end())
                return it->second._value;
            else
                return "";
        }

        string serialize() const
        {
            std::stringstream ss;

            for(map<string, Header>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
                ss << it->second._name + ": " + it->second._value + "\r\n";
            return ss.str();
        }

        void clear()
        {
            _headers.clear();
        }

    private:
        map<string, Header> _headers;
    };

    class Message
    {
    public:
        void setFirstLine(const string& str)
        {
            _firstLine = str;
        }

        string firstLine() const
        {
            return _firstLine;
        }

        void setHeader(const string& name, const string& value = "")
        {
            _headers.set(name, value);
        }

        bool hasHeader(const string& name) const
        {
            return _headers.has(name);
        }

        string header(const string& name) const
        {
            return _headers.value(name);
        }

        string& content()
        {
            return _content;
        }

        const string& content() const
        {
            return _content;
        }

        string serialize(bool chunked = false)
        {
            std::stringstream ss;

            ss << _firstLine << "\r\n";

            if(chunked)
            {
                _headers.set("Content-Length", "");
                _headers.set("Transfer-Encoding", "chunked");
            }
            else
                _headers.set("Content-Length", str( _content.length()));

            ss << _headers.serialize() << "\r\n";

            if(chunked)
            {
                if(_content.length())
                {
                    ss << std::hex << _content.length() << "\r\n";
                    ss << _content << "\r\n";
                }
            }
            else
                ss << _content;

            return ss.str();
        }

        string message(bool content = true) const
        {
            std::stringstream ss;

            ss << _firstLine << "\r\n";
            ss << _headers.serialize() << "\r\n";
            if(content)
                ss << _content;

            return ss.str();
        }

        virtual void clear()
        {
            _firstLine.clear();
            _headers.clear();
            _content.clear();
        }

    private:
        string _firstLine;
        Headers _headers;
        string _content;
    };

    class Request: public Message
    {
    public:
        Request()
        {
            clear();
        }

        void setMethod( const string& str)
        {
            _method = str;
        }

        string method() const
        {
            return _method;
        }

        void setRequestUrl( const string& str)
        {
            _requestUrl = str;
        }

        string requestUrl() const
        {
            return _requestUrl;
        }

        void setVersion(const string& str)
        {
            _version = str;
        }

        string version() const
        {
            return _version;
        }

        string serialize()
        {
            Message::setFirstLine(method() + " " + requestUrl() + " " + version());
            if(!content().length())
                if(method() != "PUT" || method() != "POST" || method() != "PATCH")
                    setHeader("Content-Length");

            return Message::serialize();
        }

        virtual void clear()
        {
            Message::clear();
            setMethod("GET");
            setRequestUrl("/");
            setVersion("HTTP/1.1");
        }

    private:
        void setFirstLine(const string& str) {}

        // 20160720
        //string firstLine() const { return "";}

    private:
        string _method;
        string _requestUrl;
        string _version;
    };

    class Response: public Message
    {
    public:
        Response()
        {
            clear();
        }

        void setStatusCode(const string& str, bool autoPhrase = true)
        {
            _statusCode = str;
            if(autoPhrase)
            {
                if(_statusCode.substr(0, 1) == "1")
                {
                    if(_statusCode == "100")
                        setReasonPhrase("Continue");
                    else if(_statusCode == "101")
                        setReasonPhrase("Switching Protocols");
                }
                else if(_statusCode.substr(0, 1) == "2")
                {
                    if(_statusCode == "200")
                        setReasonPhrase("OK");
                    else if(_statusCode == "201")
                        setReasonPhrase("Created");
                    else if(_statusCode == "202")
                        setReasonPhrase("Accepted");
                    else if(_statusCode == "203")
                        setReasonPhrase("Non-Authoritative Information");
                    else if(_statusCode =="204")
                        setReasonPhrase("No Content");
                    else if(_statusCode == "205")
                        setReasonPhrase("Reset Content");
                    else if(_statusCode == "206")
                        setReasonPhrase("Partial Content");
                }
                else if(_statusCode.substr(0, 1) == "3")
                {
                    if(_statusCode == "300")
                        setReasonPhrase("Multiple Choices");
                    else if(_statusCode == "301")
                        setReasonPhrase("Moved Permanently");
                    else if(_statusCode == "302")
                        setReasonPhrase("Found");
                    else if(_statusCode == "303")
                        setReasonPhrase("See Other");
                    else if(_statusCode == "304")
                        setReasonPhrase("Not Modified");
                    else if(_statusCode == "305")
                        setReasonPhrase("Use Proxy");
                    else if(_statusCode == "307")
                        setReasonPhrase("Temporary Redirect");
                }
                else if(_statusCode.substr(0, 1) == "4")
                {
                    if(_statusCode == "400")
                        setReasonPhrase("Bad Request");
                    else if(_statusCode == "401")
                        setReasonPhrase("Unauthorized");
                    else if(_statusCode == "403")
                        setReasonPhrase("Forbidden");
                    else if(_statusCode == "404")
                        setReasonPhrase("Not Found");
                    else if(_statusCode == "405")
                        setReasonPhrase("Method Not Allowed");
                    else if(_statusCode == "406")
                        setReasonPhrase("Not Acceptable");
                    else if(_statusCode == "407")
                        setReasonPhrase("Proxy Authentication Required");
                    else if(_statusCode == "408")
                        setReasonPhrase("Request Timeout");
                    else if(_statusCode == "409")
                        setReasonPhrase("Conflict");
                    else if(_statusCode == "410")
                        setReasonPhrase("Gone");
                    else if(_statusCode == "411")
                        setReasonPhrase( "Length Required");
                    else if(_statusCode == "412")
                        setReasonPhrase("Precondition Failed");
                    else if(_statusCode == "413")
                        setReasonPhrase("Request Entity Too Large");
                    else if(_statusCode == "414")
                        setReasonPhrase("Request URI Too Long");
                    else if(_statusCode == "416")
                        setReasonPhrase("Requested Range Not Satisfiable");
                    else if(_statusCode == "417")
                        setReasonPhrase("Expectation Failed");
                }
                else if(_statusCode.substr(0, 1) == "5")
                {
                    if(_statusCode == "500")
                        setReasonPhrase("Internal Server Error");
                    else if(_statusCode == "501")
                        setReasonPhrase("Not Implemented");
                    else if(_statusCode == "502")
                        setReasonPhrase("Bad Gateway");
                    else if(_statusCode == "503")
                        setReasonPhrase("Service Unavailable");
                    else if(_statusCode == "504")
                        setReasonPhrase("Gateway Timeout");
                    else if(_statusCode == "505")
                        setReasonPhrase("Http Version Not Supported");
                }
            }
        }

        string statusCode() const
        {
            return _statusCode;
        }

        void setReasonPhrase(const string& str)
        {
            _reasonPhrase = str;
        }

        string reasonPhrase() const
        {
            return _reasonPhrase;
        }

        void setVersion(const string& str)
        {
            _version = str;
        }

        string version() const
        {
            return _version;
        }

        string serialize(bool chunked = false)
        {
            Message::setFirstLine(version() + " "  + statusCode() + " " + reasonPhrase());
            return Message::serialize(chunked);
        }

        virtual void clear()
        {
            Message::clear();
            setStatusCode("404");
            setVersion("HTTP/1.1");
        }

    private:
        void setFirstLine(const string& str) {}

        // 20160720
        //string firstLine() const { return "";}

    private:
        string _statusCode;
        string _reasonPhrase;
        string _version;
    };

    class Parser
    {
    public:
        Parser(Message& message, size_t maxHeadLen = -1)
        {
            _message = &message;
            _maxHeadLen = maxHeadLen;
            clear();
        }

        bool input(const char* str, size_t len)
        {
            if(_leftLen == (size_t)-1 && _maxHeadLen != (size_t)-1)
            {
                if(_maxHeadLen > _buf.length())
                {
                    if(len < _maxHeadLen - _buf.length())
                        return _input(str, len);
                    else
                    {
                        size_t oLen = _maxHeadLen-_buf.length();

                        if(_input(str, oLen))
                            if(_leftLen != (size_t)-1)
                            {
                                if(_leftLen != 0)
                                    if(len - oLen)
                                        return _input(str+oLen, len-oLen);

                                return true;
                            }
                    }
                }

                return false;
            }
            else
                return _input(str, len);
        }

        bool input(const string& str)
        {
            return input(str.c_str(), str.length());
        }

        size_t contentLen() const
        {
            return _contentLen;
        }

        size_t leftLen() const
        {
            return _leftLen;
        }

        bool chunked() const
        {
            return _chunked;
        }

        void clear()
        {
            _message->clear();
            _buf.clear();
            _idx = 0;
            _contentLen = (size_t)-1;
            _leftLen = (size_t)-1;
            _chunked = false;
        }

    private:
        bool _input(const char* str, size_t len)
        {
            if(_leftLen == (size_t)-1)
            {
                _buf.append(str, len);
                while(true)
                {
                    size_t i = _buf.find("\r\n", _idx);

                    if(i != string::npos)
                    {
                        string line = _buf.substr(_idx, i-_idx);

                        _idx = i+2;
                        if(line.length())
                        {
                            if(_message->firstLine().length())
                            {
                                size_t i = line.find(": ");

                                if(i != string::npos)
                                    _message->setHeader(line.substr(0, i), line.substr(i+2));
                                else
                                    return false;
                            }
                            else
                                _message->setFirstLine(line);
                        }
                        else if(_message->firstLine().length())
                        {
                            if(_message->hasHeader("Content-Length"))
                            {
                                string cLenStr = _message->header("Content-Length");

                                _message->content() = _buf.substr(_idx);
                                if(cLenStr.length())
                                {
                                    std::stringstream ss;

                                    ss << cLenStr;
                                    ss >> _contentLen;

                                    if(_contentLen > _buf.length() - _idx)
                                        _leftLen = _contentLen - _message->content().length();
                                    else
                                        _leftLen = 0;
                                }
                                else
                                    _leftLen = 0;
                            }
                            else if(_message->header("Transfer-Encoding") == "chunked")
                            {
                                _chunked = true;
                                _message->content() = _buf.substr( _idx);
                                _leftLen = 0;
                            }
                            else
                                _leftLen = 0;
                            break;
                        }
                        else
                            return false;
                    }
                    else
                        break;
                }
            }
            else
            {
                _message->content().append(str, len);
                if(_leftLen)
                {
                    if(len >= _leftLen)
                        _leftLen = 0;
                    else
                        _leftLen -= len;
                }
            }

            return true;
        }

    protected:
        Message* _message;

    private:
        string _buf;
        size_t _idx;
        size_t _contentLen;
        size_t _leftLen;
        size_t _maxHeadLen;
        bool _chunked;
    };

    class RequestParser : public Parser
    {
    public:
        RequestParser(Request& request, size_t maxHeadLen=-1): Parser(request, maxHeadLen) {}

        bool input(const char* str, size_t len)
        {
            if(Parser::input(str, len))
            {
                if(_message->firstLine().length())
                {
                    string line = _message->firstLine();
                    size_t i = line.find(' ');

                    if(i != string::npos && i!=0)
                    {
                        Request& request = *static_cast<Request*>(_message);

                        request.setMethod(line.substr(0, i));
                        line = line.substr(i+1);
                        i = line.find(' ');
                        if(i != string::npos && i != 0)
                        {
                            request.setRequestUrl(line.substr(0, i));
                            line = line.substr(i + 1);
                            if(line.length())
                                request.setVersion(line);
                            else
                                return false;
                        }
                        else
                            return false;
                    }
                    else
                        return false;
                }
            }
            else
                return false;

            return true;
        }

        bool input(const string& str)
        {
            return input(str.c_str(), str.length());
        }
    };

    class ResponseParser : public Parser
    {
    public:
        ResponseParser(Response& response, size_t maxHeadLen = -1) : Parser(response, maxHeadLen) {}

        bool input(const char* str, size_t len)
        {
            if(Parser::input(str, len))
            {
                if(_message->firstLine().length())
                {
                    string line = _message->firstLine();
                    size_t i = line.find(' ');

                    if(i != string::npos && i != 0)
                    {
                        Response& response = *static_cast<Response*>(_message);

                        response.setVersion(line.substr(0, i));
                        line = line.substr(i+1);
                        i = line.find(' ');
                        if(i != string::npos && i != 0)
                        {
                            response.setStatusCode(line.substr(0, i));
                            line = line.substr(i + 1);
                            if(line.length())
                                response.setReasonPhrase(line);
                            else
                                return false;
                        }
                        else
                            return false;
                    } else
                        return false;
                }
            }
            else
                return false;

            return true;
        }

        bool input(const string& str)
        {
            return input(str.c_str(), str.length());
        }
    };

    class ChunkParser
    {
    public:
        ChunkParser()
        {
            clear();
        }

        bool input(const char* str, size_t len)
        {
            _input.append(str, len);

            while(true)
            {
                if(!_stage)
                {
                    size_t i = _input.find("\r\n");

                    if(i != string::npos)
                    {
                        std::stringstream ss;

                        ss << std::hex << _input.substr(0, i);
                        ss >> _len;

                        if(_len)
                        {
                            _len += 2;
                            _input = _input.substr(i + 2);
                            _stage = 1;
                        }
                        else
                        {
                            _input = _input.substr(i + 2);
                            _stage = 2;
                        }
                    }
                    else
                        break;
                }
                else if(_stage == 1)
                {
                    if(_input.length() >= _len)
                    {
                        _output += _input.substr(0, _len - 2);
                        _input = _input.substr(_len);
                        _len = 0;
                        _stage = 0;
                    }
                    else
                    {
                        _output += _input;
                        _len -= _input.length();
                        _input.clear();
                        break;
                    }
                }
                else if(_stage == 2)
                {
                    if( _input.find("\r\n") != string::npos)
                        return true;
                    else
                        break;
                }
            }

            return false;
        }

        string output()
        {
            string r = _output;

            _output.clear();
            return r;
        }

        void clear()
        {
            _stage = 0;
            _input.clear();
            _output.clear();
        }

    private:
        int _stage;
        size_t _len;
        string _input;
        string _output;
    };
}

#endif
