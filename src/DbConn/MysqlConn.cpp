#include <cstring>
#include <vector>
#include <list>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <mysql.h>
#include <mysqld_error.h>
#include "Common/Util.h"
#include "Common/Reason.h"
#include "SqlConn.h"
#include "DbUtil.h"
#include "DbReason.h"
#include "util/tc_thread_mutex.h"
#include "servant/Application.h"

typedef unsigned int uint;

// 事件对象结构，使用标准 C++ 同步原语
struct EventObject
{
    std::mutex mutex;
    std::condition_variable cond;
    bool signaled;

    EventObject() : signaled(false) {}
};

// 创建事件对象
extern "C" void* createEvent()
{
    try
    {
        return new EventObject();
    }
    catch (...)
    {
        return nullptr;
    }
}

// 等待事件，timeout < 0 表示无限等待，timeout 单位为毫秒
extern "C" bool waitEvent(void *event, int timeout)
{
    if (event == nullptr)
        return false;

    EventObject *evt = static_cast<EventObject*>(event);
    
    std::unique_lock<std::mutex> lock(evt->mutex);
    
    if (evt->signaled)
    {
        evt->signaled = false;
        return true;
    }

    if (timeout < 0)
    {
        // 无限等待
        evt->cond.wait(lock, [evt] { return evt->signaled; });
        evt->signaled = false;
        return true;
    }
    else
    {
        // 带超时的等待
        auto status = evt->cond.wait_for(lock, std::chrono::milliseconds(timeout), 
                                         [evt] { return evt->signaled; });
        bool result = evt->signaled;
        evt->signaled = false;
        return result;
    }
}

// 设置事件
extern "C" void setEvent(void *event)
{
    if (event == nullptr)
        return;

    EventObject *evt = static_cast<EventObject*>(event);
    
    std::lock_guard<std::mutex> lock(evt->mutex);
    evt->signaled = true;
    evt->cond.notify_one();
}

// 销毁事件对象
extern "C" void destroyEvent(void *event)
{
    if (event != nullptr)
    {
        delete static_cast<EventObject*>(event);
    }
}

namespace DbConn
{
    namespace SQL
    {
        class MysqlStmtI;
        class MysqlConnICtx;
        class MysqlConnPoolICtx;
        class MysqlTxnConnI;

        typedef std::shared_ptr<MysqlStmtI> MysqlStmtIPtr;
        typedef std::shared_ptr<MysqlConnICtx> MysqlConnICtxPtr;
        typedef std::shared_ptr<MysqlConnPoolICtx> MysqlConnPoolICtxPtr;
        typedef std::shared_ptr<MysqlTxnConnI> MysqlTxnConnIPtr;

        static bool clientError( unsigned int err)
        {
            return err >= 2000;
        }

        class Init: public tars::TC_ThreadRecMutex
        {
        public:
            Init()
            {
                mysql_library_init( 0, 0, 0);
            }

            ~Init()
            {
                mysql_library_end();
            }
        };

        static Init _init;

        // static void initStatistics( const ApplicationPtr &application)
        // {
        //     std::lock_guard<tars::TC_ThreadRecMutex> lock( _init);
        //     std::string v;

        //     if( !application->getStatistic( "Mysql.ConnActivatedNum", v))
        //     {
        //         application->setStatisticsLong( "Mysql.WaitConnNum", 0);
        //         application->setStatisticsLong( "Mysql.GetConnNum", 0);

        //         application->setStatisticsLong( "Mysql.WaitConnNum", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_0_100ms", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_100_500ms", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_500_1000ms", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_1_5s", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_5_10s", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_10_30s", 0);
        //         application->setStatisticsLong( "Mysql.WaitConnNum_over30s", 0);

        //         application->setStatisticsLong( "Mysql.ConnActivatedNum", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_0_100ms", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_100_500ms", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_500_1000ms", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_1_5s", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_5_10s", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_10_30s", 0);
        //         application->setStatisticsLong( "Mysql.ConnActivatedNum_over30s", 0);
        //     }
        // }

        // static void addStatisticsWait( const ApplicationPtr &application)
        // {
        //     application->addStatisticsLong( "Mysql.WaitConnNum", 1);
        // }

        // static void addStatisticsGet(unsigned long time)
        // {
        //     application->addStatisticsLong( "Mysql.GetConnNum", 1);
        //     if( time <= 100)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_0_100ms", 1);
        //     else if( time <= 500)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_100_500ms", 1);
        //     else if( time <= 1000)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_500_1000ms", 1);
        //     else if( time <= 5000)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_1_5s", 1);
        //     else if( time <= 10000)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_5_10s", 1);
        //     else if( time <= 30000)
        //         application->addStatisticsLong( "Mysql.WaitConnNum_10_30s", 1);
        //     else
        //         application->addStatisticsLong( "Mysql.WaitConnNum_over30s", 1);
        // }

