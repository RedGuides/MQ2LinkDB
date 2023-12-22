#include "SODEQItemConverters.h"

#pragma region SODEQ Item Converter - 295 columns

// SODEQ converter - 295 columns
bool SODEQItemConverter295::IsValid() const
{
	return id != 0;
}

void SODEQItemConverter295::SetField(int iField, const char* cField) 
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

void SODEQItemConverter295::PopulateItem(ItemPtr pItem) const
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
#if !IS_CLIENT_DATE(20220414)
	pItemDef->RecommendedSkill = recskill;
#endif
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
#if !IS_CLIENT_DATE(20220414)
	pItemDef->CharmFileID = charmfileid;
#endif
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
		pItem->PopulateItemEvolutionData(evomax, evolvinglevel, evoid, 0, 0.0f);
	}
	else
	{
		pItem->ResetItemEvolutionData();
	}
}

std::string SODEQItemConverter295::getSQLCreateStmt()
{
	WriteChatf("MQ2LinkDB: Item Convert 295 has no DB support.");
	return "";
}

std::string SODEQItemConverter295::getSQLInsertRawItemStmt() const
{
	WriteChatf("MQ2LinkDB: Item Convert 295 has no DB support.");
	return "";
}

bool SODEQItemConverter295::execAddItemToLinkDB(sqlite3* db) const
{
	WriteChatf("MQ2LinkDB: Item Convert 295 has no DB support.");
	return false;
}

bool SODEQItemConverter295::execAddItemToRawDB(sqlite3* db) const
{
	WriteChatf("MQ2LinkDB: Item Convert 295 has no DB support.");
	return false;
}

#pragma endregion

void SODEQItemConverter315::SetField(int iField, const char* cField)
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

void SODEQItemConverter315::PopulateItem(ItemPtr pItem) const 
{
	SODEQItemConverter295::PopulateItem(pItem);

	ItemDefinition* pItemDef = pItem->GetItemDefinition();

#if IS_CLIENT_DATE(20200413)
	pItemDef->IDFile2 = idfileextra;
#endif
#if HAS_LUCK_STAT
	pItemDef->MinLuck = minluck;
	pItemDef->MaxLuck = maxluck;
#endif
}

