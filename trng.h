// FOR_YOU: IMPORTANT WARNING: don't change or add text to this source because it will be
// replaced after new update and in this way you should lose your adding.
// Use the "Constants_mine.h" file to add your constants or macros, 
// while if you wish define external functions or variable you can type
// them in the Plugin_trng.cpp source, immediatly after the #include rows
// #VERSION_UPDATE_INFO#=PU-8

// ------------- DECLARES OF EXTERNAL FUNCTIONS OF TRNG SERVICES ----------------
// FOR_YOU:
// this is a list of available functions.
// Some of them are shortcuts for trng services, while others are utilities 
// typed in "trng.cpp" source to help you
int AbsDiff(DWORD First, DWORD Second);
int AbsDiffY(int First, int Second);
void GetTrngInfo(void);  // called byself in warm up, don't use it ever more
bool GetCallBack(int CallBackType, int CBT_Flags, WORD Index, void *pProc); // require a CB_ callback
bool SendToLog(char *szFormat, ...);  // send a messago to log with variable arguments
extern char *GetString(int StringIndex); // get string from script.dat
int FromNgleIndexToTomb4Index(int NgleIndex);  // convert ngle moveable index
void *ResizeMemory(void * pOldMemory, DWORD NewSize);  // change size of previously allocated memory usign GetMemory
int FromTomb4IndexToNgleIndex(int TombIndex);  // convert tomb4 moveable index
bool IsFullScreenMode(void); // discover if the game is playing in full (exclusive) screen mode
int Service(DWORD SRV_Type, ...);  // main call for trng services. See SRV_.. constants
bool TryMessageBox(char *pMessage); // show (if it is possible) a message box
int ApplyCodePatch(DWORD Offset, BYTE *pVetBytes, int TotBytes); // write a patch in tomb4 code
bool IsThereFile(char *pFileName); // check if a file exists
void *GetMemory(DWORD SizeOfMemory); // get the wished size of memory
void FreeMemory(void *pMem); // free the memory allocated with GetMemory()
void PrintText(int CordX, int CordY, char *pText, WORD FTflags, int FCindex, WORD FTSflags);
BYTE *LoadFile(char *pFileName, DWORD *pSize); // load in memory a file a return the address where it has been loaded
void FreeMem(BYTE *pMem); // free the memory got with GetMemory() or LoadFile()
bool PerformFlipeffect(char *pPluginName, int FlipNumber, int Arg1, int Arg2);
void PerformActionTrigger(char *pPluginName, int ActionNumber, int ObjectIndex, int ExtraTimer);
void ForceAnimationForItem(StrItemTr4 *pItem, int NumAnimation, int NextStateId);
void DrawSprite2D(RECT *pRect, WORD Slot, int SpriteIndex, BYTE Opacity, COLORREF Color);
void DrawObject2D(WORD Slot, int CordX, int CordY, WORD OrientX, WORD OrientY, WORD OrientZ, int Distance);
void DrawMesh3D(StrPos3d *pPos, int MeshIndex);
bool AllocateTombHdc(StrShowImage *pBase, bool TestHdcTemp, bool TestWriteHdc);
bool AllocateImage(int NImage, StrRecordImage *pRecord, int ForceSizeX, int ForceSizeY);
DWORD ReadDxInput(void);
void SuspendAudioTrack(int NewAudioTrack, bool TestLoop);
void ResumeAudioTrack(void);
void FreeWindowsFont(StrWindowsFont *pFont);
int ReadInputBox(HDC hDC, RECT *pRect, char Buffer[], DWORD MaxChars, WORD RIB_Flags, int SfxSound);
HFONT CreateWindowsFont(StrWindowsFont *pFont, bool TestNoResize);
void ConvertMicroUnits(RECT *pRect);
void FreeImage(StrRecordImage *pRecord);
void FreeTombHdc(StrShowImage *pBase, bool TestKeepTempHdc);
int ApplyRestoreCodePatch(DWORD StartOffset, DWORD EndOffset);
int ApplyCallPatch(DWORD *pVetOffsets, int TotOffsets, DWORD NewDestAddress);
bool PerformConditionTrigger(char *pPluginName, int ConditionNumber, int ObjectField, int Extra);
bool SetReservedDataZone(DWORD StartOffset, DWORD NBytes);
bool PerformExportedTrigger(char *pPluginName, int Arg1, int Arg2, int Arg3);
int FromStaticIndicesToNgleIndex(int RoomIndex, int StaticIndex);
int ApplyParametricPatch(StrParamPatch *pVetParamList, int TotParamList, int NewValue);
int CreateNgleStaticIndex(int RoomIndex, int StaticIndex);
int ApplyRelocatorPatch(DWORD Offset, BYTE *pVetBytes, int TotBytes, DWORD ProcStart, DWORD ProcEnd);
void ClearMemory(void *pZone, DWORD SizeBytes); // set 0 values in a wide memory zone
int CompareTrngVersion(WORD VetVersion[]); // verify se trng version is < = > than ....
void * GetRoomOff(int RoomIndex, DWORD RoomField);  // used with GetRoomOffset() macro
void * GetItemOff(int ItemIndex, DWORD ItemFields);  // used with GetItemOffset() macro
void * GetStaticOff(int StaticIndex, int RoomIndex, DWORD StaticField); // used with GetStatic() macro
DWORD ConvNgleStaticIndex(WORD NgleStaticIndex); // used with ConvertStaticIndex() macro
int FindPluginID(char PluginName[]); // to locate ID of other plugin currently loaded in trng process
int ConvertFromStrItemToItemIndex(StrItemTr4 * pItem);
void AddNGToken(WORD NGTag, DWORD TotItem, 
				   DWORD SizeSingleItem, void *pData,
				   WORD **p2VetExtra, int *pNWords);  // to add data to savegame header (see cbSaveGame() procedure)