        // static void addStatisticsActivated(unsigned long time)
        // {
        //     application->addStatisticsLong( "Mysql.ConnActivatedNum", 1);
        //     if( time <= 100)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_0_100ms", 1);
        //     else if( time <= 500)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_100_500ms", 1);
        //     else if( time <= 1000)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_500_1000ms", 1);
        //     else if( time <= 5000)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_1_5s", 1);
        //     else if( time <= 10000)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_5_10s", 1);
        //     else if( time <= 30000)
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_10_30s", 1);
        //     else
        //         application->addStatisticsLong( "Mysql.ConnActivatedNum_over30s", 1);
        // }

        class ResultI: public Result
        {
            MYSQL_RES *_res;
            unsigned int _fnum;
            MYSQL_ROW _row;
            unsigned long *_lens;

        public:
            ResultI( MYSQL_RES *res)
            {
                _res = res;
                _fnum = mysql_num_fields( _res);
            }

            virtual ~ResultI()
            {
                mysql_free_result( _res);
            }

            virtual int fetch()
            {
                _row = mysql_fetch_row( _res);
                if( _row)
                {
                    _lens = mysql_fetch_lengths( _res);
                    return 0;
                }

                return 1;
            }

            virtual std::string getResult( int idx)
            {
                if( idx < (int)_fnum)
                    return std::string( (const char*)_row[idx], _lens[idx]);
                return "";
            }
        };

        struct ExecCtx
        {
            MysqlStmtI* _curStmt;
            bool _committed;
            int _err;
        };

        union ParamBuffer
        {
            char c;
            short s;
            int i;
            int64_t l;
            float f;
            double d;
            char *str;
        };

        class MysqlStmtI: public Stmt
        {
            MYSQL_STMT *_stmt;
            ExecCtx *_ctx;

            std::vector<MYSQL_BIND> _params;
            std::vector<MYSQL_BIND> _result;
            std::vector<ParamBuffer> _pBuf;

            std::string _name;
            std::string _query;
            bool _ready;
            bool _bind;

        public:
            MysqlStmtI( const std::string &name, MYSQL *conn, ExecCtx *ctx)
            {
                _name = name;
                _stmt = mysql_stmt_init( conn);
                _ctx = ctx;
            }

            virtual ~MysqlStmtI()
            {
                for( size_t i=0; i<_params.size(); i++)
                    if( _params[i].buffer_type==MYSQL_TYPE_VAR_STRING || _params[i].buffer_type==MYSQL_TYPE_BLOB)
                        delete [] _pBuf[i].str;
                mysql_stmt_free_result( _stmt);
                mysql_stmt_close( _stmt);
            }

        private:
            void initPBind( int idx)
            {
                if( idx >= (int)_params.size())
                {
                    size_t c = _params.size();

                    _params.resize( idx+1);
                    memset( &_params[c], 0, (_params.size()-c)*sizeof( MYSQL_BIND));
                    _pBuf.resize( idx+1);
                    _bind = false;
                } else
                {
                    if( _params[idx].buffer_type==MYSQL_TYPE_VAR_STRING || _params[idx].buffer_type==MYSQL_TYPE_BLOB)
                        delete [] _pBuf[idx].str;
                }
            }

            void bindPBind()
            {
                if( !_bind)
                {
                    for( size_t i=0; i<_pBuf.size(); i++)
                    {
                        if( _params[i].buffer_type == MYSQL_TYPE_TINY)
                            _params[i].buffer = &_pBuf[i].c;
                        else if( _params[i].buffer_type == MYSQL_TYPE_SHORT)
                            _params[i].buffer = &_pBuf[i].s;
                        else if( _params[i].buffer_type == MYSQL_TYPE_LONG)
                            _params[i].buffer = &_pBuf[i].i;
                        else if( _params[i].buffer_type == MYSQL_TYPE_LONGLONG)
                            _params[i].buffer = &_pBuf[i].l;
                        else if( _params[i].buffer_type == MYSQL_TYPE_FLOAT)
                            _params[i].buffer = &_pBuf[i].f;
                        else if( _params[i].buffer_type == MYSQL_TYPE_DOUBLE)
                            _params[i].buffer = &_pBuf[i].d;
                    }

                    _bind = true;
                }
            }

            void initRBind( int idx)
            {
                if( idx >= (int)_result.size())
                {
                    size_t c = _result.size();

                    _result.resize( idx+1);
                    memset( &_result[c], 0, (_result.size()-c)*sizeof( MYSQL_BIND));
                }
            }

            bool prepare()
            {
                if( !_ready)
                {
                    std::this_thread::yield();
                    if( !mysql_stmt_prepare( _stmt, _query.c_str(), _query.size()))
                        _ready = true;
                }

                return _ready;
            }

