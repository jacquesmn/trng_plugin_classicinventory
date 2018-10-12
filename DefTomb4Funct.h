

// FOR_YOU: IMPORTANT WARNING: don't change or add text to this source because it will be
// replaced after new update and in this way you should lose your adding.
// Use the "Tomb4Discoveries_mine.h" file to add the tomb4 functions you discovered.

// don't remove following line: it's a way to keep memory about current release of 
// your DefTomb4Funct.h source
// #VERSION_UPDATE_INFO#=PU-4

// ============ DECLARATION OF TOMB4 FUNCTIONS  ==============================
// to include first of "functions.h" file, where below definitions will be
// used to assign to them the effective offset address


typedef void (__cdecl* TYPE_InviaMessaggioLog) (int Numero, char Messaggio[]);
typedef void (__cdecl* TYPE_WadeSplash) (StrItemTr4 *pItem, int WaterSurface, int WaterDepth);
typedef void (__cdecl* TYPE_pcMostraMessaggio) (char *pMessaggio);
typedef void (__cdecl* TYPE_tombInviaLog) (int Numero, char *pMex);
typedef int (__cdecl* TYPE_tombCicloPrincipale) (int Arg1, int Arg2);
typedef void (__cdecl* TYPE_tombRilocaCambiaStanzaDiCamera) (int NRoom);
typedef void (__cdecl* TYPE_GestioneTitleEMenu) (int NumeroLivello, int NumeroCD);
typedef int (__cdecl* TYPE_MuoviTelecamera) (void *pCamera, int TipoCamera);
typedef int (__cdecl* TYPE_ControlSideCar) (short ItemIndex);
typedef int (__cdecl* TYPE_ControlJeep) (short ItemIndex);
typedef void (__cdecl* TYPE_LaraBurn) (void);
typedef void (__cdecl* TYPE_PrintString) (int OrgX, int OrgY, WORD Colore, char *pTesto, WORD Flags);
typedef void (__cdecl* TYPE_S_CDPlay) (int NumeroCd, int TestLoop);
typedef void (__cdecl* TYPE_SoundEffect) (int NumeroSample, void *pCoordinate, int Flags);
typedef void (__cdecl* TYPE_StopEffect) (int NumeroSample);
typedef bool (__cdecl* TYPE_IsChannelPlaying) (int NumeroSample);
typedef void (__cdecl* TYPE_StopAllSounds) (void);
typedef int (__cdecl* TYPE_DoBar) (int CordX, int CordY, int SizeX, int SizeY, int Percentuale, DWORD Colore, DWORD ColoreFade);
typedef void (__cdecl * TYPE_TriggerExplosionSparks) (DWORD CordX, int CordY, DWORD CordZ, int Val1, int Val2, int Val3, int Room);
typedef void (__cdecl * TYPE_TriggerUnderwaterExplosion) (StrItemTr4 *pItem, bool Test);
typedef void (__cdecl * TYPE_KillItem) (WORD ItemIndex);
typedef void (__cdecl * TYPE_CreatureDie) (int ItemIndex, bool TestEsplosione);
typedef void (__cdecl * TYPE_AlterFloorHeight) (StrItemTr4 *pItem, int VariazioneY);
typedef void* (__cdecl * TYPE_GetFloor) (DWORD CordX, int CordY, DWORD CordZ, short* pRoom);
typedef int (__cdecl * TYPE_GetHeight) (void *pFloor, DWORD CordX, int CordY, DWORD CordZ);
typedef void (__cdecl * TYPE_DrawThreeDeeObject2D) (int CordX, int CordY, int IndiceInventario, int Flag, int arg1, int arg2, int arg3, int arg4, int arg5);
typedef int (__cdecl * TYPE_convert_obj_to_invobj) (short SlotId);
typedef void (__cdecl *TYPE_ScaleCurrentMatrix) (StrTripla *pTripla);
typedef bool (__cdecl *TYPE_CreatureActive) (short Indice);
typedef void (__cdecl *TYPE_SetScreenFadeIn) (WORD Valore);
typedef void (__cdecl *TYPE_SetScreenFadeOut) (WORD Incremento, int Colore);
typedef void (__cdecl *TYPE_AlterFOV) (WORD ValoreFOV);
typedef void (__cdecl *TYPE_SetCutSceneCamera) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_AddActiveItem) (short Indice);
typedef void (__cdecl *TYPE_ItemNewRoom) (int ItemIndex, WORD NewRoom);
typedef void (__cdecl *TYPE_DrawFlash) (void);
typedef int (__cdecl *TYPE_WinFrameRate) (void);
typedef void (__cdecl *TYPE_DoSlider) (int OrgX, int OrgY, int SizeX, int SizeY, int Volume, DWORD BackColor, DWORD MainColor, DWORD MaskColor);
typedef void (__cdecl *TYPE_SoundStopAllSamples) (void);
typedef bool (__cdecl *TYPE_OnTwoBlockPlatform) (StrItemTr4 *pItem, DWORD CordX, DWORD CordZ);
typedef int (__cdecl *TYPE_GetCeiling) (void *pFloor, DWORD CordX, int CordY, DWORD CordZ);
typedef void (__cdecl *TYPE_DoLotsOfBlood) (DWORD CordX, int CordY, DWORD CordZ, 
						int Casuale, WORD Orientamento, WORD Room, int Intensita);
