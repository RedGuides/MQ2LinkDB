// MQ2LinkDB
//
// Personal link database
//
// Usage: /link               - Display statistics
//        /link /import       - Import a items.txt file from items.sodeq.org
//                              into an updated MQ2LinkDB.txt file.
//        /link /max n        - Set number of maximum displayed results
//                              (default 10)
//        /link /scan on|off  - Turn on and off scanning incoming chat
//        /link /click on|off -
//        /link search        - Find items containing search string
//
// Incoming chat is scanned for links if specified, and the database is
// checked for this item. If it's not in database, it will be added.
//
// The TLO LinkDB is also added by this plugin. The LinkDB TLO supports
// a simple lookup for items by name and returns the item's link text.
// Note that since MQ2ChatWnd strips out stuff, you can't click links in
// there, so you'll have to stick the output in a variable then use
// a macro to control where you want it to go.
//
// The TLO supports substring matches and exact matches. If you pass
// =Item Name to the TLO, it will do an exact match. If you just do
// Item Name, then it will use a prefix match. If there are multiple
// items (i.e. multiple items with the same name in an exact, or
// multiple items with the same prefix in a non-exact), then the TLO
// will return the first match and you will have no idea there were
// multiple results.
//
// Example TLO usage:
// /declare BABYLINK string outer
// /varset BABYLINK ${LinkDB[=Baby Joseph Sayer]}
//
// /shout OMG I'm a dork! I have ${LinkDB[=Baby Joseph Sayer]} in my pack. Ha!
//
// If the item is not found, the TLO returns an empty string, so you probably don't
// want to be directly shouting about Baby Joseph Sayer in your backpack.
// If you do and misspell his name, you will end up shouting about an empty string
// which isn't recommended.

#include <mq/Plugin.h>
#include <cpr/cpr.h>
#include "MQ2LinkDB.h"
#include "sqlite3.h"
#include "SODEQItemConverters.h"

PreSetup("MQ2LinkDB");
PLUGIN_VERSION(5.0);

#define MY_STRING "MQ2LinkDB \ar5.0"

static sqlite3* s_linkDB = nullptr;
static std::thread s_importThread;
static std::thread s_downloadThread;
static std::atomic<bool> s_importThreadDone = true;
static std::atomic<bool> s_downloadThreadDone = true;


// Keep the last 10 results we've done and then cycle through.
// When I just kept the last one, doing two ${LinkDB[]} calls in one
// macro link crashed EQ. Well now you can do 10 on one line. If that's
// not enough, increase the LAST_RESULT_CACHE_SIZE.

constexpr int LAST_RESULT_CACHE_SIZE = 10;
static char g_tloLastResult[LAST_RESULT_CACHE_SIZE][256];
static int g_iLastResultPosition = 0;

static bool bQuietMode = true;               // Display debug chatter?
static int iAddedThisSession = 0;            // How many new links found since startup
static unsigned int iMaxResults = 10;        // Display at most this many results
static int iFindItemID = 0;                  // Item ID to /link
static bool bScanChat = true;                // Scan incoming chat for links
static bool bClickLinks = false;             // click on link generated?
static bool bSeparateDatabases = false;      // should the database be suffixed with the build (MQ2LinkDB_live.txt)

static std::string downloadURL; // can be updated in the ini

char szLinkDBFileName[MAX_PATH] = { 0 };
int linksInDBCount = 0;

static int strcnt(const char* buffer, char ch)
{
	int count = 0;
	for (int i = 0; buffer[i] != 0; ++i)
	{
		if (buffer[i] == ch)
			count++;
	}
	return count;
}

bool dataLinkDB(const char* szIndex, MQTypeVar& Ret)
{
	if (!szIndex[0])
	{
		return false;
	}

	iFindItemID = 0;
	int lastResultPosition = g_iLastResultPosition;

	const char* pItemName = szIndex;
	bool bExact = pItemName[0] == '=' && ++pItemName;

	std::vector<SearchResult> results = SearchLinkDB(pItemName, bExact, 1);

	if (!results.empty())
	{
		strcpy_s(g_tloLastResult[lastResultPosition], results[0].line.c_str());
		g_iLastResultPosition = (g_iLastResultPosition + 1) & LAST_RESULT_CACHE_SIZE;
	}
	else
	{
		g_tloLastResult[lastResultPosition][0] = 0;
	}

	Ret.Ptr = g_tloLastResult[lastResultPosition];
	Ret.Type = datatypes::pStringType;
	return true;
}

static void CloseDB()
{
	if (s_linkDB)
	{
		sqlite3_close(s_linkDB);
		s_linkDB = nullptr;
	}
}

static bool OpenDB()
{
	if (s_linkDB)
	{
		CloseDB();
	}

	if (sqlite3_open_v2(szLinkDBFileName, &s_linkDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL, nullptr) != SQLITE_OK)
	{
		WriteChatf("\arMQ2LinkDB: Error opening MQ2LinkDB database: %s", sqlite3_errmsg(s_linkDB));
		sqlite3_close(s_linkDB);
		s_linkDB = nullptr;
	}
	else
	{
		char* err_msg = nullptr;
		if (sqlite3_exec(s_linkDB, SODEQItemConverter315::getSQLCreateStmt().c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
		{
			WriteChatf("\arMQ2LinkDB: Error creating tables table: %s", err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(s_linkDB);
			s_linkDB = nullptr;
		}
	}

	queryLinkCount();

	return s_linkDB != nullptr;
}

static void SaveSettings()
{
	WritePrivateProfileInt("Settings", "MaxResults", static_cast<int>(iMaxResults), INIFileName);
	WritePrivateProfileBool("Settings", "ScanChat", bScanChat, INIFileName);
	WritePrivateProfileBool("Settings", "ClickLinks", bClickLinks, INIFileName);
	WritePrivateProfileBool("Settings", "SeparateDatabases", bSeparateDatabases, INIFileName);
	WritePrivateProfileString("Updater", "URL", downloadURL.c_str(), INIFileName);
}

static void LoadSettings()
{
	bSeparateDatabases = GetPrivateProfileBool("Settings", "SeparateDatabases", bSeparateDatabases, INIFileName);
	if (bSeparateDatabases)
	{
		sprintf_s(szLinkDBFileName, "%s\\MQ2LinkDB_%s.db", gPathResources, GetBuildTargetName(static_cast<BuildTarget>(gBuild)));
	}
	else
	{
		sprintf_s(szLinkDBFileName, "%s\\MQ2LinkDB.db", gPathResources);
	}

	iMaxResults = GetPrivateProfileInt("Settings", "MaxResults", 10, INIFileName);
	if (iMaxResults < 1) iMaxResults = 1;

	bScanChat = GetPrivateProfileBool("Settings", "ScanChat", true, INIFileName);
	bClickLinks = GetPrivateProfileBool("Settings", "ClickLinks", false, INIFileName);
	downloadURL = GetPrivateProfileString("Updater", "URL", "https://www.redguides.com/community/resources/items-txt-used-for-mq2linkdb.1720/download", INIFileName);

	OpenDB();
}

static void queryLinkCount()
{
	sqlite3_stmt* stmt;

	const std::string query("SELECT count(item_id) FROM item_links;");

	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link: %s", sqlite3_errmsg(s_linkDB));
		return;
	}
	else
	{
		if (sqlite3_step(stmt) == SQLITE_ROW)
		{
			linksInDBCount = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
}

static std::vector<std::string> queryLinkByItemID(const int itemID)
{
	sqlite3_stmt* stmt;

	std::vector<std::string> res;

	const std::string query("SELECT link FROM item_links WHERE item_id=?;");
	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link: %s", sqlite3_errmsg(s_linkDB));
	}
	else
	{
		sqlite3_bind_int(stmt, 1, itemID);

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			res.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
		}

		sqlite3_finalize(stmt);
	}

	return res;
}

static int ItemID(std::string_view link)
{
	ItemLinkInfo linkInfo;
	if (!ParseItemLink(link, linkInfo))
		return 0;

	return linkInfo.itemID;
}

static std::string_view ItemName(std::string_view link)
{
	ItemLinkInfo linkInfo;
	if (!ParseItemLink(link, linkInfo))
		return "";

	return linkInfo.itemName;
}

static bool FindLink(std::string_view link)
{
	ItemLinkInfo findLink;
	if (!ParseItemLink(link, findLink))
		return false;

	if (!s_linkDB) 
		return false;

	std::vector<std::string> links = queryLinkByItemID(findLink.itemID);

	if( links.empty() )
		return false;

	bool replaceAugLink = false;

	for (auto& link_check : links)
	{
		ItemLinkInfo linkInfo;
		if (ParseItemLink(link_check, linkInfo))
		{
			// Try to replace a socketed item with an unsocketed item, if possible.
			if (linkInfo.IsSocketed() && !findLink.IsSocketed())
			{
				replaceAugLink = true;
			}
			else
			{
				if (!bQuietMode)
				{
					WriteChatf("MQ2LinkDB: Saw link \ay%d\ax, but we already have it.", findLink.itemID);
				}
			}
		}
	}

	// The link in our db had an aug, but ours did not. we take this opportunity to replace it
	if (replaceAugLink)
	{
		if (!bQuietMode)
		{
			WriteChatf("MQ2LinkDB: Replacing auged link with un-auged link for item \ay%d\ax", findLink.itemID);
		}

		StoreLink(link);
	}

	return true;
}

static void StoreName(const std::string_view link)
{
	if (!s_linkDB)
	{
		OpenDB();

		if (!s_linkDB)
			return;
	}

	int iItemID = ItemID(link);
	std::string sItemName = std::string(ItemName(link));
	
	iAddedThisSession++;

	sqlite3_stmt* stmt;
	const std::string query("INSERT OR REPLACE INTO raw_item_data_315 (id, name) VALUES (?, ?);");
	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for raw_item_data_315 insertion: %s", sqlite3_errmsg(s_linkDB));
		return;
	}

	sqlite3_bind_int(stmt, 1, iItemID);
	sqlite3_bind_text(stmt, 2, sItemName.c_str(), -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		WriteChatf("\arMQ2LinkDB: Error inserting into raw_item_data_315 table: %s", sqlite3_errmsg(s_linkDB));
		sqlite3_finalize(stmt);
		return;
	}

	sqlite3_finalize(stmt);

}

static void StoreLink(const std::string_view link)
{
	if (!s_linkDB)
	{
		OpenDB();

		if (!s_linkDB)
			return;
	}

	int iItemID = ItemID(link);
	iAddedThisSession++;
	std::string sTrimmedLink = std::string(link);

	size_t secondPos = sTrimmedLink.find('\x12', 1);
	if (secondPos != std::string::npos)
	{
		sTrimmedLink = sTrimmedLink.substr(0, secondPos + 1);
	}

	sqlite3_stmt* stmt;
	const std::string query("INSERT OR REPLACE INTO item_links (item_id, link) VALUES (?, ?);");
	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link insertion: %s", sqlite3_errmsg(s_linkDB));
		return;
	}

	sqlite3_bind_int(stmt, 1, iItemID);
	sqlite3_bind_text(stmt, 2, sTrimmedLink.c_str(), -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		WriteChatf("\arMQ2LinkDB: Error inserting into item_link table: %s", sqlite3_errmsg(s_linkDB));
		sqlite3_finalize(stmt);
		return;
	}

	sqlite3_finalize(stmt);
	StoreName(link);

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Stored link for item ID: \ay%d\ax %.*s (\ay%d\ax stored this session)", iItemID, link.length(), link.data(), iAddedThisSession);
	}

	queryLinkCount();
}

