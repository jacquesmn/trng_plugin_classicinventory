
// IMPORTANT WARNING: don't change or add text to this source because it will be
// replaced after new update and in this way you should lose your adding.
// Use the "Constants_mine.h" file to add your constants.


// don't remove following line: it's a way to keep memory about current release of 
// your tomb_nextgeneration.h source
// #VERSION_UPDATE_INFO#=PU-8

// type of phase in creation/management between inventory, pause screen, load game, save game ect
#define PHASE_INVENTORY_MAIN 1 // main management of inventory
#define PHASE_PAUSE_SCREEN  2
#define PHASE_LOAD_GAME_PANEL 3
#define PHASE_SAVE_GAME_PANEL 4
#define PHASE_OPTIONS 5
#define PHASE_OPTION_COMMANDS 6
#define PHASE_STATISTICS 7
#define PHASE_NEW_LEVEL 8 
#define PHASE_TITLE_MENU 9 // main screen of title menu
#define PHASE_INVENTORY_EXAMINE 10 



#define CLASSIC_MANAGEMENT -2  // used to let classic management, chosen in run-time, after you got a management callback
// returned values for ApplyCodePatch (SetNewPatch() service)
#define APPC_OK  0 // patch it has been applied succesfully
#define APPC_WARN_PATCH_ALREADY_PERFORMED 1   // same code patch performed two or more times same patch
#define APPC_WARN_PLUGIN_REMOVED_RELOCATION_OF_PROCEDURE 2 // removed rilocation of procedure
#define APPC_WARN_PATCH_IN_A_RILOC_RESTORED_PROCEDURE 3  // typed code in a rilocated and then restored procedure
#define APPC_WARN_PATCH_OVER_OLD_PATCH 4 // patch wrote over another old patch
#define APPC_WARN_PATCH_OVER_CHANGED_ARGUMENTS 5 // patch in code where some arguments have been previously changed
#define APPC_ERROR_PLUGIN_ID_UNREGISTERED  -1 // the supplied plugin id is wrong (unknown)
#define APPC_ERROR_PATCH_OVERLAPPED_TO_RUNTIME_PATCH -2 // attempt to overwrite a run-time patch
#define APPC_ERROR_PATCH_IN_RILOCATED_PROCEDURE -3 // patch is writing in an unplugged code where it was been removed (rilocated) a procedure
#define APPC_ERROR_NOT_FOUND_TOMB4_IMAGE -4 // restore patch is not available for an internal error (this should never happen)
#define APPC_ERROR_RESTORE_ZONE_OUTSIDE_CODE_RANGE -5 // the zone to restore is not inside of tomb4 code zone available for restoring (0x401000 - 0x4A6FFF)
#define APPC_ERROR_WRONG_PATCH_FOR_RUN_TIME -6 // only parametric constant patches are admitted in run time mode
#define APPC_ERROR_PATCH_OUTSIDE_CODE_SEGMENT -7 // it's not possible set code patch outside of code segment (0x401000 - 0x4A6FFF)
#define APPC_ERROR_PATCH_OVERLAPPED_TO_RESERVED_ZONE -8 // a code patch tried to write over a reserved memory data
#define APPC_ERROR_RESERVED_ZONE_ALREADY_USED -9 // the zone you required to reserved for your plugin had already used by other plugins

#define TYPP_CONTIGUOUS_BYTES 0x0001  // standard patch (a serie of contiguous bytes to write)
#define TYPP_PARAMETRIC 0x0002 // patch that changes single values (usually constants) placed in non-contiguos zones
#define TYPP_RUN_TIME   0x0004   // patch that will be applied in game playing instead, as usual, before loading the game
#define TYPP_RILOCATOR  0x0008  // patch that replaced (and relocated in plugin code) a whole procedure
#define TYPP_PARAMETRIC_CALL  0x0010  // changed value is a call address to update a different location of some subroutine
#define TYPP_PARAMETRIC_CONST 0x0020  // change value is a constant (immediate) value
#define TYPP_PARAMETRIC_MEM   0x0040  // changed value is an address memoory (usually it used to expand memory zone)
#define TYPP_RESTORE          0x0080  // restore a given code zone of tomb4 with original values of tomb4 (before any patch had been performed)
#define	TYPP_FIXED_TRNG       0x0100  // not dynamic patch but a fixed patch of trng created usually to link tomb_nextgeneration.dll)


// mnemonic constant for orienting (ngle facing, direction) using signed numbers
#define ORIENT_NORTH -16384  // 0xC000
#define ORIENT_SOUTH 16384   // 0x4000
#define ORIENT_EAST 0        // 0x0000
#define ORIENT_WEST -32768   // 0x8000
#define ORIENT_SOUTH_EAST 8192  // 0x2000
#define ORIENT_NORTH_EAST -8192 // 0xE000
#define ORIENT_SOUTH_WEST 24576  // 0x6000
#define ORIENT_NORTH_WEST -24576 // 0xA000

// mnemonic constants for flags of StrSlot structure
#define FSLOT_NONE  0x0000  // used only to clear Flags field, to test presence of slot use (Flags & FSLOT_PRESENT)
#define FSLOT_PRESENT 0x0001  // object of this slot is present
#define FSLOT_AI_STANDARD 0x0002 // (not sure) enemy with traditional AI and movements (yes baddies, no wraith)
#define FSLOT_CHANGE_POS_ITEM 0x0008 // this item could change its position: it's necessary save its coordinate in savegame
#define FSLOT_MOVED_BY_ANIMATIONS 0x0010 // (not sure) moved whereby animations
#define FSLOT_SAVE_ALL_DATA 0x0020  // this item requires many data, other position, to be saved in savegame
#define FSLOT_AFFECT_LARA_AT_CONTACT 0x0040 // (not sure) lara could interacts with this item when she is closed to it
#define FSLOT_SFX_LOCAL_SOUND 0x0100 // the sounds of this moveable played in its local position (otherwise: global sound)
#define FSLOT_USE_COLLISION_BOX 0x0200 // item collisions will be checked whereby its collisional box
#define FSLOT_AMPHIBIOUS_CREATURE 0x0400 // creature is able to move underwater
#define FSLOT_HIT_BUT_NOT_HURT_BY_SHOTGUN 0x0800 // (not sure) shotgun ammo disturb them without hurt them (mummy and skeleton)
#define FSLOT_NO_DAMAGE_FOR_NO_EXPLOSIVE_AMMO 0x1000 // invulnerable at not explosive ammo
#define FSLOT_SAVE_MESH_MASK 0x2000 // this item could change visibility of its meshes: save also mesh visibility status

// mnemonic constant for FlagsMain of StrItemTr4 structure
#define FITEM_NONE 0x0000 // used only to clear flags
#define FITEM_ACTIVE 0x0001 // item is active (it will move) flag set after calling AddActiveItem()
#define FITEM_CREATURE 0x0002 // Creatures are invisible until they have not been triggered
#define FITEM_NOT_VISIBLE 0x0004 // the item was not visible at start
#define FITEM_GRAVITY_AFFECTED 0x0008 // the item is falling down (or jumping up) and it's subject to gravity simulation, currently
#define FITEM_FLAG_10  0x0010  // (let for backward compatibility but now it is sure its meaning: 
							   //  see the FITEM_ITEM_HAS_BEEN_HIT flag)
#define FITEM_ITEM_HAS_BEEN_HIT 0x0010 // the item has been hit: with weapons (but not grenade)
#define FITEM_NOT_YET_ENABLED 0x0020  // the item has not yet been enabled (triggered)
#define FITEM_KILLED_WITH_EXPLOSION 0x0040 // trng flag, added to remember that this enemy has been killed with an explosion
#define FITEM_POISONED 0x0100 // enemy (or lara?) has been poisoned
#define FITEM_AI_GUARD 0x0200 // enemy was over a AI_GUARD item
#define FITEM_AI_AMBUSH 0x0400 // emeny was over a AI_AMBUSH item
#define FITEM_AI_PATROL1 0x0800 // enemy was over a AI_PATROL1 (and perhaps AI_PATROL2)
#define FITEM_AI_MODIFY 0x1000 // enemy was over a AI_MODIFY item
#define FITEM_AI_FOLLOW 0x2000  // enemy was over a AI_FOLLOW item
#define FITEM_THROWN_AMMO 0x4000 // (not sure) it used with visible ammo like greande or arrows of crossbow

// LIST OF CALLBACK THAT YOU CAN REQUIRE:

// ------------ CALLBACKS WITH NO FLAGS, NO PARAMETERS --------------------------
#define CB_INIT_PROGRAM 0    // DEFAULT. You have already it. Look for "cbInitProgram" procedure in Plugin_trng.cpp source
#define CB_SAVING_GAME  1    // DEFAULT. You have already it. Look for "cbSaveMyData" procedure in Plugin_trng.cpp source
#define CB_LOADING_GAME 2    // DEFAULT. You have already it. Look for "cbLoadMyData" procedure in Plugin_trng.cpp
#define CB_INIT_GAME    3    // DEFAULT. You have already it. Look for "cbInitGame" procedure in Plugin_trng.cpp source 
#define CB_INIT_LOAD_NEW_LEVEL 4   // DEFAULT. You have already it. Look for "cbInitLoadNewLevel" procedure in Pluging_trng.cpp source
#define CB_FLIPEFFECT_MINE  5  // DEFAULT. You have already it. Look for "cbFlipEffectMine" procedure in Pluging_trng.cpp source
#define CB_ACTION_MINE 6    // DEFAULT. You have already it. Look for "cbActionMine" procedure in Pluging_trng.cpp source
#define CB_CONDITION_MINE 7  // DEFAULT. You have already it. Look for "cbConditionMine" procedure in Pluging_trng.cpp source
#define CB_CUSTOMIZE_MINE 8  // DEFAULT. You have already it. Look for "cbCustomizeMine" procedure in Pluging_trng.cpp source
#define CB_PARAMETER_MINE 9  // DEFAULT. You have already it. Look for "cbParametersMine" procedure in Pluging_trng.cpp source
#define CB_CYCLE_BEGIN 10  // DEFAULT. You have already it. Look for "cbCycleBegin" procedure in Plugin_trng.cpp
#define CB_CYCLE_END 11     // callback called at end of every game cycle
#define CB_GLOBAL_TRIGGER 12 // callback to give the result about a global trigger GT_... (yours or old of trng)
#define CB_INIT_OBJECTS 13 // it will be called just completed the loading of object slots from tr4 level. (CALL_VOID) You can use it to initialise your objects (moveables) or to change the settings of other moveables
#define CB_PROGR_ACTION_MINE 14 // DEFAULT: You have already it. Look for "cbProgrActionMine" procedure in Plugin_trng.cpp source
#define CB_PROGR_ACTION_DRAW_MINE 15 // callback to perform your progressive actions that requires a direct draw on screen (meshes, images, windows font texts, sprites)
#define CB_INIT_LEVEL 16       // DEFAULT: you have already it. Look for "cbInitLevel" procedure. it will be performed just a moment before entering in main game cycle. All items have already been initialized 
#define CB_COMPLETED_PROGR_ACTION 17 // callback called when a trng progressive action has been just completed. 
#define CB_VEHICLE 18    // callback for vehicle management
#define CB_ASSIGN_SLOT_MINE 19 // DEFAULT: You have already it. Look for "cbAssignSlotMine() procedure. It receives and stores all AssignSlot= data about your OBJ_ values
#define CB_FMV_MANAGER 20   // callback to replace playing fmv management
#define CB_INPUT_MANAGER 21 // callback to read in advance input command and, optionally remove them
#define CB_SAVEGAME_MANAGER 22 // callback for procedure to handle savegame list to load/save savegames
								// it works like "replace": you replace the original/trng management 
								// about savegame selection
#define CB_PAUSE_MANAGER 23 // callback to handle pause menu phase. it works as a replace callback
#define CB_STATISTICS_MANAGER 24 // callback to the procedure to show statistics screen.
#define CB_TITLE_MENU_MANAGER 25 // callback for the menu of title level
#define CB_WINDOWS_FONT_CREATE 26 // callback to replace the creation of windows font for printing window text
#define CB_WINDOWS_UNICODE_CONVERT 27 // callback to convert from text of script to unicode text for windows text printing 
#define CB_WINDOWS_TEXT_PRINT 28 // callback to replace print windows text trng function
#define CB_DIAGNOSTIC 29        // callback to add own diagnostic on screen or to monitor current trng diagnostic text

// --------- CALLBACKS WITH FLAGS (CBT_FIRST, CBT_AFTER,CBT_REPLACE) but NO PARAMETERS -----------------
#define CB_LARA_CONTROL 33   // callback for main control procedure of lara that swap environment: 
//                                      ground, water surface, underwater, vehicle
#define CB_LARA_DRAW 34      // callback for draw lara procedure 
#define CB_LARA_HAIR_DRAW 35  // callback for draw hair procedure 
#define CB_LARA_HAIR_CONTROL 36 // callback for control hair procedure 
#define CB_INVENTORY_MAIN 37 // callback for procedure that shows inventory and handles it
#define	CB_INVENT_BACKGROUND_CREATE 38 // callback for procedure ( CreateMonoScreen) that creates a backgroup (allocating resources if necessary)
#define CB_INVENT_BACKGROUND_DRAW 39 // callback for procedure (S_DisplayMonoScreen) that updates graphic for background
#define CB_INVENT_BACKGROUND_QUIT 40 // callback for procedure (FreeMonoScreen) that free futher resource allocated by BACKGROUND_CREATE
#define CB_ANIMATE_LARA 41 // callback for procedure AnimateLaran that update frames and change animations of lara
#define CB_OPTIONS_MANAGER 42  // callback to the procedure to show/edit options
								// note: it accepts only CBT_REPLACE and CBT_LOOPED flags
// ---------- CALLBACKS WITH FLAGS AND PARAMETERS -----------------------------------------------------
#define CB_FLIPEFFECT 100     // callback for a tomb_nextgeneration (old, not yours) FLIPEFFECT trigger
#define CB_ACTION 101         // callback for a tomb_nextgeneration (old, not yours) ACTION trigger 
#define CB_CONDITION 102      // callback for a tomb_nextgeneration (old, not yours) CONDITION trigger. WARNING: condition triggers have a particular management, see infos about this kind of callback
#define CB_VEHICLE_CONTROL 103  // calback for vehicles not handled by common slot control procedures
							  // set as parameter the slot of vehicle (jeep, sidecar or kayak)
#define CB_PROGR_ACTION 105   // callback for a tomb_nextgeneration (old, not yours) PROGRESSIVE ACTION. Type the AZ_ constant to choose it.
#define CB_NUMERIC_TRNG_PATCH 106 // callback affects a given MOV AX , Number  / Call dword [TrngPatcher]  patch of trng (see tomb4 patched code source for their position and target)
#define CB_SLOT_INITIALISE 107 // callboack for Initialise procedure of given slot object
#define CB_SLOT_CONTROL 108    // callback for Control procedure of given slot object
#define CB_SLOT_COLLISION 109  // callback for Collision procedure of given slot object
#define CB_SLOT_DRAW 110      // callback for Draw procedure of given slot object
#define CB_SLOT_FLOOR 111     // callback for floor procedure of given slot object 
#define CB_SLOT_CEILING 112   // callback fro ceiling procedure of given slot object
#define CB_SLOT_DRAW_EXTRA 113 // callback for drawextra procedure of given slot object
#define CB_STATE_ID_LARA_CTRL 114 // callback for control state-id lara, parameter the state-id
#define CB_STATE_ID_LARA_COLLISION 115 // callbacks for collision state-id lara, parameter the state-id


// CallBack Types
// note: not all callbacks support all following types
#define CBT_FIRST    0x0001   // your callback code it will be performed FIRST of orginal trng code, then it will be performed original trng code
#define CBT_AFTER    0x0002   // your callback code it will be performed AFTER that the original trng code has been performed
#define CBT_REPLACE  0x0004   // this callback will REPLACE totally original trng code. 
#define CBT_ASM      0x0008   // this callback will be handled by your assembly code at low level. 
							  //      CBT_ASM works only for CB_NUMERIC_TRNG_PATCH
							  //      if you omit the CBT_ASM flag, the callback should be handled 
							  // by your c++ code (see the prototype for C++ callback to CB_NUMERIC_TRNG_PATCH
							  // in DefTomb4Funct.h file). 
#define CBT_PRESENT 0x0010  // do not use, reserved
#define CBT_LOOPED  0x0020	  // performed togheter with original code, it works with management that has not a precise start and end but when it is in progress
#define CBT_ANY     0xFFFF  // do not use, reserved

// FMV callback returned values:
#define RFMV_OK     0  // the fmv has been played from your callback
#define RFMV_PERFORM_TRNG   1  // let to trng the playing of current fmv

// Trigger RETurn values for flipeffect and action callbacks
#define TRET_PERFORM_ALWAYS 0   // let enabled, the trigger, it will be continuosly performed until lara is over it
#define TRET_PERFORM_ONCE_AND_GO 1 // disable temporary further executions until lara is yet on this sector 
#define TRET_PERFORM_NEVER_MORE 2  // disable forever this trigger on this sector (single-shot)
#define TRET_EXECUTE_ORIGINAL 3 // used only with CBT_REPLACE to change idea and let execution of original code, in this way the CBT_REPLACE becomes dinamically a CBT_FIRST callback

// Condition Trigger RETurn values for callback of conditional trigger
#define CTRET_IS_TRUE  0x0001 // condition is true. se this value is missing then it will be false
#define CTRET_EXTRA_PARAM 0x0002 // this condition trigger had an Extra parameter
#define CTRET_ON_MOVEABLE 0x0004   // this condition trigger worked on a moveable
#define CTRET_PERFORM_ALWAYS 0x0008 // peforming: Perform continuosly this condition 
#define CTRET_ONLY_ONCE_ON_TRUE 0x0010 // (suggested) Next peforming: No more performing until lara is over this sector and the condition was true
#define CTRET_NEVER_MORE_ON_TRUE 0x0020 // Next peforming: No more performing forever if the condition was true 
#define CTRET_PERFORM_ONCE_AND_GO 0x040 // No more performing until lara is over this sector, no matter if the conditon was true or false 
#define CTRET_EXECUTE_ORIGINAL 0x0080  // Only for CBT_REPLACE and CBT_AFTER callbacks.  it lets to original trng code the target to return a response.

// return value from callbacks for all slot procredures: Initialise, Control, Collision, Draw, Floor
// note: you can return two or more of following contant in some case, using or | operator:
// example: return SRET_SKIP_ORIGINAL | SRET_SKIP_TRNG_CODE;
#define SRET_OK                0x0000  // all fine: go on with common callback mangement
#define SRET_SKIP_ORIGINAL     0x0001  // valid only on CBT_FIRST call backs: the callback requires to skip the execution of tomb4 original code (becoming from CBT_FIRST to CBT_REPLACE)
#define SRET_SKIP_TRNG_CODE    0x0002 // valid only with CBT_FIRST call back on codes already managed by trng code (example: control object)

// return values for CB_PAUSE_MANAGER (pause manu) callback
#define PRET_OK           0  // come back to game phase, the pause menu has been handled by this callback
#define PRET_GO_TO_TITLE  1  // player selected "exit to title"
#define PRET_EXECUTE_ORIGINAL 2 // perform original pause management

// returned value for CB_INVENTORY_MAIN callbacks
#define IRET_OK					0x0000  // all fine, go on with common callback managemnt
#define IRET_SKIP_ORIGINAL      0x0001   // skip original management, valid only for CBT_FIRST callback
#define IRET_LOADED_GAME        0x0002   // valid only for CBT_REPLACE or CBT_AFTER callbacks. 
										 // It means that it has been loaded a savegame, in callback code.
								  //     
				//Byte basso: numero stanza
                 //  Byte alto: flags 
				//		0x0100 = attivato con heavy
				//		0x0200 = One-shot solo temporaneo
				//      0x0400 = Pulsante one-shot di trigger del flipeffect
				// aggiunti dopo:
				//      0x0800 = eseguito da floor data in modo nativo
				//      0x1000 = eseguito da triggergroup di script
				//      0x2000 = eseguito con chiamata diretta da codice
// SCANF_... flags, used for trigger callbacks

#define SCANF_HEAVY				0x100  // Only for SCANF_FLOOR_DATA
#define SCANF_TEMP_ONE_SHOT		0x200  // internal setting to stop trigger until lara is yet standing on same sector
#define SCANF_BUTTON_ONE_SHOT	0x400  // one-shot button of the trigger. Only with SCANF_FLOOR_DATA
#define SCANF_YET_TO_PERFORM    0x800  // internal setting to remember this trigger has yet to be performed
#define SCANF_SCRIPT_TRIGGER	0x1000 // trigger read from triggergroup of script.dat
#define SCANF_DIRECT_CALL		0x2000 // trigger has been called from code in direct way
#define SCANF_FLOOR_DATA		0x4000  // trigger has been read directly from floor data of tr4 file
#define SCANF_ANIM_COMMAND		0x8000 // trigger has been called from an animcommand of some animation

// CPU_ flags (enumCPU. ) used to check, set, clear flag registers in pRegisters strucure inside a trng patch callback code
#define CPU_CARRY    0x00001 
#define CPU_PARITY   0x00004  
#define CPU_ZERO     0x00040
#define CPU_SIGN     0x00080


#define FIL_FROM_SAVEGAME    0x0001
#define FIL_FROM_NEW_LEVEL   0x0002 // level started from new game of title or with level jump and resethub
#define FIL_FROM_LEVEL_JUMP  0x0004 // when there is a level jump (note: if resethub was enabled, this flag will be missing)
#define FIL_PRESERVE_LARA    0x0008 // this flag is present: from savegame or from level jump (but no resethub) 
#define FIL_PRESERVE_LEVEL   0x0010 // this flag is present: from savegame or from go and back for level jump and no resethub
#define FIL_FROM_LIKE_SAVEGAME 0x0020 // this flag is enabled if level is from savegame or from level jump with no reset hub and go and back from two levels.


// list of trng services 
// notes: some of these will be called in implicity way from ther procedures
// you can use in more easy way. The following list will be used
// only with the native function Service()
#define SRV_PERFORM_FLIPEFFECT 0 // you can use the shortcut function: PerformFlipEffect() to use this service
#define SRV_PERFORM_ACTION 1 // you can use the shortcut PerformActionTrigger() to use this service
#define SRV_PERFORM_CONDITION 2 // you can use the shortcut PerformConditionTrigger() to use this service
#define SRV_PERFORM_TRIGGERGROUP_ID 3 // Arg1=IdOfTriggerGroup
									// return: -1 = not found Id
									// return: 0 = condition is false
									// return: 1 = condition is true or it's not a condition trigger group
#define SRV_PERFORM_EXPORTED_TRIGGER 4 // you can use shortcut function: PerformExportedTrigger() to use this service
								// Arg1= PluginId / Arg2=FirstWord / Arg3=SecondWord / Arg4=ThirdWord
								// return 0 = condition is false
								// return 1 = condition is true or it's not a conditional trigger

#define SRV_CREATE_TRIGGERGROUP 5 // TestDynamic and then a sequence of triple arg, {FirstWord, SecondWord, ThirdWord}
							// with END_LIST value at end
							// return -1 = error, missing available triggergroups
							// return any other value = the id of your triggergroup 


#define SRV_CREATE_ANIMATION 7 // same fields of Animation command with END_LIST value at end
				// AnimIndex, KEY1_ , KEY2_ , FAN_ flags, ENV_ Environment, Distance for Env, Extra, StateId (STATE_...) or (-)AnimationIndex array  (...)

#define SRV_CREATE_ANIMATIONSLOT 8 // same field of AnimationSlot command with END_LIST at end
				// Slot, ActionType (AXT_...), AnimIndex, Key1, Key2, FAN_ flags, ENV_ Environment, Distance For Env_, Extra, StateId (STATE_...) or (-)AnimationIndex array  (...)

