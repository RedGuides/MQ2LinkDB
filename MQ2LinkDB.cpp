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

#include <string>
#include <vector>

PreSetup("MQ2LinkDB");
PLUGIN_VERSION(4.0);

#define MY_STRING "MQ2LinkDB \ar4.0"

struct SearchResult
{
	std::string line;
	size_t textPos, textLength;

	SearchResult(std::string line, size_t start, size_t length)
		: line(std::move(line)), textPos(start), textLength(length) {}
};

static std::vector<SearchResult> SearchLinkDB(std::string_view searchText, bool bExact, int limit = -1);
static void ConvertItemsDotTxt();

// Keep the last 10 results we've done and then cycle through.
// When I just kept the last one, doing two ${LinkDB[]} calls in one
// macro link crashed EQ. Well now you can do 10 on one line. If that's
// not enough, increase the LAST_RESULT_CACHE_SIZE.

constexpr int LAST_RESULT_CACHE_SIZE = 10;
static char g_tloLastResult[LAST_RESULT_CACHE_SIZE][256];
static int g_iLastResultPosition = 0;

static bool bQuietMode = true;               // Display debug chatter?
static int iAddedThisSession = 0;            // How many new links found since startup
static bool bKnowTotal = false;              // Do we know how many links in db?
static size_t iMaxResults = 10;                 // Display at most this many results
static int iFindItemID = 0;                  // Item ID to /link
static int iVerifyCount;                     // Starting line # to generate 100 links for verification
static bool bScanChat = true;                // Scan incoming chat for links
static bool bClickLinks = false;             // click on link generated?
static bool bReadFileInFind = false;         // should we reload the file when looking for new links?

static std::unordered_set<int> presentItemIDs;
static bool bPresentItemIDsLoaded = false;

static char szLink[MAX_STRING / 4] = { 0 };
static int iCurrentID = 0;
static int iNextID = 0;

char szLinkDBFileName[MAX_PATH] = { 0 };

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

static int VerifyLinks();

static void SaveSettings()
{
	WritePrivateProfileInt("Settings", "MaxResults", iMaxResults, INIFileName);
	WritePrivateProfileBool("Settings", "ScanChat", bScanChat, INIFileName);
	WritePrivateProfileBool("Settings", "ClickLinks", bClickLinks, INIFileName);
}

static void LoadSettings()
{
	sprintf_s(szLinkDBFileName, "%s\\MQ2LinkDB.txt", gPathResources);

	iMaxResults = GetPrivateProfileInt("Settings", "MaxResults", 10, INIFileName);
	if (iMaxResults < 1) iMaxResults = 1;

	bScanChat = GetPrivateProfileBool("Settings", "ScanChat", true, INIFileName);
	bClickLinks = GetPrivateProfileBool("Settings", "ClickLinks", false, INIFileName);
}

static int ItemID(std::string_view link)
{
	ItemLinkInfo linkInfo;
	if (!ParseItemLink(link, linkInfo))
		return 0;

	return linkInfo.itemID;
}

void CreateIndex()
{
	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szLinkDBFileName, "rt");
	if (err) return;

	char szLine[MAX_STRING] = { 0 };
	while (fgets(szLine, MAX_STRING, File) != nullptr)
	{
		int iItemID = ItemID(szLine);
		presentItemIDs.insert(iItemID);
	}

	bKnowTotal = true;
	bPresentItemIDsLoaded = true;
	fclose(File);
}

static bool FindLink(std::string_view link)
{
	ItemLinkInfo findLink;
	if (!ParseItemLink(link, findLink))
		return false;

	if (bPresentItemIDsLoaded)
	{
		if (presentItemIDs.count(findLink.itemID))
		{
			if (!bQuietMode)
			{
				WriteChatf("MQ2LinkDB: Saw link \ay%d\ax, but we already have it.", findLink.itemID);
			}

			return true;
		}

		if (!bReadFileInFind)
		{
			return false;
		}
	}

	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szLinkDBFileName, "rt");
	if (err) return false;

	// Since we're scanning the file anyway, we'll make the index here to save some time, and to
	// account for ppl creating new MQ2LinkDB.txt files or whatever.
	presentItemIDs.clear();

	bool bRet = false;
	bool replaceAugLink = false;
	int replacePos = -1;
	char szLine[1024] = { 0 };

	while (fgets(szLine, 1024, File) != nullptr)
	{
		ItemLinkInfo linkInfo;
		if (ParseItemLink(szLine, linkInfo))
		{
			int itemId = linkInfo.itemID;
			presentItemIDs.insert(itemId);

			if (itemId == findLink.itemID)
			{
				bRet = true;

				// Try to replace a socketed item with an unsocketed item, if possible.
				if (linkInfo.IsSocketed() && !findLink.IsSocketed())
				{
					replaceAugLink = true;
					replacePos = ftell(File);
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
	}

	fclose(File);
	bPresentItemIDsLoaded = true;
	bKnowTotal = true;

	// The link in our db had an aug, but ours did not. we take this opportunity to replace it
	if (replaceAugLink)
	{
		if (!bQuietMode)
		{
			WriteChatf("MQ2LinkDB: Replacing auged link with un-auged link for item \ay%d\ax", findLink.itemID);
		}

		FILE* File2 = nullptr;
		err = fopen_s(&File2, szLinkDBFileName, "r+");
		if (!err)
		{
			fseek(File2, replacePos - strlen(szLine) - 2, SEEK_SET);

			// Double check position - paranoia!
			char szTemp[10];
			fread(szTemp, 10, 1, File2);
			if (memcmp(szTemp, link.data(), 8) == 0)
			{
				fseek(File2, replacePos - strlen(szLine) - 2, SEEK_SET); // Seek same place again
				fwrite(link.data(), link.length(), 1, File2);
			}
			else if (!bQuietMode)
			{
				WriteChatf("MQ2LinkDB: \arERROR - Sanity check failed while replacing");
			}

			fclose(File2);
		}
		else if (!bQuietMode)
		{
			WriteChatf("MQ2LinkDB: \arERROR - Could not open db file for writing (%d)", errno);
		}
	}

	return bRet;
}

static void StoreLink(std::string_view link)
{
	if (!bPresentItemIDsLoaded)
	{
		CreateIndex();
	}

	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szLinkDBFileName, "at");
	if (err)
	{
		if (!bQuietMode)
			WriteChatf("MQ2LinkDB: \arERROR - Could not open db file for writing (%d)", errno);
		return;
	}

	fwrite(link.data(), link.length(), 1, File);
	fputs("\n", File);
	fclose(File);

	int iItemID = ItemID(link);
	iAddedThisSession++;
	presentItemIDs.insert(iItemID);

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Stored link for item ID: \ay%d\ax %.*s (\ay%d\ax stored this session)", iItemID, link.length(), link.data(), iAddedThisSession);
	}
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
		else if (ci_equals(param, "/verify"))
		{
			GetNextParam(param);

			iVerifyCount = GetIntFromString(param, 1);
			VerifyLinks();

			bAnyParams = true;
		}
	}

	if (!bAnyParams)
	{
		WriteChatf("%s", MY_STRING);
		WriteChatf("MQ2LinkDB: Syntax: \ay/link [/max #] [/scan on|off] [/click on|off] [/import] [/item #] [/verify #] [search string]\ax");

		if (bKnowTotal)
		{
			WriteChatf("MQ2LinkDB: \ay%d\ax links in database, \ay%d\ax links added this session", presentItemIDs.size(), iAddedThisSession);
		}
		else
		{
			WriteChatf("MQ2LinkDB: \ay%d\ax links added this session", iAddedThisSession);
		}

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
		sprintf_s(szCommand, "/notify ChatWindow CW_ChatOutput link %.*s", linkData.length(), linkData.data());

		EzCommand(szCommand);
	}
}