// Parse parameters. Return value is an item id to search for, or 0 if none.
static int ParseParameters(std::string_view paramString)
{
	bool bAnyParams = false;
	int searchItemID = 0;

	std::vector<std::string_view> params = split_view(paramString, ' ');
	size_t pos = 0;

	auto GetNextParam = [&](std::string_view& param) -> bool
	{
		param = std::string_view{};

		while (pos < params.size())
		{
			param = params[pos++];
			param = trim(param);
			if (!param.empty())
				break;
		}

		return !param.empty();
	};

	std::string_view param;
	while (GetNextParam(param))
	{
		if (ci_equals(param, "/quiet"))
		{
			bQuietMode = !bQuietMode;
			WriteChatf("MQ2LinkDB: Quiet mode \ay%s\ax", bQuietMode ? "on" : "off");

			bAnyParams = true;
		}
		else if (ci_equals(param, "/max"))
		{
			GetNextParam(param);

			iMaxResults = GetIntFromString(param, 0);
			if (iMaxResults > 0)
			{
				WriteChatf("MQ2LinkDB: Max results now \ay%d\ax", iMaxResults);
				SaveSettings();
			}

			bAnyParams = true;
		}
		else if (ci_equals(param, "/item"))
		{
			GetNextParam(param);

			searchItemID = GetIntFromString(param, 0);
			bAnyParams = true;
		}
		else if (ci_equals(param, "/click"))
		{
			GetNextParam( param);

			if (!param.empty())
			{
				bClickLinks = GetBoolFromString(param, false);
			}
			else
			{
				bClickLinks = !bClickLinks;
			}

			WriteChatf("MQ2LinkDB: Will%sauto-click exact match links it generates.", bClickLinks ? " " : " NOT ");
			SaveSettings();
			bAnyParams = true;
		}
		else if (ci_equals(param, "/separate"))
		{
			GetNextParam( param);

			if (!param.empty())
			{
				bSeparateDatabases = GetBoolFromString(param, false);
			}
			else
			{
				bSeparateDatabases = !bSeparateDatabases;
			}

			WriteChatf("MQ2LinkDB: Will%ssave to separate databases based on the MQ Build Target (currently: %s).", bSeparateDatabases ? " " : " NOT ", GetBuildTargetName(static_cast<BuildTarget>(gBuild)));
			SaveSettings();
			LoadSettings();
			bAnyParams = true;
		}
		else if (ci_equals(param, "/scan"))
		{
			GetNextParam(param);

			if (!param.empty())
			{
				bScanChat = GetBoolFromString(param, false);
			}
			else
			{
				bScanChat = !bScanChat;
			}

			WriteChatf("MQ2LinkDB: Will%sscan incoming chat for item links.", bScanChat ? " " : " NOT ");
			SaveSettings();
			bAnyParams = true;
		}
		else if (ci_equals(param, "/import"))
		{
			ConvertItemsDotTxt();
			bAnyParams = true;
		}
		else if (ci_equals(param, "/query"))
		{
			const std::string queryString(params[pos].data());

			if (queryString.find(";") != std::string::npos)
			{
				WriteChatf("MQ2LinkDB: \arQuery failed.\aw '\am;\aw' is not needed in your query");

			}
			else
			{
				QueryLinkDB(queryString);
			}

			bAnyParams = true;
		}
		else if (ci_equals(param, "/update"))
		{
			DownloadLatestItemsTxt(downloadURL);
			bAnyParams = true;
		}
	}

	if (!bAnyParams)
	{
		WriteChatf("%s", MY_STRING);
		WriteChatf("MQ2LinkDB: Syntax: \ay/link [/max #] [/scan on|off] [/click on|off] [/separate on|off] [/import] [/item #] [search string] [/query <query>] [/update]\ax");

		WriteChatf("MQ2LinkDB: \ay%d\ax links in database, \ay%d\ax links added this session", linksInDBCount, iAddedThisSession);

		WriteChatf("MQ2LinkDB: Max Results \ay%d\ax", iMaxResults);

		if (bScanChat)
		{
			WriteChatf("MQ2LinkDB: Scanning incoming chat for item links");
		}
		else
		{
			WriteChatf("MQ2LinkDB: Not scanning incoming chat");
		}

		if (bClickLinks)
		{
			WriteChatf("MQ2LinkDB: Auto-clicking links on exact matches");
		}
		else
		{
			WriteChatf("MQ2LinkDB: Not auto-clicking links on exact matches");
		}
	}

	return searchItemID;
}