#define SRV_CREATE_MULTENVCONDITION 9 // TestDynamic and then same fields of MultEnvCondition= command
			// except for Id field that it will be returned from the service and the END_LIST value to type at end of the array
		// ENV_ condition, DistanceForEnv, Extra field, array of tripled of {ENV_ Condition, DistanceForEnv, Extra field}

#define SRV_CREATE_TESTPOSITION 10 // TestDynamic and then same fields of TestPosition command
			// except for Id field that it will be returned from the service
			// Flags (TPOS_...), Slot Moveable, XDistanceMin, XDistanceMax, YDistanceMin, YDistanceMax, ZDistanceMin, ZDistanceMax,  HOrientDiffMin, HOrientDiffMax, VOrientDiffMin, VOrientDiffMax, ROrientDiffMin, ROrientDiffMax


#define SRV_CREATE_ADDEFFECT 12 // TestDynamic and then same fields of AddEffect command
			// except for Id field that it will be returned from the service
			// and the END_LIST value at end 
			// EffectType (ADD_), FlagsEffect (FADD_), JointType (JOINT_), DispX, DispY, DispZ, DurateEmit, DuratePause, Extra param array

#define SRV_SetReservedDataZone 13 // You pass the startOffset and number of bytes
			// of tomb4 data that you wish set as "used" to other plugin dll
			// StartOffset, NBytes


#define SRV_F_ProporzioneDistanza 14  //       (int Incremento, int Distanza);
#define SRV_F_EseguiTriggerGroup 15
#define SRV_F_DetectedGlobalTriggerEvent 16
#define SRV_F_InviaErroreLog 17

#define SRV_CREATE_PARAM_COMMAND 18 // creare Parameters=PARAM_.. command of trng owner to have an extra input 
										//for trng triggers requiring parameters from script
										// Input:  TestDynamic and then the PARAM_ value, then skip the id, and type other arguments
										// for current parameter. It will return the id of new PARAM command
										// or -1 if there is an error
#define SRV_F_TestEnvCondition 19  
#define SRV_F_IsBoxSettore 20
#define SRV_F_VerificaTestPosition 21 
#define SRV_F_CollideItemConCustom 22
#define SRV_F_IsCollisioneConItems 23 
#define SRV_F_InviaLog 24
#define SRV_F_EseguiAnimNemico 25
#define SRV_F_CreateAIRecord 26
#define SRV_F_DeleteAIRecord 27
#define SRV_F_CreateNewMoveable 28
#define SRV_F_DeleteNewMoveable 29
#define SRV_CREATE_COLOR_RGB_COMMAND 30 // arguments: TestDynamic and then Red, Green, Blue, it will return Id for new colorrgb
#define SRV_DeleteParamCommand 31
#define SRV_DeleteTriggerGroup 32
#define SRV_DeleteColorRgb 33
#define SRV_DeleteMultEnvCondition 34
#define SRV_DeleteTestPosition 35
#define SRV_DeleteAddEffect 36
#define SRV_F_DisableSaving 37
#define SRV_F_AggiungiItemMosso 38
#define SRV_F_CheckForStartMovePushable 39
#define SRV_F_CheckForEndMovePushable 40
#define SRV_F_ExplosionOnVehicle 41
#define SRV_F_ConvertMicroUnits 42
#define SRV_F_AllocateImage 43
#define SRV_F_FreeImage 44
#define SRV_F_AllocateTombHdc  45
#define SRV_F_FreeTombHdc 46
#define SRV_F_DrawSprite2D 47
#define SRV_F_DrawMesh3D 48
#define SRV_F_DrawObject2D 49
#define SRV_F_PrintText 50
#define SRV_F_ReadDxInput 51
#define SRV_F_SuspendAudioTrack 52
#define SRV_F_ResumeAudioTrack 53
#define SRV_F_CreateWindowsFont 54
#define SRV_F_FreeWindowsFont 55
#define SRV_F_ReadInputBox 56

#define SRV_F_ReadNumVariable 57
#define SRV_F_WriteNumVariable 58
#define	SRV_F_ReadTextVariable 59
#define SRV_F_WriteTextVariable 60
#define SRV_F_ReadMemVariable 61
#define SRV_F_WriteMemVariable 62

#define SCRIPT_CODE      0x4000 // added with or "|" to trigcode convert it to a script code for trng variables
#define MASK_SCRIPT_CODE 0x3FFF // to remove flag script code da code value
// direct call required from trng to perform in your plugin (it is like the opposite of above services)
#define NGDC_APPLY_PATCHES  0x0001  // return eax=1 for ok, eax=0 for errors

// valori di frame per tight-rope dove controllare
// se il giocatore sta premendo il tasto giusto
#define FRAME_OCB1 17
#define FRAME_OCB2 13


#define CODE_FLIPEFFECT 0x0400
#define CODE_ACTION     0x0800
#define CODE_CONDITION  0x1000

#define CODE_MASK_INDEX 0x03FF

// MPS_ flag
// WARNING: this is only one flag set by tomb_nextgeneration engine.
// you should type (choosing them for your targets) other MPS_ flags but yours, 
// you should type them in "Constants_mine.h" file
// taking care to do not use the only one already set value 0x40000000

#define MPS_DISABLE 0x40000000  // disable (ignore) this plugin

// flag per sound sample flags
#define FSI_L   0x0001
#define FSI_R   0x0002
#define FSI_RAD 0x0004
#define FSI_CH  0x0008
#define FSI_N   0x1000
#define FSI_P   0x2000
#define FSI_V   0x4000

#define END_LIST -99999
#define SLOT_STATIC 1000

// flags for read input box RIB
#define RIB_ONLY_DIGITS  0x0001  // ignores letters (ABCDEF ...Z) (by default accepts letters and digits)
#define RIB_ONLY_LETTERS 0x0002  // ignores digits (0123456789)  (by default accepts letters and digits)
#define RIB_HIDE_CARET   0x0004  // by default caret is visible
#define RIB_BLINK_CARET  0x0008 // perform a blinking of caret while it's waiting input
#define RIB_SOUND_ON_KEY 0x0010  // little sound for each typed character
#define RIB_PRINT_ONLY   0x0020  // ignore input and print newly the current buffer in given rect (works like OutText() only)
#define RIB_ALIGN_CENTER 0x0040  // center aligment in pRect boundary. (by default uses a LEFT align)
#define RIB_ONLY_CAPS    0x0080  // force capital letters. By default it will be typed lower or capital letters
								 // in according with SHIFT key
#define RIB_PRINT_BIG_TEXT 0x0100 // start printing the "TextBig" trng variable
#define RIB_INPUT_BELOW_BIG_TEXT 0x0200 // require input in Y position after big text
#define RIB_ADD_SCANCODE_LIST   0x0400  // parses extra scan codes supplied in extra ng string 666
#define RIB_SHORTCUT_KEY  0x0800 // just typing a single (valid) key to have confirmed the input like with ENTER
#define RIB_HIDE_TEXT     0x1000  // not draw the inserted text but manage it as usual
// values for pLaraLocationFlags
#define LLF_GROUND			0  // lara outside water, no special situation
#define LLF_UNDERWATER		1 // lara is underwater
#define LLF_FLOATING		2 // lara is floating on the water surface
#define LLF_DOZY			3  // lara has some special status: dozy e perhaps others
#define LLF_LOW_WATER		4  // lara is walking on low water (no floating)


#define MIR_WEST_WALL 0
#define MIR_FLOOR 1
#define MIR_CEILING 2
#define MIR_INVERSE_WEST 3
#define MIR_EAST_WALL 4
#define MIR_SOUTH_WALL 5
#define MIR_NORTH_WALL 6
// flag aggiunti a indici di animating di mirror per attuare aggiustamenti
#define FMIR_MASK_INDEX 0xFFF
#define FMIR_MASK_FLAGS 0x7000
#define FMIR_ALTERNATE_REFLEX 0x1000 
#define FMIR_ADJUST_X 0x2000
#define FMIR_ADJUST_Z 0x4000

// valori di ritorno per redirwindowproc
#define RET_ABORT_FALSE 0x80000000
#define RET_ABORT_TRUE  0x80000001
#define RET_DEF_WINDOWPROC 0x0
#define RET_CONTINUE 0x1

// flag per ScriptMain 
#define SMAIN_FLY_CHEAT 0x0001  
#define SMAIN_LOAD_SAVE 0x0002
#define SMAIN_TITLE     0x0004
#define SMAIN_PLAY_ANY_LEVEL 0x0008
#define SMAIN_DEMO_DISK 0x0080

// flags per scriptlevel
#define SLEV_YOUNG_LARA 0x0001
#define SLEV_WEATHER   0x0002
#define SLEV_HORIZON  0x0004
#define SLEV_LAYER1   0x0008
#define SLEV_LAYER2   0x0010
#define SLEV_STAR_FIELD  0x0020
#define SLEV_LIGHTNING 0x0040
#define SLEV_TRAIN    0x0080
#define SLEV_PULSE    0x0100
#define SLEV_COL_ADD_HORIZON  0x0200
#define SLEV_RESET_HUB 0x0400
#define SLEV_LENS_FLARE  0x0800
#define SLEV_TIMER    0x1000 
#define SLEV_MIRROR   0x2000
#define SLEV_REMOVE_AMULET 0x4000
#define SLEV_NO_LEVEL 0x8000

// flag per room buttons of room structure
#define	FROOM_COLD         0x1000  
#define	FROOM_RAIN          0x0800  
#define	FROOM_SNOW          0x0400  
#define	FROOM_REFLECTION          0x0200  
#define	FROOM_MIST          0x0100  
#define	FROOM_NON_LENS_FLARE          0x0080  
#define	FROOM_MISTERY          0x0040  
#define	FROOM_OUTSIDE          0x0020  
#define	FROOM_DAMAGE          0x0010  
#define	FROOM_HORIZON          0x0008  
#define	FROOM_QUICKSAND          0x0004  
#define	FROOM_IS_FLIPROOM          0x0002  
#define	FROOM_WATER          0x0001  



// constant to check the bit used to recognize extrang strings
#define STRING_NG 0x8000
#define MASK_STRING_INDEX 0x7fff  // bit mask to get index of string removing further STRING_NG flag

// costanti per singole operazioni che devono essere eseguite una volta
#define ON_NONE 0
#define ON_SAVE_BACKGAME 1
#define ON_LOAD_BACKGAME 2
#define ON_SHOW_SCREEN_AT_START 3
#define ON_WAIT_AND_HIDE 4

#define ON_WAIT_ESCAPE 6

// flaga for weapon in inventory
#define FWEAP_PRESENT  0x01  // weapon is present
#define FWEAP_LASERSIGHT 0x04  // weapon has mounted the laser sight
#define FWEAP_AMMO_NORMAL 0x08  // available normal ammos
#define FWEAP_AMMO_SUPER 0x10  // for shotgun or grenadegun
#define FWEAP_AMMO_EXPLOSIVE 0x20  // for crossbow, explosive while for grenadegun, flash ammo

// flags for clockwork beetle of inventory
#define CBEET_BEETLE_WITH_KEY  0x01  // the beetle is present and has the key already mounted
#define CBEET_KEY_SINGLE  0x02  // is present the key but divided by beetle
#define CBEET_BEETLE_SINGLE 0x04  // is present the beetle but has not mounted the key


// flags for small and large water skin of inventory
#define WSKIN_MISSING 0x00
#define WSKIN_EMPTY_WATER_SKIN 0x001 // it's present but it is empty
#define WSKIN_ONE_LITER 0x002  // present with one liter
#define WSKIN_THREE_LITERS 0x004 // present with three liters
#define WSKIN_FIVE_LITERS 0x006 // present with five liters

// flags (to test with & operator) to verify the presence of combo items for puzzle
#define COMBO_ITEM1_FOR_PUZZLE_1   0x0001
#define COMBO_ITEM2_FOR_PUZZLE_1   0x0002
#define COMBO_ITEM1_FOR_PUZZLE_2   0x0004
#define COMBO_ITEM2_FOR_PUZZLE_2   0x0008
#define COMBO_ITEM1_FOR_PUZZLE_3   0x0010
#define COMBO_ITEM2_FOR_PUZZLE_3   0x0020
#define COMBO_ITEM1_FOR_PUZZLE_4   0x0040
#define COMBO_ITEM2_FOR_PUZZLE_4   0x0080
#define COMBO_ITEM1_FOR_PUZZLE_5   0x0100
#define COMBO_ITEM2_FOR_PUZZLE_5   0x0200
#define COMBO_ITEM1_FOR_PUZZLE_6   0x0400
#define COMBO_ITEM2_FOR_PUZZLE_6   0x0800
#define COMBO_ITEM1_FOR_PUZZLE_7   0x1000
#define COMBO_ITEM2_FOR_PUZZLE_7   0x2000
#define COMBO_ITEM1_FOR_PUZZLE_8   0x4000
#define COMBO_ITEM2_FOR_PUZZLE_8   0x8000

// flags (to test with & operator) to verify the presence of key combo items
#define KCOMBO_ITEM1_FOR_KEY_1   0x0001
#define KCOMBO_ITEM2_FOR_KEY_1   0x0002
#define KCOMBO_ITEM1_FOR_KEY_2   0x0004
#define KCOMBO_ITEM2_FOR_KEY_2   0x0008
#define KCOMBO_ITEM1_FOR_KEY_3   0x0010
#define KCOMBO_ITEM2_FOR_KEY_3   0x0020
#define KCOMBO_ITEM1_FOR_KEY_4   0x0040
#define KCOMBO_ITEM2_FOR_KEY_4   0x0080
#define KCOMBO_ITEM1_FOR_KEY_5   0x0100
#define KCOMBO_ITEM2_FOR_KEY_5   0x0200
#define KCOMBO_ITEM1_FOR_KEY_6   0x0400
#define KCOMBO_ITEM2_FOR_KEY_6   0x0800
#define KCOMBO_ITEM1_FOR_KEY_7   0x1000
#define KCOMBO_ITEM2_FOR_KEY_7   0x2000
#define KCOMBO_ITEM1_FOR_KEY_8   0x4000
#define KCOMBO_ITEM2_FOR_KEY_8   0x8000

// flag to test the presence of combo items for pickups
#define PCOMBO_ITEM1_FOR_PICKUP_1   0x0001
#define PCOMBO_ITEM2_FOR_PICKUP_1   0x0002
#define PCOMBO_ITEM1_FOR_PICKUP_2   0x0004
#define PCOMBO_ITEM2_FOR_PICKUP_2   0x0008
#define PCOMBO_ITEM1_FOR_PICKUP_3   0x0010
#define PCOMBO_ITEM2_FOR_PICKUP_3   0x0020
#define PCOMBO_ITEM1_FOR_PICKUP_4   0x0040
#define PCOMBO_ITEM2_FOR_PICKUP_4   0x0080
#define PCOMBO_ITEM1_FOR_PICKUP_5   0x0100
#define PCOMBO_ITEM2_FOR_PICKUP_5   0x0200
#define PCOMBO_ITEM1_FOR_PICKUP_6   0x0400
#define PCOMBO_ITEM2_FOR_PICKUP_6   0x0800
#define PCOMBO_ITEM1_FOR_PICKUP_7   0x1000
#define PCOMBO_ITEM2_FOR_PICKUP_7   0x2000
#define PCOMBO_ITEM1_FOR_PICKUP_8   0x4000
#define PCOMBO_ITEM2_FOR_PICKUP_8   0x8000

// flaga to test EXistence of items in 16 bit mask word
#define EX16_EXIST_1  0x0001
#define EX16_EXIST_2  0x0002
#define EX16_EXIST_3  0x0004
#define EX16_EXIST_4  0x0008
#define EX16_EXIST_5  0x0010
#define EX16_EXIST_6  0x0020
#define EX16_EXIST_7  0x0040
#define EX16_EXIST_8  0x0080
#define EX16_EXIST_9  0x0100
#define EX16_EXIST_10 0x0200
#define EX16_EXIST_11 0x0400
#define EX16_EXIST_12 0x0800
#define EX16_EXIST_13 0x1000
#define EX16_EXIST_14 0x2000
#define EX16_EXIST_15 0x4000
#define EX16_EXIST_16 0x8000

// values for COlor FOrmat
#define COLF_WINDOWS_COLOR  0 // format 0x00RRGGBB windows functions requires this format
#define COLF_TOMB_COLOR     1 // format 0x00BBGGRR tomb4 functions requirs this format


// value for *Trng.pGlobTomb4->pAdr->pFlagsLaraHands  variable
#define FLH_ALMOST_FREE_HANDS 0  // no weapon or torch but she could hold the flare
#define FLH_IS_GRABBING		1		// lara is grabbing something (torch, wall, ceiling but not weapons or flare)
#define FLH_IS_EXTRACTING 2  // lara is extracting (or picking up) some weapon or torch (but not flare)
#define FLH_IS_THROWING 3  // lara is throwing away current weapon or torch (not affected by flare)
#define FLH_HOLDS_ITEM  4  // lara is holding some weapon or torch (but this is not affected by flare)

// constants for Get() function (note: you can use also enumGET autoenumerate 
#define GET_LARA  0					// returns values in GET.pLara->  and GET.LaraIndex    INPUT: none
#define GET_ITEM 1					// returns value in GET.pItem->   (moveable object).INPUT: Index = index of moveable + (NGLE_INDEX if it is a ngle index) 
#define GET_STATIC 2				// returns value in GET.pStatic->  (static object) INPUT: Index = (index of static + NGLE_INDEX) or (Index=Room), SecondaryIndex = static index if it was missing NGLE_INDEX, or unused
#define GET_ROOM 3					// returns value in GET.pRoom->   (room structure) INPUT: Index = index of room (no NGLE_INDEX, use the second number you see in room list of ngle program)
#define GET_ITEM_COLL_BOX 4			// returns value in GET.pCollItem->   (collision box of moveable item)  INPUT: same of GET_ITEM
#define GET_STATIC_COLL_BOX 5		// returns value in GET.pCollStatic->  (collision box of static item) INPUT: same input of GET_STATIC
#define GET_STATIC_VIEW_BOX 6       // returns value in GET.pViewStatic->  (view box of static item) INPUT: same input of GET_STATIC
#define GET_DOOR_OF_ROOM 7			// returns value in GET.pDoor->  (structure of "room" door) INPUT: Index = index of room, SecondaryIndex = index of door
#define GET_INFO_LARA 8             // returns values in GET.LaraInfo.  (structure with various infos about lara) INPUT: none
#define GET_MY_PARAMETER_COMMAND 9    // returns value in GET.pParam->   INPUT: Index= PARAM_ value, SecondaryIndex = (optiona) id (first field after PARAM_ value) of Parameters command or -1 if you wish omit this input value
#define GET_MY_CUSTOMIZE_COMMAND 10 // returns value in GET.pCust->  INPUT: Index = CUST_ value, SecondayIndex = (optional) value of first field after CUST_ value or -1 if you omit this input value
#define GET_INPUT 11 // returns value in GET.Input.  INPUT: none
#define GET_STRINGNG 12 // return value in GET.pStringNG  INPUT: Index= Index of ng string
#define GET_STRING  13 // return value in GET.pString INPUT: Index= Index of pc/psx string
#define GET_BIG_NUMBER 14 // return value in GET.BigNumber  INPUT: Index= index of number in Parameters=PARAM_BIG_NUMBERS script command. First number has index=0
#define GET_SCRIPT_SETTINGS 15 // return values in GET.ScriptSettings. INPUT: none
#define GET_PROGRESSIVE_ACTION 16 // return value in GET.pAction->  INPUT: none (creates a new progressive action record)
#define GET_AI_ITEM 17   // return value in GET.pAI->  INPUT: index = index of AI record in AI array (got from Find() function)
#define GET_LIGHT 18  // return value in GET.pLight->  INPUT: index = index of Room  SecondaryIndex=Index of light record of given room (got from Find() function)
#define GET_CAMERA 19 // return value in GET.pCamera-> INPUT: index = index of fixed camera got from Find() function
#define GET_CAMERA_FLY 20 // return value in GET.pCameraFly->  INPUT: index = index of flyby camera got from Find() function
#define GET_COLOR_RGB 21 // return value in GET.Color  INPUT: Index = Id of ColorRgb script command, SecondaryIndex = ColorFormat (enumCOLF_..)
#define GET_GAME_INFO 22  // return values in GET.GameInfo.  INPUT: none
#define GET_SLOT 23  // return value in GET.pSlot->  INPUT: index = SLOT_ value
#define GET_INVENTORY  24 // return values in GET.InventoryData.  and (old access) in GET.pInventory->  INPUT: none
#define GET_ANIMATION 25 // return values in GET.pAnimation->  INPUT: Index of absolute animation to locate
#define GET_INFO_ITEM 26 // return values in GET.InfoItem.  INPUT: Index of moveable item to analyse
#define GET_VARIABLES 27 // return values in GET.Vars struture. Used to access to trng variables and to have infos about selected items of memory zones
#define GET_FLIPMAPS 28 // return values in GET.pFlipMap struture. Used to access to all flipmap infos. No required input values
// flag for index for Get() locator and Find() functions
#define NGLE_INDEX 0x4000  // if the index you are giving to Get() or Find() is a ngle index, add this value to the index
#define MASK_NGLE_INDEX  0x3FFF  // (internal usage: to get only index value from a index+NGLE_INDEX flag)

// constants for Convert() function (note: you can also using the self enumerate enumCONV  to select a convert constant
#define CONV_ItemIndexFromNgleToTomb  1 // Input: Index= Ngle index. Output: returns the tomb4 corresponding index
#define CONV_ItemIndexFromTombToNgle  2 // Input: Index= Tomb Index. Output: returns the corresponding ngle index
#define CONV_StaticIndexFromNgleToTomb 3 // Input: Index= Ngle Static Index / pPointer = &ReturnedStaticIndex. Output: returns: Room Index  / *pPointer = (int) Tomb Static Index
#define CONV_StaticIndexFromTombToNgle 4 // Input: Index=RoomIndex  / SecondaryIndex=StaticIndex. Output: returns ngle static index
#define CONV_RectFromMicroUnitsToPixels 5 // input: pPointer = &RECT with microunits Output: changed value in rect pointed by pPointer
#define CONV_AnimIndexFromRelativeToAbs  6 // input: Index=Relative anim index / pPointer: pointer to item structure. Output: returns Absolute animation index
#define CONV_AnimIndexFromAbsToRelative 7 // input: Index=Absolute anim index / pPointer: pointer to item structure. Output: returnns Relative animation index
#define CONV_ItemFromStrItemTr4ToIndex 8 // input: pPointer = StrItemTr4 *pItem. Output: returns item index of given structure
#define CONV_RoomIndexFromNgleToTomb 9 // input: Index=NgleRoomIndex. Output: returns Tomb room index
#define CONV_RoomIndexFromTombToNgle 10 // input: Index=TombRoomIndex, Output: returns Nlge room index
#define CONV_FrameIndexFromAbsToRelative 11 // input: Index=AbsFrameIndex / SecondaryIndex=AbsAnimationIndex. Output: returns RelativeFrameIndex
#define CONV_FrameIndexFromRelativeToAbs 12 // input: Index=RelFrameIndex / SecondaryIndex=AbsAnimationIndex. Output: returns AbsoluteFrameIndex

// constants for Find() function (note: you can use also enumFIND auto enumerate)
#define FIND_ITEM  0  // return values in FIND.VetItems and FIND.TotItems Looks for moveable items
							// INPUT: SlotType=SLOT_ (or -1), RoomIndex (or -1), Ocb (or -1)
#define FIND_STATIC 1 // return values in FIND.VetStatics and FIND.TotStatics Looks for static items
							// input: SlotType=SSLOT_ (or -1), RoomIndex (or -1), Ocb (or -1)
#define FIND_AI 2  // return values in FIND.VetAI and FIND.TotAI  Looks for AI_ items and LARA_START_POS items
							// input: SlotType= SLOT_ , RoomIndex (or -1), Ocb (or -1)
#define FIND_LIGHT  3 // return values in FIND.VetLigths and FIND.TotLights  look for lights, fog bulbs and light effects
							// input: SlotType= enumD3D. light type (or -1), RoomIndex = index of room (or -1)
