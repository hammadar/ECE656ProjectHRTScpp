//
// Created by Hammad Rehman on 2020-11-18.
//

#ifndef ECE656PROJECTHRTS_FORUM_H
#define ECE656PROJECTHRTS_FORUM_H

#endif //ECE656PROJECTHRTS_FORUM_H

#include "socialNetworkBase.h"
#include <vector>
#include <time.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn//exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class Forum: public socialNetworkBase {
    private:
        std::string forumID;
        std::string titleID;
        std::vector<std::string> threads;

    public:
        Forum(std::string forumID, std::string titleID, std::vector<std::string> threads = std::vector<std::string>());
        void createInDatabase(::sql::Connection *con) override;
        void updateInDatabase(::sql::Connection *con) override;
        bool checkDatabaseExistence(::sql::Connection *con) override;
        std::vector<std::string> getThreads(::sql::Connection *con);
        void updateThreads(std::vector<std::string> additionalThreads, ::sql::Connection *con);

};