typedef void (__cdecl *TYPE_DoBloodSplat) (DWORD CordX, int CordY, DWORD CordZ, 			   
							int Casuale, WORD Orientamento, WORD Room);
typedef int (__cdecl *TYPE_GetRandomControl) (void);
typedef bool (__cdecl *TYPE_S_SoundSampleIsPlaying) (WORD Canale);
typedef void (__cdecl *TYPE_GetJointAbsPosition) (StrItemTr4 *pItem, 
												  StrMovePosition *pCoordinate, int JointIndex);
typedef void (__cdecl *TYPE_TriggerGunSmoke) (DWORD CordX, int CordY, DWORD CordZ, 
				int Boh1, int Boh2, int Boh3, int Boh4, int Colore, int Durata);
typedef void (__cdecl *TYPE_AddFire) (DWORD CordX, int CordY, 
							DWORD CordZ, int Boh, WORD NRoom,int Boh2); 
typedef void (__cdecl *TYPE_TriggerExhaustSmoke) (DWORD CordX, int CordY, 
								DWORD CordZ, WORD Orientamento, int Intensita, int Tipo);

typedef bool (__cdecl *TYPE_TriggerActive) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_phd_RotY) (WORD Orient);
typedef void (__cdecl *TYPE_Draw2DSprite) (int CordX, int CordY, int IndiceSprite, DWORD Colore, int Mistero);
typedef void (__cdecl *TYPE_AddDisplayPickup) (int SlotItem);
typedef void (__cdecl *TYPE_KillEffect) (WORD ItemIndex);
typedef bool (__cdecl *TYPE_CanLaraHangSideways) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara, int OrientMod);
typedef int (__cdecl *TYPE_LaraHangLeftCornerTest) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef int (__cdecl *TYPE_LaraHangRightCornerTest) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef int (__cdecl *TYPE_LaraClimbLeftCornerTest) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef int (__cdecl *TYPE_LaraClimbRightCornerTest) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef void (__cdecl *TYPE_ItemPushLara) (StrItemTr4 *pItem, StrItemTr4 *pLara, StrCollisionLara *pCollLara, bool TestChangeLaraAnim, int Parameter);
typedef bool (__cdecl *TYPE_LaraCheckForLetGo) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef void (__cdecl *TYPE_lara_col_climbright) (StrItemTr4 *pOggetto, StrCollisionLara *pCollLara);
typedef bool (__cdecl *TYPE_LaraTestClimbPos) (StrItemTr4* pOggetto, int MinY, int MaxY, int MinX, int MaxX, int *pValore); 
typedef void (__cdecl *TYPE_S_SaveGame) (int NumeroSave);
typedef void (__cdecl *TYPE_S_LoadGame) (int NumeroSave);
typedef void (__cdecl *TYPE_sgSaveGame) (void);
typedef void (__cdecl *TYPE_S_InitialisePolyList) (void);
typedef void (__cdecl *TYPE_S_OutputPolyList) (void);
typedef void (__cdecl *TYPE_TIME_Init) (void);
typedef void (__cdecl *TYPE_BikeExplode) (StrItemTr4 *pBike);
typedef StrBoxCollisione * (__cdecl *TYPE_GetBestFrame) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_TestTriggers) (void *pFloorDataNow, int TestHeavy, int TestScale);
typedef int (__cdecl *TYPE_GetWaterHeight) (DWORD CordX, int CordY, DWORD CordZ, short Room);
typedef void (__cdecl *TYPE_Splash) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_BikeBaddieCollision) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_BikeCollideStaticObjects) (DWORD CordX, int CordY, DWORD CordZ, short Room, int Distanza);
typedef bool (__cdecl *TYPE_TestBlockPush) (StrItemTr4 *pItem, int Altezza, short Orientamento);
typedef int (__cdecl *TYPE_GetCollidedObjects) (StrItemTr4 *pItem, int GapY, bool TestCltrCollAttive, 
							StrItemTr4 *pVetItemTrovati[],  
							StrMeshInfo* pVetStaticTrovati, 
							bool TestIgnoraLara);

