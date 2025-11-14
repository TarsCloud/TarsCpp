#include "SqlConn.h"

namespace DbConn
{
    namespace SQL
    {
        class SQLiteConnPoolI: public ConnPool
        {
        public:

        };

        ConnPoolPtr createForSQLite( const ApplicationPtr &application, const std::string &path)
        {
            return 0;
        }
    }
}