        public:
            void init( const std::string &query)
            {
                _query = query;
                _ready = false;
                _bind = false;
            }

            virtual std::string name() const
            {
                return _name;
            }

            virtual std::string query() const
            {
                return _query;
            }

            virtual void bindParam( int idx, char val)
            {
                initPBind( idx);
                _pBuf[idx].c = val;
                _params[idx].buffer_type = MYSQL_TYPE_TINY;
            }

            virtual void bindParam( int idx, short val)
            {
                initPBind( idx);
                _pBuf[idx].s = val;
                _params[idx].buffer_type = MYSQL_TYPE_SHORT;
            }

            virtual void bindParam( int idx, int val)
            {
                initPBind( idx);
                _pBuf[idx].i = val;
                _params[idx].buffer_type = MYSQL_TYPE_LONG;
            }

            virtual void bindParam( int idx, int64_t val)
            {
                initPBind( idx);
                _pBuf[idx].l = val;
                _params[idx].buffer_type = MYSQL_TYPE_LONGLONG;
            }

            virtual void bindParam( int idx, float val)
            {
                initPBind( idx);
                _pBuf[idx].f = val;
                _params[idx].buffer_type = MYSQL_TYPE_FLOAT;
            }

            virtual void bindParam( int idx, double val)
            {
                initPBind( idx);
                _pBuf[idx].d = val;
                _params[idx].buffer_type = MYSQL_TYPE_DOUBLE;
            }

            virtual void bindParam( int idx, char *str, unsigned long len)
            {
                initPBind( idx);
                _pBuf[idx].str = str;
                _params[idx].buffer_type = MYSQL_TYPE_STRING;
                _params[idx].buffer_length = len;
                _params[idx].buffer = _pBuf[idx].str;
            }

            virtual void bindParam( int idx, const std::string &val)
            {
                initPBind( idx);
                _pBuf[idx].str = new char[val.size()];
                memcpy( _pBuf[idx].str, val.c_str(), val.size());
                _params[idx].buffer_type = MYSQL_TYPE_VAR_STRING;
                _params[idx].buffer_length = val.size();
                _params[idx].buffer = _pBuf[idx].str;
            }

            virtual void bindParam( int idx, const tars::BufferReader &val)
            {
                int len = val.size();
                const unsigned char *data = (const unsigned char*)val.base();

                initPBind( idx);
                _pBuf[idx].str = new char[len];
                memcpy( _pBuf[idx].str, data, len);
                _params[idx].buffer_type = MYSQL_TYPE_BLOB;
                _params[idx].buffer_length = len;
                _params[idx].buffer = _pBuf[idx].str;
            }

            virtual void bindResult( int idx, char &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_TINY;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, short &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_SHORT;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, int &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_LONG;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, int64_t &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_LONGLONG;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, float &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_FLOAT;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, double &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_DOUBLE;
                _result[idx].buffer = &val;
            }

            virtual void bindResult( int idx, char *str, unsigned long &len)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_STRING;
                _result[idx].buffer = str;
                _result[idx].buffer_length = len;
                _result[idx].length = &len;
            }

            virtual void bindResult( int idx, std::string &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_VAR_STRING;
                _result[idx].buffer = &val;
                _result[idx].buffer_length = 0;
                _result[idx].length = 0;
            }

            virtual void bindResult( int idx, tars::BufferReader &val)
            {
                initRBind( idx);
                _result[idx].buffer_type = MYSQL_TYPE_BLOB;
                _result[idx].buffer = &val;
                _result[idx].buffer_length = 0;
                _result[idx].length = 0;
            }

            virtual bool execute()
            {
                _ctx->_curStmt = this;
                if( prepare())
                {
                    bindPBind();
                    if( !_params.size() || !mysql_stmt_bind_param( _stmt, &_params[0]))
                        if( !_result.size() || !mysql_stmt_bind_result( _stmt, &_result[0]))
                        {
                            std::this_thread::yield();
                            if( !mysql_stmt_execute( _stmt))
                            {
                                _ctx->_committed = false;
                                if( !_result.size() || !mysql_stmt_store_result( _stmt))
                                    return true;
                            }
                        }
                }

                if( clientError( getLastReasonCode()))
                {
                    TLOGERROR("[DbConn] SQL::Stmt::execute() failed, reason=[%s]", getLastReason().c_str());
                    _ctx->_err = -1;
                }

                return false;
            }

            virtual int64_t affectedRows()
            {
                return mysql_stmt_affected_rows( _stmt);
            }

