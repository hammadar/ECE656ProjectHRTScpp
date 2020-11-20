//
// Created by Hammad Rehman on 2020-11-20.
//

#ifndef ECE656PROJECTHRTS_CREDENTIALSGENERATOR_H
#define ECE656PROJECTHRTS_CREDENTIALSGENERATOR_H

#endif //ECE656PROJECTHRTS_CREDENTIALSGENERATOR_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn//exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class CredentialsGenerator {
    private:
        std::string type;
        std::map<std::string, std::string> tableMap;
        std::map<std::string, std::string> prefixMap;
        std::map<std::string, std::string> attributeMap;

    public:
        explicit CredentialsGenerator(std::string type);
        std::string generateCredential(std::string type_name, ::sql::Connection *con);
        int getLastUsed(std::string table, ::sql::Connection *con);


};