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
static int iMaxResults = 10;                 // Display at most this many results
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
			fseek(File2, (long)(replacePos - strlen(szLine) - 2), SEEK_SET);

			// Double check position - paranoia!
			char szTemp[10];
			fread(szTemp, 10, 1, File2);
			if (memcmp(szTemp, link.data(), 8) == 0)
			{
				fseek(File2, (long)(replacePos - strlen(szLine) - 2), SEEK_SET); // Seek same place again
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
		sprintf_s(szCommand, "/notify ChatWindow CW_ChatOutput link %.*s", (int)linkData.length(), linkData.data());

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
		if (iItemID == 0)
		{
			continue;
		}
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
	sprintf_s(szTemp3, "MQ2LinkDB: Found \ay%d\ax items from database of \ay%d\ax total items", (int)results.size(), (int)presentItemIDs.size());
	sprintf_s(szTemp, "Found %d items from database of %d total items", (int)results.size(), (int)presentItemIDs.size());

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

//============================================================================

class SODEQItemConverter
{
public:
	// item ptr used for creating an item link using eq code.
	ItemPtr m_item;

	SODEQItemConverter()
	{
		// Set up our reusable item to fill with data for linking.
		ItemDefinitionPtr pItemDef = SoeUtil::MakeShared<ItemDefinition>();
		m_item = eqNew<ItemClient>();
		m_item->SetItemDefinition(pItemDef.get());
	}

	virtual ~SODEQItemConverter()
	{
	}

	std::string ConvertToItemLink(char* szLine)
	{
		std::string output;

		ReadItemLine(szLine);
		if (IsValid())
		{
			output = CreateItemLink();
		}

		return output;
	}

protected:
	void ReadItemLine(char* szLine)
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
			SetField(iField++, szField);
		}
	}

	std::string CreateItemLink() const
	{
		PopulateItem(m_item);

		char link[MAX_STRING] = { 0 };
		FormatItemLink(link, MAX_STRING, m_item.get());
		return std::string(link);
	}

	virtual void SetField(int fieldNum, const char* fieldText) = 0;
	virtual void PopulateItem(ItemPtr pItem) const = 0;
	virtual bool IsValid() const = 0;
};

#pragma region SODEQ Item Converter - 295 columns

// SODEQ converter - 295 columns
class SODEQItemConverter295 : public SODEQItemConverter
{
public:
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

	virtual bool IsValid() const override
	{
		return id != 0;
	}