// This routine will send a link click to EQ to retrieve the item data
void ShowItem(std::string_view link)
{
	DebugSpew("MQ2LinkDB::ClickLink(%.*s)", link.length(), link.data());

	TextTagInfo tagInfo = ExtractLink(link);
	if (tagInfo.tagCode == ETAG_ITEM)
	{
		// Extract the link code from the link. Two characters after the start (\x12 + tag)
		// runs right up to the text of the link. Don't include the text.
		std::string_view linkData = tagInfo.link.substr(2);
		linkData = linkData.substr(0, tagInfo.text.data() - linkData.data());

		char szCommand[MAX_STRING] = { 0 };
		sprintf_s(szCommand, "/notify ChatWindow CW_ChatOutput link %.*s", (int)linkData.length(), linkData.data());

		EzCommand(szCommand);
	}
}

static std::vector<SearchResult> QueryLinkDB(const std::string& queryText)
{
	std::vector<SearchResult> results;
	sqlite3_stmt* stmt;

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Running DB Query '\ay%s\ax'...", queryText.c_str());
	}

	std::string query("SELECT links.link FROM item_links AS links, raw_item_data_315 AS items WHERE items.id=links.item_id AND ");
	query += queryText;

	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query search by item name: %s", sqlite3_errmsg(s_linkDB));
		return results;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::string_view line{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) };
		TextTagInfo info = ExtractLink(line);

		size_t start = info.text.data() - line.data();
		results.emplace_back(std::string(line), start, info.text.length());
	}

	sqlite3_finalize(stmt);

	for (auto& r : results)
	{
		bool bSocketed = false;

		ItemLinkInfo linkInfo;
		if (ParseItemLink(r.line, linkInfo))
		{
			bSocketed = linkInfo.IsSocketed();
		}

		WriteChatf("%s%s", r.line.c_str(), bSocketed ? " (Augmented)":"");
	}

	return results;
}

