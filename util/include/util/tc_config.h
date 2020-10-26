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

#ifndef __TC_CONFIG_H_
#define __TC_CONFIG_H_

#include <map>
#include <list>
#include <stack>
#include <vector>
#include "util/tc_ex.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
 * @file  tc_config.h 
 * @brief  配置文件读取类.
 * @brief  Config File Reading Class 
 */
/////////////////////////////////////////////////

/**
* 域分隔符
* Field Separator
*/
const char TC_CONFIG_DOMAIN_SEP = '/';

/**
* 参数开始符
* Parameter Start Character
*/
const char TC_CONFIG_PARAM_BEGIN = '<';

/**
* 参数结束符
* Parameter Ender Character
*/
const char TC_CONFIG_PARAM_END = '>';

/**
* @brief 配置文件异常类.
* @brief Config File Exception Class
*/
struct TC_Config_Exception : public TC_Exception
{
    TC_Config_Exception(const string &buffer) : TC_Exception(buffer){};
    TC_Config_Exception(const string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_Config_Exception() throw(){};
};

/**
 * @brief 配置没有文件错误
 * @brief No Config File Error
 */
struct TC_ConfigNoParam_Exception : public TC_Exception
{
    TC_ConfigNoParam_Exception(const string &buffer) : TC_Exception(buffer){};
    ~TC_ConfigNoParam_Exception() throw(){};
};


/** 
* @brief 定义配置文件中的域的类. 
* @brief Class that defines the domain in the configuration file
*/
class TC_ConfigDomain
{
public:
    friend class TC_Config;

    /**
    * @brief 构造函数. 
    * @brief Constructor
    *  
    * @param sLine  指配置文件中的一行，按行读取
    * @param sLine  Refers to a line in the configuration file, read by line
    */
    TC_ConfigDomain(const string &sLine);

    /**
    *  @brief 析够函数.
    *  @brief Destructor
    */
    ~TC_ConfigDomain();

    /**
     * @brief 拷贝复制.
     * @brief Copy
     *  
     * @param tcd 一个TC_ConfigDomain对象，指配置文件的一个域
     * @param tcd a TC_ConfigDomain object, refers to a domain of the configuration file
     */
    TC_ConfigDomain(const TC_ConfigDomain &tcd);

    /**
     * @brief 赋值. 
     * @brief Assignment
     * 
     * @param tcd 一个TC_ConfigDomain对象，指配置文件的一个域
     * @param tcd a TC_ConfigDomain object, refers to a domain of the configuration file
     */
    TC_ConfigDomain& operator=(const TC_ConfigDomain &tcd);


    struct DomainPath
    {
        vector<string>  _domains;
        string          _param;
    };

    /**
     * @brief 解析一个domain 
     * @brief Parse a domain
     * 对形如"/Main/Domain<Name>"的path进行解析，解析的结果为一个 
     * DomainPath 类型，包括路径_domains和路径中对应的配置项_param.
     * 
     * Parsing a path like'/Main/Domain<Name>'results in a DomainPath type, 
     * including the path'_domains' and the corresponding configuration item in the path '_param'
     * 
     * @param path       一个经过处理的字符串，必须符合一定的要求
     * @param path       A processed string must meet certain requirements
     * @param bWithParam "/Main/Domain<Name>"时bWithParam为ture 
     *                   "/Main/Domain"时bWithParam为false
     * @param bWithParam bool: true, when'/Main/Domain<Name>'; 
     *                         false, when'/Main/Domain'
     * @return DomainPath 一个DomainPath对象，解析出域中的域名和参数
     * @return DomainPath A DomainPath object that resolves domain names and parameters in a domain
     */
    static DomainPath parseDomainName(const string& path, bool bWithParam);

    /**
    * @brief 增加子域名称
    * @brief Add Subdomain Name
    * 增加一个子域，name是子域的名字，比如"/Main/Domain" 
    * 再增加一个name为subDomain的子域,变成"/Main/Domain/subDomain" 
    * Add a subdomain, and 'name' is its name, such as "/Main/Domain" 
    * Add an other subdomain which named 'subDomain', such as "/Main/Domain/subDomain" 
    * 
    *  
    * @param name               子域的名称 
    * @param name               subdomain name
    * @return TC_ConfigDomain*  指向子域的指针 
    * @return TC_ConfigDomain*  the pointer which points to the subdomain 
    * @throws TC_Config_Exception 
    */
    TC_ConfigDomain* addSubDomain(const string& name);