#define FIND_CAMERA_FIXED 4 // return values in FIND.VetCameraFix and FIND.TotCameraFix  
								// input: RoomIndex = number of room (required), ignored other arguments

#define FIND_CAMERA_FLY 5  // return values in FIND.VetCameraFly  and FIND.TotCameraFly
									// input: SlotType = Id of Sequence (or -1), RoomIndex = index of room (or -1), Ocb = Camera Buttons (or -1), Extra = Number of camera in the sequence (or -1)
#define FIND_ITEMS_NEARBY 6 // returned values in FIND.VetItems and FIND.TotItems, looks for moveables closed to (x,y,z) point given as pPointer
									// input: RoomIndex = room where is the point to check; Extra = max distance from input point  pPointer= pointer to (x,y,z) of source point to analyse
#define FIND_ITEMS_SECTOR 7 // returned values in FIND.VetItems and FIND.TotItems, looks for all moveable with pivot in sector where falls the point given as pPointer
									// input: RoomIndex = room where is the point to check; pPointer (pointer to structure with (x,y,z) point to locate the sector where to look for items
#define FIND_STATICS_NEARBY 8 // returned values in FIND.VetStatics and FIND.TotStatics, looks for statics closed to (x,y,z) point given as pPointer
									// input: RoomIndex= room where is the point to check; Extra = max distance from input point, pPointer (pointer to structure with (x,y,z) point of source point o analyse
#define FIND_STATICS_SECTOR 9  // returned values in FIND.VetStatics and FIND.TotStatics, looks for all statics with pivot in sector where falls the point given as pPointer
									// input: RoomIndex = room where is the point to check; pPointer (pointer to structure with (x,y,z) point to locate the sector where to look for statics 								
#define FIND_ENEMY_COMMAND 10 // returned value in FIND.pEnemy. Look for Enemy script command with Slot value

#define FIND_IMAGE_COMMAND 11 // reurned value in FIND.pImage. Look for Image script command with given Id; Extra = IdImage
#define FIND_WINDOWS_FONT 12 // returned value in FIND.pWindowsFont. Look for WindowsFont script command with IdWindowsFont; Extra = IdWindowsFont to look for
#define FIND_DIAGNOSTIC 13 // returned value in FIND.Dgx. Look for Diagnostic, DiagnosticType and LogItem commands; no input required
#define FIND_PROGR_ACTION_TRNG 14 // returned value in FIND.pVetProgAxnTrng[] FIND.TotProgAxnTrng input: SlotType= AZ_ action type, Extra = Index of "ItemIndex" field or -1 if ignore 
#define FIND_PROGR_ACTION_MINE 15 // returned values in FIND.pVetProgrAxnMine[] FIND.TotProgrAxnMine  input: SlotType = AXN_ action type, Extra = index of "ItemIndex" field or -1 if ignore
#define FIND_ASSIGN_SLOT_MINE 16 // returned values in FIND.SlotAssigned  Input: SlotType= OBJ_Type to look for
// flag per azione progressiva di movimento animating
#define	fmov_CordX  0x0001
#define	fmov_CordY  0x0002
#define	fmov_CordZ  0x0004
#define	fmov_Up_Down 0x008
#define	fmov_For_Back 0x0010

// flag per disabilitare caratteristiche di lara in gioco
#define DF_GUARDA  0x0001
#define DF_COMBAT_CAMERA 0x0002


// light type (tr4 file)

#define D3D_NO_LIGHT -1
#define D3D_SUN     0x0000  
#define D3D_LIGHT   0x0001  
#define D3D_SPOT_OR_EFFECT 0x0002  
#define D3D_SHADOW   0x0003  
#define D3D_FOG_BULB 0x0004  

// flag FADD_ per comando script AddEffect=
#define FADD_NONE 0

#define FADD_NO_SOUND            0x0001 // solo per uso interno
#define FADD_IGNORE_STATUS       0x0002 
#define FADD_FIRE_STRIP          0x0004
#define FADD_VORIENT_90          0x0008
#define FADD_VORIENT_180         0x0010
#define FADD_VORIENT_270         0x0020

#define FADD_CONTINUE_EMIT       0x0100
#define FADD_DURATE_STATEID      0x0200
#define FADD_SMOKE_EXHAUST       0x0400
#define FADD_ROTATE_270          0x0800
#define FADD_DURATE_SINGLE_FRAME 0x1000
#define FADD_ROTATE_90           0x2000
#define FADD_ROTATE_180          0x4000
#define FADD_DURATE_ANIMATION    0x8000

// costanti per indici relativi a coppia claspriority/threadpriorirty
#define  NONE_PRIORITY  0
#define    Idle_IDLE  1
#define    Idle_LOWEST  2
#define    Idle_BELOW_NORMAL = 3
#define    Idle_NORMAL  4
    
#define    Normal_LOWEST  5
#define    Normal_BELOW_NORMAL  6
#define    Normal_NORMAL  7
#define    Normal_ABOVE_NORMAL  8
#define    Normal_HIGHEST  9
    
#define    High_BELOW_NORMAL  10
#define    High_NORMAL  11
#define    High_ABOVE_NORMAL  12
#define    High_TIME_CRITICAL  13
#define    RealTime_IDLE  14
#define    OVERFLOW_PRIORITY  15

// costante a volte restituita da funzione GetHeight 
// che vuol dire che nel settore analizzato c'e' un muro
#define WALL_FLOOR -32512 

#define SWAP_NORMALE 0
#define SWAP_SKIN_JOINT 1
#define SWAP_SKIN_JOINT_HAIR 2
#define SWAP_SKIN_JOINT_HAIR_HEAD 3
#define SWAP_SKIN_HEAD 4
#define SWAP_SKIN_HEAD_HAIR 5
#define SWAP_OBJ_JOINT 6

// valori per comando script HAIR=
#define HAIR_PAGE_BOY 1
#define HAIR_TWO_PLAITS 2
#define HAIR_ONE_PONYTAIL 3
#define HAIR_ONE_TR5_PONYTAIL 4
#define HAIR_NON_CAMBIARE 0xff
// valori FKP  per keypad
#define FKP_ATTESA 0
#define FKP_ATTENDI_FINE_ESCAPE 1
#define FKP_ANIMAZIONE_TASTO 2
#define FKP_RIMUOVI 3
#define FKP_WAIT_ANIMATION 4
#define FKP_WAIT_END_ANIMATION 5

// costanti per indici di vettore puntatori di lara
#define MESH_BUTT 0					// [0]
#define MESH_RIGHT_THIGH 1			// [1]
#define MESH_RIGHT_CALF 2			// [2]
#define MESH_RIGHT_FOOT 3				// [3]
#define MESH_LEFT_THIGH 4			// [4]
#define MESH_LEFT_CALF 5		// [5]
#define MESH_LEFT_FOOT 6			// [6]
#define MESH_CHEST 7					// [7]
#define MESH_RIGHT_ARM 8			// [8]
#define MESH_RIGHT_FOREARM 9		// [9]
#define MESH_RIGHT_HAND 10				// [10]
#define MESH_LEFT_ARM 11		// [11]
#define MESH_LEFT_FOREARM 12	// [12]
#define MESH_LEFT_HAND 13			// [13]
#define MESH_HEAD 14					// [14]

// costanti usate per trigger condition e per gobal trigger
#define HOLD_PISTOLS 1
#define HOLD_REVOLVER 2
#define HOLD_UZI 3
#define HOLD_SHOTGUN 4
#define HOLD_GRENADEGUN 5
#define HOLD_CROSSBOW 6
#define HOLD_FLARE 7
#define HOLD_OUT_TORCH 8
#define HOLD_FIRED_TORCH 9
#define HOLD_JEEP 10
#define HOLD_SIDECAR 11
#define HOLD_RUBBER_BOAT 12
#define HOLD_MOTOR_BOAT 13
#define HOLD_ROPE 14
#define HOLD_POLE 15
#define HOLD_ANY_TORCH 16
#define HOLD_KAYAK 17

// flags for *Trng.pGlobTomb4->pAdr->pFlagsLara variable
// note: not all meanings of following flags are sure
#define FL_HOLDS_FLARE_OR_TORCH  0x01
#define FL_UNKNOWN_02            0x02  // never used in tomb4
#define FL_SPECIAL_STATE         0x04  // not clear the meaning
#define FL_IS_BURNING            0x08 
#define FL_ON_ALL_FOUR           0x10  // not sure
#define FL_SELF_ALIGNMENT        0x20  // not sure
#define FL_UNDER_MONKEY_SECTOR   0x40 

// flags for *Trng.pGlobTomb4->pAdr->pFlagsLara2 variable 
#define FL2_OVER_MINE_TRIGGER  0x0001
#define FL2_IS_BURNING    0x0002
#define FL2_IS_DUCK       0x0004
#define FL2_IS_INVULNERABLE   0x0008
#define FL2_IS_USING_LASER_SIGHT_OR_BINOCULARS 0x0010
#define FL2_TORCH_IS_BURNING 0x0020
#define FL2_IS_CLIMBING 0x0040


// flags TKP  Tipo Key Pad da passare a GestioneKeyPad
#define TKP_ELEVATOR 1
#define TKP_SWITCH  2

// flage EF_.. per elevtore (comando script)
#define EF_NONE 0
#define EF_MULTI_DOORS  0x0001
#define EF_SINGLE_DOOR  0x0002
#define EF_INNER_KEYPAD 0x0004
#define EF_MODE_YO_YO 0x0008
#define EF_MODE_STOP_AND_GO 0x0010
#define EF_DOUBLE_DOOR 0x0020
#define EF_ANIM_DOOR 0x8000  // flag interno

#define TD_AMULETO 0
#define TD_BIG_DETECTOR 1
#define TD_LITTLE_DETECTOR 2


// flags per CUST_RAIN command
#define FR_PLAY_SFX 0x0001
#define FR_ADD_DRIPS_TO_LARA 0x0002
#define FR_CORRECT_SPRINKLERS 0x004

// costants MUN used as index for pVetAmmos of pAdr->
#define MUN_PISTOLS				0
#define MUN_UZI      			1
#define MUN_REVOLVER			2
#define MUN_SHOTGUN_NORMALS		3
#define MUN_SHOTGUN_SUPER	4
#define MUN_GRENADE_NORMALS		5
#define MUN_GRENADE_EXPLOSIVE	6
#define MUN_GRENADE_LIGHT	7
#define MUN_CROSSBOW_NORMALS	8
#define MUN_CROSSBOW_POISONED 9
#define MUN_CROSSBOW_EXPLOSIVE 10

#define SAVT_LOCAL_DATA 0x0001  // require to save only local data
#define SAVT_GLOBAL_DATA 0x0002 // require to save only global data
#define SAVT_COMPLETED 0x00800   // signal only to have completed the saving, now our callback can free temporary memory to store data to save

#define LOADT_LOCAL_DATA 1 // data loaded are local data
#define LOADT_GLOBAL_DATA 2 // data loaded are global data

// valori flag per TextureSequence= comando script
#define SEQ_LOOP 0x0001
#define SEQ_LOOP_INVERSE 0x0002
#define SEQ_STOP_AT_FIRST 0x0004
// valore per flag Elevator status
#define EST_ATTESA  0x00
#define EST_ATTENDI_KEYPAD 0x001
#define EST_ATTENDI_ALLINEAMENTO_LARA 0x002
#define EST_ATTENDI_DELAY 0x003
#define EST_MOVIMENTO 0x004
#define EST_INIZIO_MOVIMENTO 0x005
#define EST_OPEN_AND_STOP 0x006
#define EST_CLOSE_AND_GO 0x7
#define EST_STOP 8

// flag per tabella di remap tail
#define REMAP_FRAMMENTO 0x400
#define REMAP_TRIANGOLO 0x1000
#define REMAP_TRASPARENTE 0x2000
#define REMAP_SPECULARE 0x4000

// --- flag per sezione livello
// FNGL  = Flag NG Level
#define    fngl_NONE 0
#define    fngl_Rain 			0x0001
#define    fngl_Snow			0x0002
#define    fngl_VolumetricFX_Enable 0x0004
#define    fngl_VolumetricFX_Disable 0x0008
#define    fngl_Rain_All        0x0010
#define    fngl_Snow_All		0x0020
#define	   fngl_CutScene  		0x0040
#define    fngl_PreserveInventory  0x0080

// valori per script command AddEffect  tipo di effetto da aggiungere
#define ADD_FLAME 1
#define ADD_SMOKE 2
#define ADD_BLOOD 3
#define ADD_MIST 4
#define ADD_LIGHT_FLAT 5
#define ADD_LIGHT_BLINK 6
#define ADD_LIGHT_SPOT 7
#define ADD_LIGHT_GLOVE 8

// costanti per indie di joint da usare con funzione 
#define JOINT_LEFT_THIGH 1
#define JOINT_LEFT_KNEE 2
#define JOINT_LEFT_ANCKLE 3

#define JOINT_RIGHT_THIGH 4
#define JOINT_RIGHT_KNEE 5
#define JOINT_RIGHT_ANCKLE 6

#define JOINT_ABDOMEN 7
#define JOINT_NECK 8

#define JOINT_LEFT_SHOULDER 9
#define JOINT_LEFT_ELBOW 10
#define JOINT_LEFT_WRIST 11

#define JOINT_RIGHT_SHOULDER 12
#define JOINT_RIGHT_ELBOW 13
#define JOINT_RIGHT_WRIST 14
// flag per size character in textformat= script command
#define SC_NORMAL 0
#define SC_HALF_WIDTH 1
#define SC_HALF_HEIGHT 2
#define SC_HALF_SIZE 3
#define SC_DOUBLE_WIDTH 4
#define SC_DOUBLE_HEIGHT 5
#define SC_DOUBLE_SIZE  6

// flag di posizione
#define STRING_BOTTOM_CENTER 1
#define STRING_TOP_CENTER    2
#define STRING_CENTER_CENTER 3
#define STRING_TOP_LEFT      4
#define STRING_TOP_RIGHT     5
#define STRING_BOTTOM_LEFT   6
#define STRING_BOTTOM_RIGHT  7
#define STRING_DOWN_DAMAGE_BAR 8
#define STRING_DOWN_COLD_BAR 9
#define STRING_DOWN_LEFT_BARS 10
#define STRING_DOWN_RIGHT_BARS 11

// valori per flags da passare a printstring
#define FTS_ALIGN_LEFT  0x0000
#define FTS_STRETCH_TEXT 0x1000  
#define FTS_BLINK       0x2000  
#define FTS_ALIGN_RIGHT 0x4000 
#define FTS_ALIGN_CENTER 0x8000 

// valori di FT_ flags di comando script TextFormat dopo la modifica
#define FT_HALF_SIZEX  0x01   
#define FT_HALF_SIZEY  0x02
#define FT_DOUBLE_SIZEX 0x04
#define FT_DOUBLE_SIZEY 0x08
#define FT_LITTLE_ALWAYS 0x10  // caratteri forzati sempre a dimensione ok per keypad
#define FT_USE_FLOAT  0x20 // usa i valori di rapporto globali
#define FT_NO_BORDERS    0x40
#define FT_SIZE_ATOMIC_CHAR 0x80 
// flag per campo RapportoSize di dati font binary letti da sprite 
#define FONT_SIZE_TOMB 0x00
#define FONT_SIZE_LITTLE 0x10
#define FONT_SIZE_BIG 0x20
#define FONT_SIZE_MASK 0xF0

#define FONT_LAYOUT_TOMB 0x00
#define FONT_LAYOUT_SQUARE 0x01
#define FONT_LAYOUT_HIGH_RECT 0x02
#define FONT_LAYOUT_WIDE_RECT 0x03
#define FONT_LAYOUT_MASK 0x0F

// flag di font binary
#define ff_DisableShadeGraphic 0x01

// flag interni per flag
// valroi per tipo sprite per funzione DisegnaSprite
#define SPR_TargetGraphics 0
#define SPR_DefaultSprites 1
#define SPR_MiscSprites 2

// valore per colori di print string
#define FC_WHITE_PULSE 1  
#define FC_WHITE 2
#define FC_RED  3
#define FC_BLUE    4
#define FC_LIGHT_GRAY 5
#define FC_GOLD  6
#define FC_DARK_GRAY 7
#define FC_YELLOW 8

// index to access to GlobTomb4.VetStringOffsets[]
// Print OFFset
#define POFF_MainMenu 0
#define POFF_NewGameTitle 1
#define POFF_LoadGameTitle 2
#define POFF_NewGameList 3
#define POFF_LoadGameList 4
#define POFF_OptionSettings 5
#define POFF_OptionCmdList 6
#define POFF_PauseScreen 7
#define POFF_StatList 8
#define POFF_InventoryItemName 9
#define POFF_ExamineText 10

#define POFF_COUNTER 11



#define MAX_ALLOCATA 5000
#define MAX_PARAM_WTEXT 100
#define MAX_OLD_FLIPEFFECT 512
#define MAX_FMV 128
#define MAX_ACTIONS 100
#define MAX_OLD_ACTIONS 512
#define MAX_TEX_SEQUENCE 100
#define MAX_GLOBAL_TRIGGERS 500
#define MAX_RAIN_PARTICLES 0x1A00
#define MAX_SNOW_PARTICLES 0x1A00
#define MAX_ELEVATORS 20
#define MAX_ELEVATOR_DOORS 10
#define MAX_FRAME_ITEMS 200
#define MAX_MIRRORS 100
#define MAX_OFFSET_HARD_BREAK 500
#define MAX_EQUIP_ITEM 100
#define MAX_ENV_CONDITIONS 100
#define MAX_ENV_SCRIPT_CONDITIONS 256
#define MAX_SWAP_MESH 100
#define MAX_TRIGGER_GROUPS 1000
#define MAX_TRIGGERS_IN_THE_GROUP 64
#define MAX_OLD_CONDITIONS 512
#define MAX_COLLISIONI 20
#define MAX_ORGANIZE_COPPIE 100
#define MAX_ORGANIZERS 500
#define MAX_IMPORT_FILES 200
#define MAX_ITEM_GROUPS 100
#define MAX_COLORI_RGB 200
#define MAX_WATCH 32
#define MAX_ITEMS 1024
#define MAX_ADD_EFFECTS 100
#define MAX_MOVE_ADVANCE 100
#define MAX_SFX_MANCANTI 10
#define MAX_SUONATI 10
#define MAX_TEMPI_FPS 32
#define MAX_MOVE_PARAM 100
#define MAX_ROTATE_PARAM 100
#define MAX_COLORA_ITEM 100
#define MAX_TIMER_OGGETTI 200
#define MAX_ANIM_MORTE 20
#define MAX_DEAD_ANIM 100
#define MAX_PARAM_PRINT_TEXT 100
#define MAX_STRINGE_TR5 30
#define MAX_TEST_POSITION 100
#define MAX_ANIM_DIAGNOSTIC 20
#define MAX_SET_CAMERA 100
#define MAX_ALLOCA 200
#define MAX_IMAGES 200
#define MAX_STRINGHE_DIARIO 200
#define MAX_PAGINE_DIARIO 200
#define MAX_FONTS 100
#define MAX_DIARI 100
#define MAX_ROOMS 512
#define MAX_EVENTI_NOW 100
#define MAX_SWITCH 128
#define MAX_FREEZE 256
#define MAX_TEX_PER_FRAME 16
#define MAX_COLL_DISABLED 256
#define MAX_VEICOLI 50
#define MAX_TAIL_INFOS 1024
#define MAX_FLOODS 100
#define MAX_DART_CUST 50
#define MAX_STANDBY 50
#define MAX_ASSIGN_SLOT 400
#define MAX_ROLL_BOATS 80
#define MAX_ROLL_BOAT_SCRIPT 40
#define MAX_SCALE_ITEM 100
#define MAX_SHOW_SPRITES 100
#define MAX_CUST_SFX 100
#define MAX_TRIANGLES 256
#define MAX_QUADS 256
#define MAX_CIRCLES 256
#define MAX_TARGET_LEVELS 100
#define MAX_SHOW_MESH 200
#define MAX_SLOT_COLLIDE 400
#define MAX_ANIM_SCRIPT 256
#define MAX_ANIM_OBJ_SCRIPT 512
#define MAX_REMAPPED_ZONES 200
#define MAX_PARAM_LIGHTNING 100
#define MAX_NOMI_SLOT 600
#define MAX_ENEMY_SCRIPT 100
#define MAX_DGX_OLD_ERRORS 1000
#define MAX_DGX_NEW_ERRORS 10
#define MAX_IMAGE_RECORDS 30
#define MAX_MY_RECT 300
#define MAX_INPUT_BOX 100 
#define MAX_EXTRA_SCAN_CODES 64
#define MAX_SLOT_FLAGS 500
#define MAX_TIMING 12
#define MAX_DEMO_FRAMES 54000
#define MAX_DEMO_IDS 200
#define MAX_ACTORS 100
#define MAX_BASIC_COLLISIONS 100
#define MAX_SWAP_ANIM 100
#define MAX_MEM_SWAP_ANIM 100
#define MAX_SPEECH_PARAM 100
#define MAX_SPEECH_COMMANDS 100
#define MAX_ACTOR_SPEECHING 3
#define MAX_FLIP_MESH 100
#define MAX_NEW_ITEMS 256
#define MAX_PLUGINS_NGLE 256
#define MAX_EXTRA_EFFECT 20
#define MAX_STATIC_MIP 160
#define MAX_SALVA_CORD 1024
#define BREAK_CONDTION_TRIGGER 0
#define BREAK_GLOBAL_TRIGGER 1

// tipo di effetto per stand-by
#define TSB_NO_CHANGE_CAMERA 0
#define TSB_MATRIX 1
#define TSB_PORTRAIT 2
#define TSB_PANORAMA 3

// flag per cutscene camera
#define FTC_NONE 0
#define FTC_LEADING_LOOK_LARA 0x0001
#define FTC_EXTRA_LOOK_LARA 0x0002
#define FTC_LARA_LOOK_LEADING 0x0004
#define FTC_LARA_LOOK_EXTRA 0x0008

#define FPR_NO_RESUME_CRASH 2

// flags per standby

#define FSB_DISABLE_ON_CRAMPED_SPACE 0x0001
#define FSB_DISABLE_ON_COMBAT 0x0002
#define FSB_EXIT_ON_ATTACK 0x0004
#define FSB_FLIP_DISTANCE 0x0008
#define FSB_FLIP_V_ANGLE 0x0010
#define FSB_FLIP_H_ORIENT 0x0020
#define FSB_FLIP_SPEED 0x0040
#define FSB_FREEZE_ENEMIES 0x0080
#define FSB_FREEZE_LARA 0x0100
#define FSB_OVERLAP_AUDIO 0x0200
#define FSB_IMMEDIATE 0x0400

// flag per organizer
#define FO_ENABLED 0x0001 
#define FO_LOOP 0x0002 
#define FO_TICK_TIME 0x0004
#define FO_DEMO_ORGANIZER 0x0008

// flag per tipo di diagnostica
#define DGX_LARA		0x0001
#define DGX_ANIMATION   0x0002
#define DGX_SFX_SOUNDS  0x0004
#define DGX_FAR_VIEW    0x0008
#define DGX_AUDIO_TRACKS 0x0010
#define DGX_CHEATS       0x0020
#define DGX_FOG          0x0040
#define DGX_COMMON_VARIABLES 0x0080
#define DGX_STORE_VARIABLES  0x0100
#define DGX_FLYBY  0x0200
#define DGX_TEXT_VARIABLES 0x0400
#define DGX_LOG_SCRIPT_COMMANDS 0x0800
#define DGX_WEAPON_ANIMATION 0x1000
#define DGX_ADJUSTMENT_MODE 0x2000
#define DGX_FPS 0x4000
#define DGX_ERRORS 0x8000


// flag per extra field di DiagnosticType command