// Do the actual local file search. This searches the local name->link
// data file line by line returning matches.
static std::vector<SearchResult> SearchLinkDB(std::string_view searchText, bool bExact, int limit)
{
	std::vector<SearchResult> results;

	iNextID = 0; iCurrentID = 0;
	int iFound = 0, iTotal = 0;
	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szLinkDBFileName, "rt");
	if (err)
	{
		WriteChatf("MQ2LinkDB: No item database yet");
		return results;
	}

	presentItemIDs.clear();

	if (!bQuietMode)
	{
		WriteChatf("MQ2LinkDB: Searching for '\ay%.*s\ax'...", searchText.length(), searchText.data());
	}

	char szLine[256] = { 0 };
	bool bNextID = false;

	while (fgets(szLine, sizeof(szLine), File) != nullptr)
	{
		std::string_view line{ szLine };
		line = trim(line);

		int iItemID = ItemID(line);
		presentItemIDs.insert(iItemID);

		if (bNextID)
		{
			bNextID = false;
			iNextID = iItemID;
		}

		if (limit == -1 || (int)results.size() < limit)
		{
			TextTagInfo info = ExtractLink(line);

			if ((iItemID == iFindItemID) || ci_equals(info.text, searchText, bExact))
			{
				size_t start = info.text.data() - line.data();
				results.emplace_back(std::string(line), start, info.text.length());

				if (iFindItemID || ci_equals(info.text, searchText))
				{
					bNextID = true;
				}

				iFound++;
			}
		}

		iTotal++;
	}

	bKnowTotal = true;
	fclose(File);

	return results;
}

int VerifyLinks()
{
	constexpr int VERIFYLINKCOUNT = 100;

	char szFilename[MAX_PATH];
	sprintf_s(szFilename, "%s\\links.txt", gPathResources);
	FILE* File = nullptr;

	errno_t err = fopen_s(&File, szLinkDBFileName, "rt");
	if (!err)
	{
		//WriteChatf ("MQ2LinkDB: Verifying links.txt...");
		char szLine[MAX_STRING];

		int iLinkCount = 0;
		int iEndingLinkCount = iVerifyCount + VERIFYLINKCOUNT - 1;

		while (fgets(szLine, MAX_STRING, File) != nullptr)
		{
			szLine[strlen(szLine) - 1] = '\0';
			iLinkCount++;

			if (iLinkCount >= iVerifyCount)
			{
				WriteChatf("%s", szLine);
				if (iLinkCount >= iEndingLinkCount) break;
			}
		}

		if (iLinkCount < iVerifyCount)
		{
			WriteChatf("MQ2LinkDB: \ayUnable to skip to line \ar%d\ay, only \ar%d\ay lines exist in links.txt", iVerifyCount, iLinkCount);
		}
		else
		{
			WriteChatf("MQ2LinkDB: Complete! \ay%d\ax links (from \ay%d\ax to \ay%d\ax) verified", iLinkCount - iVerifyCount + 1, iVerifyCount, iLinkCount);
		}
		fclose(File);
	}
	else
	{
		WriteChatf("MQ2LinkDB: \arSource file not found (links.txt)");
	}

	return 0;
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
		ParseParameters(line);
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
		for (size_t i = 0; i < results.size(); ++i)
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
	sprintf_s(szTemp3, "MQ2LinkDB: Found \ay%d\ax items from database of \ay%d\ax total items", results.size(), presentItemIDs.size());
	sprintf_s(szTemp, "Found %d items from database of %d total items", results.size(), presentItemIDs.size());

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

	pLinkType = new MQ2LinkType;
	LoadSettings();
}

// Called once, when the plugin is to shutdown
PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("Shutting down MQ2LinkDB");
	RemoveCommand("/link");
	RemoveMQ2Data("LinkDB");

	delete pLinkType;
}

