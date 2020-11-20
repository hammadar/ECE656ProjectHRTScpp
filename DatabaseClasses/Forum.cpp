//
// Created by Hammad Rehman on 2020-11-18.
//

#include "Forum.h"
#include "Utility/setHandling.h"
#include <set>
#include <utility>

Forum::Forum(std::string forumID, std::string titleID, std::vector<std::string> threads) {
    this->forumID = std::move(forumID);
    this->titleID = std::move(titleID);
    this->threads = std::move(threads);
}

void Forum::createInDatabase(::sql::Connection *con) {
    socialNetworkBase::createInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    stmt = con->createStatement();
    query = "INSERT INTO forum (forum_id, titleID) VALUES (" + this->forumID + "," + this->titleID + ")";
    stmt->execute(query);
    if (!this->threads.empty()) {

    }

}

void Forum::updateInDatabase(::sql::Connection *con) {
    socialNetworkBase::updateInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    std::vector<std::string> oldThreads;
    std::vector<std::string> difference;
    stmt = con->createStatement();
    query = "UPDATE forum SET titleID = " + this->titleID + " WHERE forum_id = " + this->forumID;
    stmt->execute("USE ece656project");
    stmt->execute(query);
    delete stmt;

    oldThreads = this->getThreads(con);
    difference = getDifference(oldThreads, this->threads);
    this->updateThreads(difference, con);

}

bool Forum::checkDatabaseExistence(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT forum_id FROM forum WHERE forum_id = \"" + this->forumID + "\"");
    while(res->next()) {
        if (res->getString("forum_id") == this->forumID) {
            return true;
        }
    }
    return false;

}

std::vector<std::string> Forum::getThreads(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    stmt = con->createStatement();
    threads = std::vector<std::string>();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT thread_id FROM threads WHERE forum_id = \"" + this->forumID + "\"");
    while(res->next()) {
        threads.push_back(res->getString("friend_id"));
    }
    delete stmt;
    return threads;
}


void Forum::updateThreads(std::vector<std::string> additionalThreads, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    std::string addtoQuery = "";
    std::string query;
    if (additionalThreads.empty()) {
        return;
    }
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    for (auto it = additionalThreads.begin(); it!= additionalThreads.end(); it++) {
        if (it == additionalThreads.end()-1) {
            addtoQuery += "(\"" + this->forumID + "\", \"" + *it + "\")";
        } else {
            addtoQuery += "(\"" + this->forumID + "\", \"" + *it + "\"), ";
        }
        query = "INSERT INTO threads(forum_id, thread_id) VALUES " + addtoQuery;
        stmt->execute(query);

    }
    delete stmt;
}



