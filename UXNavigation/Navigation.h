//
// Created by Hammad Rehman on 2020-11-24.
//

#ifndef ECE656PROJECTHRTS_NAVIGATION_H
#define ECE656PROJECTHRTS_NAVIGATION_H

#endif //ECE656PROJECTHRTS_NAVIGATION_H

#include <iostream>
#include <cstdlib>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "DatabaseHandling/credentialsGenerator.h"

class Navigation {
    private:
        int currentLevel;
        std::string currentUser;
        ::sql::Connection *con;
		CredentialsGenerator *credentialGen;
		std::vector<std::pair<std::string, std::string>> retrievedThreads;
		std::vector<std::vector<std::string>> retrievedPosts;

    public:
        explicit Navigation(::sql::Connection *connection);
        void mainUXHandle();
        void login(std::string userID);
        void registerUser();
        void showMainMenu();
        void showUserThreads();
        void showThreadPosts();
        void showUserPosts();
        void search();
        void getRatings();
        void showForum(std::string title);
        void showRecommendations();
        void showThread(std::string threadID);
        void postInThread(std::string threadID);
        void makeThread(std::string forumID);
        void goToMainMenu();
        void addFriend();
        void showFriends();
        void searchFriends(std::string userName);
};