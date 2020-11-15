//
// Created by Hammad Rehman on 2020-11-14.
//

#ifndef ECE656PROJECTHRTS_USER_H
#define ECE656PROJECTHRTS_USER_H

#endif //ECE656PROJECTHRTS_USER_H

#include "socialNetworkBase.h"
#include <vector>

class User : public socialNetworkBase {
private:
    std::string userID;
    std::string firstName;
    std::string lastName;
    std::time_t birthDate;
    std::string stateProvince;
    std::string country;
    std::vector<std::string> friends;

public:
    User(std::string userID, std::string firstName, std::string lastName, std::time_t birthDate, const std::string& country, const std::string& stateProvince = "", std::vector<std::string> friends = std::vector<std::string>());
    void addFriend(const std::string& friendID);
    void updateInDatabase(sql::Connection *con);
    void createInDatabase(sql::Connection *con);
    bool checkDatabaseExistence(sql::Connection *con);
    std::vector<std::string> getFriends(sql::Connection *con);
    void updateFriends(std::vector<std::string> additionalFriends, sql::Connection *con);


};