#define EDGX_CONCISE_SCRIPT_LOG 0x0001
#define EDGX_SLOW_MOTION 0x0002
#define EDGX_SWAP_VIEW 0x0004
#define EDGX_LARA_CORD_IN_LOG 0x0008
#define EDGX_TRIGGER_TIMING 0x0010
#define EDGX_RECORDING_DEMO 0x0020
#define EDGX_CUTSCENE_LOG 0x0040
#define EDGX_ANIMATION_SLOT 0x0080

// flag for recording/playing demog
#define RECF_STOP 0
#define RECF_RECORDING 1
#define RECF_PLAYING 2

// ultima versione disponibile di demo:
#define DEMO_VER_NOW 100   //  1.00

// flag per demo
#define DEMF_PLAY_LEVEL_SEQUENCE 0x0001
#define DEMF_PLAY_ALL_SEQUENCE 0x0002
#define DEMF_QUIT_WITH_ESCAPE 0x0004
#define DEMF_CROSS_FADE 0x0008
#define DEMF_RANDOM 0x0010
#define DEMF_PLAY_ON_KEY 0x0020
#define DEMF_PERFORM_AT_START 0x0040
#define DEMF_CINEMA_SCREEN 0x0100
#define DEMF_MUTE_TRACK 0x200
#define DEMF_MUTE_SFX  0x400

// flags per importfile type
#define FTYPE_USERFILE 1
#define FTYPE_SOUND 2
#define FTYPE_SETTINGS 3


// flag per .mem file of crash report
#define MEM_CRASH_ON_HEADER 0x0001
#define MEM_CRASH_ON_ROOMS 0x0002
#define MEM_CRASH_ON_ITEMS 0x0004
#define MEM_CRASH_ON_ANIMATIONS 0x0008
#define MEM_CRASH_ON_STATICS 0x0010
#define MEM_EXTRA_FILE_HEADERS  0x0020 // after mem crash data there is one or more extra files
#define MEM_EXTRA_PLUGIN_DB 0x0040  // uno degli extra file sara' db_patches.bin
#define MEM_EXTRA_PLUGIN_WARM_UP 0x0080 // uno degli extra file sara'  pluginname_warm_up_log.txt

// tipo file extra salvato dopo mem crash report
#define EBP_NONE          0   // nessun extra file
#define EBT_PLUGING_DB    1   // file "db_patches.bin"
#define EBT_WARM_UP_LOG   2   // file di testo "PlUGINNAME_warm_up_log.txt"

#define OCB_BRDG_NO_SLIDING 0x080
#define OCB_BRDG_ENABLE_HANG 0x040

// costanti per tipo di estensione
#define CTE_UNKNOWN 0
#define CTE_TOM  1
#define CTE_PRJ  2
#define CTE_SAVEGAME 3
#define CTE_TR4 4
#define CTE_WAD 5
#define CTE_DAT 6

#define ts_Avanti		1
#define ts_Indietro		2

// flag per record MoveItem di parameterlist
#define FMOV_INFINITE_LOOP 0x0001
#define FMOV_HEAVY_AT_END 0x0002
#define FMOV_TRIGGERS_ALL 0x004
#define FMOV_HEAVY_ALL 0x0008
#define FMOV_USE_LEADING_ACTOR_INDEX 0x0010
#define FMOV_USE_EXTRA_ACTOR_INDEX 0x0020
#define FMOV_IGNORE_FLOOR_COLLISION 0x0040
#define FMOV_WAIT_STAND_ON_FLOOR 0x0080

#define FMOV_FROG_JUMP_GRAVITY 0x0100
#define FMOV_LEAF_GRAVITY 0x0200
#define FMOV_EXPLOSION_GRAVITY 0x0300
#define FMOV_APPLE_GRAVITY 0x0400
#define FMOV_APOLLO_GRAVITY 0x0500
#define FMOV_MAN_GRAVITY 0x0600
#define FMOV_ANVIL_GRAVITY 0x0700


#define FMOV_EXPLOSION_SPEED 0x1000
#define FMOV_MAGNET_SPEED 0x2000
#define FMOV_CAR_SPEED 0x3000

#define FMOV_MASK_GRAVITY 0x0F00
#define FMOV_MASK_SPEED 0xF000

#define SIZE_RAIN_BUFFER MAX_RAIN_PARTICLES*0x14
#define SIZE_SNOW_BUFFER MAX_SNOW_PARTICLES*0x14

// flags per script image
#define IF_TRANSPARENCE 0x01
#define IF_POP_IMAGE    0x02
#define IF_FULL_SCREEN  0x04
#define IF_OVER_FLYBY   0x08
#define IF_QUIT_ESCAPE  0x10
#define IF_EFFECT_ZOOM 0x20
#define IF_EFFECT_FROM_TOP 0x40
#define IF_EFFECT_FROM_BOTTOM 0x80
#define IF_EFFECT_FROM_LEFT 0x100
#define IF_EFFECT_FROM_RIGHT 0x200
#define IF_PLAY_AUDIO_TRACK 0x400
#define IF_PRELOAD    0x800
#define IF_OVER_FIXED_CAMERA 0x1000
#define IF_LOOP_AUDIO_TRACK 0x2000
#define IF_CRYPTIC 0x4000
#define IF_EFFECT_CROSS_FADE 0x8000



#define QSF_TRUE_COLOR 0x100

// flag ocb per pushable
#define OPU_MASCHERA_ALTEZZA 0x001F
#define OPU_SPINGI_NEL_VUOTO 0x0020
#define OPU_ENABLE_NG		0x0040
#define OPU_NO_PULL         0x0080 
#define OPU_NO_PUSH         0x0100 
#define OPU_NO_EAST_WEST    0x0200 
#define OPU_NO_SOUTH_NORTH  0x0400
#define OPU_CLIMB_WEST		0x0800 
#define OPU_CLIMB_NORTH		0x1000 
#define OPU_CLIMB_EAST		0x2000
#define OPU_CLIMB_SOUTH		0x4000

// valori per tipo import file
#define IMPORT_MEMORY 1
#define IMPORT_TEMPORARY 2

// flag per VetCollisionePushable[]
#define CP_NULL		 0x00
#define CP_COLLISION 0x01
#define CP_RAISE_PAD 0x02
#define CP_FALLING   0x04
#define CP_MOVING_ACTION 0x08

// costanti param
#define PARAM_MOVE_ADV_ITEM  1
#define PARAM_MOVE_ITEM 2
#define PARAM_ROTATE_ITEM 3
#define PARAM_COLOR_ITEM 4
#define PARAM_PRINT_TEXT 5
#define PARAM_SET_CAMERA 6
#define PARAM_BIG_NUMBERS 7
#define PARAM_SCALE_ITEM 8
#define PARAM_SHOW_SPRITE 9
#define PARAM_TRIANGLE 10
#define PARAM_QUADRILATERAL 11
#define PARAM_CIRCLE 12
#define PARAM_LIGHTNING 13
#define PARAM_WTEXT 14
#define PARAM_RECT 15
#define PARAM_SWAP_ANIMATIONS 16
#define PARAM_ACTOR_SPEECH 17
#define PARAM_INPUT_BOX 18
#define PARAM_LAST 19  // ricordarsi di aggiornare questo quando aggiungo valori

// speech commands

#define SPC_MESH 0x0000  // + numero mesh  
#define SPC_SYLL 0x1000  
#define SPC_TEXT 0x2000
#define SPC_SEQUENCE 0x3000
#define SPC_PAUSE 0x4000
#define SPC_LOOK_RIGHT 0x5000
#define SPC_LOOK_LEFT 0x6000
#define SPC_LOOK_UP 0x7000
#define SPC_LOOK_DOWN 0x8000
#define SPC_HEAD_SHAKE 0x9000
#define SPC_HEAD_NOD 0xA000
#define SPC_PLAY_SFX 0xB000
#define SPC_ANIMATION 0xC000
#define SPC_PLAY_CD 0xD000
#define SPC_PERFORM_TG 0xE000
#define SPC_NEXT_STATEID 0xF000

#define SPC_MASK_COMMANDS 0xF000
#define SPC_MASK_FRAMES   0x0FC0
#define SPC_MASK_MESH     0x003F
#define SPC_MASK_SINGLE   0x0FFF
#define SPC_MASK_PAUSE    0x07FF

// flag for param_speech_actor
#define SPCF_OLD_SPEECH_SLOTS 0x0001
#define SPCF_LOOP 0x0002
#define SPCF_FREEZE_HAIR 0x0004
#define DEMO_FRAME 0x0800




// flags for PARAM_LIGHTNING command
#define OTYPE_MOVEABLE 0x0000  
#define OTYPE_STATIC 0x2000
#define OTYPE_AI_DATA 0x4000

// flag per PARAM_SHOW_SPRITE

#define FSS_SHOW_SPRITE_GRID 0x0001
#define FSS_TRANSPARENT 0x0002
#define FSS_CLONE_SPRITE 0x0004
#define FSS_EFFECT_ZOOM 0x0008
#define FSS_ANIMATE  0x0010
#define FSS_ANIMATE_FOR_BACK 0x0020
#define FSS_EFFECT_FROM_BOTTOM 0x0040
#define FSS_EFFECT_FROM_TOP 0x0080
#define FSS_EFFECT_FROM_LEFT 0x0100
#define FSS_EFFECT_FROM_RIGHT 0x0200

// flag qsf per Customize=CUST_INNER_SCREENSHOT

// tipo variazione colore per comando Parameters=PARAM_COLOR_ITEM
#define COLTYPE_SET_COLOR 1
#define COLTYPE_SET_PULSE 2
#define COLTYPE_SHADE_COLORS 3

// costanti DIR_ per direzione
#define DIR_MASK_DIRECTION 0x00FF
#define DIR_NORTH 0  //;Absolute moving to north. 
#define DIR_EAST  1  //;Absolute moving to east. 
#define DIR_SOUTH 2  // ;Absolute moving to south.
#define DIR_WEST  3  // ;absolute moving to west

#define DIR_UP 0x10  //;Absolute moving up
#define DIR_DOWN 0x11  //;absolute moving down

#define DIR_FORWARD 0x12 // relative: moving in direction of facing of the object
#define DIR_TURNING_LEFT_90 0x13 // turning to own left for 90
#define DIR_TURNING_LEFT_45 0x14
#define DIR_LU_TURNING_180 0x15
#define DIR_TURNING_RIGHT_45 0x16
#define DIR_TURNING_RIGHT_90 0x17
#define DIR_RU_TURNING_180 0x00018

#define DIR_LARA_FACING 0x0019
#define DIR_LEADING_ACTOR_FACING 0x001A
#define DIR_DIRECTION_LARA_LEADING_ACTOR 0x001B
#define DIR_HEAD_FOR_LARA 0x001C
#define DIR_HEAD_FOR_LEADING_ACTOR 0x001D
#define DIR_HEAD_FOR_EXTRA_ACTOR 0x001E



#define DIR_INVERT_DIRECTION 0x0100

// flag interno per gestione azione progressiva
#define MOVE_ITEM_OLD_MANAGEMENT 1
#define MOVE_ITEM_NEW_MANAGEMENT 2






// flags for Parameters=PARAM_LIGHTNING
// type of object target/source
#define OTYPE_STATIC 0x2000
#define OTYPE_AI_DATA 0x4000

// flags 
#define LGTN_PLAY_SOUND      0x0001
#define LGTN_FLASH_SCREEN    0x0002
#define LGTN_ADD_GLOVE_LIGHT 0x0004
#define LGTN_EXPLODE_TARGET  0x0008
#define LGTN_FIRE_LARA       0x0010
#define LGTN_LARA_SCREAM     0x0020
#define LGTN_KILL_TARGET     0x0040
#define LGTN_GLOBAL_SOUND    0x0080
#define LGTN_RANDOM_COLOR    0x1000
#define LGTN_INCLINED_RANDOM 0x2000
#define LGTN_HEARTHQUAKE     0x4000

// flag usati per rotazione oggetti in Parameters=PARAM_ROTATE_ITEM

#define ROTH_NONE -1  //Disable the horizontal rotation
#define ROTH_CLOCKWISE 1  // Rotate the item in clockwisemode
#define ROTH_INV_CLOCKWISE 2 // Rotate the item in inverse clockwise mode

#define ROTV_NONE -1   //Disable the vertical rotation
#define ROTV_FORWARD 1  //Rotate the item in vertical forward mode
#define ROTV_BACKWARD 2  //Rotate the item in vertical backward mode

#define FROT_LOOP 0x0001  //Perform an infinite rotation

// costanti MVF_ .. MoVing flags
#define MVF_SPEED_DECREASING 0x0001  //;Start moveable at (max) speed set and decrease the speed until to end over final sector

#define MVF_SPEED_INCREASING 0x0002  // ;Start moveable slowly and increase the speed until to reach the set (max) speed

#define MVF_ENABLE_HEAVY_TRIGGER 0x0004 //;The moveable will enable the further heavy trigger placed on final sector. This work like rollingball or pushable objects, with heavy trigger enabled only in final sector.

#define MVF_ENABLE_COMMON_TRIGGERS 0x0008  //;The moveable will enable all common triggers placed on its path. This works like mechanical scarab to activate traps.

#define MVF_COLLISION_STOP 0x0010 // ;Stop movement if moveable meet obstacles, like other moveable or static

#define MVF_COLLISION_KILL 0x0020  //;Kill moveable or destroy statics on its path

#define MVF_GRAVITY 0x0040  // ;Simulate gravity. If moveable is over the floor move it down like if it was falling.



// flags StatusNG  (SNG_...) per salvare test di qualche tipo in savegame
#define SNG_NONE 0
#define SNG_INFINITE_AIR      0x00000001  // anche patch trlm
#define SNG_DISABLE_WEAPONS   0x00000002
#define SNG_HIDE_HOLSTERS     0x00000004
#define SNG_IMMORTAL_LARA	  0x00000008  // usata solo come patch trlm 2009
#define SNG_OPEN_ALL_DOORS    0x00000010 // usata per patch trlm 2009
#define SNG_KILL_ALL_ENEMIES  0x00000020 // usata per trlm 2009
#define SNG_REMOVE_IMMORTAL_LARA 0x00000040 // usata da trlm 2009
#define SNG_PATCH_LARA_STAR 0x0000080 // usata trlm 2009
#define SNG_UPDATE_LARA_POS 0x0000100 // usata trlm 2009 dopo un move lara
#define SNG_SUPER_BINOCULARS 0x0000200 // usata per binocolo potenziato
// flag FAN_ ossia flag per comando Animation=
#define FAN_SET_FREE_HANDS         0x0001
#define FAN_START_FROM_EXTRA_FRAME 0x0002
#define FAN_SET_NEUTRAL_STATE_ID   0x0004
#define FAN_KEYS_AS_SCANCODE       0x0008
#define FAN_DISABLE_PUSH_AWAY	   0x0010   
#define FAN_KEEP_NEXT_STATEID      0x0020
#define FAN_ENABLE_GRAVITY		   0x0040
#define FAN_DISABLE_GRAVITY        0x0080
#define FAN_PERFORM_TRIGGER_GROUP  0x0100
#define FAN_SET_FREE_HANDS_TEMP    0x0200
#define FAN_SET_BUSY_HANDS         0x0400
#define FAN_ALIGN_TO_ENV_POS       0x0800
#define FAN_SET_ADDEFFECT          0x1000
#define FAN_RANDOM				   0x2000
#define FAN_SET_LARA_PLACE		   0x4000


// usati per flag di testposition script command
#define TPOS_DOUBLE_HORIENT 0x0001
#define TPOS_FOUR_HORIENT 0x0002
#define TPOS_TEST_ITEM_INDEX 0x0004
#define TPOS_FAST_ALIGNMENT 0x0008
#define TPOS_OPPOSITE_FACING 0x0010
#define TPOS_ROUND_HORIENT 0x0020
#define TPOS_TURN_FACING_90 0x0040
#define TPOS_TURN_FACING_180 0x0080
#define TPOS_SELF_FIXING     0x0100
// valori costanti per campo Environment di comando animation
#define ENV_NO_BLOCK_IN_FRONT 1
#define ENV_HANG_WITH_FEET 2
#define ENV_HOLE_FLOOR_IN_FRONT 3
#define ENV_MONKEY_CEILING 4
#define ENV_CLIMB_WALL_IN_FRONT 5
#define ENV_CLIMB_WALL_AT_RIGHT 6
#define ENV_CLIMB_WALL_AT_LEFT 7
#define ENV_HOLE_IN_FRONT_CEILING_CLIMB 8
#define ENV_HOLE_BACK_CEILING_CLIMB 9
#define ENV_NO_BLOCK_AT_RIGHT 10
#define ENV_NO_BLOCK_AT_LEFT 11
#define ENV_HOLE_FLOOR_AT_RIGHT 12
#define ENV_HOLE_FLOOR_AT_LEFT 13
#define ENV_HOLE_FLOOR_BACK 14
#define ENV_NO_BLOCK_BACK 15
#define ENV_CLIMB_WALL_BACK 16
#define ENV_SUPPORT_IN_FRONT_WALL 17
#define ENV_SUPPORT_IN_RIGHT_WALL 18
#define ENV_SUPPORT_IN_LEFT_WALL 19
#define ENV_SUPPORT_IN_BACK_WALL 20
#define ENV_ITEM_EXTRA_IN_FRONT 21
#define ENV_ITEM_EXTRA_OVER 22
#define ENV_ITEM_EXTRA_UNDER 23
#define ENV_MULT_CONDITION 24
#define ENV_HANG_LEFT_IN_CORNER 25
#define ENV_HANG_LEFT_OUT_CORNER 26
#define ENV_HANG_RIGHT_IN_CORNER 27
#define ENV_HANG_RIGHT_OUT_CORNER 28
#define ENV_HANG_LEFT_SPACE 29
#define ENV_HANG_RIGHT_SPACE 30
#define ENV_DISTANCE_CEILING 31

#define ENV_CLIMB_LEFT_IN_CORNER 32
#define ENV_CLIMB_LEFT_OUT_CORNER 33
#define ENV_CLIMB_RIGHT_IN_CORNER 34
#define ENV_CLIMB_RIGHT_OUT_CORNER 35

#define ENV_CLIMB_LEFT_SPACE 36
#define ENV_CLIMB_RIGHT_SPACE 37
#define ENV_MULT_OR_CONDITION 38
#define ENV_DISTANCE_FLOOR 39
#define ENV_FRAME_NUMBER 40
#define ENV_VERTICAL_ORIENT 41

#define ENV_ON_VEHICLE  42
#define ENV_FREE_HANDS  43
#define ENV_UNDERWATER  44
#define ENV_FLOATING    45
#define ENV_ONLAND      46
#define ENV_IS_STILL    47
#define ENV_ANIM_COMPLETE 48
#define ENV_FLYING_UP  49
#define ENV_FLYING_DOWN  50
#define ENV_WALL_HOLE_IN_FRONT 51
#define ENV_IN_LEFT_SIDE_SECTOR 52
#define ENV_IN_RIGHT_SIDE_SECTOR 53
#define ENV_ITEM_EXTRA_AT_LEFT 54
#define ENV_ITEM_EXTRA_AT_RIGHT 55
#define ENV_ITEM_TEST_POSITION 56
#define ENV_HOLD_EXTRA_ITEM_IN_HANDS 57
#define ENV_CONDITION_TRIGGER_GROUP 58
#define ENV_ROOM_IS 59
#define ENV_PLAYER_IS_SLEEPING 60
#define ENV_PLAYER_WOKE_UP 61
#define ENV_DISTANCE_NORTH_WALL 62
#define ENV_DISTANCE_SOUTH_WALL 63
#define ENV_DISTANCE_EAST_WALL 64
#define ENV_DISTANCE_WEST_WALL 65
#define ENV_LARA_IN_MICRO_STRIP 66
	// usati da nemici:
#define ENV_NO_BOX_IN_FRONT 67
#define ENV_NO_BOX_AT_LEFT 68
#define ENV_NO_BOX_AT_RIGHT 69
#define ENV_NO_BOX_BACK 70

#define ENV_ENEMY_SEE_LARA 71
#define ENV_FRAME_RANGE 72

// quelli di byte alto considerarli numberi e non flag, usare maschera, eccetto ENV_NON_TRUE che puo' essere usato 
// con altri
#define ENV_NON_TRUE          0x0080
#define ENV_POS_LEFT_CORNER   0x0100
#define ENV_POS_RIGHT_CORNER  0x0200
#define ENV_POS_CENTRAL       0x0400
#define ENV_POS_HORTOGONAL    0x0800
#define ENV_POS_IN_THE_MIDDLE 0x1000
#define ENV_POS_STRIP_3       0x2000
#define ENV_POS_STRIP_2       0x4000
#define ENV_POS_STRIP_1       0x8000
	// maschere per ottenere flag o condizioni
#define ENV_MASK_CONDITION  0x7f
#define ENV_MASK_FLAGS 0xFF80

// flag da aggiungere a distanza env
#define DENV_FLAG_FACE2FACE 0x8000
#define DENV_FLAG_FACE2BACK 0x4000
#define DENV_MASK_DISTANCE 0x3fff
#define DENV_MASK_FLAGS 0xC000

#define ROOM_WATER -1
#define ROOM_QUICKSAND -2
#define ROOM_DAMAGE -3
#define ROOM_COLD -4
#define ROOM_RAIN -5
#define ROOM_SNOW -6
#define ROOM_OUTSIDE -7
#define ROOM_REFLEX -8
#define ROOM_MIST -9

// value for SlopeType of FLOOR structure to test with == operator
#define SLOPE_FLAT  0  // no slope, all corners have same height
#define SLOPE_GENTLE_SLOPE  1  // slope (1 or 2 clicks) on a single side where lara is able to walk
#define SLOPE_STEEP_SLOPE 2  // slope (3 clicks or higher) on a single side where lara is not able to walk
#define SLOPE_GENTLE_CORNER 3  // triangle slope (1 or 2 clicks) on a single corner where lara is able to walk
#define SLOPE_STEEP_CORNER 4 // triangle slope (3 clicks or higher) on a single corner where lara is not able to walk

// flag for climb status, read directly from floordata type
#define CLIMB_EAST_WALL		0x0100
#define CLIMB_SOUTH_WALL    0x0200
#define CLIMB_WEST_WALL		0x0400
#define CLIMB_NORTH_WALL	0x0800

// flag NEF ossia flag ng per comando script Enemy=
#define NEF_NONE 0
#define NEF_EXPLODE			0x0001
#define NEF_EXPLODE_AFTER	0x0002
#define NEF_NON_TARGET		0x0004
#define NEF_HIT_DEFAULT		0x0008
#define NEF_SET_AS_CREATURE 0x0010
#define NEF_SET_AS_BRIDGE_FLAT  0x0020
#define NEF_SET_AS_BRIDGE_TILT1 0x0040
#define NEF_SET_AS_BRIDGE_TILT2 0x0080
#define NEF_EASY_HEAVY_ENABLING 0x0100

#define NEF_HIT_BLOOD		0x0400
#define NEF_HIT_SMOKE		0x0800
#define NEF_HIT_FRAGMENTS	0x0C00
#define NEF_ONLY_EXPLODE	0x1000
#define NEF_SET_AS_MORTAL	0x2000
#define NEF_SAVE_MESH_VISIBILITY 0x4000
#define NEF_SET_AS_SEMIGOD 0x8000

// returned value for reply of cbGlobalTrigger call back
#define RGT_TRUE         0x01   // condition is true
#define RGT_FALSE		 0x00   // condition is false
#define RGT_IGNORE       0x80   // the callback doesn't handle this kind of GT_ value

// flag per comando script Damage=
#define DMG_INDIRECT_BAR  0x01
#define DMG_ONLY_PAD   0x02
#define DMG_INCREASE_BAR 0x04
#define DMG_COLD_WATER 0x08
#define DMG_POISON_LARA 0x10
#define DMG_BURNING_DEATH 0x20
#define DMG_SLOW_DISAPPEARING 0x40
#define DMG_BURNING_SCREAM 0x80
#define DMG_ALERT_BEEP 0x100
#define DMG_LITTLE_TEXT 0x200

