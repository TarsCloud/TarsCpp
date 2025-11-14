#ifndef __DbConn_SqlConn_h
#define __DbConn_SqlConn_h

#include <string>
#include <vector>
#include <memory>
#include "servant/tup/Tars.h"

namespace DbConn
{
    namespace SQL
    {
        class Result;
        class Stmt;
        class Conn;
        class TxnConn;
        class ConnPool;
        class AsyncListener;
        class AsyncStmt;
        class AsyncConn;
        class AsyncConnPool;

        typedef std::shared_ptr<Result> ResultPtr;
        typedef std::shared_ptr<Stmt> StmtPtr;
        typedef std::shared_ptr<Conn> ConnPtr;
        typedef std::shared_ptr<TxnConn> TxnConnPtr;
        typedef std::shared_ptr<ConnPool> ConnPoolPtr;
        typedef std::shared_ptr<AsyncListener> AsyncListenerPtr;
        typedef std::shared_ptr<AsyncStmt> AsyncStmtPtr;
        typedef std::shared_ptr<AsyncConn> AsyncConnPtr;
        typedef std::shared_ptr<AsyncConnPool> AsyncConnPoolPtr;

        class Result
        {
        public:
            virtual tars::Int32 fetch() = 0;
            virtual std::string getResult( tars::Int32 idx) = 0;
        };

        class Stmt
        {
        public:
            virtual std::string name() const = 0;
            virtual std::string query() const = 0;
            virtual void bindParam( tars::Int32 idx, tars::Char val) = 0;
            virtual void bindParam( tars::Int32 idx, tars::Short val) = 0;
            virtual void bindParam( tars::Int32 idx, tars::Int32 val) = 0;
            virtual void bindParam( tars::Int32 idx, tars::Int64 val) = 0;
            virtual void bindParam( tars::Int32 idx, tars::Float val) = 0;
            virtual void bindParam( tars::Int32 idx, tars::Double val) = 0;
            virtual void bindParam( tars::Int32 idx, char *str, unsigned long len) = 0;
            virtual void bindParam( tars::Int32 idx, const std::string &val) = 0;
            virtual void bindParam( tars::Int32 idx, const tars::BufferReader &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Char &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Short &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Int32 &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Int64 &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Float &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::Double &val) = 0;
            virtual void bindResult( tars::Int32 idx, char *str, unsigned long &len) = 0;
            virtual void bindResult( tars::Int32 idx, std::string &val) = 0;
            virtual void bindResult( tars::Int32 idx, tars::BufferReader &val) = 0;
            virtual tars::Bool execute() = 0;
            virtual tars::Int64 affectedRows() = 0;
            virtual tars::Int32 fetch() = 0;
            virtual tars::Int64 insertId() = 0;
            virtual tars::UInt32 getLastReasonCode() = 0;
            virtual std::string getLastReason() = 0;
        };

        class Conn
        {
        public:
            virtual StmtPtr insertStmt( const std::string &name, const std::string &query) = 0;
            virtual StmtPtr findStmt( const std::string &name) const = 0;
            virtual StmtPtr initStmt( const std::string &name, const std::string &query) = 0;
            virtual void removeStmt( const std::string &name) = 0;
            virtual void clearStmt() = 0;
            virtual tars::Bool execute( const std::string &query) = 0;
            virtual tars::Bool execute( const std::string &query, ResultPtr &result) = 0;
            virtual tars::Int64 affectedRows() = 0;
            virtual tars::Int64 insertId() = 0;
            virtual tars::Bool commit() = 0;
            virtual tars::Bool rollback() = 0;
            virtual tars::Bool autoCommit( tars::Bool mode) = 0;
            virtual tars::UInt32 getLastReasonCode() = 0;
            virtual std::string getLastReason() = 0;
            virtual tars::Bool tableNotFound() = 0;
            virtual tars::Bool isDeadLock() = 0;
        };

