//
// Created by Trishal Sudula on 2020-11-20.
//

#include "Thread.h"
#include "Utility/setHandling.h"
#include <set>
#include <utility>

Thread::Thread(std::string threadID, std::string forumID) {
    this->threadID = std::move(threadID);
    this->forumID = std::move(forumID);

}

void Thread::createInDatabase(::sql::Connection *con) {
    socialNetworkBase::createInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    stmt = con->createStatement();
    query = "INSERT INTO threads (thread_id, forum_id) VALUES (\"" + this->threadID + "\", \"" + this->forumID + "\")";
    stmt->execute(query);
    delete stmt;

}

void Thread::updateInDatabase(::sql::Connection *con) {
    socialNetworkBase::updateInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    stmt = con->createStatement();
    query = "UPDATE threads SET forum_id = " + this->forumID + " WHERE thread_id = " + this->threadID;
    stmt->execute("USE ece656project");
    stmt->execute(query);
    delete stmt;

}

bool Thread::checkDatabaseExistence(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT thread_id FROM threads WHERE thread_id = \"" + this->threadID + "\"");
    while(res->next()) {
        if (res->getString("thread_id") == this->threadID) {
            return true;
        }
    }
    delete stmt;
    delete res;
    return false;

}





