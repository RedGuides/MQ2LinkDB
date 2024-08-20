#pragma once

#include<string>
#include <mq/Plugin.h>
#include "sqlite3.h"

class MQ2LinkDBTables
{
public:
	static std::string getSQLCreateStmt();

	static int getDBVersion(sqlite3* db);
	static bool execUpgradeDB(sqlite3* db);
};