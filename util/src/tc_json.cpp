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

#include "util/tc_json.h"

#include <math.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "util/tc_common.h"

namespace tars
{

#define FILTER_SPACE while(isspace((int)c)) {c=reader.read();}

//json里面定义的空白字符
bool JsonValue::isspace(char c)
{
    if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
        return true;
     return false;
}

uint32_t JsonValue::getHex(BufferJsonReader & reader)
{
	uint32_t iCode=0;
    char c;
    for(int iLoop=0;iLoop<4;iLoop++)
    {
        c=reader.read();
        if(c>='a'&&c<='f')
            iCode=iCode*16+c-'a'+10;
        else if(c>='A'&&c<='F')
            iCode=iCode*16+c-'A'+10;
        else if(c>='0'&&c<='9')
            iCode=iCode*16+c-'0';
        else
        {
            char s[64];
            snprintf(s, sizeof(s), "get string error3(\\u)[pos:%u]", (uint32_t)reader.getCur());
            throw TC_Json_Exception(s);
        }
    }
    return iCode;
}

void JsonValue::writeString(const string &str, ostream& ostr)
{
	ostr << "\"";
    std::string::const_iterator it(str.begin()), itEnd(str.end());
	for (; it != itEnd; ++it)
    {
        switch(*it)
        {
        case '"':
        	ostr << "\\\"";
        	break;
        case '\\':
        	ostr << "\\\\";
            break;
        case '/':
        	ostr << "\\/";
            break;
		case '\b':
			ostr << "\\b";
			break;
		case '\f':
			ostr << "\\f";
			break;
		case '\n':
			ostr << "\\n";
			break;
		case '\r':
			ostr <<  "\\r";
			break;
		case '\t':
			ostr << "\\t";
			break;
		default:
		{
			if((unsigned char)(*it)<0x20)
			{
				char buf[16];
				snprintf(buf,sizeof(buf),"\\u%04x",(unsigned char)*it);
				ostr << string(buf,6);
			}
			else
			{
				ostr << *it;
			}
			break;
		}
        }
    }
	ostr << "\"";
}

void JsonValue::write(ostream& ostr) const
{
    switch(getType())
    {
        case eJsonTypeString:
            dynamic_cast<const JsonValueString*>(this)->write(ostr);
            break;
        case eJsonTypeNum:
            dynamic_cast<const JsonValueNum*>(this)->write(ostr);
            break;
        case eJsonTypeObj:
            dynamic_cast<const JsonValueObj*>(this)->write(ostr);
            break;
        case eJsonTypeArray:
            dynamic_cast<const JsonValueArray*>(this)->write(ostr);
            break;
        case eJsonTypeBoolean:
            dynamic_cast<const JsonValueBoolean*>(this)->write(ostr);
            break;
        case eJsonTypeNull:
            dynamic_cast<const JsonValueNull*>(this)->write(ostr);
            break;
        default:
            assert(false);
    }

}

JsonValuePtr JsonValue::createJsonValue(BufferJsonReader & reader)
{
    char c = reader.read();

    FILTER_SPACE;

    JsonValue* obj = NULL;
    switch(c)
    {
        case '{':
            obj = new JsonValueObj();
            break;
        case '[':
            obj = new JsonValueArray();
            break;
        case '"':
            obj = new JsonValueString();
            break;
        case 'T':
        case 't':
        case 'F':
        case 'f':
            obj = new JsonValueBoolean();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '-':
            obj = new JsonValueNum();
            break;
        case 'n':
        case 'N':
            obj = new JsonValueNull();
            break;
        default:
            char s[64];
            snprintf(s, sizeof(s), "buffer overflow when peekBuf, over %u.", (uint32_t)reader.getCur());
            throw TC_Json_Exception(s);
    }
    obj->read(reader, c);

    return obj;
 }

void JsonValueString::write(ostream& ostr) const
{
    writeString(value, ostr);
}

void JsonValueString::read(BufferJsonReader & reader, char head)
{
    const char * pChar=reader.getPoint();
    uint32_t i=0;
    while(1)
    {
        char c = reader.read(); 
        if(c == '\\')
        {
            value.append(pChar,i);
            pChar=pChar+i+2;
            i=0;
            c=reader.read();
            switch(c) 
            {
                case '\\':
                case '\"':
                case '/':
                    value.append(1, c);
                    break;
                case 'b':
                    value.append(1,'\b');
                    break;
                case 'f':
                    value.append(1,'\f');
                    break;
                case 'n':
                    value.append(1,'\n');
                    break;
                case 'r':
                    value.append(1,'\r');
                    break;
                case 't':
                    value.append(1,'\t');
                    break;
                case 'u':
            // }
            // if(c == '\\' || c == '\"' || c == '/')
            //     value.append(1,c);
            // else if(c == 'b')
            //     value.append(1,'\b');
            // else if(c == 'f')
            //     value.append(1,'\f');
            // else if(c == 'n')
            //     value.append(1,'\n');
            // else if(c == 'r')
            //     value.append(1,'\r');
            // else if(c == 't')
            //     value.append(1,'\t');
            // else if(c == 'u')
                {
                    uint32_t iCode=getHex(reader);

                    if (iCode < 0x00080)
                    {
                        value.append(1,(char)(iCode & 0xFF));
                    }
                    else if (iCode < 0x00800)
                    {
                        value.append(1,(char)(0xC0 + ((iCode >> 6) & 0x1F)));
                        value.append(1,(char)(0x80 + (iCode & 0x3F)));
                    }
                    else if (iCode < 0x10000)
                    {
                        value.append(1,(char)(0xE0 + ((iCode >> 12) & 0x0F)));
                        value.append(1,(char)(0x80 + ((iCode >> 6) & 0x3F)));
                        value.append(1,(char)(0x80 + (iCode & 0x3F)));
                    }
                    else
                    {
                        value.append(1,(char)(0xF0 + ((iCode >> 18) & 0x07)));
                        value.append(1,(char)(0x80 + ((iCode >> 12) & 0x3F)));
                        value.append(1,(char)(0x80 + ((iCode >> 6) & 0x3F)));
                        value.append(1,(char)(0x80 + (iCode & 0x3F)));
                    }
                    pChar+=4;
                }
            }
        }
        else if(c==head)
            break;
        else
            i++;
    }
    value.append(pChar,i);
}

void JsonValueNum::write(ostream& ostr) const
{
    if (!isInt)
    {
        ostr << TC_Common::tostr(value);
    }
    else
    {
        ostr << (int64_t)value;
    }
}

void JsonValueNum::read(BufferJsonReader & reader, char head)
{
    bool bOk=true;
    bool bFloat=false;
    bool bExponential=false;
    bool bNegative=false;
    bool bExponentialNegative=false;
    int64_t iInt=0;
    double dFloat=0;
    double dFloatRat=0;
    int64_t iExponential=0;
    if(head == '-')
    {
        bOk=false;
        bNegative=true;
    }
    else
        iInt=head-0x30;

    char c;
    bool bNeedBack=false;

    while(1)
    {
        if(reader.hasEnd())
            break;
        c=reader.read();
        if(c>=0x30 && c<=0x39)
        {
            bOk=true;
            if(bExponential)
                iExponential=iExponential*10+c-0x30;
            else if(bFloat)
            {
                dFloat=dFloat+dFloatRat*(c-0x30);
                dFloatRat=dFloatRat*0.1;
            }
            else
                iInt=iInt*10+c-0x30;
        }
        else if(c == '.' && !bFloat && !bExponential && bOk)
        {
            bOk=false;
            bFloat=true;
            dFloatRat=0.1;
        }
        else if((c == 'e' || c == 'E') && !bExponential && bOk)
        {
            bOk=false;
            bExponential=true;
            iExponential=0;
            if(reader.hasEnd())
                break;
            c=reader.read();
            if(c == '-')
                bExponentialNegative=true;
            else if(c == '+')
                bExponentialNegative=false;
            else if(c>=0x30 && c<=0x39)
            {
                bOk=true;
                bExponential=(bool)(c-0x30);
            }
            else
            {
                bNeedBack=true;
                break;
            }
        }
        else
        {
            bNeedBack=true;
            break;
        }
    }
    if(!bOk)
    {
        char s[64];
        snprintf(s, sizeof(s), "get num error[pos:%u]", (uint32_t)reader.getCur());
        throw TC_Json_Exception(s);
    }
    if(bNeedBack)
        reader.back();
    if(bExponentialNegative)
        iExponential=0-iExponential;
    double dResult=(iInt+dFloat)*pow(10,iExponential);
    if(bNegative)
        dResult=0-dResult;

    this->value=dResult;
}

JsonValueObj::~JsonValueObj()
{
    // for(auto it = value.begin(); it != value.end(); ++it)
    // {
    //     delete it->second;
    // }
    // value.clear();
}

const JsonValuePtr& JsonValueObj::get(const char *name)
{
    auto it = value.find(name);

    if(it == value.end())
    {
        char s[64];
        snprintf(s, sizeof(s), "get obj error(key is not exists)[key:%s]", name);
        throw TC_Json_Exception(s);
    }

    return it->second;
}

void JsonValueObj::write(ostream& ostr) const
{
	ostr << "{ ";
    auto it = value.begin(); //unordered_map<string, JsonValuePtr>::const_iterator it(value.begin()), it_end(value.end());
	while (it != value.end())
    {
		writeString(it->first, ostr);

		ostr << ": " ;

        it->second->write(ostr);
        if(++it != value.end())
        {
        	ostr << ", ";
        }
    }
	ostr << " }";
}

void JsonValueObj::read(BufferJsonReader & reader, char cc)
{
    bool bFirst=true;
    while(1)
    {
        char c=reader.read();
        FILTER_SPACE;
        if(c == '}' && bFirst)
        {
            return;
        }
        bFirst=false;

        if(c != '"')
        {
            char s[64];
            snprintf(s, sizeof(s), "get obj error(key is not string)[pos:%u]", (uint32_t)reader.getCur());
            throw TC_Json_Exception(s);
        }

        JsonValueString vs;
        vs.read(reader, c);

        c=reader.read();
        FILTER_SPACE;
        if(c != ':')
        {
            char s[64];
            snprintf(s, sizeof(s), "get obj error(: not find)[pos:%u]", (uint32_t)reader.getCur());
            throw TC_Json_Exception(s);
        }
 
        value.insert(make_pair(vs.value, JsonValue::createJsonValue(reader))); 

        c=reader.read();
        FILTER_SPACE;

        if(c == ',') continue;

        if(c == '}')
        {
            return;
        }

        char s[64];
        snprintf(s, sizeof(s), "get obj error(, not find)[pos:%u]", (uint32_t)reader.getCur());
        throw TC_Json_Exception(s);
    }
}

JsonValueArray::~JsonValueArray()
{
    // for(auto it = value.begin(); it != value.end(); ++it)
    // {
    //     delete *it;
    // }
    // value.clear();
}

void JsonValueArray::write(ostream& ostr) const
{
	ostr << "[ ";
    auto it = value.begin(); //vector<JsonValuePtr>::const_iterator it(value.begin()), it_end(value.end());
	while (it != value.end())
    {
        (*it)->write(ostr);

		if (++it != value.end())
		{
			ostr << ", ";
		}
    }
	ostr << " ]";
}

void JsonValueArray::read(BufferJsonReader & reader, char cc)
{
    bool bFirst=true;
    while(1)
    {
        char c;
        if(bFirst)
        {
            c=reader.read();
            FILTER_SPACE;
            if(c == ']')
            {
                return;
            }
            reader.back();
        }
        bFirst=false;

        this->push_back(JsonValue::createJsonValue(reader));

        c=reader.read();
        FILTER_SPACE;
        if(c == ',')
            continue;
        if(c == ']')
            return;

        char s[64];
        snprintf(s, sizeof(s), "get vector error(, not find )[pos:%u]", (uint32_t)reader.getCur());
        throw TC_Json_Exception(s);
    }
}

void JsonValueBoolean::write(ostream& ostr) const
{
    if(value)
    	ostr << "true";
    else
    	ostr << "false";
}

//为了提高效率和代码好写就先这么写了
void JsonValueBoolean::read(BufferJsonReader & reader, char c)
{
    bool bOk=false;
    // bool bValue;
    if(c=='t'||c=='T')
    {
        c=reader.read();
        if(c=='r'||c=='R')
        {
            c=reader.read();
            if(c=='u'||c=='U')
            {
                c=reader.read();
                if(c=='e'||c=='E')
                {
                    value=true;
                    bOk=true;
                }
            }
        }
    }
    else if(c=='f'||c=='F')
    {
        c=reader.read();
        if(c=='a'||c=='A')
        {
            c=reader.read();
            if(c=='l'||c=='L')
            {
                c=reader.read();
                if(c=='s'||c=='S')
                {
                    c=reader.read();
                    if(c=='e'||c=='E')
                    {
                        value=false;
                        bOk=true;
                    }
                }
            }
        }
    }

    if(!bOk)
    {
        char s[64];
        snprintf(s, sizeof(s), "get bool error[pos:%u]", (uint32_t)reader.getCur());
        throw TC_Json_Exception(s);
    }
}

void JsonValueNull::write(ostream& ostr) const
{
    ostr << "null";
}

void JsonValueNull::read(BufferJsonReader & reader, char c)
{
    assert(c=='n' || c=='N');
    bool bOk=false;
    c=reader.read();
    if(c=='u'||c=='U')
    {
        c=reader.read();
        if(c=='l'||c=='L')
        {
            c=reader.read();
            if(c=='l'||c=='L')
            {
                bOk=true;
            }
        }
    }
    if(!bOk)
    {
        char s[64];
        snprintf(s, sizeof(s), "get NULL error[pos:%u]", (uint32_t)reader.getCur());
        throw TC_Json_Exception(s);
    }
}

string TC_Json::writeValue(const JsonValuePtr& p)
{
 	ostringstream ostr;
    p->write(ostr);
    return ostr.str();
}

JsonValuePtr TC_Json::getValue(const string & str)
{
    BufferJsonReader reader;
    reader.setBuffer(str.c_str(),str.length());
    return JsonValue::createJsonValue(reader);
}

}

