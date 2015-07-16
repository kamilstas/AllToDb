/*
*	File: recovery.cpp
*	Author: Kamil Stas ( kamil.stas@gmail.com )
*/

#include "recovery.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
#include <algorithm>
#include <my_global.h>
#include <mysql.h>
#include <stdio.h>
#include "main.h"

using namespace std;

		Recovery::Recovery(){
			}
                Recovery::~Recovery(){
			}
	
	       void print_usr_rev()
{
        cout << "dbuser: " <<  dbUser << "\ndbpass: *****" << "\ndbhost: " << dbHost << "\ndbname: " << dbName << "\nday: " << dDay << "\nmonth: " << dMonth << "\nyear: " << dYear << "\nhttp code: " << httpCode << "\nrequest method: " << reqMeth << endl;
	 
}


        void Recovery::query_s(){
	// getting input parameters
	// remove file if there is any with the same name
	remove(log_rec.c_str());	
	// read from DB and create a new log
        Recovery::readLogDb (dbName, dbHost, dbUser, dbPass);
	// check if new created reverse log exists
	Recovery::exist(log_rec);	
	
				}	



	// function for establish the connection with DB and for sql query
	int Recovery::ff(const char *la, MYSQL *conn)
	{
        char *quer = strdup(la);
	//mysql_options( conn, MYSQL_OPT_LOCAL_INFILE, 0 );
      	 mysql_query(conn, quer);
	 result = mysql_use_result(conn);
	//printf(result);
	// looks like writing stdout to log file is a bit faster but writing rows more exact
	//freopen ((log_rec.c_str()),"w",stdout);
	ofstream myfile;
	myfile.open (log_rec);

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)))
	{

	myfile << row[0] << endl;
	//printf("%s\n",row[0]);
	}
	//myfile << row[0];
	myfile.close();
	//fclose(stdout);
 	 mysql_free_result(result);
	return 0;	
	}
	// function for closing the connection
        int Recovery::ffClose(MYSQL *cl ){
  	mysql_close(conn);
	}

        int Recovery::ffOpen(const char *dBconn, const char *dBhost, const char *dBuser, const char *dBpass){
  	conn = mysql_init(NULL);
 	if (!mysql_real_connect(conn, dBhost, dBuser, dBpass, dBconn, 0, NULL, 0)){
	
      	printf("Conection error : %s\n", mysql_error(conn));
      	exit(1);
		}


	}


        int Recovery::readLogDb (string dBa, string dbH, string dbU, string dbP)
	{


        string dbName=dBa;
        const char *dBconn=dbName.c_str();
        
	string dbHost=dbH;
        const char *dBhost=dbHost.c_str();

        string dbUser=dbU;
        const char *dBuser=dbUser.c_str();

        string dbPass=dbP;
        const char *dBpass=dbPass.c_str();

        Recovery::ffOpen(dBconn, dBhost, dBuser, dBpass);

	string sDay;	
	string sMonth;
	string sYear;
	string sHttpCode;
	string sReqMeth;
	string selectDB;
	string dayAnd;	

	if (( dDay == "" )) { dDay = "%"; }
	if (( dMonth == "" )) { dMonth = "%"; }
	if (( dYear == "" )) { dYear = "%"; }
	if (( httpCode == "" )) { httpCode = "%"; }
	if (( reqMeth == "" )) { reqMeth = "%"; }
	selectDB = "SELECT line FROM `line` WHERE day like '"+dDay+"' and month like '"+dMonth+"' and year like '"+dYear+"' and http_code like '"+httpCode+"' and req_method like '"+reqMeth+"';";
	
        const char *sDb = selectDB.c_str();
        Recovery::ff(sDb, conn);

        Recovery::ffClose(conn);
	}

	inline bool Recovery::exist( const string& name )
	{

	ifstream file(name);
	if(!file)
		{
		cout << "Something went wrong, recovered log file does not exist!" ;
		return false;
		}
	else
		{
		
		cout << "Check your recovered log file " << log_rec << "" << endl;
		return true;
		}
	}




