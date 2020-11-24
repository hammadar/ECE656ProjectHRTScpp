//
// Created by Trishal Sudula on 2020-11-20.
//

#include "Post.h"
#include "Utility/setHandling.h"
#include <set>
#include <utility>

Post::Post(std::string postID, std::string threadID, std::string userID, std::string postText) {
    this->postID = std::move(postID);
    this->threadID = std::move(threadID);
    this->userID = std::move(userID);
	this->postText = std::move(postText);
}

void Post::createInDatabase(::sql::Connection *con) {
    socialNetworkBase::createInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    stmt = con->createStatement();
    query = "INSERT INTO posts (post_id, thread_id, user_id, post) VALUES (\"" + this->postID + "\", \"" + this->threadID + "\", \"" +
        this->userID + "\", \"" + this->postText + "\")";
    stmt->execute(query);

}

void Post::updateInDatabase(::sql::Connection *con) {
    socialNetworkBase::updateInDatabase(con);
    ::sql::Statement *stmt;
    std::string query;
    stmt = con->createStatement();
    query = "UPDATE posts SET post = " + this->postText + " WHERE post_id = " + this->postID;
    stmt->execute("USE ece656project");
    stmt->execute(query);
    delete stmt;

}

bool Post::checkDatabaseExistence(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT post_id FROM posts WHERE post_id = \"" + this->postID + "\"");
    while(res->next()) {
        if (res->getString("post_id") == this->postID) {
            return true;
        }
    }
    delete stmt;
    delete res;
    return false;

}

std::string Post::getPostText(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    stmt = con->createStatement();
    std::string post_text;
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT post FROM posts WHERE post_id = \"" + this->postID + "\"");
    post_text = res->getString("post");
    
    delete stmt;
    delete res;
    return post_text;
}



