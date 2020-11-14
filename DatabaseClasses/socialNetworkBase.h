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

class socialNetworkBase {
    public:
        virtual ~socialNetworkBase();
        virtual void updateInDatabase(sql::Connection &con);
        virtual void createInDatabase(sql::Connection &con);
        virtual void checkDatabaseExistence(sql::Connection &con);
};