/*
	1 a.ITEM_NUMBER
	2 a.NAME
	3 a.LORE_DESCRIPTION
	4 a.ADVANCED_LORE_TEXT_FILENAME
	5 a.TYPE
	6 a.VALUE
	7 CONCAT('IT',a.ACTOR_TAG)
	8 a.IMAGE_NUMBER
	9 a.SIZE_ITEM
	10 a.WEIGHT
	11 a.MAX_ITEM_COUNT
	12 a.ITEM_CLASS
	13 a.IS_LORE
	14 a.IS_ARTIFACT
	15 a.IS_SUMMONED
	16 a.REQUIRED_LEVEL
	17 a.RECOMMENDED_LEVEL
	18 a.RECOMMENDED_SKILL
	19 a.NO_DROP
	20 a.RENTABLE
	21 a.NO_ALT_TRANSFER
	22 a.FV_NO_DROP
	23 a.NO_PET_EQUIP
	24 a.CHARM
	25 a.EARS
	26 a.HEAD
	27 a.FACE
	28 a.NECK
	29 a.SHOULDERS
	30 a.ARMS
	31 a.BACK
	32 a.WRISTS
	33 a.RANGE
	34 a.HANDS
	35 a.PRIMARY
	36 a.SECONDARY
	37 a.FINGERS
	38 a.CHEST
	39 a.LEGS
	40 a.FEET
	41 a.WAIST
	42 a.POWER_SLOT
	43 a.AMMO
	44 a.WARRIOR_USEABLE
	45 a.CLERIC_USEABLE
	46 a.PALADIN_USEABLE
	47 a.RANGER_USEABLE
	48 a.SHADOWKNIGHT_USEABLE
	49 a.DRUID_USEABLE
	50 a.MONK_USEABLE
	51 a.BARD_USEABLE
	52 a.ROGUE_USEABLE
	53 a.SHAMAN_USEABLE
	54 a.NECROMANCER_USEABLE
	55 a.WIZARD_USEABLE
	56 a.MAGICIAN_USEABLE
	57 a.ENCHANTER_USEABLE
	58 a.BEASTLORD_USEABLE
	59 a.BERSERKER_USEABLE
	60 a.MERCENARY_USABLE
	61 a.HUMAN_USEABLE
	62 a.BARBARIAN_USEABLE
	63 a.ERUDITE_USEABLE
	64 a.WOODELF_USEABLE
	65 a.HIGHELF_USEABLE
	66 a.DARKELF_USEABLE
	67 a.HALFELF_USEABLE
	68 a.DWARF_USEABLE
	69 a.TROLL_USEABLE
	70 a.OGRE_USEABLE
	71 a.HALFLING_USEABLE
	72 a.GNOME_USEABLE
	73 a.IKSAR_USEABLE
	74 a.VAHSHIR_USEABLE
	75 a.FROGLOK_USEABLE
	76 a.DRAKKIN_USEABLE
	77 a.TEMPLATE_USEABLE
	78 a.REQ_AGNOSTIC
	79 a.REQ_BERTOXXULOUS
	80 a.REQ_BRELLSERILIS
	81 a.REQ_CAZICTHULE
	82 a.REQ_EROLLISIMARR
	83 a.REQ_FIZZLETHORP
	84 a.REQ_INNORUUK
	85 a.REQ_KARANA
	86 a.REQ_MITHMARR
	87 a.REQ_PREXUS
	88 a.REQ_QUELLIOUS
	89 a.REQ_RALLOSZEK
	90 a.REQ_RODCETNIFE
	91 a.REQ_SOLUSEKRO
	92 a.REQ_TRIBUNAL
	93 a.REQ_TUNARE
	94 a.REQ_VEESHAN
	95 a.MODFACTION1_NUM
	96 a.MODFACTION1_VALUE
	97 a.MODFACTION2_NUM
	98 a.MODFACTION2_VALUE
	99 a.MODFACTION3_NUM
	100 a.MODFACTION3_VALUE
	101 a.MODFACTION4_NUM
	102 a.MODFACTION4_VALUE
	103 a.GEM_SIZE
	104 a.SOCKET_CLASS
	105 a.SOLVENT_ITEM_ID
	106 a.POINT_TYPE
	107 a.POINT_THEME_BIT_MASK
	108 a.POINT_COST
	109 a.POINT_BUY_BACK_PERCENT
	110 a.ADVENTURE_ESTEEM_NEEDED
	111 a.FOOD_DURATION
	112 a.LIGHT_TYPE
	113 a.MAGIC
	114 a.SKIN_TYPE
	115 a.ARMOR_VARIANT
	116 a.ARMOR_MAT
	117 a.R_TINT
	118 a.G_TINT
	119 a.B_TINT
	120 a.ANIMATION_OVERRIDE
	121 a.TINT_PALETTE_INDEX
	122 a.MERCHANT_MULTIPLIER
	123 a.LOG
	124 a.LOOT_LOG
	125 a.REQ_AVATAR
	126 a.SKILL_MOD
	127 a.SKILL_BONUS
	128 a.SPECIAL_SKILL_CAP
	129 a.POOF_ON_DEATH
	130 a.INSTRUMENT_TYPE
	131 a.INSTRUMENT_PERCENTAGE_MOD
	132 a.SCRIPTID
	133 a.SCRIPT_FILE_NAME
	134 a.TRADESKILL
	135 a.TRIBUTE_VALUE
	136 a.GUILD_TRIBUTE_VALUE
	137 a.HIGH_PROFILE
	138 a.IS_POTION_BELT_ALLOWED
	139 a.NUM_POTION_SLOTS
	140 a.CAN_USE_FILTER_ID
	141 a.RIGHT_CLICK_SCRIPT_ID
	142 a.IS_QUEST_ITEM
	143 a.NO_ENDLESS_QUIVER
	144 a.POWER_CHARGES
	145 a.POWER_PURITY
	146 a.IS_EPIC_1
	147 a.IS_EPIC_15
	148 a.IS_EPIC_2
	149 b.STR_MOD
	150 b.INT_MOD
	151 b.WIS_MOD
	152 b.AGI_MOD
	153 b.DEX_MOD
	154 b.STA_MOD
	155 b.CHA_MOD
	156 b.HP_MOD
	157 b.MANA_MOD
	158 b.AC_MOD
	159 b.ENDURANCE_MOD
	160 b.SAVE_MAGIC_MOD
	161 b.SAVE_FIRE_MOD
	162 b.SAVE_COLD_MOD
	163 b.SAVE_DISEASE_MOD
	164 b.SAVE_POISON_MOD
	165 b.SAVE_CORRUPTION_MOD
	166 c.HASTE
	167 c.ATTACK
	168 c.HP_REGEN
	169 c.MANA_REGEN
	170 c.ENDURANCE_REGEN
	171 c.DAMAGE_SHIELD
	172 c.COMBAT_EFFECTS
	173 c.SHIELDING
	174 c.SPELL_SHIELDING
	175 c.AVOIDANCE
	176 c.ACCURACY
	177 c.STUN_RESIST
	178 c.STRIKETHROUGH
	179 c.SKILL_DAMAGE_NUM
	180 c.SKILL_DAMAGE_MOD
	181 c.DOT_SHIELDING
	182 d.DELAY
	183 d.BASE_DAMAGE
	184 d.ITEM_RANGE
	185 d.BANE_RACE
	186 d.BANE_BODYTYPE
	187 d.RACE_BANE_DAMAGE
	188 d.BODYTYPE_BANE_DAMAGE
	189 d.ELEMENT_CRIT
	190 d.ELEMENT_DAMAGE
	191 e.CONTAINER_TYPE
	192 e.CONTAINER_CAPACITY
	193 e.CONTAINER_ITEM_SIZE_LIMIT
	194 e.CONTAINER_WEIGHT_RDX
	195 f.NOTE_TYPE
	196 f.NOTE_LANGUAGE
	197 f.NOTE_TEXTFILE
	198 d.BACKSTAB_DAMAGE
	199 c.DAMAGE_SHIELD_MITIGATION
	200 b.HEROIC_STR_MOD
	201 b.HEROIC_INT_MOD
	202 b.HEROIC_WIS_MOD
	203 b.HEROIC_AGI_MOD
	204 b.HEROIC_DEX_MOD
	205 b.HEROIC_STA_MOD
	206 b.HEROIC_CHA_MOD
	207 b.HEROIC_SAVE_MAGIC_MOD
	208 b.HEROIC_SAVE_FIRE_MOD
	209 b.HEROIC_SAVE_COLD_MOD
	210 b.HEROIC_SAVE_DISEASE_MOD
	211 b.HEROIC_SAVE_POISON_MOD
	212 b.HEROIC_SAVE_CORRUPTION_MOD
	213 c.HEAL_AMOUNT
	214 c.SPELL_DAMAGE
	215 c.CLAIRVOYANCE
	216 a.SUB_CLASS
	217 a.LOGIN_REGISTRATION_REQUIRED
	218 a.LAUNCH_SCRIPT_ID
	219 a.HEIRLOOM
	220 g.EQG_ID
	221 g.PLACEMENT_FLAGS
	222 g.IGNORE_COLLISIONS
	223 g.PLACEMENT_TYPE
	224 g.REAL_ESTATE_DEF_ID
	225 g.PLACEABLE_SCALE_RANGE_MIN
	226 g.PLACEABLE_SCALE_RANGE_MAX
	227 g.REAL_ESTATE_UPKEEP_ID
	228 g.MAX_PER_REAL_ESTATE
	229 g.NPC_FILENAME
	230 g.TROPHY_BENEFIT_ID
	231 g.DISABLE_PLACEMENT_ROTATION
	232 g.DISABLE_FREE_PLACEMENT
	233 g.NPC_RESPAWN_INTERVAL
	234 g.PLACEABLE_SCALE_DEFAULT
	235 g.PLACEABLE_ORIENTATION_HEADING
	236 g.PLACEABLE_ORIENTATION_PITCH
	237 g.PLACEABLE_ORIENTATION_ROLL
	238 a.NO_BANK
	239 CONCAT('IT',a.SECONDARY_ACTOR_TAG)
	240 g.IS_INTERACTIVE_OBJECT
	241 a.SKILL_MOD_OFFSET
	242 a.SOCKET_SUB_CLASSES
	243 a.NEW_ARMOR_ID
	244 a.ITEM_RANK
	245 a.SOCKET_SKIN_TYPE_MASK
	246 a.IS_COLLECTIBLE
	247 a.NO_DESTROY
	248 a.NO_NPC
	249 a.NO_ZONE
	250 a.CREATOR_ID
	251 a.NO_GROUND
	252 a.NO_LOOT
*/

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


