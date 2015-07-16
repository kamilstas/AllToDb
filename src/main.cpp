#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include "lines.h"
#include <boost/tokenizer.hpp>
#include <my_global.h>
#include <mysql.h>
#include <algorithm>
#include "recovery.h"
#include <fstream>
#include "main.h"

using namespace std;

string dbUser;
string dbPass;
string dbHost;
string dbName;
string log_file;
string dDay;
string dMonth;
string dYear;
string httpCode;
string reqMeth;

void getParams( string u, string p, string h, string d );
void showHelp( char *s );
int main (int argc, char **argv)
{

    char *user = NULL;
    char *dbname = NULL;
    char *password = NULL;
    char *hostname = NULL;
    char *help = NULL;
    char *logfile = NULL;
    char *day = NULL;
    char *month = NULL;
    char *year = NULL;
    char *http_code = NULL;
    char *req_meth = NULL;    



    string reverse;
    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "p:d:u:H:l:D:M:Y:C:R:hvr")) != -1)
        switch (c)
        {
        case 'u':
            user = optarg;
            break;
        case 'd':
            dbname = optarg;    
            break;
        case 'p':
            password = optarg;
            break;
        case 'H':
            hostname = optarg;
	    break;
	case 'l':
	    logfile = optarg;
	    break;
	case 'D':
	    day = optarg;
	    break;
	case 'M':
	    month = optarg;
	    break;
	case 'Y':
	    year = optarg;
	    break;
	case 'C':
            http_code = optarg;
            break;
        case 'R':
            req_meth = optarg;
            break;
        case 'h':
	showHelp (argv[0]);
		exit(0);

	case 'v':
	cout << "Version 1.0" << endl;
		exit(0);	
	
	case 'r':
	reverse = "yes";	
		break;

        default:
        showHelp (argv[0]);   
	abort ();
        }

	if (( user == NULL ) || ( password == NULL ) || ( hostname == NULL ) || ( dbname == NULL ))
	{
		cout << "Some parameters missing: user, dbname, hostname, password or logfile in absolute path not entered" << endl;
		cout << "Try -h for help" << endl;
		cout << reverse;
		exit(0);

	}
	
	else if
		(( reverse == "yes" ))
	{
		cout << "Creating reverse log from DB" << endl;
	if (( req_meth == NULL ))
	{
		req_meth = "";
	}
	if (( http_code == NULL ))
	{
		http_code = "";
	}
	if (( day == NULL ))
	{
		day = "";
	}
	if (( month == NULL ))
	{
		month = "";
	}
	if (( year == NULL ))
	{
		year = "";
	}
		dbUser = user;
        	dbPass = password;
        	dbHost = hostname;
        	dbName = dbname;		
		dDay = day;
		dMonth = month;
		dYear = year;
		httpCode = http_code;		
		reqMeth = req_meth;
		print_usr_rev();
		Recovery ro;
        	ro.query_s();
	}

	else
	{

	dbUser = user;
	dbPass = password;
	dbHost = hostname;
	dbName = dbname;
		if (( logfile == NULL ))
		{
			cout << "Probabely you forgot to add -l with your log file in absolute path" << endl;
			exit(0);
		}
	cout << logfile;
	log_file = logfile;
        print_usr();
   // printf ("user = %s, dbname = %s, password = %s, hostname = %s\n", user, dbname, password, hostname);
	Lines lo;	
	lo.query_i();

 	}   

 //   for (index = optind; index < argc; index++)
 //       printf ("Non-option argument %s\n", argv[index]);
    return 0;
}

void showHelp( char *s )
{
  cout<<"Usage:   "<<s<<" [-option] [argument]"<<endl;
  cout<<"option:  "<<"-h  show help information"<<endl;
  cout<<"         "<<"-u  MySQL username"<<endl;
  cout<<"         "<<"-p  MySQL password"<<endl;
  cout<<"         "<<"-H  DB Host"<<endl;
  cout<<"         "<<"-d  Database name which binary will create to store your log"<<endl;
  cout<<"	 "<<"-r  Recover log from DB"<<endl;
  cout<<"	  "<<"Recovery Log Filter Parameters:"<<endl;
  cout<<"	  	  "<<"-D enter day in number ( 1-31 )"<<endl;
  cout<<"	  	  "<<"-M enter month as 3 chars ( Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec )"<<endl;
  cout<<"	  	  "<<"-Y enter year as 4 digits ( example: 2014, 2015 )"<<endl;
  cout<<"		  "<<"-C enter http code ( example: 200, 404 )"<< endl;
  cout<<"		  "<<"-R enter request method ( POST, GET )"<<endl;
  cout<<"         "<<"-v  show version infomation"<<endl;
  cout<<"         "<<"-l  enter logfile in absolute path"<<endl;
  cout<<"example: "<<s<<" -u user -d databasename -p pass -H localhost -l /tmp/access.log"<<endl;
  cout<<"example: "<<s<<" -r -u username -d databasename -p pass -H localhost -D 15 -M Sep -Y 2014 -C 200 -R GET"<<endl;
}


