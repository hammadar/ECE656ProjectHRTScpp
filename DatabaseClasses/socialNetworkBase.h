//
// Created by Hammad Rehman on 2020-11-14.
//

#ifndef ECE656PROJECTHRTS_SOCIALNETWORKBASE_H
#define ECE656PROJECTHRTS_SOCIALNETWORKBASE_H

#endif //ECE656PROJECTHRTS_SOCIALNETWORKBASE_H

#include <iostream>
#include <cstdlib>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class socialNetworkBase {
    public:
        //virtual ~socialNetworkBase();
        virtual void updateInDatabase(::sql::Connection *con) {};
        virtual void createInDatabase(::sql::Connection *con) {};
        virtual bool checkDatabaseExistence(::sql::Connection *con) { return false;};
};