            virtual int fetch()
            {
                int r = mysql_stmt_fetch( _stmt);

                if( !r)
                {
                    for( size_t i=0; i<_result.size(); i++)
                    {
                        if (_result[i].buffer_type == MYSQL_TYPE_VAR_STRING)
                        {
                            MYSQL_BIND bind;
                            my_bool isNull = false;
                            vector<char> val;
                            unsigned long len = 0;

                            memset( &bind, 0, sizeof( MYSQL_BIND));
                            bind.is_null = &isNull;
                            bind.buffer_type = MYSQL_TYPE_VAR_STRING;
                            bind.length = &len;
                            mysql_stmt_fetch_column( _stmt, &bind, i, 0);

                            if (len == 0)
                            {
                                ((std::string*)_result[i].buffer)->clear();
                            }
                            else if (!isNull)
                            {
                                val.resize( len);
                                bind.buffer = &val[0];
                                bind.buffer_length = len;
                                mysql_stmt_fetch_column( _stmt, &bind, i, 0);
                                *((std::string*)_result[i].buffer) = std::string( &val[0], val.size());
                            }
                        }
                        else if (_result[i].buffer_type == MYSQL_TYPE_BLOB)
                        {
                            MYSQL_BIND bind;
                            my_bool isNull = false;
                            vector<unsigned char> val;
                            unsigned long len = 0;

                            memset( &bind, 0, sizeof( MYSQL_BIND));
                            bind.is_null = &isNull;
                            bind.buffer_type = MYSQL_TYPE_BLOB;
                            bind.length = &len;
                            mysql_stmt_fetch_column( _stmt, &bind, i, 0);

                            if (len == 0)
                            {
                                ((tars::BufferReader*)_result[i].buffer)->setBuffer(nullptr, 0);
                            }
                            else if (!isNull)
                            {
                                val.resize( len);
                                bind.buffer = &val[0];
                                bind.buffer_length = len;
                                mysql_stmt_fetch_column( _stmt, &bind, i, 0);
                                ((tars::BufferReader*)_result[i].buffer)->setBuffer((const char*)&val[0], val.size());
                            }
                        }
                    }
                    return 0;
                } else if( r == MYSQL_NO_DATA)
                    return 1;
                else
                {
                    TLOGERROR("[DbConn] SQL::Stmt::fetch() failed, reason=[%s]", getLastReason().c_str());
                    _ctx->_err = -1;
                    return -1;
                }
            }

            virtual int64_t insertId()
            {
                return mysql_stmt_insert_id( _stmt);
            }

            virtual unsigned int getLastReasonCode()
            {
                return mysql_stmt_errno( _stmt);
            }

            virtual std::string getLastReason()
            {
                return mysql_stmt_error( _stmt);
            }
        };

        class HostInfo: public MysqlConfig
        {
        public:
            bool activated;

            HostInfo( const MysqlConfig &host): MysqlConfig( host)
            {
                activated = true;
            }
        };

        class Event
        {
            void *_evt;
            bool _sign;

        public:
            def_link_node( Event) node;

            Event(void* evt)
            {
                link_node_init(node);
                _evt = evt;
                _sign = false;
            }

            ~Event()
            {
                destroyEvent( _evt);
            }

            void set()
            {
                _sign = true;
                setEvent( _evt);
            }

            void wait( int timeout)
            {
                waitEvent( _evt, timeout);
            }

            bool quit()
            {
                return !_sign;
            }
        };

        class MysqlConnPoolICtx: public tars::TC_ThreadRecMutex
        {
        public:
            //ApplicationPtr application;
            std::vector<HostInfo> hosts;
            std::string db;

            bool activated;
            int connectTimeout;
            int connNum;
            int maxConnNum;
            int maxWaitConnNum;
            std::list<MysqlConnICtxPtr> conns;
            def_link_entity( Event) waits;
        };