// flag usati in gioco per monitorare l'esecuzione di damage
#define FPD_NONE 0x00
#define FPD_ATTIVO 0x01
#define FPD_BLINK 0x02
#define FPD_RICRESCITA 0x04
#define FPD_BEEP 0x08
#define FPD_DIMINUIRE_VITA 0x10
#define FPD_INCENDIATA_LARA 0x20
#define FDP_BLINK_SHOW_ORA 0x40
#define FDP_MOSTRATA_STRINGA 0x80

#define INPUT_BOX_MODE 1000 // to pass to Seconds parameters to signal inputbox inside overlapped image
// costanti per azioni
#define AZ_NONE  0 // record vuoto
#define AZ_TURN_OBJECT  1  // ruota oggetto orizzontalmente fino ad un certo orientamento
#define AZ_TURN_VERT_OBJECT 2 // ruota verticalmente oggetto
#define AZ_BLOCK_DISABLE_KEY 3 // disabilita il blocco a tasti dopo tempo
#define AZ_BLOCK_SENDING_KEY 4 // disabilita invio di comando
#define AZ_HIDE_SCREEN 5  // disabilita schermo per tot secondi
#define AZ_FORCE_VOLUMETRIC_FX 6 // NON PIU' USATA
#define AZ_PRINT_STRING 7
#define AZ_SOUND_SAMPLE 8
#define AZ_TURN_FACING 9 // fino a raggiungere facing richiesto
#define AZ_TURN_VERT_FACING 10
#define AZ_DELAY_TRIGGER 11  // attiva con ritardo qualcosa
#define AZ_ADD_EFFECT 12 // aggiunge effetto a moveable
#define AZ_MOVE_ANIMATING 13 // muove animating
#define AZ_BLINK_MIRROR 14
#define AZ_LARA_TRANSPARENT 15
#define AZ_LARA_INVULNERABLE 16
#define AZ_PREPARA_FILMATO 17
#define AZ_TEXTURE_SEQUENCE 18
#define AZ_MOVE_ROLLINGBALL 19
#define AZ_MIST_ROLLINGBALL 20
#define AZ_TRAP_PUSHABLE 21
#define AZ_PUSH_PUSHABLE 22
#define AZ_CAMERA_TARGET 23
#define AZ_CAMERA_AXIS 24
#define AZ_CAMERA_EFFECT 25
#define AZ_CHANGE_LAYER_COLOR 26
#define AZ_MOVE_ADVANCED 27
#define AZ_WAIT_KILL 28
#define AZ_MOVE_STATIC 29
#define AZ_MOVE_MOVEABLE 30
#define AZ_ROTATE_STATIC 31
#define AZ_ROTATE_MOVEABLE 32
#define AZ_CHANGE_COLOR_STATIC 33
#define AZ_CHANGE_COLOR_MOVEABLE 34
#define AZ_TRIGGER_GROUP_CONTINUO 35
#define AZ_SHOW_TIMER_ITEM 36
#define AZ_PLAY_CD_RESTORE 37
#define AZ_CHANGE_FOG_DISTANCE 38
#define AZ_PULSE_FOG_DISTANCE 39
#define AZ_SCROLLING_VTEXT 40
#define AZ_SCROLLING_HTEXT 41
#define AZ_TEMP_FREE_HANDS 42
#define AZ_SET_CAMERA 43
#define AZ_SHOW_FLARE_LIGHT 44
#define AZ_TRANSPARENCY_ENEMY 45
#define AZ_CHANGE_FOG_DENSITY 46
#define AZ_TIMER_TRNG 47
#define AZ_SHOW_TIMER_TRNG 48
#define AZ_RESUME_FROZEN 49
#define AZ_SHOW_CUSTOM_BAR 50
#define AZ_ALTERNATE_FLIPMAP 51
#define AZ_SEQUENZA_SHOT 52
#define AZ_FLOOD_WATER 53
#define AZ_FREEZE_ALL 54
#define AZ_H_CIRCULAR_ROTATE 55
#define AZ_SCALE_STATIC 56
#define AZ_ENABLE_FREEZE_TRASP 57
#define AZ_HARPY_LOADING_EFFECT 58
#define AZ_SHOW_SPRITE 59
#define AZ_PUSH_AWAY 60
#define AZ_LIGHTNING 61
#define AZ_PRINT_WINDOWS_STRING 62  // stampa automatica di testi standard in modalita' windows font
#define AZ_PRINT_WINDOWS_TEXT 63   // stampa testo con flipeffect e wparam data
#define AZ_CROSS_FADE_IMAGE 64
#define AZ_CURTAIN_FADE_EFFECT 65
#define AZ_CAMERA_INFINITE 66



// constants for cutscene camera commands
#define CCT_MOVE_UP 1
#define CCT_MOVE_DOWN 2
#define CCT_ZOOM_IN 3
#define CCT_ZOOM_OUT 4
#define CCT_ROTATE_LEFT 5
#define CCT_ROTATE_RIGHT 6
#define CCT_FREEZE 7


// tipo di effetto da visualizzare a meta e alla fine del cross fade
#define ET_CROSS_FADE_SHOW_IMAGE 1
// costanti per tipo di variazione fog fatto con azione AZ_CHANGE_FOG_DENSITY
#define TVF_FOG_END 1
#define TVF_FOG_DISTANCE 2

// valori ocb per static
#define OCBS_ATTIVO 0x0001
#define OCBS_SALVARE 0x002
#define OCBS_NO_COLLISIONI 0x0004
#define OCBS_TRASPARENZA_GLASS 0x0008
#define OCBS_TRASPARENZA_ICE 0x0010
#define OCBS_DAMAGE_LARA 0x0020
#define OCBS_BURN_LARA 0x0040
#define OCBS_EXPLODE 0x0080
#define OCBS_POISON 0x0100
#define OCBS_HUGE_COLLISION 0x0200
#define OCBS_HARD_SHATTER 0x0400
#define OCBS_HEAVY_TRIGGER 0x0800
#define OCBS_SCALABLE 0x1000
#define OCBS_SHATTER_SPECIFIC 0x2000

// ocb for frogman

#define OCBFM_NO_SHOOT_FROM_FAR 0x0001  // distance in sectors N * 256 ocb value
#define OCBFM_NO_SHOOT_OFF_WATER 0x002
#define OCBFM_NEVER_SHOOTING 0x0004
#define OCBFM_GO_DEEP_WATER 0x0008  // go in deep water when lara is on ground
#define OCBFM_KEEP_DISTANCE_FROM_LARA  0x0010 // avoid that frogman pushes lara or enter across her body

// flags per comando Switch
#define SWT_BASE_ZERO 0x0001
#define SWT_RANDOM_MODE 0x0002
// costanti per tipo di speed scrolling verticale
#define SPEED_NORMAL_30_FPS  0
#define SPEED_SLOW_15_FPS  1
#define SPEED_VERY_SLOW_10_FPS 2
#define SPEED_FAST_60_FPS 3
#define SPEED_VERY_FAST_90_FPS 4

// definizioni di trigger what
#define TO_OBJECT 0
#define TO_CAMERA 1
#define TO_SINK 2
#define TO_FLIPMAP 3
#define TO_FLIPON 4
#define TO_FLIPOFF 5
#define TO_TARGET 6
#define TO_FINISH 7
#define TO_CD 8
#define TO_FLIPEFFECT 9
#define TO_SECRET 10
#define TO_ACTION 11
#define TO_FLYBY 12
#define TO_PARAMETER 13
#define TO_FMV 14
#define TO_TIMER_FIELD 15

#define bp_Errore   MB_ICONASTERISK
#define bp_OK   MB_ICONEXCLAMATION



// questo e' il valore usato per cicli effettuati ogni secondi
#define FRAME_SECONDO 30


// costanti per ocb di rollingball
#define RB_SILEN_MODE         0x0001
#define RB_KILL_ENEMIES       0x0002
#define RB_PUSHING_ACTIVATION 0x0004
#define RB_PUSHING_MULTIPLE   0x0008
#define RB_DESTROY_SHATTERS   0x0010
#define RB_CHECK_WATER        0x0020
#define RB_ENABLE_TRIGGERS    0x0040

// valori global triggers
#define GT_USED_INVENTORY_ITEM 1  // proc separata
#define GT_SCREEN_TIMER_REACHED 2
#define GT_ENEMY_KILLED  3
#define GT_LARA_HP_LESS_THAN 4
#define GT_LARA_HP_HIGHER_THAN 5
#define GT_USED_BIG_MEDIPACK 6
#define GT_USED_LITTLE_MEDIPACK 7
#define GT_USING_BINOCULAR 8
#define GT_DAMAGE_BAR_LESS_THAN 9
#define GT_LARA_POISONED 10
#define GT_CONDITION_GROUP 11
#define GT_DISTANCE_FROM_ITEM 12
#define GT_COLLIDE_ITEM 13
#define GT_COLLIDE_SLOT 14
#define GT_COLLIDE_CREATURE 15
#define GT_LOADED_SAVEGAME 16
#define GT_SAVED_SAVEGAME 17
#define GT_COLLIDE_STATIC_SLOT 18
#define GT_DISTANCE_FROM_STATIC 19
#define GT_LARA_HOLDS_ITEM 20
#define GT_VSCROLL_COMPLETE 21
#define GT_VSCROLL_LAST_VISIBLE 22
#define GT_GAME_KEY1_COMMAND 23
#define GT_GAME_KEY2_COMMAND 24
#define GT_KEYBOARD_CODE 25
#define GT_FMV_COMPLETED 26
#define GT_TITLE_SCREEN 27
#define GT_ELEVATOR_STOPS_AT_FLOOR 28
#define GT_ELEVATOR_STARTS_FROM_FLOOR 29
#define GT_BEFORE_SAVING_VARIABLES 30
#define GT_AFTER_RELOADING_VARIABLES 31
#define GT_ALWAYS 32
#define GT_TRNG_G_TIMER_EQUALS 33
#define GT_TRNG_L_TIMER_EQUALS 34
#define GT_KEYPAD_SHOWED 35
#define GT_KEYPAD_REMOVED 36
#define GT_NO_ACTION_ON_ITEM 37
#define GT_COMPLETED_SCALING_ON_ITEM 38
#define GT_SELECTED_INVENTORY_ITEM 39
#define GT_CREATED_NEW_ITEM 40

#define GTD_IGNORE_HEIGHT 0x40000000

#define TSCR_MAIN_TITLES 0
#define TSCR_NEW_GAME 1
#define TSCR_LOAD_GAME 2
#define TSCR_OPTIONS 3

// flags per global triggers
#define FGT_SINGLE_SHOT 0x0001
#define FGT_NOT_TRUE 0x0002
#define FGT_PUSHING_COLLISION 0x0004
#define FGT_DISABLED 0x0008
#define FGT_REMOVE_INPUT 0x0010
#define FGT_SINGLE_SHOT_RESUMED 0x0020
#define FGT_REPLACE_MANAGEMENT 0x0040
#define FGT_HIDE_IN_DEBUG 0x2000 // per escluderlo da log
#define FGT_PAUSE_ONE_SHOT 0x4000  // interno

// ----- DATI CONVERSIONE TOMB3->TOMB4 ----------------------
// ---- record slot ------
#define SLOT_TR3_24  26h
#define SLOT_TR3_26  28h
#define SLOT_TR3_28  2Ah
#define SLOT_TR3_2A  2Ch
#define SLOT_TR3_2C  2Eh
#define SLOT_TR3_30  32h
#define SLOT_TR3_31	  33h

// --- record item ----
#define ITEM_TR3_82  15EAh

// --- record animazione -----
#define ANIM_TR3_04  05h
#define ANIM_TR3_10  18h
#define ANIM_TR3_12  1Ah
#define ANIM_TR3_14  1Ch
#define ANIM_TR3_16  1Eh
#define ANIM_TR3_18  20h
#define ANIM_TR3_1A  22h
#define ANIM_TR3_1C  24h
#define ANIM_TR3_1E  26h

// --- record particelle tr3 ------
#define SPARK_TR3_1B Non Esiste
#define SPARK_TR3_1C 1Bh
#define SPARK_TR3_1D Non esiste
#define SPARK_TR3_1E Non esiste
#define SPARK_TR3_1F 1Ch
#define SPARK_TR3_20 1Dh
#define SPARK_TR3_21 1Eh
#define SPARK_TR3_22 1Fh
#define SPARK_TR3_23 20h
#define SPARK_TR3_24 21h
#define SPARK_TR3_25 22h
#define SPARK_TR3_26 23h
#define SPARK_TR3_27 24h
#define SPARK_TR3_28 25h
#define SPARK_TR3_29 26h
#define SPARK_TR3_2A 27h
#define SPARK_TR3_2B 28h 
#define SPARK_TR3_2C 29h
#define SPARK_TR3_2D 2Ah
#define SPARK_TR3_2E 2Bh
#define SPARK_TR3_2F 2Ch
#define SPARK_TR3_30 2Dh
#define SPARK_TR3_31 2Eh
#define SPARK_TR3_32 2Fh
#define SPARK_TR3_33 Non esiste
#define SPARK_TR3_34 30h
#define SPARK_TR3_35 31h
#define SPARK_TR3_36 32h
#define SPARK_TR3_37 33h

// --- reecord AI (puntato da stritemtr4.3c)
#define AI_TR3_12  0x160e
#define AI_TR3_14  0x1610
#define AI_TR3_18  0x1614
#define AI_TR3_1C  0x1618
#define AI_TR3_20  0x12
#define AI_TR3_24  0x161C
#define AI_TR3_2C  0x1624
#define AI_TR3_30  0x1628
#define AI_TR3_32  0x162A
#define AI_TR3_34  0x1632
#define AI_TR3_36  0x162C
#define AI_TR3_38  0x162E
#define AI_TR3_3A  0x1630
#define AI_TR3_3C  0x1636
#define AI_TR3_40  0x163A
#define AI_TR3_44  0x163E

// Flag ANimated texture
#define FAN_MASK_FPS_UV    0x1F00
#define FAN_MASK_UVROTATE  0x00FF
#define FAN_MASK_DELAY     0x1FFF
#define FAN_MASK_ANIM      0xE000
#define FAN_FRAMES         0x0000
#define FAN_P_FRAMES       0x4000
#define FAN_FULL_ROTATE    0x8000
#define FAN_RIVER_ROTATE   0xA000 // 0x8000 + 0x2000
#define FAN_HALF_ROTATE    0xC000
#define FAN_ANY_ROTATE     0x8000


// ------ per Splash -----------
#define SPLASH_TR3_00  00h
#define SPLASH_TR3_04  04h
#define SPLASH_TR3_08  08h
#define SPLASH_TR3_0C  0Ch
#define SPLASH_TR3_0E  0Eh
#define SPLASH_TR3_12  10h
#define SPLASH_TR3_14  12h
#define SPLASH_TR3_1A  14h
#define SPLASH_TR3_1C  16h
#define SPLASH_TR3_20  18h
#define SPLASH_TR3_22  1Ah
#define SPLASH_TR3_28  1Ch
#define SPLASH_TR3_2A  1Eh
#define SPLASH_TR3_2C  20h

// ------------- FINE CONVERSIONE TOMB3->TOMB4 -------------------
// flag per livello globali tomb4
#define LF_NONE 0
#define LF_DISABLE_SAVE  0x0000001
#define LF_DISABLE_LOAD  0x0000002
// era 0x1300000
#define NEW_SIZE_MEM1_4 0x2600000
// prima era 0x2000000
#define NEW_SIZE_MEM_5 0x4000000
// prima era 0xe00000
#define NEW_SIZE_ALLOCA_TEX 0x1e00000

#define NEW_SIZE_MEM_LIVELLO 0x3938700  // 60.000.000
// costanti per tipo di memoria speciale che richiede
// un analisi per occupazione che salti alcuni campi di ogni struttura
#define SPEC_NONE -1
#define SPEC_SCENE_MEMORY 1
#define SPEC_PARTICELLE 2
#define SPEC_FIRE_SPARKS 3






// costanti usate per settare il tipo di cambio alt_enter
#define OAE_NORMALE 0
#define OAE_SHOW_FILM 1
#define OAE_REFRESH_FULL_SCREEN 2


// comando key1
#define KEY1_RELEASED 0x8000
// comandi tastiera da bloccare o inviare
#define CMD_ALL			0xffffffff    // 0 All keyboard commands
#define CMD_UP			0x00000001			//  1: Up
#define CMD_DOWN		0x00000002			// 2: Down
#define CMD_LEFT		0x00000004			// 3: Left
#define CMD_RIGHT		0x00000008		// 4: Right
#define CMD_DUCK		0x20000000		// 5: Duck
#define CMD_DASH		0x40000000		// 6: Dash
#define CMD_WALK		0x00000080		// 7: Walk
#define CMD_JUMP		0x00000010		// 8: Jump
#define CMD_ACTION		0x00000040		// 9: Action
#define CMD_DRAW_WEAPON 0x00000020  // 10: Draw Weapon
#define CMD_USE_FLARE	0x00080000   // 11: Use Flare
#define CMD_LOOK		0x00000200	// 12: Look
#define CMD_ROLL		0x00001000	//  13: Roll
#define CMD_INVENTORY	0x00200100  //  14: Inventory
#define CMD_STEP_LEFT	0x00000480  // 15: Step Left
#define CMD_STEP_RIGHT	0x00000880 // 16: Step Right
#define CMD_PAUSE		0x00002000    //  17: Pause
#define CMD_SAVE_GAME	0x00400000    // 18: Save the game (special)
#define CMD_LOAD_GAME   0x00800000    // 19: Load the game (special)
#define CMD_WEAPON_KEYS 0x10000000		//  20: Select weapon keys
#define CMD_ENTER       0x00100000     // 21: Enter 
// costanti per xcodici memroia
#define MEM_MASK_INDEX     0x00FF
#define MEM_TIPO_SAVEGAME  0x0100
#define MEM_TIPO_CODICE    0x0200
#define MEM_TIPO_ITEM      0x0400
#define MEM_TIPO_SLOT      0x0800
#define MEM_TIPO_ANIMATION 0x1000
#define MEM_TIPO_INVENTORY 0x2000

#define MEM_MASK_ALL_TYPES 0x3F00


// duplicated with english names and enum support:
// MEMT_ (Memory Type) to add with "|" to triggercode for memory trng variables
#define MEMT_SAVEGAME  0x0100
#define MEMT_CODE      0x0200
#define MEMT_ITEM      0x0400
#define MEMT_SLOT      0x0800
#define MEMT_ANIMATION 0x1000
#define MEMT_INVENTORY 0x2000



// costanti per accedere a variabili (BaseVariabili)
#define VAR_MASK_TYPE  0x0070  // -111 ----
#define VAR_MASK_SIZE  0x0030  // --11 ----
#define VAR_MASK_INDEX 0x000F  // ---- 1111
#define VAR_TYPE_TEXT  0x0020  // --10 ----
#define VAR_TYPE_LOCAL 0x0040  // 64
#define VAR_TYPE_BYTE  0x0000  //  0
#define VAR_TYPE_SHORT 0x0010  // 16
#define VAR_TYPE_LONG  0x0030  // 48

#define VAR_TYPE_STORE          0x0100  // e' un tipo di variabile store
#define VAR_TYPE_INPUT_NUMBER   0x0200  // variabile numerica LastInputNumber
#define VAR_TYPE_INPUT_TEXT     0x0400 // ultimo testo inserito da utente
#define VAR_TYPE_CURRENT_VALUE  0x0800 // variabile speciale current value
#define VAR_TYPE_BIG_TEXT       0x1000 // variable speciale big text

// used only to add to 0/15 values for specific long type store variables
#define VAR_TYPE_LONG_STORE		0x1c0
#define VAR_SPECIAL_INPUT_NUMBER 0x0035 // numero speciale per pStartFog poi da cambiare
#define VAR_SPECIAL_CURRENT_VALUE 0x00FF // solo per LST_ poi dovra' essere convertito
#define VAR_SPECIAL_INPUT_TEXT 0x0036 // numero speciale per LST_
#define VAR_SPECIAL_BIG_TEXT   0x0037 // numero per LST_

#define STORE_MASK_INDEX 0x003F // indice
#define STORE_MASK_SIZE  0x00C0  // tipo size byte, short, long 
#define STORE_TYPE_BYTE  0x0040
#define STORE_TYPE_SHORT 0x0080 
#define STORE_TYPE_LONG  0x00C0


// costanti per operazione su inventario di funzione GestionePickups
#define INV_LEGGI  0
#define INV_SCRIVI 1
#define INV_INCREMENTA 2
#define INV_DECREMENTA 3
#define INV_SCRIVI_NO_BIT 4
#define INV_LEGGI_NO_BIT 5

// fflag usati da aggiungere a indice di moveabel o static in calcolo
// collisioni tra veicolo e oggetto
#define FCV_MASK_INDEX 0x1FFF
#define FCV_COLLISIONE 0x4000 // oggetto che ha avuto collisione in calcolo punti
#define FCV_SLIM 0x2000  // oggetto piccolo


#define PANEL_START 472
#define PANEL_END 479









#define MAX_STACK_SCAN 5000



#define FPT_NONE 0
#define FPT_NO_RESUME_CRASH 1






#define NO_ARRAY 0x10000

// flag status anim range
#define SAR_TEST_STOP   0x1
#define SAR_UV_NEGATIVE 0x2

// costanti per tipo di testo in tomb4, per poter alterare i colori
#define TT_ITEM_NAME    0
#define TT_ACTION_INVENTORY_MENU_OFF   1 //combine examine
#define TT_ACTION_INVENTORY_MENU_ON    2 // lampegiante
#define TT_AMMO       3
#define TT_EXAMINE1_TOP 4
#define TT_EXAMINE1_BOTTOM 5
#define TT_EXAMINE3 6
#define TT_SELECT_LEVEL 7
#define TT_NEW_LEVEL_ARROWS 8
#define TT_LEVEL_NAME_OFF 9
#define TT_LEVEL_NAME_ON 10
#define TT_MAIN_MENU_OFF 11  // new game/ load game/option/exit
#define TT_LEGEND  12
#define TT_CREDITS 13
#define TT_SCREEN_TIMER 14
#define TT_SAVEGAME_PANEL_TITLE  15
#define TT_SAVEGAME_DESCRIPTION_OFF 16
#define TT_SAVEGAME_DESCRIPTION_ON 17
#define TT_OPTION_DESCRIPTIONS 20  //Text used to describe the option, like "Control Method"
#define TT_OPTION_VALUES 21        //Text of possible value for options, like "Joystick/Keboard"
#define TT_STATISTICS_DESCRIPTIONS 22  //Text used to describe the statistics, like "Distance Travelled"
#define TT_STATISTICS_VALUES 23  //Text of statistic values, like "43m", i.e. 43 meters for "Distance Travelled"
#define TT_PAUSED_MENU_TITLE 24  //Title of "Paused" menu. This text is the "Paused" word
#define TT_PAUSED_MENU_ITEMS 25    //Text for paused menu, "Statistics", "Options", "Exit to Title"
#define TT_MAIN_MENU_ON 26  // testi menu principale selezionati
#define TT_CAMERA_VIEW 27  // messaggio di LoadCamera quando si preme F1
#define TT_MAX_TEXT_TYPES 28  





#define SCRIPT_IGNORE 0xFFFF

// flag per comando LogItem
#define FLI_SHOW_DIFFERENCES 0x0001
#define FLI_SHOW_BOUND       0x0002
#define FLI_STATIC_ITEM      0x0004
#define FLI_DISTANCE_IN_SECTORS 0x0008

// flag per comando turbo
#define TRB_SELECTIVE_VIEW 0x0001
#define TRB_HIGH_PRIORITY 0x0002
#define TRB_ASYNC_FRAMES 0x0004
#define TRB_OPTIMIZE_SORTING 0x0008
#define TRB_ADAPTIVE_FARVIEW 0x0010

// valore per TipoVar usata in adaptive far view
#define TV_MIGLIORATO 1
#define TV_UGUALE 2
#define TV_PEGGIORATO 3

