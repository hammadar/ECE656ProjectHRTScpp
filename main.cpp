#include <iostream>
#include "DatabaseHandling/databaseConnection.h"
#include "DatabaseClasses/User.h"
#include <time.h>
#include <ctime>

#define LOCAL_IP "192.168.0.57"
#define NORMAL_IP "99.251.104.144"
#define DB "ece656project"

int main() {
    const std::string user = "hammad";
    const std::string password = "hammadtrishal";
    struct tm birthDate;
    std::string firstName = "Hammad";
    std::string lastName = "Rehman";
    std::string stateProvince = "ON";
    std::string country = "Canada";
    std::string userID = "u0001";
    std::vector<std::string> friends = std::vector<std::string>();

    strptime("1989-11-25 00:00:00", "%Y-%m-%d %H:%M:%S", &birthDate);
    birthDate.tm_isdst = -1;

    User *moo = new User(userID, firstName, lastName, &birthDate, country, stateProvince, friends);
    dataBaseConnection *dbconn = new dataBaseConnection(LOCAL_IP, user, password);
    if (!moo->checkDatabaseExistence(dbconn->getSQLConnection())) {
        std::cout << "Doesn't exist confirmed" <<std::endl;
    } else {
        std::cout << "PRoblem, it does exist" << std::endl;
        return 0;
    }
    moo->createInDatabase(dbconn->getSQLConnection());



}
