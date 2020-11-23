//
// Created by Hammad Rehman on 2020-11-20.
//

#include "credentialsGenerator.h"
#include <regex>

CredentialsGenerator::CredentialsGenerator() {
    this->tableMap = {
            {"User", "users"},
            {"Forum", "forum"},
            {"Thread", "threads"},
            {"Post", "posts"}
    };
    this->prefixMap = {
            {"User", "u"},
            {"Forum", "f"},
            {"Thread", "t"},
            {"Post", "p"}
    };
    this->attributeMap = {
        {"users", "user_id"},
        {"forum", "forum_id"},
        {"threads", "thread_id"},
        {"posts", "post_id"}
    };

}

std::string CredentialsGenerator::generateCredential(std::string type_name, ::sql::Connection *con) {
    std::string table = this->tableMap[type_name];
    long int lastUsed = this->getLastUsed(table, con);
    std::string toReturn = this->prefixMap[type_name] + std::to_string(lastUsed+1);
    return toReturn;
}

long int CredentialsGenerator::getLastUsed(std::string table, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    long int lastUsed = -1;
    std::smatch m;
    std::regex e("[0-9]+");
    stmt = con->createStatement();
    std::string attribute = this->attributeMap[table];
    stmt->execute("USE ece656project");
    res = stmt->executeQuery("SELECT " + attribute + " FROM " + table);
    while(res->next()) {
        std::string item = res->getString(attribute);
        bool found = std::regex_search(item, m, e);
        if (found) {
            for (auto x:m) {
                long int currentIndex = std::stoi(x);
                if (currentIndex > lastUsed) {
                    lastUsed = currentIndex;
                }
            }
        }
    }
    return lastUsed;
}