typedef bool (__cdecl *TYPE_TestBoundCollide) (StrItemTr4 *pPrimoItem, 
									StrItemTr4 *pSecondoItem, int Distanza);
typedef void (__cdecl *TYPE_RefreshCamera) (WORD Pulsanti, WORD *pVettoreTrigger);
typedef void (__cdecl *TYPE_InitialiseSpotCam) (WORD IndiceFlyBy);
typedef void (__cdecl *TYPE_FlipMap) (int Indice);
typedef void (__cdecl *TYPE_S_CDStop) (void);
typedef void (__cdecl *TYPE_S_UpdateInput) (void);
typedef void (__cdecl *TYPE_ShatterObject) (void *pShatterItem, 
				StrMeshInfo* pStaticMesh, WORD Costante80, WORD Room, int Mistero);
typedef void (__cdecl *TYPE_ExplosionFX) (void);
typedef void (__cdecl *TYPE_SetFogColor) (int Rosso, int Verde, int Blu);
typedef void (__cdecl *TYPE_GetFlameTorch) (void);
typedef void (__cdecl *TYPE_freeTr4) (void *pMemory);
typedef int  (__cdecl *TYPE_GetStringLength) (char *pTesto, short *pSizeX, short *pSizeY);
typedef StrBoxCollisione * (__cdecl *TYPE_GetBoundsAccurate) (StrItemTr4 *pItem);
typedef bool (__cdecl *TYPE_TestLaraPosition) (StrTestPosition *pPosition, StrItemTr4 *pItem, StrItemTr4 *pLara);
typedef bool (__cdecl *TYPE_MoveLaraPosition) (StrMovePosition *pMovPos, StrItemTr4 *pItem, StrItemTr4* pLara);
typedef void (__cdecl *TYPE_SetMapRoom) (void);
typedef void (__cdecl *TYPE_InitialiseCamera) (void);
typedef void (__cdecl *TYPE_AnimateItem) (StrItemTr4 *pOggetto);
typedef void (__cdecl *TYPE_KillActiveBaddies) (StrItemTr4 *pOggetto);
typedef void (__cdecl *TYPE_DoFlareLight) (StrMovePosition *pPosition, int Tempo);
typedef void (__cdecl *TYPE_UpdateLaraRoom) (StrItemTr4 *pLara, int ChangeY);
typedef void (__cdecl *TYPE_IsRoomOutside) (DWORD CordX, int CordY, DWORD CordZ);
typedef void (__cdecl *TYPE_FreeMonoScreen) (void);
typedef void (__cdecl *TYPE_CreateMonoScreen) (void);
typedef void (__cdecl *TYPE_S_DumpScreen) (void);
typedef bool (__cdecl *TYPE_IsComandoPremuto) (int Index);
typedef void (__cdecl *TYPE_SOUND_EndScene) (void);
typedef bool (__cdecl *TYPE_have_i_got_object) (int SlotNumber);
typedef void (__cdecl *TYPE_TriggerDynamic) (int OrgX, int OrgY, int OrgZ, int Intensity, BYTE Red, BYTE Green, BYTE Blue);
typedef void (__cdecl *TYPE_LaraTorch) (StrMovePosition *pStartPoint, StrMovePosition *pEndPoint, WORD Orient, int Intensita);
typedef void (__cdecl *TYPE_StopSoundEffect) (short SoundEffect);
typedef void (__cdecl *TYPE_TriggerSuperJetFlame) (StrItemTr4 *pItem, int Lunghezza, int TestQualcosa);
typedef bool (__cdecl *TYPE_ItemNearLara) (void *pTripletta, int DistanzaMax);
typedef void (__cdecl *TYPE_RemoveActiveItem) (short ItemIndex);
typedef void (__cdecl *TYPE_TriggerFlareSparks) (int CordX, int CordY, int CordZ, 
								int Red, int Green, int Blue,
								DWORD TestSmoke, DWORD Unused);
typedef void (__cdecl *TYPE_TriggerLightningGlow) (DWORD CordX, int CordY, DWORD CordZ, DWORD Colore);
typedef void (__cdecl *TYPE_SetD3DViewMatrix) (void);
typedef void (__cdecl *TYPE_phd_PushMatrix) (void);
typedef void (__cdecl *TYPE_phd_TranslateAbs) (DWORD x, int y, DWORD z);
typedef void (__cdecl *TYPE_phd_RotYXZ) (short HOrient, short VOrient, short ROrient);
	// questa funzione in pratica scrive una singola mesh:
typedef void (__cdecl *TYPE_phd_PutPolygons_train) (StrMeshTr4 *pMesh, int SpostamentoSuX);
typedef int (__cdecl *TYPE_GetFreeScarab) (void);
typedef void (__cdecl *TYPE_DisableBaddieAI) (short EnemyIndex);
typedef BOOL (__cdecl *TYPE_EnableBaddieAI) (short EnemyIndex, BOOL TestInitSlot);
typedef void (__cdecl *TYPE_ACMSetVolume) (void);
typedef int (__cdecl *TYPE_LOS) (GAME_VECTOR *pPunto1, GAME_VECTOR *pPunto2);
typedef int (__cdecl *TYPE_mGetAngle) (DWORD SrcX, DWORD SrcZ, DWORD DestX, DWORD DestZ);
typedef void(__cdecl *TYPE_TriggerSmallSplash) (DWORD SrcX, int SrcY, DWORD SrcZ, int Size);
typedef void (__cdecl *TYPE_CreateBubble) (StrPosizione *pPos, WORD Room, int Intensita1, int Intensita2);
typedef void (__cdecl *TYPE_SetupRipple) (DWORD CordX, int CordY, DWORD CordZ, int Intensity, int Quantity);
typedef void (__cdecl *TYPE_insert_object_into_list_v2) (int ItemIndex);
typedef int (__cdecl *TYPE_GetWaterDepth) (DWORD CordX, int CordY, DWORD CordZ, short Room);
typedef int* (__cdecl *TYPE_S_GetObjectBounds) (StrBoxCollisione *pBox);
typedef void (__cdecl *TYPE_EffettoShockwaveBaboon) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_ControlMineHelicopter) (int ItemIndex, void *pColl);
typedef void (__cdecl *TYPE_DoHarpyEffects) (StrItemTr4 *pItem, int Indice);
typedef void (__cdecl *TYPE_TriggerRiseEffect) (StrItemTr4 *pItem);
typedef void (__cdecl *TYPE_DrawSprite) (int OrgX, int OrgY, int IndiceSprite, DWORD Colore, DWORD Size, DWORD Sequenza);
typedef void (__cdecl *TYPE_TriggerLightning) (StrPosizione *pSrc, StrPosizione *pTarget, BYTE Intensita, DWORD ColorePiuDurataLuce, int Arg1, int Size, int Arg3);
typedef int (__cdecl *TYPE_SubBridgeTilt) (StrItemTr4 *pBridge, DWORD CordX, DWORD CordZ);
typedef WORD (__cdecl *TYPE_GetTiltType) (void *pFloor, DWORD CordX, int CordY, DWORD CordZ);
typedef DWORD (__cdecl *TYPE_TestCollision) (StrItemTr4 *pItemDetails, StrItemTr4 *pItem);
typedef int (__cdecl *TYPE_ObjectOnLOS2) (GAME_VECTOR *pSource, GAME_VECTOR * pTarget, 
										  StrPosizione *pPosizione, StrMeshInfo **pFoundStatic);
typedef void (__cdecl *TYPE_SetFade) (int FirstDark, int LastDark);


//---------------- CALLBACKS PROTOTYPES ------------------------
// description of prototypes for callbacks used in C++ to create 
// the (your) function that will receive the control from trng, 
// or with the asm procedure, to remember 
// the arguments and their order (you can read the arguments in asm using 
// the MoveArg() macro) when your asm procedure will be performed from trng


// for CB_FLIPEFFECT callback
// ActivationMode has SCANF_.. flags to inform about the source of the trigger (floor data in level map, triggergroup, 
// direct call from code) 
// CBType remember to you the kind of callback it was (CBT_FIRST, CBT_AFTER, CBT_REPLACE) in this way you could use
// same callback procedure to handle both types of all triggers callbacks
// return TRET_ mask values
typedef int (__cdecl *CALL_FLIPEFFECT) (WORD FlipIndex, WORD Timer, WORD Extra, WORD ActivationMode, WORD CBType);


// for CB_FLIPEFFECT_MINE callback:
// ActivationMode has SCANF_.. flags to inform about the source of the trigger (floor data in level map, triggergroup, 
// direct call from code) 
// return TRET_ mask values
typedef int (__cdecl *CALL_FLIPEFFECT_MINE) (WORD FlipIndex, WORD Timer, WORD Extra, WORD ActivationMode);



// for CB_ACTION callback
// CBType remember to you the kind of callback it was (CBT_FIRST, CBT_AFTER, CBT_REPLACE) in this way you could use
// same callback procedure to handle both types of all triggers callbacks
// return TRET_ mask values
typedef int (__cdecl *CALL_ACTION) (WORD ActionIndex, int ItemIndex, WORD Extra, WORD ActivationMode, WORD CBType);