	virtual void SetField(int iField, const char* cField) override
	{
		int lValue = atol(cField);
		double dValue = atof(cField);
		bool bValue = lValue > 0;

		switch (iField) {
		case   0: itemclass = lValue; break;
		case   1: strcpy_s(name, cField); break;
		case   2: strcpy_s(lore, cField); break;
		case   3: strcpy_s(idfile, cField); break;
		case   4: strcpy_s(lorefile, cField); break;
		case   5: id = lValue; break;
		case   6: weight = lValue; break;
		case   7: norent = bValue; break;
		case   8: nodrop = bValue; break;
		case   9: attuneable = bValue; break;
		case  10: size = lValue; break;
		case  11: slots = lValue; break;
		case  12: price = lValue; break;
		case  13: icon = lValue; break;
		case  14: benefitflag = lValue; break;
		case  15: tradeskills = bValue; break;
		case  16: cr = lValue; break;
		case  17: dr = lValue; break;
		case  18: pr = lValue; break;
		case  19: mr = lValue; break;
		case  20: fr = lValue; break;
		case  21: svcorruption = lValue; break;
		case  22: astr = lValue; break;
		case  23: asta = lValue; break;
		case  24: aagi = lValue; break;
		case  25: adex = lValue; break;
		case  26: acha = lValue; break;
		case  27: aint = lValue; break;
		case  28: awis = lValue; break;
		case  29: hp = lValue; break;
		case  30: mana = lValue; break;
		case  31: endur = lValue; break;
		case  32: ac = lValue; break;
		case  33: regen = lValue; break;
		case  34: manaregen = lValue; break;
		case  35: enduranceregen = lValue; break;
		case  36: classes = lValue; break;
		case  37: races = lValue; break;
		case  38: deity = lValue; break;
		case  39: skillmodvalue = lValue; break;
		case  40: skillmodmax = lValue; break;
		case  41: skillmodtype = lValue; break;
		case  42: skillmodextra = lValue; break;
		case  43: banedmgrace = lValue; break;
		case  44: banedmgbody = lValue; break;
		case  45: banedmgraceamt = lValue; break;
		case  46: banedmgamt = lValue; break;
		case  47: magic = lValue; break;
		case  48: foodduration = lValue; break;
		case  49: reqlevel = lValue; break;
		case  50: reclevel = lValue; break;
		case  51: recskill = lValue; break;
		case  52: bardtype = lValue; break;
		case  53: bardvalue = lValue; break;
		case  54: unk002 = lValue; break;
		case  55: unk003 = lValue; break;
		case  56: unk004 = lValue; break;
		case  57: light = lValue; break;
		case  58: delay = lValue; break;
		case  59: elemdmgtype = lValue; break;
		case  60: elemdmgamt = lValue; break;
		case  61: range = lValue; break;
		case  62: damage = lValue; break;
		case  63: color = lValue; break;
		case  64: prestige = lValue; break;
		case  65: unk006 = lValue; break;
		case  66: unk007 = lValue; break;
		case  67: unk008 = lValue; break;
		case  68: itemtype = lValue; break;
		case  69: material = lValue; break;
		case  70: materialunk1 = lValue; break;
		case  71: elitematerial = lValue; break;
		case  72: heroforge1 = lValue; break;
		case  73: heroforge2 = lValue; break;
		case  74: sellrate = lValue; break;
		case  75: extradmgskill = lValue; break;
		case  76: extradmgamt = lValue; break;
		case  77: charmfileid = lValue; break;
		case  78: factionmod1 = lValue; break;
		case  79: factionamt1 = lValue; break;
		case  80: factionmod2 = lValue; break;
		case  81: factionamt2 = lValue; break;
		case  82: factionmod3 = lValue; break;
		case  83: factionamt3 = lValue; break;
		case  84: factionmod4 = lValue; break;
		case  85: factionamt4 = lValue; break;
		case  86: strcpy_s(charmfile, cField); break;
		case  87: augtype = lValue; break;
		case  88: augstricthidden = lValue; break;
		case  89: augrestrict = lValue; break;
		case  90: augslot1type = lValue; break;
		case  91: augslot1visible = lValue; break;
		case  92: augslot1unk2 = lValue; break;
		case  93: augslot2type = lValue; break;
		case  94: augslot2visible = lValue; break;
		case  95: augslot2unk2 = lValue; break;
		case  96: augslot3type = lValue; break;
		case  97: augslot3visible = lValue; break;
		case  98: augslot3unk2 = lValue; break;
		case  99: augslot4type = lValue; break;
		case 100: augslot4visible = lValue; break;
		case 101: augslot4unk2 = lValue; break;
		case 102: augslot5type = lValue; break;
		case 103: augslot5visible = lValue; break;
		case 104: augslot5unk2 = lValue; break;
		case 105: augslot6type = lValue; break;
		case 106: augslot6visible = lValue; break;
		case 107: augslot6unk2 = lValue; break;
		case 108: pointtype = lValue; break;
		case 109: ldontheme = lValue; break;
		case 110: ldonprice = lValue; break;
		case 111: ldonsellbackrate = lValue; break;
		case 112: ldonsold = lValue; break;
		case 113: bagtype = lValue; break;
		case 114: bagslots = lValue; break;
		case 115: bagsize = lValue; break;
		case 116: bagwr = lValue; break;
		case 117: booktype = lValue; break;
		case 118: booklang = lValue; break;
		case 119: strcpy_s(filename, cField); break;
		case 120: loregroup = lValue; break;
		case 121: artifactflag = bValue; break;
		case 122: summoned = bValue; break;
		case 123: favor = lValue; break;
		case 124: fvnodrop = bValue; break;
		case 125: attack = lValue; break;
		case 126: haste = lValue; break;
		case 127: guildfavor = lValue; break;
		case 128: augdistiller = lValue; break;
		case 129: unk009 = lValue; break;
		case 130: unk010 = lValue; break;
		case 131: nopet = lValue; break;
		case 132: unk011 = lValue; break;
		case 133: stacksize = lValue; break;
		case 134: notransfer = bValue; break;
		case 135: expendablearrow = lValue; break;
		case 136: unk012 = lValue; break;
		case 137: unk013 = lValue; break;
		case 138: clickeffect = lValue; break;
		case 139: clicklevel2 = lValue; break;
		case 140: clicktype = lValue; break;
		case 141: clicklevel = lValue; break;
		case 142: clickmaxcharges = lValue; break;
		case 143: clickcasttime = lValue; break;
		case 144: clickrecastdelay = lValue; break;
		case 145: clickrecasttype = lValue; break;
		case 146: clickunk5 = lValue; break;
		case 147: clickname = lValue; break;
		case 148: clickunk7 = lValue; break;
		case 149: proceffect = lValue; break;
		case 150: proclevel2 = lValue; break;
		case 151: proctype = lValue; break;
		case 152: proclevel = lValue; break;
		case 153: procunk1 = lValue; break;
		case 154: procunk2 = lValue; break;
		case 155: procunk3 = lValue; break;
		case 156: procunk4 = lValue; break;
		case 157: procrate = lValue; break;
		case 158: procname = lValue; break;
		case 159: procunk7 = lValue; break;
		case 160: worneffect = lValue; break;
		case 161: wornlevel2 = lValue; break;
		case 162: worntype = lValue; break;
		case 163: wornlevel = lValue; break;
		case 164: wornunk1 = lValue; break;
		case 165: wornunk2 = lValue; break;
		case 166: wornunk3 = lValue; break;
		case 167: wornunk4 = lValue; break;
		case 168: wornunk5 = lValue; break;
		case 169: wornname = lValue; break;
		case 170: wornunk7 = lValue; break;
		case 171: focuseffect = lValue; break;
		case 172: focuslevel2 = lValue; break;
		case 173: focustype = lValue; break;
		case 174: focuslevel = lValue; break;
		case 175: focusunk1 = lValue; break;
		case 176: focusunk2 = lValue; break;
		case 177: focusunk3 = lValue; break;
		case 178: focusunk4 = lValue; break;
		case 179: focusunk5 = lValue; break;
		case 180: focusname = lValue; break;
		case 181: focusunk7 = lValue; break;
		case 182: scrolleffect = lValue; break;
		case 183: scrolllevel2 = lValue; break;
		case 184: scrolltype = lValue; break;
		case 185: scrolllevel = lValue; break;
		case 186: scrollunk1 = lValue; break;
		case 187: scrollunk2 = lValue; break;
		case 188: scrollunk3 = lValue; break;
		case 189: scrollunk4 = lValue; break;
		case 190: scrollunk5 = lValue; break;
		case 191: scrollname = lValue; break;
		case 192: scrollunk7 = lValue; break;
		case 193: bardeffect = lValue; break;
		case 194: bardlevel2 = lValue; break;
		case 195: bardeffecttype = lValue; break;
		case 196: bardlevel = lValue; break;
		case 197: bardunk1 = lValue; break;
		case 198: bardunk2 = lValue; break;
		case 199: bardunk3 = lValue; break;
		case 200: bardunk4 = lValue; break;
		case 201: bardunk5 = lValue; break;
		case 202: bardname = lValue; break;
		case 203: bardunk7 = lValue; break;
		case 204: strcpy_s(unk014, cField); break;
		case 205: strcpy_s(unk015, cField); break;
		case 206: strcpy_s(unk016, cField); break;
		case 207: unk017 = lValue; break;
		case 208: unk018 = lValue; break;
		case 209: unk019 = lValue; break;
		case 210: unk020 = lValue; break;
		case 211: unk021 = lValue; break;
		case 212: unk022 = lValue; break;
		case 213: scriptfile = lValue; break;
		case 214: questitemflag = bValue; break;
		case 215: powersourcecapacity = lValue; break;
		case 216: purity = lValue; break;
		case 217: epic = lValue; break;
		case 218: backstabdmg = lValue; break;
		case 219: heroic_str = lValue; break;
		case 220: heroic_int = lValue; break;
		case 221: heroic_wis = lValue; break;
		case 222: heroic_agi = lValue; break;
		case 223: heroic_dex = lValue; break;
		case 224: heroic_sta = lValue; break;
		case 225: heroic_cha = lValue; break;
		case 226: unk029 = lValue; break;
		case 227: healamt = lValue; break;
		case 228: spelldmg = lValue; break;
		case 229: clairvoyance = lValue; break;
		case 230: unk030 = lValue; break;
		case 231: unk031 = lValue; break;
		case 232: unk032 = lValue; break;
		case 233: unk033 = lValue; break;
		case 234: unk034 = lValue; break;
		case 235: unk035 = lValue; break;
		case 236: unk036 = lValue; break;
		case 237: unk037 = lValue; break;
		case 238: heirloom = bValue; break;
		case 239: placeable = lValue; break;
		case 240: unk038 = lValue; break;
		case 241: unk039 = lValue; break;
		case 242: unk040 = lValue; break;
		case 243: unk041 = static_cast<long>(dValue * 100); break;
		case 244: unk042 = static_cast<long>(dValue * 100); break;
		case 245: unk043 = lValue; break;
		case 246: unk044 = lValue; break;
		case 247: strcpy_s(placeablenpcname, cField); break;
		case 248: unk046 = lValue; break;
		case 249: unk047 = lValue; break;
		case 250: unk048 = lValue; break;
		case 251: unk049 = lValue; break;
		case 252: unk050 = static_cast<long>(dValue * 100); break;
		case 253: unk051 = lValue; break;
		case 254: unk052 = lValue; break;
		case 255: unk053 = lValue; break;
		case 256: unk054 = lValue; break;
		case 257: unk055 = lValue; break;
		case 258: unk056 = lValue; break;
		case 259: unk057 = lValue; break;
		case 260: unk058 = lValue; break;
		case 261: unk059 = lValue; break;
		case 262: unk060 = lValue; break;
		case 263: unk061 = lValue; break;
		case 264: unk062 = lValue; break;
		case 265: strcpy_s(unk063, cField); break;
		case 266: collectible = bValue; break;
		case 267: nodestroy = bValue; break;
		case 268: nonpc = bValue; break;
		case 269: nozone = bValue; break;
		case 270: unk068 = lValue; break;
		case 271: unk069 = lValue; break;
		case 272: unk070 = lValue; break;
		case 273: unk071 = lValue; break;
		case 274: noground = lValue; break;
		case 275: unk073 = lValue; break;
		case 276: marketplace = lValue; break;
		case 277: freestorage = lValue; break;
		case 278: unk076 = lValue; break;
		case 279: unk077 = lValue; break;
		case 280: unk078 = lValue; break;
		case 281: unk079 = lValue; break;
		case 282: evolving = bValue; break;
		case 283: evoid = lValue; break;
		case 284: evolvinglevel = lValue; break;
		case 285: evomax = lValue; break;
		case 286: convertable = lValue; break;
		case 287: convertid = lValue; break;
		case 288: strcpy_s(convertname, cField); break;
		case 289: updated = lValue; break;
		case 290: created = lValue; break;
		case 291: strcpy_s(submitter, cField); break;
		case 292: verified = lValue; break;
		case 293: strcpy_s(verifiedby, cField); break;
		case 294: strcpy_s(collectversion, cField); break;
		}
	}

