// MQ2LinkDB
//
// Version 4.0 - 07 Feb 2014 - Originally by Ziggy,
//  then updated for DoD, PoR, SS, BS, modifications by SwiftyMUSE
//  updated for CotF
//  rewritten for mqnext by brainiac
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
//
//
// Changes:
// 5.0  Changed link db from a file to SQLite3 DB - Derple
// 4.0  Rewritten for mqnext using new item link parsing apis provided by EQLib and
//      string_view. Now is able to parse multiple links on the same message. Parts of
//      defunct linkbot removed.
// 3.4  Fixed a crash when spell links was parsed (Eqmule Jan 09 2020)
// 3.3  Updated to not require an item on cursor to do / link / import and to not poof
//      the item used as the template. (Eqmule Jan 08 2020)
// 3.2  Updated for new link format for TBL.
// 3.1  Updated for new link format for TBL.
// 3.0  Added string safety. (Eqmule 07-22-2016)
// 2.4  Updated to include new sodeq db dump format as the input based on old
//      13th-floor database.
// 2.3  Fixed exact search link clicking. Replaced entire import function to use
//      a dump from Lucy. 13th-floor is no longer updating their file so it is of no
//      real use to us anymore. I will keep an updated MQ2LinkDB.txt file on the MQ2
//      site for everyones use. The file will be updated on a monthly basis, at best.
// 2.2  Updated for CotF release. Linkbot ability is not working
// 2.1  Updated to fix charm links.  Added all the new fields as defined on 13-floor and
//      corrected a long standing issue with an escaped delimiter in the names of 3 items.
//      You NO LONGER have to remove the left and right hand entries, they are created
//      as links correctly.
// 2.0  This version, with linkbot ability, must remain in the VIP forums.
//      Added linkbot functionality using an authorization list.  Will automatically
//      click an exact match link.  Added the ability to retrieve a link based on
//      the item id using /item #.
//
//      Linkbot called with tells using the !link command.  It will respond to the
//      caller with the list of links as if you entered a /link command directly.
// 1.7  Added simple TLO for accessing links from item names.
// 1.6  Updated for Broken Seas item format changes. Thanks to ksmith and
//      Nilwean at EQItems. See
//      http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=229
// 1.5  Updated for 12/5 item format changes. Thanks to Nilwean and ksmith
//      at EQItems. See
//      http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=215
// 1.4  Updated for SS expansion. Thanks to ksmith at EQItems. See
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=202
// 1.3  Updated for PoR expansion. Thanks to ksmith at EQItems. See
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=170
//
// 1.2  Added ScanChat ini setting to toggle whether to snarf links from
//          seen chatlines. Defaults to on to simulate current behavior.
//          Also updated for EQItems fixes to their export which was missing
//          a field.
//
// 1.1  Updated to reflect new link format. Thanks to ksmith and Nilwean
//          and topside from
//          http://eqitems.13th-floor.org/phpBB2/viewtopic.php?t=145
//
// 1.06 Alpha sorts the list
//      Makes sure that if an exact match is found, then it's displayed, even
//      if we've already displayed max results
//      Only searches the name for the item name, previously searched the
//      whole link (eg: EB41 would have matched 2 items from their hash key)
//
// 1.05 Fixed the really stupid mistake with item bitfield. And found out
//      a new equation:
//      Moved items.txt import into plugin (You can still use it external if
//      you want)
//
// 1.04 Added Max item list so we don't get too spammed by results.
//
// 1.03 Added some more clues for debugging purposes (do: /link /quiet to show)
//
// 1.02 Fixed file locking fudge up. Should now add items to database properly.
//
// 1.01 Added item index so we know already which items the DB has to save a
//      bunch of time with checking when adding new items
//
// TODO:
// Auto update from 13th floor website
//

#include <mq/Plugin.h>
#include "MQ2LinkDB.h"
#include "sqlite3.h"
#include "SODEQItemConverters.h"

PreSetup("MQ2LinkDB");
PLUGIN_VERSION(5.0);

#define MY_STRING "MQ2LinkDB \ar5.0"

sqlite3* m_db = nullptr;
std::thread _workerThread;
std::atomic<bool> _threadDone;


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
static int iVerifyCount;                     // Starting line # to generate 100 links for verification
static bool bScanChat = true;                // Scan incoming chat for links
static bool bClickLinks = false;             // click on link generated?
static bool bReadFileInFind = false;         // should we reload the file when looking for new links?
static bool bSeparateDatabases = false;      // should the database be suffixed with the build (MQ2LinkDB_live.txt)

static char szLink[MAX_STRING / 4] = { 0 };
static int iCurrentID = 0;
static int iNextID = 0;

char szLinkDBFileName[MAX_PATH] = { 0 };
int linksInDBCount = 0;

class MQ2LinkType : public MQ2Type
{
public:
	enum class LinkMembers
	{
		Link = 1,
		CurrentID,
		NextID,
	};

	MQ2LinkType() : MQ2Type("linkdb")
	{
		ScopedTypeMember(LinkMembers, Link);
		ScopedTypeMember(LinkMembers, CurrentID);
		ScopedTypeMember(LinkMembers, NextID);
	}

	virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest) override
	{
		MQTypeMember* pMember = MQ2LinkType::FindMember(Member);
		if (!pMember)
			return false;

		switch (static_cast<LinkMembers>(pMember->ID))
		{
		case LinkMembers::Link:
			strcpy_s(DataTypeTemp, szLink);
			Dest.Ptr = &DataTypeTemp[0];
			Dest.Type = datatypes::pStringType;
			return true;
		case LinkMembers::CurrentID:
			Dest.DWord = iCurrentID;
			Dest.Type = datatypes::pIntType;
			return true;
		case LinkMembers::NextID:
			Dest.DWord = iNextID;
			Dest.Type = datatypes::pIntType;
			return true;
		}
		return false;
	}

	bool ToString(MQVarPtr VarPtr, char* Destination) override
	{
		strcpy_s(Destination, MAX_STRING, szLink);
		return true;
	}
};
MQ2LinkType* pLinkType = nullptr;

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
		Ret.DWord = 0;
		Ret.Type = pLinkType;
		return true;
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
	if (m_db)
	{
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}

static bool OpenDB()
{
	if (m_db)
	{
		CloseDB();
	}

	if (sqlite3_open_v2(szLinkDBFileName, &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_WAL, nullptr) != SQLITE_OK)
	{
		WriteChatf("\arMQ2LinkDB: Error opening console buffer database: %s", sqlite3_errmsg(m_db));
		sqlite3_close(m_db);
		m_db = nullptr;
	}
	else
	{
		char* err_msg = nullptr;
		if (sqlite3_exec(m_db, SODEQItemConverter315::getSQLCreateStmt().c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
		{
			WriteChatf("\arMQ2LinkDB: Error creating tables table: %s", err_msg);
			sqlite3_free(err_msg);
			sqlite3_close(m_db);
			m_db = nullptr;
		}
	}

	queryLinkCount();

	return m_db != nullptr;
}

static void SaveSettings()
{
	WritePrivateProfileInt("Settings", "MaxResults", iMaxResults, INIFileName);
	WritePrivateProfileBool("Settings", "ScanChat", bScanChat, INIFileName);
	WritePrivateProfileBool("Settings", "ClickLinks", bClickLinks, INIFileName);
	WritePrivateProfileBool("Settings", "SeparateDatabases", bSeparateDatabases, INIFileName);
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

	OpenDB();
}

static void queryLinkCount()
{
	sqlite3_stmt* stmt;

	std::vector<std::string> res;

	std::string query("SELECT count(item_id) FROM item_links;");

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link: %s", sqlite3_errmsg(m_db));
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

static std::vector<std::string> queryLinkByItemID(int itemID)
{
	sqlite3_stmt* stmt;

	std::vector<std::string> res;

	std::string query("SELECT link FROM item_links WHERE item_id=?;");
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link: %s", sqlite3_errmsg(m_db));
		return res;
	}
	else
	{
		sqlite3_bind_int(stmt, 1, itemID);

		char cBuffer[MAX_STRING / 4] = { 0 };
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			sprintf_s(cBuffer, "%s", sqlite3_column_text(stmt, 0));
			res.push_back(std::string(cBuffer));
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

static bool FindLink(std::string_view link)
{
	ItemLinkInfo findLink;
	if (!ParseItemLink(link, findLink))
		return false;

	if (!m_db) return false;

	std::vector<std::string> links = queryLinkByItemID(findLink.itemID);

	if( links.size() == 0)
		return false;

	bool replaceAugLink = false;

	for (auto& link : links)
	{
		ItemLinkInfo linkInfo;
		ParseItemLink(link, linkInfo);

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

static void StoreLink(std::string_view link)
{
	if (!m_db)
	{
		OpenDB();

		if (!m_db)
			return;
	}

	int iItemID = ItemID(link);
	iAddedThisSession++;

	sqlite3_stmt* stmt;
	std::string query("INSERT OR REPLACE INTO item_links (item_id, link) VALUES (?, ?);");
	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query for item_link insertion: %s", sqlite3_errmsg(m_db));
		return;
	}
	else
	{
		sqlite3_bind_int(stmt, 1, iItemID);
		sqlite3_bind_blob(stmt, 2, link.data(), (int)link.length(), SQLITE_STATIC);

		if (sqlite3_step(stmt) != SQLITE_DONE)
		{
			WriteChatf("\arMQ2LinkDB: Error inserting into item_link table: %s", sqlite3_errmsg(m_db));
			return;
		}

		sqlite3_finalize(stmt);
	}

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
		else if (ci_equals(param, "/import"))
		{
			ConvertItemsDotTxt();
			bAnyParams = true;
		}
		else if (ci_equals(param, "/query"))
		{
			std::string queryString(params[pos].data());
			QueryLinkDB(queryString);
			bAnyParams = true;
		}
	}

	if (!bAnyParams)
	{
		WriteChatf("%s", MY_STRING);
		WriteChatf("MQ2LinkDB: Syntax: \ay/link [/max #] [/scan on|off] [/click on|off] [/separate on|off] [/import] [/item #] [search string]\ax");

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

static std::vector<SearchResult> QueryLinkDB(std::string& queryText)
{
	std::vector<SearchResult> results;
	sqlite3_stmt* stmt;

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Running DB Query '\ay%s\ax'...", queryText.c_str());
	}

	std::string query("SELECT links.link FROM item_links AS links, raw_item_data_315 AS items WHERE items.id=links.item_id AND ");
	query += queryText;

	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query search by item name: %s", sqlite3_errmsg(m_db));
		return results;
	}
	else
	{
		char cBuffer[MAX_STRING / 4] = { 0 };
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			sprintf_s(cBuffer, "%s", reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)));
			std::string_view line{ cBuffer };
			TextTagInfo info = ExtractLink(line);

			size_t start = info.text.data() - line.data();
			results.emplace_back(std::string(line), start, info.text.length());
		}

		sqlite3_finalize(stmt);
	}

	for (auto& r : results)
	{
		char szTemp[256] = { 0 };
		strcpy_s(szTemp, r.line.c_str());

		ItemLinkInfo linkInfo;
		if (ParseItemLink(r.line, linkInfo))
		{
			if (linkInfo.IsSocketed())
				strcat_s(szTemp, " (Augmented)");
		}

		WriteChatf("%s", szTemp);
	}

	return results;
}

static std::vector<SearchResult> SearchLinkDB(std::string_view searchText, bool bExact, int limit)
{
	std::vector<SearchResult> results;
	sqlite3_stmt* stmt;

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Searching for '\ay%.*s\ax'...", searchText.length(), searchText.data());
	}

	std::string query("SELECT links.link FROM item_links AS links, raw_item_data_315 AS items WHERE items.id=links.item_id AND items.name");
	query += bExact ? "=?  ORDER BY items.name ASC LIMIT " : " LIKE ? ORDER BY items.name ASC LIMIT ";	
	query += std::to_string(limit);
	query += ";";

	std::string wildCard;


	if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		WriteChatf("MQ2LinkDB: Error preparing query search by item name: %s", sqlite3_errmsg(m_db));
		return results;
	}
	else
	{
		if (bExact)
		{
			sqlite3_bind_text(stmt, 1, searchText.data(), -1, SQLITE_STATIC);
		}
		else
		{
			wildCard = "%";
			wildCard += searchText;
			wildCard += "%";

			sqlite3_bind_text(stmt, 1, wildCard.c_str(), -1, SQLITE_STATIC);
		}

		char cBuffer[MAX_STRING / 4] = { 0 };
		while ( sqlite3_step(stmt) == SQLITE_ROW )
		{
			sprintf_s(cBuffer, "%s", reinterpret_cast<const char*>(sqlite3_column_blob(stmt, 0)));
			std::string_view line{ cBuffer };
			TextTagInfo info = ExtractLink(line);

			size_t start = info.text.data() - line.data();
			results.emplace_back(std::string(line), start, info.text.length());
		}

		sqlite3_finalize(stmt);
	}

	return results;
}

