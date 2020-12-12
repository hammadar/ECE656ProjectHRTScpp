

DROP TABLE IF EXISTS connections;
DROP TABLE IF EXISTS contributors;
DROP TABLE IF EXISTS forum;
DROP TABLE IF EXISTS nameBasics;
DROP TABLE IF EXISTS posts;
DROP TABLE IF EXISTS principals;
DROP TABLE IF EXISTS ratings;
DROP TABLE IF EXISTS threads;
DROP TABLE IF EXISTS titleBasics;
DROP TABLE IF EXISTS titleTypes;
DROP TABLE IF EXISTS titles;
DROP TABLE IF EXISTS userCategories;
DROP TABLE IF EXISTS userRatings;
DROP TABLE IF EXISTS users;


CREATE TABLE `connections` (
  `user_id` varchar(255) NOT NULL,
  `friend_id` varchar(255) NOT NULL,
  PRIMARY KEY (`user_id`,`friend_id`)
); 

CREATE TABLE `contributors` (
  `forum_id` varchar(255) NOT NULL,
  `contributor_id` varchar(255) NOT NULL,
  PRIMARY KEY (`forum_id`,`contributor_id`)
);

CREATE TABLE `forum` (
  `forum_id` varchar(255) NOT NULL,
  `titleID` varchar(255) NOT NULL,
  PRIMARY KEY (`forum_id`),
  KEY `titleID` (`titleID`),
  CONSTRAINT `forum_ibfk_1` FOREIGN KEY (`titleID`) REFERENCES `titles` (`titleID`)
);

CREATE TABLE `nameBasics` (
  `nconst` varchar(255) NOT NULL,
  `primaryName` varchar(255) NOT NULL,
  `birthYear` int DEFAULT NULL,
  `deathYear` int DEFAULT NULL,
  `primaryProfession` varchar(255) DEFAULT NULL,
  `knownForTitles` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`nconst`)
);

CREATE TABLE `posts` (
  `post_id` varchar(255) NOT NULL,
  `thread_id` varchar(255) NOT NULL,
  `user_id` varchar(255) NOT NULL,
  `post` mediumtext,
  PRIMARY KEY (`post_id`),
  KEY `thread_id` (`thread_id`),
  KEY `user_id` (`user_id`),
  CONSTRAINT `posts_ibfk_1` FOREIGN KEY (`thread_id`) REFERENCES `threads` (`thread_id`),
  CONSTRAINT `posts_ibfk_2` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`)
);

CREATE TABLE `principals` (
  `tconst` varchar(255) NOT NULL,
  `ordering` int NOT NULL,
  `nconst` varchar(255) NOT NULL,
  `category` varchar(255) DEFAULT NULL,
  `job` varchar(300) DEFAULT NULL,
  `chrctr` varchar(500) DEFAULT NULL,
  PRIMARY KEY (`tconst`,`ordering`,`nconst`)
);


CREATE TABLE `ratings` (
  `tconst` varchar(255) NOT NULL,
  `averageRating` float NOT NULL,
  `numVotes` int DEFAULT NULL,
  PRIMARY KEY (`tconst`)
);

CREATE TABLE `threads` (
  `thread_id` varchar(255) NOT NULL,
  `forum_id` varchar(255) NOT NULL,
  `title` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`thread_id`),
  KEY `forum_id` (`forum_id`),
  CONSTRAINT `threads_ibfk_1` FOREIGN KEY (`forum_id`) REFERENCES `forum` (`forum_id`)
);

CREATE TABLE `titleBasics` (
  `tconst` varchar(255) NOT NULL,
  `titleType` varchar(255) DEFAULT NULL,
  `primaryTitle` varchar(500) DEFAULT NULL,
  `originalTitle` varchar(500) DEFAULT NULL,
  `isAdult` tinyint(1) DEFAULT NULL,
  `startYear` int DEFAULT NULL,
  `endYear` int DEFAULT NULL,
  `runtimeMinutes` int DEFAULT NULL,
  `genres` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`tconst`)
);

CREATE TABLE `titleTypes` (
  `titleID` varchar(255) NOT NULL,
  `alternative` tinyint(1) DEFAULT NULL,
  `dvd` tinyint(1) DEFAULT NULL,
  `festival` tinyint(1) DEFAULT NULL,
  `tv` tinyint(1) DEFAULT NULL,
  `video` tinyint(1) DEFAULT NULL,
  `working` tinyint(1) DEFAULT NULL,
  `original` tinyint(1) DEFAULT NULL,
  `imdbDisplay` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`titleID`)
);

CREATE TABLE `titles` (
  `titleID` varchar(255) NOT NULL,
  `ordering` int NOT NULL,
  `title` varchar(255) NOT NULL,
  `region` varchar(255) DEFAULT NULL,
  `language` varchar(255) DEFAULT NULL,
  `attributes` mediumtext,
  `isOriginalTitle` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`titleID`,`ordering`)
);

CREATE TABLE `userCategories` (
  `user_id` varchar(255) NOT NULL,
  `category` varchar(255) NOT NULL,
  PRIMARY KEY (`user_id`,`category`)
);

CREATE TABLE `userRatings` (
  `user_id` varchar(255) NOT NULL,
  `titleID` varchar(255) NOT NULL,
  `rating` float NOT NULL,
  PRIMARY KEY (`user_id`,`titleID`)
);

CREATE TABLE `users` (
  `user_id` varchar(255) NOT NULL,
  `first_name` varchar(255) NOT NULL,
  `last_name` varchar(255) NOT NULL,
  `birth_date` date NOT NULL,
  `stateProvince` varchar(50) DEFAULT NULL,
  `country` varchar(50) NOT NULL,
  PRIMARY KEY (`user_id`)
);

CREATE TABLE `actorTitles` (
  `nconst` varchar(255) NOT NULL,
  `tconst` varchar(255) NOT NULL,
  PRIMARY KEY (`nconst`,`tconst`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci





