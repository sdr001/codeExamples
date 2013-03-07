/*****************************************************************
queries.cpp
Written By: Sean Richardson

Queries Interface
******************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "database.hpp"
#include "genRoutines.hpp"
#include "cgin.hpp"
#include "queries.hpp"
#include "gRout2Db.hpp"


using namespace std;


void loginCheck(const string &user, const string &pwd, string &skey)
// RETURNS 0 ON SUCCESS
// 1: COULD NOT OPEN DB
// 2: QUERY HAD ERROR
// 3: USER NOT FOUND
{

string 	query,
		userid;
		
rowvectype result;
	
if (DATABASE_OpenDB("sdr001", "****", "dbsdr001"))
   exit(0);
   
query = "SELECT password FROM users WHERE userid='" + user + "' AND password='" + pwd + "';";
if (DATABASE_Do_query(query, result)) 
   exit(0);
if (result.size() == 0)
   exit(0);

// Generate session key (put it into skey)

skey = updateSessionFile(userid, 300);

menuScreen(user,skey);  

DATABASE_CloseDB();


}

void checkUser(const string &user, int answer, const string &fname, const string &lname, const string &birthdate, const string &hometown, const string &currentCity, const string &sex, const string &userid, const string &likes, const string &dislikes, const string &statement, const string &passwd1)
{

string 	query;
		
rowvectype result;
vector<string> maxPid;

if (DATABASE_OpenDB("sdr001", "****", "dbsdr001"))
   exit(0);

query = "SELECT * FROM users WHERE userid='" + user + "';";

if (DATABASE_Do_query(query, result)) 
		cout << "Query caused an error... " << endl;
    if (result.size() != 0)
		backToLogin("<strong>There is already a user with this user id</strong>");
		
query = "SELECT MAX(pid) FROM users;";


if (DATABASE_Do_query(query, result))
{ 
		cout << "Query caused an error... " << endl;
}		
for(int i=0; i < result.size();i++)
{
maxPid.push_back(result[i][0]);
}
maxPid[0][0] += 1;

query = "INSERT INTO users(Fname,Lname,Bdate,Hometown,CurCity,Sex,userid,likes,dislies,statement,password,pid) VALUES ('" + fname + "', '" + lname + "', '" + birthdate + "', '" + hometown+ "', '" + currentCity + "', '" + sex + "', '" + userid + "', '" + likes + "', '" + dislikes + "', '" + statement + "','" + passwd1 + "','" + maxPid[0][0] + "');";
		
   if (DATABASE_Do_query(query, result)) 
		cout << "Query caused an error... " << endl;
  
   else 
		backToLogin("<strong>Your regisration was successful... </strong>");
	  
	 DATABASE_CloseDB();
	
	
}
		
void registerUser(const string &fname, const string &lname, const string &birthdate, const string &hometown, const string &currentCity, const string &sex, const string &userid, const string &likes, const string &dislikes, const string &statement, const string &passwd1)

{
string 	query;
		
rowvectype result;

if (DATABASE_OpenDB("sdr001", "****", "dbsdr001"))
   exit(0);
   
query = "INSERT INTO users(Fname,Lname,Bdate,Hometown,CurCity,Sex,userid,likes,dislies,statement,password) VALUES ('" + fname + "', '" + lname + "', '" + birthdate + "', '" + hometown+ "', '" + currentCity + "', '" + sex + "', '" + userid + "', '" + likes + "', '" + dislikes + "', '" + statement + "','" + passwd1 + "');";
		
		
   if (DATABASE_Do_query(query, result)) 
		cout << "Query caused an error... " << endl;

   else 
		backToLogin("<strong>Your regisration was successful... </strong>");
	  
		DATABASE_CloseDB();
}





