#include "DbUtil.h"
#include "SqlConn.h"
#include <libpq-fe.h>


using namespace DbConn::SQL;

class StmtI;
class PGConnPoolCtx;
class PGConnI;

typedef Handle<StmtI> StmtIPtr;
typedef Handle<PGConnPoolCtx> PGConnPoolCtxPtr;
typedef Handle<PGConnI> PGConnIPtr;

class StmtI: public Stmt
{
    PGconn *_conn;
    PGresult *_result;

    std::string _name;
    std::string _query;
    bool _ready;
    bool _bind;

public:
    StmtI( const std::string &name, PGconn *conn)
    {
        _name = name;
        _conn = conn;
    }

    virtual ~StmtI()
    {
        
    }

private:

public:
    void init( const std::string &query)
    {
        _query = query;
        _ready = false;
        _bind = false;
    }

    std::string name() const
    {
        return _name;
    }


};

class PGConnPoolCtx
{
public:
    ApplicationPtr application;
    PGConfig host;

    bool activated;
    int connNum;
    int maxConnNum;
    std::list<PGConnIPtr> conns;
    void *event;
    int waitNum;

    PGConnPoolCtx( const PGConfig &host1): host( host1)
    {
    }
};

class PGConnI: public Conn
{
    PGConnPoolCtxPtr _ctx;
    PGconn *_conn;
    
    //map<String, StmtIPtr> _stmts;
    unsigned int _ticks;

public:
    PGConnI( const PGConnPoolCtxPtr &ctx)
    {
        _ctx = ctx;
        _conn = 0;
    }

    virtual ~PGConnI()
    {
        close();
    }

private:
    void close()
    {

    }

public:
    unsigned int ticks() const
    {
        return _ticks;
    }

    bool login()
    {
        if( !_conn)
        {
            std::string conf = "host="+_ctx->host.host()+" "
                +"port="+std::to_string( _ctx->host.port())+" "
                +"dbname="+_ctx->host.db()+" "
                +"user="+_ctx->host.user()+" "
                +"password="+_ctx->host.pwd()+" "
                +_ctx->host.info();

            _conn = PQconnectdb( conf.c_str());
            if( _conn)
            {
                if( PQstatus( _conn) != CONNECTION_OK)
                    return true;
                else
                {
                    PQfinish( _conn);
                    _conn = 0;
                }
            }
        }

        return false;
    }

    virtual StmtPtr insertStmt( const std::string &name, const std::string &query)
    {
        return 0;
    }

    virtual StmtPtr findStmt( const std::string &name) const
    {
        return 0;
    }

    virtual void removeStmt( const std::string &name)
    {
    }

    virtual void clearStmt()
    {
    }

    virtual bool execute( const std::string &query)
    {
        return false;
    }

    virtual bool execute( const std::string &query, ResultPtr &result)
    {
        return false;
    }

    virtual int64_t affectedRows()
    {
        return 0;
    }

    virtual int64_t insertId()
    {
        return 0;
    }

    virtual bool commit()
    {
        return false;
    }

    virtual bool rollback()
    {
        return false;
    }

    virtual bool autoCommit( bool mode)
    {
        return false;
    }

    virtual unsigned int getLastReasonCode()
    {
        return 0;
    }

    virtual std::string getLastReason()
    {
        return "";
    }

    virtual void release()
    {
    }
};

class PGConnPoolI: public ConnPool
{
    PGConnPoolCtxPtr _ctx;
    unsigned long _timeoutTicks;

public:
    PGConnPoolI( const ApplicationPtr &application, const PGConfig &host)
    {
        _ctx = new PGConnPoolCtx( host);
        _ctx->application = application;
        _ctx->activated = true;
        _ctx->connNum = 0;
        _ctx->maxConnNum = 100;
        _ctx->event = createEvent();
        _ctx->waitNum = 0;
        _timeoutTicks = getMonoTicks();
    }

    virtual ~PGConnPoolI()
    {
        std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);

        if( _ctx->activated)
        {
            _ctx->activated = false;
            _ctx->conns.clear();
            destroyEvent( _ctx->event);
        }
    }

    virtual ConnPtr getConnection()
    {
        return 0;
        /*PGConnIPtr conn;

        while( true)
        {
            std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);

            if( _ctx->conns.size())
            {
                conn = _ctx->conns.front();
                _ctx->conns.pop_front();
            } else
            {
                if( _ctx->connNum < _ctx->maxConnNum)
                {
                    conn = new PGConnI( _ctx);

                    _ctx->connNum++;
                    {
                        TmpUnlock unlock( _ctx.get());

                        if( !conn->login())
                            conn = 0;
                    }
                } else
                {
                    _ctx->waitNum++;
                    TmpUnlock unlock( _ctx.get());

                    waitEvent( _ctx->event, -1);
                    continue;
                }
            }

            break;
        }

        return conn;*/
    }

    virtual void schd()
    {
        if( getMonoTicks()-_timeoutTicks > 1*1000)
        {
            std::lock_guard<tars::TC_ThreadRecMutex> lock( *_ctx);
            int period = 30*1000;
            unsigned long ticks = getMonoTicks();

            _ctx->application->getAppConfigAsInt( "MysqlMaxConnection", _ctx->maxConnNum);
            _ctx->application->getAppConfigAsInt( "MysqlInactivatedTimeout", period);
            for( std::list<PGConnIPtr>::iterator it=_ctx->conns.begin(); it!=_ctx->conns.end();)
            {
                if( ticks-(*it)->ticks() > (unsigned int)period)
                    it = _ctx->conns.erase( it);
                else
                    break;
            }

            _timeoutTicks = getMonoTicks();
        }
    }
};

PGConfig::PGConfig( const std::string &uri)
{
    URI u( uri);

    if( !u.proto().size() || u.proto()=="postgresql")
    {
        _host = u.host();
        _port = u.port().toInt( 5432);
        if( u.path().size())
            _db = u.path().substr( 1);
        _user = u.param( "user");
        if( u.param( "password").size())
            _pwd = u.param( "password");
        else
            _pwd = u.param( "pwd");
        for( std::map<std::string, std::string>::const_iterator it=u.params().begin(); it!=u.params().end(); it++)
            _info += it->first+"='"+it->second+"'";
    }
}

PGConfig::PGConfig( const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd, const std::string &info)
{
    _host = host;
    _port = port;
    _db = db;
    _user = user;
    _pwd = pwd;
    _info = info;
}

const std::string& PGConfig::host() const
{
    return _host;
}

int PGConfig::port() const
{
    return _port;
}

const std::string& PGConfig::db() const
{
    return _db;
}

const std::string& PGConfig::user() const
{
    return _user;
}

const std::string& PGConfig::pwd() const
{
    return _pwd;
}

const std::string& PGConfig::info() const
{
    return _info;
}

ConnPoolPtr ConnPool::createForPG( const ApplicationPtr &application, const std::string &uri)
{
    return 0;
    //return new PGConnPoolI( application, PGConfig( uri));
}

ConnPoolPtr ConnPool::createForPG( const ApplicationPtr &application, const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd, const std::string &info)
{
    return 0;
    //return new PGConnPoolI( application, PGConfig( host, port, db, user, pwd, info));
}