// SODEQ converter
struct SODEQITEM
{
	int  itemclass;
	char name[ITEM_NAME_LEN];
	char lore[LORE_NAME_LEN];
	char idfile[0x20];
	char lorefile[255];
	long id;
	long weight;
	short norent;
	short nodrop;
	short attuneable;
	short size;
	short slots;
	long price;
	long icon;
	int benefitflag;
	short tradeskills;
	short cr;
	short dr;
	short pr;
	short mr;
	short fr;
	short svcorruption;
	short astr;
	short asta;
	short aagi;
	short adex;
	short acha;
	short aint;
	short awis;
	long hp;
	long mana;
	long endur;
	long ac;
	long regen;
	long manaregen;
	long enduranceregen;
	long classes;
	long races;
	long deity;
	long skillmodvalue;
	long skillmodmax;
	long skillmodtype;
	long skillmodextra;
	long banedmgrace;
	long banedmgbody;
	short banedmgraceamt;
	short banedmgamt;
	short magic;
	long foodduration;
	long reqlevel;
	long reclevel;
	short recskill;
	long bardtype;
	long bardvalue;
	short unk002;
	short unk003;
	short unk004;
	short light;
	short delay;
	short elemdmgtype;
	short elemdmgamt;
	short range;
	long damage;
	long color;
	long prestige;
	short unk006;
	short unk007;
	short unk008;
	long itemtype;
	short material;
	short materialunk1;
	short elitematerial;
	short heroforge1;
	short heroforge2;
	short sellrate;
	long extradmgskill;
	long extradmgamt;
	long charmfileid;
	long factionmod1;
	long factionamt1;
	long factionmod2;
	long factionamt2;
	long factionmod3;
	long factionamt3;
	long factionmod4;
	long factionamt4;
	char charmfile[0x20];
	long augtype;
	long augstricthidden;
	long augrestrict;
	long augslot1type;
	short augslot1visible;
	long augslot1unk2;
	long augslot2type;
	short augslot2visible;
	long augslot2unk2;
	long augslot3type;
	short augslot3visible;
	long augslot3unk2;
	long augslot4type;
	short augslot4visible;
	long augslot4unk2;
	long augslot5type;
	short augslot5visible;
	long augslot5unk2;
	long augslot6type;
	short augslot6visible;
	long augslot6unk2;
	short pointtype;
	long ldontheme;
	long ldonprice;
	short ldonsellbackrate;
	short ldonsold;
	short bagtype;
	short bagslots;
	short bagsize;
	short bagwr;
	short booktype;
	short booklang;
	char filename[0x1e];
	long loregroup;
	short artifactflag;
	short summoned;
	long favor;
	short fvnodrop;
	long attack;
	long haste;
	long guildfavor;
	long augdistiller;
	short unk009;
	short unk010;
	short nopet;
	short unk011;
	//short potionbelt;
	//short potionbeltslots;
	long stacksize;
	short notransfer;
	short expendablearrow;
	short unk012;
	short unk013;
	short clickeffect;
	short clicklevel2;
	short clicktype;
	short clicklevel;
	short clickmaxcharges;
	short clickcasttime;
	short clickrecastdelay;
	short clickrecasttype;
	short clickunk5;
	short clickname;
	short clickunk7;
	short proceffect;
	short proclevel2;
	short proctype;
	short proclevel;
	short procunk1;
	short procunk2;
	short procunk3;
	short procunk4;
	short procrate;
	short procname;
	short procunk7;
	short worneffect;
	short wornlevel2;
	short worntype;
	short wornlevel;
	short wornunk1;
	short wornunk2;
	short wornunk3;
	short wornunk4;
	short wornunk5;
	short wornname;
	short wornunk7;
	short focuseffect;
	short focuslevel2;
	short focustype;
	short focuslevel;
	short focusunk1;
	short focusunk2;
	short focusunk3;
	short focusunk4;
	short focusunk5;
	short focusname;
	short focusunk7;
	short scrolleffect;
	short scrolllevel2;
	short scrolltype;
	short scrolllevel;
	short scrollunk1;
	short scrollunk2;
	short scrollunk3;
	short scrollunk4;
	short scrollunk5;
	short scrollname;
	short scrollunk7;
	short bardeffect;
	short bardlevel2;
	short bardeffecttype;
	short bardlevel;
	short bardunk1;
	short bardunk2;
	short bardunk3;
	short bardunk4;
	short bardunk5;
	short bardname;
	short bardunk7;
	char unk014[0x40];
	char unk015[0x40];
	char unk016[0x40];
	short unk017;
	short unk018;
	short unk019;
	short unk020;
	short unk021;
	short unk022;
	short scriptfile;
	short questitemflag;
	long powersourcecapacity;
	long purity;
	short epic;
	long backstabdmg;
	long heroic_str;
	long heroic_int;
	long heroic_wis;
	long heroic_agi;
	long heroic_dex;
	long heroic_sta;
	long heroic_cha;
	short unk029;
	long healamt;
	long spelldmg;
	long clairvoyance;
	short unk030;
	short unk031;
	short unk032;
	short unk033;
	short unk034;
	short unk035;
	short unk036;
	short unk037;
	short heirloom;
	long placeable;
	long unk038;
	short unk039;
	short unk040;
	long unk041; // read as double convert by *100
	long unk042; // read as double convert by *100
	short unk043;
	long unk044;
	char placeablenpcname[0x40];
	short unk046;
	long unk047;
	short unk048;
	short unk049;
	long unk050; // read as double convert by *100
	short unk051;
	short unk052;
	short unk053;
	short unk054;
	short unk055;
	short unk056;
	short unk057;
	short unk058;
	short unk059;
	short unk060;
	short unk061;
	short unk062;
	char unk063[0x40];
	short collectible;
	short nodestroy;
	short nonpc;
	short nozone;
	short unk068;
	short unk069;
	short unk070;
	short unk071;
	short noground;
	short unk073;
	short marketplace;
	short freestorage;
	short unk076;
	short unk077;
	short unk078;
	short unk079;
	short evolving;
	long evoid;
	long evolvinglevel;
	long evomax;
	short convertable;
	long convertid;
	char convertname[ITEM_NAME_LEN];

	short updated;
	short created;
	char submitter[255];
	short verified;
	char verifiedby[255];
	char collectversion[0x20];
};