// for CB_ACTION_MINE callback
// return TRET_ mask values
typedef int (__cdecl *CALL_ACTION_MINE) (WORD ActionIndex, int ItemIndex, WORD Extra, WORD ActivationMode);


// for CB_CONDITION callback
// note: the Response field works only for CB_CONDITION with CBT_AFTER, where your code will receive
// in Response the previous response returned by original trng code.
// CBType remember to you the kind of callback it was (CBT_FIRST, CBT_AFTER, CBT_REPLACE) in this way you could use
// same callback procedure to handle both types of all triggers callbacks
// the callback has to return a mask with CTRET_ constant values
typedef int (__cdecl *CALL_CONDITION) (WORD ConditionIndex, int ItemIndex, WORD Extra, WORD ActivationMode, bool Response, WORD CBType);


// for CB_CONDITION_MINE callback
// the callback has to return a mask with CTRET_ constant values
typedef int (__cdecl *CALL_CONDITION_MINE) (WORD ConditionIndex, int ItemIndex, WORD Extra, WORD ActivationMode);



// for callback CB_CUSTOMIZE
typedef void (__cdecl *CALL_CUSTOMIZE) (WORD CustomizeValue, int NumberOfItems, short *pItemArray);

// for callback CB_PARAMETER
typedef void (__cdecl *CALL_PARAMETER) (WORD ParameterValue, int NumberOfItems, short *pItemArray);
 
// for callback CB_ASSIGN_SLOT_MINE
typedef void (__cdecl *CALL_ASSIGN_SLOT) (WORD Slot, WORD OBJ_TYPE);


// for callback CB_INIT_PROGRAM  
typedef void (__cdecl *CALL_INIT_PROGRAM) (int NumberLoadedPlugins, char *VetPluginNames[]);

// for callback CB_SAVING_GAME
// SavingType  0= local  / 1=global
typedef DWORD (__cdecl *CALL_SAVING_GAME) (BYTE **pAdrZone, int SavingType);

// for callback CB_LOADING_GAME
typedef void (__cdecl *CALL_LOADING_GAME) (BYTE *pAdrZone, DWORD SizeData);

// for generic void / void callback procedure
typedef void (__cdecl *CALL_VOID) (void);

// for cbCycleEnd
typedef int (__cdecl *CALL_CYCLE_END) (void);

// for cbGlobalTrigger
// it has to return a RGT_ value  (Return for Global Trigger)
typedef int (__cdecl *CALL_GLOBAL_TRIGGER) (int GT_Value, StrGlobalTrigger *pGlobalTrigger);


// prototype for CB_COMPLETED_PROGR_ACTION callback
// note: ActionType will be a AZ_... value to identify a trng progressive action
// note: pAction will point to progressive action structure with all data, exception the field Action
// that will be 0, but the previous value (AZ_) will be store in ActionType argument
typedef void (__cdecl *CALL_COMPLETED_PROGR_ACTION) (int ActionType, StrProgressiveAction *pAction);


// prototype for CB_VEHICLE callback
// if your callback found one vehicle of yours, you have to return "true" and trng will perform no other code
// for current vehicle.
// while if current vehicle it's not yours, you have to return "false" to let working the default code for that vehicle
typedef bool (__cdecl *CALL_VEHICLE) (short IndexItem);

// prototype for CB_SLOT_INIT , CB_SLOT_CONTROL or CB_SLOT_DRAW or CB_SLOT_DRAW_EXTRA
// receive in input:
// IndexItem : the index of object is calling its InitialiseObject or ControlObject procedure
// pItem     : the structure of above moveable
// CBT_Flags : the CBT_ flag of this callback (CBT_FIRST, CBT_AFTER or CBT_REPLACE)
// return a SRET_ constant value
typedef int (__cdecl *CALL_SLOT_MANY) (short IndexItem, StrItemTr4 *pItem, WORD CBT_Flags);

// prototype for CB_SLOT_COLLISION
// receive in input:
// IndexItem : the index of object is calling its collision procedure
// pItem     : the structure of above moveable
// CBT_Flags : the CBT_ flag of this callback (CBT_FIRST , CBT_AFTER or CBT_REPLACE)
// pCollision : pointer to StrCollisionLara structure (about lara collision and other infos)
// return a SRET_ constant value
typedef int (__cdecl *CALL_SLOT_CB_COLLISION) (short IndexItem, StrItemTr4 *pItem, WORD CBT_Flags, StrCollisionLara * pCollision);

