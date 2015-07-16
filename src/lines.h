#ifndef LINES_H
#define LINES_H
#include <iostream>
#include <fstream>
#include <string>
#include <boost/tokenizer.hpp>
#include <my_global.h>
#include <mysql.h>
#include <algorithm>
#include "main.h"

using namespace std;
using namespace boost;

class Lines{
public:
	Lines();
	~Lines();
	void numberoflines();
	int number_of_lines;
	void query_i();
	MYSQL *conn;
        MYSQL_RES *result;
	int ff(const char*arg, MYSQL *cn);
	int ffOpen(const char*arg, const char*arga, const char*argb, const char*argc);
	int ffClose(MYSQL *cl );
	void parse_line(string);
	int parseReqMet(char*arg, string &, string &, string &, string &, string &); 
//	int bulkInsert(string);
	int removeBulk(string);
	int createLogDb(string, string, string, string);
	void ReplaceStringInPlace(string& subject, const string& search, const string& replace);
};
#endif
