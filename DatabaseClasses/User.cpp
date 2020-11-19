//
// Created by Hammad Rehman on 2020-11-14.
//

#include "User.h"
#include "Utility/setHandling.h"
#include <utility>
#include <set>
#include <cppconn/resultset.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

User::User(std::string userID, std::string firstName, std::string lastName, tm *birthDate,
           const std::string &country, const std::string& stateProvince, std::vector<std::string> friends) {

    this->userID = std::move(userID);
    this->firstName = std::move(firstName);
    this->lastName = std::move(lastName);
    this->birthDate = birthDate;
    this->country = country;
    if (!stateProvince.empty()) {this->stateProvince = stateProvince;} else {this->stateProvince = nullptr;}
    this->friends = std::move(friends);

}

void User::addFriend(const std::string& friendID) {
    this->friends.push_back(friendID);
}

void User::updateInDatabase(::sql::Connection *con) {
    socialNetworkBase::updateInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    std::vector<std::string> oldFriends;
    std::vector<std::string> difference;
    char birthDatechar [80];
    std::string  birthDateString;

    strftime(birthDatechar, 80, "%F", this->birthDate);
    birthDateString = std::string((const char*)birthDatechar);
    stmt = con->createStatement();
    if (!this->stateProvince.empty()) {
        query = "UPDATE users SET first_name = \"" + this->firstName + "\", last_name = \"" + this->lastName + "\", birth_date = " +
                birthDateString + ", stateProvince = \"" + this->stateProvince +
                "\", country = \"" + this->country + "\" WHERE user_id = \"" + this->userID + "\"";
    } else {
        query = "UPDATE users SET first_name = \"" + this->firstName + "\", last_name = \"" + this->lastName + "\", birth_date = " +
                birthDateString + ", country = \"" + this->country +
                "\" WHERE user_id = \"" + this->userID + "\"";
    }
    stmt->execute("USE ece656project");
    stmt->execute(query);
    delete stmt;

    oldFriends = this->getFriends(con);
    difference = getDifference(oldFriends, this->friends);
    this->updateFriends(difference, con);

}

void User::createInDatabase(::sql::Connection *con) {
    socialNetworkBase::createInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    char birthDatechar [80];
    std::string  birthDateString;

    strftime(birthDatechar, 80, "%F", this->birthDate);
    birthDateString = std::string((const char*)birthDatechar);

    stmt = con->createStatement();
    if (this->stateProvince.empty()) {
        query = "INSERT INTO users (user_id, first_name, last_name, birth_date, country) VALUES (\"" + this->userID + "\", \"" + this->firstName + "\", \"" +
                this->lastName + "\", \"" + birthDateString + "\", \"" + this->country + "\")";
    } else {
        query = "INSERT INTO users (user_id, first_name, last_name, birth_date, stateProvince, country) VALUES (\"" + this->userID + "\", \"" + this->firstName + "\", \"" +
                this->lastName + "\", \"" + birthDateString +
                "\", \"" + this->stateProvince + "\", \"" + this->country + "\")";
    }
    stmt->execute("USE ece656project");
    stmt->execute(query);
    delete stmt;
}

bool User::checkDatabaseExistence(::sql::Connection *con) {
    socialNetworkBase::checkDatabaseExistence(con);
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT user_id FROM users WHERE user_id = \"" + this->userID + "\"");
    while(res->next()) {
        if (res->getString("user_id") == this->userID) {
            return true;
        }
    }
    return false;


}

std::vector<std::string> User::getFriends(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    stmt = con->createStatement();
    friends = std::vector<std::string>();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT friend_id FROM connections WHERE user_id = \"" + this->userID + "\"");
    while(res->next()) {
        friends.push_back(res->getString("friend_id"));
    }
    delete stmt;
    return friends;
}

void User::updateFriends(std::vector<std::string> additionalFriends, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    std::string addtoQuery = "";
    std::string query;
    if (additionalFriends.empty()) {
        return;
    }
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    for (auto it = additionalFriends.begin(); it!= additionalFriends.end(); it++) {
        if (it == additionalFriends.end()-1) {
            addtoQuery += "(\"" + this->userID + "\", \"" + *it + "\")";
        } else {
            addtoQuery += "(\"" + this->userID + "\", \"" + *it + "\"), ";
        }
		query = "INSERT INTO connections(user_id, friend_id) VALUES " + addtoQuery;
		stmt->execute(query);
		delete stmt;
	}


}


