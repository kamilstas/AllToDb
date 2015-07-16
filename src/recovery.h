#ifndef RECOVERY_H
#define RECOVERY_H
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <my_global.h>
#include <mysql.h>
#include "main.h"

using namespace std;

class Recovery{
public:
        Recovery();
        ~Recovery();
        void query_s();
        MYSQL *conn;
        MYSQL_RES *result;
        int ff(const char*arg, MYSQL *cn);
        int ffOpen(const char*arg, const char*arga, const char*argb, const char*argc);
        int ffClose(MYSQL *cl );
        int readLogDb(string, string, string, string);
	string log_rec ="/tmp/access.log.recovered";
	inline bool exist( const string& name );
};
#endif