        class MysqlConnICtx:
            public ExecCtx,
            virtual public Shared
        {
        public:
            MysqlConnPoolICtxPtr _pool;
            MYSQL *_conn;
            std::map<std::string, MysqlStmtIPtr> _stmts;

            unsigned long _ticks;

        public:
            MysqlConnICtx( const MysqlConnPoolICtxPtr &pool)
            {
                _pool = pool;
                _conn = 0;
                _committed = true;
                _err = 0;
            }

            virtual ~MysqlConnICtx()
            {
                close();
            }

        public:
            unsigned long ticks() const
            {
                return _ticks;
            }

            bool login( std::string &reason)
            {
                if( !_conn)
                {
                    _conn = mysql_init( 0);
                    if( _conn)
                    {
                        my_bool dataTruncation = 0;
                        std::vector<HostInfo> hosts;
                        bool empty = true;

                        if( _pool->connectTimeout)
                            mysql_optionsv( _conn, MYSQL_OPT_CONNECT_TIMEOUT, &_pool->connectTimeout);
                        mysql_optionsv( _conn, MYSQL_REPORT_DATA_TRUNCATION, &dataTruncation);
                        mysql_optionsv( _conn, MYSQL_SET_CHARSET_NAME, "utf8mb4");

                        my_bool use = 0;
                        mysql_optionsv( _conn, MYSQL_OPT_SSL_ENFORCE, &use);

                        {
                            std::lock_guard<tars::TC_ThreadRecMutex> lock( *_pool);
                            hosts = _pool->hosts;
                        }

                        for( size_t i=0; i<hosts.size(); i++)
                        {
                            if( hosts[i].activated)
                            {
                                std::this_thread::yield();
                                if( mysql_real_connect( _conn, hosts[i].host().c_str(), hosts[i].user().c_str(), hosts[i].pwd().c_str(), _pool->db.c_str(), hosts[i].port(), 0, 0))
                                    return true;
                                else
                                {
                                    reason = mysql_error( _conn);
                                    TLOGERROR("[DbConn] mysql_real_connect() failed, host=[" << _pool->hosts[i].host() << "], port=[" << hosts[i].port() << "], reason=[" << reason << "]" << endl);

                                    std::lock_guard<tars::TC_ThreadRecMutex> lock( *_pool);
                                    _pool->hosts[i].activated = false;
                                    empty = false;
                                }
                            }
                        }

                        if( empty)
                        {
                            std::this_thread::yield();
                            if( mysql_real_connect( _conn, hosts[0].host().c_str(), hosts[0].user().c_str(), hosts[0].pwd().c_str(), _pool->db.c_str(), hosts[0].port(), 0, 0))
                                return true;
                            else
                            {
                                reason = mysql_error( _conn);
                                TLOGERROR("[DbConn] mysql_real_connect() failed, host=[" << _pool->hosts[0].host() << "], port=[" << hosts[0].port() << "], reason=[" << reason << "]" << endl);
                            }
                        }

                        close();
                    }
                }

                return false;
            }

            void release()
            {
                std::lock_guard<tars::TC_ThreadRecMutex> lock( *_pool);

                if( _pool->activated && _pool->connNum<=_pool->maxConnNum)
                {
                    _ticks = TNOWMS;
                    _pool->conns.push_back( this);

                    if( link_node_num( _pool->waits))
                    {
                        link_head( _pool->waits)->set();
                        link_remove_head( _pool->waits, node);
                    }
                } else
                    close();
            }

            void close()
            {
                if( _conn)
                {
                    std::this_thread::yield();
                    _stmts.clear();
                    mysql_close( _conn);

                    _curStmt = 0;
                    _conn = 0;

                    {
                        std::lock_guard<tars::TC_ThreadRecMutex> lock( *_pool);

                        _pool->connNum--;

                        if( link_node_num( _pool->waits))
                        {
                            link_head( _pool->waits)->set();
                            link_remove_head( _pool->waits, node);
                        }
                    }

                    _pool = 0;
                }
            }
        };

        class MysqlConnI: public Conn
        {
            unsigned long _activateTicks;

        protected:
            MysqlConnICtxPtr _ctx;

        public:
            MysqlConnI( const MysqlConnICtxPtr &ctx)
            {
                _activateTicks = TNOWMS;
                _ctx = ctx;
            }

            virtual ~MysqlConnI()
            {
                //addStatisticsActivated( _ctx->_pool->application, TNOWMS-_activateTicks);
                if( !_ctx->_err)
                    _ctx->release();
                else
                    _ctx->close();
            }

            virtual StmtPtr insertStmt( const std::string &name, const std::string &query)
            {
                MysqlStmtIPtr stmt = new MysqlStmtI( name, _ctx->_conn, _ctx.get());

                stmt->init( query);
                _ctx->_stmts[name] = stmt;

                return stmt;
            }

            virtual StmtPtr findStmt( const std::string &name) const
            {
                if( _ctx->_stmts.find( name) != _ctx->_stmts.end())
                    return _ctx->_stmts.find( name)->second;
                else
                    return 0;
            }

            virtual StmtPtr initStmt( const std::string &name, const std::string &query)
            {
                StmtPtr r = findStmt( name);

                if( r)
                    return r;
                else
                    return insertStmt( name, query);
            }

            virtual void removeStmt( const std::string &name)
            {
                _ctx->_stmts.erase( name);
            }

            virtual void clearStmt()
            {
                _ctx->_stmts.clear();
            }

            virtual bool execute( const std::string &query)
            {
                _ctx->_curStmt = 0;
                if( !mysql_query( _ctx->_conn, query.c_str()))
                {
                    _ctx->_committed = false;
                    return true;
                }

                if( clientError( getLastReasonCode()))
                {
                    TLOGERROR("[DbConn] SQL::Conn::execute() failed, reason=[" << getLastReason() << "]" << endl);
                    _ctx->_err = -1;
                }

                return false;
            }

