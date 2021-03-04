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

#ifndef __TC_MYSQL_H
#define __TC_MYSQL_H

#include "util/tc_platform.h"
#include "mysql.h"
#include "util/tc_ex.h"
#include <map>
#include <vector>
#include <stdlib.h>
#include <functional>

namespace tars
{

/////////////////////////////////////////////////
/** 
* @file  tc_mysql.h 
* @brief mysql操作类. 
* @brief MySQL operation class.
* 
*/           
/////////////////////////////////////////////////

/**
* @brief 数据库异常类
* @brief Database exception class
*/
struct TC_Mysql_Exception : public TC_Exception
{
    TC_Mysql_Exception(const string &sBuffer) : TC_Exception(sBuffer){};
    ~TC_Mysql_Exception() throw(){};    
};

/**
* @brief 数据库配置接口
* @brief Database configuration interface
*/
struct TC_DBConf
{
    /**
    * 主机地址
    * Host Address
    */
    string _host;

    /**
    * 用户名
    * User name
    */
    string _user;

    /**
    * 密码
    */
    string _password;

    /**
    * 数据库
    * Database
    */
    string _database;

    /**
     * 字符集
     * Character Set
     */
    string _charset;

    /**
    * 端口
    * Port
    */
    int _port;

    /**
    * 客户端标识
    * Client Identity
    */
    int _flag;

    /**
    * 连接超时
    * Port
    */
    int _connectTimeout;

    /**
    * 读写超时
    * Port
    */
    int _writeReadTimeout;

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_DBConf()
        : _port(0)
        , _flag(0)
    {
    }

    /**
    * @brief 读取数据库配置. 
    * @brief Read Database Configuration.
    * 
    * @param mpParam 存放数据库配置的map 
    * @param mpParam Map holding database configuration
    *        dbhost: 主机地址
    *        dbhost: host address
    *        dbuser:用户名
    *        dbuser:username
    *        dbpass:密码
    *        dbpass:password
    *        dbname:数据库名称
    *        dbname:database name
    *        dbport:端口
    *        dbport:port
    */
    void loadFromMap(const map<string, string> &mpParam)
    {
        map<string, string> mpTmp = mpParam;

        _host               = mpTmp["dbhost"];
        _user               = mpTmp["dbuser"];
        _password           = mpTmp["dbpass"];
        _database           = mpTmp["dbname"];
        _charset            = mpTmp["charset"];
        _port               = atoi(mpTmp["dbport"].c_str());
        _flag               = 0;        
        _connectTimeout     = atoi(mpTmp["connectTimeout"].c_str());
        _writeReadTimeout   = atoi(mpTmp["writeReadTimeout"].c_str());

        if(mpTmp["dbport"] == "")
        {
            _port = 3306;
        }

        if(mpTmp["connectTimeout"] == "")
        {
            _connectTimeout = 5;
        }

        if(mpTmp["writeReadTimeout"] == "")
        {
            _writeReadTimeout = 15;
        }
    }

};

/**
* @brief Mysql数据库操作类 
* @brief Mysql database operation class
* 
* 非线程安全，通常一个线程一个TC_Mysql对象；
* Non-thread safe, usually one thread and one TC_Mysql object;
* 
* 对于insert/update可以有更好的函数封装，保证SQL注入；
* For insert/update better function encapsulation guarantees SQL injection:
* 
* TC_Mysql::DB_INT表示组装sql语句时，不加””和转义；
* TC_Mysql:: DB_INT means no "" and escape when assembling SQL statements;
* 
* TC_Mysql::DB_STR表示组装sql语句时，加””并转义；
* TC_Mysql:: DB_STR means to add "" and escape when assembling SQL statements;
*/
class TC_Mysql 
{
public:
    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_Mysql();

    /**
    * @brief 构造函数. 
    * @brief Constructor.
    *  
    * @param sHost        主机IP
    * @param sHost        Host IP
    * @param sUser        用户
    * @param sUser        User
    * @param sPasswd      密码
    * @param sPasswd      Password
    * @param sDatebase    数据库
    * @param sDatebase    Database
    * @param port         端口
    * @param port         Port
    * @param iUnixSocket  socket
    * @param iUnixSocket  Socket
    * @param iFlag        客户端标识
    * @param iFlag        Client Identity
    */
    TC_Mysql(const string& sHost, const string& sUser = "", const string& sPasswd = "", const string& sDatabase = "", const string &sCharSet = "", int port = 0, int iFlag = 0);

