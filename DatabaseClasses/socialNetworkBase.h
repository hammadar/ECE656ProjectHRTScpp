//
// Created by Hammad Rehman on 2020-11-14.
//

#ifndef ECE656PROJECTHRTS_SOCIALNETWORKBASE_H
#define ECE656PROJECTHRTS_SOCIALNETWORKBASE_H

#endif //ECE656PROJECTHRTS_SOCIALNETWORKBASE_H

#include <iostream>
#include <cstdlib>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn//exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

class socialNetworkBase {
    public:
        //virtual ~socialNetworkBase();
        virtual void updateInDatabase(::sql::Connection *con) {};
        virtual void createInDatabase(::sql::Connection *con) {};
        virtual bool checkDatabaseExistence(::sql::Connection *con) { return false;};
};