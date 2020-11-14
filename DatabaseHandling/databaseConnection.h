//
// Created by Hammad Rehman on 2020-11-13.
//

#ifndef ECE656PROJECTHRTS_DATABASECONNECTION_H
#define ECE656PROJECTHRTS_DATABASECONNECTION_H

#endif //ECE656PROJECTHRTS_DATABASECONNECTION_H

#include <cstdlib>
#include <iostream>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_error.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn//exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>



#define LOCAL_IP "192.168.0.57"
#define NORMAL_IP "99.251.104.144"
#define DB "ece656project"

using namespace std;


class dataBaseConnection {

    private:
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

    public:
        dataBaseConnection(const string& host, const string& user, const string& password);


        void disconnect();
        void createUser(const string& user, const string& password);

};