    /**
    * @brief 构造函数. 
    * @brief Constructor.
    * @param tcDBConf 数据库配置
    * @param tcDBConf Database Configuration
    */
    TC_Mysql(const TC_DBConf& tcDBConf);

    /**
    * @brief 析构函数.
    * @brief Destructor.
    */
    ~TC_Mysql();

    /**
    * @brief 初始化. 
    * @brief Initialization
    *  
    * @param sHost        主机IP
    * @param sHost        Host IP
    * @param sUser        用户
    * @param sUser        User
    * @param sPasswd      密码
    * @param sPasswd      Password
    * @param sDatebase    数据库
    * @param sDatebase    Database
    * @param port         端口
    * @param port         Port
    * @param iUnixSocket  socket
    * @param iFlag        客户端标识
    * @param iFlag        Client Identity
    * @return 无
    * @return none
    */
    void init(const string& sHost, const string& sUser  = "", const string& sPasswd  = "", const string& sDatabase = "", const string &sCharSet = "", int port = 0, int iFlag = 0);

    /**
    * @brief 初始化. 
    * @brief Initialization
    *  
    * @param tcDBConf 数据库配置
    * @param tcDBConf Database Configuration
    */
    void init(const TC_DBConf& tcDBConf);

    /**
    * @brief 读取mysql配置
    * @brief Read MySQL configuration
    *
    * @return tcDBConf 
    */
    TC_DBConf getDBConf();

    /**
    * @brief 连接数据库. 
    * @brief Connect to database.
    *  
    * @throws TC_Mysql_Exception
    * @return 无
    * @return none
    */
    void connect();
 
    /**
    * @brief 断开数据库连接. 
    * @brief Disconnect database.
    * @return 无
    * @return none
    */
    void disconnect();

    /**
     * @brief 获取数据库变量. 
     * @brief Get database variables.
     * 
     * @return 数据库变量
     * @return Database Variables
     */
    string getVariables(const string &sName);

    /**
    *  @brief 直接获取数据库指针. 
    *  @brief Get database pointer directly.
    *  
    * @return MYSQL* 数据库指针
    * @return MYSQL*Database Pointer
    */
    MYSQL *getMysql();

    /**
    *  @brief 字符转义, 需要连接到数据库,考虑了字符集
    *  @brief Character escape, need to connect to database, consider character set
    *  
    * @param sFrom  源字符串
    * @param sFrom  Source string
    * @param sTo    输出字符串
    * @param sTo    Output string
    * @return       输出字符串
    * @return       Output string
    */
    string realEscapeString(const string& sFrom);

    /**
    * @brief 更新或者插入数据. 
    * @brief Update or insert data.
    *  
    * @param sSql  sql语句
    * @param sSql  SQL statement
    * @throws      TC_Mysql_Exception
    * @return
    */
    void execute(const string& sSql);

    /**
     *  @brief mysql的一条记录
     *  @brief A record of MySQL
     */
    class MysqlRecord
    {
    public:
        /**
         * @brief 构造函数.
         * @brief Constructor.
         *  
         * @param record
         */
        MysqlRecord(const map<string, string> &record);

        /**
         * @brief 获取数据，s一般是指数据表的某个字段名 
         * @brief Get data, s is generally a field name of an index table
         * @param s 要获取的字段
         * @param s Fields to get
         * @return  符合查询条件的记录的s字段名
         * @return  s field name of the record that meets the query criteria
         */
        const string& operator[](const string &s);
    protected:
        const map<string, string> &_record;
    };

    /**
     * @brief 查询出来的mysql数据
     * @brief MySQL data queried
     */
    class MysqlData
    {
    public:
        /**
         * @brief 所有数据.
         * @brief All data.
         * 
         * @return vector<map<string,string>>&
         */
        vector<map<string, string> >& data();

        /**
         * 数据的记录条数
         * Number of records for data
         * 
         * @return size_t
         */
        size_t size();

