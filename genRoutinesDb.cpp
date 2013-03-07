/*****************************************************************
genRoutines.cpp
Written By: Sean Richardson

General Routines that are useful in a variety of places
******************************************************************/

#include <iostream>
#include <fstream>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genRoutines.hpp"
#include "database.hpp"

using namespace std;

/*****************************************************************
******************************************************************
TIMESTAMP STUFF
******************************************************************
******************************************************************/

/**************************************************************************
tstamp.cpp

Created By: Randy Campbell

DEPENDENCIES: NONE

DESCRIPTION:

Provides a timestamp class.

The class represents a timestamp as a long long - which is at least a
64 bit integer in C.  Although long long is not, de jure, a type in C++
at present (JAN 2010) it is slated to be added to the next ISO C++ standard
and there are compilers (such as g++) that implement it.  If your compiler
implements a 64-bit int with another name, then you will need to modify the
class.  If your compiler does not implement 64-bit integers then this will
not work for you.

The timestamp type assumes that days begin at midnight and represents 
a time stamp by the number of seconds elapsed since midnight, Jan 1, of 
Julian Day 0 (altering the Julian Day scheme so that the days start at
midnight instead of noon).  Timestamps may be compared using relational 
operators.  They may also be subtracted from each other to yield
the number of seconds between them.  

Timestamps assume that a desired year, month, day, hour, minute, second
values can be represented as ints and that the largest number of desired Julian
Days can be represented as a long.

The routines used to convert from Julian Days to Gregorian Dates and
vice versa are based on the sample routines from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).

ASSUMPTIONS:
* The Julian Day scheme used here assumes that Days begin at midnight
  (rather than noon)
* Assumes that the largest desired year can fit in an int
* Assumes that the largest desired number of Julian Days can fit into
  a long.

**************************************************************************/

/*************************************************************************
**************************************************************************
Julian Day functions (static, so local to module)
**************************************************************************
**************************************************************************/

static void GetGregorianDate(long jdays, int *year, int *month, int *day);
static long GetJulianDays (int year, int month, int day);

/*************************************************************************
**************************************************************************/

static void GetGregorianDate(long jdays, int &year, int &month, int &day)  
/*----------------------------------------------------------
Parameters:

jdays:  Number of Julian Days
*year:  Integer giving the year
*month: Integer giving the month (starts at 1)
*day:   Integer giving the day (starts at 1)

~~~~~~~~~~~
Description:

Converts a number of Julian Days to a Gregorian Date

This routine based on the sample routine from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).
------------------------------------------------------------*/
{ long t1, t2, ty, tm, td;

t1= jdays+68569L;
t2= 4*t1/146097L;
t1= t1-(146097L*t2+3L)/4L;
ty= 4000L*(t1+1L)/1461001L;
t1= t1-1461L*ty/4L+31L;
tm= 80L*t1/2447L;
td= t1-2447L*(tm)/80L;
t1= tm/11L;
tm= tm+2L-12L*t1;
ty= 100L*(t2-49L)+ty+t1;
year = (int) ty;
month = (int) tm;
day = (int) td;
}

/*************************************************************************
**************************************************************************/

