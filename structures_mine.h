// let below pragma directive at top of this source
#pragma pack(1)

// TYPE_HERE: here you can type your structure definitions like it has been done
// in the "structures.h" file for trng dll

typedef struct StrSavegameGlobalInventoryData {
	int item_qty[256];
	BYTE map_marker_active[256];
}SavegameGlobalInventoryData;

typedef struct StrSavegameLocalInventoryData {
	int ring_id_selected;
	int item_id_selected;
	int item_id_used;

	StrSavegameLocalInventoryData()
		:
		ring_id_selected(-1),
		item_id_selected(-1),
		item_id_used(-1)
	{}
}SavegameLocalInventoryData;

typedef struct StrStatistics {
	uint32_t time_taken_seconds;
	uint32_t distance_travelled_meters;
	uint32_t ammo_used;
	uint32_t hits;
	uint32_t kills;
	uint32_t pickups;
	uint32_t health_packs_used;
	uint32_t health_lost;
	uint32_t flares_used;
	uint32_t times_saved;
	uint32_t secrets_found;

	StrStatistics()
		:
		time_taken_seconds(0),
		distance_travelled_meters(0),
		ammo_used(0),
		hits(0),
		kills(0),
		pickups(0),
		health_packs_used(0),
		health_lost(0),
		flares_used(0),
		times_saved(0),
		secrets_found(0)
	{}
}Statistics;

// --------------- PRESET STRUCTURE ZONE -----------------------------
// Please, don't remove structures and fields you find in this "PRESET ZONE". They will be used by some
// preset functions of your plugin sources
// However, you can add new fields and structures in following structures, of course
typedef struct StrSavegameGlobalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in GLOBAL section 
	//           (only one for all levels)
	// note: the size of this structure should be always even (if you add BYTE variable, remember to compensate that 
	//       with another BYTE vairable or placefolder)
	SavegameGlobalInventoryData inventory_data;
	Statistics statistics;

	SavegameGlobalInventoryData placeholder_inventory_data;
	Statistics placeholder_statistics;

	StrSavegameGlobalData()
		:
		inventory_data(SavegameGlobalInventoryData()),
		statistics(Statistics()),
		placeholder_inventory_data(SavegameGlobalInventoryData()),
		placeholder_statistics(Statistics())
	{}
}SavegameGlobalDataFields;

typedef struct StrSavegameLocalData {
	// FOR_YOU:
	// define here your variables that you wish were saved (and then restored) to/from savegame in LOCAL section (one for each different level)
	// note: the size of this structure should be always even (if you add BYTE variable, compensate it with another BYTE vairable or placefolder)
	SavegameLocalInventoryData inventory_data;
	Statistics statistics;

	SavegameLocalInventoryData placeholder_inventory_data;
	Statistics placeholder_statistics;

	StrSavegameLocalData()
		:
		inventory_data(SavegameLocalInventoryData()),
		statistics(Statistics()),
		placeholder_inventory_data(SavegameLocalInventoryData()),
		placeholder_statistics(Statistics())
	{}
}SavegameLocalDataFields;


typedef struct StrSavegameData {
	StrSavegameGlobalData Global;
	StrSavegameLocalData  Local;
}SavegameDataFields;


typedef struct StrBaseAssignSlotMine {
	int TotAssign;
	StrRecordAssSlot VetAssignSlot[MAX_ASSIGN_SLOT_MINE];
}BaseAssignSlotMineFields;

typedef struct StrMyData {
	StrSavegameData Save;  // variable that it will be saved and restored to/from savegame
	// FOR_YOU:
	// define here all your global variables, i.e. those variables that will be seen from all procedures and keep 
	// their value between all cycles of the game.
	// after you defined a variabile like:
	// int  Alfa;
	// then you'll be able to access to them using syntax like:
	// MyData.Alfa = 3;
	// if (MyData.Alfa == 5) .
	int TotProgrActions;
	int LastProgrActionIndex; // used in case of overloading

	StrProgressiveAction VetProgrActions[MAX_MYPROGR_ACTIONS];
	StrBaseGenericCustomize BaseCustomizeMine;  // stored all your customize script commands for current level
	StrBaseGenericParameters BaseParametersMine; // store of all your parameters= script commands of current level
	StrBaseAssignSlotMine BaseAssignSlotMine; // stored all AssignSlot= command script commands of current level
}MyDataFields;
// ----------------- END PRESET STRUCTURE ZONE ---------------------------------------


// let below pragma directive at bottom of this source
#pragma pack(2)