	virtual void PopulateItem(ItemPtr pItem) const override
	{
		ItemDefinition* pItemDef = pItem->GetItemDefinition();

		strcpy_s(pItemDef->Name, name);
		strcpy_s(pItemDef->LoreName, lore);
		pItemDef->ItemNumber = id;
		pItemDef->EquipSlots = slots;
		pItemDef->Cost = price;
		pItemDef->IconNumber = icon;
		pItemDef->Weight = weight;
		pItemDef->NoRent = norent;
		pItemDef->IsDroppable = notransfer;
		pItemDef->Attuneable = attuneable;
		pItemDef->Heirloom = heirloom;
		pItemDef->Collectible = collectible;
		pItemDef->NoDestroy = nodestroy;
		pItemDef->Size = static_cast<uint8_t>(size);
		pItemDef->Type = itemclass;
		pItemDef->TradeSkills = tradeskills;
		pItemDef->Lore = (loregroup == 0 ? 0 : 1);
		pItemDef->Artifact = artifactflag;
		pItemDef->Summoned = summoned;
		pItemDef->SvCold = static_cast<char>(cr);
		pItemDef->SvFire = static_cast<char>(fr);
		pItemDef->SvMagic = static_cast<char>(mr);
		pItemDef->SvDisease = static_cast<char>(dr);
		pItemDef->SvPoison = static_cast<char>(pr);
		pItemDef->SvCorruption = static_cast<char>(svcorruption);
		pItemDef->STR = static_cast<char>(astr);
		pItemDef->STA = static_cast<char>(asta);
		pItemDef->AGI = static_cast<char>(aagi);
		pItemDef->DEX = static_cast<char>(adex);
		pItemDef->CHA = static_cast<char>(acha);
		pItemDef->INT = static_cast<char>(aint);
		pItemDef->WIS = static_cast<char>(awis);
		pItemDef->HP = hp;
		pItemDef->Mana = mana;
		pItemDef->AC = ac;
		pItemDef->RequiredLevel = reqlevel;
		pItemDef->RecommendedLevel = reclevel;
//		pItemDef->RecommendedSkill = recskill;
		pItemDef->SkillModType = skillmodtype;
		pItemDef->SkillModValue = skillmodvalue;
		pItemDef->SkillModMax = skillmodmax;
		pItemDef->SkillModBonus = skillmodextra;
		pItemDef->BaneDMGRace = banedmgrace;
		pItemDef->BaneDMGBodyType = banedmgbody;
		pItemDef->BaneDMGBodyTypeValue = banedmgamt;
		pItemDef->BaneDMGRaceValue = banedmgraceamt;
		pItemDef->InstrumentType = bardtype;
		pItemDef->InstrumentMod = bardvalue;
		pItemDef->Classes = classes;
		pItemDef->Races = races;
		pItemDef->Deity = deity;
		pItemDef->Magic = magic;
		pItemDef->Light = static_cast<uint8_t>(light);
		pItemDef->Delay = static_cast<uint8_t>(delay);
		pItemDef->ElementalFlag = static_cast<uint8_t>(elemdmgtype);
		pItemDef->ElementalDamage = static_cast<uint8_t>(elemdmgamt);
		pItemDef->Range = static_cast<uint8_t>(range);
		pItemDef->Damage = damage;
		pItemDef->BackstabDamage = backstabdmg;
		pItemDef->HeroicSTR = heroic_str;
		pItemDef->HeroicINT = heroic_int;
		pItemDef->HeroicWIS = heroic_wis;
		pItemDef->HeroicAGI = heroic_agi;
		pItemDef->HeroicDEX = heroic_dex;
		pItemDef->HeroicSTA = heroic_sta;
		pItemDef->HeroicCHA = heroic_cha;
		pItemDef->HealAmount = healamt;
		pItemDef->SpellDamage = spelldmg;
		pItemDef->Prestige = prestige;
		pItemDef->ItemClass = static_cast<uint8_t>(itemtype);
		pItemDef->ArmorProps.material = material;
		pItemDef->AugData.Sockets[0].Type = augslot1type;
		pItemDef->AugData.Sockets[0].bVisible = augslot1unk2;
		pItemDef->AugData.Sockets[1].Type = augslot2type;
		pItemDef->AugData.Sockets[1].bVisible = augslot2unk2;
		pItemDef->AugData.Sockets[2].Type = augslot3type;
		pItemDef->AugData.Sockets[2].bVisible = augslot3unk2;
		pItemDef->AugData.Sockets[3].Type = augslot4type;
		pItemDef->AugData.Sockets[3].bVisible = augslot4unk2;
		pItemDef->AugData.Sockets[4].Type = augslot5type;
		pItemDef->AugData.Sockets[4].bVisible = augslot5unk2;
		pItemDef->AugData.Sockets[5].Type = augslot6type;
		pItemDef->AugData.Sockets[5].bVisible = augslot6unk2;
		pItemDef->AugType = augtype;
		pItemDef->AugRestrictions = augrestrict;
		pItemDef->SolventItemID = augdistiller;
		pItemDef->LDTheme = ldontheme;
		pItemDef->LDCost = ldonprice;
		strcpy_s(pItemDef->CharmFile, charmfile);
		pItemDef->DmgBonusSkill = extradmgskill;
		pItemDef->DmgBonusValue = extradmgamt;
//		pItemDef->CharmFileID = charmfileid;
		pItemDef->FoodDuration = foodduration;
		pItemDef->ContainerType = static_cast<uint8_t>(bagtype);
		pItemDef->Slots = static_cast<uint8_t>(bagslots);
		pItemDef->SizeCapacity = static_cast<uint8_t>(bagsize);
		pItemDef->WeightReduction = static_cast<uint8_t>(bagwr);
		pItemDef->BookType = static_cast<uint8_t>(booktype);
		pItemDef->BookLang = static_cast<int8_t>(booklang);
		strcpy_s(pItemDef->BookFile, filename);
		pItemDef->Favor = favor;
		pItemDef->GuildFavor = guildfavor;
		pItemDef->bIsFVNoDrop = fvnodrop;
		pItemDef->Endurance = endur;
		pItemDef->Attack = attack;
		pItemDef->HPRegen = regen;
		pItemDef->ManaRegen = manaregen;
		pItemDef->EnduranceRegen = enduranceregen;
		pItemDef->Haste = haste;
		pItemDef->bNoPetGive = nopet;
		pItemDef->StackSize = stacksize;
		pItemDef->MaxPower = powersourcecapacity;
		pItemDef->Purity = purity;
		pItemDef->QuestItem = questitemflag;
		pItemDef->Expendable = expendablearrow;
		pItemDef->Clairvoyance = clairvoyance;
		pItemDef->Placeable = placeable;
#if defined(ROF2EMU) || defined(UFEMU)
		pItemDef->FactionModType[0] = factionmod1;
		pItemDef->FactionModType[1] = factionmod2;
		pItemDef->FactionModType[2] = factionmod3;
		pItemDef->FactionModType[3] = factionmod4;
		pItemDef->FactionModValue[0] = factionamt1;
		pItemDef->FactionModValue[1] = factionamt2;
		pItemDef->FactionModValue[2] = factionamt3;
		pItemDef->FactionModValue[3] = factionamt4;
		strcpy_s(pItemDef->IDFile, idfile);
#else
		pItemDef->bNoNPC = nonpc;
		pItemDef->LoreEquipped = 1;
#endif

		bool IsEvolvingItem = evoid > 0 && evoid < 10000;
		if (IsEvolvingItem)
		{
			pItem->pEvolutionData = SoeUtil::MakeShared<ItemEvolutionData>();
			pItem->pEvolutionData->EvolvingMaxLevel = evomax;
			pItem->pEvolutionData->EvolvingExpPct = 0;
			pItem->pEvolutionData->EvolvingCurrentLevel = (IsEvolvingItem ? evolvinglevel : 0);
			pItem->pEvolutionData->GroupID = (IsEvolvingItem ? evoid : (loregroup > 0) ? loregroup & 0xFFFF : 0);
			pItem->pEvolutionData->LastEquipped = 0;
		}
		else
		{
			pItem->pEvolutionData.reset();
		}
	}
};