static std::vector<SearchResult> SearchLinkDB(const std::string& searchText, bool bExact, int limit /* = -1 no limit */)
{
	std::vector<SearchResult> results;
	sqlite3_stmt* stmt;

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Searching for '\ay%.*s\ax'...", searchText.length(), searchText.data());
	}

	std::string query("SELECT links.link FROM item_links AS links, raw_item_data_315 AS items WHERE items.id=links.item_id AND items.name");
	query += bExact ? "=?" : " LIKE ?";	
	query += " ORDER BY items.name ASC LIMIT ?;";

	if (sqlite3_prepare_v2(s_linkDB, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query search by item name: %s", sqlite3_errmsg(s_linkDB));
	}
	else
	{
		std::string searchString(searchText);
		if (!bExact)
		{
			searchString = "%" + searchText + "%";
		}

		sqlite3_bind_text(stmt, 1, searchString.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt, 2, limit);

		while ( sqlite3_step(stmt) == SQLITE_ROW )
		{
			std::string_view line{ reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) };
			TextTagInfo info = ExtractLink(line);

			size_t start = info.text.data() - line.data();
			results.emplace_back(std::string(line), start, info.text.length());
		}

		sqlite3_finalize(stmt);
	}

	return results;
}

static void CommandLink(SPAWNINFO* pChar, const char* szLine_)
{
	// We copy because DoParameters will mutate the string.
	std::string line{ szLine_ };

	iFindItemID = 0;
	bRunNextCommand = true;

	if (line.length() < 3)
	{
		ParseParameters(line);
		return;
	}

	if (line[0] == '/' || line[0] == '-')
	{
		iFindItemID = ParseParameters(line);
		if (!iFindItemID)
			return;
	}

	std::vector<SearchResult> results = SearchLinkDB(line, false);
	bool bForcedExtra = false;

	if (!results.empty())
	{
		// Sort the list
		std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b)
		{
			std::string_view a_view = std::string_view{ a.line }.substr(a.textPos, a.textLength);
			std::string_view b_view = std::string_view{ b.line }.substr(b.textPos, b.textLength);
			return a_view < b_view;
		});

		// Show list
		for (unsigned int i = 0; i < results.size(); ++i)
		{
			bool bShow = i < iMaxResults;

			char szTemp[256] = { 0 };
			strcpy_s(szTemp, results[i].line.c_str());

			ItemLinkInfo linkInfo;
			if (ParseItemLink(results[i].line, linkInfo))
			{
				if (linkInfo.IsSocketed())
					strcat_s(szTemp, " (Augmented)");

				if (iFindItemID || ci_equals(linkInfo.itemName, line))
				{
					strcat_s(szTemp, " <Exact Match>");
					bShow = true;        // We display this result even if we've already shown iMaxResults count
					bForcedExtra = i >= iMaxResults;
					if (bClickLinks)
						ShowItem(results[i].line);
				}
			}

			if (bShow)
			{
				WriteChatf("%s", szTemp);
			}
		}
	}

	char szTemp3[128] = { 0 };
	char szTemp[128] = { 0 };
	sprintf_s(szTemp3, "MQ2LinkDB: Found \ay%d\ax items from database of \ay%d\ax total items", static_cast<int>(results.size()), linksInDBCount);
	sprintf_s(szTemp, "Found %d items from database of %d total items", static_cast<int>(results.size()), linksInDBCount);

	if (results.size() > iMaxResults)
	{
		char szTemp2[64];
		sprintf_s(szTemp2, " - \arList capped to \ay%d\ar results", iMaxResults);
		strcat_s(szTemp3, szTemp2);

		sprintf_s(szTemp2, " - List capped to %d results", iMaxResults);
		strcat_s(szTemp, szTemp2);

		if (bForcedExtra)
		{
			strcat_s(szTemp, " plus exact match");
			strcat_s(szTemp3, " plus exact match");
		}
	}

	if (!bQuietMode)
	{
		WriteChatf("%s", szTemp3);
	}

	WriteChatf("%s", szTemp);
}