    /**
    * @brief 递归搜索子域 Sub Domain. 
    * @brief Recursive search subdomain Sub Domain.
    *  
    * @param itBegin 迭代器 ，指向_domains的顶部
    * @param itBegin iterator, point to the button of the _domain
    * @param itEnd  迭代器 ，指向_domains的底部
    * @param itBegin iterator, point to the button of the _domain
    * @return TC_ConfigDomain* 指向目标子域的指针
    * @return TC_ConfigDomain* a point point to the target subdomain
    */
    TC_ConfigDomain *getSubTcConfigDomain(vector<string>::const_iterator itBegin, vector<string>::const_iterator itEnd);
    const TC_ConfigDomain *getSubTcConfigDomain(vector<string>::const_iterator itBegin, vector<string>::const_iterator itEnd) const;

    /**
    * @brief Get Param Value 获取参数/值对. 
    * @brief Get Param Value Get parameter/value pairs. 
    *  
    * @param sName 配置项名称
    * @param sName Configuration Item Name
    * @return      配置项对应的值
    * @return      the value of the configration item
    */
    string getParamValue(const string &sName) const;

    /**
    * @brief Get Param Map 获取map
    * map是参数/值对，是配置项和配置项的值对应的键值对, 
    * eg. SyncThreadNum = 2
    * Map is a parameter/value pair, a key-value pair corresponding to the value of the configuration item and the configuration item, 
    * eg. SyncThreadNum = 2
    * @return 一个域中的参数和其对应的值的map 
    * @return a map of parameters and their corresponding values in a domain
    */
    const map<string, string>& getParamMap() const { return _param; }

    /**
    * @brief Get Domain Map 获取map. 
    *  
    * @return map
    */
    const map<string, TC_ConfigDomain*>& getDomainMap() const { return _subdomain; }

    /**
    * @brief Set Param Value 设置参数/值对. 
    * 
    * @param sLine 行
    * @param sLine line
    * @return 
    */
    void setParamValue(const string &sLine);

    /**
     * @brief 插入参数，把key里没有的配置项添加到最后 
     * @brief Insert a parameter to add a configuration item that is not in the key to the last
     *  
     * @param 存配置项和对应配置项值的map
     */
    void insertParamValue(const map<string, string> &m);

    /**
    * @brief Destroy 释放. 
    *  
    * @return 无
    * @return void
    */
    void destroy();

    /**
    * @brief Get Name 获取域名称
    * @brief Get Name Get Domain Name
    * @return 域名称
    * @return the domain name
    */
    string getName() const;

    /**
     * @brief 设置域名称 
     * @brief Set Domain Name
     * 
     * @param 域名称
     * @param name domain name
     */
    void setName(const string& name);

    /**
     * @brief 按照文件中的顺序获取key
     * @brief Get keys in the order they appear in the file
     * 
     * @return vector<string>存放key的vector
     * @return the vector which stores keys in vector<string>
     */
    vector<string> getKey() const;

    /**
     * @brief 按照文件中的顺序获取line
     * @brief Get 'line' in the order they appear in the file
     * 
     * @return vector<string>存放key的vector
     * @return the vector which stores keys in vector<string>
     */
    vector<string> getLine() const;

    /**
     * @brief 按照文件中的顺序获取子Domain
     * @brief Get subdomains in the order they appear in the file
     * 
     * @return vector<string>存放域名字的vector
     * @return the vector which stores keys in vector<string>
     */
    vector<string> getSubDomain() const;

    /**
    * @brief 转换成配置文件的字符串格式. 
    * @brief Convert to the string format of the configuration file.
    *  
    * @param i  tab的层数
    * @param i  number of layers of 'tab'
    * @return   string类型配置字符串
    * @return   string type configuration string
    */
    string tostr(int i) const;

    /**
     * @brief 克隆.
     * @brief Clone.
     * 
     * @return TC_ConfigDomain*
     */
    TC_ConfigDomain* clone() const 
    { 
        return new TC_ConfigDomain(*this); 
    }

    /**
     * @brief 转义. 
     * @brief Escape.
     *  
     * @param name 
     * @return string
     */
    static string parse(const string& s);

