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
    std::string titleName;
	
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
			std::cout << "Enter the name of the movie:" << std::endl;
			//std::istringstream ss(titleName);
			std::cin.ignore();
			getline(std::cin, titleName);
		    showForum(titleName);
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

void Navigation::showThreadPosts() { //might be redundant

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

}

void Navigation::showThread(std::string threadID) {
    //HR
    ::sql::Statement *stmt;
    ::sql::ResultSet  *res;
    std::string query;
    std::string tempPostID;
    std::string tempuserID;
    std::string tempPost;
    std::vector<std::string> tempResults;
    this->retrievedPosts.clear();
    stmt = con->createStatement();

    query = "SELECT post_id, user_id, post from posts where thread_id = \"" + threadID + "\"";
    stmt->execute("use ece656project");
    res = stmt->executeQuery(query);
    int counter = 1;

    while (res->next()) {
        tempResults.clear();
        tempPostID = res->getString("post_id");
        tempuserID = res->getString("user_id");
        tempPost = res->getString("post");
        tempResults.push_back(tempPostID);
        tempResults.push_back(tempuserID);
        tempResults.push_back(tempPost);
        this->retrievedPosts.push_back(tempResults);
        std::cout << counter << ". By " << tempuserID << ": " << std::endl;
        std::cout << "Text: " << tempPost << std::endl;
        counter++;
    }

    delete res;
    delete stmt;
    bool add = false;

    std::cout << "Do you want to add to this thread? Press 1 for yes, 0 for no" << std::endl;
    std::cin >> add;

    if (add) {
        this->postInThread(threadID);
    }
    this->showMainMenu();


}

void Navigation::postInThread(std::string threadID) {
    //HR
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

}

void Navigation::goToMainMenu() { //might be redundant

}

void Navigation::addFriend() {
    //TS
}

void Navigation::showFriends() {
    //TS
}

void Navigation::searchFriends(std::string userName) {
    //TS
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





