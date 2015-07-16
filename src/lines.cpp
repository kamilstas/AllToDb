/*
*       File: lines.cpp
*       Author: Kamil Stas ( kamil.stas@gmail.com )
*/

#include "lines.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
//#include <boost/tokenizer.hpp>
#include <algorithm>
#include <my_global.h>
#include <mysql.h>
#include "main.h"

using namespace std;

     Lines::Lines(){
	}
	        Lines::~Lines(){
		}

	void print_usr()
{
        cout << "dbuser: " <<  dbUser << "\ndbpass: *****" << "\ndbhost: " << dbHost << "\ndbname: " << dbName << "\nlogfile: " << log_file << endl;
}

///// Funkcia query
	
	int a = 0;
	void Lines::query_i(){

	// Getting all user parameters
	// Check if log file is possible to use for reading
	 ifstream myfile(log_file);
                if (!myfile.is_open()) { cout << "Log file not possible to read, or does not exist. Exitting ..." << endl; exit(0); }
	// Getting all other user parameters
	// Creating default DB to store log in and checking DB connection

	Lines::createLogDb (dbName, dbHost, dbUser, dbPass);
	
	// Starting working with Log lines and creating the Insert Sql Query
      string day;
      string month;
      string year;
      string req_meth;
      string http_code;


	string line;
	string ln;
	ofstream bulkfile;
        string bulkname="/tmp/bulk.txt";
	bulkfile.open ((bulkname), ios::app); 
//	bulkfile << "SET GLOBAL local_infile=1;\n";
	bulkfile << "USE "+dbName+";\n";
	bulkfile << "SET autocommit=0;";
	bulkfile << "\nINSERT INTO line (line,req_method,http_code,day,month,year) VALUES "; 
	if(myfile.is_open()){ 
	while(!myfile.eof() ){
            getline(myfile,line);
	Lines::ReplaceStringInPlace(line, "'", "\\'");	

	string aline = line;
	
if ( !line.empty()){ 
//	if ( !line.length() > 0 ) {	
        char *chline = &line[0u];
        Lines::parseReqMet(chline, day, month, year, req_meth, http_code);
}

	ln ="('"+aline+"','"+req_meth+"','"+http_code+"','"+day+"','"+month+"','"+year+"')";
	bulkfile << ln;

//	if ( !line.length() > 0 ) { break; }
	
// Counting lines, each bulk insert will consists of "mi" value of lines as maximum 
	a++;
	int mi = 200000;
	if ( line.empty()){ break; }	
	else if ( a % mi == 0 ){ 
	bulkfile << ";";
        bulkfile << "\nCOMMIT;";
	cout << a << " inserts created in bulk\n";
	bulkfile << "\nINSERT INTO line (line,req_method,http_code,day,month,year) VALUES ";
	}
	else
	{
	if ( !line.length() > 0 ) { cout << "00000000000000"; break;}
	bulkfile << ",";
	}	

	
	}
	bulkfile << ";";
	bulkfile << "\nCOMMIT;";
	
	
	bulkfile.close();
        myfile.close();
	
	}
	
	// Inserting to DB
	string sysInsrt ="mysql -u ";
	string sysInsrt1=" --password=\"";
	string sysInsrtH="\" -h ";
	string sysInsrt2=" -e ";
	string sysInsrt3="\"SOURCE "+bulkname+"\";";
	string sysIn = sysInsrt+dbUser+sysInsrt1+dbPass+sysInsrtH+dbHost+sysInsrt2+sysInsrt3;	
	cout << "Inserting to DB\n" ;
	system(sysIn.c_str());
	// Removing Bulk file with all inserts
	Lines::removeBulk(bulkname);	
}
		

///////////////////////mysql functions part


	// Function ff will execute sql query
	int Lines::ff(const char *la, MYSQL *conn)
	{
	char *quer = strdup(la);
	//mysql_options( conn, MYSQL_OPT_LOCAL_INFILE, 0 );
	mysql_query(conn, quer);
	result = mysql_store_result(conn);
  	mysql_free_result(result);
	}
	
	// Function for closing DB connection
	int Lines::ffClose(MYSQL *cl ){
	  mysql_close(conn);	
	}
	
	// function for opening and checking DB connection
	int Lines::ffOpen(const char *dBconn, const char *dBhost, const char *dBuser, const char *dBpass){
  	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, dBhost, dBuser, dBpass, NULL, 0, NULL, 0)){
      	printf("Conection error : %s\n", mysql_error(conn));
      	exit(1);
        }


}

// function for parsing the lines in boost library, but not ready yet - 2nd part of the task
void Lines::parse_line(string a){
 //       cout << "Parsovacia Funkcia ( separator - ciarka, alebo space ):" << endl;
        //string text = "token, test   string";
    char_separator<char> sep("[","]");
    tokenizer<char_separator<char> > tokens(a, sep);
    for (const string& t : tokens) {
        cout << t << endl;
    }

}

int Lines::parseReqMet(char * yy, string & day, string & month, string & year, string & req_meth, string & http_code){
	
	char * pch;
	pch = strtok (yy,"[");	
	pch = strtok (NULL,"/");

	day = pch;
	pch = strtok (NULL,"/");
	month = pch;	
	pch = strtok (NULL,":");
	year = pch;	

	pch = strtok (NULL," ");
	pch = strtok (NULL," ");
	pch = strtok (NULL," \"");
	
	req_meth= pch;

	pch = strtok (NULL," ");
	pch = strtok (NULL," ");
	pch = strtok (NULL," ");
	
	http_code=pch;
}


	// Function will just remove large bulk inserts file from temporary directory
	int Lines::removeBulk (string blk)
	{

  if( remove( blk.c_str()) != 0 )
    perror( "Error deleting bulk file" );
  else
    puts( "Bulk file successfully deleted at the end" );
  return 0;
	}

	// function which will get all user input parameters and will pass it to DB functions for perform queries
	int Lines::createLogDb (string dBa, string dbH, string dbU, string dbP)
{
        
	string dbName=dBa;
	const char *dBconn=dbName.c_str();
	
	string dbHost=dbH;
	const char *dBhost=dbHost.c_str();	
	
	string dbUser=dbU;
	const char *dBuser=dbUser.c_str();
	
	string dbPass=dbP;
	const char *dBpass=dbPass.c_str();	

	// sending params to DB function to check, open connection and insert default DB
	Lines::ffOpen(dBconn, dBhost, dBuser, dBpass);

	// creating SQL query for default DB
        string createDB="CREATE DATABASE "+dbName+";";
        const char *cDb = createDB.c_str();
        // sending db query
	Lines::ff(cDb, conn);

        string createTable="CREATE TABLE "+dbName+".line ( id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, line TEXT, reg_date TIMESTAMP,req_method varchar(7), http_code int, day int, month varchar(3), year int);";
        const char *cTb = createTable.c_str();
 
        // sendin db query
	Lines::ff(cTb, conn);

	// closing the db connection
        Lines::ffClose(conn);

}

	void Lines::ReplaceStringInPlace(string& subject, const string& search,
                          const string& replace) {
         size_t pos = 0;
    	 while((pos = subject.find(search, pos)) != string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}