    /**
     * @brief 方向转义. 
     * @brief Direction escaping
     *  
     * @param s 
     * @return string
     */
    static string reverse_parse(const string &s);

    /**
    * @brief 设置参数/值对 
    * @brief Set param/key pairs
    *  
    * @param sName  配置项名称
    * @param sName  the name of the configuration item
    * @param sValue 配置项的值
    * @param sValue the value of the configuration item
    * @return 无
    * @return void
    */
    void setParamValue(const string &sName, const string &sValue);

protected:

    /**
    * 域名称
    * Domain Name
    */
    string                  _name;

    /**
    * name/value对，配置项的名称和配置项的值
    * name/value pair, the name and value of the configuration item 
    */
    map<string, string>     _param;

    /**
     * key也就是配置项的插入顺序
     * key, the insertion order of configuration items
     */
    vector<string>          _key;

    /**
    * 子域
    * Subdomain
    */
    map<string, TC_ConfigDomain*>        _subdomain;

    /**
     * 域的插入顺序
     * the insertion order of the domain
     */
    vector<string>          _domain;
 
    /**
     * 整行的配置列表
     * Configuration list for the whole row
     */
    vector<string>          _line;
};

/**
 * @brief 定义配置文件的类（兼容wbl模式）.
 * @brief Define the class of the profile (compatible with WBL mode).
 
 * 支持从string中解析配置文件；
 * Support parsing configuration files from strings;
 
 * 支持生成配置文件；
 * Support the generation of configuration files;
 
 * 解析出错抛出异常；
 * Parse error and throw exception;

 
 * 采用[]获取配置，如果无配置则抛出异常；
 * Use [] to get the configuration and throw an exception if there is no configuration;
 
 * 采用get获取配置，不存在则返回空；
 * Use 'get' to get the configuration, and return null if it doesn't exist
 
 * 读取配置文件是线程安全的，insert域等函数非线程安全；
 * Reading configuration files is thread-safe, functions such as insert fields are not thread-safe;
 
 * 例如：
 * For example:
 
 *  <Main>
 *  <Domain>
 *      Name = Value
 *  </Domain>
 *   <Domain1>
 *       Name = Value
 *   </Domain1>
 *  </Main>
 
 *  获取参数:conf["/Main/Domain<Name>"] 获取域Map:
 *  Get parameter: conf ["/Main/Domain<Name>"] Get domain Map:
 *  getDomainMap("/Main/Domain", m); m得到Name/Value对
 *  getDomainMap("/Main/Domain", m); 'm' can get the Name/Value pair
 * 获取域Vector: getDomainVector("/Main", v); v得到Domain列表 可以增加域或域下面的值对
 * Get Domain Vector: getDomainVector ('/Main', v); 'v' can get a list of Domains to increase the number of the value pairs under the domain or domain
 
 */
class TC_Config
{
public:

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_Config();

    /**
     * @brief 拷贝复制.
     * @brief Copy
     *  
     * @param tc为TC_Config类型
     */
    TC_Config(const TC_Config &tc);

    /**
     * @brief 赋值. 
     * @brief Assignment
     *  
     * @param tcd 
     * @return TC_Config&
     */
    TC_Config& operator=(const TC_Config &tc);

    /**
    * @brief 解析文件. 
    * @brief Parse the file.
    *  
    * 把fileName对应的文件转化为输入流后调用parse对其进行解析 
    * Convert the file corresponding to fileName into an input stream and call parse to parse it.
    * @param sFileName : 文件名称 
    * @param sFileName file name
    * @return 无 
    * @return void
    * @throws TC_Config_Exception 
    */
    void parseFile(const string& sFileName);

    /**
    * @brief 解析字符串.
    * @brief Parse string 
    *  
    * 把string 类型先转化为输入流后调用parse对其进行解析 
    * Convert string type to input stream and call parse to parse it
    * @return void 
    * @throws TC_Config_Exception 
    */
    void parseString(const string& buffer);

    /**
    * @brief 获取值, 如果没有则抛出异常， 
    * 对于型如/Main/Domain<Param>的字符串，获取域/Main/Domain下的配置项名字为Param的值
    * @brief Get value. 
    * If no exception is thrown, for strings of which type like /Main/Domain<Param>, get the value of the configuration item named 'Param' under the domain '/Main/Domain'
    * @param sName 参数名称,例如: /Main/Domain<Param>
    * @param sName param name, like: /Main/Domain<Param>
    * @return      配置项对应的值
    * @return      the value of the configuration file 
    *  @throws     TC_Config_Exception 
    */
    string operator[](const string &sName) const;