#pragma endregion

class SODEQItemConverter315 : public SODEQItemConverter295
{
public:
	SODEQItemConverter315()
	{}

	~SODEQItemConverter315()
	{}

	// new/changed fields since previous format
	int idfileextra;                      // 4   lorefile -> idfileextra
	int mounteffect;                      // 78  factionmod1 -> mounteffect
	int mountlevel2;                      // 79  factionamt1 -> mountlevel2
	int mounteffecttype;                  // 80  factionmod2 -> mounteffecttype
	int mountlevel;                       // 81  factionamt2 -> mountlevel
	int mountunk1;                        // 82  factionmod3 -> mountunk1
	int mountunk2;                        // 83  factionamt3 -> mountunk2
	int mountunk3;                        // 84  factionmod4 -> mountunk3
	int mountunk4;                        // 85  factionamt4 -> mountunk4
	int mountunk5;                        // 204 UNKNOWN14 -> mountunk5
	char mountname[0x40];                 // 207 UNKNOWN17 -> mountname
	int mountunk7;                        // 208 UNKNOWN18 -> mountunk7
	int blessinglevel2;                   // 209 UNKNOWN19 -> blessinglevel2
	int blessingeffecttype;               // 210 UNKNOWN20 -> blessingeffecttype
	int blessinglevel;                    // 211 UNKNOWN21 -> blessinglevel
	int blessingunk1;                     // 212 UNKNOWN22 -> blessingunk1
	int blessingunk2;                     // 282 blessingunk2
	int blessingunk3;                     // 283 blessingunk3
	int blessingunk4;                     // 284 blessingunk4
	int blessingunk5;                     // 285 blessingunk5
	int blessingunk7;                     // 286 blessingunk7
	int familiareffect;                   // 287 familiareffect
	int familiarlevel2;                   // 288 familiarlevel2
	int familiareffecttype;               // 289 familiareffecttype
	int familiarlevel;                    // 290 familiarlevel
	int familiarunk1;                     // 291 familiarunk1
	int familiarunk2;                     // 292 familiarunk2
	int familiarunk3;                     // 293 familiarunk3
	int familiarunk4;                     // 294 familiarunk4
	int familiarunk5;                     // 295 familiarunk5
	char familiarname[0x40];              // 296 familiarname
	int familiarunk7;                     // 297 familiarunk7
	int unk80;                            // 298 UNKNOWN80
	int minluck;                          // 299 minluck
	int maxluck;                          // 300 maxluck
	char loreequippedgroup[0x40];         // 301 loreequippedgroup