void AddTokenFinalSequence(WORD **p2VetExtra, int *pNWords); // write final sequence for ng header
bool ParseNgField(WORD *pNgArray, DWORD CurrentIndex, 
										StrParseNGField* pParseNGField); // scan ng array of extra ng header
bool DetectedGlobalTriggerEvent(int GT_Event, int Parameter, bool TestIgnoreParameter);
void InitialiseEnums(void);
bool Get(int GET_Type, int Index, int SecondaryIndex);
bool Find(int FindType, short SlotType, short RoomIndex, short Ocb, int Extra, void *pPointer);
bool CheckControlGlobTomb4(void);
bool IsGraySector(void *pFloor);
bool TestEnvCondition(int ItemIndex, int EnvCondition, int EnvPosFlags, int DistanceEnv, int Extra);
void GetIncrements(WORD Facing, int *pIncX, int *pIncZ, int Distance);
int GetDistanceXZ(DWORD SourceX, DWORD SourceZ, DWORD TargetX, DWORD TargetZ);
int GetDistanceXZY(DWORD SourceX, int SourceY, DWORD SourceZ, DWORD TargetX, int TargetY, DWORD TargetZ);
int GetMaxDistance(DWORD *pSource, DWORD *pTarget, bool TestIgnoreY);
WORD GetDirection(DWORD SourceX, DWORD SourceZ, DWORD TargetX, DWORD TargetZ);
bool CheckDirection(StrItemTr4 *pSourceItem, int OffSourceY, int TargetIndex, int OffTargetY, 
					short TolleranceH, short TolleranceV);
bool CheckPositionAlignment(StrTestPositionCmd *pTestPosition, int ObjectIndex);
short GetAlignedOrient(short Orient, bool TestForceHortogonal, int *pGap);
bool IsCollideWithStatic(int IndexItem, int StaticIndex, int StaticRoomIndex, int Tollerance);
bool IsCollideWithMoveable(int IndexPrimaryItem, int IndexSecondaryItem, int Tollerance);
bool IsCollidingWithSomeItem(int ItemIndex, DWORD x, int y, DWORD z, int RoomIndex, 
							 int MaxDistance, int MinLargerSide, int MinHeight, int Tollerance);
