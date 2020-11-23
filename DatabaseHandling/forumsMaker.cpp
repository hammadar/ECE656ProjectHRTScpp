//
// Created by Hammad Rehman on 2020-11-22.
//

#include "forumsMaker.h"

forumsMaker::forumsMaker(std::string mode) {
    this->mode = mode;
    this->credentialGen = new CredentialsGenerator();

}

void forumsMaker::makeForums(::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet *res;
    stmt = con->createStatement();
    stmt->execute("USE ece656project");

    if (this->mode == "test") {
        //make test thingy and check test
        std::string forum_id = "f1";
        std::string tconst;
        res = stmt->executeQuery("SELECT tconst FROM titleBasics");
        while (res->next()) {
            tconst = res->getString("tconst");
        }
        std::string query = "INSERT INTO forum (forum_id, titleID) VALUES (\"" + forum_id + "\",\"" + tconst +"\")";
        stmt->execute(query);
        delete stmt;
    } else {
        std::string tconst;
        std::string forum_id;
        res = stmt->executeQuery("SELECT DISTINCT tconst FROM titleBasics");
        while (res->next()) {
            tconst = res->getString("tconst");
            forum_id = this->credentialGen->generateCredential("Forum", con);
            std::string query = "INSERT INTO forum (forum_id, titleID) VALUES (\"" + forum_id + "\",\"" + tconst +"\")";
            try {
                stmt->execute(query);
            } catch (::sql::SQLException e) {
                std::cerr << "An exception occured: " << e.what() << std::endl;

            }

        }
        delete stmt;
    }
}
