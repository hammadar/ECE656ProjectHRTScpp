//
// Created by Hammad Rehman on 2020-12-08.
//

#ifndef ECE656PROJECTHRTS_RECOMMENDATIONENGINE_H
#define ECE656PROJECTHRTS_RECOMMENDATIONENGINE_H

#endif //ECE656PROJECTHRTS_RECOMMENDATIONENGINE_H
#include <mysql_driver.h>
#include <mysql_error.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "credentialsGenerator.h"

class RecommendationEngine {
    private:

    public:
        RecommendationEngine();
        void getRecommendations(std::string userID, ::sql::Connection *con);
        std::vector<std::pair<std::string, float>> getUserRatings(std::string userID, ::sql::Connection *con);
        std::vector<std::pair<float, std::string>> getUserDeviations(std::string userID, ::sql::Connection *con);
        std::pair<float, float> getRatingRange(std::string userID, ::sql::Connection *con);
        std::vector<std::string> getUserCategories(std::string userID, ::sql::Connection *con);

};