            virtual bool execute( const std::string &query, ResultPtr &result)
            {
                _ctx->_curStmt = 0;
                if( !mysql_query( _ctx->_conn, query.c_str()))
                {
                    MYSQL_RES *res = mysql_store_result( _ctx->_conn);

                    _ctx->_committed = false;
                    result = res ? new ResultI( res):0;
                    return true;
                }

                if( clientError( getLastReasonCode()))
                {
                    TLOGERROR("[DbConn] SQL::Conn::execute() failed, reason=[" << getLastReason() << "]" << endl);
                    _ctx->_err = -1;
                }

                return false;
            }

            virtual int64_t affectedRows()
            {
                return mysql_affected_rows( _ctx->_conn);
            }

            virtual int64_t insertId()
            {
                return mysql_insert_id( _ctx->_conn);
            }

            virtual bool commit()
            {
                std::this_thread::yield();
                if( !mysql_commit( _ctx->_conn))
                {
                    _ctx->_committed = true;
                    return true;
                }

                TLOGERROR("[DbConn] SQL::Conn::commit() failed, reason=[" << getLastReason() << "]" << endl);
                _ctx->_err = -1;
                return false;
            }

            virtual bool rollback()
            {
                std::this_thread::yield();
                if( !mysql_rollback( _ctx->_conn))
                {
                    _ctx->_committed = true;
                    return true;
                }

                TLOGERROR("[DbConn] SQL::Conn::rollback() failed, reason=[" << getLastReason() << "]" << endl);
                _ctx->_err = -1;
                return false;
            }

            virtual bool autoCommit( bool mode)
            {
                std::this_thread::yield();
                if( !mysql_autocommit( _ctx->_conn, mode))
                    return true;

                TLOGERROR("[DbConn] SQL::Conn::autoCommit() failed, reason=[" << getLastReason() << "]" << endl);
                _ctx->_err = -1;
                return false;
            }

            virtual unsigned int getLastReasonCode()
            {
                if( _ctx->_curStmt)
                    return _ctx->_curStmt->getLastReasonCode();
                else
                    return mysql_errno( _ctx->_conn);
            }

            virtual std::string getLastReason()
            {
                if( _ctx->_curStmt)
                    return _ctx->_curStmt->getLastReason();
                else
                    return mysql_error( _ctx->_conn);
            }

            virtual bool tableNotFound()
            {
                return getLastReasonCode()==ER_NO_SUCH_TABLE;
            }

            virtual bool isDeadLock()
            {
                return getLastReasonCode()==ER_LOCK_DEADLOCK;
            }
        };

        class MysqlTxnConnI:
            public TxnConn,
            public MysqlConnI
        {
        public:
            MysqlTxnConnI( const MysqlConnICtxPtr &ctx): MysqlConnI( ctx)
            {
            }

            virtual ~MysqlTxnConnI()
            {
                if( !_ctx->_err && !_ctx->_committed)
                    rollback();
                if( !_ctx->_err)
                    autoCommit( true);
            }

            bool init()
            {
                return autoCommit( false);
            }

            virtual StmtPtr insertStmt( const std::string &name, const std::string &query)
            {
                return MysqlConnI::insertStmt( name, query);
            }

            virtual StmtPtr findStmt( const std::string &name) const
            {
                return MysqlConnI::findStmt( name);
            }

            virtual StmtPtr initStmt( const std::string &name, const std::string &query)
            {
                return MysqlConnI::initStmt( name, query);
            }

            virtual void removeStmt( const std::string &name)
            {
                return MysqlConnI::removeStmt( name);
            }

            virtual void clearStmt()
            {
                return MysqlConnI::clearStmt();
            }

            virtual bool execute( const std::string &query)
            {
                return MysqlConnI::execute( query);
            }

            virtual bool execute( const std::string &query, ResultPtr &result)
            {
                return MysqlConnI::execute( query, result);
            }

            virtual int64_t affectedRows()
            {
                return MysqlConnI::affectedRows();
            }

            virtual int64_t insertId()
            {
                return MysqlConnI::insertId();
            }

            virtual bool commit()
            {
                return MysqlConnI::commit();
            }

            virtual bool rollback()
            {
                return MysqlConnI::rollback();
            }

            virtual bool autoCommit( bool mode)
            {
                return MysqlConnI::autoCommit( mode);
            }

            virtual unsigned int getLastReasonCode()
            {
                return MysqlConnI::getLastReasonCode();
            }

            virtual std::string getLastReason()
            {
                return MysqlConnI::getLastReason();
            }

            virtual bool tableNotFound()
            {
                return MysqlConnI::tableNotFound();
            }

            virtual bool isDeadLock()
            {
                return MysqlConnI::isDeadLock();
            }
        };

