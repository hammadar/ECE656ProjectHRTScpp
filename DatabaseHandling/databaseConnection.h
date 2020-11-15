//
// Created by Hammad Rehman on 2020-11-13.
//

#ifndef ECE656PROJECTHRTS_DATABASECONNECTION_H
#define ECE656PROJECTHRTS_DATABASECONNECTION_H

#endif //ECE656PROJECTHRTS_DATABASECONNECTION_H

#include <cstdlib>
#include <iostream>
#include <mysql-cppconn-8/jdbc/mysql_driver.h>
#include <mysql-cppconn-8/jdbc/mysql_error.h>
#include <mysql-cppconn-8/jdbc/mysql_connection.h>
#include <mysql-cppconn-8/jdbc/cppconn/driver.h>
#include <mysql-cppconn-8/jdbc/cppconn//exception.h>
#include <mysql-cppconn-8/jdbc/cppconn/resultset.h>
#include <mysql-cppconn-8/jdbc/cppconn/statement.h>
#include <mysql-cppconn-8/jdbc/cppconn/prepared_statement.h>


class dataBaseConnection {

    private:
        ::sql::mysql::MySQL_Driver *driver;
        ::sql::Connection *con;

    public:
        dataBaseConnection(const std::string& host, const std::string& user, const std::string& password);


        void disconnect();
        void createUser(const std::string& user, const std::string& password);

};