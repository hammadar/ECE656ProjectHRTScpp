//
// Created by Hammad Rehman on 2020-11-13.
//

#ifndef ECE656PROJECTHRTS_DATABASECONNECTION_H
#define ECE656PROJECTHRTS_DATABASECONNECTION_H

#endif //ECE656PROJECTHRTS_DATABASECONNECTION_H

#include <cstdlib>
#include <iostream>

#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>



class dataBaseConnection {

    private:
        ::sql::mysql::MySQL_Driver *driver;
        ::sql::Connection *con;

    public:
        dataBaseConnection(const std::string& host, const std::string& user, const std::string& password);


        void disconnect();
        void createUser(const std::string& user, const std::string& password);
        ::sql::Connection* getSQLConnection();

};