// tipo layout per pannello savegame
#define SPL_LEFT_IMAGE_RIGHT_INFO 1
#define SPL_LEFT_IMAGE_BOTTOM_INFO 2
#define SPL_LEFT_IMAGE_NO_INFO 3 
#define SPL_RIGHT_IMAGE_LEFT_INFO 4
#define SPL_RIGHT_IMAGE_BOTTOM_INFO 5
#define SPL_RIGHT_IMAGE_NO_INFO 6
#define SPL_CENTRAL_IMAGE 7

// flags per panello savegame
#define SPF_NO_TIME_IN_LIST 0x0001
#define SPF_SCROLL_PAGE     0x0002
#define SPF_BLINK_SELECTED 0x0004
#define SPF_NO_PANEL_TITLE   0x0008
#define SPF_PRELOAD_BKG_IMAGE 0x0010
// flag per comando TriggerGroup
// campo flag
//#define TGROUP_AND						 0x0000 // non usato
#define TGROUP_USE_FOUND_ITEM_INDEX		 0x0001
#define TGROUP_USE_TRIGGER_ITEM_INDEX	 0x0002
#define TGROUP_USE_OWNER_ANIM_ITEM_INDEX 0x0004
#define TGROUP_SINGLE_SHOT_RESUMED       0x0008
#define TGROUP_OR						 0x0010
#define TGROUP_NOT						 0x0020
#define TGROUP_ELSE						 0x0040
#define TGROUP_CONDITION				 0x0080
#define TGROUP_USE_EXECUTOR_ITEM_INDEX   0x0100
#define TGROUP_DISABLED					 0x0200 // non eseguirlo piu'
#define TGROUP_SINGLE_SHOT				 0x0400 // dopo averlo eseguito una volta disattivarlo
#define TGROUP_USE_ITEM_USED_BY_LARA_INDEX   0x0800 // indice di oggetto con cui lara sta interagendo
// questi sono impostati nella fase di exporting e messi nella prima word 
#define TGROUP_CONDITION_TRIGGER		0x8000
#define TGROUP_ACTION					0x4000
#define TGROUP_FLIPEFFECT				0x2000
#define TGROUP_MOVEABLE					0x1000
// valore assoluto di prima word (no maschera bit)
#define TGROUP_COMMAND					0x0003


// commands for TGROUP_COMMAND script flag 
#define TCMD_GOTO				1   // go to Index trigger of current TriggerGroup
#define TCMD_EXIT				2   // quit from current triggergroup, returning the value true/false (1/0) in conditions
#define TCMD_SET_TIMER			3   // 0/255 used only with Flipeffects 
#define TCMD_SET_FULL_TIMER		4   // 0/32767 used only with flipeffect having NO (E)xtra argument
#define TCMD_SET_EXTRA_TIMER	5   // 0/127 used with flipeffects and Action triggers
#define TCMD_SET_EXTRA_CONDITION 6   // 0/31 used only with conditons. It is the (E)xtra argument
#define TCMD_SET_OBJECT			7   // 0/4095 used with conditions and action triggers
#define TCMD_LOG				8   // enable/disable the script log
#define TCMD_PAUSE				9   // feeze the game for a given microseconds or wait for user input (scancode)
#define TCMD_TIMER_FIELD        10
#define TCMD_MAX_ID             11 // limit of TCMD values ATTENZIONE: DUPLICATO PER ROOM EDITOR
// flag ocb per switch1/2/3 trng
#define OCB_SW_FLIP_FLOP 0x2000
#define OCB_SW_INVERSE 0x1000

// flag ocb per rubber boat and motorboat
#define OCB_BOAT_FANALE 0x0001
#define OCB_BOAT_NO_HEAVY_TRIGGER 0x0002
#define OCB_BOAT_NO_LARA_TRIGGER 0x0004
#define OCB_BOAT_LOOK_AROUND 0x0008
#define OCB_BOAT_NO_FUEL 0x0010
#define OCB_BOAT_FUEL_MANAGEMENT 0x0020
#define OCB_BOAT_ANCHORED 0x0040
#define OCB_BOAT_SHOW_FUEL_BAR 0x0080

// flag per kayak
#define OCB_KAYAK_MIST_WAKE 0x0001
#define OCB_KAYAK_GO_ON_JUMPING 0x0002
#define OCB_KAYAK_GO_ON_FROM_LAND 0x0004
#define OCB_KAYAK_NO_LARA_TRIGGERS 0x0008
#define OCB_KAYAK_RAPID_MIST 0x0010
#define OCB_KAYAK_LOOK_AROUND 0x0020

// costanti per indicare il tipo di veicoo guidato da lara
#define VEC_NONE  -1  // nessun veicolo
#define VEC_JEEP 0 
#define VEC_SIDECAR 1
#define VEC_RUBBER_BOAT 2
#define VEC_MOTOR_BOAT 3
#define VEC_UNKNOWN 100

// flag per diario
#define LDF_SILENT         0x0100
#define LDF_PLAY_TRACK     0x0200
#define LDF_SOUND_EFFECTS    0x0400
#define LDF_ZOOM_START     0x0800
#define LDF_TRANSPARENT_BKG    0x2000

// Page Layout --- i primi 7 sono layout preordinati
#define PL_DOUBLE_PAGE     0x0001
#define PL_LEFT_IMAGE      0x0002
#define PL_LEFT_IMAGE_LOGO 0x0003
#define PL_RIGHT_IMAGE     0x0004
#define PL_RIGHT_IMAGE_LOGO 0x0005
#define PL_CENTRAL_IMAGE   0x0006
#define PL_WIDE_IMAGE      0x0007

#define PL_ADD_INFO_BAR    0x0100
#define PL_CUSTOM_LAYOUT   0x0200
#define PL_FIX_WIDE_SCREEN 0x0400

// flags per windows font
#define WFF_BOLD 0x0001
#define WFF_ULTRA_BOLD 0x0002
#define WFF_LIGHT 0x0004
#define WFF_SHADOW 0x0008
#define WFF_ITALIC 0x0010
#define WFF_UNDERLINE 0x0020
#define WFF_LEFT_ALIGN 0x0080
#define WFF_RIGHT_ALIGN 0x0100
#define WFF_CENTER_ALIGN 0x0200
#define WFF_FORCE_FIXED_PITCH 0x0400
#define WFF_ROTATE_90 0x0800
#define WFF_ROTATE_INV_90 0x1000
#define WFF_FROM_RIGHT_TO_LEFT 0x2000
#define WFF_UNICODE 0x4000
#define WFF_UTF8 0x8000


// flags per PARAM_WTEXT command
#define WTF_FLYING_TEXT 0x0001
#define WTF_PULSING_TEXT 0x0002
#define WTF_CHANGE_COLOR 0x0004
#define WTF_OVER_INVENTORY 0x0008
#define WTF_OVER_IMAGE 0x0010
#define WTF_OVER_FLYCAMERA 0x0020
#define WTF_OVER_FIXCAMERA 0x0040
#define WTF_OVER_BINOCULAR 0x0080
#define WTF_OVER_LASER_SIGHT 0x0100


// flag per record pulsanti duplicati di ButtonWindow
#define FB_NONE 0

// costanti per EXTRA

#define EXTRA_TEETH_NO_DAMAGE_ON_WALKING 1
#define EXTRA_MUTANT_NO_LOCUSTS 1
#define EXTRA_WRAITH_BURN_LARA 1

// costanti per tipo di estensione da usare
#define SEXT_WAV 0  
#define SEXT_OGG 1 
#define SEXT_MP3 2 
#define SEXT_MP2 3 
#define SEXT_MP1 4 
#define SEXT_AIFF 5 
#define SEXT_MULTIPLE 100 
// flags di tipo customize
#define CUST_DISABLE_SCREAMING_HEAD 1
#define CUST_SET_SECRET_NUMBER 2
#define CUST_SET_CREDITS_LEVEL 3
#define CUST_DISABLE_FORCING_ANIM_96 4
#define CUST_ROLLINGBALL_PUSHING 5
#define CUST_NEW_SOUND_ENGINE 6
#define CUST_SPEED_MOVING 7
#define CUST_SHATTER_RANGE 8
#define CUST_WEAPON 9
#define CUST_AMMO 10
#define CUST_SHOW_AMMO_COUNTER 11
#define CUST_SET_INV_ITEM 12
#define CUST_SET_JEEP_KEY_SLOT 13
#define CUST_STATIC_TRANSPARENCY 14
#define CUST_SET_STATIC_DAMAGE 15
#define CUST_LOOK_TRASPARENT 16
#define CUST_HAIR_TYPE 17
#define CUST_KEEP_DEAD_ENEMIES 18
#define CUST_SET_OLD_CD_TRIGGER 19
#define CUST_ESCAPE_FLY_CAMERA 20
#define CUST_PAUSE_FLY_CAMERA 21
#define CUST_TEXT_ON_FLY_SCREEN 22
#define CUST_CD_SINGLE_PLAYBACK 23
#define CUST_ADD_DEATH_ANIMATION 24
#define CUST_BAR 25
#define CUST_NO_TIME_IN_SAVELIST 26
#define CUST_PARALLEL_BARS 27
#define CUST_CAMERA 28
#define CUST_DISABLE_MISSING_SOUNDS 29
#define CUST_INNER_SCREENSHOT 30
#define CUST_FMV_CUTSCENE 31
#define CUST_FIX_WATER_FOG_BUG 32
#define CUST_SAVE_LOCUST 33
#define CUST_LIGHT_OBJECT 34
#define CUST_HARPOON 35
#define CUST_SCREENSHOT_CAPTURE 36
#define CUST_RAIN 37
#define CUST_TR5_UNDERWATER_COLLISIONS 38
#define CUST_DARTS 39
#define CUST_FLARE 40
#define CUST_SET_TEXT_COLOR 41
#define CUST_SET_STILL_COLLISION 42
#define CUST_WATERFALL_SPEED 43
#define CUST_ROLLING_BOAT 44
#define CUST_SFX 45
#define CUST_TITLE_FMV 46
#define CUST_KEEP_LARA_HP 47
#define CUST_BIKE_VS_ENEMIES 48
#define CUST_BACKGROUND 49
#define CUST_DISABLE_PUSH_AWAY_ANIMATION 50
#define CUST_BINOCULARS 51
#define CUST_SLOT_FLAGS 52
#define CUST_FIX_BUGS 53
#define CUST_SHATTER_SPECIFIC 54
#define CUST_LAST  55

// BUGF_... flag per CUST_FIX_BUGS
#define BUGF_TRANSPARENT_WHITE_ON_FOG 0x0001
#define BUGF_DART_NO_POISON_LARA 0x0002
#define BUGF_LAND_WATER_SFX_ENEMIES 0x0004
// FFS_ ... flags
#define FFS_PUSHABLE_CAN_OVERSTEP_IT  0x0001

// BINF_ ... BINoculars Flags
#define BINF_COMPASS 0x0001
#define BINF_SEXTANT 0x0002
#define BINF_LIGHTNESS 0x0004
#define BINF_ZOOM 0x0008
#define BINF_LIGHT_SWITCH 0x0080
#define BINF_SOUND_ON_LIGHT 0x0100
#define BINF_NOTATION_EXTENDED 0x0200  
#define BINF_SWING_COMPASS 0x0400
#define BINF_SWING_SEXTANT 0x0800
#define BINF_SUPER_ZOOM 0x1000
#define BINF_PROGRESSIVE_ZOOM 0x2000
#define BINF_SOUND_ON_ZOOM 0x4000


// BINT_  ... BINoculars Types
#define BINT_STRIP 0x8000
#define BINT_BAR 0x4000
#define BINT_NUMERIC 0x2000
#define BINT_PATTERN_BAR 0x1000
#define BINT_ROUND 0x0800   // only for lightswitch


#define BINT_MASK_ID 0x03FF  // to get the index of param_rect


// types per Customize=CUST_BACKGROUND
#define BKGDT_INVENTORY 1
#define BKGDT_LOADING_LEVEL 2
#define BKGDT_TITLE 3
#define BKGDT_BINOCULAR 4
#define BKGDT_LASER_SIGHT 5


// flags per Customize=CUST_BACKGROUND
#define BKGDF_HIDE_LOADING_BAR 0x0001
#define BKGDF_MINIMAL_LOADING_TIME 0x0002
#define BKGDF_HIDE_SPRITE_LASER_SIGHT 0x0004
#define BKGDF_COLORIZE 0x0008
#define BKGDF_KEEP_GAME_SCREEN 0x0010

#define BKGDF_ADD_COLOR_OR 0x0020
#define BKGDF_ADD_COLOR_AND 0x0040
#define BKGDF_ADD_COLOR_XOR 0x0080
#define BKGDF_ADD_NEGATIVE_EFFECT 0x0100
#define BKGDF_ADD_DARKNESS 0x200
#define BKGDF_SEMI_TRANSPARENT 0x400
#define BKGDF_SKIP_LOADING_TIME 0x800

// flags for extra activity off normal game
#define SKIP_NONE 0x0000
#define SKIP_FIXED_CAMERA 0x0001
#define SKIP_FLY_CAMERA   0x0002
#define SKIP_LOADING_LEVEL 0x0004
#define SKIP_FADE 0x0008
#define SKIP_TITLE_LEVEL 0x0010
#define SKIP_GRAY_SCREEN 0x0020  // pause, inventory load/save game from the game
#define SKIP_NO_VIEW_OGGETTI 0x0040
#define SKIP_BINOCULARS 0x0080
#define SKIP_LASER_SIGHT 0x0100
#define SKIP_FULL_IMAGE 0x200 // creata da trng 

// flag gestione demo FGD_...
#define FDG_NONE 0
#define FDG_START 0x0001
#define FDG_END   0x0002
#define FDG_END_FOR_ESCAPE 0x0004
#define FDG_END_FOR_TRIGGER 0x0008  //required stot demo via trigger
#define FDG_FROM_TITLE  0x0010  // demo launched from title demo
#define FDG_IN_PROGRESS 0x0020 // in the midle of playing

// values for Emergeny Settings (in register of windows)
#define ES_DISABLED 0
#define ES_SOFT_FULL_SCREEN 1
#define ES_NO_WAITING_REFRESH 2

// flag per collisione di bike con nemici
#define HIT_KILL           0x0400 
#define HIT_PUSH_AWAY      0x0800
#define HIT_HURT           0x1000
#define HIT_WALL           0x2000
#define HIT_EXPLODE        0x4000
#define HIT_BIKE_EXPLODE   0x8000

#define HIT_MASK_SLOT      0x03ff
#define HIT_MASK_FLAGS     0xFC00



#define KLH_ALL_LEVELS -2

#define TS_DIARY_CHANGE_PAGE 1  // 7
#define TS_DIARY_NO_PAGE 2      // 2
#define TS_DIARY_ZOOM_START 3  // 76
#define TS_SCREENSHOT_CAPTURE 4 // 9
#define TS_VIBRATE_RESUME_FROM_FROZEN 5 // 0
#define TS_AFTER_FALLING_AS_INVULNERABLE 6 // 1
#define TS_SAVEGAME_PANEL_SELECTED 7 // 109
#define TS_DAMAGE_ROOM_SCREAM_BURNING 8 // 0x1e
#define TS_DAMAGE_ROOM_BEEP_ALERT 9 // 0x70
#define TS_WHIRLPOOL_SINKED_LARA 10 // 33
#define TS_MISSING_REQUIRED_ITEM 11 // 2
#define TS_ANIMATING_DOOR_OPEN 12 // 162
#define TS_ANIMATING_DOOR_CLOSE 13 // 162
#define TS_DETECTOR_SHOW 14 // 0x27
#define TS_ELEVATOR_SQUASHED_LARA 15 // 0x1f
#define TS_MIST_EMITTER_WITH_OCB 16 // 0x4f
#define TS_PUSHED_ITEM_IMPACT 17 // 72 
#define TS_BINOCULAR_ZOOM 18  // 309 mapper_move
#define TS_BINOCULAR_LIGHT 19  // 369 LARA_CLICK_SWITCH
#define TS_SHOT_HARPOON_UW 20  // 68 PENDULUM_BLADES




#define FRB_SWINGING_NORMAL 0
#define FRB_SWINGING_LOW 0x0001
#define FRB_SWINGING_HIGH 0x0002
#define FRB_PITCHING_NORMAL 0
#define FRB_PITCHING_LOW 0x0004
#define FRB_PITCHING_HIGH 0x0008
#define FRB_ALLOW_DRIFT 0x0010

// flag per still collision
#define COLL_STATICS 0x0001
#define COLL_DOORS   0x0002
#define COLL_ANIMATINGS 0x0004
#define COLL_PANELS  0x0008
#define COLL_VEHICLES 0x0010
#define COLL_FAKE_WALLS 0x0020
#define COLL_STOP_ON_45_DEGREES 0x0040
#define COLL_FAST_TURNING 0x0080
#define COLL_NO_SPLAT 0x0200
#define COLL_NO_SLIDING 0x0400
#define COLL_ENABLED 0x8000 // uso interno

#define SHOWC_OMIT_AMMO_NAME 0x0001
#define SHOWC_USE_GRAPHIC_AMMO 0x0002
#define SHOWC_USE_GRAPHIC_WEAPON 0x0004

// flag per show counter ammo

// flag per customize flare

#define FFL_ADD_SPARKS 0x0001
#define FFL_ADD_FIRE 0x0002
#define FFL_ADD_SMOKE_TO_SPARKS 0x0004
#define FFL_ADD_GLOW_LIGHT 0x0008
#define FFL_FLAT_LIGHT 0x0010



// flag per struttura particelle fish
// campo Flags
#define FISH_SLOW      0x02
#define FISH_TIMID     0x04
#define FISH_JUMP      0x08
#define FISH_NO_BRANCO 0x10
#define FISH_MASK_MESH 0x60
#define FISH_ATTACK    0x80

// campo TipoFase
// tipi di fase
#define FISH_NO_TARGET         0
#define FISH_FOLLOW_PIVOT      1
#define FISH_FOLLOW_LARA       2 
#define FISH_JUMP_OUT          3
#define FISH_DYING_ON_GROUND   4
#define FISH_DEATH_ON_GROUND   5
#define FISH_BUBBLES		   6
#define FISH_ESCAPE_FROM_LARA  7


// ocb per pesci

#define OCB_FISH_MASK_NUMBER 0x007F
#define OCB_FISH_SLOW		 0x0080
#define OCB_FISH_FRIEND_FISH 0x0100
#define OCB_FISH_NO_BRANCO   0x0200
#define OCB_FISH_PIRAHNA     0x0400
#define OCB_FISH_CLOWN       0x0800
#define OCB_FISH_BUTTERFLY   0x1000
#define OCB_FISH_ANGEL       0x2000
#define OCB_FISH_JUMP        0x4000
#define OCB_FISH_TIMID       0x8000



// flag per CUST_DART
#define DRT_NO_POISON 0x0001
#define DRT_ADD_EFFECT 0x0002
#define DRT_PERFOM_TRIGGERGROUP 0x0004
#define DRT_NO_SMOKE 0x0008
#define DRT_HIDE_DART 0x0010
#define DRT_FIX_POISON_BUG 0x0020

// flagsper harpoon
#define HRP_NO_SWIM_UNDERWATER 0x0001
#define HRP_SINGLE_AMMO 0x0002
#define HRP_DOUBLE_AMMO 0x0004
#define HRP_DISABLE_LASER_SIGHT 0x0008

#define FMV_FADE_OUT  0x01
#define FMV_SHORT_BLACK_RESTART 0x02
#define FMV_LONG_BLACK_RESTART 0x04
#define FMV_NO_AUDIO_RESTART 0x08
#define FMV_PRE_CACHE 0x10

// flag per CUST_CAMERA
#define FCAM_DISABLE_COMBAT_CAM 0x0001
#define FCAM_INVISIBLE_LARA_ON_LOOK_CAM 0x0002
// flag per PARAM_SET_CAMERA

#define FSCAM_DISABLE_COMBAT_CAM 0x0001
#define FSCAM_DISABLE_LOOK_CAM   0x0002
// costanti per tipo barra
#define BAR_HEALTH 0
#define BAR_DASH   1
#define BAR_AIR    2
#define BAR_LOAD_LEVEL 3
#define BAR_CUSTOM1 4
#define BAR_CUSTOM2 5
#define BAR_CUSTOM3 6
#define BAR_CUSTOM4 7
#define BAR_DAMAGE 8
#define BAR_COLD 9
#define BAR_CUST_TOT 10


// flag per tipo di barre
#define FBAR_SOUND_BAR_ANIM 0x0001
#define FBAR_SHOW_BAR_NAME 0x0002
#define FBAR_USED_FOR_BOAT_FUEL 0x0004
#define FBAR_DRAW_ALWAYS 0x0008


// costanti per tipo di customizzazione di salvatggio di cd con single-playback
#define CDM_NO_SAVE 1
#define CDM_RESTORE_FROM_MIDDLE 2
#define CDM_RESTORE_FROM_BEGIN 3

// flag per Customize=CUST_PARALLEL_BARS

#define PB_PROGRESSIVE_CHARGE 0x0001
#define PB_LARA_CAN_SLIDE 0x0002
#define PB_MULTIPLE_ENDINGS 0x0004
#define PB_DOUBLE_FACE 0x0008
#define PB_SHOW_CHARGE_BAR 0x0010
#define PB_SHOW_CHARGE_COUNTER 0x0020
// costranti per tipo di exe

#define EXE_TOMB4  1
#define EXE_EDITOR 2
#define EXE_TOM2PC 3
#define EXE_TOMB5  4
#define EXE_TOMB3  5

// flag per dati binary nuovo font
#define FS_Custom  0x100
#define FS_Redirect  0x200
#define FS_Spazio  0x400
#define FS_Comando  0x800
#define FS_Ignora  0x1000
#define FS_Binary  0x2000
// --- flag per customize munizioni ammo

#define AMMO_PUSH_TARGET 0x0002
#define AMMO_PUSH_LARA 0x0004
#define AMMO_SET_GRENADE_TIMER 0x0008
#define AMMO_ADD_GUN_SHELL 0x0010
#define AMMO_ADD_SHOTGUN_SHELL 0x0020
#define AMMO_REMOVE_SHOTGUN_SHELL 0x0040


// -- flag per weapon customize -----
#define WEAP_DISABLE_AUTO_SHOT 0x0001
#define WEAP_DISABLE_CHANGE_WEAPON 0x0002



// flag per snow e rain
#define RAIN_DISABLED 0
#define RAIN_SINGLE_ROOMS 1
#define RAIN_ALL_OUTSIDE 2

#define SNOW_DISABLED 0
#define SNOW_SINGLE_ROOM 1
#define SNOW_ALL_OUTSIDE 2
// flag per detector (comando script)
#define DTF_NONE 0
#define DTF_SWINGING_POINTER 0x01
#define DTF_RADAR_MODE 0x02
#define DTF_ENGAGE_INVENTORY 0x04
#define DTF_ENGAGE_IN_RANGE 0x08
#define DTF_REQUIRED_ITEM 0x10
#define DTF_ENGAGE_ALWAYS 0x20
#define DTF_FAST_RADAR_SCAN 0x40
#define DTF_INVERSE_VPOINTER 0x80

// valore di fase per target FTR_
// FTR_ 0= nascita / 1 = lampeggio fisso / -1 non attivo
#define FTR_NASCITA 0
#define FTR_LAMPEGGIO 1
#define FTR_INATTIVO -1

// durata in frame delle diverse fasi
#define DTAR_NASCITA (FRAME_SECONDO / 4)
#define DTAR_LAMPEGGIO_TONDO (FRAME_SECONDO / 8)
#define DTAR_LAMPEGGIO_TRIANGOLO (FRAME_SECONDO / 2)
#define DTAR_DURATA_TARGET (FRAME_SECONDO * 7)

#define PTR_BASE_LARA 0
#define PTR_SOPRA_LARA 1
#define PTR_SOTTO_LARA -1