void SODEQSetField(SODEQITEM* Item, int iField, const char* cField)
{
	int lValue = atol(cField);
	double dValue = atof(cField);
	bool bValue = lValue > 0;
	switch (iField) {
	case   0: Item->itemclass = lValue; break;
	case   1: strcpy_s(Item->name, cField); break;
	case   2: strcpy_s(Item->lore, cField); break;

	case   3: strcpy_s(Item->idfile, cField); break;
	case   4: strcpy_s(Item->lorefile, cField); break;
	case   5: Item->id = lValue; break;
	case   6: Item->weight = lValue; break;
	case   7: Item->norent = bValue; break;
	case   8: Item->nodrop = bValue; break;
	case   9: Item->attuneable = bValue; break;
	case  10: Item->size = lValue; break;
	case  11: Item->slots = lValue; break;
	case  12: Item->price = lValue; break;
	case  13: Item->icon = lValue; break;
	case  14: Item->benefitflag = lValue; break;
	case  15: Item->tradeskills = bValue; break;
	case  16: Item->cr = lValue; break;
	case  17: Item->dr = lValue; break;
	case  18: Item->pr = lValue; break;
	case  19: Item->mr = lValue; break;
	case  20: Item->fr = lValue; break;
	case  21: Item->svcorruption = lValue; break;
	case  22: Item->astr = lValue; break;
	case  23: Item->asta = lValue; break;
	case  24: Item->aagi = lValue; break;
	case  25: Item->adex = lValue; break;
	case  26: Item->acha = lValue; break;
	case  27: Item->aint = lValue; break;
	case  28: Item->awis = lValue; break;
	case  29: Item->hp = lValue; break;
	case  30: Item->mana = lValue; break;
	case  31: Item->endur = lValue; break;
	case  32: Item->ac = lValue; break;
	case  33: Item->regen = lValue; break;
	case  34: Item->manaregen = lValue; break;
	case  35: Item->enduranceregen = lValue; break;
	case  36: Item->classes = lValue; break;
	case  37: Item->races = lValue; break;
	case  38: Item->deity = lValue; break;
	case  39: Item->skillmodvalue = lValue; break;
	case  40: Item->skillmodmax = lValue; break;
	case  41: Item->skillmodtype = lValue; break;
	case  42: Item->skillmodextra = lValue; break;
	case  43: Item->banedmgrace = lValue; break;
	case  44: Item->banedmgbody = lValue; break;
	case  45: Item->banedmgraceamt = lValue; break;
	case  46: Item->banedmgamt = lValue; break;
	case  47: Item->magic = lValue; break;
	case  48: Item->foodduration = lValue; break;
	case  49: Item->reqlevel = lValue; break;
	case  50: Item->reclevel = lValue; break;
	case  51: Item->recskill = lValue; break;
	case  52: Item->bardtype = lValue; break;
	case  53: Item->bardvalue = lValue; break;
	case  54: Item->unk002 = lValue; break;
	case  55: Item->unk003 = lValue; break;
	case  56: Item->unk004 = lValue; break;
	case  57: Item->light = lValue; break;
	case  58: Item->delay = lValue; break;
	case  59: Item->elemdmgtype = lValue; break;
	case  60: Item->elemdmgamt = lValue; break;
	case  61: Item->range = lValue; break;
	case  62: Item->damage = lValue; break;
	case  63: Item->color = lValue; break;
	case  64: Item->prestige = lValue; break;
	case  65: Item->unk006 = lValue; break;
	case  66: Item->unk007 = lValue; break;
	case  67: Item->unk008 = lValue; break;
	case  68: Item->itemtype = lValue; break;
	case  69: Item->material = lValue; break;
	case  70: Item->materialunk1 = lValue; break;
	case  71: Item->elitematerial = lValue; break;
	case  72: Item->heroforge1 = lValue; break;
	case  73: Item->heroforge2 = lValue; break;
	case  74: Item->sellrate = lValue; break;
	case  75: Item->extradmgskill = lValue; break;
	case  76: Item->extradmgamt = lValue; break;
	case  77: Item->charmfileid = lValue; break;
	case  78: Item->factionmod1 = lValue; break;
	case  79: Item->factionamt1 = lValue; break;
	case  80: Item->factionmod2 = lValue; break;
	case  81: Item->factionamt2 = lValue; break;
	case  82: Item->factionmod3 = lValue; break;
	case  83: Item->factionamt3 = lValue; break;
	case  84: Item->factionmod4 = lValue; break;
	case  85: Item->factionamt4 = lValue; break;
	case  86: strcpy_s(Item->charmfile, cField); break;
	case  87: Item->augtype = lValue; break;
	case  88: Item->augstricthidden = lValue; break;
	case  89: Item->augrestrict = lValue; break;
	case  90: Item->augslot1type = lValue; break;
	case  91: Item->augslot1visible = lValue; break;
	case  92: Item->augslot1unk2 = lValue; break;
	case  93: Item->augslot2type = lValue; break;
	case  94: Item->augslot2visible = lValue; break;
	case  95: Item->augslot2unk2 = lValue; break;
	case  96: Item->augslot3type = lValue; break;
	case  97: Item->augslot3visible = lValue; break;
	case  98: Item->augslot3unk2 = lValue; break;
	case  99: Item->augslot4type = lValue; break;
	case 100: Item->augslot4visible = lValue; break;
	case 101: Item->augslot4unk2 = lValue; break;
	case 102: Item->augslot5type = lValue; break;
	case 103: Item->augslot5visible = lValue; break;
	case 104: Item->augslot5unk2 = lValue; break;
	case 105: Item->augslot6type = lValue; break;
	case 106: Item->augslot6visible = lValue; break;
	case 107: Item->augslot6unk2 = lValue; break;
	case 108: Item->pointtype = lValue; break;
	case 109: Item->ldontheme = lValue; break;
	case 110: Item->ldonprice = lValue; break;
	case 111: Item->ldonsellbackrate = lValue; break;
	case 112: Item->ldonsold = lValue; break;
	case 113: Item->bagtype = lValue; break;
	case 114: Item->bagslots = lValue; break;
	case 115: Item->bagsize = lValue; break;
	case 116: Item->bagwr = lValue; break;
	case 117: Item->booktype = lValue; break;
	case 118: Item->booklang = lValue; break;
	case 119: strcpy_s(Item->filename, cField); break;
	case 120: Item->loregroup = lValue; break;
	case 121: Item->artifactflag = bValue; break;
	case 122: Item->summoned = bValue; break;
	case 123: Item->favor = lValue; break;
	case 124: Item->fvnodrop = bValue; break;
	case 125: Item->attack = lValue; break;
	case 126: Item->haste = lValue; break;
	case 127: Item->guildfavor = lValue; break;
	case 128: Item->augdistiller = lValue; break;
	case 129: Item->unk009 = lValue; break;
	case 130: Item->unk010 = lValue; break;
	case 131: Item->nopet = lValue; break;
	case 132: Item->unk011 = lValue; break;
		//case 133: Item->potionbelt = lValue; break;
		//case 134: Item->potionbeltslots = lValue; break;
	case 133: Item->stacksize = lValue; break;
	case 134: Item->notransfer = bValue; break;
	case 135: Item->expendablearrow = lValue; break;
	case 136: Item->unk012 = lValue; break;
	case 137: Item->unk013 = lValue; break;
	case 138: Item->clickeffect = lValue; break;
	case 139: Item->clicklevel2 = lValue; break;
	case 140: Item->clicktype = lValue; break;
	case 141: Item->clicklevel = lValue; break;
	case 142: Item->clickmaxcharges = lValue; break;
	case 143: Item->clickcasttime = lValue; break;
	case 144: Item->clickrecastdelay = lValue; break;
	case 145: Item->clickrecasttype = lValue; break;
	case 146: Item->clickunk5 = lValue; break;
	case 147: Item->clickname = lValue; break;
	case 148: Item->clickunk7 = lValue; break;
	case 149: Item->proceffect = lValue; break;
	case 150: Item->proclevel2 = lValue; break;
	case 151: Item->proctype = lValue; break;
	case 152: Item->proclevel = lValue; break;
	case 153: Item->procunk1 = lValue; break;
	case 154: Item->procunk2 = lValue; break;
	case 155: Item->procunk3 = lValue; break;
	case 156: Item->procunk4 = lValue; break;
	case 157: Item->procrate = lValue; break;
	case 158: Item->procname = lValue; break;
	case 159: Item->procunk7 = lValue; break;
	case 160: Item->worneffect = lValue; break;
	case 161: Item->wornlevel2 = lValue; break;
	case 162: Item->worntype = lValue; break;
	case 163: Item->wornlevel = lValue; break;
	case 164: Item->wornunk1 = lValue; break;
	case 165: Item->wornunk2 = lValue; break;
	case 166: Item->wornunk3 = lValue; break;
	case 167: Item->wornunk4 = lValue; break;
	case 168: Item->wornunk5 = lValue; break;
	case 169: Item->wornname = lValue; break;
	case 170: Item->wornunk7 = lValue; break;
	case 171: Item->focuseffect = lValue; break;
	case 172: Item->focuslevel2 = lValue; break;
	case 173: Item->focustype = lValue; break;
	case 174: Item->focuslevel = lValue; break;
	case 175: Item->focusunk1 = lValue; break;
	case 176: Item->focusunk2 = lValue; break;
	case 177: Item->focusunk3 = lValue; break;
	case 178: Item->focusunk4 = lValue; break;
	case 179: Item->focusunk5 = lValue; break;
	case 180: Item->focusname = lValue; break;
	case 181: Item->focusunk7 = lValue; break;
	case 182: Item->scrolleffect = lValue; break;
	case 183: Item->scrolllevel2 = lValue; break;
	case 184: Item->scrolltype = lValue; break;
	case 185: Item->scrolllevel = lValue; break;
	case 186: Item->scrollunk1 = lValue; break;
	case 187: Item->scrollunk2 = lValue; break;
	case 188: Item->scrollunk3 = lValue; break;
	case 189: Item->scrollunk4 = lValue; break;
	case 190: Item->scrollunk5 = lValue; break;
	case 191: Item->scrollname = lValue; break;
	case 192: Item->scrollunk7 = lValue; break;
	case 193: Item->bardeffect = lValue; break;
	case 194: Item->bardlevel2 = lValue; break;
	case 195: Item->bardeffecttype = lValue; break;
	case 196: Item->bardlevel = lValue; break;
	case 197: Item->bardunk1 = lValue; break;
	case 198: Item->bardunk2 = lValue; break;
	case 199: Item->bardunk3 = lValue; break;
	case 200: Item->bardunk4 = lValue; break;
	case 201: Item->bardunk5 = lValue; break;
	case 202: Item->bardname = lValue; break;
	case 203: Item->bardunk7 = lValue; break;
	case 204: strcpy_s(Item->unk014, cField); break;
	case 205: strcpy_s(Item->unk015, cField); break;
	case 206: strcpy_s(Item->unk016, cField); break;
	case 207: Item->unk017 = lValue; break;
	case 208: Item->unk018 = lValue; break;
	case 209: Item->unk019 = lValue; break;
	case 210: Item->unk020 = lValue; break;
	case 211: Item->unk021 = lValue; break;
	case 212: Item->unk022 = lValue; break;
	case 213: Item->scriptfile = lValue; break;
	case 214: Item->questitemflag = bValue; break;
	case 215: Item->powersourcecapacity = lValue; break;
	case 216: Item->purity = lValue; break;
	case 217: Item->epic = lValue; break;
	case 218: Item->backstabdmg = lValue; break;
	case 219: Item->heroic_str = lValue; break;
	case 220: Item->heroic_int = lValue; break;
	case 221: Item->heroic_wis = lValue; break;
	case 222: Item->heroic_agi = lValue; break;
	case 223: Item->heroic_dex = lValue; break;
	case 224: Item->heroic_sta = lValue; break;
	case 225: Item->heroic_cha = lValue; break;
	case 226: Item->unk029 = lValue; break;
	case 227: Item->healamt = lValue; break;
	case 228: Item->spelldmg = lValue; break;
	case 229: Item->clairvoyance = lValue; break;
	case 230: Item->unk030 = lValue; break;
	case 231: Item->unk031 = lValue; break;
	case 232: Item->unk032 = lValue; break;
	case 233: Item->unk033 = lValue; break;
	case 234: Item->unk034 = lValue; break;
	case 235: Item->unk035 = lValue; break;
	case 236: Item->unk036 = lValue; break;
	case 237: Item->unk037 = lValue; break;
	case 238: Item->heirloom = bValue; break;
	case 239: Item->placeable = lValue; break;
	case 240: Item->unk038 = lValue; break;
	case 241: Item->unk039 = lValue; break;
	case 242: Item->unk040 = lValue; break;
	case 243: Item->unk041 = static_cast<long>(dValue * 100); break;
	case 244: Item->unk042 = static_cast<long>(dValue * 100); break;
	case 245: Item->unk043 = lValue; break;
	case 246: Item->unk044 = lValue; break;
	case 247: strcpy_s(Item->placeablenpcname, cField); break;
	case 248: Item->unk046 = lValue; break;
	case 249: Item->unk047 = lValue; break;
	case 250: Item->unk048 = lValue; break;
	case 251: Item->unk049 = lValue; break;
	case 252: Item->unk050 = static_cast<long>(dValue * 100); break;
	case 253: Item->unk051 = lValue; break;
	case 254: Item->unk052 = lValue; break;
	case 255: Item->unk053 = lValue; break;
	case 256: Item->unk054 = lValue; break;
	case 257: Item->unk055 = lValue; break;
	case 258: Item->unk056 = lValue; break;
	case 259: Item->unk057 = lValue; break;
	case 260: Item->unk058 = lValue; break;
	case 261: Item->unk059 = lValue; break;
	case 262: Item->unk060 = lValue; break;
	case 263: Item->unk061 = lValue; break;
	case 264: Item->unk062 = lValue; break;
	case 265: strcpy_s(Item->unk063, cField); break;
	case 266: Item->collectible = bValue; break;
	case 267: Item->nodestroy = bValue; break;
	case 268: Item->nonpc = bValue; break;
	case 269: Item->nozone = bValue; break;
	case 270: Item->unk068 = lValue; break;
	case 271: Item->unk069 = lValue; break;
	case 272: Item->unk070 = lValue; break;
	case 273: Item->unk071 = lValue; break;
	case 274: Item->noground = lValue; break;
	case 275: Item->unk073 = lValue; break;
	case 276: Item->marketplace = lValue; break;
	case 277: Item->freestorage = lValue; break;
	case 278: Item->unk076 = lValue; break;
	case 279: Item->unk077 = lValue; break;
	case 280: Item->unk078 = lValue; break;
	case 281: Item->unk079 = lValue; break;
	case 282: Item->evolving = bValue; break;
	case 283: Item->evoid = lValue; break;
	case 284: Item->evolvinglevel = lValue; break;
	case 285: Item->evomax = lValue; break;
	case 286: Item->convertable = lValue; break;
	case 287: Item->convertid = lValue; break;
	case 288: strcpy_s(Item->convertname, cField); break;

	case 289: Item->updated = lValue; break;
	case 290: Item->created = lValue; break;
	case 291: strcpy_s(Item->submitter, cField); break;
	case 292: Item->verified = lValue; break;
	case 293: strcpy_s(Item->verifiedby, cField); break;
	case 294: strcpy_s(Item->collectversion, cField); break;
	}
}