std::string SODEQItemConverter315::getSQLCreateStmt()
{
	return std::string(R"SQL(
		PRAGMA journal_mode=WAL;
		CREATE TABLE IF NOT EXISTS raw_item_data_315(
			itemclass INT,
			name TEXT,
			lore TEXT,
			idfile TEXT,
			lorefile TEXT,
			id INT PRIMARY KEY,
			weight INT,
			norent INT,
			nodrop INT,
			attuneable INT,
			size INT,
			slots INT,
			price INT,
			icon INT,
			benefitflag INT,
			tradeskills INT,
			cr INT,
			dr INT,
			pr INT,
			mr INT,
			fr INT,
			svcorruption INT,
			astr INT,
			asta INT,
			aagi INT,
			adex INT,
			acha INT,
			aint INT,
			awis INT,
			hp INT,
			mana INT,
			endur INT,
			ac INT,
			regen INT,
			manaregen INT,
			enduranceregen INT,
			classes INT,
			races INT,
			deity INT,
			skillmodvalue INT,
			skillmodmax INT,
			skillmodtype INT,
			skillmodextra INT,
			banedmgrace INT,
			banedmgbody INT,
			banedmgraceamt INT,
			banedmgamt INT,
			magic INT,
			foodduration INT,
			reqlevel INT,
			reclevel INT,
			recskill INT,
			bardtype INT,
			bardvalue INT,
			unk002 INT,
			unk003 INT,
			unk004 INT,
			light INT,
			delay INT,
			elemdmgtype INT,
			elemdmgamt INT,
			range INT,
			damage INT,
			color INT,
			prestige INT,
			unk006 INT,
			unk007 INT,
			unk008 INT,
			itemtype INT,
			material INT,
			materialunk1 INT,
			elitematerial INT,
			heroforge1 INT,
			heroforge2 INT,
			sellrate INT,
			extradmgskill INT,
			extradmgamt INT,
			charmfileid INT,
			factionmod1 INT,
			factionamt1 INT,
			factionmod2 INT,
			factionamt2 INT,
			factionmod3 INT,
			factionamt3 INT,
			factionmod4 INT,
			factionamt4 INT,
			charmfile TEXT,
			augtype INT,
			augstricthidden INT,
			augrestrict INT,
			augslot1type INT,
			augslot1visible INT,
			augslot1unk2 INT,
			augslot2type INT,
			augslot2visible INT,
			augslot2unk2 INT,
			augslot3type INT,
			augslot3visible INT,
			augslot3unk2 INT,
			augslot4type INT,
			augslot4visible INT,
			augslot4unk2 INT,
			augslot5type INT,
			augslot5visible INT,
			augslot5unk2 INT,
			augslot6type INT,
			augslot6visible INT,
			augslot6unk2 INT,
			pointtype INT,
			ldontheme INT,
			ldonprice INT,
			ldonsellbackrate INT,
			ldonsold INT,
			bagtype INT,
			bagslots INT,
			bagsize INT,
			bagwr INT,
			booktype INT,
			booklang INT,
			filename TEXT,
			loregroup INT,
			artifactflag INT,
			summoned INT,
			favor INT,
			fvnodrop INT,
			attack INT,
			haste INT,
			guildfavor INT,
			augdistiller INT,
			unk009 INT,
			unk010 INT,
			nopet INT,
			unk011 INT,
			stacksize INT,
			notransfer INT,
			expendablearrow INT,
			unk012 INT,
			unk013 INT,
			clickeffect INT,
			clicklevel2 INT,
			clicktype INT,
			clicklevel INT,
			clickmaxcharges INT,
			clickcasttime INT,
			clickrecastdelay INT,
			clickrecasttype INT,
			clickunk5 INT,
			clickname INT,
			clickunk7 INT,
			proceffect INT,
			proclevel2 INT,
			proctype INT,
			proclevel INT,
			procunk1 INT,
			procunk2 INT,
			procunk3 INT,
			procunk4 INT,
			procrate INT,
			procname INT,
			procunk7 INT,
			worneffect INT,
			wornlevel2 INT,
			worntype INT,
			wornlevel INT,
			wornunk1 INT,
			wornunk2 INT,
			wornunk3 INT,
			wornunk4 INT,
			wornunk5 INT,
			wornname INT,
			wornunk7 INT,
			focuseffect INT,
			focuslevel2 INT,
			focustype INT,
			focuslevel INT,
			focusunk1 INT,
			focusunk2 INT,
			focusunk3 INT,
			focusunk4 INT,
			focusunk5 INT,
			focusname INT,
			focusunk7 INT,
			scrolleffect INT,
			scrolllevel2 INT,
			scrolltype INT,
			scrolllevel INT,
			scrollunk1 INT,
			scrollunk2 INT,
			scrollunk3 INT,
			scrollunk4 INT,
			scrollunk5 INT,
			scrollname INT,
			scrollunk7 INT,
			bardeffect INT,
			bardlevel2 INT,
			bardeffecttype INT,
			bardlevel INT,
			bardunk1 INT,
			bardunk2 INT,
			bardunk3 INT,
			bardunk4 INT,
			bardunk5 INT,
			bardname INT,
			bardunk7 INT,
			unk014 TEXT,
			unk015 TEXT,
			unk016 TEXT,
			unk017 INT,
			unk018 INT,
			unk019 INT,
			unk020 INT,
			unk021 INT,
			unk022 INT,
			scriptfile INT,
			questitemflag INT,
			powersourcecapacity INT,
			purity INT,
			epic INT,
			backstabdmg INT,
			heroic_str INT,
			heroic_int INT,
			heroic_wis INT,
			heroic_agi INT,
			heroic_dex INT,
			heroic_sta INT,
			heroic_cha INT,
			unk029 INT,
			healamt INT,
			spelldmg INT,
			clairvoyance INT,
			unk030 INT,
			unk031 INT,
			unk032 INT,
			unk033 INT,
			unk034 INT,
			unk035 INT,
			unk036 INT,
			unk037 INT,
			heirloom INT,
			placeable INT,
			unk038 INT,
			unk039 INT,
			unk040 INT,
			unk041 INT,
			unk042 INT,
			unk043 INT,
			unk044 INT,
			placeablenpcname TEXT,
			unk046 INT,
			unk047 INT,
			unk048 INT,
			unk049 INT,
			unk050 INT,
			unk051 INT,
			unk052 INT,
			unk053 INT,
			unk054 INT,
			unk055 INT,
			unk056 INT,
			unk057 INT,
			unk058 INT,
			unk059 INT,
			unk060 INT,
			unk061 INT,
			unk062 INT,
			unk063 TEXT,
			collectible INT,
			nodestroy INT,
			nonpc INT,
			nozone INT,
			unk068 INT,
			unk069 INT,
			unk070 INT,
			unk071 INT,
			noground INT,
			unk073 INT,
			marketplace INT,
			freestorage INT,
			unk076 INT,
			unk077 INT,
			unk078 INT,
			unk079 INT,
			evolving INT,
			evoid INT,
			evolvinglevel INT,
			evomax INT,
			convertable INT,
			convertid INT,
			convertname TEXT,
			updated INT,
			created INT,
			submitter TEXT,
			verified INT,
			verifiedby TEXT,
			collectversion TEXT,
			idfileextra INT,
			mounteffect INT,
			mountlevel2 INT,
			mounteffecttype INT,
			mountlevel INT,
			mountunk1 INT,
			mountunk2 INT,
			mountunk3 INT,
			mountunk4 INT,
			mountunk5 INT,
			mountname TEXT,
			mountunk7 INT,
			blessinglevel2 INT,
			blessingeffecttype INT,
			blessinglevel INT,
			blessingunk1 INT,
			blessingunk2 INT,
			blessingunk3 INT,
			blessingunk4 INT,
			blessingunk5 INT,
			blessingunk7 INT,
			familiareffect INT,
			familiarlevel2 INT,
			familiareffecttype INT,
			familiarlevel INT,
			familiarunk1 INT,
			familiarunk2 INT,
			familiarunk3 INT,
			familiarunk4 INT,
			familiarunk5 INT,
			familiarname TEXT,
			familiarunk7 INT,
			unk80 INT,
			minluck INT,
			maxluck INT,
			loreequippedgroup TEXT
		);
		CREATE INDEX IF NOT EXISTS `idx_raw_item_name` ON `raw_item_data_315` (`name` ASC);
		CREATE TABLE IF NOT EXISTS item_links( item_id INTEGER PRIMARY KEY NOT NULL, link TEXT );)SQL");
}