// velocita' di rotazione di lancetta per radar
#define RADAR_SPEED 256
#define DISTANZA_NULLA 100000

// -- codici per nuovi oggetti OBJ_
#define		OBJ_RUBBER_BOAT 501
#define		OBJ_MOTOR_BOAT  502
#define		OBJ_SUB_PROPULSOR 503
#define		OBJ_MINE_CART 504
#define		OBJ_RUBBER_BOAT_ANIM 505
#define		OBJ_MOTOR_BOAT_ANIM 506
#define		OBJ_SUB_PROPULSOR_ANIM 507
#define		OBJ_MINE_CART_ANIM  508

// ---- flag per level_tr4 
#define		FLT_NONE 0
#define		FLT_NEW_TRIGGERS 0x0001
#define     FLT_EXTRA_SOUND_TABLE 0x0002



#define NO_HANG_NORTH 1
#define NO_HANG_EAST  2
#define NO_HANG_SOUTH 4
#define NO_HANG_WEST  8

// lista costanti per accedere a byte di savegame di vettore
// che parte da pistole
#define SAVE_PISTOLA  0
#define SAVE_UZI 1
#define SAVE_FUCILE 2
#define SAVE_BALESTRA 3
#define SAVE_LANCIA_GRANATE 4
#define SAVE_REVOLVER 5

// flag per scaling static
#define FSCA_IMMEDIATE 0x0001
#define FSCA_ENDLESS 0x0002
#define FSCA_ITEMGROUP_INDEX 0x0004


#define TFF_NONE 0
#define TFF_VERT_STRIP 0x10
#define TFF_STRIP 0x20
#define TFF_CROSSED_STRIP 0x40
#define TFF_DIAGONAL 0x80


#define ALTEZZA_ACQUA -710  ;era -775


// flag per bit extrabuttons di fragment trigger
#define FRF_PAD 0x1
#define FRF_INVERSE 0x2

// values to return by cbCycleEnd callback
#define RET_CYCLE_CONTINUE 0  // common value to continue the game cycle
#define RET_CYCLE_QUIT_GAME_CYCLE  0x80  // (internal) quit temporarily current game cycle but keep current level
#define RET_CYCLE_QUIT_AND_GO_TITLE  0x81  // force loading title level
#define RET_CYCLE_QUIT_AND_LOAD_LEVEL 0x82 // (internal) after having loaded a savegame. 
											//In Trng.pGlobTomb4->pAdr->pLevelNext there should be the number of level to load
#define RET_CYCLE_QUIT_AND_NEW_LEVEL  0x83   // set in Trng.pGlobTomb4->pAdr->pLevelNext variable the number of new level to load
#define RET_CYCLE_QUIT_PROGRAM 0x84  // from title level, quit tomb raider program, from level go to title
#define RET_CYCLE_QUIT_RELOAD_CURRENT_LEVEL 0x85 // (internal)