        /**
         * @brief 获取某一条记录. 
         * @brief Get a record
         *  
         * @param i  要获取第几条记录 
         * @param i  Which record to get
         * @return   MysqlRecord类型的数据，可以根据字段获取相关信息，
         * @return   MysqlRecord type data, you can get relevant information based on the field,
         */
        MysqlRecord operator[](size_t i);

    protected:
        vector<map<string, string> > _data;
    };

    /**
    * @brief Query Record. 
    *  
    * @param sSql sql语句
    * @param sSql SQL statement
    * @throws     TC_Mysql_Exception
    * @return     MysqlData类型的数据，可以根据字段获取相关信息
    * @return     MysqlData type of data, you can get information based on the field
    */
    MysqlData queryRecord(const string& sSql);

    /**
    * @brief Query Record.
    *
    * @param sSql sql语句
    * @param sSql SQL statement
    * @param pdatafunc ，函数参数为map<string,string> ,key为column 名，value为数据
    * @param pdatafunc , Function parameter is map<string, string>, key is column name, value is data
    * @throws TC_Mysql_Exception
    * @return MysqlData类型的数据，可以根据字段获取相关信息
    * @return MysqlData type of data, you can get information based on the field
    */
    size_t travelRecord(const string& sSql, const std::function<void(const map<string,string> &)> & pdatafunc);

    /**
     * @brief 定义字段类型， 
     * @brief Define the field type,
     *  DB_INT:数字类型 
     *  DB_INT:Number type
     *  DB_STR:字符串类型
     *  DB_STR:String Type
     */
    enum FT
    {
        DB_INT,     
        DB_STR,    
    };

     /**
     * 数据记录
     * Data Rercord
     */
    typedef map<string, pair<FT, string> > RECORD_DATA;

    /**
    * @brief 更新记录. 
    * @brief Update Records.
    *  
    * @param sTableName 表名
    * @param sTableName Table name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column Name/Value Pair
    * @param sCondition where子语句,例如:where A = B
    * @param sCondition Where clause, for example, where A = B
    * @throws           TC_Mysql_Exception
    * @return           size_t 影响的行数
    * @return           Number of rows affected by size_t
    */
    size_t updateRecord(const string &sTableName, const map<string, pair<FT, string> > &mpColumns, const string &sCondition);