    /**
     * @brief 获取值, 注意如果没有不抛出异常,返回空字符串. 
     * 对于型如/Main/Domain<Param>的字符串，获取域/Main/Domain下的配置项名字为Param的值
     * @brief Get value. Note that if no exception is thrown, an empty string is returned.
     * For strings of type like'/main/domain<param>', get the value of the configuration item named 'Param' under the domain '/Main/Domain'
     * @param sName 参数名称,  例如: /Main/Domain<Param>
     * @param sName param name, like: /Main/Domain<Param>
     * @return     配置项对应的值
     * @return     the value of the configuration file 
     */
    string get(const string &sName, const string &sDefault="") const;

    /**
	 * @brief 设置值.
     * @brief Set Value 
	 * @param sName 参数名称,  例如: /Main/Domain<Param>
     * @param sName param name, like: /Main/Domain<Param>
     */
    void set(const string &sName, const string &value);

    /**
    * @brief GetDomainParamMap获取域下面的参数值对. 
    * @brief GetDomainParamMap Get the parameter-value pair under the domain

    *  
    * @param path 域名称, 域标识, 例如: /Main/Domain
    * @param path domain name, domain identifier, such as: '/Main/Domain'
    * @param m    map<string, string>类型，域内的map列表
    * @param m    map<string, string> type, the map list in the domain
    * @return     bool, 返回域下面的参数值对
    * @return     bool, return the parameter-value pair under the domain
    */
    bool getDomainMap(const string &path, map<string, string> &m) const;

    /**
     * @brief 获取域下面的参数值对,不存则返回空map. 
     * @brief Get the parameter value pair under the domain. If it is not saved, an empty map will be returned.
     *  
     * @param path 域名称, 域标识, 例如: /Main/Domain 
     * @param path domain name, domain identifier, such as: '/Main/Domain'
     * @return     map<string,string>,对应域下面的所有参数值对
     * @return     bool, return the parameter-value pair under the domain
     */
    map<string, string> getDomainMap(const string &path) const;

    /**
     * @brief 获取域下面的所有key, 按照文件的行顺序返回
     * @brief Get all the keys under the domain and return them in line order
     * @param path 域名称, 域标识, 例如: /Main/Domain 
     * @param path domain name, domain identifier, such as: '/Main/Domain'
     * @return     vector<string>，某个标识下的所有key
     * @return     vector<string>，All keys under a certain ID
     */
    vector<string> getDomainKey(const string &path) const;

    /**
     * @brief 获取域下面的所有非域行, 按照文件的行顺序返回 
     *        区别于getDomainKey只能获取到key,该接口对带"="的配置也整行返回
     * @brief Get all non domain lines under the domain, and return them in line order of the file
     * The difference is that this interface getDomainKey, which can only get the key, also returns the whole line for the configuration with "=".
     * @param path 域名称, 域标识, 例如: /Main/Domain 
     * @param path domain name, domain identifier, such as: '/Main/Domain'
     * @return     vector<string>，某个标识下的所有非域行
     * @return     vector<string>，All non domain lines under a certain ID
     */
    vector<string> getDomainLine(const string &path) const;

    /**
    * @brief getDomainMap 获取域下面的子域. 
    * 
    *  
    * @param path  域名称, 域标识, 例如: /Main/Domain
    * @param path domain name, domain identifier, such as: '/Main/Domain'
    * @param v     要获取的子域名字， vector<string>下的域名
    * @param v     the target subdomian name，the domain name under vector<string>
    * @return      成功获取返回true，否则返回false
    * @return      bool: true, successful , false, failed
    */
    bool getDomainVector(const string &path, vector<string> &v) const;

    /**
     * @brief 获取域下面的子域, 
     *        不存在则返回空vector按照在文件中的顺序返回
     * @brief Get the subdomain under the domain
     * @param path 域名称, 域标识, 例如: /Main/Domain 
     * @param path domain name, domain identifier, such as: '/Main/Domain'
     * @return     vector<string>目标域下面的子域
     * @return     vector<string> the subdomain under the target domain
     */
    vector<string> getDomainVector(const string &path) const;

