//
// Created by Hammad Rehman on 2020-11-14.
//

#include "User.h"
#include "Utility/setHandling.h"
#include <utility>
#include <set>
#include <mysql-cppconn-8/jdbc/cppconn/resultset.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

User::User(std::string userID, std::string firstName, std::string lastName, time_t birthDate,
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

    stmt = con->createStatement();
    if (!this->stateProvince.empty()) {
        query = "UPDATE users SET first_name = " + this->firstName + ", last_name = " + this->lastName + ", birth_date = " +
                boost::posix_time::to_simple_string(boost::posix_time::from_time_t(this->birthDate)) + ", stateProvince = " + this->stateProvince +
                ", country = " + this->country + " WHERE user_id = " + this->userID;
    } else {
        query = "UPDATE users SET first_name = " + this->firstName + ", last_name = " + this->lastName + ", birth_date = " +
                boost::posix_time::to_simple_string(boost::posix_time::from_time_t(this->birthDate)) + ", country = " + this->country +
                " WHERE user_id = " + this->userID;
    }
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

    stmt = con->createStatement();
    if (!this->stateProvince.empty()) {
        query = "INSERT INTO users (user_id, first_name, last_name, birth_date, country) VALUES (" + this->userID + ", " + this->firstName + ", " +
                this->lastName + ", " + boost::posix_time::to_simple_string(boost::posix_time::from_time_t(this->birthDate)) + ", " + this->country + ")";
    } else {
        query = "INSERT INTO users (user_id, first_name, last_name, birth_date, stateProvince, country) VALUES (" + this->userID + ", " + this->firstName + ", " +
                this->lastName + ", " + boost::posix_time::to_simple_string(boost::posix_time::from_time_t(this->birthDate)) +
                ", " + this->stateProvince + ", " + this->country + ")";
    }
    stmt->execute(query);
    delete stmt;
}

bool User::checkDatabaseExistence(::sql::Connection *con) {
    socialNetworkBase::checkDatabaseExistence(con);
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    res = stmt->executeQuery("SELECT user_id FROM users WHERE user_id = " + this->userID);
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
    res = stmt->executeQuery("SELECT friend_id FROM connections WHERE user_id = " + this->userID);
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
    stmt = con->createStatement();
    for (auto it = additionalFriends.begin(); it!= additionalFriends.end(); it++) {
        if (it == additionalFriends.end()-1) {
            addtoQuery += "(" + this->userID + ", " + *it + ")";
        } else {
            addtoQuery += "(" + this->userID + ", " + *it + "), ";
        }
		query = "INSERT INTO connections(user_id, friend_id) VALUES " + addtoQuery;
		stmt->execute(query);
		delete stmt;
	}


}


