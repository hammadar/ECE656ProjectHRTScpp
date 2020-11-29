//
// Created by Hammad Rehman on 2020-11-24.
//

#include "Navigation.h"
#include "DatabaseClasses/User.h"
#include <string>
#include <time.h>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>

Navigation::Navigation(::sql::Connection *connection) {
	this->con = connection;
	this->credentialGen = new CredentialsGenerator();
	this->currentUser = "";
	this->currentLevel = -1;
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
			this->currentUser = userID;
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
	
	mainUXHandle();
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
	std::cout << "\t9. Log Out" << std::endl;
	
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
			//showForum();
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
		case 9:
			logOut();
			break;
		
		
	}	
}

void Navigation::showUserThreads() {

}

void Navigation::showThreadPosts() { //might be redundant

}

void Navigation::showUserPosts() {

}

void Navigation::search() {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	
	std::string query;
	stmt = con->createStatement();
	
	std::string title="";
	std::cout << "Enter movie title: ";
	std::cin >> title;

	query = "SELECT * FROM titleBasics WHERE primaryTitle = \"" + title + "\" LIMIT 1";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);

	while(res->next()) {
		//DETERMINE WHAT INFO and HOW to display
		std::cout << "Movie Title: " << res->getString("primaryTitle") << std::endl;
		std::cout << "Genres: " << res->getString("genres") << std::endl;
		// HR - wait before going to main menu, or prompt before going back to main menu
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
	std::getline(std::cin >> std::ws, title);
	
	query = "SELECT averageRating AS Movie_Average_Rating FROM ratings\
		INNER JOIN titleBasics ON ratings.tconst = titleBasics.tconst\
		WHERE titleBasics.primaryTitle = \"" + title + "\" LIMIT 1";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);
	
	while(res->next()) {
		std::cout << "Average rating for " << title << ": " << res->getString("Movie_Average_Rating") << std::endl;
		delete stmt;
		delete res;
		
		std::cout << std::endl;
		showMainMenu();
	}
	
	delete stmt;
	delete res;
	
	std::cout << "Movie not in database" << std::endl;
	std::cout << std::endl;
	showMainMenu();
	

}

void Navigation::showForum(std::string title) {
    //HR
}

void Navigation::showRecommendations() {

}

void Navigation::showThread(std::string threadID) {
    //HR
}

void Navigation::postInThread(std::string threadID) {
    //HR
}

void Navigation::goToMainMenu() { //might be redundant

}

void Navigation::addFriend() {
    ::sql::Statement *stmt;
	
	std::string query;
	stmt = con->createStatement();
	
	std::string firstName = "";
	std::string lastName = "";
	std::string friendID = "";
	
	std::cout << "Enter full name of user you would like to add to your friends list: ";
	std::cin >> firstName >> lastName;
	
	friendID = searchFriends(firstName,lastName);
	
	if(friendID != "NotFound"){	
		query = "INSERT INTO connections (user_id, friend_id) VALUES (\"" + this->currentUser + "\", \"" + friendID + "\")";
		stmt->execute("USE ece656project");
		stmt->execute(query);
		std::cout << "Added!" << std::endl;
		std::cout << std::endl;
		delete stmt;
		showMainMenu();
	}
	else{
		std::cout << "The user name you have entered does not exist" << std::endl;
		std::cout << std::endl;
		delete stmt;
		showMainMenu();
	}
}

void Navigation::showFriends() {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	
	std::string friendID = "";
	std::string query;
	stmt = con->createStatement();
	
	query = "SELECT first_name,last_name FROM users\
		INNER JOIN connections ON users.user_id = connections.friend_id\
		WHERE connections.user_id = \"" + this->currentUser + "\"";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);

	std::cout << std::endl;
	while(res->next()) {
		std::cout << res->getString("first_name") << " " << res->getString("last_name") << std::endl;
	}
	
	delete stmt;
	delete res;
	
	std::cout << std::endl;
	showMainMenu();
}

std::string Navigation::searchFriends(std::string firstName,std::string lastName) {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	
	std::string friendID = "";
	std::string query;
	stmt = con->createStatement();
	
	query = "SELECT user_id FROM users WHERE first_name = \"" + firstName + "\" AND last_name = \"" + lastName + "\"";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);
	
	while(res->next()) {
		friendID = res->getString("user_id");
		delete stmt;
	    delete res;
		return friendID;
	}
	
	delete stmt;
	delete res;
	
	return "NotFound";
}

void Navigation::logOut() {
	mainUXHandle();
}