static void SODEQReadItem(SODEQITEM* Item, char* szLine)
{
	char* ptr = szLine;
	int iField = 0;

	while (*ptr)
	{
		char szField[256];
		char* pDest = szField;
		bool bEscape = false;

		//DebugSpew("Escape: %s, cPtr: %c", bEscape?"True":"False", *cPtr);
		while ((*ptr != '|' || bEscape) && *ptr != '\0')
		{
			if (bEscape) bEscape = !bEscape; else bEscape = *ptr == '\\';
			if (bEscape) { ptr++; /*DebugSpew("Escape: %s, cPtr: %c", bEscape?"True":"False", *cPtr);*/ continue; }
			*(pDest++) = *(ptr++);
			//DebugSpew("Escape: %s, cPtr: %c", bEscape?"True":"False", *cPtr);
		}
		*pDest = '\0';
		ptr++;

		//WriteChatf("cField: %s", cField);
		SODEQSetField(Item, iField++, szField);
	}
}

static std::string SODEQMakeLink(const SODEQITEM& Item, ItemPtr pItem)
{
	ItemDefinition* pItemDef = pItem->GetItemDefinition();

	strcpy_s(pItemDef->Name, Item.name);
	strcpy_s(pItemDef->LoreName, Item.lore);
	pItemDef->ItemNumber = Item.id;
	pItemDef->EquipSlots = Item.slots;
	pItemDef->Cost = Item.price;
	pItemDef->IconNumber = Item.icon;
	pItemDef->Weight = Item.weight;
	pItemDef->NoRent = Item.norent;
	pItemDef->IsDroppable = Item.notransfer;
	pItemDef->Attuneable = Item.attuneable;
	pItemDef->Heirloom = Item.heirloom;
	pItemDef->Collectible = Item.collectible;
	pItemDef->NoDestroy = Item.nodestroy;
	pItemDef->Size = static_cast<uint8_t>(Item.size);
	pItemDef->Type = Item.itemclass;
	pItemDef->TradeSkills = Item.tradeskills;
	pItemDef->Lore = (Item.loregroup == 0 ? 0 : 1);
	pItemDef->Artifact = Item.artifactflag;
	pItemDef->Summoned = Item.summoned;
	pItemDef->SvCold = static_cast<char>(Item.cr);
	pItemDef->SvFire = static_cast<char>(Item.fr);
	pItemDef->SvMagic = static_cast<char>(Item.mr);
	pItemDef->SvDisease = static_cast<char>(Item.dr);
	pItemDef->SvPoison = static_cast<char>(Item.pr);
	pItemDef->SvCorruption = static_cast<char>(Item.svcorruption);
	pItemDef->STR = static_cast<char>(Item.astr);
	pItemDef->STA = static_cast<char>(Item.asta);
	pItemDef->AGI = static_cast<char>(Item.aagi);
	pItemDef->DEX = static_cast<char>(Item.adex);
	pItemDef->CHA = static_cast<char>(Item.acha);
	pItemDef->INT = static_cast<char>(Item.aint);
	pItemDef->WIS = static_cast<char>(Item.awis);
	pItemDef->HP = Item.hp;
	pItemDef->Mana = Item.mana;
	pItemDef->AC = Item.ac;
	pItemDef->RequiredLevel = Item.reqlevel;
	pItemDef->RecommendedLevel = Item.reclevel;
	pItemDef->RecommendedSkill = Item.recskill;
	pItemDef->SkillModType = Item.skillmodtype;
	pItemDef->SkillModValue = Item.skillmodvalue;
	pItemDef->SkillModMax = Item.skillmodmax;
	pItemDef->SkillModBonus = Item.skillmodextra;
	pItemDef->BaneDMGRace = Item.banedmgrace;
	pItemDef->BaneDMGBodyType = Item.banedmgbody;
	pItemDef->BaneDMGBodyTypeValue = Item.banedmgamt;
	pItemDef->BaneDMGRaceValue = Item.banedmgraceamt;
	pItemDef->InstrumentType = Item.bardtype;
	pItemDef->InstrumentMod = Item.bardvalue;
	pItemDef->Classes = Item.classes;
	pItemDef->Races = Item.races;
	pItemDef->Diety = Item.deity;
	pItemDef->Magic = Item.magic;
	pItemDef->Light = static_cast<uint8_t>(Item.light);
	pItemDef->Delay = static_cast<uint8_t>(Item.delay);
	pItemDef->ElementalFlag = static_cast<uint8_t>(Item.elemdmgtype);
	pItemDef->ElementalDamage = static_cast<uint8_t>(Item.elemdmgamt);
	pItemDef->Range = static_cast<uint8_t>(Item.range);
	pItemDef->Damage = Item.damage;
	pItemDef->BackstabDamage = Item.backstabdmg;
	pItemDef->HeroicSTR = Item.heroic_str;
	pItemDef->HeroicINT = Item.heroic_int;
	pItemDef->HeroicWIS = Item.heroic_wis;
	pItemDef->HeroicAGI = Item.heroic_agi;
	pItemDef->HeroicDEX = Item.heroic_dex;
	pItemDef->HeroicSTA = Item.heroic_sta;
	pItemDef->HeroicCHA = Item.heroic_cha;
	pItemDef->HealAmount = Item.healamt;
	pItemDef->SpellDamage = Item.spelldmg;
	pItemDef->Prestige = Item.prestige;
	pItemDef->ItemClass = static_cast<uint8_t>(Item.itemtype);
	pItemDef->ArmorProps.material = Item.material;
	pItemDef->AugData.Sockets[0].Type = Item.augslot1type;
	pItemDef->AugData.Sockets[0].bVisible = Item.augslot1unk2;
	pItemDef->AugData.Sockets[1].Type = Item.augslot2type;
	pItemDef->AugData.Sockets[1].bVisible = Item.augslot2unk2;
	pItemDef->AugData.Sockets[2].Type = Item.augslot3type;
	pItemDef->AugData.Sockets[2].bVisible = Item.augslot3unk2;
	pItemDef->AugData.Sockets[3].Type = Item.augslot4type;
	pItemDef->AugData.Sockets[3].bVisible = Item.augslot4unk2;
	pItemDef->AugData.Sockets[4].Type = Item.augslot5type;
	pItemDef->AugData.Sockets[4].bVisible = Item.augslot5unk2;
	pItemDef->AugData.Sockets[5].Type = Item.augslot6type;
	pItemDef->AugData.Sockets[5].bVisible = Item.augslot6unk2;
	pItemDef->AugType = Item.augtype;
	pItemDef->AugRestrictions = Item.augrestrict;
	pItemDef->SolventItemID = Item.augdistiller;
	pItemDef->LDTheme = Item.ldontheme;
	pItemDef->LDCost = Item.ldonprice;
	strcpy_s(pItemDef->CharmFile, Item.charmfile);
	pItemDef->DmgBonusSkill = Item.extradmgskill;
	pItemDef->DmgBonusValue = Item.extradmgamt;
	pItemDef->CharmFileID = Item.charmfileid;
	pItemDef->FoodDuration = Item.foodduration;
	pItemDef->ContainerType = static_cast<uint8_t>(Item.bagtype);
	pItemDef->Slots = static_cast<uint8_t>(Item.bagslots);
	pItemDef->SizeCapacity = static_cast<uint8_t>(Item.bagsize);
	pItemDef->WeightReduction = static_cast<uint8_t>(Item.bagwr);
	pItemDef->BookType = static_cast<uint8_t>(Item.booktype);
	pItemDef->BookLang = static_cast<int8_t>(Item.booklang);
	strcpy_s(pItemDef->BookFile, Item.filename);
	pItemDef->Favor = Item.favor;
	pItemDef->GuildFavor = Item.guildfavor;
	pItemDef->bIsFVNoDrop = Item.fvnodrop;
	pItemDef->Endurance = Item.endur;
	pItemDef->Attack = Item.attack;
	pItemDef->HPRegen = Item.regen;
	pItemDef->ManaRegen = Item.manaregen;
	pItemDef->EnduranceRegen = Item.enduranceregen;
	pItemDef->Haste = Item.haste;
	pItemDef->bNoPetGive = Item.nopet;
	pItemDef->StackSize = Item.stacksize;
	pItemDef->MaxPower = Item.powersourcecapacity;
	pItemDef->Purity = Item.purity;
	pItemDef->QuestItem = Item.questitemflag;
	pItemDef->Expendable = Item.expendablearrow;
	pItemDef->Clairvoyance = Item.clairvoyance;
	pItemDef->Placeable = Item.placeable;
#if defined(ROF2EMU) || defined(UFEMU)
	pItemDef->FactionModType[0] = Item.factionmod1;
	pItemDef->FactionModType[1] = Item.factionmod2;
	pItemDef->FactionModType[2] = Item.factionmod3;
	pItemDef->FactionModType[3] = Item.factionmod4;
	pItemDef->FactionModValue[0] = Item.factionamt1;
	pItemDef->FactionModValue[1] = Item.factionamt2;
	pItemDef->FactionModValue[2] = Item.factionamt3;
	pItemDef->FactionModValue[3] = Item.factionamt4;
	strcpy_s(pItemDef->IDFile, Item.idfile);
#else
	pItemDef->bNoNPC = Item.nonpc;
	pItemDef->LoreEquipped = 1;
#endif

	bool IsEvolvingItem = Item.evoid > 0 && Item.evoid < 10000;
	if (IsEvolvingItem)
	{
		pItem->pEvolutionData = SoeUtil::MakeShared<ItemEvolutionData>();
		pItem->pEvolutionData->EvolvingMaxLevel = Item.evomax;
		pItem->pEvolutionData->EvolvingExpPct = 0;
		pItem->pEvolutionData->EvolvingCurrentLevel = (IsEvolvingItem ? Item.evolvinglevel : 0);
		pItem->pEvolutionData->GroupID = (IsEvolvingItem ? Item.evoid : (Item.loregroup > 0) ? Item.loregroup & 0xFFFF : 0);
		pItem->pEvolutionData->LastEquipped = 0;
	}
	else
	{
		pItem->pEvolutionData.reset();
	}

	char link[MAX_STRING] = { 0 };
	GetItemLink(pItem, link);
	return std::string(link);
}