bool AlignLaraAtPosition(StrTestPositionCmd *pTestPosition, int ObjectIndex);
int AbsDiffO(short First, short Second);
bool CheckFloor(DWORD x, int y, DWORD z, int RoomIndex);
bool UpdateItemRoom(int ItemIndex);
void ForceAnimationForLara(int NumAnimation, int NextStateId);
int PerformTriggerGroup(int IdOfTriggerGroup);
int GetCurrentFrame(StrItemTr4 *pItem);
void SetCurrentFrame(StrItemTr4 *pItem, int Frame);
void SignalMovedItem(int IndexItem);
void DeleteTriggerGroup(int IdTriggerGroup);
void DeleteGlobalTrigger(int IdGlobalTrigger);
void DeleteMultEnvCondition(int IdMultEnv);
void DeleteTestPosition(int IdTest);
void DeleteOrganizer(int IdOrg);
void DeleteAddEffect(int IdEffect);
void DeleteParamCommand(int ParamType, int IdParam);
short CreateNewMoveable(WORD Slot, DWORD CordX, int CordY, DWORD CordZ, int Room);
void DeleteNewMoveable(short Index);
int CreateAIObject(WORD Slot, DWORD CordX, int CordY, DWORD CordZ, WORD Room, int OcbValue, short Facing);
void DeleteAIObject(WORD Slot, WORD OcbValue, bool TestAlls);
bool LogOnDebug(char *szFormat, ...);
void DisableSaving(int FrameDurate);
bool StartMoveItem(int ItemIndex);
void EndMoveItem(int ItemIndex);
void SetCamera(int Distance, int HOrient, int VOrient, int Speed);

// ----------------- DECLARES of variables declared in trng.cpp source -----------------------

extern StrTrngInfos Trng;
extern StrGetLocator GET;
extern StrFind FIND;
extern StrFloorAnalyse FLOOR;
extern StrLOFData LOF;
extern StrCollisionLast COLLIDE;
// #TAG_START_AUTO_ENUM_STUFF#
extern StrEnumFL enumFL;
extern StrEnumRIB enumRIB;
extern StrEnumWTF enumWTF;
extern StrEnumFT enumFT;
extern StrEnumFTS enumFTS;
extern StrEnumFC enumFC;
extern StrEnumPRET enumPRET;
extern StrEnumPHASE enumPHASE;
extern StrEnumIRET enumIRET;
extern StrEnumCPU enumCPU;
extern StrEnumAPPC enumAPPC;
extern StrEnumCB enumCB;
extern StrEnumSLOT enumSLOT;
extern StrEnumGET enumGET;
extern StrEnumSKIP enumSKIP;
extern StrEnumFL2 enumFL2;
extern StrEnumHOLD enumHOLD;
extern StrEnumLLF enumLLF;
extern StrEnumSCANF enumSCANF;
extern StrEnumMESH enumMESH;
extern StrEnumTRET enumTRET;
extern StrEnumCTRET enumCTRET;
extern StrEnumSRET enumSRET;
extern StrEnumJOINT enumJOINT;
extern StrEnumSC enumSC;
extern StrEnumSTRING enumSTRING;
extern StrEnumADD enumADD;
extern StrEnumENV enumENV;
extern StrEnumCBT enumCBT;
extern StrEnumFAN enumFAN;
extern StrEnumFLH enumFLH;
extern StrEnumCMD enumCMD;
extern StrEnumSRV enumSRV;
extern StrEnumSLEV enumSLEV;
extern StrEnumSMAIN enumSMAIN;
extern StrEnumEDGX enumEDGX;
extern StrEnumDGX enumDGX;
extern StrEnumFROOM enumFROOM;
extern StrEnumFIND enumFIND;
extern StrEnumSSLOT enumSSLOT;
extern StrEnumFWEAP enumFWEAP;
extern StrEnumCBEET enumCBEET;
extern StrEnumWSKIN enumWSKIN;
extern StrEnumCOMBO enumCOMBO;
extern StrEnumKCOMBO enumKCOMBO;
extern StrEnumEX16 enumEX16;
extern StrEnumPCOMBO enumPCOMBO;
extern StrEnumDENV enumDENV;
extern StrEnumSLOPE enumSLOPE;
extern StrEnumORIENT enumORIENT;
extern StrEnumFSLOT enumFSLOT;
extern StrEnumFITEM enumFITEM;

// #TAG_END_AUTO_ENUM_STUFF#
