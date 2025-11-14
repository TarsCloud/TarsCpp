#ifndef __MysqlHelper_h
#define __MysqlHelper_h

#include "DbConn.h"

namespace DbConn
{
    bool jsonToMap(const std::string& s, std::map<std::string, std::string>& m);
    std::string mapToJson(const std::map<std::string, std::string>& m);

    namespace SQL
    {
#define GET_CONN2(tx) \
    ConnPtr conn = _sql->getConnection(reason); \
    if (conn) \
    { \
        bool tranx = tx; \
        unsigned int mysqlErrno = 0; \
        if (tranx) \
            conn->autoCommit(false); \
        do \
        {

#define GET_CONN(tranx) \
    std::string reason; \
    GET_CONN2(tranx)

#define CLOSE_CONN \
        } while (0); \
        if (tranx) \
        { \
            if (reason.empty()) \
                conn->commit(); \
            else \
                conn->rollback(); \
            conn->autoCommit(true); \
        } \
    } \
    else \
    { \
        reason = "conn_error:" + reason; \
    }

#define CLOSE_CONN_AND_RET \
    CLOSE_CONN \
    if (!reason.empty()) \
        throw std::runtime_error(reason); \
    return true;

#define GET_STMT(name, statement) \
    stmt = conn->findStmt(name); \
    { \
        if (!stmt) \
            stmt = conn->insertStmt(name, statement); \
    }

// Dreprecated, plz use DbConn::execute instead of below 2 macros!
#define INSERT_STMT(name, statement) \
    stmt = conn->insertStmt(name, statement);
#define REMOVE_STMT(name) \
    conn->removeStmt(name);


#define EXECUTE_STMT \
    { \
        if (!stmt->execute()) \
        { \
            mysqlErrno = stmt->getLastReasonCode(); \
            reason = stmt->name() + "_execute_error:" + std::string((int)mysqlErrno) + ":" + stmt->getLastReason() + ":" + stmt->query(); \
            break; \
        } \
    }

#define EXECUTE_STMT_CONTINUE_IF(errno) \
    { \
        if (!stmt->execute()) \
        { \
            mysqlErrno = stmt->getLastReasonCode(); \
            if (mysqlErrno != errno) \
            { \
                reason = stmt->name() + "_execute_error:" + std::string((int)mysqlErrno) + ":" + stmt->getLastReason() + ":" + stmt->query(); \
                break; \
            } \
        } \
    }

#define FETCH_ONCE_STMT(ret) \
    { \
        if ((ret = stmt->fetch()) < 0) \
        { \
            reason = "fetch_error:" + stmt->getLastReason(); \
            break; \
        } \
    }

        enum LOCK_MODE
        {
            X_LOCK,
            S_LOCK,
            NONE_LOCK
        };
#define STMT_NAME_LOCK_MODE_SUFFIX(lockMode) ((lockMode)==X_LOCK ? "_X" : ((lockMode)==S_LOCK ? "_S" : "_N"))
#define SELECT_SQL_LOCK_MODE_SUFFIX(lockMode) ((lockMode)==X_LOCK ? " FOR UPDATE;" : ((lockMode)==S_LOCK ? " LOCK IN SHARE MODE;" : ";"))

        std::string escapeStr(const std::string& str);
    }
}

#endif