	virtual void SetField(int iField, const char* cField) override
	{
		int lValue = atol(cField);
		double dValue = atof(cField);
		bool bValue = lValue > 0;

		switch (iField)
		{
		case 4: idfileextra = lValue; break;
		case 78: mounteffect = lValue; break;
		case 79: mountlevel2 = lValue; break;
		case 80: mounteffecttype = lValue; break;
		case 81: mountlevel = lValue; break;
		case 82: mountunk1 = lValue; break;
		case 83: mountunk2 = lValue; break;
		case 84: mountunk3 = lValue; break;
		case 85: mountunk4 = lValue; break;
		case 204: mountunk5 = lValue; break;
		case 207: strcpy_s(mountname, cField); break;
		case 208: mountunk7 = lValue; break;
		case 209: blessinglevel2 = lValue; break;
		case 210: blessingeffecttype = lValue; break;
		case 211: blessinglevel = lValue; break;
		case 212: blessingunk1 = lValue; break;
		case 282: blessingunk2 = lValue; break;
		case 283: blessingunk3 = lValue; break;
		case 284: blessingunk4 = lValue; break;
		case 285: blessingunk5 = lValue; break;
		case 286: blessingunk7 = lValue; break;
		case 287: familiareffect = lValue; break;
		case 288: familiarlevel2 = lValue; break;
		case 289: familiareffecttype = lValue; break;
		case 290: familiarlevel = lValue; break;
		case 291: familiarunk1 = lValue; break;
		case 292: familiarunk2 = lValue; break;
		case 293: familiarunk3 = lValue; break;
		case 294: familiarunk4 = lValue; break;
		case 295: familiarunk5 = lValue; break;
		case 296: strcpy_s(familiarname, cField); break;
		case 297: familiarunk7 = lValue; break;
		case 298: unk80 = lValue; break;
		case 299: minluck = lValue; break;
		case 300: maxluck = lValue; break;
		case 301: strcpy_s(loreequippedgroup, cField); break;
		default:
			// 302 -> 282 (-20)
			if (iField < 282)
				SODEQItemConverter295::SetField(iField, cField);
			else if (iField >= 302)
				SODEQItemConverter295::SetField(iField - 20, cField);
			break;
		}
	}