    /**
     * @brief 是否存在域. 
     * @brief Whether the domain exists or not.
     *  
     * @param path 域名称, 域标识, 例如: /Main/Domain 
     * @param path domain name, domain identifier, such as: '/Main/Domain'
     * @return     存在返回true，否则返回false
     * @return     If the domain does exist, return true, else return false.
     */
    bool hasDomainVector(const string &path) const;

    /** 
     * @brief 增加域，在当前域下面增加域, 如果已经存在sAddDomain域, 
     *        则认为成功
     * @brief 

Add domain. Add a domain under the current domain. If the sadddomain domain already exists, it is considered to be successful.
     * @param sCurDomain 域标识符, 例如:/Main/Domain
     * @param sCurDomain domain identifier, such as: '/Main/Domain'
     * @param sAddDomain 需要增加的域名称: 例如: sCurDomain
     * @param sAddDomain the name of the domain which need to be added: like: sCurDomain
     * @param bCreate    sCurDomain域不存在的情况下, 是否自动创建
     * @param bCreate    When the domain sCurDomain does not exist, determine wheter it needs to be created automatically.
     * @return           0-成功增加, 1-sCurDomain不存在 
     * @return           0 - add successfull, 1 - sCurDomain doesn't exist.
     */
    int insertDomain(const string &sCurDomain, const string &sAddDomain, bool bCreate);


    /** 
     * @brief 增加参数，即配置项，当前域下面增加配置项参数, 
     *        如果已经有相关参数, 则忽略(不替换)
     * @brief Add parameter, i.e. configuration item. Add configuration item parameter under the current domain. If there are already related parameters, ignore (do not replace).
     * @param sCurDomain  域标识符, 例如:/Main/Domain
     * @param sCurDomain domain identifier, such as: '/Main/Domain'
     * @param m           map类型，存参数值对
     * @param m           map type, save parameter-value pairs
     * @param bCreate     sCurDomain域不存在的情况下, 是否自动创建
     * @param bCreate     When the domain sCurDomain does not exist, determine wheter it needs to be created automatically.
     * @return            0: 成功, 1:sCurDomain不存在 
     * @return            0 - add successfull, 1 - sCurDomain doesn't exist.
     */
    int insertDomainParam(const string &sCurDomain, const map<string, string> &m, bool bCreate);

    /**
     * @brief 合并配置文件到当前配置文件. 
     * @brief Merge profile to current profile
     *  
     * @param cf
     * @param bUpdate true-冲突项更新本配置, false-冲突项不更新
     * @param bUpdate true - update the conflict item to this configuration, false - don't update the conflict item
     */
    void joinConfig(const TC_Config &cf, bool bUpdate);

    /**
    * @brief 转换成配置文件的字符串格式. 
    * @brief convert to the string format of the configuration file
    *  
    * @return 配置字符串
    * @return the configuration string
    */
    string tostr() const;

protected:
    /**
    * @brief Parse输入流， 
    * 最终把输入流解析成为一个TC_ConfigDomain装入stack中 
    * @brief Parse Input Stream
    * Finally, the input stream is parsed into a TC_ ConfigDomain and is loaded into the stack.
    * @param  要解析的输入流，按行解析 
    * @param  the input stream, parsed by line
    * @throws TC_Config_Exception
    * @return
    */
    void parse(istream &is);

    /**
    * @brief create New Domain 生成子域. 
    * @brief Create New subdomain
    *  
    * @param sName 域名称
    * @param sName domain name
    * @return      指向新生成的子域的指针
    * @return      Pointer to the newly generated subfield
    * @throws TC_Config_Exception 
    */
    TC_ConfigDomain *newTcConfigDomain(const string& sName);

    /**
    * @brief Search Domain 搜索域. 
    *  
    * @param sDomainName 参数名称,支持子域搜索
    * @param sDomainName param name, support the subdomain search
    * @return value
    */
    TC_ConfigDomain *searchTcConfigDomain(const vector<string>& domains);
    const TC_ConfigDomain *searchTcConfigDomain(const vector<string>& domains) const;

protected:

    /**
    * 根domain
    * the root domain
    */
    TC_ConfigDomain _root;
};

}
#endif //_TC_CONFIG_H_
