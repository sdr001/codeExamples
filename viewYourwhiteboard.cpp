/*****************************************************************
viewWhiteboard.cpp
Written By: Sean Richardson

View someone else's personal whiteboard
******************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "cgin.hpp"
#include "genRoutines.hpp"
#include "gRout2.hpp"

using namespace std;

main()
{ cgin input;
  ifstream ifl;
  ofstream ofl;
  int ownerIndex;                   // *** LOCATION OF FRIEND IN USERS LIST
  
  string path = "../../private/",    // *** PATH TO PRIVATE DATA
         pfn,                        // *** PATH AND FILENAME FOR FILES
         userid,                     // *** USER ID
         filePasswd,                 // *** PASSWORD ON FILE
         userName,                   // *** USER REAL NAME ON FILE
		 comment,
         skey,                       // *** SESSION KEY GENERATED FOR USER
         owner;                  // *** FRIEND BEING DENIED PERMISSION
         
  vector<string>userList;           
         
// *** BRING IN THE DATA AND VALIDATE SESSION
input.process_input();
userid = input["uid"];
owner = input["index"];

if (badAlphaNumStr(userid))
   backToLogin("Malicious User ID Error...");  
skey = input["skey"];
validateSession(userid, skey);

ownerIndex = atoi(owner.c_str());
if (ownerIndex < 0)
	owner = ""; 
else
	{
	pfn = path + "allUsers.sys";
	vectorFromFile(pfn, userList); 
	if (ownerIndex >= userList.size())
	owner = "";
	else 
		owner = userList[ownerIndex];
	}
pfn = path + userid + ".wb";
ifl.open(pfn.c_str(), ios::out | ios::app);
	while (ifl.eof())
	{
	ifl >> comment;
	cout << comment << endl;
	}
ifl.close();



// *** UPDATE THE SESSION
skey = updateSessionFile(userid, 300); // --- NEW SESSION KEY EACH TIME,
                                       //     5 MIN TIMEOUT
// *** OUTPUT WHITEBOARD PAGE
viewYourwhiteboard(userid, skey, owner);
}