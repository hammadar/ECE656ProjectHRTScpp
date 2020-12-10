//
// Created by Hammad Rehman on 2020-11-24.
//

#include "Navigation.h"
#include "DatabaseClasses/User.h"
#include <string>
#include <sstream>
#include <time.h>
#include <ctime>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include "DatabaseHandling/recommendationEngine.h"
#include <sstream>
#include <map>

Navigation::Navigation(::sql::Connection *connection) {
	this->con = connection;
	this->credentialGen = new CredentialsGenerator();
	this->currentUser = "";
	this->currentLevel = -1;
	this->retrievedThreads = std::vector<std::pair<std::string,std::string>>();
	this->retrievedPosts = std::vector<std::vector<std::string>>();
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
	std::cout << "\t2. Search movie info by title" << std::endl;
	std::cout << "\t3. Find forums by title" << std::endl;
	std::cout << "\t4. Find ratings of movies by title" << std::endl;
	std::cout << "\t5. Get movie recommendations" << std::endl;
	std::cout << "\t6. Show friends" << std::endl;
	std::cout << "\t7. Add friend" << std::endl;
	std::cout << "\t8. Delete friend" << std::endl;
	std::cout << "\t9. Rate a movie" << std::endl;
	std::cout << "\t10. Show personal ratings" << std::endl;
	std::cout << "\t11. Log Out" << std::endl;
	
	int userInput;
    std::string titleName;
	
	std::cin >> userInput;
	
	switch (userInput) {
		case 1:
			showUserThreads();
			break;
		case 2:
			search();
			break;
		case 3:
			std::cout << "Enter the name of the movie:" << std::endl;
			//std::istringstream ss(titleName);
			std::cin.ignore();
			getline(std::cin, titleName);
		    showForum(titleName);
			break;
		case 4:
			getRatings();
			break;
		case 5:
			showRecommendations();
			break;
		case 6:
			showFriends();
			break;
		case 7:
			addFriend();
			break;
		case 8:
			deleteFriend();
			break;
		case 9:
			rateMovie();
			break;
		case 10:
			showPersonalRatings();
			break;
		case 11:
			logOut();
			break;
		default:
			showMainMenu();
			break;	
	}	
}

void Navigation::showUserThreads() {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::vector<std::vector<std::string>> threads;
    std::string tempThreadID;
    std::string tempForumID;
    std::string tempTitle;
    int threadView;
    int counter;
    query = "select distinct threads.thread_id as thread_id, threads.forum_id as forum_id, threads.title as title from threads inner join"
            " posts on threads.thread_id = posts.thread_id where posts.user_id = \"" + this->currentUser + "\"";
    threads = std::vector<std::vector<std::string>>();
    stmt = con->createStatement();
    res = stmt->executeQuery(query);
    counter = 1;
    while (res->next()) {
        std::vector<std::string> tempVec = std::vector<std::string>();
        tempThreadID = res->getString("thread_id");
        tempForumID = res->getString("forum_id");
        tempTitle = res->getString("title");
        tempVec.push_back(tempThreadID);
        tempVec.push_back(tempForumID);
        tempVec.push_back(tempTitle);
        std::cout << counter << ". " << tempTitle << std::endl;
        threads.push_back(tempVec);
        counter++;
    }
    if (counter > 1) {
        std::cout << "Select which thread to view:";
        std::cin >> threadView;
        threadView--;
        std::vector<std::string> tempVec = threads[threadView];
        this->showThread(tempVec[0]);
    } else {
        std::cout << "You are not participating in any threads." << std::endl;
        this->showMainMenu();
    }
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
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string temptitleID;
    std::string temptitle;
    std::string tempForumID;
    std::string forumID;
    std::string tempThreadID;
    std::string tempThreadTitle;
    std::string threadID;
    std::vector<std::pair<std::string,std::string>> titleVec;
    std::vector<std::string> forumIDs;
    int nextAction;
    int counter;
    stmt = con->createStatement();
    this->retrievedThreads.clear();

    query = "SELECT forum_id, titleID, primaryTitle FROM forum INNER JOIN titleBasics on forum.titleID = titleBasics.tconst where titleBasics.primaryTitle = \"" + title + "\"";
    stmt->execute("use ece656project");
    res = stmt->executeQuery(query);
    int count = 0;
    while (res->next()) {
        temptitleID = res->getString("titleID");
        temptitle = res->getString("primaryTitle");
        tempForumID = res->getString("forum_id");
        titleVec.push_back(std::pair<std::string, std::string>(temptitleID, temptitle));
        forumIDs.push_back(tempForumID);
        count++;
    }
    int selection = 0;
    if (count == 0) { //need to make a forum for this title
        std::string tempQuery = "SELECT tconst, primaryTitle from titleBasics where primaryTitle = \"" + title + "\"";
        int tempcount = 0;
        std::vector<std::pair<std::string,std::string>> tempTitleVec;
        std::string temptconst;
        std::string tempPrimaryTitle;
        res = stmt->executeQuery(tempQuery);
        tempTitleVec = std::vector<std::pair<std::string, std::string>>();
        while (res->next()) {
            temptconst = res->getString("tconst");
            tempPrimaryTitle = res->getString("primaryTitle");
            tempTitleVec.push_back(std::pair<std::string,std::string>(temptconst, tempPrimaryTitle));
            tempcount++;
        }
        if (tempcount <1) {
            std::cout << "No titles found with that name. Taking you back to main menu." << std::endl;
            delete stmt;
            delete res;
            goto label;
            } else if (tempcount>1) {
            std::cout << "There are " << tempcount << " matches for searched title " << title << ". Please specify which one:" << std::endl;
            int localcounter = 1;
            for (auto it = tempTitleVec.begin(); it!= tempTitleVec.end(); it++) {
                std::cout << localcounter << ". " << it->second << std::endl;
                localcounter++;
            }
            std::cin >> selection;
            selection--;
        }
        std::string newForumID = this->credentialGen->generateCredential("Forum", this->con);
        std::string newTitleID = tempTitleVec[selection].first;
        tempQuery = "insert into forum(forum_id, titleID) values (\"" + newForumID + "\",\"" + newTitleID + "\")";
        forumIDs.push_back(newForumID);
        stmt->execute(tempQuery);
    } else if (count>1) {
        std::cout << "There are " << count << " matches for searched title " << title << ". Please specify which one:" << std::endl;
        int localcounter = 1;
            for (auto it = titleVec.begin(); it != titleVec.end(); it++) {
                std::cout << localcounter << ". " << it->second << std::endl;
                localcounter++;
            }
        std::cin >> selection;
        selection--;
    }
    forumID = forumIDs[selection];

    query = "SELECT thread_id, forum_id, title from threads where forum_id = \"" + forumID + "\"";
    res = stmt->executeQuery(query);
    counter = 1;
    while (res->next()) {
        tempThreadID = res->getString("thread_id");
        tempThreadTitle = res->getString("title");
        retrievedThreads.push_back(std::pair<std::string,std::string>(tempThreadID, tempThreadTitle));
        std::cout << counter << ". " << tempThreadTitle << std::endl;
        counter++;
    }
    if (counter == 1) {
        std::cout << "There are no threads in this forum for title " << title << std::endl;
    }
    nextAction = 0;
    std::cout << "Select thread number to view posts or type 0 to go to main menu or -1 to make a new thread in this forum: " << std::endl;
    std::cin >> nextAction;

    delete stmt;
    delete res;

    if (nextAction > 0) {
        threadID = retrievedThreads[nextAction-1].first;
        this->showThread(threadID);
        this->showMainMenu();
    } else if (nextAction < 0){
        this->makeThread(forumID);
        this->showMainMenu();
    }
      else {
        label:
        this->showMainMenu();
    }
}