bool SODEQItemConverter315::execAddItemToLinkDB(sqlite3* db) const
{
	if (db)
	{
		sqlite3_stmt* stmt;
		std::string query("INSERT OR REPLACE INTO item_links (item_id, link) VALUES (?, ?);");
		if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		{
			WriteChatf("MQ2LinkDB: Error preparing query for item_link insertion: %s", sqlite3_errmsg(db));
			return false;
		}
		else
		{
			sqlite3_bind_int(stmt, 1, id);
			std::string link = GetItemLink();
			sqlite3_bind_text(stmt, 2, link.c_str(), -1, SQLITE_STATIC);

			if (sqlite3_step(stmt) != SQLITE_DONE)
			{
				WriteChatf("\arMQ2LinkDB: Error inserting into item_link table: %s", sqlite3_errmsg(db));
				return false;
			}

			sqlite3_finalize(stmt);

			return true;
		}
	}

	return false;
}

std::string SODEQItemConverter315::getSQLInsertRawItemStmt() const
{
	return std::string("INSERT OR REPLACE INTO raw_item_data_315 (itemclass, name, lore, idfile, lorefile, id, weight, norent, nodrop, attuneable, size, slots, price, icon, benefitflag, tradeskills, cr, dr, pr, mr, fr, svcorruption, astr, asta, aagi, adex, acha, aint, awis, hp, mana, endur, ac, regen, manaregen, enduranceregen, classes, races, deity, skillmodvalue, skillmodmax, skillmodtype, skillmodextra, banedmgrace, banedmgbody, banedmgraceamt, banedmgamt, magic, foodduration, reqlevel, reclevel, recskill, bardtype, bardvalue, unk002, unk003, unk004, light, delay, elemdmgtype, elemdmgamt, range, damage, color, prestige, unk006, unk007, unk008, itemtype, material, materialunk1, elitematerial, heroforge1, heroforge2, sellrate, extradmgskill, extradmgamt, charmfileid, factionmod1, factionamt1, factionmod2, factionamt2, factionmod3, factionamt3, factionmod4, factionamt4, charmfile, augtype, augstricthidden, augrestrict, augslot1type, augslot1visible, augslot1unk2, augslot2type, augslot2visible, augslot2unk2, augslot3type, augslot3visible, augslot3unk2, augslot4type, augslot4visible, augslot4unk2, augslot5type, augslot5visible, augslot5unk2, augslot6type, augslot6visible, augslot6unk2, pointtype, ldontheme, ldonprice, ldonsellbackrate, ldonsold, bagtype, bagslots, bagsize, bagwr, booktype, booklang, filename, loregroup, artifactflag, summoned, favor, fvnodrop, attack, haste, guildfavor, augdistiller, unk009, unk010, nopet, unk011, stacksize, notransfer, expendablearrow, unk012, unk013, clickeffect, clicklevel2, clicktype, clicklevel, clickmaxcharges, clickcasttime, clickrecastdelay, clickrecasttype, clickunk5, clickname, clickunk7, proceffect, proclevel2, proctype, proclevel, procunk1, procunk2, procunk3, procunk4, procrate, procname, procunk7, worneffect, wornlevel2, worntype, wornlevel, wornunk1, wornunk2, wornunk3, wornunk4, wornunk5, wornname, wornunk7, focuseffect, focuslevel2, focustype, focuslevel, focusunk1, focusunk2, focusunk3, focusunk4, focusunk5, focusname, focusunk7, scrolleffect, scrolllevel2, scrolltype, scrolllevel, scrollunk1, scrollunk2, scrollunk3, scrollunk4, scrollunk5, scrollname, scrollunk7, bardeffect, bardlevel2, bardeffecttype, bardlevel, bardunk1, bardunk2, bardunk3, bardunk4, bardunk5, bardname, bardunk7, unk014, unk015, unk016, unk017, unk018, unk019, unk020, unk021, unk022, scriptfile, questitemflag, powersourcecapacity, purity, epic, backstabdmg, heroic_str, heroic_int, heroic_wis, heroic_agi, heroic_dex, heroic_sta, heroic_cha, unk029, healamt, spelldmg, clairvoyance, unk030, unk031, unk032, unk033, unk034, unk035, unk036, unk037, heirloom, placeable, unk038, unk039, unk040, unk041, unk042, unk043, unk044, placeablenpcname, unk046, unk047, unk048, unk049, unk050, unk051, unk052, unk053, unk054, unk055, unk056, unk057, unk058, unk059, unk060, unk061, unk062, unk063, collectible, nodestroy, nonpc, nozone, unk068, unk069, unk070, unk071, noground, unk073, marketplace, freestorage, unk076, unk077, unk078, unk079, evolving, evoid, evolvinglevel, evomax, convertable, convertid, convertname, updated, created, submitter, verified, verifiedby, collectversion, idfileextra, mounteffect, mountlevel2, mounteffecttype, mountlevel, mountunk1, mountunk2, mountunk3, mountunk4, mountunk5, mountname, mountunk7, blessinglevel2, blessingeffecttype, blessinglevel, blessingunk1, blessingunk2, blessingunk3, blessingunk4, blessingunk5, blessingunk7, familiareffect, familiarlevel2, familiareffecttype, familiarlevel, familiarunk1, familiarunk2, familiarunk3, familiarunk4, familiarunk5, familiarname, familiarunk7, unk80, minluck, maxluck, loreequippedgroup) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
}

bool SODEQItemConverter315::execAddItemToRawDB(sqlite3* db) const
{
	if (db)
	{
		sqlite3_stmt* stmt;
		if (sqlite3_prepare_v2(db, getSQLInsertRawItemStmt().c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		{
			WriteChatf("MQ2LinkDB: Error preparing query for raw_item insertion: %s", sqlite3_errmsg(db));
			return false;
		}
		else
		{
			int bind_idx = 1;
			sqlite3_bind_int(stmt, bind_idx++, itemclass);
			sqlite3_bind_text(stmt, bind_idx++, name, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, lore, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, idfile, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, lorefile, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, id);
			sqlite3_bind_int(stmt, bind_idx++, weight);
			sqlite3_bind_int(stmt, bind_idx++, norent);
			sqlite3_bind_int(stmt, bind_idx++, nodrop);
			sqlite3_bind_int(stmt, bind_idx++, attuneable);
			sqlite3_bind_int(stmt, bind_idx++, size);
			sqlite3_bind_int(stmt, bind_idx++, slots);
			sqlite3_bind_int(stmt, bind_idx++, price);
			sqlite3_bind_int(stmt, bind_idx++, icon);
			sqlite3_bind_int(stmt, bind_idx++, benefitflag);
			sqlite3_bind_int(stmt, bind_idx++, tradeskills);
			sqlite3_bind_int(stmt, bind_idx++, cr);
			sqlite3_bind_int(stmt, bind_idx++, dr);
			sqlite3_bind_int(stmt, bind_idx++, pr);
			sqlite3_bind_int(stmt, bind_idx++, mr);
			sqlite3_bind_int(stmt, bind_idx++, fr);
			sqlite3_bind_int(stmt, bind_idx++, svcorruption);
			sqlite3_bind_int(stmt, bind_idx++, astr);
			sqlite3_bind_int(stmt, bind_idx++, asta);
			sqlite3_bind_int(stmt, bind_idx++, aagi);
			sqlite3_bind_int(stmt, bind_idx++, adex);
			sqlite3_bind_int(stmt, bind_idx++, acha);
			sqlite3_bind_int(stmt, bind_idx++, aint);
			sqlite3_bind_int(stmt, bind_idx++, awis);
			sqlite3_bind_int(stmt, bind_idx++, hp);
			sqlite3_bind_int(stmt, bind_idx++, mana);
			sqlite3_bind_int(stmt, bind_idx++, endur);
			sqlite3_bind_int(stmt, bind_idx++, ac);
			sqlite3_bind_int(stmt, bind_idx++, regen);
			sqlite3_bind_int(stmt, bind_idx++, manaregen);
			sqlite3_bind_int(stmt, bind_idx++, enduranceregen);
			sqlite3_bind_int(stmt, bind_idx++, classes);
			sqlite3_bind_int(stmt, bind_idx++, races);
			sqlite3_bind_int(stmt, bind_idx++, deity);
			sqlite3_bind_int(stmt, bind_idx++, skillmodvalue);
			sqlite3_bind_int(stmt, bind_idx++, skillmodmax);
			sqlite3_bind_int(stmt, bind_idx++, skillmodtype);
			sqlite3_bind_int(stmt, bind_idx++, skillmodextra);
			sqlite3_bind_int(stmt, bind_idx++, banedmgrace);
			sqlite3_bind_int(stmt, bind_idx++, banedmgbody);
			sqlite3_bind_int(stmt, bind_idx++, banedmgraceamt);
			sqlite3_bind_int(stmt, bind_idx++, banedmgamt);
			sqlite3_bind_int(stmt, bind_idx++, magic);
			sqlite3_bind_int(stmt, bind_idx++, foodduration);
			sqlite3_bind_int(stmt, bind_idx++, reqlevel);
			sqlite3_bind_int(stmt, bind_idx++, reclevel);
			sqlite3_bind_int(stmt, bind_idx++, recskill);
			sqlite3_bind_int(stmt, bind_idx++, bardtype);
			sqlite3_bind_int(stmt, bind_idx++, bardvalue);
			sqlite3_bind_int(stmt, bind_idx++, unk002);
			sqlite3_bind_int(stmt, bind_idx++, unk003);
			sqlite3_bind_int(stmt, bind_idx++, unk004);
			sqlite3_bind_int(stmt, bind_idx++, light);
			sqlite3_bind_int(stmt, bind_idx++, delay);
			sqlite3_bind_int(stmt, bind_idx++, elemdmgtype);
			sqlite3_bind_int(stmt, bind_idx++, elemdmgamt);
			sqlite3_bind_int(stmt, bind_idx++, range);
			sqlite3_bind_int(stmt, bind_idx++, damage);
			sqlite3_bind_int(stmt, bind_idx++, color);
			sqlite3_bind_int(stmt, bind_idx++, prestige);
			sqlite3_bind_int(stmt, bind_idx++, unk006);
			sqlite3_bind_int(stmt, bind_idx++, unk007);
			sqlite3_bind_int(stmt, bind_idx++, unk008);
			sqlite3_bind_int(stmt, bind_idx++, itemtype);
			sqlite3_bind_int(stmt, bind_idx++, material);
			sqlite3_bind_int(stmt, bind_idx++, materialunk1);
			sqlite3_bind_int(stmt, bind_idx++, elitematerial);
			sqlite3_bind_int(stmt, bind_idx++, heroforge1);
			sqlite3_bind_int(stmt, bind_idx++, heroforge2);
			sqlite3_bind_int(stmt, bind_idx++, sellrate);
			sqlite3_bind_int(stmt, bind_idx++, extradmgskill);
			sqlite3_bind_int(stmt, bind_idx++, extradmgamt);
			sqlite3_bind_int(stmt, bind_idx++, charmfileid);
			sqlite3_bind_int(stmt, bind_idx++, factionmod1);
			sqlite3_bind_int(stmt, bind_idx++, factionamt1);
			sqlite3_bind_int(stmt, bind_idx++, factionmod2);
			sqlite3_bind_int(stmt, bind_idx++, factionamt2);
			sqlite3_bind_int(stmt, bind_idx++, factionmod3);
			sqlite3_bind_int(stmt, bind_idx++, factionamt3);
			sqlite3_bind_int(stmt, bind_idx++, factionmod4);
			sqlite3_bind_int(stmt, bind_idx++, factionamt4);
			sqlite3_bind_text(stmt, bind_idx++, charmfile, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, augtype);
			sqlite3_bind_int(stmt, bind_idx++, augstricthidden);
			sqlite3_bind_int(stmt, bind_idx++, augrestrict);
			sqlite3_bind_int(stmt, bind_idx++, augslot1type);
			sqlite3_bind_int(stmt, bind_idx++, augslot1visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot1unk2);
			sqlite3_bind_int(stmt, bind_idx++, augslot2type);
			sqlite3_bind_int(stmt, bind_idx++, augslot2visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot2unk2);
			sqlite3_bind_int(stmt, bind_idx++, augslot3type);
			sqlite3_bind_int(stmt, bind_idx++, augslot3visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot3unk2);
			sqlite3_bind_int(stmt, bind_idx++, augslot4type);
			sqlite3_bind_int(stmt, bind_idx++, augslot4visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot4unk2);
			sqlite3_bind_int(stmt, bind_idx++, augslot5type);
			sqlite3_bind_int(stmt, bind_idx++, augslot5visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot5unk2);
			sqlite3_bind_int(stmt, bind_idx++, augslot6type);
			sqlite3_bind_int(stmt, bind_idx++, augslot6visible);
			sqlite3_bind_int(stmt, bind_idx++, augslot6unk2);
			sqlite3_bind_int(stmt, bind_idx++, pointtype);
			sqlite3_bind_int(stmt, bind_idx++, ldontheme);
			sqlite3_bind_int(stmt, bind_idx++, ldonprice);
			sqlite3_bind_int(stmt, bind_idx++, ldonsellbackrate);
			sqlite3_bind_int(stmt, bind_idx++, ldonsold);
			sqlite3_bind_int(stmt, bind_idx++, bagtype);
			sqlite3_bind_int(stmt, bind_idx++, bagslots);
			sqlite3_bind_int(stmt, bind_idx++, bagsize);
			sqlite3_bind_int(stmt, bind_idx++, bagwr);
			sqlite3_bind_int(stmt, bind_idx++, booktype);
			sqlite3_bind_int(stmt, bind_idx++, booklang);
			sqlite3_bind_text(stmt, bind_idx++, filename, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, loregroup);
			sqlite3_bind_int(stmt, bind_idx++, artifactflag);
			sqlite3_bind_int(stmt, bind_idx++, summoned);
			sqlite3_bind_int(stmt, bind_idx++, favor);
			sqlite3_bind_int(stmt, bind_idx++, fvnodrop);
			sqlite3_bind_int(stmt, bind_idx++, attack);
			sqlite3_bind_int(stmt, bind_idx++, haste);
			sqlite3_bind_int(stmt, bind_idx++, guildfavor);
			sqlite3_bind_int(stmt, bind_idx++, augdistiller);
			sqlite3_bind_int(stmt, bind_idx++, unk009);
			sqlite3_bind_int(stmt, bind_idx++, unk010);
			sqlite3_bind_int(stmt, bind_idx++, nopet);
			sqlite3_bind_int(stmt, bind_idx++, unk011);
			sqlite3_bind_int(stmt, bind_idx++, stacksize);
			sqlite3_bind_int(stmt, bind_idx++, notransfer);
			sqlite3_bind_int(stmt, bind_idx++, expendablearrow);
			sqlite3_bind_int(stmt, bind_idx++, unk012);
			sqlite3_bind_int(stmt, bind_idx++, unk013);
			sqlite3_bind_int(stmt, bind_idx++, clickeffect);
			sqlite3_bind_int(stmt, bind_idx++, clicklevel2);
			sqlite3_bind_int(stmt, bind_idx++, clicktype);
			sqlite3_bind_int(stmt, bind_idx++, clicklevel);
			sqlite3_bind_int(stmt, bind_idx++, clickmaxcharges);
			sqlite3_bind_int(stmt, bind_idx++, clickcasttime);
			sqlite3_bind_int(stmt, bind_idx++, clickrecastdelay);
			sqlite3_bind_int(stmt, bind_idx++, clickrecasttype);
			sqlite3_bind_int(stmt, bind_idx++, clickunk5);
			sqlite3_bind_int(stmt, bind_idx++, clickname);
			sqlite3_bind_int(stmt, bind_idx++, clickunk7);
			sqlite3_bind_int(stmt, bind_idx++, proceffect);
			sqlite3_bind_int(stmt, bind_idx++, proclevel2);
			sqlite3_bind_int(stmt, bind_idx++, proctype);
			sqlite3_bind_int(stmt, bind_idx++, proclevel);
			sqlite3_bind_int(stmt, bind_idx++, procunk1);
			sqlite3_bind_int(stmt, bind_idx++, procunk2);
			sqlite3_bind_int(stmt, bind_idx++, procunk3);
			sqlite3_bind_int(stmt, bind_idx++, procunk4);
			sqlite3_bind_int(stmt, bind_idx++, procrate);
			sqlite3_bind_int(stmt, bind_idx++, procname);
			sqlite3_bind_int(stmt, bind_idx++, procunk7);
			sqlite3_bind_int(stmt, bind_idx++, worneffect);
			sqlite3_bind_int(stmt, bind_idx++, wornlevel2);
			sqlite3_bind_int(stmt, bind_idx++, worntype);
			sqlite3_bind_int(stmt, bind_idx++, wornlevel);
			sqlite3_bind_int(stmt, bind_idx++, wornunk1);
			sqlite3_bind_int(stmt, bind_idx++, wornunk2);
			sqlite3_bind_int(stmt, bind_idx++, wornunk3);
			sqlite3_bind_int(stmt, bind_idx++, wornunk4);
			sqlite3_bind_int(stmt, bind_idx++, wornunk5);
			sqlite3_bind_int(stmt, bind_idx++, wornname);
			sqlite3_bind_int(stmt, bind_idx++, wornunk7);
			sqlite3_bind_int(stmt, bind_idx++, focuseffect);
			sqlite3_bind_int(stmt, bind_idx++, focuslevel2);
			sqlite3_bind_int(stmt, bind_idx++, focustype);
			sqlite3_bind_int(stmt, bind_idx++, focuslevel);
			sqlite3_bind_int(stmt, bind_idx++, focusunk1);
			sqlite3_bind_int(stmt, bind_idx++, focusunk2);
			sqlite3_bind_int(stmt, bind_idx++, focusunk3);
			sqlite3_bind_int(stmt, bind_idx++, focusunk4);
			sqlite3_bind_int(stmt, bind_idx++, focusunk5);
			sqlite3_bind_int(stmt, bind_idx++, focusname);
			sqlite3_bind_int(stmt, bind_idx++, focusunk7);
			sqlite3_bind_int(stmt, bind_idx++, scrolleffect);
			sqlite3_bind_int(stmt, bind_idx++, scrolllevel2);
			sqlite3_bind_int(stmt, bind_idx++, scrolltype);
			sqlite3_bind_int(stmt, bind_idx++, scrolllevel);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk1);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk2);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk3);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk4);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk5);
			sqlite3_bind_int(stmt, bind_idx++, scrollname);
			sqlite3_bind_int(stmt, bind_idx++, scrollunk7);
			sqlite3_bind_int(stmt, bind_idx++, bardeffect);
			sqlite3_bind_int(stmt, bind_idx++, bardlevel2);
			sqlite3_bind_int(stmt, bind_idx++, bardeffecttype);
			sqlite3_bind_int(stmt, bind_idx++, bardlevel);
			sqlite3_bind_int(stmt, bind_idx++, bardunk1);
			sqlite3_bind_int(stmt, bind_idx++, bardunk2);
			sqlite3_bind_int(stmt, bind_idx++, bardunk3);
			sqlite3_bind_int(stmt, bind_idx++, bardunk4);
			sqlite3_bind_int(stmt, bind_idx++, bardunk5);
			sqlite3_bind_int(stmt, bind_idx++, bardname);
			sqlite3_bind_int(stmt, bind_idx++, bardunk7);
			sqlite3_bind_text(stmt, bind_idx++, unk014, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, unk015, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, unk016, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, unk017);
			sqlite3_bind_int(stmt, bind_idx++, unk018);
			sqlite3_bind_int(stmt, bind_idx++, unk019);
			sqlite3_bind_int(stmt, bind_idx++, unk020);
			sqlite3_bind_int(stmt, bind_idx++, unk021);
			sqlite3_bind_int(stmt, bind_idx++, unk022);
			sqlite3_bind_int(stmt, bind_idx++, scriptfile);
			sqlite3_bind_int(stmt, bind_idx++, questitemflag);
			sqlite3_bind_int(stmt, bind_idx++, powersourcecapacity);
			sqlite3_bind_int(stmt, bind_idx++, purity);
			sqlite3_bind_int(stmt, bind_idx++, epic);
			sqlite3_bind_int(stmt, bind_idx++, backstabdmg);
			sqlite3_bind_int(stmt, bind_idx++, heroic_str);
			sqlite3_bind_int(stmt, bind_idx++, heroic_int);
			sqlite3_bind_int(stmt, bind_idx++, heroic_wis);
			sqlite3_bind_int(stmt, bind_idx++, heroic_agi);
			sqlite3_bind_int(stmt, bind_idx++, heroic_dex);
			sqlite3_bind_int(stmt, bind_idx++, heroic_sta);
			sqlite3_bind_int(stmt, bind_idx++, heroic_cha);
			sqlite3_bind_int(stmt, bind_idx++, unk029);
			sqlite3_bind_int(stmt, bind_idx++, healamt);
			sqlite3_bind_int(stmt, bind_idx++, spelldmg);
			sqlite3_bind_int(stmt, bind_idx++, clairvoyance);
			sqlite3_bind_int(stmt, bind_idx++, unk030);
			sqlite3_bind_int(stmt, bind_idx++, unk031);
			sqlite3_bind_int(stmt, bind_idx++, unk032);
			sqlite3_bind_int(stmt, bind_idx++, unk033);
			sqlite3_bind_int(stmt, bind_idx++, unk034);
			sqlite3_bind_int(stmt, bind_idx++, unk035);
			sqlite3_bind_int(stmt, bind_idx++, unk036);
			sqlite3_bind_int(stmt, bind_idx++, unk037);
			sqlite3_bind_int(stmt, bind_idx++, heirloom);
			sqlite3_bind_int(stmt, bind_idx++, placeable);
			sqlite3_bind_int(stmt, bind_idx++, unk038);
			sqlite3_bind_int(stmt, bind_idx++, unk039);
			sqlite3_bind_int(stmt, bind_idx++, unk040);
			sqlite3_bind_int(stmt, bind_idx++, unk041);
			sqlite3_bind_int(stmt, bind_idx++, unk042);
			sqlite3_bind_int(stmt, bind_idx++, unk043);
			sqlite3_bind_int(stmt, bind_idx++, unk044);
			sqlite3_bind_text(stmt, bind_idx++, placeablenpcname, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, unk046);
			sqlite3_bind_int(stmt, bind_idx++, unk047);
			sqlite3_bind_int(stmt, bind_idx++, unk048);
			sqlite3_bind_int(stmt, bind_idx++, unk049);
			sqlite3_bind_int(stmt, bind_idx++, unk050);
			sqlite3_bind_int(stmt, bind_idx++, unk051);
			sqlite3_bind_int(stmt, bind_idx++, unk052);
			sqlite3_bind_int(stmt, bind_idx++, unk053);
			sqlite3_bind_int(stmt, bind_idx++, unk054);
			sqlite3_bind_int(stmt, bind_idx++, unk055);
			sqlite3_bind_int(stmt, bind_idx++, unk056);
			sqlite3_bind_int(stmt, bind_idx++, unk057);
			sqlite3_bind_int(stmt, bind_idx++, unk058);
			sqlite3_bind_int(stmt, bind_idx++, unk059);
			sqlite3_bind_int(stmt, bind_idx++, unk060);
			sqlite3_bind_int(stmt, bind_idx++, unk061);
			sqlite3_bind_int(stmt, bind_idx++, unk062);
			sqlite3_bind_text(stmt, bind_idx++, unk063, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, collectible);
			sqlite3_bind_int(stmt, bind_idx++, nodestroy);
			sqlite3_bind_int(stmt, bind_idx++, nonpc);
			sqlite3_bind_int(stmt, bind_idx++, nozone);
			sqlite3_bind_int(stmt, bind_idx++, unk068);
			sqlite3_bind_int(stmt, bind_idx++, unk069);
			sqlite3_bind_int(stmt, bind_idx++, unk070);
			sqlite3_bind_int(stmt, bind_idx++, unk071);
			sqlite3_bind_int(stmt, bind_idx++, noground);
			sqlite3_bind_int(stmt, bind_idx++, unk073);
			sqlite3_bind_int(stmt, bind_idx++, marketplace);
			sqlite3_bind_int(stmt, bind_idx++, freestorage);
			sqlite3_bind_int(stmt, bind_idx++, unk076);
			sqlite3_bind_int(stmt, bind_idx++, unk077);
			sqlite3_bind_int(stmt, bind_idx++, unk078);
			sqlite3_bind_int(stmt, bind_idx++, unk079);
			sqlite3_bind_int(stmt, bind_idx++, evolving);
			sqlite3_bind_int(stmt, bind_idx++, evoid);
			sqlite3_bind_int(stmt, bind_idx++, evolvinglevel);
			sqlite3_bind_int(stmt, bind_idx++, evomax);
			sqlite3_bind_int(stmt, bind_idx++, convertable);
			sqlite3_bind_int(stmt, bind_idx++, convertid);
			sqlite3_bind_text(stmt, bind_idx++, convertname, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, updated);
			sqlite3_bind_int(stmt, bind_idx++, created);
			sqlite3_bind_text(stmt, bind_idx++, submitter, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, verified);
			sqlite3_bind_text(stmt, bind_idx++, verifiedby, -1, SQLITE_STATIC);
			sqlite3_bind_text(stmt, bind_idx++, collectversion, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, idfileextra);
			sqlite3_bind_int(stmt, bind_idx++, mounteffect);
			sqlite3_bind_int(stmt, bind_idx++, mountlevel2);
			sqlite3_bind_int(stmt, bind_idx++, mounteffecttype);
			sqlite3_bind_int(stmt, bind_idx++, mountlevel);
			sqlite3_bind_int(stmt, bind_idx++, mountunk1);
			sqlite3_bind_int(stmt, bind_idx++, mountunk2);
			sqlite3_bind_int(stmt, bind_idx++, mountunk3);
			sqlite3_bind_int(stmt, bind_idx++, mountunk4);
			sqlite3_bind_int(stmt, bind_idx++, mountunk5);
			sqlite3_bind_text(stmt, bind_idx++, mountname, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, mountunk7);
			sqlite3_bind_int(stmt, bind_idx++, blessinglevel2);
			sqlite3_bind_int(stmt, bind_idx++, blessingeffecttype);
			sqlite3_bind_int(stmt, bind_idx++, blessinglevel);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk1);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk2);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk3);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk4);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk5);
			sqlite3_bind_int(stmt, bind_idx++, blessingunk7);
			sqlite3_bind_int(stmt, bind_idx++, familiareffect);
			sqlite3_bind_int(stmt, bind_idx++, familiarlevel2);
			sqlite3_bind_int(stmt, bind_idx++, familiareffecttype);
			sqlite3_bind_int(stmt, bind_idx++, familiarlevel);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk1);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk2);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk3);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk4);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk5);
			sqlite3_bind_text(stmt, bind_idx++, familiarname, -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, bind_idx++, familiarunk7);
			sqlite3_bind_int(stmt, bind_idx++, unk80);
			sqlite3_bind_int(stmt, bind_idx++, minluck);
			sqlite3_bind_int(stmt, bind_idx++, maxluck);
			sqlite3_bind_text(stmt, bind_idx++, loreequippedgroup, -1, SQLITE_STATIC);

			if (sqlite3_step(stmt) != SQLITE_DONE)
			{
				WriteChatf("\arMQ2LinkDB: Error inserting into raw_item table: %s", sqlite3_errmsg(db));
				return false;
			}

			sqlite3_finalize(stmt);

			return true;
		}
	}

	return false;
}

