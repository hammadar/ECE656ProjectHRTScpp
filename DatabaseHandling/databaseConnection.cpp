//
// Created by Hammad Rehman on 2020-11-13.
//
#include <stdlib.h>
#include <iostream>
#include "databaseConnection.h"

dataBaseConnection::dataBaseConnection(const string& host, const string& user, const string& password) {
    this->driver = sql::mysql::get_mysql_driver_instance();
    this->con = driver->connect(host, user, password);
}

void dataBaseConnection::disconnect() {
    this->con->close();
}

void dataBaseConnection::createUser(const string& user, const string& password) {
    sql::Statement *stmt;

    stmt = this->con->createStatement();
    stmt->execute("USE ece656project");
    stmt->execute(("CREATE USER '%s'@'%' IDENTIFIED BY '%s'", user, password));
    delete stmt;
}