    /**
    * @brief 插入记录. 
    * @brief insert record.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column Name/Value Pair
    * @throws           TC_Mysql_Exception
    * @return           size_t 影响的行数
    * @return           Number of rows affected by size_t
    */
    size_t insertRecord(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

    /**
    * @brief 插入记录(一次插入多条). 
    * @brief Insert a record (insert more than one record at a time).
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对数组
    * @param mpColumns  Column name / value pair array
    * @throws           TC_Mysql_Exception
    * @return           size_t 影响的行数
    * @return           Number of rows affected by size_t 
    */    
    size_t insertRecord(const string &sTableName, const map<string, pair<FT,vector<string>>> &mpColumns);

    /**
    * @brief 替换记录. 
    * @brief Replace record.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns   列名/值对
    * @param mpColumns   Column name / value pair
    * @throws            TC_Mysql_Exception
    * @return            size_t 影响的行数
    * @return            Number of rows affected by size_t 
    */
    size_t replaceRecord(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

   /**
    * @brief 替换记录(一次替换多条). 
    * @brief Replace records (replace multiple records at a time).
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns   列名/值对数组
    * @param mpColumns   Column name / value pair array
    * @throws            TC_Mysql_Exception
    * @return            size_t 影响的行数
    * @return            Number of rows affected by size_t 
    */
    size_t replaceRecord(const string &sTableName, const map<string, pair<FT, vector<string>>> &mpColumns);

    /**
    * @brief 删除记录.  
    * @brief Delete record.
    *  
    * @param sTableName   表名
    * @param sTableName   Table Name
    * @param sCondition   where子语句,例如:where A = B
    * @param sCondition   Where clause, for example, where A = B
    * @throws             TC_Mysql_Exception
    * @return             size_t 影响的行数
    * @return             Number of rows affected by size_t 
    */
    size_t deleteRecord(const string &sTableName, const string &sCondition = "");

    /**
    * @brief 获取Table查询结果的数目. 
    * @brief Number of Tables query results.
    *  
    * @param sTableName 用于查询的表名
    * @param sTableName Table name for query
    * @param sCondition where子语句,例如:where A = B
    * @param sCondition Where clause, for example, where A = B
    * @throws           TC_Mysql_Exception
    * @return           size_t 查询的记录数目
    * @return           size_t Number of records queried
    */
    size_t getRecordCount(const string& sTableName, const string &sCondition = "");

    /**
    * @brief 获取Sql返回结果集的个数. 
    * @brief Gets the number of result sets returned by Sql.
    *  
    * @param sCondition where子语句,例如:where A = B
    * @param sCondition Where clause, for example, where A = B
    * @throws           TC_Mysql_Exception
    * @return           查询的记录数目
    * @return           Number of records queried
    */
    size_t getSqlCount(const string &sCondition = "");

    /**
     * @brief 存在记录. 
     * @brief Existing records.
     *  
     * @param sql  sql语句
     * @param sql  SQL statement
     * @throws     TC_Mysql_Exception
     * @return     操作是否成功
     * @return     Is the operation successful
     */
    bool existRecord(const string& sql);

    /**
    * @brief 获取字段最大值. 
    *  
    * @brief Get the maximum value of a field.
    * @param sTableName 用于查询的表名
    * @param sTableName Table name for query
    * @param sFieldName 用于查询的字段
    * @param sFieldName Fields for Query
    * @param sCondition where子语句,例如:where A = B
    * @param sCondition Where clause, for example, where A = B
    * @throws           TC_Mysql_Exception
    * @return           查询的记录数目
    * @return           Number of records queried
    */
    int getMaxValue(const string& sTableName, const string& sFieldName, const string &sCondition = "");

    /**
    * @brief 获取auto_increment最后插入得ID. 
    * @brief Get the auto_increment last inserted ID.
    *  
    * @return ID值
    * @return ID value
    */
    long lastInsertID();

    /**
    * @brief 构造Insert-SQL语句. 
    * @brief Construct Insert-SQL statements.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column Name/Value Pair
    * @return           string insert-SQL语句
    * @return           String insert-SQL statement
    */
    string buildInsertSQL(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

    /**
    * @brief 构造Insert-SQL语句(一次插入多条). 
    * @brief Construct Insert-SQL statements (insert more than one at a time).
    *  
    * @param sTableName  表名
    * @param sTableName  table name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column Name/Value Pair
    * @return           string insert-SQL语句
    * @return           String insert-SQL statement
    */    
    string buildInsertSQL(const string &sTableName, const map<string, pair<FT, vector<string> >> &mpColumns);

    /**
    * @brief 构造Replace-SQL语句. 
    * @brief Construct Replace-SQL statements.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns 列名/值对
    * @param mpColumns Column name / value pair
    * @return           string insert-SQL语句
    * @return           string insert-SQL statement
    */
    string buildReplaceSQL(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

    /**
    * @brief 构造Replace-SQL语句(一次替换多条). 
    * @brief Construct replace SQL statement (replace multiple statements at a time)
    *  
    * @param sTableName  表名
    * @param sTableName  Tabel Name
    * @param mpColumns 列名/值对
    * @param mpColumns Column name / value pair
    * @return           string insert-SQL语句
    * @return           string insert-SQL statement
    */    
    string buildReplaceSQL(const string &sTableName, const map<string, pair<FT, vector<string> >> &mpColumns);

    /**
    * @brief 构造Update-SQL语句. 
    * @brief Construct update SQL statement
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns   列名/值对
    * @param mpColumns   Column name / value pair
    * @param sCondition  where子语句
    * @param sCondition  where clause
    * @return            string Update-SQL语句
    * @return            string Update-SQL statement
    */
    string buildUpdateSQL(const string &sTableName,const map<string, pair<FT, string> > &mpColumns, const string &sCondition);

    /**
     * @brief 获取最后执行的SQL语句.
     * @brief Get the last SQL statement executed.
     * 
     * @return SQL语句
     * @return SQL statement
     */
    string getLastSQL() { return _sLastSql; }

    /**
     * @brief 获取查询影响数
     * @brief Get query influence number
     * @return int
     */
     size_t getAffectedRows();


    /**
    *  @brief 字符转义, 不考虑字符集(有一定的风险). 
    *  @brief Character escape, regardless of character set (there is a certain risk)
    *  
    * @param sFrom  源字符串
    * @param sFrom  Source string
    * @param sTo    输出字符串
    * @param sTo    Output string
    * @return       输出字符串
    * @return       Output string
    */
    static string escapeString(const string& sFrom);

    /**
    * @brief 构造Insert-SQL语句. 
    * @brief Construct insert SQL statement.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column name / value pair
    * @return           string insert-SQL语句
    * @return           String insert SQL statement
    */
    static string buildInsertSQLNoSafe(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

    /**
    * @brief 构造Insert-SQL语句(批量). 
    * @brief Construct insert SQL statement (batch).
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column name / value pair
    * @return           string insert-SQL语句
    * @return           string insert-SQL statement
    */    
    static string buildInsertSQLNoSafe(const string &sTableName, const map<string, pair<FT, vector<string> >> &mpColumns);

    /**
    * @brief 构造Replace-SQL语句. 
    * @brief Construct replace SQL statement.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns 列名/值对
    * @param mpColumns Column name / value pair
    * @return           string insert-SQL语句
    * @return           string Insert SQL statement
    */
    static string buildReplaceSQLNoSafe(const string &sTableName, const map<string, pair<FT, string> > &mpColumns);

    /**
    * @brief 构造Replace-SQL语句(批量). 
    * @brief Construct replace SQL statement (batch)
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns 列名/值对
    * @param mpColumns Column name / value pair
    * @return           string insert-SQL语句
    * @return           String insert SQL statement
    */    
    static string buildReplaceSQLNoSafe(const string &sTableName, const map<string, pair<FT, vector<string> >> &mpColumns);

    /**
    * @brief 构造Update-SQL语句. 
    * @brief Construct an Update-SQL statement.
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns   列名/值对
    * @param mpColumns   Column Name/Value Pair
    * @param sCondition  where子语句
    * @param sCondition  where cluase
    * @return            string Update-SQL语句
    * @return            string Update-SQL statement
    */
    static string buildUpdateSQLNoSafe(const string &sTableName,const map<string, pair<FT, string> > &mpColumns, const string &sCondition);

protected:
    /**
    * @brief copy contructor，只申明,不定义,保证不被使用 
    * @brief copy contructor. Statement only, undefined, guaranteed not to be used
    */
    TC_Mysql(const TC_Mysql &tcMysql);

    /**
    * 
    * @brief 只申明,不定义,保证不被使用
    * @brief Statement only, undefined, guaranteed not to be used
    */
    TC_Mysql &operator=(const TC_Mysql &tcMysql);

    /**
    * @brief 构造SQL语句(批量). 
    * @brief Construct SQL statements (batches).
    *  
    * @param sTableName  表名
    * @param sTableName  Table Name
    * @param mpColumns  列名/值对
    * @param mpColumns  Column Name/Value Pair
    * @return           string insert-SQL语句
    * @return           string insert-SQL statement
    */   
    string buildSQL(const string &sTableName, const string &command, const map<string, pair<FT, string> > &mpColumns);
    string buildBatchSQL(const string &sTableName, const string &command, const map<string, pair<FT, vector<string> >> &mpColumns);
    static string buildSQLNoSafe(const string &sTableName, const string &command, const map<string, pair<FT, string> > &mpColumns);
    static string buildBatchSQLNoSafe(const string &sTableName, const string &command, const map<string, pair<FT, vector<string> >> &mpColumns);

private:

    /**
    * 数据库指针
    * Database Pointer
    */
    MYSQL         *_pstMql;

    /**
    * 数据库配置
    * Database Configuration
    */
    TC_DBConf   _dbConf;
    
    /**
    * 是否已经连接
    * Is Connected
    */
    bool        _bConnected;

    /**
     * 最后执行的sql
     * Last executed SQL
     */
    string      _sLastSql;
  
};

}
#endif //_TC_MYSQL_H
