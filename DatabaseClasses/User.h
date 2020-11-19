//
// Created by Hammad Rehman on 2020-11-14.
//

#ifndef ECE656PROJECTHRTS_USER_H
#define ECE656PROJECTHRTS_USER_H

#endif //ECE656PROJECTHRTS_USER_H

#include "socialNetworkBase.h"
#include <vector>
#include <time.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


class User : public socialNetworkBase {
private:
    std::string userID;
    std::string firstName;
    std::string lastName;
    const tm *birthDate;
    std::string stateProvince;
    std::string country;
    std::vector<std::string> friends;

public:
    User(std::string userID, std::string firstName, std::string lastName, tm *birthDate, const std::string& country, const std::string& stateProvince = "", std::vector<std::string> friends = std::vector<std::string>());
    void addFriend(const std::string& friendID);
    void updateInDatabase(::sql::Connection *con);
    void createInDatabase(::sql::Connection *con);
    bool checkDatabaseExistence(::sql::Connection *con);
    std::vector<std::string> getFriends(::sql::Connection *con);
    void updateFriends(std::vector<std::string> additionalFriends, ::sql::Connection *con);


};