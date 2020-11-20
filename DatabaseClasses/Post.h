//
// Created by Trishal Sudula on 2020-11-20.
//

#ifndef ECE656PROJECTHRTS_POST_H
#define ECE656PROJECTHRTS_POST_H

#endif //ECE656PROJECTHRTS_POST_H

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

class Post: public socialNetworkBase {
    private:
		std::string postID;
        std::string threadID;
        std::string userID;
        std::string postText;

    public:
        Post(std::string postID, std::string threadID, std::string userID, std::string postText);
        void createInDatabase(::sql::Connection *con) override;
        void updateInDatabase(::sql::Connection *con) override;
        bool checkDatabaseExistence(::sql::Connection *con) override;
        std::string getPostText(::sql::Connection *con);

};