// prototype for CB_SLOT_FLOOR
// receive in input:
// CBT_Flags   : CBT_ value of type of callback: (CBT_FIRST , CBT_AFTER or CBT_REPLACE)
// pItem       : the structure of moveable (triggered with dummy) owner of current floor procedure
// CordX, CordY, CordZ : current posizione to verify where checking collision (usually current lara's position)
// *pNewCordY   : pointer where to type new CordY if virtual floor stopped lara  (example:  *pNewCordy = NewValue;
// note: if your (replace) procedure wish enable collisions with virtual floor (the topy coordinate of object)
//       you should write in *pNewCordY the new coordinate for Lara and also you have to set the global variables:
//       Trng.pGlobTomb4->pAdr->pTestOverPlatform  = TRUE 
//       Trng.pGlobTomb4->pAdr->pInclinationType = enumSLOPE...(some slope value for virtual floor)
// return a SRET_ constant value
typedef int (__cdecl *CALL_SLOT_CB_FLOOR) (WORD CBT_Flags, StrItemTr4 *pItem, DWORD CordX, int CordY, DWORD CordZ, int *pNewCordY);
 
// prototype for CB_SLOT_CEILING
// receive in input:
// CBT_Flags   : CBT_ value of type of callback: (CBT_FIRST , CBT_AFTER or CBT_REPLACE)
// pItem       : the structure of moveable (triggered with dummy) owner of current ceiling procedure
// CordX, CordY, CordZ : current posizione to verify where checking collision (usually current lara's position)
// *pNewCordY   : pointer where to type new CordY of virtual ceiling;
// return a SRET_ constant value
typedef int (__cdecl *CALL_SLOT_CB_CEILING) (WORD CBT_Flags, StrItemTr4 *pItem, DWORD CordX, int CordY, DWORD CordZ, int *pNewCordY);
 
// prototype for CB_NUMERIC_TRNG_PATCH 
// note: this prototype is usefull only when is missing the CBT_ASM flag. 
//       While, if it has CBT_ASM flag, there is no function c++ to call but only an address where to jump
// receive in input:
// PatchIndex:  the index of trng patch you are elaborating
// CBT_Flags: the type of current callback (CBT_FIRST,	CBT_AFTER or CBT_REPLACE)
// TestFromJump : if this trng patch has been executed with a "jmp call dword [TrngPatcher]" TestFromJump == true
// pRegisters:  pointer to strutcture with all current values of registers. You can read or change these values
// return: callback code can return a valid tomb4 code offset to jump directly to it, avoiding 
//         the coming back to address follwing the caller code in tomb4 (ONLY FOR CBT_REPLACE callback) 
//         or NULL if you wish letting the standard coming back after caller code in tomb4 
typedef void* (__cdecl *CALL_CB_NUMERIC_TRNG_PATCH) (WORD PatchIndex, WORD CBT_Flags, bool TestFromJump, StrRegisters *pRegisters);


// prototype for CB_FMV_PLAY callback
// receive in input:
// pMovieFullPath: the path of fmv to play in long format (including drive letter and current trle folder)
// TestEscape: if == true, it means your player should check for Escape key and break the playing if it occurs
// return RFMV_ values
// note: when your callback will be executed the fade-in will be already completed (black screen) and 
//		 dumpscene of tomb4 blocked
//      if the game was working in exclusive vide mode, trng will swap to cooperative level (disabling 
//		exclusive video mode) , will perform your callback, and then it will restore newly original video mode
typedef int (__cdecl *CALL_CB_FMV_PLAY) (char *pMovieFullPath, bool TestEscape);

// prototype for CB_STATE_ID_LARA_COLLISION and CB_STATE_ID_LARA_CTRL callbacks
// receive in input:
// StateId : the current StateId of this callback
// CBT_Type : enumCBT. flags of this callback (CBT_FIRST, CBT_AFTER, CBT_REPLACE)
// pLara : StrItemTr4 pointer to lara structure
// pCollision: pointer to collision info about lara
// return: enumSRET_ constant
typedef int (__cdecl *CALL_CB_STATE_ID) (WORD StateId, WORD CBT_Type, StrItemTr4 *pLara, StrCollisionLara * pCollision);

// prototype for CB_VEHICLE_CONTROL callback (used for those vehicle that ignore ControlProcedure of slot
//						like Jeep, bike and kayak)
// receive in input:
// CBT_Flags : type of callback executed: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// VehicleIndex: index of vehicle. For instance the itemidex of jeep or bike object
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_VEHICLE_CONTROL) (WORD CBT_Flags, int VehicleIndex);

// prototype for CB_LARA_CONTROL
// receive in input:
// CBT_Flags : flags of current callback CBT_FIRST, CBT_AFTER or CBT_REPLACE
// pLara : structure of lara
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_LARA_CTRL) (WORD CBT_Flags, StrItemTr4 *pLara);