        class MysqlConnPoolI: public ConnPool
        {
            MysqlConnPoolICtxPtr _ctx;
            int _waitTimeout;
            unsigned long _timeoutTicks;
            unsigned long _resetTicks;

        public:
            MysqlConnPoolI(const std::vector<MysqlConfig> &hosts, const std::string &db)
            {
                //initStatistics( application);
                _ctx = MysqlConnPoolICtxPtr(new MysqlConnPoolICtx());
                //_ctx->application = application;
                for( size_t i=0; i<hosts.size(); i++)
                    _ctx->hosts.push_back( HostInfo( hosts[i]));
                _ctx->db = db;
                _ctx->activated = true;
                _ctx->connNum = 0;
                _resetTicks = _timeoutTicks = TNOWMS;
                loadConfig();
                link_init( _ctx->waits);
            }

            virtual ~MysqlConnPoolI()
            {
                close();
            }

        private:
            void loadConfig()
            {
                _ctx->connectTimeout = 5;
                //_ctx->application->getAppConfigAsInt( "MysqlConnectTimeout", _ctx->connectTimeout);

                _ctx->maxConnNum = 50;
                //_ctx->application->getAppConfigAsInt( "MysqlMaxConnection", _ctx->maxConnNum);

                _ctx->maxWaitConnNum = 100;
                //_ctx->application->getAppConfigAsInt( "MysqlMaxWaitConnection", _ctx->maxWaitConnNum);

                int timeout = 10;
                //_ctx->application->getAppConfigAsInt( "MysqlWaitConnectionTimeout", timeout);
                _waitTimeout = timeout>0 ? timeout*1000:-1;
            }

            MysqlConnICtxPtr getConnCtx( std::string &reason)
            {
                MysqlConnICtxPtr connCtx;
                bool schd = false;

                while( true)
                {
                    std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);

                    if( _ctx->conns.size())
                    {
                        connCtx = _ctx->conns.back();
                        _ctx->conns.pop_back();
                    } else
                    {
                        if( _ctx->connNum < _ctx->maxConnNum)
                        {
                            connCtx = new MysqlConnICtx( _ctx);

                            _ctx->connNum++;
                            {
                                TmpUnlock unlock( _ctx.get());

                                if( !schd)
                                {
                                    std::this_thread::yield();
                                    schd = true;
                                }

                                if( !connCtx->login( reason))
                                    connCtx = 0;
                            }
                        } else
                        {
                            if( link_node_num( _ctx->waits) < _ctx->maxWaitConnNum)
                            {
                                void* e = createEvent();
                                if (e == NULL)
                                {
                                    reason = SERVER_BUSY ":createEvent fail";
                                    break;
                                }
                                Event evt(e);

                                link_add_tail( _ctx->waits, &evt, node);
                                {
                                    int timeout = _waitTimeout;

                                    {
                                        TmpUnlock unlock( _ctx.get());
                                        evt.wait( timeout);
                                    }

                                    if( evt.quit())
                                    {
                                        link_remove_node( _ctx->waits, &evt, node);
                                        reason = TIMEOUT_ERROR;
                                        break;
                                    }
                                }

                                continue;
                            } else
                            {
                                reason = SERVER_BUSY;
                                break;
                            }
                        }
                    }

                    break;
                }

                if( connCtx && !schd)
                {
                    std::this_thread::yield();
                    schd = true;
                }

                return connCtx;
            }

            virtual ConnPtr getConnection( std::string &reason)
            {
                unsigned long ticks = TNOWMS;
                //addStatisticsWait( _ctx->application);
                MysqlConnICtxPtr connCtx = getConnCtx( reason);
                //addStatisticsGet( _ctx->application, TNOWMS-ticks);

                if( connCtx)
                    return new MysqlConnI( connCtx);

                TLOGERROR("[DbConn] SQL::ConnPool::getConnection() failed, reason=[" << reason << "]" << endl);
                return 0;
            }

            virtual ConnPtr getConnection()
            {
                std::string reason;
                return getConnection( reason);
            }

            virtual TxnConnPtr getTransactConnection( std::string &reason)
            {
                MysqlConnICtxPtr connCtx = getConnCtx( reason);

                if( connCtx)
                {
                    MysqlTxnConnIPtr conn = new MysqlTxnConnI( connCtx);

                    if( conn->init())
                        return conn;
                    else
                        reason = conn->getLastReason();
                }

                TLOGERROR("[DbConn] SQL::ConnPool::getTransactConnection() failed, reason=[" << reason << "]" << endl);
                return 0;
            }

            virtual TxnConnPtr getTransactConnection()
            {
                std::string reason;
                return getTransactConnection( reason);
            }