        class TxnConn: public Conn
        {
        protected:
            virtual tars::Bool autoCommit( tars::Bool mode) = 0;
        };

        class MysqlConfig
        {
            std::string _host;
            int _port;
            std::string _db;
            std::string _user;
            std::string _pwd;

        public:
            MysqlConfig( const std::string &uri);
            MysqlConfig( const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd);

            const std::string& host() const;
            int port() const;
            const std::string& db() const;
            const std::string& user() const;
            const std::string& pwd() const;
        };

        class PGConfig
        {
            std::string _host;
            int _port;
            std::string _db;
            std::string _user;
            std::string _pwd;
            std::string _info;

        public:
            PGConfig( const std::string &uri);
            PGConfig( const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd, const std::string &info="");

            const std::string& host() const;
            int port() const;
            const std::string& db() const;
            const std::string& user() const;
            const std::string& pwd() const;
            const std::string& info() const;
        };

        class ConnPool
        {
        public:
            static ConnPoolPtr createForMysql(const std::string &uri);
            static ConnPoolPtr createForMysql(const std::string &uri, const std::string &db);
            static ConnPoolPtr createForMysql(const std::string &host, int port, const std::string &user, const std::string &pwd, const std::string &db);
            static ConnPoolPtr createForMysql(const std::vector<MysqlConfig> &hosts, const std::string &db);
            static ConnPoolPtr createForPG(const std::string &uri);
            static ConnPoolPtr createForPG(const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd, const std::string &info="");
            static ConnPoolPtr createForSQLite(const std::string &path);
            
            virtual ConnPtr getConnection() = 0;
            virtual ConnPtr getConnection( std::string &reason) = 0;
            virtual TxnConnPtr getTransactConnection() = 0;
            virtual TxnConnPtr getTransactConnection( std::string &reason) = 0;
            virtual void schd() = 0;
            virtual void close() = 0;
        };

        class AsyncListener
        {
        public:
            virtual void onGetConnection( const AsyncConnPtr &conn, const std::string &reason) {}
            virtual void onRemoveStmt( bool success, const std::string &reason) {}
            virtual void onClearStmt( bool success, const std::string &reason) {}
            virtual void onExecute( bool success, const std::string &reason) {}
            virtual void onCommit( bool success, const std::string &reason) {}
            virtual void onRollback( bool success, const std::string &reason) {}
            virtual void onAutoCommitAsync( bool success, const std::string &reason) {}
        };

        class AsyncStmt: public Stmt
        {
        public:
            virtual void executeAsync( const std::string &name) = 0;
        };

        class AsyncConn: public Conn
        {
        public:
            virtual void removeStmtAsync( const std::string &name) = 0;
            virtual void clearStmtAsync() = 0;
            virtual void executeAsync( const std::string &query) = 0;
            virtual void commitAsync() = 0;
            virtual void rollbackAsync() = 0;
            virtual void autoCommitAsync( bool mode) = 0;
        };

        class AsyncConnPool
        {
        public:
            static AsyncConnPoolPtr createForMysqlAsync(const std::string &uri, const std::string &db);
            static AsyncConnPoolPtr createForMysqlAsync(const std::string &host, int port, const std::string &user, const std::string &pwd, const std::string &db);
            static AsyncConnPoolPtr createForMysqlAsync(const std::vector<MysqlConfig> &hosts, const std::string &db);
            static AsyncConnPoolPtr createForPGAsync(const std::string &uri);
            static AsyncConnPoolPtr createForPGAsync(const std::string &host, int port, const std::string &db, const std::string &user, const std::string &pwd, const std::string &info="");
            
            virtual AsyncConnPtr getConnection( const AsyncListenerPtr &listener) = 0;
            virtual void getConnectionAsync( const AsyncListenerPtr &listener) = 0;
            virtual void schd() = 0;
            virtual void close() = 0;
        };
    }
}

#endif