// prototype for CB_LARA_DRAW callback
// receive in input:
// CBT_Flags : flag of current callback: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// pLara     : Pointer to Lara's structure
// TestNoUpdateLight:  if is == true, the light of lara will be not updated. If == false, the light will be
//						calculated newly
// TestMirror : if == true, the lara to draw is that in the mirror
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_LARA_DRAW) (WORD CBT_Flags, StrItemTr4 * pLara, bool TestNoUpdateLight, bool TestMirror);

// prototype for CB_LARA_HAIR_DRAW
// receive the CBT_ flag for kind of callback (CBT_FIRST, CBT_AFTER or CBT_REPLACE)
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_HAIR_DRAW) (WORD CBT_Flags);

// prototype for CB_LARA_HAIR_CONTROL
// receive in input:
// CBT_Flags: flag of current callback: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// TestKeepDownHair: if == true, the ponytail will point down (not sure about this)
// TestSecondTail: if == true, lara is young lara and there is to handle the second tail. If == false we are working of first, or only one, ponytail
// pData: pointer to some data to manange hair. I don't know the meaning of this data. Sometimes it is NULL (0) to
//		  mean : no data this time.
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_HAIR_CONTROL) (WORD CBT_Flags, bool TestKeepDownHair, bool TestSecondTail, void *pData);


// prototype for CB_INVENTORY_MAIN callback
// receive in input:
// CBT_Flags: flag of current callback: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// TestLoadedGame: valid only for CBT_AFTER callback: it has been loaded a savegame from last inventory session
//  SelectedItem: the slot of item chosen by inventory (like a key, puzzleitem ect) 
//					note: this parameter has a valid value only in CBT_AFTER callback
//						  if its value ==  -1, then it has not been chosen any item
// return a IRET_ constant
typedef int (__cdecl *CALL_INVENTORY_MAIN) (WORD CBT_Flags, bool TestLoadedGame, int SelectedItem);

// prototype for CB_INVENT_BACKGROUND_CREATE, CB_INVENT_BACKGROUND_DRAW, CB_INVENT_BACKGROUND_QUIT
// receive in input:
// CB_Type: the CB_.. constant for callback type
// CBT_Flags: flag of current callback: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// PHASE_Type: enumPHASE about inventory, pause screen, load game or savegame panel
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_INVENTORY_BACKGROUND) (WORD CB_Type, WORD CBT_Flags, WORD PHASE_Type);

// prototype for CB_SAVEGAME_MANAGER callback
// receive in input:
// TestLoad: if == true callback has to work to load a savegame, if == false, to save current game in some savegame slot
// TestTitle: if == true call back it has been called from title level
// returns: number of savegame (its numerical extension) to load, or to save it
//        return -1 when no saving/loading it has been performed (aborted operation, or "go on" message
//					 in title management)
//        return CLASSIC_MANAGEMENT, when you wish let to original procedure the management of savegame
//				It used, when you want check some data (from script or other) and change idea (in run-time phase) 
//				about the required managment of savegame.
typedef int (__cdecl *CALL_SAVEGAME_MANAGER) (bool TestLoad, bool TestTitle);

// prototype for CB_INPUT_MANAGER callback
// received input:
// VetInputKeyboard[255] = array of 256 byte items with current down (or less) key 
// pInputGameCommands = pointer to current input commands (enumCMD.  constants)
// pInputCommandFlags = pointer to current input commands (enumCMD.  constants)
// pInputExtGameCommands = pointer to current input commands (enumCMD.  constants)
// return SRET constant
// notes: 
//        1) it receives in input all pointers (and an array, VetInputKeyboard). To access to pointer you have 
//           to prefix them with "*" operator
//        2) VetInputKeyboard[]  access to all keys. You use, as index for this array, the same code of 
//           "Keyboard scancodes list", in Reference panel of NG_Center program. If value = 0, key is not selected  
//        3) The three pointers with enumCMD commands are used in different side of tomb4 but they are,
//            very often, the same. Only exception for pInputExtGameCommands that hosts also extended commands
//            missing in other flag dword. When you wish filter (remove) some command, you should
//            clear right value in VetInputKeyboard[] and in all three flag masks.
//        4) On read, the more valid flag dword to test is: "pInputExtGameCommands" 
//        5) In spite of "manager" definition, by default, the CB_INPUT_MANAGER callback don't remove the original
//            trng/tomb4 management, at least you didn't return with SRET_SKIP_ORIGINAL (skip tomb4 managment)
//            or with SRET_SKIP_TRNG_CODE value (skip trng management) Usually you'll return these "skip"
//			values only in seldom cirucstances to remove all kinds of input
typedef bool (__cdecl *CALL_INPUT_MANAGER) (BYTE VetInputKeyboard[], WORD *pInputGameCommands, 
											DWORD *pInputCommandFlags, DWORD *pInputExtGameCommands);

