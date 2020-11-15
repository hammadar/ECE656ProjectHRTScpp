#include <iostream>
#include "DatabaseHandling/databaseConnection.h"
#include "DatabaseClasses/User.h"

#define LOCAL_IP "192.168.0.57"
#define NORMAL_IP "99.251.104.144"
#define DB "ece656project"

int main() {
    const std::string user = "hammad";
    const std::string password = "hammadtrishal";
    dataBaseConnection *dbconn = new dataBaseConnection(LOCAL_IP, user, password);

}