	virtual void PopulateItem(ItemPtr pItem) const override
	{
		SODEQItemConverter295::PopulateItem(pItem);

		ItemDefinition* pItemDef = pItem->GetItemDefinition();

		pItemDef->IDFile2 = idfileextra;
		pItemDef->MinLuck = minluck;
		pItemDef->MaxLuck = maxluck;
	}
};

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

static void ConvertItemsDotTxt()
{
	WriteChatf("MQ2LinkDB: Importing items.txt...");

	char szFilename[MAX_PATH];
	sprintf_s(szFilename, "%s\\items.txt", gPathResources);
	FILE* File = nullptr;
	errno_t err = fopen_s(&File, szFilename, "rt");
	if (err)
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

	FILE* LinkFile = nullptr;
	if (fopen_s(&LinkFile, szLinkDBFileName, "wt") != 0)
	{
		WriteChatf("MQ2LinkDB: \arCould not create link file (MQ2LinkDB.txt) (err: %d)", errno);
		DebugSpewAlways("MQ2LinkDB: \arCould not create link file (MQ2LinkDB.txt) (err: %d)", errno);

		fclose(File);
		return;
	}

	// reset this so we read it in next time.
	presentItemIDs.clear();
	bKnowTotal = false;

	WriteChatf("MQ2LinkDB: Generating links...");
	int iCount = 0;

	while (fgets(szLine, MAX_STRING * 2, File) != nullptr)
	{
		szLine[strlen(szLine) - 1] = '\0';

		std::string sLink = converter->ConvertToItemLink(szLine);

		if (!sLink.empty())
		{
			fprintf(LinkFile, "%s\n", sLink.c_str());
			iCount++;
		}
	}

	WriteChatf("MQ2LinkDB: Complete! \ay%d\ax links generated", iCount);
	DebugSpewAlways("MQ2LinkDB: Complete! \ay%d\ax links generated", iCount);

	fclose(LinkFile);
	fclose(File);
}