static void ConvertItemsDotTxt()
{
	WriteChatf("MQ2LinkDB: Importing items.txt...");

	char szFilename[MAX_PATH];
	sprintf_s(szFilename, "%s\\items.txt", gPathResources);
	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szFilename, "rt");
	if (!err)
	{
		FILE* LinkFile = nullptr;
		err = fopen_s(&LinkFile, szLinkDBFileName, "wt");

		if (!err)
		{
			// reset this so we read it in next time.
			presentItemIDs.clear();
			bKnowTotal = false;

			WriteChatf("MQ2LinkDB: Generating links...");
			char szLine[MAX_STRING * 2] = { 0 };
			SODEQITEM SODEQItem;

			bool bFirst = true;
			int iCount = 0;

			// Set up our reusable item to fill with data for linking.
			ItemDefinitionPtr pItemDef = SoeUtil::MakeShared<ItemDefinition>();
			ItemPtr pItem = eqNew<ItemClient>();
			pItem->SetItemDefinition(pItemDef.get());

			while (fgets(szLine, MAX_STRING * 2, File) != nullptr)
			{
				szLine[strlen(szLine) - 1] = '\0';

				if (bFirst)
				{
					// quick sanity check on file
					int nCheck = strcnt(szLine, '|');
					if (nCheck != 294)
					{
						WriteChatf("MQ2LinkDB: \arInvalid items.txt file. \ay%d\ax fields found. Aborting", nCheck);
						break;
					}

					bFirst = false;
				}
				else
				{
					memset(&SODEQItem, 0, sizeof(SODEQItem));
					SODEQReadItem(&SODEQItem, szLine);

					if (SODEQItem.id)
					{
						std::string sLink = SODEQMakeLink(SODEQItem, pItem);

						//WriteChatf("Test ItemID[%d]: %d", SODEQItem.id, ItemID(cLink));

						fprintf(LinkFile, "%s\n", sLink.c_str());
						iCount++;
					}
				}
			}

			WriteChatf("MQ2LinkDB: Complete! \ay%d\ax links generated", iCount);
			DebugSpewAlways("MQ2LinkDB: Complete! \ay%d\ax links generated", iCount);

			fclose(LinkFile);
		}
		else
		{
			WriteChatf("MQ2LinkDB: \arCould not create link file (MQ2LinkDB.txt) (err: %d)", errno);
			DebugSpewAlways("MQ2LinkDB: \arCould not create link file (MQ2LinkDB.txt) (err: %d)", errno);
		}

		fclose(File);
	}
	else
	{
		WriteChatf("MQ2LinkDB: \arSource file not found (items.txt)");
		DebugSpewAlways("MQ2LinkDB: \arSource file not found (items.txt)");
	}
}
