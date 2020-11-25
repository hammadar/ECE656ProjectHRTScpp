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

class Navigation {
    private:
        int currentLevel;
        std::string currentUser;
        ::sql::Connection *con;

    public:
        explicit Navigation(::sql::Connection *con);
        void mainUXHandle();
        void login(std::string user);
        void registerUser();
        void showMainMenu();
        void showUserThreads();
        void showThreadPosts();
        void showUserPosts();
        void search(std::string titleName);
        void getRatings(std::string titleName);
        void showForum(std::string titleID);
        void showRecommendations();
        void showThread(std::string threadID);
        void postInThread(std::string threadID);
        void goToMainMenu();
        void addFriend(std::string friendID);
        void showFriends();
};