static int daylims[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//======================================================================

static long GetJulianDays (int year, int month, int day)
/*----------------------------------------------------------
Parameters:

year:  Integer giving the year
month: Integer giving the month (starts at 1)
day:   Integer giving the day (starts at 1)

~~~~~~~~~~~
Description:

Returns the number of Julian Days matching the given Gregorian
Date  Year must be >= 1.
Returns >= 0 on success and negative number on failure

Failure Values:
-1: Year < 1
-2: Month not in range 1-12
-3: Day out of range for the month
-4: Number of Julian Days is not in range JULIAN_MIN - JULIAN_MAX 


This routine based on the sample routine from the national naval 
observatory website which, in turn, created them from algorithms that
came from:

Fliegel, H.F. and van Flandern, T.C., Communications of the ACM,
Vol. 11, No. 10 (October 1968).
------------------------------------------------------------*/
{ long jdays,         //*** Julian days
       ty,            //*** Temp Years
       tm,            //*** Temp Months
       td,            //*** Temp Days
       extraday = 0L;  //*** Extra day for leap years

if (year < 1)
   return -1;
ty = year;
   
if ((month < 1) || (month > 12))
   return -2;
tm = month;
   
if ((month == 2) && (((year % 100) && !(year % 4)) || !(year % 400)))
   extraday = 1L;

if ((day < 1) || (day > (daylims[month-1] + 
             ((month == 2) ? extraday : 0))))
   return -3;
td = day;

jdays= td-32075L+1461L*(ty+4800L+(tm-14L)/12L)/4L
              +367L*(tm-2L-(tm-14L)/12L*12L)/12L
              -3L*((ty+4900L+(tm-14L)/12L)/100L)/4L;
              
return jdays;
}

/*************************************************************************
**************************************************************************
friend functions
**************************************************************************
**************************************************************************/

tstamp operator+(long seconds, const tstamp &stamp)
// Add seconds to a time stamp (seconds is first operator)
{ tstamp rval; 

rval.stmp = stamp.stmp + seconds; 
return rval;
}


/*************************************************************************
**************************************************************************
member functions
**************************************************************************
**************************************************************************/

/************************************************************
tstamp::getDayNum

Return the number in the week (Sunday: 1, Monday: 2, Tuesday: 3, etc.)
of the day in the date given by the timestamp
------------------------------------------------------------*/
int tstamp::getDayNum()
{ int temp;

temp = (int) ((stmp/DAYSECLL) % 7LL);
if (temp != 6)
   return temp + 2; //*** Any day but Sunday

return 1; //*** Sunday
}

/************************************************************
tstamp::getDate

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)

Return all constituent date parts of the time stamp through
pointer params.  
------------------------------------------------------------*/
void tstamp::getDate (int &year, int &month, int &day)
{ long jdays;    //*** Julian days (giving the date)
      
GetGregorianDate((long) (stmp/DAYSECLL), year, month, day); 
}

/************************************************************
tstamp::getStampParts

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)
hour:     Hours of the timestamp   (range: 0-23)
min:      Minutes of the timestamp (range: 0-59)
sec:      Seconds of the timestamp (range: 0-59)

Return all constituent time and date parts of the time stamp through
pointer params.  
------------------------------------------------------------*/
void tstamp::getStampParts (int &year, int &month, int &day,
                    int &hour, int &min,   int &sec)
{ long tsec;     //*** Time of day (in seconds)
      
tsec = (long) (stmp % DAYSECLL);
GetGregorianDate((long) (stmp/DAYSECLL), year, month, day); 
hour = (int) (tsec / HOURSECL);
min = (int) ((tsec % HOURSECL) / 60L);
sec = (int) (tsec % 60L);
}


/************************************************************
tstamp::setCurrentDateTimeStamp

Parameters:
uselocal: If NZ, use local time, otherwise Universal Time

Set the timestamp to the current date and time
------------------------------------------------------------*/
void tstamp::setCurrentDateTimeStamp (int uselocal)
{ time_t curdate;     //*** Current date in time.h format
  struct tm *tdptr; //*** Defined by time.h
  
curdate = time(NULL);

if (uselocal)
   tdptr = localtime(&curdate);
else
   tdptr = gmtime(&curdate);
   
stmp =  GetJulianDays(tdptr->tm_year + 1900, tdptr->tm_mon + 1, 
                       tdptr->tm_mday) * DAYSECLL;
                       
stmp +=  tdptr->tm_hour * HOURSECL + tdptr->tm_min * MINUTESECL + 
                tdptr->tm_sec;
}

/************************************************************
tstamp::setTimeStamp

Parameters:
year:     Year of the timestamp
mon:      Month of the timestamp   (range: 1-12)
day:      Day of the timestamp     (range: 1-31, or less depending on month)
hour:     Hours of the timestamp   (range: 0-23)
min:      Minutes of the timestamp (range: 0-59)
sec:      Seconds of the timestamp (range: 0-59)

Set the timestamp to the time and date indicated
Note... Violating range expectations will likely result in incorrect return
value.
------------------------------------------------------------*/
void tstamp::setTimeStamp (int year, int mon, int day,
                     int hour, int min, int sec)
{       
stmp = GetJulianDays (year, mon, day);
stmp = (stmp * DAYSECLL) + (hour * HOURSECL)
                                  + (min * MINUTESECL) + sec;
}



/*****************************************************************
******************************************************************
RANDOM STRING STUFF
******************************************************************
******************************************************************/

#define SEEDSIZE 8
   
void randomString(string &rstr, int slen, const string &basechars)
{ unsigned long param;
  char seed[SEEDSIZE];
  ifstream randSource;
  int i;

// *** GET SEED VALUE  
randSource.open("/dev/urandom", ios::in|ios::binary);
randSource.read(seed, SEEDSIZE);
randSource.close();

// *** SET SEED
param = 0L;
for (i = 0; i < SEEDSIZE; i++)
   {
   param <<= 8;
   param += seed[i];
   } // -- END FOR
   
// *** GENERATE THE RANDOM STRING
rstr = "";
for (i = 0; i < slen; i++)
   {
   param = 1664525L * param + 1013904223L;
   rstr += basechars[static_cast<unsigned int>(param >> 16) % basechars.size()];
   } // -- END FOR
} // -- END 
    

/*****************************************************************
******************************************************************
MISC STUFF
******************************************************************
******************************************************************/

/********************
alphaNumStr

This routine checks a string to ensure that it is only composed of 
letters and numbers.

str: string to check

Returns: 0 if a good string, NZ if bad
*********************/

int badAlphaNumStr(const string &str)
{ int i;

for (i = 0; i < str.size(); i++)
   if (!isalnum(str[i]))
      return 1;
      
return 0;
}


/********************
backToLogin

This routine goes back to the login screen and allows a message at the
top of it.  It outputs its own content type header.

msg: message to output

Returns: void
*********************/

void backToLogin(const string &msg)
{
cout << "Content-Type: text/html\n\n";

cout << "<html><head><title>Login</title></head>\n"
     << "\n" << msg << "\n<h2>Login</h2>\n"
     << "<form name=\"login\"\n"
     << "action=\"https://gimli.morningside.edu/~sdr001/cgi-bin/loginDb.cgi\"\n"
     << "method=\"POST\">\n"
     << "User ID: <br/><input name=\"uid\" type=\"text\"><br /><br />\n"
     << "Password: <br/><input name=\"pwd\" type=\"password\"><br /><br />\n"
     << "<input type=\"submit\" value=\"Login\">\n</form>\n\n"
     << "<br /><br /><br /><br />\n<a href=\"../registration.html\">\n"
     << "Not registered?  Register here... </a>\n\n</body></html>" << endl;
}


/********************
updateSessionFile

This routine updates the session file or creates it if it doesn't exist.
If the skey parameter is the empty string, a new skey is generated of the
length given by skeyLen.  Otherwise, the skey sent in is used as the skey.

uid: user id
toutSeconds: Number of seconds before a timeout
skey: session key... if "" then generate new, otherwise use this one.  If key
      is changed, then new key is returned through this parameter.
skeyLen: Length of new session key if one is to be generated

Returns: New session key (key has size 0 if there was an error)
*********************/

const string &updateSessionFile(const string &uid, unsigned toutSeconds, 
                                const string &skey, int skeyLen)
{ ofstream ofl;
  tstamp ts;
  static string skeyOut = skey; // --- SESSION KEY TO OUTPUT
  string pfname = PATH,
         toutStr,        // --- STRING FORM OF TIMEOUT
		 query;
         
// *** GENERATE NEW SESSION KEY IF NEEDED
if (skeyOut.size() == 0) // --- NEED A NEW ONE
   {
   if (skeyLen < 1) // --- SESSION KEY LENGTH MUST BE AT LEAST 1
      skeyLen = 1;
   randomString(skeyOut, skeyLen);
   }
   
// *** GENERATE TIMEOUT VALUE AND ASSOCIATED STRING
ts.setCurrentDateTimeStamp(1); // --- SET TO CURRENT LOCAL DATE & TIME
ts += toutSeconds; // --- ADD AMOUNT OF TIME BEFORE TIME OUT
ts.stampToString (toutStr); // --- CONVERT TIMESTAMP TO A STRING

// *** WRITE IT ALL OUT
//OPEN DATABASE
if (DATABASE_OpenDB("sdr001", "bus72lib", "dbsdr001"))
   {
   cout << "Failed to open the database... Goodbye." << endl;
   exit(0);
   }
else

	query = "INSERT INTO users(username,skey,toutStr) VALUES ('" + uid + "', '" + skeyOut + "', '" + toutStr + "');";
   
//pfname += uid + ".ses";
/*****
ofl.open(pfname.c_str());
if (!ofl) // --- ERROR OPENING FILE - YEP, THIS IS BAD
   skeyOut = "";
else
   {
   ofl << skeyOut << '\n' << toutStr << endl;
   ofl.close();
   }
*****/
return skeyOut;
}  

/********************
validateSession

This routine validates a session.  If for any reason the session can't be
validated, it sends the user back to the login screen.  This routine outputs
its own content type header if it goes back to the login screen

uid: user id
skey: session key to be matches

Returns: void
*********************/

void validateSession(const string &uid, const string &skey)
{ ifstream ifl;
  tstamp ts,    // --- CURRENT TIMESTAMP
         fts;   // --- FILED TIMESTAMP
         
 string pfname = PATH,
        fskey,          // --- SKEY FROM FILE
        toutStr;        // --- STRING FORM OF TIMEOUT

// *** GET CURRENT LOCAL TIME STAMP
ts.setCurrentDateTimeStamp(1);        
       
// *** READ IN VALUES
pfname += uid + ".ses";
ifl.open(pfname.c_str());
if (!ifl) // --- ERROR OPENING FILE - YEP, THIS IS BAD
   {
   backToLogin("Expired Session... Please login again...");
   exit(0);
   }
   
getline(ifl, fskey);
getline(ifl, toutStr);
ifl.close();
fts.stringToStamp(toutStr);
   
// *** CHECK VALUES, TO LOGIN IF MISMATCH, OTHERWISE RETURN TO CALLING ROUTINE
if ((fskey != skey) || (ts > fts))
   {
   backToLogin("Expired Session... Please login again...");
   exit(0);
   }
}

/********************
menuScreen

Output the menu screen with the userid and session key in place.  This outputs
its own content type header.

uid: user id
skey: session key

Returns: void
*********************/

void menuScreen(const string &uid, const string &skey)
{
cout << "Content-Type: text/html\n\n";

cout << "<html><head><LINK REL=StyleSheet HREF='../style.css' TYPE='text/css'><title>Menu Page</title></head>\n";
  
cout << "<body class='twoColElsLtHdr'>\n"
	 << "<div id='container'>\n"
		"<div id='header'><h1><center>The Locker Room</center></h1>\n"
		"<!-- end #header -->\n"
		"</div>\n";
cout << "<div id='sidebar1'>\n"
	 <<	"<center>\n"
	 << "<a href=\"viewWhiteboardDb.cgi?uid="
     << uid << "&skey=" << skey <<"&index=-1""\"> Employees </a><br /><br />\n"
	 << "<a href=\"postOnWhiteboardDb.cgi?uid=" 
	 << uid << "&skey=" << skey << "&index=-1&comment=-1""\">" "Start Meeting </a><br /><br />\n"
	 << "<a href=\"privateMessageDb.cgi?uid=" 
	 << uid << "&skey=" << skey << "&index=-1&comment=-1""\">" "Write a Private Message </a><br /><br />\n"
	  << "<a href=\"viewPrivateMessageDb.cgi?uid="
     << uid << "&skey=" << skey <<"&index=-1""\"> View Your Private Messages </a><br /><br />\n"
     << "<a href=\"setPermissionsDb.cgi?uid="
     << uid << "&skey=" << skey << "\"> Grant/Revoke Access for Whiteboard</a><br /><br />\n"
	 << "<a href=\"privatePermissionsDb.cgi?uid="
     << uid << "&skey=" << skey << "\"> Grant/Revoke Access for Private Messages</a><br /><br />\n"
	 << "<a href=\"roomboardPermissionsDb.cgi?uid="
     << uid << "&skey=" << skey << "\"> Grant/Revoke Access for Roomboard</a><br /><br />\n"
	 << "<a href=\"roomBoardDb.cgi?uid="
     << uid << "&skey=" << skey << "\"> View Your Room Board</a><br /><br />\n"
	 << "<a href=\"roomBoardWriteDb.cgi?uid="
     << uid << "&skey=" << skey << "\"> Write on Friends Room Board</a><br /><br />\n"
     << "<a href=\"logout.cgi?uid="
     << uid << "&skey=" << skey << "\"> Logout </a><br /><br />\n"
	 << "</center>\n"
	 << "</div>\n";
cout << "<div id='mainContent' align='center'>\n"
	 << "<center><br/>\n"
	 << "<br />\n"
     << "<br />\n"
     << "<br />\n"
	 << "Welcome to the Locker Room"
     << "<br />\n"
     << "<br />\n"
     << "<br />\n"
     << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "</center>\n"
	 << "</div>\n";
cout << "<center>\n"
	 << "<br class='clearfloat' /></center>\n"
     << "<div id='footer'>\n"
     << "<p><center> </center></p>\n"
     << "</div>\n"
	 << "</div>\n"
	<< "</body></html>" << endl;
}

void menuScreen2(const string &uid,const string &skey)
{
cout << "Content-Type: text/html\n\n";

cout << "<html><head><LINK REL=StyleSheet HREF='../style.css' TYPE='text/css'><title>Menu Page</title></head>\n";
  
cout << "<body class='twoColElsLtHdr'>\n"
	 << "<div id='container'>\n"
		"<div id='header'><h1><center>The Locker Room</center></h1>\n"
		"<!-- end #header -->\n"
		"</div>\n";
cout << "<div id='sidebar1'>\n"
	 <<	"<center>\n"
      << "<a href=\"viewWhiteboardDb.cgi?uid="
     << uid << "&index=-1" "\"> Read someone else's private whiteboard</a><br /><br />\n"
	 << "</center>\n"
	 << "</div>\n";
cout << "<div id='mainContent' align='center'>\n"
	 << "<center><br/>\n"
	 << "<br />\n"
     << "<br />\n"
     << "<br />\n"
	 << "Welcome to the Locker Room"
     << "<br />\n"
     << "<br />\n"
     << "<br />\n"
     << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "<br />\n"
	 << "</center>\n"
	 << "</div>\n";
cout << "<center>\n"
	 << "<br class='clearfloat' /></center>\n"
     << "<div id='footer'>\n"
     << "<p><center> </center></p>\n"
     << "</div>\n"
	 << "</div>\n"
	<< "</body></html>" << endl;
}