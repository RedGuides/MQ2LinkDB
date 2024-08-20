#include "MQ2LinkDBTables.h"

static void extract_item_name(sqlite3_context* context, int argc, sqlite3_value** argv)
{
	if (argc == 1)
	{
		const char* value = reinterpret_cast<const char*>(sqlite3_value_text(argv[0]));
		TextTagInfo info = ExtractLink(value);

		sqlite3_result_text(context, info.text.data(), static_cast<int>(info.text.length()), SQLITE_STATIC);
	}
}

std::string MQ2LinkDBTables::getSQLCreateStmt()
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
		CREATE TABLE IF NOT EXISTS item_links( item_id INTEGER PRIMARY KEY, link TEXT );
		CREATE TABLE IF NOT EXISTS db_metadata( key TEXT PRIMARY KEY, value TEXT, description TEXT );)SQL");
}

/* This will update the tables if needed.*/

bool MQ2LinkDBTables::execUpgradeDB(sqlite3 * db)
{
	if (db)
	{
		int currentVersion = getDBVersion(db);

		switch (currentVersion)
		{
		case -1:
			WriteChatf("MQ2LinkDB: Error determining db schema version.");
			break;
		case 0:
			// upgrade to version 1
			{
				// add the new column
				char* err_msg = nullptr;
				std::string query("ALTER TABLE item_links ADD COLUMN item_name");
				if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
				{
					WriteChatf("MQ2LinkDB: Error preparing query for item_links ALTER TABLE ADD COLUMN: %s", err_msg);
					sqlite3_free(err_msg);
					// this is okay. don't bail on failure.
				}

				query = "ALTER TABLE raw_item_data_315 RENAME TO raw_item_data";
				if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
				{
					WriteChatf("MQ2LinkDB: Error preparing query for item_links ALTER TABLE RENAME: %s", err_msg);
					sqlite3_free(err_msg);
					// this is okay. don't bail on failure.
				}

				// add an index on item name
				query = "CREATE INDEX IF NOT EXISTS `idx_item_links_name` ON `item_links` (`item_name` ASC);";
				if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
				{
					WriteChatf("MQ2LinkDB: Error preparing query for item_links INDEX: %s", err_msg);
					sqlite3_free(err_msg);
					return false;
				}
				
				if (sqlite3_create_function(db, "extract_item_name", 1, SQLITE_UTF8, NULL, &extract_item_name, NULL, NULL) != SQLITE_OK) 
				{
					WriteChatf("MQ2LinkDB: Error creating function for item updates: %s", sqlite3_errmsg(db));
					return false;
				}

				// populate item name into the DB.
				query = "UPDATE item_links SET item_name = extract_item_name(link) WHERE item_name IS NULL";

				if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
				{
					WriteChatf("MQ2LinkDB: Error updating item_names: %s", err_msg);
					sqlite3_free(err_msg);
					return false;
				}

				// change version to the current version. -- futures functions should UPDATE version instead of insert.
				query = R"(INSERT INTO db_metadata(key,value,description) VALUES ('version', '1', 'DB Schema Versioning added, added item_name to item_links table, renamed raw_item_data_315 to raw_item_data.') 
							ON CONFLICT(key) DO UPDATE SET value='1';)";
				if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK)
				{
					WriteChatf("MQ2LinkDB: Error preparing query for db_metadata INSERT: %s", err_msg);
					sqlite3_free(err_msg);
					return false;
				}
			}
			WriteChatf("\agMQ2LinkDB: Successfully updated DB Schema version to 1!");
			// fall through to upgrade to the next version...
			[[fallthrough]];
		default:
			return true;
		}
	}

	return false;
}

int MQ2LinkDBTables::getDBVersion(sqlite3 * db)
{
	if (db)
	{
		sqlite3_stmt* stmt;

		const std::string query("SELECT value FROM db_metadata WHERE key = 'version';");

		if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
		{
			WriteChatf("MQ2LinkDB: Error preparing query for item_link: %s", sqlite3_errmsg(db));
			return -1;
		}
		else
		{
			std::string versionStr("0");

			if (sqlite3_step(stmt) == SQLITE_ROW)
			{
				versionStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
			}
			sqlite3_finalize(stmt);

			return std::stoi(versionStr);
		}
	}

	return -1;
}