static void CommandLink(SPAWNINFO* pChar, char* szLine_)
{
	// We copy because DoParameters will mutate the string.
	std::string_view line{ szLine_ };

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
		for (unsigned int i = 0; i < (int)results.size(); ++i)
		{
			bool bShow = i < (int)iMaxResults;

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
	sprintf_s(szTemp3, "MQ2LinkDB: Found \ay%d\ax items from database of \ay%d\ax total items", (int)results.size(), (int)linksInDBCount);
	sprintf_s(szTemp, "Found %d items from database of %d total items", (int)results.size(), (int)linksInDBCount);

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

	_threadDone = true;

	pLinkType = new MQ2LinkType;
	LoadSettings();
}

// Called once, when the plugin is to shutdown
PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("Shutting down MQ2LinkDB");
	RemoveCommand("/link");
	RemoveMQ2Data("LinkDB");

	if( _workerThread.joinable() )
		_workerThread.join();

	CloseDB();

	delete pLinkType;
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
	_threadDone = false;

	WriteChatf("MQ2LinkDB: Importing items.txt...");

	char szFilename[MAX_PATH];
	sprintf_s(szFilename, "%s\\items.txt", gPathResources);
	FILE* File = _fsopen(szFilename, "rt", _SH_DENYNO);
	if (!File)
	{
		WriteChatf("MQ2LinkDB: \arSource file not found (items.txt)");
		DebugSpewAlways("MQ2LinkDB: \arSource file not found (items.txt)");
		return;
	}

	char szLine[MAX_STRING * 2] = { 0 };

	// Get the first line
	if (fgets(szLine, MAX_STRING * 2, File) == nullptr)
	{
		WriteChatf("MQ2LinkDB: \arInvalid items.txt file: failed to read header.");
		fclose(File);
		return;
	}

	szLine[strlen(szLine) - 1] = '\0';
	std::unique_ptr<SODEQItemConverter> converter = MakeItemConverter(szLine);
	if (!converter)
	{
		WriteChatf("MQ2LinkDB: \arCould not create item converter. The items.txt file is not in the right format.");
		DebugSpewAlways("MQ2LinkDB: \arCould not create item converter. The items.txt file is not in the right format.");
		fclose(File);
		return;
	}

	WriteChatf("MQ2LinkDB: Generating links...");
	int iCount = 0;
	
	char* err_msg = nullptr;
	sqlite3_exec(m_db, "BEGIN IMMEDIATE TRANSACTION;", nullptr, nullptr, &err_msg);

	while (fgets(szLine, MAX_STRING * 2, File) != nullptr)
	{
		iCount++;
		if (converter->LoadItemLine(szLine))
		{
			converter->execAddItemToRawDB(m_db);
			converter->execAddItemToLinkDB(m_db);
		}
		else
		{
			WriteChatf("\amMQ2LinkDB: \arFailed to load item on line %d!", iCount);
		}

		if (iCount % 1000 == 0)
		{
			WriteChatf("\amMQ2LinkDB: \ay%d\at items converted.", iCount);
		}
	}

	sqlite3_exec(m_db, "COMMIT;", nullptr, nullptr, &err_msg);

	WriteChatf("\amMQ2LinkDB: \agComplete! \ay%d\at links generated", iCount);
	DebugSpewAlways("\amMQ2LinkDB: \agComplete! \ay%d\at links generated", iCount);

	fclose(File);

	_threadDone = true;

	queryLinkCount();
}

static void ConvertItemsDotTxt()
{
	if (!_threadDone)
	{
		DebugSpewAlways("\amMQ2LinkDB:\ayAn import is already running!");
	}
	else
	{
		if (_workerThread.joinable())
			_workerThread.join();

		_workerThread = std::thread{ ConvertItemsDotTxtWorker };
	}
}
