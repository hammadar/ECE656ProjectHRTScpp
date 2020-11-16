//
// Created by Hammad Rehman on 2020-11-13.
//

#include <iostream>
#include "databaseConnection.h"

#define LOCAL_IP "192.168.0.57"
#define NORMAL_IP "99.251.104.144"
#define DB "ece656project"

dataBaseConnection::dataBaseConnection(const std::string& host, const std::string& user, const std::string& password) {
    ::sql::mysql::MySQL_Driver *tempdriver;
    //::sql::Connection *con;
    try {
        tempdriver = ::sql::mysql::get_mysql_driver_instance();
    } catch(int e) {
        //???
    }
    this->driver = tempdriver;
    this->con = this->driver->connect(host, user, password);
}

void dataBaseConnection::disconnect() {
    this->con->close();
}

void dataBaseConnection::createUser(const std::string& user, const std::string& password) {
    ::sql::Statement *stmt;

    stmt = this->con->createStatement();
    stmt->execute("USE ece656project");
    stmt->execute(("CREATE USER '%s'@'%' IDENTIFIED BY '%s'", user, password));
    delete stmt;
}

::sql::Connection* dataBaseConnection::getSQLConnection() {
    return this->con;
}