// Called once, when the plugin is to initialize
PLUGIN_API void InitializePlugin()
{
	DebugSpewAlways("Initializing MQ2LinkDB");
	AddCommand("/link", CommandLink);
	AddMQ2Data("LinkDB", dataLinkDB);

	LoadSettings();
}

// Called once, when the plugin is to shutdown
PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("Shutting down MQ2LinkDB");
	RemoveCommand("/link");
	RemoveMQ2Data("LinkDB");

	if(s_importThread.joinable())
		s_importThread.join();

	if (s_downloadThread.joinable())
		s_downloadThread.join();


	CloseDB();
}

// This is called every time EQ shows a line of chat with CEverQuest::dsp_chat,
// but after MQ filters and chat events are taken care of.
PLUGIN_API DWORD OnIncomingChat(char* Line, DWORD Color)
{
	//"Soandso hit a venomshell pest for 106012 points of magic damage by \x1263^56723^'Claw of Ellarr\x12."
	if (bScanChat)
	{
		TextTagInfo tags[MAX_EXTRACT_LINKS];
		size_t count = ExtractLinks(Line, tags, MAX_EXTRACT_LINKS);

		for (size_t i = 0; i < count; ++i)
		{
			if (tags[i].tagCode == ETAG_ITEM)
			{
				if (!FindLink(tags[i].link))
				{
					StoreLink(tags[i].link);
				}
			}
		}
	}
	return 0;
}

std::unique_ptr<SODEQItemConverter> MakeItemConverter(const char* szLine)
{
	// quick sanity check on file
	int columnCount = strcnt(szLine, '|') + 1;

	if (columnCount == 295)
		return std::make_unique<SODEQItemConverter295>();

	if (columnCount == 315)
		return std::make_unique<SODEQItemConverter315>();

	return nullptr;
}

