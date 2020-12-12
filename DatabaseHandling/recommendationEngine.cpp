//
// Created by Hammad Rehman on 2020-12-08.
//

#include "recommendationEngine.h"
#include <string>
#include <set>
#include <sstream>
#include <iostream>


RecommendationEngine::RecommendationEngine() {

}

void RecommendationEngine::getRecommendations(std::string userID, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string temp;
    int counter = 1;
    std::pair<float, float> ratingsRange;
    std::vector<std::string> categories;
    std::vector<std::string> recommendations;

    stmt = con->createStatement();
    stmt->execute("use ece656project");
    ratingsRange = this->getRatingRange(userID, con);
    categories = this->getUserCategories(userID, con);
    if (categories.empty()) {
        query = "select primaryTitle from titleBasics inner join ratings on titleBasics.tconst = ratings.tconst where "
                "averageRating between 9.0 and 10.0 order by rand() limit 10";
        res = stmt->executeQuery(query);
        std::cout << "Based on your previous ratings, we think you may like the following titles:" << std::endl;
        while (res->next()) {
            temp = res->getString("primaryTitle");
            std::cout << counter << ". " << temp << std::endl;
            counter++;
        }
    }
    else {
        std::cout << "Based on your previous ratings, we think you may like the following titles:" << std::endl;
        for (auto it = categories.begin(); it != categories.end(); it++) {
            query = "select primaryTitle from titleBasics inner join ratings on titleBasics.tconst = ratings.tconst where "
                    "averageRating between " + std::to_string(ratingsRange.first) + " and "
                    + std::to_string(ratingsRange.second) + "order by rand() limit 10";
            res = stmt->executeQuery(query);

            while (res->next()) {
                temp = res->getString("primaryTitle");
                std::cout << counter << ". " << temp << std::endl;
                counter++;
            }

        }
    }

    delete stmt;
    delete res;
}

std::vector<std::pair<std::string, float>>
RecommendationEngine::getUserRatings(std::string userID, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::vector<std::pair<std::string, float>> ratings;
    std::string titleID;
    float rating;

    query = "select titleID, rating from userRatings where user_id = \"" + userID + "\"";
    stmt = con->createStatement();
    stmt->execute("use ece656project");
    res = stmt->executeQuery(query);

    while (res->next()) {
        titleID = res->getString("titleID");
        rating = res->getDouble("rating");
        ratings.push_back(std::pair<std::string, float>(titleID, rating));
    }

    delete stmt;
    delete res;
    return ratings;
}

std::vector<std::pair<float, std::string>>
RecommendationEngine::getUserDeviations(std::string userID, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string titleID;
    float rating;
    float deviation;
    std::vector<std::pair<float, std::string>> deviations;
    std::vector<std::pair<std::string, float>> ratings;
    stmt = con->createStatement();
    stmt->execute("use ece656project");
    ratings = this->getUserRatings(userID, con);
    for (auto it = ratings.rbegin(); it!= ratings.rend(); it++) {
        titleID = it->first;
        rating = it->second;
        query = "select averageRating from ratings where tconst = \"" + titleID + "\"";
        res = stmt->executeQuery(query);
        while (res->next()) {
            deviation = rating - res->getDouble("averageRating");
        }
        deviations.push_back(std::pair<float, std::string>(deviation, titleID));
    }
    delete stmt;
    delete res;

    return deviations;
}

std::pair<float, float> RecommendationEngine::getRatingRange(std::string userID, ::sql::Connection *con) {
    std::vector<std::pair<float,std::string>> deviations;
    std::vector<std::string> ratedTitles;
    std::map<float, std::pair<float, float>> devianceMap;
    std::pair<float, float> ratingRange;
    int averageDeviation;
    int count = 0;
    float total = 0;
    devianceMap = {
            {5, std::pair<float, float>(4.0, 5.0)},
            {4, std::pair<float, float>(5.0, 6.0)},
            {3, std::pair<float, float>(6.0, 7.0)},
            {2, std::pair<float, float>(7.0, 8.0)},
            {1, std::pair<float, float>(8.0, 9.0)},
            {0, std::pair<float, float>(9.0, 10.0)},
            {6, std::pair<float,float>(3.0,4.0)},
            {7, std::pair<float, float>(2.0, 3.0)},
            {8, std::pair<float, float>(1.5, 2.0)},
            {9, std::pair<float, float>(0.5, 1.5)},
            {10, std::pair<float, float>(0.0, 0.5)}
    };

    deviations = this->getUserDeviations(userID, con);
    for (auto it = deviations.begin(); it != deviations.end(); it++) {
        total += it->first;
        count++;
        ratedTitles.push_back(it->second);
    }
    averageDeviation = int(abs(total/count));
    ratingRange = devianceMap[averageDeviation];
    return ratingRange;
}

std::vector<std::string> RecommendationEngine::getUserCategories(std::string userID, ::sql::Connection *con) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string temp;
    std::set<std::string> genres;
    std::vector<std::string> genVec;
    stmt = con->createStatement();
    query = "select genres from titleBasics join userRatings on titleBasics.tconst = userRatings.titleID where userRatings.user_id = \"" + userID + "\"";
    stmt->execute("use ece656project");
    res = stmt->executeQuery(query);
    while (res->next()) {
        temp = res->getString("genres");
        std::stringstream ss(temp);
        std::string token;
        while (getline(ss, token, ',' )) {
            genres.insert(token);
        }
    }
    std::copy(genres.begin(), genres.end(), std::back_inserter(genVec));
    delete stmt;
    delete res;
    return genVec;
}
