#pragma once

#include<string>
#include <mq/Plugin.h>
#include "sqlite3.h"

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

	bool LoadItemLine(const char* szLine)
	{
		ReadItemLine(szLine);
		return IsValid();
	}

	std::string GetItemLink() const
	{
		if (IsValid())
			return CreateItemLink();
		return "";
	}

	std::string ConvertToItemLink(char* szLine)
	{
		LoadItemLine(szLine);
		return GetItemLink();
	}

	virtual std::string getSQLInsertRawItemStmt() const = 0;
	virtual bool execAddItemToRawDB(sqlite3* db) const = 0;
	virtual bool execAddItemToLinkDB(sqlite3* db) const = 0;
	static std::string getSQLCreateStmt()
	{
		return "";
	}

protected:
	void ReadItemLine(const char* szLine)
	{
		if (*szLine == 0)
			return;

		const char* ptr = szLine;
		int iField = 0;
		char szField[256];

		do
		{
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

			//WriteChatf("cField: %s", cField);
			SetField(iField++, szField);
		} while (*ptr++);
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

	virtual bool IsValid() const override;

	virtual void SetField(int iField, const char* cField) override;

	virtual void PopulateItem(ItemPtr pItem) const override;

	static std::string getSQLCreateStmt();

	virtual std::string getSQLInsertRawItemStmt() const override;

	virtual bool execAddItemToLinkDB(sqlite3* db) const override;
	virtual bool execAddItemToRawDB(sqlite3* db) const override;
};

#pragma endregion

// SODEQ converter - 315 columns in the items.txt file.
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

	virtual void SetField(int iField, const char* cField) override;

	virtual void PopulateItem(ItemPtr pItem) const override;

	static std::string getSQLCreateStmt();

	virtual std::string getSQLInsertRawItemStmt() const override;

	virtual bool execAddItemToLinkDB(sqlite3* db) const override;
	virtual bool execAddItemToRawDB(sqlite3* db) const override;
};