void ConvertItemsDotTxtWorker()
{
	s_importThreadDone = false;

	WriteChatf("MQ2LinkDB: Importing items.txt...");

	std::string line;

	const std::string filePath = fmt::format("{}\\items.txt", gPathResources);
	std::ifstream itemsFile(filePath);
	if (!itemsFile)
	{
		WriteChatf("MQ2LinkDB: \arSource file not found (items.txt)");
		DebugSpewAlways("MQ2LinkDB: \arSource file not found (items.txt)");
		return;
	}

	// Get the first line
	if (!std::getline(itemsFile, line))
	{
		WriteChatf("MQ2LinkDB: \arInvalid items.txt file: failed to read header.");
		return;
	}

	std::unique_ptr<SODEQItemConverter> converter = MakeItemConverter(line.c_str());
	if (!converter)
	{
		WriteChatf("MQ2LinkDB: \arCould not create item converter. The items.txt file is not in the right format.");
		DebugSpewAlways("MQ2LinkDB: \arCould not create item converter. The items.txt file is not in the right format.");
		return;
	}

	WriteChatf("MQ2LinkDB: Generating links...");
	int iCount = 0;

	char* err_msg = nullptr;
	if (sqlite3_exec(s_linkDB, "BEGIN IMMEDIATE TRANSACTION;", nullptr, nullptr, &err_msg) == SQLITE_OK)
	{
		while (std::getline(itemsFile, line))
		{
			iCount++;
			if (converter->LoadItemLine(line.c_str()))
			{
				converter->execAddItemToRawDB(s_linkDB);
				converter->execAddItemToLinkDB(s_linkDB);
			}
			else
			{
				WriteChatf("\amMQ2LinkDB: \arFailed to load item on line %d!", iCount);
			}

			if (iCount % 1000 == 0)
			{
				WriteChatf("\amMQ2LinkDB: \atProgress Update: \ay%d\at items converted.", iCount);
			}
		}

		if (sqlite3_exec(s_linkDB, "COMMIT;", nullptr, nullptr, &err_msg) == SQLITE_OK)
		{
			WriteChatf("\amMQ2LinkDB: \agComplete! \ay%d\at links generated", iCount);
			DebugSpewAlways("\amMQ2LinkDB: \agComplete! \ay%d\at links generated", iCount);

			queryLinkCount();
		}
		else
		{
			WriteChatf("\amMQ2LinkDB: \arFailed! Error: %s", err_msg);
			DebugSpewAlways("\amMQ2LinkDB: \arFailed! Error: %s", err_msg);
		}

	}

	s_importThreadDone = true;
}

static void ConvertItemsDotTxt()
{
	if (!s_importThreadDone)
	{
		DebugSpewAlways("\amMQ2LinkDB:\ayAn import is already running!");
	}
	else
	{
		if (s_importThread.joinable())
			s_importThread.join();

		s_importThread = std::thread{ ConvertItemsDotTxtWorker };
	}
}

static void DownloadLatestItemsTxtWorker(const std::string& itemsURL)
{
	WriteChatf("MQ2LinkDB: \ayDownloading latest items.txt from: \am%s", itemsURL.c_str());

	cpr::Response r = cpr::Get(cpr::Url{itemsURL});
	if (r.status_code == 200)
	{
		const std::string filePath = fmt::format("{}\\items.txt", gPathResources);
		std::ofstream fileStream(filePath, std::ios::binary);

		if (!fileStream)
		{
			WriteChatf("MQ2LinkDB: \arUnable to write to items.txt");
			DebugSpewAlways("MQ2LinkDB: \arUnable to write to items.txt");
			return;
		}

		fileStream.write(r.text.c_str(), static_cast<int>(r.text.size()));

		WriteChatf("MQ2LinkDB: \agLatest items.txt downloaded! Running import next.");
	}
	else
	{
		WriteChatf("Failed to download (%s) with code(%d)", itemsURL.c_str(), r.status_code);
	}

	ConvertItemsDotTxt();

	s_downloadThreadDone = true;
}

static void DownloadLatestItemsTxt(const std::string& itemsURL)
{
	if (!s_downloadThreadDone)
	{
		DebugSpewAlways("\amMQ2LinkDB:\ayA download is already running!");
	}
	else
	{
		if (s_downloadThread.joinable())
			s_downloadThread.join();

		s_downloadThread = std::thread{ DownloadLatestItemsTxtWorker, itemsURL };
	}
}