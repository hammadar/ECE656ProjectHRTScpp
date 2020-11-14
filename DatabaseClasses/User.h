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
    std::string location;
    std::vector<std::string> friends;

public:
    User(std::string userID, std::string firstName, std::string lastName, std::time_t birthDate, std::string location = NULL, std::vector<std::string> friends = std::vector<std::string>());


};