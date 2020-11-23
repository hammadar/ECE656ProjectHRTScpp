//
// Created by Hammad Rehman on 2020-11-22.
//

#ifndef ECE656PROJECTHRTS_FORUMSMAKER_H
#define ECE656PROJECTHRTS_FORUMSMAKER_H

#endif //ECE656PROJECTHRTS_FORUMSMAKER_H
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
#include "credentialsGenerator.h"

class forumsMaker {
    private:
        std::string mode;
        CredentialsGenerator *credentialGen;

    public:
        forumsMaker(std::string mode);
        void makeForums(::sql::Connection *con);

};