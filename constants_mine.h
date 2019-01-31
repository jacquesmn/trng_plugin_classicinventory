
// TYPE_HERE: here you can type your mnemonic constants, using the #define directive
// like trng did it in the "Tomb_NextGeneration.h" file
// in this source you can also type your declaration of tomb raider function
// like it happens for "functions.h" source

#define CUST_CINV						1
#define CUST_CINV_RING					2
#define CUST_CINV_ITEM					3
#define CUST_CINV_ITEM_RING				4
#define CUST_CINV_ITEM_MODEL			5
#define CUST_CINV_ITEM_DISPLAY			6
#define CUST_CINV_ITEM_QUANTITY			7
#define CUST_CINV_ITEM_ANIMATION		8
#define CUST_CINV_ITEM_ACTION			9
#define CUST_CINV_ITEM_SFX				10
#define CUST_CINV_DURATION				11
#define CUST_CINV_DISPLAY				12
#define CUST_CINV_SFX					13
#define CUST_CINV_TEXT					14
#define CUST_CINV_CAMERA				15
#define CUST_CINV_LIGHTING				16
#define CUST_CINV_CHEATS				17
#define CUST_CINV_AMMO					18
#define CUST_CINV_COMBO					19
#define CUST_CINV_EXAMINE				20
#define CUST_CINV_HEALTH				21
#define CUST_CINV_COMPASS				22
#define CUST_CINV_STOPWATCH				23
#define CUST_CINV_PASSPORT				24
#define CUST_CINV_MAP					25
#define CUST_CINV_DEBUG					26

#define CINV_FALSE						0
#define CINV_TRUE						1

// -------- START PRESET CONSTANTS ---------------------------------
// please don't remove the constants in this "PRESET CONSTANT" zone. 
// They will be used by some preset function in your plugin source

#define MAX_MYPROGR_ACTIONS 100
#define MAX_ASSIGN_SLOT_MINE 200

#define ENDLESS_DURATE 0xFFFF // to use as number of frames to set an infinite durate
#define NGTAG_END_SEQUENCE		   0   // signals the end of ng token sequence
#define NGTAG_PROGRESSIVE_ACTIONS  1
#define NGTAG_LOCAL_DATA           2
#define NGTAG_GLOBAL_DATA          3
#define AXN_FREE  0  // this record is to free an action record. You type this value in ActionType to free a action progress record
// --------- END PRESET CONSTANTS ---------------------------------
// type here the constant name for new progressive action you create
// use progressive number to have always different value for each AXN_ constant/action


// MPS_ flags
// you should type here your MPS_ flags for plugin command.
// then you'll test the presence of these flags checking the Trng.MainPluginFlags  value using "&" operator
// please: do not use the value 0x40000000 (MPS_DISABLE) because it has been already set by trng engine
