#include <iostream>
#include <sstream>
#include "UserData.h"
#include "Database.h"
#include <algorithm>
#include "BabelServer.h"

void	testServerDatabase()
{
	{
		BabelServer server;

		server.start();
		Database	db;
		UserData	*user = new UserData("toto", "tata");

		db.addUser(user->getName(), user->getPassword());
		//server.setDb(db);
	}
	BabelServer server;
	//std::cout << server.getDb().getUsers().size() << std::endl;
	system("PAUSE");
}

void	testUserData()
{
	UserData *user1 = new UserData();
	UserData *user2 = new UserData();
	std::list<UserData *> myList;

	std::cout << "ADDING USER2 TO USER1 FRIENDLIST !" << std::endl;
	user1->addFriend(*user2);
	std::cout << "SETTING USER2 NAME TO toto" << std::endl;
	user2->setName(std::string("toto"));
	std::cout << "PRINTING USER 2 NAME " << std::endl;
	std::cout << user2->getName() << std::endl;
	std::cout << "Deleating two time user2 from user 1 friendlist" << std::endl;
	std::cout << user1->removeFriend(*user2) << std::endl;
	std::cout << user1->removeFriend(*user2) << std::endl;
	std::cout << "GETTING user2 friendlist" << std::endl;
	myList = user2->getListFriend();
	std::cout << "PRINTING LISTFRIEND SIZE" << std::endl;
	std::cout << myList.size() << std::endl;
	std::cout << "ADDING USER1 TO USER2 FRIENDLIST" << std::endl;
	user2->addFriend(*user1);
	std::cout << "PRINTING NEW LIST SIZE" << std::endl;
	std::cout << user2->getListFriend().size() << std::endl;
	user1->addFriendRequest(*user2);
	std::cout << "PRINTING FRIENDLISTREQUEST SIZE" << std::endl;
	std::cout << user1->getListFriendRequest().size() << std::endl;
	system("PAUSE");
}

void	displayName(UserData *user)
{
	std::cout << user->getName() << std::endl;
}


void	testSerializingUserData()
{
	std::string tata = "tata";
	std::string toto = "toto";
	std::string tutu = "tutu";
	std::string titi = "titi";

	Database	*db = new Database();
	UserData *user1 = new UserData(tata, tutu);
	UserData *user2 = new UserData(toto, titi);
	std::list<UserData *>		myList;
	std::list<UserData *>		myList2;
	std::string					s;

	myList.push_back(user1);
	myList.push_back(user2);
	db->addUser(user1->getName(), user1->getPassword());
	db->addUser(user2->getName(), user2->getPassword());
	s = db->serialize();
	std::cout << s << std::endl;
	db->deserialize(s);
	system("PAUSE");
}

void	testSerializeDatabase()
{
	std::string	r;
	std::cout << "CREATING DATABASES" << std::endl;
	Database *db = new Database();
	Database *db2 = new Database();
	std::cout << "ADDING USERS IN DATABASE" << std::endl;
	db->addUser("user1", "pwd1");
	db->addUser("user2", "pwd2");
	std::cout << "PRINTING SERIALIZED STRING " << std::endl;
	r = db->serialize();
	std::cout << r << std::endl;
	std::cout << "DB2 SIZE BEFORE DESERIALIZING" << std::endl;
	std::cout << db2->getUsers().size() << std::endl;
	std::cout << "DESERIALIZING IN DATABASE2" << std::endl;
	db2->deserialize(r);
	std::cout << db2->getUsers().size() << std::endl;
	system("PAUSE");
}