void Navigation::showRecommendations() {
    auto *recEng = new RecommendationEngine();
    recEng->getRecommendations(this->currentUser, this->con);
    this->showMainMenu();
}

void Navigation::showThread(std::string threadID) {
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string tempPostID;
    std::string tempUserFirstName;
	std::string tempUserLastName;
    std::string tempPost;
    std::vector<std::string> tempResults;
	std::map<int,std::string> post_id_mapping;
	
    this->retrievedPosts.clear();
    stmt = con->createStatement();

    query = "SELECT post_id, users.first_name, users.last_name, post FROM posts JOIN users ON posts.user_id = users.user_id WHERE thread_id = \"" + threadID + "\"";
    stmt->execute("use ece656project");
    res = stmt->executeQuery(query);
    int counter = 1;
	
	std::cout << std::endl;
	std::cout << "Posts in Thread: " << std::endl;

    while (res->next()) {
        tempResults.clear();
        tempPostID = res->getString("post_id");
        tempUserFirstName = res->getString("first_name");
		tempUserLastName = res->getString("last_name");
        tempPost = res->getString("post");
        tempResults.push_back(tempPostID);
        tempResults.push_back(tempUserFirstName);
		tempResults.push_back(tempUserLastName);
        tempResults.push_back(tempPost);
        this->retrievedPosts.push_back(tempResults);
		post_id_mapping[counter] = tempPostID;
        std::cout << counter << ". By " << tempUserFirstName << " " << tempUserLastName << ": " << std::endl;
        std::cout << "Text: " << tempPost << std::endl;
        counter++;
    }

    delete res;
    delete stmt;
    int userInput;

    std::cout << "Please select next action: Add to Thread (1) , Delete Post from Thread (2) , Return to Main Menu (3)" << std::endl;
    std::cin >> userInput;
	
	switch (userInput) {
		case 1:
			this->postInThread(threadID);
			break;
		case 2:
			this->deletePostInThread(threadID,post_id_mapping);
			break;
		case 3:
			this->showMainMenu();
			break;
		default:
			showMainMenu();
			break;
	}   
}

void Navigation::postInThread(std::string threadID) {
    ::sql::Statement *stmt;
    std::string query;
    std::string postText;
    std::string postID;
    stmt = con->createStatement();

    std::cout << "Please enter your post below:" << std::endl;
    std::cin.ignore();
    getline(std::cin, postText);
    postID = this->credentialGen->generateCredential("Post", this->con);
    query = "insert into posts(post_id, thread_id, user_id, post) values (\"" + postID + "\", \"" + threadID +
            "\", \"" + this->currentUser + "\",\"" + postText + "\")";
    stmt->execute(query);
    delete stmt;
	
	showMainMenu();

}

void Navigation::deletePostInThread(std::string threadID,std::map<int,std::string> post_id_mapping) {
    ::sql::Statement *stmt;
    std::string query;
    int post_num;
    stmt = con->createStatement();

    std::cout << "Please enter post number you would like to remove:" << std::endl;
    std::cin >> post_num;
    
    query = "DELETE FROM posts WHERE user_id =\"" + this->currentUser + "\" AND thread_id = \"" + threadID + "\" AND post_id = \"" + post_id_mapping[post_num] + "\"";
    stmt->execute(query);
    delete stmt;
	
	std::cout << "Deleted!" << std::endl;
	std::cout << std::endl;
	
	showMainMenu();
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

void Navigation::deleteFriend() {
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
	
	std::cout << "Current friends:" << std::endl;

	std::cout << std::endl;
	while(res->next()) {
		std::cout << res->getString("first_name") << " " << res->getString("last_name") << std::endl;
	}
	
	delete res;
	
	stmt = con->createStatement();
	
	std::string firstName = "";
	std::string lastName = "";
	
	std::cout << std::endl;
	std::cout << "Enter full name of user you would like to remove from your friends list:" << std::endl;
	std::cin >> firstName >> lastName;
	
	friendID = searchFriends(firstName,lastName);
	
	if(friendID != "NotFound"){	
		query = "DELETE FROM connections WHERE user_id =\"" + this->currentUser + "\" AND friend_id = \"" + friendID + "\"";
		stmt->execute("USE ece656project");
		stmt->execute(query);
		std::cout << "Deleted!" << std::endl;
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

void Navigation::makeThread(std::string forumID) {
    ::sql::Statement *stmt;
    std::string text1;
    std::string title;
    std::string threadID;
    std::string postID;
    std::string query;

    threadID = this->credentialGen->generateCredential("Thread", this->con);
    postID = this->credentialGen->generateCredential("Post", this->con);
    stmt = con->createStatement();

    std::cout << "Enter the title of the thread:" << std::endl;
    std::cin.ignore();
    getline(std::cin, title);
    std::cout << "Enter the the first post for this thread:" << std::endl;
    //std::cin.ignore();
    getline(std::cin, text1);

    query = "insert into threads(thread_id, forum_id, title) values (\"" + threadID + "\",\"" + forumID + "\",\"" + title + "\")";
    stmt->execute(query);
    query = "insert into posts(post_id, thread_id, user_id, post) values (\"" + postID + "\",\"" + threadID + "\",\"" + this->currentUser + "\",\"" + text1 + "\")";
    std::cout << query << std::endl;
    stmt->execute(query);
    delete stmt;
    this->showMainMenu();

}

void Navigation::rateMovie() {
    ::sql::Statement *stmt;
	::sql::ResultSet  *res;
	
	std::string query;
	stmt = con->createStatement();
	
	std::string title = "";
	std::string rating  = "";
	std::string title_id = "";
	
	std::cout << "Enter name of movie you would like to rate: ";
	std::getline(std::cin >> std::ws, title);
	
	query = "SELECT * FROM titleBasics WHERE primaryTitle = \"" + title + "\" LIMIT 1";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);

	while(res->next()) {

		title_id = res->getString("tconst");
		
		std::cout << "What rating (out of 10.0) would you like to give: "<< title << std::endl;
		std::cin >> rating;
		
		query = "INSERT INTO userRatings (user_id, titleID, rating) VALUES (\"" + this->currentUser + "\", \"" + title_id +  "\", \"" + rating + "\")";
		stmt->execute("USE ece656project");
		std::cout << query << std::endl;
		stmt->execute(query);
		std::cout << "Added!" << std::endl;
		std::cout << std::endl;
		delete stmt;
		showMainMenu();
	}
	
	std::cout << "The movie you have entered does not exist in this database" << std::endl;
	std::cout << std::endl;
	delete stmt;
	showMainMenu();
	
}

void Navigation::showPersonalRatings() {
	::sql::Statement *stmt;
	::sql::ResultSet  *res;
	
	std::string query;
	stmt = con->createStatement();
	
	query = "SELECT titleBasics.primaryTitle,rating FROM userRatings INNER JOIN titleBasics ON userRatings.titleID = titleBasics.tconst";
	stmt->execute("USE ece656project");
	res = stmt->executeQuery(query);

	std::cout << std::endl;

	while(res->next()) {
		std::cout << res->getString("primaryTitle") << "|" << res->getString("rating") << std::endl;
	}
	
	delete stmt;
	delete res;
	
	std::cout << std::endl;
	showMainMenu();
}

/*
 * do: delete friends
 * delete your own posts
 * in showThread, show user name in addition to user id
 * User: add ability to rate movies, this ties in to recommendations
 * Recommendations: do mean variance of user ratings from average rating, use that as reference to estimate movies they would like
 */