// #TAG_START_AUTO_ENUM_STUFF#
// ----------- define for SLOT_ trigger constants -------------
#define SLOT_LARA  0		// [0] slot
#define SLOT_PISTOLS_ANIM  1		// [1] slot
#define SLOT_UZI_ANIM  2		// [2] slot
#define SLOT_SHOTGUN_ANIM  3		// [3] slot
#define SLOT_CROSSBOW_ANIM  4		// [4] slot
#define SLOT_GRENADE_GUN_ANIM  5		// [5] slot
#define SLOT_SIXSHOOTER_ANIM  6		// [6] slot
#define SLOT_FLARE_ANIM  7		// [7] slot
#define SLOT_LARA_SKIN  8		// [8] slot
#define SLOT_LARA_SKIN_JOINTS  9		// [9] slot
#define SLOT_LARA_SCREAM  10		// [10] slot
#define SLOT_LARA_CROSSBOW_LASER  11		// [11] slot
#define SLOT_LARA_REVOLVER_LASER  12		// [12] slot
#define SLOT_LARA_HOLSTERS  13		// [13] slot
#define SLOT_LARA_HOLSTERS_PISTOLS  14		// [14] slot
#define SLOT_LARA_HOLSTERS_UZIS  15		// [15] slot
#define SLOT_LARA_HOLSTERS_SIXSHOOTER  16		// [16] slot
#define SLOT_LARA_SPEECH_HEAD1  17		// [17] slot
#define SLOT_LARA_SPEECH_HEAD2  18		// [18] slot
#define SLOT_LARA_SPEECH_HEAD3  19		// [19] slot
#define SLOT_LARA_SPEECH_HEAD4  20		// [20] slot
#define SLOT_ACTOR1_SPEECH_HEAD1  21		// [21] slot
#define SLOT_ACTOR1_SPEECH_HEAD2  22		// [22] slot
#define SLOT_ACTOR2_SPEECH_HEAD1  23		// [23] slot
#define SLOT_ACTOR2_SPEECH_HEAD2  24		// [24] slot
#define SLOT_LARA_WATER_MESH  25		// [25] slot
#define SLOT_LARA_PETROL_MESH  26		// [26] slot
#define SLOT_LARA_DIRT_MESH  27		// [27] slot
#define SLOT_CROWBAR_ANIM  28		// [28] slot
#define SLOT_TORCH_ANIM  29		// [29] slot
#define SLOT_HAIR  30		// [30] slot
#define SLOT_MOTORBIKE  31		// [31] slot
#define SLOT_JEEP  32		// [32] slot
#define SLOT_VEHICLE_EXTRA  33		// [33] slot
#define SLOT_ENEMY_JEEP  34		// [34] slot
#define SLOT_SKELETON  35		// [35] slot
#define SLOT_SKELETON_MIP  36		// [36] slot
#define SLOT_GUIDE  37		// [37] slot
#define SLOT_GUIDE_MIP  38		// [38] slot
#define SLOT_VON_CROY  39		// [39] slot
#define SLOT_VON_CROY_MIP  40		// [40] slot
#define SLOT_BADDY_1  41		// [41] slot
#define SLOT_BADDY_1_MIP  42		// [42] slot
#define SLOT_BADDY_2  43		// [43] slot
#define SLOT_BADDY_2_MIP  44		// [44] slot
#define SLOT_SETHA  45		// [45] slot
#define SLOT_SETHA_MIP  46		// [46] slot
#define SLOT_MUMMY  47		// [47] slot
#define SLOT_MUMMY_MIP  48		// [48] slot
#define SLOT_SPHINX  49		// [49] slot
#define SLOT_SPHINX_MIP  50		// [50] slot
#define SLOT_CROCODILE  51		// [51] slot
#define SLOT_CROCODILE_MIP  52		// [52] slot
#define SLOT_HORSEMAN  53		// [53] slot
#define SLOT_HORSEMAN_MIP  54		// [54] slot
#define SLOT_SCORPION  55		// [55] slot
#define SLOT_SCORPION_MIP  56		// [56] slot
#define SLOT_JEAN_YVES  57		// [57] slot
#define SLOT_JEAN_YVES_MIP  58		// [58] slot
#define SLOT_TROOPS  59		// [59] slot
#define SLOT_TROOPS_MIP  60		// [60] slot
#define SLOT_KNIGHTS_TEMPLAR  61		// [61] slot
#define SLOT_KNIGHTS_TEMPLAR_MIP  62		// [62] slot
#define SLOT_MUTANT  63		// [63] slot
#define SLOT_MUTANT_MIP  64		// [64] slot
#define SLOT_HORSE  65		// [65] slot
#define SLOT_HORSE_MIP  66		// [66] slot
#define SLOT_BABOON_NORMAL  67		// [67] slot
#define SLOT_BABOON_NORMAL_MIP  68		// [68] slot
#define SLOT_BABOON_INV  69		// [69] slot
#define SLOT_BABOON_INV_MIP  70		// [70] slot
#define SLOT_BABOON_SILENT  71		// [71] slot
#define SLOT_BABOON_SILENT_MIP  72		// [72] slot
#define SLOT_WILD_BOAR  73		// [73] slot
#define SLOT_WILD_BOAR_MIP  74		// [74] slot
#define SLOT_HARPY  75		// [75] slot
#define SLOT_HARPY_MIP  76		// [76] slot
#define SLOT_DEMIGOD1  77		// [77] slot
#define SLOT_DEMIGOD1_MIP  78		// [78] slot
#define SLOT_DEMIGOD2  79		// [79] slot
#define SLOT_DEMIGOD2_MIP  80		// [80] slot
#define SLOT_DEMIGOD3  81		// [81] slot
#define SLOT_DEMIGOD3_MIP  82		// [82] slot
#define SLOT_LITTLE_BEETLE  83		// [83] slot
#define SLOT_BIG_BEETLE  84		// [84] slot
#define SLOT_BIG_BEETLE_MIP  85		// [85] slot
#define SLOT_WRAITH1  86		// [86] slot
#define SLOT_WRAITH2  87		// [87] slot
#define SLOT_WRAITH3  88		// [88] slot
#define SLOT_WRAITH4  89		// [89] slot
#define SLOT_BAT  90		// [90] slot
#define SLOT_DOG  91		// [91] slot
#define SLOT_DOG_MIP  92		// [92] slot
#define SLOT_HAMMERHEAD  93		// [93] slot
#define SLOT_HAMMERHEAD_MIP  94		// [94] slot
#define SLOT_SAS  95		// [95] slot
#define SLOT_SAS_MIP  96		// [96] slot
#define SLOT_SAS_DYING  97		// [97] slot
#define SLOT_SAS_DYING_MIP  98		// [98] slot
#define SLOT_SAS_CAPTAIN  99		// [99] slot
#define SLOT_SAS_CAPTAIN_MIP  100		// [100] slot
#define SLOT_SAS_DRAG_BLOKE  101		// [101] slot
#define SLOT_AHMET  102		// [102] slot
#define SLOT_AHMET_MIP  103		// [103] slot
#define SLOT_LARA_DOUBLE  104		// [104] slot
#define SLOT_LARA_DOUBLE_MIP  105		// [105] slot
#define SLOT_SMALL_SCORPION  106		// [106] slot
#define SLOT_LOCUST_EMITTER  107		// [107] slot
#define SLOT_GAME_PIECE1  108		// [108] slot
#define SLOT_GAME_PIECE2  109		// [109] slot
#define SLOT_GAME_PIECE3  110		// [110] slot
#define SLOT_ENEMY_PIECE  111		// [111] slot
#define SLOT_WHEEL_OF_FORTUNE  112		// [112] slot
#define SLOT_SCALES  113		// [113] slot
#define SLOT_DARTS  114		// [114] slot
#define SLOT_DART_EMITTER  115		// [115] slot
#define SLOT_HOMING_DART_EMITTER  116		// [116] slot
#define SLOT_FALLING_CEILING  117		// [117] slot
#define SLOT_FALLING_BLOCK  118		// [118] slot
#define SLOT_FALLING_BLOCK2  119		// [119] slot
#define SLOT_SMASHABLE_BIKE_WALL  120		// [120] slot
#define SLOT_SMASHABLE_BIKE_FLOOR  121		// [121] slot
#define SLOT_TRAPDOOR1  122		// [122] slot
#define SLOT_TRAPDOOR2  123		// [123] slot
#define SLOT_TRAPDOOR3  124		// [124] slot
#define SLOT_FLOOR_TRAPDOOR1  125		// [125] slot
#define SLOT_FLOOR_TRAPDOOR2  126		// [126] slot
#define SLOT_CEILING_TRAPDOOR1  127		// [127] slot
#define SLOT_CEILING_TRAPDOOR2  128		// [128] slot
#define SLOT_SCALING_TRAPDOOR  129		// [129] slot
#define SLOT_ROLLINGBALL  130		// [130] slot
#define SLOT_SPIKEY_FLOOR  131		// [131] slot
#define SLOT_TEETH_SPIKES  132		// [132] slot
#define SLOT_JOBY_SPIKES  133		// [133] slot
#define SLOT_SLICER_DICER  134		// [134] slot
#define SLOT_CHAIN  135		// [135] slot
#define SLOT_PLOUGH  136		// [136] slot
#define SLOT_STARGATE  137		// [137] slot
#define SLOT_HAMMER  138		// [138] slot
#define SLOT_BURNING_FLOOR  139		// [139] slot
#define SLOT_COG  140		// [140] slot
#define SLOT_SPIKEBALL  141		// [141] slot
#define SLOT_FLAME  142		// [142] slot
#define SLOT_FLAME_EMITTER  143		// [143] slot
#define SLOT_FLAME_EMITTER2  144		// [144] slot
#define SLOT_FLAME_EMITTER3  145		// [145] slot
#define SLOT_ROPE  146		// [146] slot
#define SLOT_FIREROPE  147		// [147] slot
#define SLOT_POLEROPE  148		// [148] slot
#define SLOT_ONEBLOCK_PLATFORM  149		// [149] slot
#define SLOT_TWOBLOCK_PLATFORM  150		// [150] slot
#define SLOT_RAISING_BLOCK1  151		// [151] slot
#define SLOT_RAISING_BLOCK2  152		// [152] slot
#define SLOT_EXPANDING_PLATFORM  153		// [153] slot
#define SLOT_SQUISHY_BLOCK1  154		// [154] slot
#define SLOT_SQUISHY_BLOCK2  155		// [155] slot
#define SLOT_PUSHABLE_OBJECT1  156		// [156] slot
#define SLOT_PUSHABLE_OBJECT2  157		// [157] slot
#define SLOT_PUSHABLE_OBJECT3  158		// [158] slot
#define SLOT_PUSHABLE_OBJECT4  159		// [159] slot
#define SLOT_PUSHABLE_OBJECT5  160		// [160] slot
#define SLOT_TRIPWIRE  161		// [161] slot
#define SLOT_SENTRY_GUN  162		// [162] slot
#define SLOT_MINE  163		// [163] slot
#define SLOT_MAPPER  164		// [164] slot
#define SLOT_OBELISK  165		// [165] slot
#define SLOT_FLOOR_4BLADE  166		// [166] slot
#define SLOT_ROOF_4BLADE  167		// [167] slot
#define SLOT_BIRD_BLADE  168		// [168] slot
#define SLOT_CATWALK_BLADE  169		// [169] slot
#define SLOT_MOVING_BLADE  170		// [170] slot
#define SLOT_PLINTH_BLADE  171		// [171] slot
#define SLOT_SETH_BLADE  172		// [172] slot
#define SLOT_LIGHTNING_CONDUCTOR  173		// [173] slot
#define SLOT_ELEMENT_PUZZLE  174		// [174] slot
#define SLOT_PUZZLE_ITEM1  175		// [175] slot
#define SLOT_PUZZLE_ITEM2  176		// [176] slot
#define SLOT_PUZZLE_ITEM3  177		// [177] slot
#define SLOT_PUZZLE_ITEM4  178		// [178] slot
#define SLOT_PUZZLE_ITEM5  179		// [179] slot
#define SLOT_PUZZLE_ITEM6  180		// [180] slot
#define SLOT_PUZZLE_ITEM7  181		// [181] slot
#define SLOT_PUZZLE_ITEM8  182		// [182] slot
#define SLOT_PUZZLE_ITEM9  183		// [183] slot
#define SLOT_PUZZLE_ITEM10  184		// [184] slot
#define SLOT_PUZZLE_ITEM11  185		// [185] slot
#define SLOT_PUZZLE_ITEM12  186		// [186] slot
#define SLOT_PUZZLE_ITEM1_COMBO1  187		// [187] slot
#define SLOT_PUZZLE_ITEM1_COMBO2  188		// [188] slot
#define SLOT_PUZZLE_ITEM2_COMBO1  189		// [189] slot
#define SLOT_PUZZLE_ITEM2_COMBO2  190		// [190] slot
#define SLOT_PUZZLE_ITEM3_COMBO1  191		// [191] slot
#define SLOT_PUZZLE_ITEM3_COMBO2  192		// [192] slot
#define SLOT_PUZZLE_ITEM4_COMBO1  193		// [193] slot
#define SLOT_PUZZLE_ITEM4_COMBO2  194		// [194] slot
#define SLOT_PUZZLE_ITEM5_COMBO1  195		// [195] slot
#define SLOT_PUZZLE_ITEM5_COMBO2  196		// [196] slot
#define SLOT_PUZZLE_ITEM6_COMBO1  197		// [197] slot
#define SLOT_PUZZLE_ITEM6_COMBO2  198		// [198] slot
#define SLOT_PUZZLE_ITEM7_COMBO1  199		// [199] slot
#define SLOT_PUZZLE_ITEM7_COMBO2  200		// [200] slot
#define SLOT_PUZZLE_ITEM8_COMBO1  201		// [201] slot
#define SLOT_PUZZLE_ITEM8_COMBO2  202		// [202] slot
#define SLOT_KEY_ITEM1  203		// [203] slot
#define SLOT_KEY_ITEM2  204		// [204] slot
#define SLOT_KEY_ITEM3  205		// [205] slot
#define SLOT_KEY_ITEM4  206		// [206] slot
#define SLOT_KEY_ITEM5  207		// [207] slot
#define SLOT_KEY_ITEM6  208		// [208] slot
#define SLOT_KEY_ITEM7  209		// [209] slot
#define SLOT_KEY_ITEM8  210		// [210] slot
#define SLOT_KEY_ITEM9  211		// [211] slot
#define SLOT_KEY_ITEM10  212		// [212] slot
#define SLOT_KEY_ITEM11  213		// [213] slot
#define SLOT_KEY_ITEM12  214		// [214] slot
#define SLOT_KEY_ITEM1_COMBO1  215		// [215] slot
#define SLOT_KEY_ITEM1_COMBO2  216		// [216] slot
#define SLOT_KEY_ITEM2_COMBO1  217		// [217] slot
#define SLOT_KEY_ITEM2_COMBO2  218		// [218] slot
#define SLOT_KEY_ITEM3_COMBO1  219		// [219] slot
#define SLOT_KEY_ITEM3_COMBO2  220		// [220] slot
#define SLOT_KEY_ITEM4_COMBO1  221		// [221] slot
#define SLOT_KEY_ITEM4_COMBO2  222		// [222] slot
#define SLOT_KEY_ITEM5_COMBO1  223		// [223] slot
#define SLOT_KEY_ITEM5_COMBO2  224		// [224] slot
#define SLOT_KEY_ITEM6_COMBO1  225		// [225] slot
#define SLOT_KEY_ITEM6_COMBO2  226		// [226] slot
#define SLOT_KEY_ITEM7_COMBO1  227		// [227] slot
#define SLOT_KEY_ITEM7_COMBO2  228		// [228] slot
#define SLOT_KEY_ITEM8_COMBO1  229		// [229] slot
#define SLOT_KEY_ITEM8_COMBO2  230		// [230] slot
#define SLOT_PICKUP_ITEM1  231		// [231] slot
#define SLOT_PICKUP_ITEM2  232		// [232] slot
#define SLOT_PICKUP_ITEM3  233		// [233] slot
#define SLOT_PICKUP_ITEM4  234		// [234] slot
#define SLOT_PICKUP_ITEM1_COMBO1  235		// [235] slot
#define SLOT_PICKUP_ITEM1_COMBO2  236		// [236] slot
#define SLOT_PICKUP_ITEM2_COMBO1  237		// [237] slot
#define SLOT_PICKUP_ITEM2_COMBO2  238		// [238] slot
#define SLOT_PICKUP_ITEM3_COMBO1  239		// [239] slot
#define SLOT_PICKUP_ITEM3_COMBO2  240		// [240] slot
#define SLOT_PICKUP_ITEM4_COMBO1  241		// [241] slot
#define SLOT_PICKUP_ITEM4_COMBO2  242		// [242] slot
#define SLOT_EXAMINE1  243		// [243] slot
#define SLOT_EXAMINE2  244		// [244] slot
#define SLOT_EXAMINE3  245		// [245] slot
#define SLOT_CROWBAR_ITEM  246		// [246] slot
#define SLOT_BURNING_TORCH_ITEM  247		// [247] slot
#define SLOT_CLOCKWORK_BEETLE  248		// [248] slot
#define SLOT_CLOCKWORK_BEETLE_COMBO1  249		// [249] slot
#define SLOT_CLOCKWORK_BEETLE_COMBO2  250		// [250] slot
#define SLOT_MINE_DETECTOR  251		// [251] slot
#define SLOT_QUEST_ITEM1  252		// [252] slot
#define SLOT_QUEST_ITEM2  253		// [253] slot
#define SLOT_QUEST_ITEM3  254		// [254] slot
#define SLOT_QUEST_ITEM4  255		// [255] slot
#define SLOT_QUEST_ITEM5  256		// [256] slot
#define SLOT_QUEST_ITEM6  257		// [257] slot
#define SLOT_MAP  258		// [258] slot
#define SLOT_SECRET_MAP  259		// [259] slot
#define SLOT_PUZZLE_HOLE1  260		// [260] slot
#define SLOT_PUZZLE_HOLE2  261		// [261] slot
#define SLOT_PUZZLE_HOLE3  262		// [262] slot
#define SLOT_PUZZLE_HOLE4  263		// [263] slot
#define SLOT_PUZZLE_HOLE5  264		// [264] slot
#define SLOT_PUZZLE_HOLE6  265		// [265] slot
#define SLOT_PUZZLE_HOLE7  266		// [266] slot
#define SLOT_PUZZLE_HOLE8  267		// [267] slot
#define SLOT_PUZZLE_HOLE9  268		// [268] slot
#define SLOT_PUZZLE_HOLE10  269		// [269] slot
#define SLOT_PUZZLE_HOLE11  270		// [270] slot
#define SLOT_PUZZLE_HOLE12  271		// [271] slot
#define SLOT_PUZZLE_DONE1  272		// [272] slot
#define SLOT_PUZZLE_DONE2  273		// [273] slot
#define SLOT_PUZZLE_DONE3  274		// [274] slot
#define SLOT_PUZZLE_DONE4  275		// [275] slot
#define SLOT_PUZZLE_DONE5  276		// [276] slot
#define SLOT_PUZZLE_DONE6  277		// [277] slot
#define SLOT_PUZZLE_DONE7  278		// [278] slot
#define SLOT_PUZZLE_DONE8  279		// [279] slot
#define SLOT_PUZZLE_DONE9  280		// [280] slot
#define SLOT_PUZZLE_DONE10  281		// [281] slot
#define SLOT_PUZZLE_DONE11  282		// [282] slot
#define SLOT_PUZZLE_DONE12  283		// [283] slot
#define SLOT_KEY_HOLE1  284		// [284] slot
#define SLOT_KEY_HOLE2  285		// [285] slot
#define SLOT_KEY_HOLE3  286		// [286] slot
#define SLOT_KEY_HOLE4  287		// [287] slot
#define SLOT_KEY_HOLE5  288		// [288] slot
#define SLOT_KEY_HOLE6  289		// [289] slot
#define SLOT_KEY_HOLE7  290		// [290] slot
#define SLOT_KEY_HOLE8  291		// [291] slot
#define SLOT_KEY_HOLE9  292		// [292] slot
#define SLOT_KEY_HOLE10  293		// [293] slot
#define SLOT_KEY_HOLE11  294		// [294] slot
#define SLOT_KEY_HOLE12  295		// [295] slot
#define SLOT_WATERSKIN1_EMPTY  296		// [296] slot
#define SLOT_WATERSKIN1_1  297		// [297] slot
#define SLOT_WATERSKIN1_2  298		// [298] slot
#define SLOT_WATERSKIN1_3  299		// [299] slot
#define SLOT_WATERSKIN2_EMPTY  300		// [300] slot
#define SLOT_WATERSKIN2_1  301		// [301] slot
#define SLOT_WATERSKIN2_2  302		// [302] slot
#define SLOT_WATERSKIN2_3  303		// [303] slot
#define SLOT_WATERSKIN2_4  304		// [304] slot
#define SLOT_WATERSKIN2_5  305		// [305] slot
#define SLOT_SWITCH_TYPE1  306		// [306] slot
#define SLOT_SWITCH_TYPE2  307		// [307] slot
#define SLOT_SWITCH_TYPE3  308		// [308] slot
#define SLOT_SWITCH_TYPE4  309		// [309] slot
#define SLOT_SWITCH_TYPE5  310		// [310] slot
#define SLOT_SWITCH_TYPE6  311		// [311] slot
#define SLOT_SWITCH_TYPE7  312		// [312] slot
#define SLOT_SWITCH_TYPE8  313		// [313] slot
#define SLOT_UNDERWATER_SWITCH1  314		// [314] slot
#define SLOT_UNDERWATER_SWITCH2  315		// [315] slot
#define SLOT_TURN_SWITCH  316		// [316] slot
#define SLOT_COG_SWITCH  317		// [317] slot
#define SLOT_LEVER_SWITCH  318		// [318] slot
#define SLOT_JUMP_SWITCH  319		// [319] slot
#define SLOT_CROWBAR_SWITCH  320		// [320] slot
#define SLOT_PULLEY  321		// [321] slot
#define SLOT_DOOR_TYPE1  322		// [322] slot
#define SLOT_DOOR_TYPE2  323		// [323] slot
#define SLOT_DOOR_TYPE3  324		// [324] slot
#define SLOT_DOOR_TYPE4  325		// [325] slot
#define SLOT_DOOR_TYPE5  326		// [326] slot
#define SLOT_DOOR_TYPE6  327		// [327] slot
#define SLOT_DOOR_TYPE7  328		// [328] slot
#define SLOT_DOOR_TYPE8  329		// [329] slot
#define SLOT_PUSHPULL_DOOR1  330		// [330] slot
#define SLOT_PUSHPULL_DOOR2  331		// [331] slot
#define SLOT_KICK_DOOR1  332		// [332] slot
#define SLOT_KICK_DOOR2  333		// [333] slot
#define SLOT_UNDERWATER_DOOR  334		// [334] slot
#define SLOT_DOUBLE_DOORS  335		// [335] slot
#define SLOT_BRIDGE_FLAT  336		// [336] slot
#define SLOT_BRIDGE_TILT1  337		// [337] slot
#define SLOT_BRIDGE_TILT2  338		// [338] slot
#define SLOT_SARCOPHAGUS  339		// [339] slot
#define SLOT_SEQUENCE_DOOR1  340		// [340] slot
#define SLOT_SEQUENCE_SWITCH1  341		// [341] slot
#define SLOT_SEQUENCE_SWITCH2  342		// [342] slot
#define SLOT_SEQUENCE_SWITCH3  343		// [343] slot
#define SLOT_SARCOPHAGUS_CUT  344		// [344] slot
#define SLOT_HORUS_STATUE  345		// [345] slot
#define SLOT_GOD_HEAD  346		// [346] slot
#define SLOT_SETH_DOOR  347		// [347] slot
#define SLOT_STATUE_PLINTH  348		// [348] slot
#define SLOT_PISTOLS_ITEM  349		// [349] slot
#define SLOT_PISTOLS_AMMO_ITEM  350		// [350] slot
#define SLOT_UZI_ITEM  351		// [351] slot
#define SLOT_UZI_AMMO_ITEM  352		// [352] slot
#define SLOT_SHOTGUN_ITEM  353		// [353] slot
#define SLOT_SHOTGUN_AMMO1_ITEM  354		// [354] slot
#define SLOT_SHOTGUN_AMMO2_ITEM  355		// [355] slot
#define SLOT_CROSSBOW_ITEM  356		// [356] slot
#define SLOT_CROSSBOW_AMMO1_ITEM  357		// [357] slot
#define SLOT_CROSSBOW_AMMO2_ITEM  358		// [358] slot
#define SLOT_CROSSBOW_AMMO3_ITEM  359		// [359] slot
#define SLOT_CROSSBOW_BOLT  360		// [360] slot
#define SLOT_GRENADE_GUN_ITEM  361		// [361] slot
#define SLOT_GRENADE_GUN_AMMO1_ITEM  362		// [362] slot
#define SLOT_GRENADE_GUN_AMMO2_ITEM  363		// [363] slot
#define SLOT_GRENADE_GUN_AMMO3_ITEM  364		// [364] slot
#define SLOT_GRENADE  365		// [365] slot
#define SLOT_SIXSHOOTER_ITEM  366		// [366] slot
#define SLOT_SIXSHOOTER_AMMO_ITEM  367		// [367] slot
#define SLOT_BIGMEDI_ITEM  368		// [368] slot
#define SLOT_SMALLMEDI_ITEM  369		// [369] slot
#define SLOT_LASERSIGHT_ITEM  370		// [370] slot
#define SLOT_BINOCULARS_ITEM  371		// [371] slot
#define SLOT_FLARE_ITEM  372		// [372] slot
#define SLOT_FLARE_INV_ITEM  373		// [373] slot
#define SLOT_DIARY_ITEM  374		// [374] slot
#define SLOT_COMPASS_ITEM  375		// [375] slot
#define SLOT_MEMCARD_LOAD_INV_ITEM  376		// [376] slot
#define SLOT_MEMCARD_SAVE_INV_ITEM  377		// [377] slot
#define SLOT_PC_LOAD_INV_ITEM  378		// [378] slot
#define SLOT_PC_SAVE_INV_ITEM  379		// [379] slot
#define SLOT_SMOKE_EMITTER_WHITE  380		// [380] slot
#define SLOT_SMOKE_EMITTER_BLACK  381		// [381] slot
#define SLOT_STEAM_EMITTER  382		// [382] slot
#define SLOT_EARTHQUAKE  383		// [383] slot
#define SLOT_BUBBLES  384		// [384] slot
#define SLOT_WATERFALLMIST  385		// [385] slot
#define SLOT_GUNSHELL  386		// [386] slot
#define SLOT_SHOTGUNSHELL  387		// [387] slot
#define SLOT_GUN_FLASH  388		// [388] slot
#define SLOT_BUTTERFLY  389		// [389] slot
#define SLOT_SPRINKLER  390		// [390] slot
#define SLOT_RED_LIGHT  391		// [391] slot
#define SLOT_GREEN_LIGHT  392		// [392] slot
#define SLOT_BLUE_LIGHT  393		// [393] slot
#define SLOT_AMBER_LIGHT  394		// [394] slot
#define SLOT_WHITE_LIGHT  395		// [395] slot
#define SLOT_BLINKING_LIGHT  396		// [396] slot
#define SLOT_LENS_FLARE  397		// [397] slot
#define SLOT_AI_GUARD  398		// [398] slot
#define SLOT_AI_AMBUSH  399		// [399] slot
#define SLOT_AI_PATROL1  400		// [400] slot
#define SLOT_AI_MODIFY  401		// [401] slot
#define SLOT_AI_FOLLOW  402		// [402] slot
#define SLOT_AI_PATROL2  403		// [403] slot
#define SLOT_AI_X1  404		// [404] slot
#define SLOT_AI_X2  405		// [405] slot
#define SLOT_LARA_START_POS  406		// [406] slot
#define SLOT_KILL_ALL_TRIGGERS  407		// [407] slot
#define SLOT_TRIGGER_TRIGGERER  408		// [408] slot
#define SLOT_SMASH_OBJECT1  409		// [409] slot
#define SLOT_SMASH_OBJECT2  410		// [410] slot
#define SLOT_SMASH_OBJECT3  411		// [411] slot
#define SLOT_SMASH_OBJECT4  412		// [412] slot
#define SLOT_SMASH_OBJECT5  413		// [413] slot
#define SLOT_SMASH_OBJECT6  414		// [414] slot
#define SLOT_SMASH_OBJECT7  415		// [415] slot
#define SLOT_SMASH_OBJECT8  416		// [416] slot
#define SLOT_MESHSWAP1  417		// [417] slot
#define SLOT_MESHSWAP2  418		// [418] slot
#define SLOT_MESHSWAP3  419		// [419] slot
#define SLOT_DEATH_SLIDE  420		// [420] slot
#define SLOT_BODY_PART  421		// [421] slot
#define SLOT_CAMERA_TARGET  422		// [422] slot
#define SLOT_WATERFALL1  423		// [423] slot
#define SLOT_WATERFALL2  424		// [424] slot
#define SLOT_WATERFALL3  425		// [425] slot
#define SLOT_PLANET_EFFECT  426		// [426] slot
#define SLOT_ANIMATING1  427		// [427] slot
#define SLOT_ANIMATING1_MIP  428		// [428] slot
#define SLOT_ANIMATING2  429		// [429] slot
#define SLOT_ANIMATING2_MIP  430		// [430] slot
#define SLOT_ANIMATING3  431		// [431] slot
#define SLOT_ANIMATING3_MIP  432		// [432] slot
#define SLOT_ANIMATING4  433		// [433] slot
#define SLOT_ANIMATING4_MIP  434		// [434] slot
#define SLOT_ANIMATING5  435		// [435] slot
#define SLOT_ANIMATING5_MIP  436		// [436] slot
#define SLOT_ANIMATING6  437		// [437] slot
#define SLOT_ANIMATING6_MIP  438		// [438] slot
#define SLOT_ANIMATING7  439		// [439] slot
#define SLOT_ANIMATING7_MIP  440		// [440] slot
#define SLOT_ANIMATING8  441		// [441] slot
#define SLOT_ANIMATING8_MIP  442		// [442] slot
#define SLOT_ANIMATING9  443		// [443] slot
#define SLOT_ANIMATING9_MIP  444		// [444] slot
#define SLOT_ANIMATING10  445		// [445] slot
#define SLOT_ANIMATING10_MIP  446		// [446] slot
#define SLOT_ANIMATING11  447		// [447] slot
#define SLOT_ANIMATING11_MIP  448		// [448] slot
#define SLOT_ANIMATING12  449		// [449] slot
#define SLOT_ANIMATING12_MIP  450		// [450] slot
#define SLOT_ANIMATING13  451		// [451] slot
#define SLOT_ANIMATING13_MIP  452		// [452] slot
#define SLOT_ANIMATING14  453		// [453] slot
#define SLOT_ANIMATING14_MIP  454		// [454] slot
#define SLOT_ANIMATING15  455		// [455] slot
#define SLOT_ANIMATING15_MIP  456		// [456] slot
#define SLOT_ANIMATING16  457		// [457] slot
#define SLOT_ANIMATING16_MIP  458		// [458] slot
#define SLOT_HORIZON  459		// [459] slot
#define SLOT_SKY_GRAPHICS  460		// [460] slot
#define SLOT_BINOCULAR_GRAPHICS  461		// [461] slot
#define SLOT_TARGET_GRAPHICS  462		// [462] slot
#define SLOT_DEFAULT_SPRITES  463		// [463] slot
#define SLOT_MISC_SPRITES  464		// [464] slot
#define SLOT_MOTOR_BOAT  465		// [465] slot
#define SLOT_MOTOR_BOAT_LARA  466		// [466] slot
#define SLOT_RUBBER_BOAT  467		// [467] slot
#define SLOT_RUBBER_BOAT_LARA  468		// [468] slot
#define SLOT_MOTORBIKE_LARA  469		// [469] slot
#define SLOT_FONT_GRAPHICS  470		// [470] slot
#define SLOT_PARALLEL_BARS  471		// [471] slot
#define SLOT_PANEL_BORDER  472		// [472] slot
#define SLOT_PANEL_MIDDLE  473		// [473] slot
#define SLOT_PANEL_CORNER  474		// [474] slot
#define SLOT_PANEL_DIAGONAL  475		// [475] slot
#define SLOT_PANEL_STRIP  476		// [476] slot
#define SLOT_PANEL_HALF_BORDER1  477		// [477] slot
#define SLOT_PANEL_HALF_BORDER2  478		// [478] slot
#define SLOT_PANEL_MIDDLE_CORNER  479		// [479] slot
#define SLOT_TIGHT_ROPE  480		// [480] slot
#define SLOT_LASER_HEAD  481		// [481] slot
#define SLOT_LASER_HEAD_BASE  482		// [482] slot
#define SLOT_LASER_HEAD_TENTACLE  483		// [483] slot
#define SLOT_HYDRA  484		// [484] slot
#define SLOT_HYDRA_MISSILE  485		// [485] slot
#define SLOT_ENEMY_SUB_MARINE  486		// [486] slot
#define SLOT_ENEMY_SUB_MARINE_MIP  487		// [487] slot
#define SLOT_SUB_MARINE_MISSILE  488		// [488] slot
#define SLOT_FROG_MAN  489		// [489] slot
#define SLOT_FROG_MAN_HARPOON  490		// [490] slot
#define SLOT_FISH_EMITTER  491		// [491] slot
#define SLOT_KAYAK  492		// [492] slot
#define SLOT_KAYAK_LARA  493		// [493] slot
#define SLOT_CUSTOM_SPRITES  494		// [494] slot
#define SLOT_BRIDGE_TILT3  495		// [495] slot
#define SLOT_BRIDGE_TILT4  496		// [496] slot
#define SLOT_BRIDGE_CUSTOM  497		// [497] slot
#define SLOT_ROBOT_CLEANER  498		// [498] slot
#define SLOT_ROBOT_STAR_WARS  499		// [499] slot
#define SLOT_MECH_WARRIOR  500		// [500] slot
#define SLOT_MECH_WARRIOR_LARA  501		// [501] slot
#define SLOT_UW_PROPULSOR  502		// [502] slot
#define SLOT_UW_PROPULSOR_LARA  503		// [503] slot
#define SLOT_MINE_CART  504		// [504] slot
#define SLOT_MINE_CART_LARA  505		// [505] slot
#define SLOT_NEW_SLOT5  506		// [506] slot
#define SLOT_NEW_SLOT6  507		// [507] slot
#define SLOT_NEW_SLOT7  508		// [508] slot
#define SLOT_NEW_SLOT8  509		// [509] slot
#define SLOT_NEW_SLOT9  510		// [510] slot
#define SLOT_NEW_SLOT10  511		// [511] slot
#define SLOT_NEW_SLOT11  512		// [512] slot
#define SLOT_NEW_SLOT12  513		// [513] slot
#define SLOT_NEW_SLOT13  514		// [514] slot
#define SLOT_NEW_SLOT14  515		// [515] slot
#define SLOT_NEW_SLOT15  516		// [516] slot
#define SLOT_NEW_SLOT16  517		// [517] slot
#define SLOT_NEW_SLOT17  518		// [518] slot
#define SLOT_NEW_SLOT18  519		// [519] slot
#define SLOT_NUMBER_OBJECTS  520		// [520] slot
// ----------- define for SSLOT_ trigger constants -------------
#define SSLOT_PLANT0  0		// [0] slot
#define SSLOT_PLANT1  1		// [1] slot
#define SSLOT_PLANT2  2		// [2] slot
#define SSLOT_PLANT3  3		// [3] slot
#define SSLOT_PLANT4  4		// [4] slot
#define SSLOT_PLANT5  5		// [5] slot
#define SSLOT_PLANT6  6		// [6] slot
#define SSLOT_PLANT7  7		// [7] slot
#define SSLOT_PLANT8  8		// [8] slot
#define SSLOT_PLANT9  9		// [9] slot
#define SSLOT_FURNITURE0  10		// [10] slot
#define SSLOT_FURNITURE1  11		// [11] slot
#define SSLOT_FURNITURE2  12		// [12] slot
#define SSLOT_FURNITURE3  13		// [13] slot
#define SSLOT_FURNITURE4  14		// [14] slot
#define SSLOT_FURNITURE5  15		// [15] slot
#define SSLOT_FURNITURE6  16		// [16] slot
#define SSLOT_FURNITURE7  17		// [17] slot
#define SSLOT_FURNITURE8  18		// [18] slot
#define SSLOT_FURNITURE9  19		// [19] slot
#define SSLOT_ROCK0  20		// [20] slot
#define SSLOT_ROCK1  21		// [21] slot
#define SSLOT_ROCK2  22		// [22] slot
#define SSLOT_ROCK3  23		// [23] slot
#define SSLOT_ROCK4  24		// [24] slot
#define SSLOT_ROCK5  25		// [25] slot
#define SSLOT_ROCK6  26		// [26] slot
#define SSLOT_ROCK7  27		// [27] slot
#define SSLOT_ROCK8  28		// [28] slot
#define SSLOT_ROCK9  29		// [29] slot
#define SSLOT_ARCHITECTURE0  30		// [30] slot
#define SSLOT_ARCHITECTURE1  31		// [31] slot
#define SSLOT_ARCHITECTURE2  32		// [32] slot
#define SSLOT_ARCHITECTURE3  33		// [33] slot
#define SSLOT_ARCHITECTURE4  34		// [34] slot
#define SSLOT_ARCHITECTURE5  35		// [35] slot
#define SSLOT_ARCHITECTURE6  36		// [36] slot
#define SSLOT_ARCHITECTURE7  37		// [37] slot
#define SSLOT_ARCHITECTURE8  38		// [38] slot
#define SSLOT_ARCHITECTURE9  39		// [39] slot
#define SSLOT_DEBRIS0  40		// [40] slot
#define SSLOT_DEBRIS1  41		// [41] slot
#define SSLOT_DEBRIS2  42		// [42] slot
#define SSLOT_DEBRIS3  43		// [43] slot
#define SSLOT_DEBRIS4  44		// [44] slot
#define SSLOT_DEBRIS5  45		// [45] slot
#define SSLOT_DEBRIS6  46		// [46] slot
#define SSLOT_DEBRIS7  47		// [47] slot
#define SSLOT_DEBRIS8  48		// [48] slot
#define SSLOT_DEBRIS9  49		// [49] slot
#define SSLOT_SHATTER0  50		// [50] slot
#define SSLOT_SHATTER1  51		// [51] slot
#define SSLOT_SHATTER2  52		// [52] slot
#define SSLOT_SHATTER3  53		// [53] slot
#define SSLOT_SHATTER4  54		// [54] slot
#define SSLOT_SHATTER5  55		// [55] slot
#define SSLOT_SHATTER6  56		// [56] slot
#define SSLOT_SHATTER7  57		// [57] slot
#define SSLOT_SHATTER8  58		// [58] slot
#define SSLOT_SHATTER9  59		// [59] slot
#define SSLOT_EXTRA00  60		// [60] slot
#define SSLOT_EXTRA01  61		// [61] slot
#define SSLOT_EXTRA02  62		// [62] slot
#define SSLOT_EXTRA03  63		// [63] slot
#define SSLOT_EXTRA04  64		// [64] slot
#define SSLOT_EXTRA05  65		// [65] slot
#define SSLOT_EXTRA06  66		// [66] slot
#define SSLOT_EXTRA07  67		// [67] slot
#define SSLOT_EXTRA08  68		// [68] slot
#define SSLOT_EXTRA09  69		// [69] slot
#define SSLOT_EXTRA10  70		// [70] slot
#define SSLOT_EXTRA11  71		// [71] slot
#define SSLOT_EXTRA12  72		// [72] slot
#define SSLOT_EXTRA13  73		// [73] slot
#define SSLOT_EXTRA14  74		// [74] slot
#define SSLOT_EXTRA15  75		// [75] slot
#define SSLOT_EXTRA16  76		// [76] slot
#define SSLOT_EXTRA17  77		// [77] slot
#define SSLOT_EXTRA18  78		// [78] slot
#define SSLOT_EXTRA19  79		// [79] slot
#define SSLOT_EXTRA20  80		// [80] slot
#define SSLOT_EXTRA21  81		// [81] slot
#define SSLOT_EXTRA22  82		// [82] slot
#define SSLOT_EXTRA23  83		// [83] slot
#define SSLOT_EXTRA24  84		// [84] slot
#define SSLOT_EXTRA25  85		// [85] slot
#define SSLOT_EXTRA26  86		// [86] slot
#define SSLOT_EXTRA27  87		// [87] slot
#define SSLOT_EXTRA28  88		// [88] slot
#define SSLOT_EXTRA29  89		// [89] slot
#define SSLOT_EXTRA30  90		// [90] slot
#define SSLOT_EXTRA31  91		// [91] slot
#define SSLOT_EXTRA32  92		// [92] slot
#define SSLOT_EXTRA33  93		// [93] slot
#define SSLOT_EXTRA34  94		// [94] slot
#define SSLOT_EXTRA35  95		// [95] slot
#define SSLOT_EXTRA36  96		// [96] slot
#define SSLOT_EXTRA37  97		// [97] slot
#define SSLOT_EXTRA38  98		// [98] slot
#define SSLOT_EXTRA39  99		// [99] slot
#define SSLOT_EXTRA40  100		// [100] slot
#define SSLOT_EXTRA41  101		// [101] slot
#define SSLOT_EXTRA42  102		// [102] slot
#define SSLOT_EXTRA43  103		// [103] slot
#define SSLOT_EXTRA44  104		// [104] slot
#define SSLOT_EXTRA45  105		// [105] slot
#define SSLOT_EXTRA46  106		// [106] slot
#define SSLOT_EXTRA47  107		// [107] slot
#define SSLOT_EXTRA48  108		// [108] slot
#define SSLOT_EXTRA49  109		// [109] slot
#define SSLOT_EXTRA50  110		// [110] slot
#define SSLOT_EXTRA51  111		// [111] slot
#define SSLOT_EXTRA52  112		// [112] slot
#define SSLOT_EXTRA53  113		// [113] slot
#define SSLOT_EXTRA54  114		// [114] slot
#define SSLOT_EXTRA55  115		// [115] slot
#define SSLOT_EXTRA56  116		// [116] slot
#define SSLOT_EXTRA57  117		// [117] slot
#define SSLOT_EXTRA58  118		// [118] slot
#define SSLOT_EXTRA59  119		// [119] slot
#define SSLOT_EXTRA60  120		// [120] slot
#define SSLOT_EXTRA61  121		// [121] slot
#define SSLOT_EXTRA62  122		// [122] slot
#define SSLOT_EXTRA63  123		// [123] slot
#define SSLOT_EXTRA64  124		// [124] slot
#define SSLOT_EXTRA65  125		// [125] slot
#define SSLOT_EXTRA66  126		// [126] slot
#define SSLOT_EXTRA67  127		// [127] slot
#define SSLOT_EXTRA68  128		// [128] slot
#define SSLOT_EXTRA69  129		// [129] slot
#define SSLOT_EXTRA70  130		// [130] slot
#define SSLOT_EXTRA71  131		// [131] slot
#define SSLOT_EXTRA72  132		// [132] slot
#define SSLOT_EXTRA73  133		// [133] slot
#define SSLOT_EXTRA74  134		// [134] slot
#define SSLOT_EXTRA75  135		// [135] slot
#define SSLOT_EXTRA76  136		// [136] slot
#define SSLOT_EXTRA77  137		// [137] slot
#define SSLOT_EXTRA78  138		// [138] slot
#define SSLOT_EXTRA79  139		// [139] slot
#define SSLOT_EXTRA80  140		// [140] slot
#define SSLOT_EXTRA81  141		// [141] slot
#define SSLOT_EXTRA82  142		// [142] slot
#define SSLOT_EXTRA83  143		// [143] slot
#define SSLOT_EXTRA84  144		// [144] slot
#define SSLOT_EXTRA85  145		// [145] slot
#define SSLOT_EXTRA86  146		// [146] slot
#define SSLOT_EXTRA87  147		// [147] slot
#define SSLOT_EXTRA88  148		// [148] slot
#define SSLOT_EXTRA89  149		// [149] slot
#define SSLOT_EXTRA90  150		// [150] slot
#define SSLOT_EXTRA91  151		// [151] slot
#define SSLOT_EXTRA92  152		// [152] slot
#define SSLOT_EXTRA93  153		// [153] slot
#define SSLOT_EXTRA94  154		// [154] slot
#define SSLOT_EXTRA95  155		// [155] slot
#define SSLOT_EXTRA96  156		// [156] slot
#define SSLOT_EXTRA97  157		// [157] slot
#define SSLOT_EXTRA98  158		// [158] slot
#define SSLOT_EXTRA99  159		// [159] slot
#define SSLOT_NUMBER_STATIC_OBJECTS  160		// [160] slot

// #TAG_END_AUTO_ENUM_STUFF#