            virtual void schd()
            {
                if( TNOWMS-_timeoutTicks > 1*1000)
                {
                    std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);
                    int period = 30*1000;
                    unsigned long ticks = TNOWMS;

                    loadConfig();

                    //_ctx->application->getAppConfigAsInt( "MysqlInactivatedTimeout", period);
                    for( std::list<MysqlConnICtxPtr>::iterator it=_ctx->conns.begin(); it!=_ctx->conns.end();)
                    {
                        if( ticks-(*it)->ticks() > (unsigned int)period)
                        {
                            (*it)->close();
                            it = _ctx->conns.erase( it);
                        } else
                            break;
                    }

                    _timeoutTicks = TNOWMS;
                }

                if( TNOWMS-_resetTicks > 3*60*1000)
                {
                    std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);

                    for( size_t i=0; i<_ctx->hosts.size(); i++)
                        _ctx->hosts[i].activated = true;

                    _resetTicks = TNOWMS;
                }
            }

            virtual void close()
            {
                MysqlConnPoolICtxPtr ctx = _ctx;
                std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);

                if( _ctx->activated)
                {
                    _ctx->activated = false;
                    for( std::list<MysqlConnICtxPtr>::iterator it=_ctx->conns.begin(); it!=_ctx->conns.end(); it++)
                        (*it)->close();
                }            
            }
        };

        MysqlConfig::MysqlConfig( const std::string &uri)
        {
            URI u( uri);

            if( !u.proto().size() || u.proto()=="mysql")
            {
                _host = u.host();
                _port = u.port().toInt( 3306);
                if( u.path().size())
                    _db = u.path().substr( 1);
                _user = u.param( "user");
                _pwd = u.param( "pwd");
           }
        }

        MysqlConfig::MysqlConfig( const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd)
        {
            _host = host;
            _port = port;
            _db = db;
            _user = user;
            _pwd = pwd;
        }

        const std::string& MysqlConfig::host() const
        {
            return _host;
        }

        int MysqlConfig::port() const
        {
            return _port;
        }

        const std::string& MysqlConfig::db() const
        {
            return _db;
        }

        const std::string& MysqlConfig::user() const
        {
            return _user;
        }
        const std::string& MysqlConfig::pwd() const
        {
            return _pwd;
        }

        ConnPoolPtr ConnPool::createForMysql(const std::string &uri)
        {
            std::vector<MysqlConfig> hosts;

            // 支持多host地址，mysql集群
            URI u(uri);
            std::list<std::string> mysqlHosts, mysqlPorts;
            std::string strHost,strPort,strUser,strPwd,strDb;
            if ( u.valid())
            {
                if (u.proto() == "mysql")
                    u.addrs(mysqlHosts, mysqlPorts);
                strDb = u.path().substr(1);
                strUser = u.param("user");
                strPwd = u.param("pwd");
                std::list<std::string>::const_iterator it1 = mysqlHosts.begin();
                std::list<std::string>::const_iterator it2 = mysqlPorts.begin();
                do
                {
                    hosts.push_back( DbConn::SQL::MysqlConfig( *it1,it2->toInt(-1),strDb,strUser,strPwd));
                    ++it1, ++it2;
                } while (it1 != mysqlHosts.end());
            }

            return new MysqlConnPoolI( hosts, strDb);
        }

        ConnPoolPtr ConnPool::createForMysql(const std::string &uri, const std::string &db)
        {
            std::vector<MysqlConfig> hosts;

            // 支持多host地址，mysql集群
            URI u(uri);
            std::list<std::string> mysqlHosts, mysqlPorts;
            std::string strHost,strPort,strUser,strPwd,strDb;
            if (u.valid())
            {
                if (u.proto() == "mysql")
                    u.addrs(mysqlHosts, mysqlPorts);
                strDb = u.path().substr(1);
                strUser = u.param("user");
                strPwd = u.param("pwd");

                std::list<std::string>::const_iterator it1 = mysqlHosts.begin();
                std::list<std::string>::const_iterator it2 = mysqlPorts.begin();
                do
                {
                    hosts.push_back( DbConn::SQL::MysqlConfig( *it1,it2->toInt(-1),strDb,strUser,strPwd));
                    ++it1, ++it2;
                } while (it1 != mysqlHosts.end());
            }

            if( strDb.empty())
                strDb = db;
            return new MysqlConnPoolI( hosts, strDb);
        }

        ConnPoolPtr ConnPool::createForMysql(const std::string &host, int port, const std::string &user, const std::string &pwd, const std::string &db)
        {
            vector<MysqlConfig> hosts;

            hosts.push_back( MysqlConfig( host, port, db, user, pwd));
            return new MysqlConnPoolI( hosts, db);
        }

        ConnPoolPtr  ConnPool::createForMysql(const std::vector<MysqlConfig> &hosts, const std::string &db)
        {
            return new MysqlConnPoolI( hosts, db);
        }
    }
}
