//
// Created by Hammad Rehman on 2020-11-24.
//

#include "Navigation.h"
#include "DatabaseClasses/User.h"
#include <string>
#include <time.h>
#include <ctime>
#include <boost/algorithm/string.hpp>

Navigation::Navigation(::sql::Connection *connection) {
	this->con = connection;
	this->credentialGen = new CredentialsGenerator();
}

void Navigation::mainUXHandle() {
	std::string userInput="";
	std::string userID="";
	std::cout << "Are you a registered user? (Yes/No): ";
	std::cin >> userInput;
	if(boost::iequals(userInput, "YES"))
	{
		std::cout << "Please enter userID:";
		std::cin >> userID;
		login(userID);	
	}
	else
	{
		registerUser();
	}
}

void Navigation::login(std::string userID) {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	std::string query;
	stmt = con->createStatement();
	
	query = "SELECT COUNT(1) FROM users WHERE user_id = \"" + userID + "\"";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);
	
	while(res->next()) {
		if (res->getInt(1) == 1) {
			std::cout << "Login successful" << std::endl;
			showMainMenu();
		}
		else{
			std::cout << "UserID does not exist" << std::endl;
			mainUXHandle();
		}
	}
	
	delete stmt;
	delete res;
}

void Navigation::registerUser() {
	struct tm birthDate;
    std::string firstName = "";
    std::string lastName = "";
    std::string stateProvince = "";
    std::string country = "";
	std::string dob = "";
    std::string userID = "";
    std::vector<std::string> friends = std::vector<std::string>();

	std::cout << "Enter first name:";
	std::cin >> firstName;
	
	std::cout << "Enter last name:";
	std::cin >> lastName;
	
	std::cout << "Enter state/province code:";
	std::cin >> stateProvince;
	
	std::cout << "Enter country:";
	std::cin >> country;
	
	std::cout << "Enter birthdate (YYYY-MM-DD):";
	std::cin >> dob;
	const char * date = dob.c_str();
	strptime(date, "%Y-%m-%d", &birthDate);
    birthDate.tm_isdst = -1;

	userID = this->credentialGen->generateCredential("User", con);
	
	std::cout << "Creating user profile. Your userID is "<< userID << std::endl;
	
	User *user = new User(userID, firstName, lastName, &birthDate, country, stateProvince, friends);
	user->createInDatabase(con);
}

void Navigation::showMainMenu() {
	
	std::cout << "What would you like to do?" << std::endl;
	std::cout << "\t1. Show threads you have participated in" << std::endl;
	std::cout << "\t2. Show posts you have submitted" << std::endl;
	std::cout << "\t3. Search movie info by title" << std::endl;
	std::cout << "\t4. Find forums by title" << std::endl;
	std::cout << "\t5. Find ratings of movies by title" << std::endl;
	std::cout << "\t6. Get movie recommendations" << std::endl;
	std::cout << "\t7. Show friends" << std::endl;
	std::cout << "\t8. Add friend" << std::endl;
	
	int userInput;
	
	std::cin >> userInput;
	
	switch (userInput) {
		case 1:
			showUserThreads();
			break;
		case 2:
			showUserPosts();
			break;
		case 3:
			search();
			break;
		case 4:
			showForum();
			break;
		case 5:
			getRatings();
			break;
		case 6:
			showRecommendations();
			break;
		case 7:
			showFriends();
			break;
		case 8:
			addFriend();
			break;
		
		
	}	
}

void Navigation::showUserThreads() {

}

void Navigation::showThreadPosts() {

}

void Navigation::showUserPosts() {

}

void Navigation::search() {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	::sql::ResultSetMetaData *columnMetaData;
	
	std::string query;
	stmt = con->createStatement();
	
	std::string title="";
	std::cout << "Enter movie title: ";
	std::cin >> title;

	query = "select * from titleBasics where primaryTitle = \"" + title + "\" LIMIT 1";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);

	while(res->next()) {
		//DETERMINE WHAT INFO and HOW to display
		std::cout << "Movie Title: " << res->getString("primaryTitle") << std::endl;
		std::cout << "Genres: " << res->getString("genres") << std::endl;
		
		showMainMenu();
	}
	
	delete stmt;
	delete res;
	

}

void Navigation::getRatings() {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	std::string query;
	stmt = con->createStatement();
	
	std::string title="";
	std::cout << "Enter movie title: ";
	std::cin >> title;
	
	query = "SELECT averageRating AS Movie_Average_Rating FROM ratings\
		INNER JOIN titleBasics ON ratings.tconst = titleBasics.tconst\
		WHERE titleBasics.primaryTitle = \"" + title + "\" LIMIT 1";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);
	
	while(res->next()) {
		std::cout << "Average rating for " << title << ": " << res->getString("Movie_Average_Rating") << std::endl;
		std::cout << std::endl;
		showMainMenu();
	}
	
	delete stmt;
	delete res;
}

void Navigation::showForum() {

}

void Navigation::showRecommendations() {

}

void Navigation::showThread(std::string threadID) {

}

void Navigation::postInThread(std::string threadID) {

}

void Navigation::goToMainMenu() {

}

void Navigation::addFriend() {

}

void Navigation::showFriends() {

}

