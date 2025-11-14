#ifndef __Reason_h__
#define __Reason_h__

// #define LAST_REASON                     Common::ObjectAgent::getLastReason()
// #define IS_MAIN_REASON(reason,main)     reason.subequ(0,main)

#define AGENT_ERROR                     "agent-error"           /* error occurred befor server receive request, for example: network error */ 
#define SERVER_ERROR                    "server-error"          /* error occurred when server process request, for example: object not found */
#define PROXY_ERROR                     "proxy-error"           /* error occurred when communication to proxy server, for example: RESTful server timeout */
#define INNER_ERROR                     "inner-error"           /* inner error */
#define PARAMS_ERROR                    "params-error"          /* params error */
#define VERSION_ERROR                   "version-error"         /* version error, for example: rpc version error */
#define NOT_SUPPORT_ERROR               "not-support-error"     /* features not support */
#define DB_ERROR                        "db-error"              /* access database error */
#define PERMISSION_DENIED               "permission-denied"     /* permission denied */
#define LICENCE_ERROR                   "licence-error"         /* licence error */
#define REASONABLE_ERROR                "reasonable-error"      /* reasonable error */

#define APPKEY_ERROR                    "appkey-error"          /* appkey error */
#define DOMAIN_ERROR                    "domain-error"          /* domain error */
#define RANDOM_ERROR                    "random-error"          /* random error, maybe random expired, or connection closed */
#define ACCOUNT_ERROR                   "account-error"         /* account error */
#define SESSION_ERROR                   "session-error"         /* session error */
#define OBJECT_ERROR                    "object-error"          /* object error, maybe objectId not found */
#define TOKEN_ERROR                     "token-error"           /* token error, maybe prev valid connection released */
#define CONDS_ERROR                     "conds-error"           /* conditions error, inner error */
#define PWD_ERROR                       "pwd-error"             /* password error */
#define SERVER_BUSY                     "server-busy"           /* server-busy:xxx-error, client retry after 10min when server busy */
#define ANOTHER_DEVICE_LOGINED          "another-device-logined" /* kick off by server when another device logined */
#define CONNECTION_ERROR                "connection-error"      /* connection error, inner error */
#define TIMEOUT_ERROR                   "timeout-error"         /* wait timeout */
#define TERMINATED_ERROR                "terminated-error"      /* call terminated when application shutdown */
#define THIRD_AUTH_ERROR                "third-auth-error"      /* 3rd auth server return error */
#define KX_VERSION_ERROR                "kx-version-error"      /* key exchange version error */
#define KX_CERT_ERROR                   "kx-cert-error"         /* key exchange certificate error */
#define DOMAIN_BLOCKED_ERROR            "domain-blocked-error"  /* domain blocked error */


#define SAME_SESSION_LOGIN              "same-session-login"
#define SAME_DEVICE_LOGIN               "same-device-login"
#define SERVER_KICK_OFF                 "server-kick-off"
#define CLIENT_LOGOUT                   "client-logout"

#define TOKEN_ERROR_ANOTHER_DEVICE_LOGINED TOKEN_ERROR ":" ANOTHER_DEVICE_LOGINED
#define TOKEN_ERROR_SAME_SESSION_LOGIN  TOKEN_ERROR ":" ANOTHER_DEVICE_LOGINED
#define TOKEN_ERROR_SAME_DEVICE_LOGIN   TOKEN_ERROR ":" SAME_DEVICE_LOGIN
#define TOKEN_ERROR_SERVER_KICK_OFF     TOKEN_ERROR ":" SERVER_KICK_OFF
#define TOKEN_ERROR_CLIENT_LOGOUT       TOKEN_ERROR ":" CLIENT_LOGOUT

#endif 