// prototype for CB_ANIMATE_LARA callback
// received input:
// CBT_Flags: flag of current callback: CBT_FIRST, CBT_AFTER or CBT_REPLACE
// pLara: pointer to Lara's structure
// return a SRET_ constant (read only for CBT_FIRST callback)
typedef int (__cdecl *CALL_ANIMATE_LARA) (WORD CBT_Flags, StrItemTr4 *pLara);

// prototype for CB_PAUSE_MANAGER callback
// no input argument for this callback procedure
// return PRET_ value
typedef int (__cdecl *CALL_PAUSE_MANAGER) (void);

// prototype for CB_STATISTICS_MANAGER 
// received input: no arguments
// returns: true if callback handled the target, while false if callback want that original code was performed
typedef bool (__cdecl *CALL_STAT_MANAGER) (void);

// prototype for CB_OPTIONS_MANAGER callbacks
// received input:
// TestTitle:  true when executed from title level
// TestCommands: true when options are working on keyboard/joystick commands
// SelectedRow: number of selected (and interactive) row. Starting from 1 (that at top of the screen)
// returns: SRET constant (valid only for CBT_LOOPED callback)
// notes: this callback accepts two kinds of flags: CBT_REPLACE and CBT_LOOPED
//		the CBT_LOOPED is very alike than CBT_FIRST but it will be peformed in progress, contiuosly, 
//      while option screen is running

typedef int (__cdecl *CALL_OPTIONS_MANAGER) (bool TestTitle, bool TestCommands, int SelectedRow);

// prototype for CB_TITLE_MENU_MANAGER callback
// received in input:
// PHASE_Type: enumPHASE value describing current phase in title level
// pInputFlags: pointer to flag mask with input commands. You can read them and/or change/remove them
// returns: 0= no operation /  negative values: number of savegame to load / 
//          Positive (non-zero) values: number of level to load
typedef int (__cdecl *CALL_TITLE_MANAGER) (WORD PHASE_Type, DWORD * pInputFlags);

// prototype for CB_WINDOWS_FONT_CREATE callback
// received in input: 
// pFont: pointer to StrWindowsFont structure with many data about feature of required font (gotten from script.dat)
// TestFixWideScreen: if == true it requires to resize wide of font to compensate widescreen distorsion
// return an handle of created font, or NULL if callback wants let to original code the creation of this font
// note: This callback has been imagined for who wishes fix some problem with view of eastern charset fonts
//       probably setting right font you coudl fix that problem with china fonts
typedef HFONT (__cdecl *CALL_WINDOWS_FONT_CREATE) (StrWindowsFont *pFont, bool TestFixWideScreen);

// prototype for CB_WINDOWS_UNICODE_CONVERT callback
// received in input:
// pFont : pointer to StrWindowsFont structure with many settings (about font) specified in script.dat
//         you should use these infos only on-read, to understand better the kind of characters you are
//         going to convert but don't change data of StrWindowsFont structure
// pText : pointer to byte based string of text to convert. You should read with debugger the format
//         of this text to understand better further bugs to fix and then convert it in unicode format
//         returning the pointer to wchar_t buffer with converted text to unicode
// note: the pointer you returns has to be in persistent memory of course, (no local stack memory of your
//       function
// note:  if you return NULL it means you want letting to original code the conversion
typedef wchar_t* (__cdecl * CALL_WINDOWS_UNICODE_CONVERT) (StrWindowsFont *pFont, char *pText);

// prototype for CB_WINDOWS_TEXT_PRINT callback
// received in input:
// pRect : pointer to RECT structure with coordinate of box where to print the text
// pFont : pointer to StrWindowsFont structure with many settings (about font) specified in script.dat
// hDC : handle of Device Context where to print text (usually tomb raider hdc or a memory DC)
// pText: pointer to text to print
// pTestError: pointer to bool variable to set "true" if it happned an error, otherwise set to "false"
// TestDummyPrint: it required to do not print really the text but perform only a computation about size
//              of printed text (use DT_CALCRECT flag in DrawText() function)
// WTF_Flags : one or more enumWTF_ flags
// returns the size of printed text or 0 if callback wants letting to original code the target to print this text
// note: the font has already been selected in hDC
typedef int (__cdecl *CALL_WINDOWS_TEXT_PRINT)(RECT *pRect, StrWindowsFont *pFont,HDC hDC, char *pText, 
						bool *pTestError, bool TestDummyPrint, DWORD WTF_Flags);
