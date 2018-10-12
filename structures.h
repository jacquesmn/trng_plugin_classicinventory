

// FOR_YOU: IMPORTANT WARNING: don't change or add text to this source because it will be
// replaced after new update and in this way you should lose your adding.
// Use the "structures_mine.h" file to add your structures


// don't remove following line: it's a way to keep memory about current release of 
// your structures.h source
// #VERSION_UPDATE_INFO#=PU-8

// =========== STRUTTURE PRAGMA ALLINEATE BYTE ============================

#pragma pack(1)




typedef struct StrProcBassDll {
	TYPE_BASS_Init BASS_Init;
	TYPE_BASS_Free BASS_Free;
	TYPE_BASS_ChannelSlideAttribute  BASS_ChannelSlideAttribute;
	TYPE_BASS_ChannelSetAttribute BASS_ChannelSetAttribute;
	TYPE_BASS_StreamCreateFile BASS_StreamCreateFile;
	TYPE_BASS_ChannelPlay BASS_ChannelPlay;
	TYPE_BASS_ErrorGetCode 	BASS_ErrorGetCode;
	TYPE_BASS_Pause BASS_Pause;
	TYPE_BASS_Start BASS_Start;
	TYPE_BASS_ChannelFlags BASS_ChannelFlags;
	TYPE_BASS_ChannelStop BASS_ChannelStop;
	TYPE_BASS_Stop BASS_Stop;
	TYPE_BASS_ChannelSetPosition BASS_ChannelSetPosition;
	TYPE_BASS_ChannelIsActive  BASS_ChannelIsActive;
	TYPE_BASS_ChannelGetPosition BASS_ChannelGetPosition;
	TYPE_BASS_StreamGetFilePosition  BASS_StreamGetFilePosition;
}ProcBassDllFields;

// data to apply asm patch on tomb4 exe
typedef struct StrPatchInfo {
	DWORD PluginId;
	DWORD StartOffset;
	DWORD ProcStart;  // only for relocator patch, it keeps the real start offset of relocated procedure
	DWORD ProcEnd;  // only for relocator patch, it keeps the ending offset of relocated procedure
	int TotItems;
	void *pVetItems;
	int PatchType;  // TYPP_
	int ErrorCode;  // APPC_ 
	int NewValue;  // per parametric constant e call (nuovo valore assegnato)
}BasePatchInfoFields;

typedef struct StrPatchParameters {
	DWORD Offset; // where to write
	int   Value; // value to write 
	int   SizeValue;  // 1 = byte / 2=word / 4=dword
}BasePatchParametersFields;

// function protypes used only with trng plugins
typedef bool (WINAPI  *TYPE_RequireCallBack) 
			(WORD ID_Plugin, int CBValue, int CBType, int Index, void *pProc);

typedef int (WINAPI *TYPE_SetNewPatch)
			(StrPatchInfo *pDataPatch, bool TestWarnings);


typedef int (WINAPI *TYPE_Service) 
			(DWORD ID_Plugin, DWORD SRV_Value, va_list pArgs);



typedef struct StrExtractNG {
	int Result;
	DWORD SizeHeader;
	DWORD StartOffset;
	DWORD NWords;
	WORD *pNGArray;
}ExtractNGFields;

typedef struct StrLastScriptDat {
	FILETIME DataLastWrite;
	DWORD SizeFile;
}LastScriptDatFields;

typedef struct StrMiniNG_Header {
	WORD NumeroLivello;
	WORD TotWords;
	WORD *pVetWords;
}MiniNG_HeaderFields;

typedef struct StrBaseNG_Hub {
	WORD TotHub;
	WORD LastIndex; // indice dove era stato salvato l'ultimo livello
	StrExtractNG LaraHUB; // StrExtractNG per lara
	StrMiniNG_Header VetNG[10];
}BaseNG_HubFields;

typedef struct StrNomeMessaggio {
	char *pNome;
	DWORD Numero;
}NomeMessaggioFields;

typedef struct StrParseNGField {
	DWORD NextIndex;
	WORD *pData;
	DWORD SizeData;
	DWORD StartDataIndex;
	WORD  Type;
}ParseNGFieldFields;


typedef struct StrPointer {
	DWORD *pMem;
	char  Nome[80];
	int TipoExe;  // 1=tomb  2=ngle
}PointerFields;




typedef struct StrPrefTomb {
	DWORD Flags;
	int Reserved[80]; // to reduce when I add new fields to keep with same length the StrPrefTomb structure
}PrefTombFields;

typedef struct StrScriptTrigger {
	WORD PluginId;
	WORD Flags;
	WORD Object;
	union {
		WORD Timer;
		short TimerSigned;
	};
}ScriptTriggerFields;

typedef struct StrTriggerGroup {
	WORD IdGroup;
	WORD TotTriggers;
	StrScriptTrigger VetTriggers[MAX_TRIGGERS_IN_THE_GROUP];
	WORD Dynamic;
}TriggerGroupFields;

typedef struct StrBaseTriggerGroups {
	int TotTriggerGroups;
	StrTriggerGroup VetTriggerGroups[MAX_TRIGGER_GROUPS];
	short VetID[MAX_TRIGGER_GROUPS*10];
}BaseTriggerGroupsFields;

typedef struct StrMoveAdvance {
	WORD IdMove;
	WORD Flags;
	WORD Direction;
	WORD Distance;
	WORD Speed;
	short IndiceTriggerGroup;
	short SoundStart;
	short SoundMoving;
	short SoundFInal;
	WORD Dynamic;
}MoveAdvanceFields;

typedef struct StrBaseMovAdvance {
	int TotMoveAdvance;
	StrMoveAdvance VetMove[MAX_MOVE_ADVANCE];
	short VetID[MAX_MOVE_ADVANCE*10];
}BaseMovAdvanceFields;

typedef struct StrMoveParameters {
	WORD IdMove;
	WORD Dynamic;
	WORD Flags;
	WORD ItemIndex;
	WORD Direzione;
	WORD Distanza;
	WORD Speed;
	short Extra;
	short SuonoMovimento;
	short SuonoFinale;
}MoveParametersFields;

typedef struct StrBaseMove {
	int TotMove;
	StrMoveParameters VetMove[MAX_MOVE_PARAM];
	short VetID[MAX_MOVE_PARAM*10];
}BaseMoveFields;

typedef struct StrRotateItem {
	WORD IdMove;
	WORD Flags;
	short ItemIndex;
	short DirezioneH;
	WORD AngoloH;
	WORD SpeedH;
	short DirezioneV;
	WORD AngoloV;
	WORD SpeedV;
	short SoundMoving;
	short SoundFinal;
	WORD Dynamic;
}RotateItemFields;

typedef struct StrBaseRotate {
	int TotRotate;
	StrRotateItem VetRotate[MAX_ROTATE_PARAM];
	short VetID[MAX_ROTATE_PARAM*10];
}BaseRotateFields;


// structure to point to registers saved in the stack (used by callback for trng patches)
typedef struct StrRegisters {
	// ESP value (adjusted to be the same of last istruction in tomb4 code before calling trng patch)
	BYTE * pESP;	

	// Register flags
	DWORD CPU_Flags;  // flags (enumCPU_) used to test [c]arry, [z]ero, [o]verflow ect ect (flag registers).
	// EDI
	union {
		DWORD EDI; // unsingned
		int   edi;   // signed

		union {
			WORD DI;
			short di;
		};
		

	};
	// ESI
		
	union {
		DWORD ESI; // unsingned
		int   esi;   // signed
	
		union {
			WORD SI;
			short si;
		};

		
	};

	// EBP
	union {
		DWORD EBP; // unsingned
		int   ebp;   // signed

		union {
			WORD BP;
			short bp;
		};

		
	};
	DWORD UnusedREG;

	// EBX 

	union {
		DWORD EBX;
		int ebx;
		
		union {
			WORD BX;
			short bx;

		};

		struct {
			
			BYTE BL;
			BYTE BH;

		};
	};


	// EDX 


	union {
		DWORD EDX;
		int edx;

		union {
			WORD DX;
			short dx;

		};

		struct {
			
			BYTE DL;
			BYTE DH;

		};
	};

	// ECX 

	union {
		DWORD ECX;
		int ecx;

		union {
			WORD CX;
			short cx;

		};

		struct {
			
			BYTE CL;
			BYTE CH;
		};
	};


	// EAX 


	union {
		DWORD EAX;
		int eax;

		union {
			WORD AX;
			short ax;
		};

		struct {
			BYTE AL;
			BYTE AH;
		};
	
	};


}RegistersFields;

typedef struct StrRecordFlip {
		WORD  Numero;  // numero del flipeffect
		WORD Timer; // valore argomento timer
		WORD Flags; // dati peridentificare in modo univoco questo trigger  SCANF_... scanf flags

		DWORD OffsetFloorData;
				// distanza da inizio floor data di record 
				//						<TRIGGER WHAT=flipeffect> <NUMERO FLIP>
		WORD Indice; // indice item che ha eseguito questo flipeffect
		WORD PluginId; // 0= trng, or id of plugin owner of this trigger
}RecordFlipFields;
typedef struct StrOldTrigger {
	WORD Flags;
	DWORD OffsetFloorData;
}OldTriggerFields;
// record per salvataggio orientamento di animating
// da salvare e ripristinare da savegame per mantenere
// le rotazioni effettuate con flipeffects

typedef struct StrSalvaCords {
	WORD OrientingH;
	WORD OrientingV;
	WORD FlagInvisibile;
	DWORD CordX;
	int   CordY;
	DWORD CordZ;
	WORD  Room;

}SalvaCordsFields;

typedef struct StrBaseSalvaCords {
	WORD TotSalvati;
	WORD VetIndici[MAX_SALVA_CORD];
	StrSalvaCords VetSalvati[MAX_SALVA_CORD];
}BaseSalvaCordsFields;

typedef struct StrBaseInvItems {
	int TotInvItems;
	WORD VetInvItems[256];
}BaseInvItemsFields;

typedef struct StrCameraTr4 {
	DWORD CordX;		// 0X00
	int  CordY;			// 0X04
	DWORD CordZ;		// 0X08
	WORD Room;			// 0X0c
	WORD Flags;			// 0X0e
}CameraTr4Fields;		// 0X10

typedef struct StrTwoBytes {
	BYTE AlfaByte;
	BYTE BetaByte;
}DueByteFields;

typedef struct StrPosizione {
	int OrgX;
	int OrgY;
	int OrgZ;
}PosizioneFields;

typedef struct StrTriplePoint {
	DWORD CordX;
	int CordY;
	DWORD CordZ;
}TriplePointFields;

typedef struct StrDoublePosition {
	StrPosizione From;
	StrPosizione To;
}DoublePositionFields;


typedef struct StrRectXY {
	RECT Rect;
	int XCord;
	int YCord;
}RectXY;

// structure of progressive action
// size 36 (0x24)
typedef struct StrProgressiveAction {
	WORD ActionType;  // AXN_ code
	
	short ItemIndex;  // Index of item to manage
	WORD Arg1;  // durate of the action: number of frames or 0xffff for endless
	union {
		WORD Arg2;    //  variable for customizable targets
		StrTwoBytes Bytes;
	};
	union {
		int  VetArg[6];  // 6 numbers of int type (32 signed bits)
		float VetArgFloat[6];
		WORD VetArgWord[12];
		short VetArgShort[12];
		BYTE VetArgBytes[24];
		char VetArgSignedBytes[24];
		DWORD VetArgDword[6];
		StrDoublePosition Coords;
		StrRectXY Box;

	};
}ProgressiveActionFields;




// struttura per vetcostantinemici size 0x26
typedef struct StrArmiTr4 {
	short ArmaSingleHMinAngle;  // 0x00
	short ArmaSingleHMaxAngle;  // 0x02
	short ArmaSingleVMinAngle;  // 0x04
	short ArmaSingleVMaxAngle;  // 0x06


	short ArmaRightHMinAngle;    // 0x08
	short ArmaRightHMaxAngle;	// 0x0a
	short ArmaRightVMinAngle;    // 0x0c
	short ArmaRightVMaxAngle;	// 0x0e

	short ArmaLeftHMinAngle;    // 0x10
	short ArmaLeftHMaxAngle;	   // 0x12
	short ArmaLeftVMinAngle;    // 0x14
	short ArmaLeftVMaxAngle;	   // 0x16


	WORD DatoArmaEstratta;		    // 0x18  senza sparare o puntare nemici
	WORD Random;		// 0x1A quando si spara

	short CordYArma;			// 0x1C
	WORD DistanceTarget;		// 0x1E

	BYTE DannoArmaNormale;		// 0x20
	BYTE TempoRicarica;		    // 0x21  

	BYTE DurateFlash;			// 0x22
	BYTE FrameCambioArma;		// 0x23
	WORD SuonoSparo;			// 0x24
}ArmiTr4Fields;

// struttura per qazioni rilevate durante un ciclo di gioco (in modo
// simile a quanto avviene per flipeffects)
typedef struct StrScanAction {
	WORD Timer; // valore argomento timer
	WORD ItemIndex;
	WORD Flags; // dati peridentificare in modo univoco questo trigger
			//Byte basso: numero stanza
             //  Byte alto: flags 
			//		0x0100 = attivato con heavy
			//		0x0200 = One-shot solo temporaneo
			//      0x0400 = Pulsante one-shot di trigger del flipeffect
			//      0x0800 = Ancora da eseguire
	DWORD OffsetFloorData;
			// distanza da inizio floor data di record 
			//						<TRIGGER WHAT=Action>
	WORD PluginId; // 0= trng, or plugin owns this trigger
}ScanActionFields;




typedef struct StrTexInfoTr4 {
	WORD Attribute;			// 0
	WORD Tail;				// 2
	WORD Flags;				// 4
	float TopLeft[2];		// 6
	float TopRight[2];		// 0E
	float BottomRight[2];	// 16
	float BottomLeft[2];	// 1E
							// 26
}TexInfoTr4Fields;
// struttura ancora presente in file .tr4, poi viene modificata
// e diventa come struttura qua sopra (StrTexInfoTr4)
typedef struct StrTailInfoTr4 {
	WORD Attribute;			// 0
	WORD Tail;				// 2
	WORD Flags;				// 4
	WORD Vertices[8];		// 6
	int OrgX;				// 16
	int OrgY;				// 1A
	int SizeX;				// 1E
	int SizeY;				// 22	
							// 26
}TailInfoTr4Fields;


typedef struct StrAIDataTr4 {
	WORD SlotAI;  // valore di slot id
	WORD RoomIndex;
	DWORD CordX;
	int  CordY;
	DWORD CordZ;
	WORD Ocb;
	WORD Buttons;
	DWORD Orientation;
}AIDataTr4Fields;

typedef struct StrDatiXRain {
	int  LastRoomCamera;
	float Rain_Float_1;
	float Rain_Float_2;
	float Rain_Float_4;
	float Rain_Float_8;
	float Rain_Float_16;
	float Rain_Float_20480;
	WORD Max_Rain;
	WORD Min_Rain;
	WORD Flags; // FR_ ...
	short SoundSFX; 
	int  LastIntensita;
	DWORD SplashRain;

}DatiXRainFields;

typedef struct StrDatiXSnow {
	WORD Max_Snow;
	WORD Min_Snow;
	int LastRoomCamera;
	int LastIntensita;
}DatiXSnowFields;

typedef struct StrConstExeValue {
	WORD SlotId;
	DWORD Offset;
	BYTE TipoSize;
}ConstExeValueFields;

typedef struct StrInventoryItems {
	BYTE WeaponPistols;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponUZI;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponShotGun;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponCrossBow;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponGrenadeGun;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE WeaponRevolver;		//  FWEAP_ values to test with & operator (bit flags)
	BYTE LaserSight;		//  0 / 1
	BYTE Binoculars;		//  0 / 1
	BYTE Crowbar;		//  0 / 1
	BYTE ClockWorkBeetle;		//  CBEET_ values to test with & operator (bit flags)
	BYTE WaterSkinSmall;		//  WSKIN_ values to test in mixed way, sometimes with == and other flag with &
	BYTE WaterSkinLarge;		//  WSKIN_ values to test in mixed way, sometimes with == and other flag with &
	BYTE ExamineItem1;		//  0 / 1
	BYTE ExamineItem2;		//  0 / 1
	BYTE ExamineItem3;		//  0 / 1
	BYTE PuzzleItem1;		//  quantity
	BYTE PuzzleItem2;		//  quantity
	BYTE PuzzleItem3;		//  quantity
	BYTE PuzzleItem4;		//  quantity
	BYTE PuzzleItem5;		//  quantity
	BYTE PuzzleItem6;		//  quantity
	BYTE PuzzleItem7;		//  quantity
	BYTE PuzzleItem8;		//  quantity
	BYTE PuzzleItem9;		//  quantity
	BYTE PuzzleItem10;		//  quantity
	BYTE PuzzleItem11;		//  quantity
	BYTE PuzzleItem12;		//  quantity
	WORD ComboItems;		//  COMBO_ values to test with "&" operator (bit flags)
	WORD Keys;				//  EX16_ values to test with "&" operator (bit flags) 
	WORD KeyCombo;			//  KCOMBO_ values to test with "&" operator (bit flags)
	short PickupItems;		//  EX16_ values to test with "&" operator (bit flags) 
	short PickupCombo;		//  PCOMBO_ values to test with "&" operator (bit flags) 
	short QuestItems;		//  EX16_ values to test with "&" operator (bit flags) 
	short MediPackSmall;		//  quantity (-1 = unlimited)
	short MediPackLarge;		// quantity (-1 = unlimited)
	short Flares;		// quantity (-1 = unlimited)
	short AmmoPistols;		// quantity (-1 = unlimited)
	short AmmoUZI;		//  quantity (-1 = unlimited)
	short AmmoRevolver;		// quantity (-1 = unlimited)
	short AmmoShotgunNormals;		//  quantity multiple by 6 (-1 = unlimited)
	short AmmoShotgunWideShot;		//  quantity multiple by 6 (-1 = unlimited)
	short AmmoGrenadeNormals;		// quantity (-1 = unlimited)
	short AmmoGrenadeSuper;		//  quantity (-1 = unlimited)
	short AmmoGrenadeFlash;		// quantity (-1 = unlimited)
	short AmmoCrossBowNormals;		// quantity (-1 = unlimited)
	short AmmoCrossBowPoison;		// quantity (-1 = unlimited)
	short AmmoCrossBowExplosive;		// quantity (-1 = unlimited)
	BYTE ClockWorkBeetleAvailUsage;		// available times to use it. When it becomes 0 it will explode but it is 0 also when beetle has not yet been picked up

}InventoryItemsFields;

typedef struct StrSlot {
	WORD  TotMesh;		// 0
	WORD  IndexFirstMesh;	// 2
	int   IndexFirstTree;	// 4
	int   IndexFirstFrame;  // 8
	void *pProcInitialise;	// 0C
	void *pProcControl;		// 10	
	void *pProcFloor;		// 14
	void *pProcCeiling;		// 18
	void *pProcDraw;		// 1C
	void *pProcCollision;   // 20
	WORD  DistanceForMIP;      // 24  
	WORD  IndexFirstAnim;   // 26
	short Vitality;			// 28
	WORD DistanceDetectLara;		// 2A
	WORD ss_Unknown3;		// 2C
	WORD FootStep;			// 2E
	WORD  TestGuard;		// 30
	WORD Flags;				// 32  (FSLOT_ flags)
	void *pProcDrawExtras;	// 34
	int  ShatterableMeshes;		// 38  
	int  ss_Unknown5;		// 3C
}SlotFields;


typedef struct StrExtraInfoRoom {
	BYTE  WaterIntensity; // usato per pioggia e neve
	BYTE  Reserved[7]; // altri 8 byte da usare in futuro
}ExtraInfoRoomFields;

typedef struct StrExtraLangugage {
	WORD Indice;
	char *pTesto;
}ExtraLangugageFields;

// dati relativi a sezione Options di scrip.dat, valide per tutti i livelli
typedef struct StrScriptOpzioni {
	WORD MainFlags;   // valori ngfm_...
	WORD DisablePatchMem;  // valori DPM_ ...
	WORD Reserved[80]; // to change when I add new fields to this structure
}ScriptOpzioniFields;
typedef struct StrIndiciAssign
{
	int IndiceAnimRubberBoat;	// dati per nuovo oggetto RUBBER BOAT
	int IndiceRubberBoat;
	int IndiceAnimMotorBoat;
	int IndiceMotorBoat;
}IndiciAssignFields;

typedef struct StrScriptLevel {
	WORD LevelFlags; // valori fngl_
	StrIndiciAssign  AssignSlot;
	
}ScriptLevelFields;

// struttura per dati relativi a formattazione printstring
typedef struct StrPrintString {
	WORD Colore;   // CL_...
	WORD Unused;
	WORD Posizione; // STRING_...
	WORD Flags;  // FT_...
	BYTE BlinkSpeed;   // default era 0x10
	BYTE FlagsMicro;  
	BYTE DefFlagsMicro;  // per livello al di fuori di setting per ogni flipeffect
	BYTE DefALLFlagsMicro;  // per title e roba menu
	WORD DefColore;
	WORD Unused2;
	WORD DefPosition;
	BYTE DefBlinkSpeed;
	WORD DefFlags;
	BYTE Unused3;
}PrintStringFields;

typedef struct StrParamPrintText {
	StrPrintString Formatting;
	short IdPrint;
	short  DurateTime;
	short  OrgX;
	short  OrgY;
	WORD Dynamic;
}ParamPrintTextFields;

typedef struct StrMemAllocata {
	void *pBaseMem;
	int ThreadId;
	WORD TestFree; // richiesta di liberare memoria
	char *pDescrizione; // testo per ricordare cosa sta allocando
}MemAllocataFields;

typedef struct StrBaseMemAllocata {
	int TotAllocata;
	StrMemAllocata VetAllocata[MAX_ALLOCATA];
}BaseMemAllocataFields;



typedef struct StrBasePrintText {
	int TotPrintText;
	StrParamPrintText VetPrint[MAX_PARAM_PRINT_TEXT];
	short VetID[MAX_PARAM_PRINT_TEXT*10];
}BasePrintTextFields;

typedef struct StrSetCamera {
	short IdCamera;
	WORD Flags; // FSCAM_...
	short Distance;
	short HOrient;
	short VOrient;
	WORD Speed;
	WORD Dynamic;
}SetCameraFields;

typedef struct StrBaseSetCamera {
	int TotSetCamera;
	StrSetCamera VetSetCamera[MAX_SET_CAMERA];
	short VetID[MAX_SET_CAMERA*10];
}BaseSetCameraFields;

typedef struct StrRecordPoint {
	int Valore; // usato per #POINT# 
	int Operazione; // usato per #POINT#
	DWORD OffsetBreak;
	BYTE Originale; // valore originale
	short Registro; // numero di indice registro oppure -1 se non e' un registro
	bool TestAttiva; // se e' stato messo 0xcc e' attiva
}RecordPointFields;

typedef struct StrRecordDGN {
		void *pIndirizzo;  // 
		int  OldValue; // per confronto con precedente scansione
		int CodiceCostante; // se e' una variabile mnemonica qui c'e' OFF_...
		WORD FlagsType;  // (mascherato a byte, word, long o struct)
		DWORD StructOffset;  // offset da inizio struttura
		int  Indice; // indice di vettore (usati solo se pointer)
		WORD MaxIndice;  // (se si  deve stampare la serie di un vettore)
		WORD SizeStruct;  // (se e' un vettore di strutture questa e' la dmensione)
		bool TestPoint;  // se true allora e' una POINT
		StrRecordPoint Point; // dati point
		char Descrizione[40]; // (stringa descrittiva)
}RecordDGNFields;
// dati per comandi script LogItem=
typedef struct StrLogItem {
		WORD Flags; // FLI_...
		short ItemIndex;  // indice da controllare
}LogItemFields;

typedef struct StrDgxInfoSlot
{
	WORD Slot;
	WORD PrimaAnimazione;
	WORD UltimaAnimazione;
}DgxInfoSlotFields;

typedef struct StrBaseDiagnostica {
		int TotRecord;
		bool TestPrimaEsecuzione;
		bool TestCaricato;
		bool TestPremutoF7;
		int TotPoints;
		WORD VetSFXMancanti[MAX_SFX_MANCANTI];
		int TotSFXMancanti;
		bool TestErrore;  // se = true non provare piu' a caricare file
		bool TestSospendiBreakCond; // se true ignora modifiche finche questo true
		WORD When;   // FWN_...
		WORD Flags;  // FLAG_...
		DWORD LastTime; // usato per esecuzione ogni secondo
			//usati per point_media e point time
		DWORD Tempo; // usato per POINT_TIME e POINT_START_TIME
		DWORD SommaTempo; // usato per le medie
		int TotCicli;  // numero di cicli scanditi
		WORD FlagsDgx; // DGX_.. cosa mostrare e cosa no
		WORD DgxExtra; // eventuale parametro extra 
		StrRecordDGN  VetWatch[MAX_WATCH];
		StrLogItem  LogItem;
		StrDgxInfoSlot  InfoSlot;
		short LivelloExtractSFX; // estrarre sfx di livello
		int TotTiming;
		DWORD LastTiming;
		DWORD VetTiming[MAX_TIMING];  // conserva le ultime differenze di tick frames tra attivazioni trigger
}BaseDiagnosticaFields;
	

typedef struct StrSalvaVettoriRemap {
	short VetObjRemap[6000];
	int  TotRooms;
	short VetRoomRemap[400];
	bool  TestAttivo;
	char NomeFileTom[256];
}SalvaVettoriRemapFields;

// ----------------------  strutture per file .mem di crash report


typedef struct StrBlockMem {
		DWORD StartCode;
		DWORD EndCode;
}BlockMemFields;


// struttura generale file mem:
//    StrHeaderMem
//    array of memory dump
//          all blocks
//          StrBlockMem
//				Bytes of block now
//	  DWORD CheckControl; // dword for control it has to be 0x01234567
//    array of rooms:
//          StrRoomTr4
//				array StrMeshInfo  (all statics of room now)
//				WORD TotDoors
//				array structStrDoorTr4  (all doors of room now)
//	  DWORD CheckControl; // dword for control it has to be 0x01234567
//    array of items (moveables)
//			StrItemTr4
//    DWORD CheckControl; // dword for control it has to be 0x01234567
//    array of slot of items			
//			StrSlot
//    DWORD CheckControl; // dword for control it has to be 0x01234567
//    array of animations
//			StrAnimationTr4
//    DWORD CheckControl;  // dword for control it has to be 0x01234567
//	  array of collisions for statics  (data about collision and view box of statics)
//			StrStaticSlot  
//	  DWORD CheckFinalControl;  // it has to be  0x76543210


typedef struct StrHeaderMem {
		DWORD MemVersion;  // current version with number 0x130913  (13 september 2013)
		DWORD MemFlags;  // mem_... flags
		int   MemParseHeader;   // numero di campo che si stava scandendo (in caso di crash)
		int	  MemParseBlocks;
		int   MemParseRooms;
		int   MemParseItems;
		int   MemParseSlots;
		int   MemParseAnimation;
		int   MemParseStatics;
		short VetRemapObjects[6000]; // conversion array from ngleindex to tombraider index   
		short VetRemapRooms[MAX_ROOMS];
		int Mem01_TotBlocks;
		int Mem02_TotRooms;
		int Mem03_TotItems;
		int Mem04_TotSlots;
		int Mem05_TotAnimazioni;
		int Mem06_TotStaticSlot;  // totveteditobj


		void *hm_01_pLara; 
		WORD hm_02_IndiceItemLara;
		WORD hm_03_ZonaSave_59_FlagOggettiManoLara;
		WORD hm_04_ZonaSave_5B_OggettoNow; 
		WORD hm_05_ZonaSave_5D_OggettoNext; 
		WORD hm_06_ZonaSave_5F_ArmaCorrente; 
		WORD hm_07_ZonaSave_63_StatusLara;
		short hm_08_ZonaSave_7F_IndiceVeicoloAttivo;
		short hm_09_ZonaSave_81_IndiceArmaInMano;
		DWORD hm_0A_FlagTastiPremuti;
		WORD  hm_0B_FlagTastiInput;
		DWORD hm_0C_TastiAltri; // altra variabile con tasti premuti
		BYTE hm_0D_ZonaSave_9B_TipoAnimazione;
		BYTE hm_0E_ZonaSave_9C_StatusLaraSpecial;
		int hm_0F_ZonaSave_157_IndiceMoveableAttivo;
		WORD hm_10_ZonaSave_119_FlareInMano;
		int hm_11_TimerScreen;
		short hm_12_SizeScreenX;
		short hm_13_SizeScreenY;
		short hm_14_SizeRiga;
		BYTE hm_15_NLivelloAttuale;
		WORD hm_16_FlagLivello81;
		BYTE hm_17_FlagsScriptDat;
		BYTE hm_18_SegretiTrovati;
		short hm_19_ZonaSave_6D_AriaDisponibile;
		int hm_1A_TestEsisteFlyByInCorso;
		BYTE hm_1B_TestLoadONewGame;
		DWORD hm_1C_ContatoreFrame;
		int hm_1D_CDNowMultiShot;
		BYTE hm_1E_TestSuonoMultishot;
		BYTE hm_1F_TestItemMoveable;
		float hm_20_StartFog;
		BOOL hm_21_TestDisableFogBulbs;
		BYTE hm_22_Setting_Volumetric;
		int hm_23_TipoInclinaturaSettore;
		int hm_24_InclinaturaSettoreX;
		int hm_25_InclinaturaSettoreZ;
		bool hm_26_TestLoading; // loading level in progress	

}HeaderMemFields;

// struttura di extra file posto subito dopo mem crash report
typedef struct StrExtraMemFile {
		WORD  ExtraBlockType;  // EBT_...
		DWORD CheckControl;  // sempre 0x01234567
		char FileName[64]; // file name (no folder, only the name) for current file
		DWORD FileSize;  // number of bytes of file
		// e ora seguiranno tutti i byte del file
}ExtraMemFileFields;

// ----------------------- fine strutture file .mem
typedef struct StrAddEffect {
		WORD Id;
		WORD Type;    // ADD_
		WORD Flags;   // FADD_
		WORD Joint;   // JOINT_
		short DispX;
		short DispY;
		short DispZ;
		WORD DurateEmit;
		WORD DuratePause;
		int  TotExtra;
		WORD VetExtra[MAX_EXTRA_EFFECT];
		WORD Dynamic;
}AddEffectFields;
typedef struct StrBaseEffects {
	int TotEffects;
	short VetID[MAX_ADD_EFFECTS*10];
	StrAddEffect VetEffects[MAX_ADD_EFFECTS];
}BaseEffectsFields;

typedef struct StrDamage {
	DWORD Colore;
	WORD Flags;   // FDM_..
	WORD SecondiMorte;
	short IndiceStringa; // o -1 se non presente
	WORD SecondiRicrescita;
		// fino a qui era valori presi da script, dopo questa riga
		// ci sono valori impostati nel corso della gestione damage
	int DamValue; // da salvare e ripristinare in savegame (moltiplicato * 100)
	int BlinkTime;
	WORD FlagProgresso;  // FPD_..
	WORD Percentuale; // valore eprcentuale da mostrare (a volte zero per lampeggio)
	WORD QuotaRiduzione; // numero da sottrarre a ogni frame di gioco
	WORD QuotaRicrescita; // numero da aggiungere per ripristino barra piena
	WORD QuotaRiduzioneRapida; // di vita, quando barra e' finita
	WORD PercentualeBeep;
}DamageFields;


typedef struct StrBoxCollisione {
	short MinX;			// 0x00
	short MaxX;			// 0x02
	short MinY;			// 0x04
	short MaxY;			// 0x06
	short MinZ;			// 0x08
	short MaxZ;			// 0x0A
}BoxCollisioneFields;

typedef struct StrBoxOrienting {
	short OrientVMin;	// 0x0C
	short OrientVMax;	// 0x0E
	short OrientHMin;	// 0x10
	short OrientHMax;	// 0x12
	short OrientRMin;	// 0x14
	short OrientRMax;	// 0x16
}BoxOrientingFields;		


// structure used to test alignment of lara with some object
typedef struct StrTestPosition {
	StrBoxCollisione Distance;
	StrBoxOrienting Orienting;
}TestPositionFields;

typedef struct StrAbsBoxCollision {
	int MinX;		//00
	int MaxX;		//04
	int MinY;		//08
	int MaxY;		//0C
	int MinZ;		//10
	int MaxZ;		//14
}AbsBoxCollisionFields;
// dati fissi da salvre in savegame per salvare molti dati modificati
// nel corso del gioco
// 32 word + 32 dword
typedef struct StrDatiVariabili {
	WORD  ValoreCold;
	WORD  FlagProgressoCold;
	WORD ValoreDamage;
	WORD FlagProgressoDamage;
	int KeysToStop;
	DWORD StatusNG;  // flags SNG_...

	WORD TestDisableFeatures;  // flags DF_....
	DWORD CounterGame;
	WORD FlagLivelloNow; // valore con i valori attuali di flag livello script
	DWORD ColoreLayer1;
	DWORD ColoreLayer2;
	BYTE SpeedLayer1;
	BYTE SpeedLayer2;
	short CdLoopSecondario;
	WORD LevelNGFlags;
	short CdSingleSecondario;
	DWORD Canale2StartPos;
	short CdLoopMain;
	short CdSingleMain;
	DWORD Canale1StartPos;
	float FloatFogStart;
	BOOL  NonUsato;
	DWORD Unused;
	short IndicePushSpinto;
	WORD ParBarGiri;
	DWORD ParBarFrames;
	WORD TestPopUp;  // se = 1 esiste un'immagine popup visualizzata
	WORD PopUpContatore;	// numero di tick frame di visualizzazione (o 0xffff per infinito)
	WORD PopUpIndiceImageCmd; // indice dell'immagine comando da visualizzare
	BYTE SalvaVolumetric; // salva stato prima di sospensione all fog
	BYTE TestHardFog; // usare questo solo come segnalato di esistnza di campi
					// successivi e usare in futuro questa tecnica
	BYTE TestMostraDetector; 
	BYTE NonUsatoByte;
	short FogEnd;
	BYTE FogColors[4];  // in realta' solo i primi 3 sono usati
	short FogDistanceNow;  // che sarebbe il nuovo fog start
	short FogBulbMaxDistance; // poi creato con calcolo gigantesco
		//fino blocco che eiste solo se c'e' testhardfog diverso da zero
	DWORD Reserved[100]; // to change when I add new fields

}DatiVariabiliFields;


typedef struct StrJiga {

	int TotAnim;
	bool TestLaraBalla;
	WORD VetAnim[100];
}JigaFields;

typedef struct StrRecordEnemyScript {
	WORD  SlotId;
	WORD  FlagsNEF;  // NEF_...
	WORD  Health; 
	short  Damage;  // danno arrecato a lara
	WORD  TombFlags;  //TCF_ in campo flags di slot
	short Extra; // word extra per setting particolari
	int  TotDamage; // numero di argomenti extra
	short  VetDamage[6];  // eventuali valori extra damage o segnali particolari
}RecordEnemyScriptFields;


typedef struct StrBaseEnemyScript {
	int TotEnemy;
	StrRecordEnemyScript VetEnemy[MAX_ENEMY_SCRIPT];

}BaseEnemyScriptFields;





typedef struct StrAnimScript {
	short AnimIndex;
	WORD  Key1;
	WORD  Key2;
	WORD  Flags;  // FAN_ flags
	WORD  Environment;  // ENV_ costanti
	short DistanceEnv; // distanza per environment
	WORD  Extra;
	int  TotStateId;
	short VetStateId[32];
}AnimScriptFields;

typedef struct BaseAnimScript {
	int TotAnimazioni;
	StrAnimScript VetAnimations[MAX_ANIM_SCRIPT];
}BaseAnimScriptFields;

typedef struct StrAnimObjScript {
	short Slot;
	short ActionType; // AXT_... or  negative value
	StrAnimScript Animation;
}AnimObjScriptFields;

typedef struct StrDisablePushAway {
	int TotDisable;
	short VetAnimNumber[MAX_ANIM_SCRIPT]; // number of animation where disable push away for lara
}DisablePushAwayFields;



typedef struct BaseAnimObjScript {
	int TotAnimObj;
	StrAnimObjScript VetAnimObj[MAX_ANIM_OBJ_SCRIPT];
}BaseAnimObjScriptFields;

typedef struct RecordMirror{
	WORD MirrorRoom;		// 0x00
	WORD HiddenRoom;		// 0x02
	WORD MirrorType;		// 0x04
	int CordMirror;			// 0x06
	int MinCordMirror;      // 0x0A usate solo per
	int MaxCordMirror;      // 0x0E mirror inverso
	int TotAnimating;		// 0x12
	WORD TestAttivo;		// 0x16
	WORD VetAnimMain[256];   // 0x18
	WORD VetAnimMirror[256]; // 0x58
	int VetFixX[256];  // valore da aggiungere a x per fixing
	int VetFixZ[256];  // valore da aggiungere a z per fixing

}RecordMirrorFields;

typedef struct StrOrient {
	short OrientV;
	short OrientH;
	short OrientR;
}OrientFields;

typedef struct BaseMirrors {
	WORD TotMirror;
	RecordMirror VetMirror[MAX_MIRRORS];
	WORD MirrorType;
	int  CordMirror;
	int MinCordMirror;
	int MaxCordMirror;
	int IndiceNow;			
	RecordMirror *pRecNow;
	DWORD CordX;
	int CordY;
	DWORD CordZ;
	short OrientH;
	short OrientV;
	short OrientR;
}BaseMirrorsFields;

typedef struct StrRecordClimbPush {
	WORD Indice;
	WORD TestClimb;
}RecordClimbPushFields;

typedef struct StrBaseClimbPush {
	WORD TotPush;
	StrRecordClimbPush VetPush[200];
}BaseClimbPushFields;

typedef struct StrBackupLara {
	WORD AnimationNow;
	WORD StateIdCurrent;
	WORD StateIdNext;
	WORD FrameNow;
	DWORD LaraX;
	int  LaraY;
	DWORD LaraZ;
	short LaraRoom;
	WORD Orient;
	short SpeedH;
	short SpeedV;
	DWORD CopiaOrientamento;

}BackupLaraFields;

// struttura per salvari tutti i mesh swap effettuati
typedef struct StrFlipSwapMesh {
	WORD TipoSwapMesh;  // se c'e il bit 0x4000 allora e' solo un copy
	short SlotItem; // se 0 allora e' swap per lara
	short SlotAltroSlot;
}FlipSwapMeshFields;



typedef struct StrMeshTr4 {
	short CenterX;		// 0x00
	short CenterY;		// 0x02
	short CenterZ;		// 0x04
	WORD SphereRadius;  // 0x06
	WORD Collision;		// 0x08
	short NVertici;		// 0x0A
}MeshTr4Fields;

typedef struct StrRollingPush {
	WORD Animation; //usata per pushing rollingball
	WORD AnimFallito; // usata quando lara non puo' spingere rolling
	WORD FrameStartPush; // frame da cui si inizia a spingere passivamente rolling
	WORD FrameStartActivation; // frame di quando viene attivato il rolling
	WORD Distance; // usata per calcolo distanza corretta da rolling
	WORD FrameInvulnerabile; // numero di frame senza ferire lara
	WORD SpeedPushing; // relativo a spostamento passivo
}RollingPushFields;

typedef struct StrBassDll {
	WORD Frequenza; // default 44100
	WORD TimeFadeOut; // default 1000
	WORD TimeFadeOutCorto; // default 300
	WORD Flags; // NSE_ ... flags
	WORD TipoEstensione;  // SEXT_ ... costanti

}BassDllFields;



typedef struct StrImportFile {
	WORD Id;
	DWORD Size;
	WORD Tipo;
	short NumeroFile;
	BYTE *pData;
}ImportFileFields;

typedef struct StrBaseImportFile {
	int TotFiles;
	StrImportFile VetFiles[MAX_IMPORT_FILES];
	short VetID[MAX_IMPORT_FILES*10];
}BaseImportFileFields;




// Dati specifci per ogni munizione
typedef struct StrCustAmmo {
	WORD Flags;  // AMMO_...   00
	short DamageNormale;	// 02
	short Pickup;           // 04
	short PickupRegalo;     // 06
	short DamageEsplosivo;  // 08
	short IdTriggerHitEnemy;// 0A
	short Speed;            // 0C
	short Gravity;          // 0E
	short IdAddEffect;      // 10
	short IdTriggerAtEnd;   // 12
	short Extra;            // 14

}CustAmmoFields;
typedef struct StrCatturaShot {
	WORD Secondi;
	WORD FrameGap;
	short QaulitaImage;
	bool TestInCorso;
}CatturaShotFields;
// contiene tutti i dati trovati nel comando script
typedef struct StrCustWeapon {
	WORD Flags;      // WEAP_...
	short SoundShot;
	BYTE FramesRicarica;
	BYTE SizeShell;
	WORD DistanceAiming;
	BYTE  FrameToTakeWeapon;
	BYTE  FrameToLetWeapon;
	WORD Dispersion;
	short Extra;
	short VPositionOfWeapon;
	WORD Unknow; // DatoArmaEstratta
	BYTE FrameCounter; // VetDatiOggettiMano[2]
	BYTE FrameMinRange; // VetDatiOggettiMano[3]
	BYTE FrameMaxRange; // VetDatiOggettiMano[5]
	short OrigX;  // origine
	short OrigY;  // di proiettili
	short OrigZ;  // visibili
	WORD Orient;

}CustWeaponFields;

typedef struct StrShowAmmoCounter {
	StrPrintString TexAmmoCounter;
	bool TestShowAmmoCounter;
	StrProgressiveAction AzioneTestoCounter;
	char Testo[80];
	WORD Flags;

}ShowAmmoCounterFields;

typedef struct StrCustCamera {
	WORD Flags;  // FCAM_ ...
	short ChaseDistance;
	short ChaseHeight;
	WORD ChaseHOrient;
	short CombatDistance;
	short CombatHeight;
	short LookDistance;
	short LookHeight;
	WORD Speed;
}CustCameraFields;


typedef struct StrInfoChar {
	float OrgX;			// 0x00
	float OrgY;			// 0x04
	WORD XSizeDestinazione;  // 0x08
	WORD YSizeDestinzione;			// 0x0A
	short BaseLine;		// 0x0C
	BYTE IndicePattern;		// 0x0E
	BYTE IndicePattern2;	// 0x0F
}InfoCharFields;				// size = 0x10

typedef struct StrFontParameter {
	DWORD Mask1;
	DWORD Mask2;
}FontParameterFields;

typedef struct StrRecordMask {
	StrFontParameter VetRighe[16];
}RecordMaskFields;

// salva i dati di temporizzazione di acluni oggetti
// particolari come i flame emitter
typedef struct StrTimerOggetti {
	WORD ItemIndex;
	WORD Campo34;
	WORD Campo36;
	WORD Campo38;
	WORD Campo3A;
}TimerOggettiFields;

typedef struct StrHeaderTimer {
	DWORD ContaQuindicesimi;
	WORD ValOscilla;
}HeaderTimerFields;

typedef struct StrBaseTimerOggetti {
	WORD TotOggetti;
	StrHeaderTimer Header;
	StrTimerOggetti VetOggetti[MAX_TIMER_OGGETTI];
}BaseTimerOggettiFields;

typedef struct StrAnimMorte {
	short Slot;
	short AnimIndex;
}AnimMorteFields;

typedef struct StrBaseAnimMorte {
	WORD TotCustAnimMorte;
	StrAnimMorte VetAnimMorte[MAX_ANIM_MORTE];
}BaseAnimMorteFields;

typedef struct StrPtrBars {
	DWORD *pColor1;
	DWORD *pColor2;
	union {
		int *pSizeX;
		int SizeX;
	};
	union {
		BYTE *pSizeY;
		int SizeY;
	};
	DWORD DefColor1; 
	DWORD DefColor2;  

}PtrBarsFields;

typedef struct StrBarraCust {
	short OrgX;   // 0x00
	short OrgY;   // 0x02
	WORD SizeX;   // 0x04
	BYTE SizeY;   // 0x06  
	WORD Flags;   // 0x07
	DWORD Color1;  // 0x09 (non piu' id ma colore rgb gia' espanso)
	DWORD Color2;  // 0x0D (non id ma colore)
	short Extra;  // 0x11 valore originale di script
	DWORD ExtraLong; // 0x13 valore riealborato se colore 3
	short OrgYUpdated; // 0x17  valore aggiornato in modo corretto
}BarraCustFields;  // 0x19

#define SIZE_RECORD_BAR  0x19

typedef struct StrCustInnerShot {
	WORD FlagQSF;
	bool TestAddInnerShot;
}CustInnerShotFields;

typedef struct StrCustLightItems {
	int Rosso;
	int Verde;
	int Blue;
	int Intensita;
	int Time;
}CustLightItemsFields;

typedef struct StrBaseHarpoon {
	bool TestArpione; // al posto di balestra
	WORD ArpioneFlags; // ARP_...
	WORD OrientX;
	WORD OrientY;
	WORD OrientZ;
	short Distance;
	short TopY;
	short HSpeed;
	WORD Gravity;
}BaseHarpoonFields;

typedef struct StrDartCustomize {
	short IdCustomize;    // 0x0000
	WORD Flags;			  // 0x0002   DRT_ ...
	short IdAddEffect;    // 0x0004
	short Speed;          // 0x0006
	short EmittingTimer;  // 0x0008
	COLORREF ColorMain;   // 0x000A
	COLORREF ColorSecondary; // 0x000E
	short IdTriggerGroup;  // 0x0012
}DartCustomizeFields;

typedef struct StrBaseDartCustomize {
	WORD TotCust;
	StrDartCustomize VetCust[MAX_DART_CUST];
}BaseDartCustomizeFields;

typedef struct StrCustFlare {
	WORD Flags;  // FFL_
	WORD Durata; // -1 = infinita
	WORD Rosso;
	WORD Verde;
	WORD Blu;
	WORD Intensita;
	DWORD Valore810;
	DWORD Valore876;
}CustFlareFields;


typedef struct StrStillCollision {
	WORD Flags; // COLL_...
	short LowerHeight;
	BYTE VetSlotStillCollisions[SLOT_NUMBER_OBJECTS];
}StillCollisionFields;
typedef struct StrRollBoatScript {
	WORD Slot;
	WORD Flags; // FRB_...
	short SpeedSwing;
	short SpeedPitch;
	short SoundSfx;
}RollBoatScriptFields;

typedef struct StrBaseRollBoatScript {
	WORD TotRollBoatScript;
	StrRollBoatScript VetRollBoatsScript[MAX_ROLL_BOAT_SCRIPT];

}BaseRollBoatScriptFields;

typedef struct StrLaraHp {
	short LastHpValue;
	WORD TotTargetLevels;
	short VetTargetLevels[MAX_TARGET_LEVELS];
}LaraHpFields;

typedef struct StrSlotVehicleCollide {
	int TotSlotCollide;
	WORD VetSlotCollide[MAX_SLOT_COLLIDE];
}SlotVehicleCollideFields;

typedef struct StrCustBinocular {
	bool TestPresente;  // when there is a Customize=CUST_BINOCULARS in current level
	WORD Flags;   // BINF_... flags
	short Parameter;   // option value that could work in according with some BINF_ flag
	short CompassImage;  // direct number, not image ID  (1024x768)
	short CompassRect;   // Id rect + BINT_...
	short SextantImage;  // direct number of image
	short SextantRect;    // Id rect+ BINT_...
	short LightnessRect;       // lightness rect id
	short ZoomRect;    // id rect + BINT_ for zoom-in factor
	short LighSwitchRect;  // zone of light switch and BINT_ type
	int FontId;   // id of font used for typing operation
}CustBinocularFields;


typedef struct StrCustSlotFlag {
	WORD Slot;
	WORD Flags; // FFS_... flags
}CustSlotFlagFields;

typedef struct StrBaseSlotFlags {
	int TotSlotFlags;
	StrCustSlotFlag VetSlotFlags[MAX_SLOT_FLAGS];
}BaseSlotFlagsFields;

typedef struct StrShaterCustSpecific {
	int TotSpecific;
	short VetSpecific[256];
}ShatterCustSpecificFields;

typedef struct StrBaseCustomize {
	bool TestDisabilitaDoppiaTesta; // def = false
	short SecretsAmount;			    // def = 70
	short CreditsLevel;				// def = 39
	bool TestDisableAnim96;
	WORD ShatterInizio;  // def = 0x32
	WORD ShatterFine;    // def = 0x3b (ma usare -1)
	StrRollingPush RollingBallPush;
	StrBassDll BassDll;
	StrLaraHp BaseHpLara;
	
	DWORD SlotChiaveJeep; // def= 175
	WORD SpeedMoving; // def=32 usata per velocita' movimgneot oggetto
	StrCustAmmo AmmoPistols;
	StrCustAmmo AmmoRevolver;
	StrCustAmmo AmmoFucileNormale;
	StrCustAmmo AmmoFucilePotenziate;
	StrCustAmmo AmmoUZI;
	StrCustAmmo AmmoBalestraNormali;
	StrCustAmmo AmmoBalestraEsplosive;
	StrCustAmmo AmmoBalestraVelenose;
	StrCustAmmo AmmoGranateNormali;
	StrCustAmmo AmmoGranateEsplosive;
	StrCustAmmo AmmoGranateLuminose;

	StrCustWeapon WeaponPistols;
	StrCustWeapon WeaponUzi;
	StrCustWeapon WeaponFucile;
	StrCustWeapon WeaponBalestra;
	StrCustWeapon WeaponLanciaGranate;
	StrCustWeapon WeaponRevolver;
	StrBaseInvItems BaseInvItems;
	StrShowAmmoCounter ShowAmmoCounter;
	DWORD TrasparenzaGlass;
	DWORD TrasparenzaIce;
	WORD DamageStatic;
	WORD PoisonStatic;
	bool TestNoTransparencyLara;
	WORD HairType;
	bool TestKeepNemiciMorti;
	bool TestOldCDTrigger;
	bool TestEscapeFlyCamera;
	WORD TastoEsciFlyCamera;
	bool TestPauseFlyCamera;
	WORD TastoPauseFlyCamera;
	bool TestPrintOnFlyby;
	bool TestNoTimeInSaveList;
	short TipoCDM; // CDM_...
	StrBaseAnimMorte BaseAddAnimMorte;
	StrBarraCust VetBar[BAR_CUST_TOT];
	WORD ParallelBarFlags; // PB_...
	WORD ParalledSpeedSlide; 
	WORD ParallelMaxTurns;
	DWORD BugsToFix;  // BUGF_...
	StrCustCamera CameraCust;
	bool TestDisableMissingSounds;
	StrCustInnerShot ScreenShotInner;
	WORD FlagsFMV;  // FMV_ ...
	bool TestFixWaterFogBug;
	bool TestSaveLocust;
	StrBaseHarpoon BaseHarpoon;

	StrCustLightItems AmberLight;
	StrCustLightItems WhiteLight;
	StrCustLightItems BlinkingLight;

	StrCatturaShot BaseCatturaShot;
	bool TestTr5Collisions;
	StrBaseDartCustomize BaseDartCustomization;
	StrCustFlare BaseFlare;
	BYTE VetTTColors[TT_MAX_TEXT_TYPES];
	StrStillCollision BaseStillCollision;
	char WaterfallSpeed;
	StrBaseRollBoatScript BaseRollScript;
	short VetCustSFX[MAX_CUST_SFX];
	short TitleFmv;  // -1 = no fmv
	bool TitleTestMultiply;
	StrSlotVehicleCollide BaseSlotCollideVehicles;
	bool TestDisablePushAway;
	StrCustBinocular CustBinoculars;
	StrBaseSlotFlags BaseSlotFlags;
	StrShaterCustSpecific BaseShatterSpecific;
}BaseCustomizeFields;

typedef struct StrRecordLocuste {
	BYTE Zona[30];
}RecordLocusteFields;

typedef struct StrBaseFlipSwap {

	WORD TotMeshSwap;
	StrFlipSwapMesh VetMeshSwap[MAX_SWAP_MESH];
}BaseFlipSwapFields;



// struttura Item per tomb4


typedef struct StrRectPos {
	DWORD MinX;
	DWORD MaxX;
	DWORD MinZ;
	DWORD MaxZ;
}RectPosFields;




typedef struct StrInfoAnimDoor {
	short Incremento;
	int CordChiusa;
	int CordAperta;
	WORD FlagsMov;
}InfoAnimDoorFields;

typedef struct StrSalvaOldDebug {
	WORD OldFlagsDgx;
	WORD OldCounter;
}SalvaOldDebugFields;

// struttura Item per tomb4
typedef struct StrItemTr4 {
	int HeightFloor;		// 00
	int ContactFlags;		// 04  (oggetto toccato lara (darts))
	DWORD MeshVisibilityMask; // 08
	WORD SlotID;			// 0C
	WORD StateIdCurrent;	    // 0E
	WORD StateIdNext;			// 10
	WORD StateIdAI;				// 12
	WORD AnimationNow;		    // 14
	WORD FrameNow;			    // 16
	short Room;					// 18
	short ItemIndexNext;	    // 1A
	short ItemIndexPrevious;     // 1C
	short SpeedH;			// 1E
	short SpeedV;			// 20
	short Health;				// 22
	WORD BoxZoneIndex;			// 24
	short ObjectTimer;			// 26
	WORD Objectbuttons;	// 28
	WORD Intensity1;			// 2a
	WORD OcbCode;				// 2c
	BYTE IdSprite1;				// 2e
	BYTE IdSprite2;				// 2f
	WORD Trasparency;			// 30
	WORD Mistery1;				// 32
	short Reserved_34;		    // 34
	short Reserved_36;				// 36
	short Reserved_38;				// 38
	short Reserved_3A;				// 3A
	void *pZonaSavegame;		// 3C	or pCreatureInfo structure for enemies				
	DWORD  CordX;					// 40
	int  CordY;					// 44
	DWORD  CordZ;					// 48
	short OrientationV;  // 4c
	short OrientationH;  // 4e
	short OrientationT;		// 50
	DWORD LightRed;		//52
	DWORD LightGreen;		// 56
	DWORD LightBlue;		// 5A
	DWORD LightRGB;		// 5E
	DWORD LightDifferenceRed;		// 62
	DWORD LightDifferenceGreen;		// 66
	DWORD LightDifferenceBlue;		// 6A
	DWORD LightChangeDegrees;		// 6E
	BYTE Zone1[2730];		// 72
	BYTE Zone2[2758];		// B1C
	BYTE *pZone1;		// 15e2
	BYTE *pZone2;		// 15e6
	DWORD FlagsMain;		// 15ea (FITEM_.. flags)
	DWORD FlagsSwapMesh;	// 15ee
	DWORD Mistery5;		// 15f2
}ItemFields;   // size 0x15f6


typedef struct StrScriptElevator {
	WORD IndiceElevatore;
	int  FirstFloorY;   // cordy originale di ascensore e primo piano.
	WORD ClickDistance;
	WORD TotFloors;
	WORD Flags;  // EF_... flags
	WORD IndexFirstDoor;
	WORD IndexFirstDoorSecodnary;
	WORD KeyPadIndex;
	WORD ImagePrompt;
	WORD TotDoors;
	StrInfoAnimDoor AnimDoor;
	StrInfoAnimDoor AnimDoorSecondary;
	WORD VetDoors[MAX_ELEVATOR_DOORS];
	WORD VetDoorsSecondary[MAX_ELEVATOR_DOORS];
	WORD TotFrameItems;
	WORD VetFrameItems[MAX_FRAME_ITEMS];
	StrRectPos LimitiBloccoKeypad;
	WORD OrientLaraPerKeyPad;
	WORD Speed;
	StrItemTr4 *pItem;
	int MaxYFloor;
	short MaxRoom;

}ScriptElevatorFields;

// struttura con dati dinamici di ascensore da salvare in savegame
typedef struct StrElevator {
	int  IncY;  // praticamente velocita', positiva o negativa
	int OrgYTarget; // coordinata y da raggiungere 
	BYTE Status;  // EST_ .. in attesa, in movimento, in pausa, in partenza
	BYTE FloorNow; // 0 = primo piano
	BYTE FloorTarget; // a che piano si  sta andando
	int Soffitto;  // zero o 0x800 a secona se collisione sul soffitto o meno
	BYTE NextStatus;  // prossima cosa da fare, usto solo per delay
	BYTE Delay;  // numero di click di attesa per fase delay
	short LastIncy; // escludendo assestamento
	BYTE StoppedStatus; // valore di status nel momento in cui e' stato stoppato
	int FirstFloorY; // coordinata y salvata in savegame
	int VetCordYFrame[30]; // cordy di tutti i frame
	int CordYElevator; // per salvare in savegame cordy attuale
	StrInfoAnimDoor SalvaAnimDoor; // non piu' usata
}AscensoreFields;

typedef struct StrBaseElevator {
	WORD TotElelevators;
	StrScriptElevator VetScriptElevators[MAX_ELEVATORS];
	StrElevator VetAscensori[MAX_ELEVATORS];
	int GapSoffittoY;
}BaseElevatorFields;

// setting (eventuali) per keypad impostate da comando script keypad=
typedef struct StrKeyPadScript {
	WORD IndexFirstAnim;
	WORD FrameFirstAnim;
	WORD IndexAfterAnim;
	WORD ClickSound;
}KeyPadScriptFields;

typedef struct StrKeyPad {
	bool TestAttivo;
	int  Fase;  // FKP  valori
	int IndiceTasto;
	int IndiceTastoOld;
	WORD Slot;
	DWORD LastFrameKey;
	DWORD LastFrameExit;
	int CordYAnimazione;
	bool TestAttivaTriggerSwitch;
	int IncYAnimazione;
	DWORD OldTastiBloccati;
	int TipoKeyPad;
	int ValoreInserito;
	int TotInseriti;
	int ValoreAtteso; // solo per switch
	int *pStartMeshTree;
	int IndiceKeypad; // indice di item switch
	StrKeyPadScript ScriptSettings;
	DWORD LaraX;
	DWORD LaraZ;
	char BufOldText[10];
	BYTE VetInseriti[4];
}KeyPadFields;

typedef struct InventarioItem {
	WORD Slot;		// 0x00
	short TopY;     // 0x02
	WORD Distanza;  // 0x04
	WORD OrientX;   // 0x06
	WORD OrientY;   // 0x08
	WORD OrientZ;   // 0x0A
	WORD Flags;     // 0x0C
	WORD Speed;		// 0x0E
	short Mistero1; // 0x10
	short Mistero2; // 0x12
}InventarioItemFields;


// informazioni per animazioni di palline colorate sul detector
// in modalita' radar
typedef struct StrTargetDetector {
	short Fase;  // FTR_ 0= nascita / 1 = lampeggio fisso / -1 non attivo
	short DifY;  // distanza da floor sotto lara 
	WORD  Orient; // orient rispetto a lara nord
	short DifX;   // distanza su cord x in metri da lara 
	short DifZ;   // distanza su cordz
	int Distanza3d; // distanza 3d in metri (valore assoluto)
	short VPos;  // 0 = piano di lara, +1 = sopra lara, -1 = sotto lara 
	short Incremento; // usato per lampeggio fisso
	short DurataFase; // GlobTomb4.Adr.VetTexInfomentare e azionare prossima fase solo a 0
	short DurataTotale; // click ancora da eseguire con target visibile
	WORD Frame; // numero di sprite mostrato in ultima operazione
}TargetDetectorFields;

typedef struct StrDetector {
	bool TestMostra;  // se mostrato sullo schermo
	bool TestAttivo; // se impostato in script
	int Distanza; // distanza in metri 
	int DistanzaY; // minore di zero se target e' piu' in alto
	WORD OrientLast; // ultimo orientamento impostato (magari diverso per oscillazione)
	WORD OrientRealLast;  // orientamento effettivo di ago
	WORD OrientRealNow;  // orientamento effetivo attuale
	WORD ScalaMetrica; // per ogni segno
	DWORD LastFrame; // dipressione f8
	short IncOscillazione;
	WORD RangeMetri; // mostrarlo quando si e' entro i...metri
	WORD MaxDifVMetri; 
	WORD MaxDifHMetri;
	WORD Indice;
	WORD SlotDetector;
	WORD Flags;  // dtf_ flags
	WORD TotIndici;
	WORD VetIndici[200];
	StrTargetDetector VetTargets[200];
}DetectorFields;

typedef struct StrSuoni {
	int RubberBoatRun;
	int RubberBoatFolle;
	int MotorBoatRun;
	int MotorBoatFolle;
	int ElevatorMove;
	int ElevatorStop;
	int DetectorBeep;
	int Rain[4];
}SuoniFields;


// record per ogni valore da modificare per cambiare danno che nemici
// trappole arrecano a lara
typedef struct StrCambiaDamage {
	int Slot; // numero di slot a cui corrisponde questo damage
	DWORD pOffset;  // puntatore
	char Tipo;  // lettera 'B' 'W' o 'D' per segnalare dimensione 
	int ValoreOriginale;  // da usare per ripristino

}CambiaDamageFields;


// struttura singolo record animation
// old structure
/*
typedef struct StrAnimationTr4 {
	DWORD FrameOffset;		// 0x00
	BYTE  FrameRate;		// 0x04
	BYTE  FrameSize;		// 0x05
	WORD  StateId;			// 0x06
	WORD  Mistery1;			// 0x08
	WORD  Speed;			// 0x0A
	WORD  AccelLO;			// 0x0C
	WORD  AccelHI;			// 0x0e
	WORD  Mistery2;			// 0x10
	WORD  Mistery_Speed;	// 0x12
	WORD  Mistery_AccelLO;	// 0x14
	WORD  Mistery_AccelHI;	// 0x16
	WORD  FrameStart;		// 0x18
	WORD  FrameEnd;			// 0x1A
	WORD  NextAnimation;	// 0x1C
	WORD  NextFrame;		// 0x1E
	WORD  NumStateChanges;	// 0x20
	WORD  StateChangeOffset; // 0x22
	WORD  NunAnimCommands;	// 0x24
	WORD  AnimCommand;		// 0x26
}AnimationTr4Fields;  // size 0x28
*/

// struttura singolo record animation
typedef struct StrAnimationTr4 {
	DWORD FrameOffset;		// 0x00
	BYTE  FrameRate;		// 0x04
	BYTE  FrameSize;		// 0x05
	WORD  StateId;			// 0x06
	int   Speed;			// 0x08  (speed multiplied by 65536)
	int   Accel;			// 0x0C  (accel multiplied by 65536)
	int   SpeedSide;		// 0x10  (speed multiplied by 65536)
	int   AccelSide;		// 0x14  (accel multiplied by 65536)
	WORD  FrameStart;		// 0x18
	WORD  FrameEnd;			// 0x1A
	WORD  NextAnimation;	// 0x1C
	WORD  NextFrame;		// 0x1E
	WORD  NumStateChanges;	// 0x20
	WORD  StateChangeOffset; // 0x22
	WORD  NunAnimCommands;	// 0x24
	WORD  AnimCommand;		// 0x26
}AnimationTr4Fields;  // size 0x28

typedef struct structStrDoorTr4 {
	WORD IndiceRoom;
	short VisiblePoint[3];
	short VetCorners[12];
}uctStrDoorTr4Fields;

typedef struct StrBaseDoors {
	WORD TotDoors;
	structStrDoorTr4 VetDoors[256];
}BaseDoorsFields;

// struttura mesh info che in pratica e'
// struttura per oggetti statici
typedef struct StrMeshInfo {
	DWORD  x;			// 0
	int    y;			// 4
	DWORD  z;			// 8
	WORD  Orient;		// C
	WORD Color;	// E
	WORD OCB;	// 10 OCB
	WORD SlotId;		// 12
}StaticFields;

typedef struct StrCollisionAbsBox {
	int MinX;
	int MaxX;
	int MinY;
	int MaxY;
	int MinZ;
	int MaxZ;
}CollisionAbsBoxFields;

typedef struct StrBaseCollisione {
	bool TestCollideStatic;
	bool TestStillCollision;
	bool TestSbattiMuro;
	StrMeshInfo *pMesh;
	WORD RoomStaticObject;
	StrCollisionAbsBox TempColBox;
	StrCollisionAbsBox BoxAbsStatic;
	StrCollisionAbsBox BoxAbsMoveable;
	WORD OrientMoveable;
	WORD SlotMoveable;
	DWORD OrgXMoveable;
	DWORD OrgZMoveable;
	int OldLaraX;
	int OldLaraZ;
	DWORD OldFrame;
	WORD OldAnimazione;
	WORD OldStateId;

}BaseCollisioneFields;
typedef struct StrMioPuntoInt {
	int x;
	int z;
}MioPuntoIntFields;

typedef struct StrRettangolo {
	StrMioPuntoInt VetVertici[4];

}RettangoloFields;

typedef struct StrMyRect {
	short Id;
	short OrgX;
	short OrgY;
	WORD SizeX;
	WORD SizeY;
	short ForeColor;
	short BackColor;
	WORD Dynamic;
}MyRectFields;

typedef struct StrBaseMyRect {
	int TotMyRect;
	StrMyRect VetMyRect[MAX_MY_RECT];
	short VetId[MAX_MY_RECT*10];
}BaseMyRectFields;

typedef struct StrMyInputBox {
	short IdInputBox;
	short IdImage;
	short IdWFont;
	WORD MaxChars;
	short SfxSound;
	WORD RIB_Flags; // FIB_ flags, in the script
	int ExtraParam; 
}InputBoxFields;

typedef struct StrExtraCode {
	BYTE  VetScanCodes[3];
	BYTE  ValAscii;
	WORD TotScanCodes;
}ExtraCodeFields;

typedef struct StrBaseInputBox {
	int TotInputBox;
	StrMyInputBox VetInputBox[MAX_INPUT_BOX];
	StrMyInputBox *pInputBoxNow; // pointer to current Input Box
	int TotExtraCodes;
	StrExtraCode VetExtraCodes[MAX_EXTRA_SCAN_CODES];
	short VetId[MAX_INPUT_BOX*10];
}BaseInputBoxFields;

typedef struct StrSwapAnim {
	short Id;
	WORD FirstSourceAnim;
	WORD FirstTargetAnim;
	short NumberOfAim;
	WORD Dynamic;
}SwapAnimFields;

typedef struct StrBaseSwapAnim {
	int TotSwapAnim;
	StrSwapAnim VetSwapAnim[MAX_SWAP_ANIM];
	short VetId[MAX_SWAP_ANIM*10];
}BaseSwapAnimFields;

typedef struct StrSpeechActor {
	short IdSpeech;
	WORD Dynamic; // if different by 0 it has been allocated dynamically and this number is the id of plugin
	WORD Flags;
	WORD Parameter;
	short FrameRate;
	WORD SpeechSlot;
	WORD HeadSlotMesh;
	WORD FirstSpeechMesh;
	WORD TotSpeechMesh;
	int TotCommands;
	WORD VetCommands[MAX_SPEECH_COMMANDS];
}SpeechActorFields;

typedef struct StrTextActors {
	WORD Colore;
	WORD Posizione;
}TextActorsFields;

// variabili globali per mantenenere effetto speech 
typedef struct StrPlaySpeech {
	bool TestAttivo;
	WORD Flags;  // SPCF_ flags
	int TotSpeech;   // numero di mesh usate per apertura bocca
 	int ItemIndex;
	int IndexMeshActorHead;
	int IndexFirstMeshSpeech;  // slot dove sono contenute le facce
	int IndexLastString; // se 0 vuol dire che non c'e' alcuna stringa
	StrTextActors TextSettings;
	WORD SlotActor;
	StrSpeechActor *pParam;
	WORD IndexCommandNow;
	DWORD LastFramePerformed; // to avoid to repeat performing with same frame
	bool TestAbs;  // se mesh in SpeechIndexNow e' di tipo assoluto (comando MESH o comando SEQUENCE) = true

	int IndiceSecondarioSpeech; // da 0 incrementato di uno dopo ogni fase
	int IndiceSecondarioRotateH; // da 0 a 1 tipo fase 
	int IndiceSecondarioRotateV;  // da 0 a 1 tipo fase: muovi tieni fermo in quella posizione
	int FrameRateSpeech;  // impostato da utente o uguale a framerate standard

	bool TestLara;
	bool TestPause;

	DWORD RemovedMeshAdr; // mesh adr presente per testa prima della modifica (da ripristinare)

	int SpeechIndexNow;    // indice mesh impostata come testa attualmente
	int SpeechIndexInc;
	int SpeechTotSequence; // numero di diverse teste da cambiare nel corso della sequenza
	int SpeechTotSyll; // totale syllable, movimento da apri e chiudi o viceversa
	DWORD FrameEndSpeech; // frame when complete current speech command

	int  OrientHTurnNow;
	int	 OrientHTurnInc;  	
	int  OrientHTurnEnd;
	int OrientHLastCommand;
	int OrientHTimes; 
	DWORD FrameEndHTurn;  // frame when complete current horizontal turn head (shake)

	int  OrientVTurnNow;
	int  OrientVTurnInc;
	int OrientVTurnEnd;
	int OrientVLastCommand;
	int OrientVTimes;
	DWORD FrameEndVTurn;  // frame when complete current vertical turn head (nod)
	DWORD FrameEndLoop; // continuare ad eseguire finche' non si raggiungono i frame finali

}PlaySpeechFields;



typedef struct StrBaseSpeechActor {
	int TotSpeechActor;
	StrSpeechActor VetSpeechActor[MAX_SPEECH_PARAM];
	short VetId[MAX_SPEECH_PARAM*10];
	StrPlaySpeech VetPlay[MAX_ACTOR_SPEECHING];
}BaseSpeechActorFields;



// size = 0x1c
typedef struct StrStaticSlot {
	WORD IndiceMesh;   // 00
	WORD SlotStatic;   // 02
	StrBoxCollisione ViewBox;  // 04
	StrBoxCollisione CollisionBox;  // 10
}StaticSlotFields;

typedef struct StrAdaptiveFarView{
	float VetLastFps[3];
	float BlocchiFar;
	float MaxFarView;
	float Incremento;
	float FPStoKeep;
	DWORD LastTime; // per calcolare il passaggio di 1 secondo
}AdaptiveFarViewFields;



typedef struct StrSalvaCapelli {
	StrPosizione Posizione;   // offset 0
	short OrientV;        // offset 0C
	short OrientH;		  // offset 0E
}SalvaCapelliFields;

typedef struct StrPos3d {
	StrPosizione Posizione;   // offset 0
	short OrientV;        // offset 0C
	short OrientH;		  // offset 0E
	short OrientR;		  // offset 10
}Pos3dFields;

typedef struct StrCercaStatic {
	WORD IndiceRoom;
	short IndiceStatic; // interno a stanza
}CercaStaticFields;

typedef struct StrSalvaStatic {
	StrCercaStatic Indici;
	WORD Flags;
	int OrgX;
	int OrgY;
	int OrgZ;
	WORD Orient;
	WORD Colore;
}SalvaStaticFields;

typedef struct StrBaseSalvaStatic {
	WORD TotStatics;
	StrSalvaStatic VetStatics[4096];
}BaseSalvaStaticFields;

typedef struct Tipo_CollGridTr4 {
	WORD FloorDataIndex;   // 00
	WORD BoxZoneIndex;     // 02
	char BelowRoomIndex;   // 04
	char FloorHeight;	   // 05
	char AboveRoomIndex;   // 06
	char CeilingHeight;		// 07
	
}CollGridTr4Fields;

// record di 7FE890h    ;Ptr_VetBoxInfos
typedef struct StrBoxZones {
	BYTE x1;		// 0x00
	BYTE x2;		// 0x01
	BYTE z1;		// 0x02
	BYTE z2;		// 0x03
	WORD Flags;		// 0x04
	WORD OverlapIndex;  // 0x06
}BoxZonesFields;	// size = 0x08


// struttura verticii puntati da room stanza
typedef struct Tipo_VerticeRoom {
	float x;
	float y;
	float z;
}VerticeRoomFields;

// struttura per vertici d3d 
typedef struct Tipo_VerticeD3D {
	float x;		// 0X00
	float y;		// 0X04
	float z;		// 0x08
	DWORD Reserved;	// 0x0C
	DWORD Color;	// 0x10
	DWORD Specular;	// 0x14
	float tu;		// 0x18
	float TV;		// 0x1c
}VerticeD3DFields;


// structure for tr4 room light
typedef struct StrLigthTr4 {
	DWORD OrigX;    // 00
	int OrigY;      // 04
	DWORD OrigZ;    // 08
	BYTE ColorRed;   // 0c
	BYTE ColorGreen; // 0d
	BYTE ColorBlue;  // 0e
	BYTE LightType;  // D3D_ values
	BYTE lgh_Mistery1;  // usually  255  (0xFF)
	BYTE Intensity;   // 11
	float LightIn;    // 12
	float LightOut;   // 16
	float LightLenght;  // 1A
	float LightCutOff;  // 1e
	float TargetX;   // targets are not absolute coordinates but they seem degrees 
	float TargetY;  // 26
	float TargetZ;  // 2a
}LightTr4Fields;



// strutture room per tr4 (size 0x94)
typedef struct StrRoomTr4 {
	void *pStaticDataRoom;   // 00
	StrBaseDoors *pDoors; 	// 04
	Tipo_CollGridTr4 *pCollsionSectors; // 08
	StrLigthTr4 *pLights;           // 0c
	StrMeshInfo *Ptr_StaticMesh;  // 10
	int  OriginX;                 // 14
	int  OrgYMistery;           // 18 roba per superfice acqua
	int  OriginZ;                 // 1C
	int  OrigYBottom;           // 20
	int  OrigYTop;              // 24
	WORD Z_SizeSectors;         // 28
	WORD X_SizeSectors;         // 2A
	DWORD ColorIntensityLight;  // 2C
	WORD TotLights;             // 30
	WORD TotStaticMesh;         // 32
	BYTE WaterIntensity;		// 34
	char FlipMapIndex;    // 35
	char IndexTabWater;			// 36
	BYTE TestFlagsBound;        // 37
	short  SizeXScreenOther;           // 38 
	short rm_Mistery3A;			// 3A
	short  SizeYScreenOther;		    // 3C
	short rm_Mistery3E;			// 3E
	short  rm_Mistery40;			// 40
	short SizeXScreen;			// 42
	short  rm_Mistery44; 			// 44
	short  SizeYScreen;		// 46
	short FirstItemIndex;		// 48
	short FirstEffect;		// 4A
	short  AlternateRoom;		// 4C  -1 se non c'e' o e' stanza flippata?
	WORD  FlagsRoom;		    // 4E
	int  VerticesAmount;			// 50
	int  VerticesWaterAmount;				// 54  solo centrali e mobili
	int  VerticesDryAmount;				// 58  numero dei vertici non acqua
	void* pDirect3dVertexBuffer; // 5C  vertici passati per creare la stanza
	void* pRectangles;			// 60
	float floatOrigX;			// 64
	float floatOrigY;			// 68
	float floatOrigZ; 			// 6C
	int VerticesOffset;				// 70 accede a vetverticifloat per inizio vertici di qualche tipo
	void *pTriAndQuads;		// 74
	void *pQuadVertices;  //78
	void *pTriVertices;  // 7C
	int  rm_Mistero80; 			// 80
	Tipo_VerticeRoom *pVetVerticiFloat;    // 84
	int  TrianglesAmount; 			// 88
	int  RectanglesAmount;			// 8C
	void *pLightObjects;		// 90  directx object created with light data
}RoomFields;



typedef struct Tipo_ErroriEccezioni {
	DWORD Codice;
	char *pNomeFlag;
	char *pDescrizione;
}ErroriEccezioniFields;

// struttura per header sound sample (usata in tomb5 per exporting)
typedef struct Tipo_HeaderSoundSFX {
	DWORD MexRIFF;	// "RIFF"
	DWORD SizeFile; // meno 8
	DWORD MexWAVE; // "WAVE"
	DWORD Mexfmt;  // "fmt "
	DWORD Size16; // 16
	WORD IdValueUno; // 1
	WORD NChannels;  // 1
	DWORD SamplePerSec; // 22050
	DWORD AverageBytes; // 44100
	WORD BlockAlign;  // 2
	WORD Bits; // 16
	DWORD Mexdata; // "data"
	DWORD SizeCampioni; // dimensioni dei dati audio
}HeaderSoundSFXFields;

// struttura di soundinfos
typedef struct StrSoundInfos {
	short Indice;
	BYTE Volume;
	BYTE RAD;   // valore originale
	BYTE CH;    // totale con 100 = 255
	char PIT;   // valori positivi o negativi con massimi (+/) 128 
	WORD Flags;  // FSI_
}SoundInfosFields;

typedef struct StrEndNgHeader {
	DWORD EndCheck;  // it should be number 0x454C474E ( "NGLE")
	DWORD SizeNgHeader; // size of whole extra ng header
}EndNgHeaderFields;

typedef struct GAME_VECTOR {
	DWORD CordX;
	int CordY;
	DWORD CordZ;
	short Room;
}GAME_VECTORFields;

typedef struct StrAdrCamera {

	StrCameraTr4 *pVetCamera;
	short *pIndexCameraNext;
	short *pIndexCameraNow;
	StrItemTr4 **pTargetCameraNext;	
	StrItemTr4 **pTargetCameraNow;
	GAME_VECTOR *pCameraSrc;
	GAME_VECTOR *pCameraTarget;

	short *pSecondsTimerCamera;
	WORD *pButtonTriggerCamera;
	int *pModeCameraNext;
	int *pModeCameraNow;
	int *pTotCameras;
}AdrCameraFields;


typedef struct StrAdrFlipMap {
	int *pVetEnabledFlipMaps;
	int *pVetButtonFlipMaps;
}AdrFlipMapFields;

typedef struct StrCustDatiOggettoMano {
	WORD SlotAnim;			// 0x00
	BYTE FrameCounter;		// 0x02
	BYTE FrameMinRange;		// 0x03
	BYTE FrameChangeArma;  // 0x04
	BYTE FrameMaxRange;   // 0x05
}CustDatiOggettoManoFields;

typedef struct StrEnemiesNotAimable {
	int TotSlot;
	WORD VetSlotNonPuntabili[MAX_ITEMS];
}NemiciNonPuntabiliFields;

typedef struct StrBaseHandle {
	HANDLE hThread;
	HANDLE hProcess;
}BaseHandleFields;

typedef struct StrBaseFPS {
	DWORD VetTempi[MAX_TEMPI_FPS];
	DWORD VetFrameCount[MAX_TEMPI_FPS];
	int IndiceNow;
	int LastIndice;
	int LastFps;
	DWORD TotCicli; // < o = MAX_TEMPI_FPS per sapere quanti sono disponibili
	float FPS;

}BaseFPSFields;

typedef struct StrRecordImage {
	HDC MemHdc;  // hdc compatibile con gia' inserita l'immagine
	HANDLE hBitMap; // handle del bitmap
	HANDLE hOldBitMap; // handle del bitmap che era nell'hdc
	DWORD SizeX; // dimensione x
	DWORD SizeY; // dimensione y
	int NImage;
	bool TestUsata; // true ci sono dati singiicativi
	bool TestPreload; // immagine in preload, non scartalra finco a chiusura livello
}RecordImageFields;

typedef struct StrPopUp {
	WORD NImage;
	RECT Zona;
	WORD Contatore;
	WORD IdImageCmd;
	bool TestTrasparente;
	bool TestSopraFlyBy;
	bool TestSopraFixed;
}PopUpFields;

typedef struct RectFloat {
	float OrgX;
	float OrgY;
	float SizeX;
	float SizeY;
}RectFloatFields;

typedef struct StrEffettoImage {
	RectFloat Inizio;
	RectFloat Fine;
	RectFloat Inc;
	int TotFrames;
	bool TestAttivo;
	StrRecordImage  EffectImage;
}EffettoImageFields;

#define TOT_IMAGES 12

typedef struct StrShowImage {
	StrRecordImage ImageBackGround;      // 0
	StrRecordImage ImageLittle;          // 1
	StrRecordImage ImageOverlap;         // 2
	StrRecordImage Temp;                 // 3
	StrRecordImage ImageMonoScreen;      // 4
	StrRecordImage ImageLoadLevel;       // 5
	StrRecordImage ImageTitle;           // 6
	StrRecordImage ImageBinocular;		// 7
	StrRecordImage ImageLaserSight;     // 8
	StrRecordImage ImagePageDiario;     // 9
	StrRecordImage ImageBinocCompass;   // 10
	StrRecordImage ImageBinocSextant;   // 11
	StrRecordImage VetImages[MAX_IMAGE_RECORDS];
	bool TestTempHdc;
	RECT ZonaImageLittle;
	HDC HdcTomb;
	bool TestPopUp;
	StrPopUp PopUp;
	bool TestWideScreen;
	RECT ZonaSchermoTomb;
	bool TestFullScreen;
	bool TestTombAllocato;
	bool TestHdcBack;
	bool TestWriteHdc;
	StrEffettoImage Effetto;
}ShowImageFields;


typedef struct StrSalvaGlobFloor {
	int InsideCondition;
	int InsideDummyTrigger;
	DWORD FloorDataNow;
	int TipoInclinatura;
	int InclinaturaX;
	int InclinaturaZ;
	int Reserved[100]; // to change when I add fields to the structure
}SalvaGlobFloorFields;

typedef struct StrWindowsFont {
	HFONT hFont;
	HFONT hFontOld;
	WORD IdFont;
	WORD FlagsWFF;
	int CharSet;
	HDC HdcConFont;
	int OutSizePixel; // ottenuto da calcolo proporzionale
	COLORREF ColoreText;
	COLORREF ColoreShadow;
	short SizeFont;
	char NomeFont[80];
}WindowsFontFields;

// struttura per pagina diario
typedef struct StrComPage {
	char *pTesto; // allocato dinamicamente  (lista savegames per panello)
	char *pTitolo; // allocato dinamicamente (non usata in pannello savegame)
	char *pInfoSave; // allocato dinamicamente, usata solo da panello savegame
	RECT FrameText1;  // se saveagame panel lista savegame 
	RECT FrameText2;  // se savegame panel info text
	RECT FrameTitle;  // solo in savegame panel
	RECT FrameImg;
	WORD FlagsPL; // page layout
	short BgImage; // numero di immagine da usare come sfondo
	short NImage;  // o -1 se assente
	short CbBack; // suono cd di fondo (-1 niente)
	bool TestFrameText2; // true se c'e' anche il secondo text frame
	StrWindowsFont FontTitle;  // non usati in savegame panel
	StrWindowsFont FontText;  // non usati in savegame panel
	short NumeroPaginaNow;

}ComPageFields;



typedef struct StrBaseDiario {
	short ID_Diario;
	WORD TotStringhe;
	WORD VetStringhe[MAX_STRINGHE_DIARIO]; // indici ng
	int SizeTestoDiario;
	char* pTestoDiario; 
	StrWindowsFont FontTitle;
	StrWindowsFont FontText;
	StrWindowsFont FontInfoSave; // usato solo per panello savegame
	int IndicePaginaToShow;  // questo in panello save significa
							// indice savegame attualmete selezionato
	bool TestWideScreen; // ci sono premesse per fix widescreen
	WORD TotPagine; // calcolato in modo dinamico
	WORD FlagsPL; // page layout
	WORD FlagsLDF;  // flag LDF per diario
	WORD SlotDiario; // numero slot che azionera' diario in inventario
	WORD IdImage; // background image
	StrComPage PaginaNow;
	short CdIsPlaying; // suono cd di canale 1 che si sta suonando
}BaseDiarioFields;

typedef struct StrTempWindowsFont
{
	HFONT hFontInTombHdc; 
	StrWindowsFont FontNow;
}TempWindowsFontFields;

typedef struct StrBaseWindowsFonts {
	int TotFonts;
	StrWindowsFont VetFonts[MAX_FONTS];
	short VetID[MAX_FONTS*10];
	bool TestUsaWindowsFont; 
	StrWindowsFont DefWindowsFont; // this is the windowsfont for all prints 
	WORD FlagDWF;
	StrTempWindowsFont TempFont;  // used by current single print string 
	short VetOffsetPosY[POFF_COUNTER]; // changes for y position of system menus
	short LineSpacing;  // value to modify height of character row
}BaseWindowsFontsFields;

typedef struct StrListaFiles {
	char Testo[80];  // era 64
}ListaFilesFields;

typedef struct StrListaWav {
	char Testo[64];
}ListaWavFields;

typedef struct StrRecordRigaConst {
	int Numero;
	char *pDescrizione;
	char *pMexRemark;  // puntatore a sola parte di eventuale #remark# (senza codice #remark#)
	char *pMexHelp;     //  puntatore a sola di parte di eventuale testo #START_DOC# (senza codici inziialei e finali)

}RecordRigaConstFields;

typedef struct StrRecordSezione {
	int TipoSezione;  // valori SEZ_...
	int NumeroSezione;  // codice trigger relativo a sezione
	int ListPredefinito;  // 0 = no, altrimenti tipo di costante LST_
	int TipoListaShow;  // dove copiare lista SHOW_IN.. valori
	int TotValori;
	WORD Flags;  // flag FTL_
	StrRecordRigaConst *pVetValori;
}RecordSezioneFields;

typedef struct StrNGConstants {
	int TotSezioni;
	StrRecordSezione *pVetSezioni;
	int IndiceSezTesti; // indice a pvetsezioni per testi
}NGConstantsFields;

typedef struct StrDatoInventario {
	WORD Slot;		// 0x00
	short OffsetY;       // 0x02
	WORD Distance;    // 0x04
	WORD OrientX;      // 0x06
	WORD OrientY;      // 0x08
	WORD OrientZ;      // 0x0A
	WORD Flags;    	// 0x0C
	WORD IndiceStringa;    // 0x0E
	int  Mistero;    // 0x10 passato a drawinventoryitemme
}DatoInventarioFields;

// struttura per record pesci (e simile a quella di scarabei
typedef struct StrFish {
	int CordX;		// 00
	int   CordY;		// 04
	int CordZ;		// 08
	short OrientV;		// 0C
	short OrientH;		// 0E
	BYTE IndicePivot;	// 10  (era OrientR)
	BYTE Tempo;			// 11 (Aggressivo=0x80 / MeshTipo 0x60 / TempoFase 0x1f
	BYTE Room;			// 12
	BYTE FlagFish;      // 13
	short SpeedH;	// 14
	short SpeedV;	// 16
	BYTE TipoUsato;		// 18  0=no, 1=scrable, 2 = fish
	BYTE TipoFase;		// 19 status attuale
}FishFields;

typedef struct StrSalvaFish {
	WORD CordX;  // diviso 2
	short CordY;   // diviso 2
	WORD CordZ;   // diviso 2
	short OrientH;
	short OrientV;
	BYTE IndicePivot;
	BYTE IndiceNow;  // indice originario per questo fish
	BYTE Room;
	BYTE FlagFish;
	short SpeedH;
	char SpeedV;
	BYTE TipoFase;
}SalvaFishFields;

// structure inside tomb raider about sfx infos
typedef struct StrZonaSound {
	DWORD  Volume;
	DWORD  FrequenzaHigh;    // <= 32767
	DWORD  CameraAngle;      // (horient?)
	DWORD  FrequenzaLow;     // >6000
	int    IndiceRel;      // (restituito da VetSoundIndices[SfxIndex] )
	DWORD  CameraAngle2;      // (sembra uguale a cameraAngle)
	DWORD  CordX;
	int    CordY;
	DWORD  CordZ;
}ZonaSoundFields;

typedef struct StrCombine {
	void *pProcedura;
	WORD PrimoItem;
	WORD SecondoItem;
	WORD ItemFinale;
}CombineFields;
typedef struct StrDatiCollSettore {
	int FloorDist;
	int CeilingDist;
	int Inclinatura;
}DatiCollSettoreFields;

// super mega struttura per dati collisione
typedef struct StrCollisionLara {
	StrDatiCollSettore  VetInfoSettori[6];  // 0x000
	int LaraSizeX;		// 0x048    
	int LaraBottomY;		// 0x04C  
	int LaraTopY;			// 0x050   
	int LaraSizeZ;			// 0x054 
	
	int GridShiftX;			// 0x058 ??
	int GridShiftY;			// 0x05C  ??
	int GridShiftZ;			// 0x060  ??

	int LaraOldX;		// 0x064
	int LaraOldY;		// 0x068
	int LaraOldZ;		// 0x06c

	WORD StateIdOld;		// 0x070
	WORD AnimNowOld;		// 0x072
	WORD FrameNowOld;		// 0x074  
	WORD OrientHOld;		// 0x076
	WORD Direzione;		// 0x078
	WORD EsitoColl;		// 0x07A  1=hang bordo
	WORD *pFloorData;	// 0x07C
	char ClickSlopeAlfa;		// 0x080
	char ClickSlopeBeta;  // 0x81
	BYTE Flag1;		// 0x082
	BYTE TestBlocca;  // 0x83
	WORD Flags2;		// 0x084   ;0x5 = climb girando angolo
	
}CollisionLaraFields;

typedef struct StrFlyByTr4 {
	DWORD OrgX;				// 0x00
	int OrgY;				// 0x04
	DWORD OrgZ;				// 0x08
	DWORD TargetX;			// 0x0C
	int TargetY;			// 0x10
	DWORD TargetZ;			// 0x14
	BYTE Seq;				// 0x18
	BYTE Num;				// 0x19
	WORD FrameOffset;		// 0x1A
	WORD Mistery1;			// 0x1C
	WORD Mistery2;			// 0x1E
	WORD Speed;				// 0x20
	WORD CameraButtons;		// 0x22
	WORD RoomIndex;			// 0x24
	WORD Mistery3;			// 0x26
}FlyByTr4Fields;

typedef struct StrMemSwapAnim {
	short IdParamSwap;
	WORD Slot;
}MemSwapAnimFields;

typedef struct StrBaseMemSwapAnim {
	int TotMemSwap;
	StrMemSwapAnim VetMemSwapAnim[MAX_MEM_SWAP_ANIM];
}BaseMemSwapAnimFields;

// contiene alcuni valori globali di tomb4 che vengono usati spesso
typedef struct StrGlobAddress {

	StrItemTr4 *pLara;
	StrRoomTr4 *pVetRooms;
	StrItemTr4 *pVetItems;

	int *pTotItemsAtBegin;
	int *pTotItems; // era "AncoraTotItems"
	StrInventoryItems *pInventory;
	StrSlot* pVetSlot;
	int *pSlopeType; // SLOPE_ values
	int *pSlopeX; 
	int *pSlopeZ;
	StrBaseHandle BaseHandles;
	StrAnimationTr4 *pVetAnimations;
	StrTexInfoTr4 *VetTexInfo;
	StrStaticSlot *pVetEditObjects;
	StrCollisionLara *pCollisionLara;
	StrMeshTr4 **VetMeshPointer; // tutte le mesh del wad
	StrMeshTr4 **VetMeshLara; // punta alle 15 mesh che formano corpo di lara
	WORD *pFlagsLaraHands;
	WORD *pObjInLaraHandsNow; 
	WORD *pObjInLaraHandsNext; 
	WORD *pWeaponSelected; 
	WORD *pLaraLocationFlags;  // LLF_ values to test with == operator
	short *pVehicleIndex;
	short *pWeaponHolding;
	short *pDashBarValue;
	BYTE *pVetDrip;  
	WORD *pPoison1;
	WORD *pPoison2;
	DWORD *pInputExtGameCommands;
	int *pRopeNowIndex;
	BYTE *pVetInputKeyboard;
	StrAIDataTr4 *pVetAIData;
	int *pTotAIData;
	WORD *pInputGameCommands;
	BYTE *pFlagsLara;
	BYTE *pFlagsLara2;
	int *pObjectActive;
	WORD *pFlareLifeTime;
	WORD *pLaraIndex;
	int *pTimerScreen;
	short *pSizeScreenX;
	short *pSizeScreenY;
	short *pRowCharHeight;
	int *pEarthQuake;
	HWND  *pWindowHandle;
	BYTE *pLevelNow;
	WORD *pScriptLevelFlags;
	BYTE *pScriptMainFlags;
	char *pScriptDat;
	BYTE *pTotSecrets;
	short *pAirAvailable;
	WORD *pTotRooms;
	StrAdrCamera Camera;
	StrAdrFlipMap Remap;
	int *pTotFlyBy;
	StrFlyByTr4 *pVetFlyBy;
	int *pTestFlybyInProgress;
	BYTE *pTestLoadOrNewLevel;
	DWORD *pFrameCounter;
	int *pSetting_MusicVolume;
	int *pSetting_SFXVolume;
	int *pSetting_SoundQuality;
	StrItemTr4 **p2CurrentEnemyTarget;
	DWORD *pColorLayer1;
	DWORD *pColorLayer2;
	BYTE *pSpeedLayer1;
	BYTE *pSpeedLayer2;
	WORD *pVetLightningColors;
	WORD *pVetLightningSourceColors;
	BYTE *pVetSavegameItems; // ZonaSave_169_ArmaPistola
	short *pVetAmmos; // costanti MUN_...
	StrCustDatiOggettoMano *pVetCustObjectsHand;
	WORD *pVetRoomOfDestroyedShatter;
	StrMeshInfo **pVetStructOfDestroyedShatter;
	WORD *pTotDestroyedShatters;
	int *pAudioTrackLoop;
	BYTE *pTestAudioTrackLoop;
	BYTE *pTestWorkingOnMoveables;
	float *pStartFog;
	BOOL *pTestDisableFogBulbs;
	BYTE *pSetting_Volumetric;
	StrDatoInventario *pVetStructInventoryItems;
	WORD *pVetFlagsInventoryItems;
	int *pZonaRecord30;
	StrBoxZones *pVetBoxZones;
	StrFish *pVetFish;
	int *pInclinationType;
	int *pInclinationX;
	int *pInclinationZ;
	StrCombine *pVetNewCombine;
	short *pSetCameraHOrient;
	short *pSetCameraVOrient;
	short *pSetCameraDistance;
	WORD *pSetCameraSpeed;
	BOOL *pTestOverPlatform;
	StrZonaSound *pVetSfxSoundInfos;
	bool *pTestLaserSight;
	int *pZoomFactor;  // while there is binocular or lasersight
	bool *pTestGrayScreen; // pause, inventory, load/save, options
	bool *pTestLoading; // loading level in progress
	short *pFadeScreen;
	int *pCDTrackNow;
	int *pTestCdLoop;  // true= loop mode for pCDTrackNow
	short *pTurnHLaraHeadEnd;
	short *pTurnVLaraHeadEnd;
	short *pTurnRLaraHeadEnd;

	short *pTurnHLaraHeadNow;
	short *pTurnVLaraHeadNow;
	short *pTurnRLaraHeadNow;
	WORD *pFlashRed;
	WORD *pFlashGreen;
	WORD *pFlashBlue;
	WORD *pFlashDurate; // it affects also the intensity, 
						// value in tick frames 1/30 of seconds, only even numbers
	BYTE *pLevelNext; // new level that is going to be loaded
	DWORD *pInputCommandFlags; // altra variabile con tasti premuti
	DWORD *pFadeCurtain;
	StrMeshTr4 *pVonCroyMeshFreeHand; // right free hand
	StrMeshTr4 *pVonCroyMeskKnifeHand; // right hand with knife
	int TotItemsMax; // tot moveable + new created items 
	int *pInventoryRequiredSlotItem; // item that lara requires to engage some moveable: example: crowbar, jeep's key
	int *pInventoryChosenItem; // item selected in inventory

}GlobAddress;

// dati per eventuale breakpoint condizionata

typedef struct StrHardwareB {
	bool Attivo;
	int m_index;
	void *pOffset;
	int NBytes;
	int TipoThread; // tth_ tipo thread handle
	int NLivello;  // numero livello dove applicarla
	int Valore; // da usare come confronto in condizione
	DWORD NControlli;
	char Descrizione[60];
	int TipoOperazione; //  tpob_
	int CodiceCostante; // OFF_
	HANDLE hThread;
	DWORD RecSizeStruct;
	DWORD VetOffset[MAX_OFFSET_HARD_BREAK];
	DWORD VetCount[MAX_OFFSET_HARD_BREAK];
	int  VetLastValue[MAX_OFFSET_HARD_BREAK];
	int TotOffset;
}HardwareBFields;

typedef struct StrFMV {
	int SalvaBloccatiFMV;
	DWORD LastFMVSoundObject;
	int LastCdSound;
	int LastCdLoop;
	DWORD LastCdPosizione;
	int IndiceFmvAttivato;
	int IndiceFmvInCorso;
	int IndiceUltimoFMVAttivato; // usato per global trigger
	BYTE VetFmv[MAX_FMV];
	char NomeFmv[256];  // nome fmv corrente da attivare
	BYTE VetFmvEseguiti[MAX_FMV];
	DWORD TempoInizioFmv;
	DWORD TempoFineFmv;
	HINSTANCE Istanza;
	bool BreakEnabled;
	HRESULT HR;
	bool TestFattoFadeOut;
	int OperazioneAltEnter;  // OAE_...
	bool TestIgnoraTutto;
	bool TestFullScreen;
	bool TestFMVInCorso;
	bool TestFinitoFmv;
	bool TestTombSotto;
	bool TestFinitoAltEnv;
	HWND WindowBlack;
	int TotFmv;
}FMVFields;




typedef struct StrEnvPosition {
	WORD Orient;
	DWORD CordX;
	int CordY;
	DWORD CordZ;
}EnvPositionFields;


typedef struct StrTexParziali {
	WORD  IndiceRange; // indice range
	WORD  IndiceFull; // indice della texture full di cui fa parte questo frammento
	WORD IndiceFrammento;
	WORD  OffX;
	WORD  OffY;
}TexParzialiFields;

typedef struct StrBaseParziali {
	WORD TotRecords;
	StrTexParziali VetParziali[4096];
}BaseParzialiFields;
// dati per animare texture in tomb4


typedef struct StrAnimUVRotate {
	float VetOrgY[MAX_TEX_PER_FRAME]; // posizione OrgY originale di texture in pagine 256x256
	WORD  VetTailIndex[MAX_TEX_PER_FRAME]; // indice tail da modificare

	int TotTextures;
	float Height;  // altezza texture (sara' la meta' di originale)
	int UvRotate; // valore da aggiungere a ScrollPos per movimento
	int   Maschera; // maschera per valore massimo di posy (0x1f, 0x3f o 0x7f)
	DWORD LastTime; // tickcount di ultima esecuzione
	DWORD DelayTime;  // numero di tick oltre il quale si deve eseguire un altro
				// scroll se 0, eseguire sempre
	int ScrollPos;  // posizione di inquadratura attuale da aggiungere a OrgY
	bool TestStop; // per bloccare scrorrimento (da usare con flipeffect)
	bool TestTriangolo; // e' un triangolo
	int IndiceVerticeTop; // indice dove c'e' cordy piu' in alto del triangolo
	WORD TipoAnim; // per tipo di scroll
	int IndiceRangeNG; // indice range originale
}AnimUVRotateFields;
	

typedef struct StrAnimFrame {
	DWORD LastTime;
	DWORD DelayTime;
	bool TestStop;
	int  TotTextures;
	WORD TipoAnim;
	WORD VetTailIndex[MAX_TEX_PER_FRAME];  // indici originali
	WORD VetChangedPos[MAX_TEX_PER_FRAME]; // indici d'ordine dopo modifiche
	StrTexInfoTr4 VetTexInfoRecords[MAX_TEX_PER_FRAME]; // valori originali dei tex info
	int  FrameToSet;  // usato solo per p-frame animazione
	int IndiceToSet;  // indice (da 0..) della texture da sostituire.
						// se -1 farlo con tutte le texture.
	int IndiceScrollRiver;
	int IndiceRangeNG;
}AnimFrameFields;

typedef struct StrBaseAnimTr4 {
	StrAnimUVRotate VetUVRanges[MAX_TAIL_INFOS]; // dati per animazione tex uv range
	StrAnimFrame VetFrameRanges[MAX_TAIL_INFOS]; // dati per animazioni di tipo frame
	int TotUvRanges;  // numero di uv ranges salvati in VetUvRanges[]
	int TotFrameRanges; // numro di fram range salvati in vetframe range
}BaseAnimTr4Fields;

typedef struct StrRemapTailInfo {
	WORD IndiceTom;
	WORD IndiceTr4;
}RemapTailInfoFields;

typedef struct StrBaseRemapTailInfo {
	WORD TotTails;
	StrRemapTailInfo VetRemapTail[MAX_TAIL_INFOS*2];
}BaseRemapTailInfoFields;

typedef struct StrTexSequence {
	WORD IdSequenza;
	WORD Flags; // SEQ_ .. flags
	WORD FPS;
	WORD TotFrames;
	WORD FrameDelay;
	BYTE VetFrames[1200];
}TexSequenceFields;

typedef struct StrBaseTexSequence {
	int TotSequenze;
	StrTexSequence VetSequenze[MAX_TEX_SEQUENCE];
	short VetID[MAX_TEX_SEQUENCE*10];
}BaseTexSequenceFields;

typedef struct StrScriptImage {
	WORD IdCommand;
	WORD NumeroImage;
	WORD Flags;
	WORD EffectTime;
	short AudioTrack;
	RECT RectZona;
}ScriptImageFields;

typedef struct StrBaseScriptImages {
	int TotScriptImages;
	StrScriptImage VetImages[MAX_IMAGES];
	short VetID[MAX_IMAGES*10];
}BaseScriptImagesFields;

typedef struct StrEquipItem {
	WORD Slot;
	short Quantita;
}EquipItemFields;

typedef struct StrBaseEquipItem {
	int TotEquip;
	StrEquipItem VetEquip[MAX_EQUIP_ITEM];
}BaseEquipItemFields;

typedef struct StrEnvMultCondtion {
	WORD EnvCondition;
	short DistanceEnv;
	WORD Extra;
}EnvMultCondtionFields;

typedef struct StrScriptEnvMultCondition {
	WORD TotEnvCondition;
	WORD IdEnvScript;
	StrEnvMultCondtion VetEnvCondition[MAX_ENV_CONDITIONS];
	WORD Dynamic;
}ScriptEnvMultConditionFields;

typedef struct StrBaseScriptEnvCondition {
	int TotScriptEnvCondition;
	StrScriptEnvMultCondition VetScriptEndCondition[MAX_ENV_SCRIPT_CONDITIONS];
	short VetID[MAX_ENV_SCRIPT_CONDITIONS*10];
}BaseScriptEnvConditionFields;

typedef struct StrDatiDiagAnim {
	short NumAnim;
	short ReplaceAnim; // only for animslot
	bool TestEsito;
	WORD Slot;         // only for animslot
	short ItemIndex;   // only for animslot
}DatiDiagAnim;

typedef struct StrAnimDiag {
	int TotAnim;
	StrDatiDiagAnim VetDiag[MAX_ANIM_DIAGNOSTIC];

}AnimDiagFields;

typedef struct StrDoppiaWord {
	short WordBassa;
	short WordAlta;
}DoppiaWordFields;

typedef struct StrGlobalTrigger {
	short GlobalTrigger;
	union {
		long Parameter;
		StrDoppiaWord ParamWords;
	};
	WORD Id;
	WORD Flags;
	short IndiceConditionGroup;
	short IndicePerformGroup;
	short IndiceFalseGroup;
}GlobalTriggerFields;

typedef struct StrBaseGlobalTriggers {
	int TotTriggers;
	StrGlobalTrigger VetTriggers[MAX_GLOBAL_TRIGGERS];
	short VetID[MAX_GLOBAL_TRIGGERS*10];
	bool TestPresoLittleMedipack;
	bool TestPresoBigMedipack;
	bool TestSalvatoSavegame;
	bool TestCaricatoSavegame;
}BaseGlobalTriggersFields;

typedef struct BaseNewObjects {
	int IndiceAnimRubberBoat;	// dati per nuovo oggetto RUBBER BOAT
	int IndiceRubberBoat;
	int IndiceAnimMotorBoat;
	int IndiceMotorBoat;	
}BaseNewObjectsFields;

typedef struct StrWideScreen {
	int SizeX;
	int SizeY;
	float RapportoSchermo;

}WideScreenFields;

typedef struct StrSalvaCollisioni {
	StrItemTr4 *pNemico;
	WORD Flags;
}SalvaCollisioniFields;

typedef struct StrBaseSalvaCollisioni {
	int TotCollisioni;
	StrSalvaCollisioni VetCollisioni[MAX_COLLISIONI];
}BaseSalvaCollisioniFields;

typedef struct StrCoppiaPerform {
	WORD Secondi;
	WORD IdGroup;
}CoppiaPerformFields;

typedef struct StrScriptOrganizer {
	WORD Id;
	WORD Flags;
	WORD Parameter;
	int  TotCoppie;
	StrCoppiaPerform VetCoppie[MAX_ORGANIZE_COPPIE];
}ScriptOrganizerFields;


typedef struct StrStatusOrganizer {
	WORD Status;   // 0 disabled / 1 enabled
	WORD indiceNow;
	DWORD StartPerformed;  // secondi di ultima esecuzione o abilitazione
}StatusOrganizerFields;

typedef struct BaseScriptOrganizer {
	WORD TotOrganizer;

	DWORD CounterGame;  // tempo assoluto di gioco in frame
	StrScriptOrganizer VetOrganizer[MAX_ORGANIZERS];
	StrStatusOrganizer VetStatusOrganizer[MAX_ORGANIZERS]; // da salvare in savegame
	short VetID[MAX_ORGANIZERS*10];
}BaseScriptOrganizerFields;


typedef struct StrSaveActorSlot {
	WORD Slot;
	void *pOriginalCtrl;
	int ItemIndex;
}SaveActorSlotFields;

typedef struct StrBaseSaveActors {
	int TotActors;
	StrSaveActorSlot VetActors[MAX_ACTORS];
}BaseSaveActorsFields;


typedef struct StrCanaleBass {
	DWORD Canale;  //   0x00  handle of open channel or 0 if missing
	short NumeroCd; //  0x04  number of track in progress or -1 if it is missing
	int Loop;       //  0x06  when audio track is looped
}CanaleBassFields;  //  0x0A

typedef struct StrProcBassDllExtra {
	// only example: then it will be replace and integrated with new bass proc:
	TYPE_BASS_ChannelGetPosition BASS_ChannelGetPosition;
}ProcBassDllExtraFields;

typedef struct StrBassHandles {
	StrCanaleBass VetCanali[4];  // reduced by [1] the channel array to host following pointer for extra Proc
	StrProcBassDllExtra * pProcExtra; // point to new list of bass procedures
	BYTE Reserved[6];  // placefolder to preserve offset of following "CanaleNow" field
	int CanaleNow; // 0 o 1 used to set what channel we are using
	DWORD StartOffset;
	float VolumeMusica;
	short OldCdLoop; // Last cd audio track on 0 channel
	bool TestPresente;
	HINSTANCE  HandleDll;
	StrProcBassDll Proc;
}BassHandlesFields;

typedef struct StrSoundSettings {
	WORD MusicQuality;
	short VolumeMusic;
	short VolumeSFX;
}SoundSettingsFields;

typedef struct StrHeaderImportFile {
	WORD Id;
	WORD TipoImport;
	WORD TipoFile;
	short NumeroFile;
	char NomeFile[80];
	DWORD SizeFile;
	BYTE VetBytes[1];
}HeaderImportFileFields;

typedef struct StrItemGroup {
	WORD IdGroup;
	int TotIndici;
	short VetIndici[84];
}ItemGroupFields;

typedef struct StrBaseItemGroup {
	int TotGroups;
	StrItemGroup VetItemGroup[MAX_ITEM_GROUPS];
	short VetRemapFlipActions[256];
	short VetID[MAX_ITEM_GROUPS*10];
}BaseItemGroupFields;

typedef struct StrSingleColors {
	BYTE Red;
	BYTE Green;
	BYTE Blue;
	BYTE Unused;
}SingleColorsFields;

typedef struct StrColorRGB {
	WORD IdColor;
	union {		
		DWORD Colore;
		StrSingleColors Single;
		WORD Dynamic;
	};

}ColorRGBFields;

typedef struct StrBaseColorRGB {
	int TotColori;
	StrColorRGB  VetColori[MAX_COLORI_RGB];
	short VetID[MAX_COLORI_RGB*10];
}BaseColorRGBFields;
// strutture per memorizzare dati di comando script TestPosition
typedef struct StrTestPositionCmd {
	short IdTestPosition;
	WORD Flags;   // TPOS_ flags
	WORD Slot;    // slot of item to detect or index
	StrTestPosition  DatiPosition;
	WORD Dynamic;
}TestPositionCmdFields;

typedef struct StrBaseTestPosition {
	int TotTestPositions;
	StrTestPositionCmd VetTestPosition[MAX_TEST_POSITION];
	short VetId[MAX_TEST_POSITION*10];
}BaseTestPositionFields;


typedef struct StrColoraItem {
	WORD IdColItem;
	WORD Flags; // COLTYPE_
	WORD ItemIndex;
	short IdColore1;
	short IdColore2;
	WORD Speed;
	WORD Dynamic;
}ColoraItemFields;

typedef struct StrBaseColoraItem {
	int TotColoraItem;
	StrColoraItem VetColoraItem[MAX_COLORA_ITEM];
	short VetID[MAX_COLORA_ITEM*10];
}BaseColoraItemFields;

	

typedef struct StrFlipMapRooms {
	int TotFlipRooms;
	short VetRoomMain[512];  // access with index of fliproom. example MainRoomIndex = VetRoomMain[FlipRoomIndex]

}FlipMapRoomsFields;

typedef struct StrTempRoomFlags {
	WORD TotRooms;
	WORD VetRoomFlags[512];
}TempRoomFlagsFields;





// valida per tomb4 e tomb5
typedef struct StrStateChange {
	WORD StateId;
	WORD N_AnimDispatches;
	WORD AnimDispatchIndex;
}StateChangeFields;

// valida per tomb4 o tomb5
typedef struct StrAnimDispatch {
	WORD Low;
	WORD High;
	WORD Animation;
	WORD Frame;
}AnimDispatchFields;




typedef struct StrRecordSFXSuonato {
	short Suono;
	DWORD FrameStart;
}RecordSFXSuonatoFields;

typedef struct StrSFXSuonati {
	int TotSuonati;
	StrRecordSFXSuonato VetSuonati[MAX_SUONATI];
}SFXSuonatiFields;

typedef struct StrBaseTurbo {
	WORD Flags;
	bool TestPriorita;
	int IndicePriorita;
	bool TestAsync;
	WORD TotArg;
	WORD VetArg[255];

}BaseTurboFields;

typedef struct StrBasePushables {
	bool TestCadutaPushable;
	WORD TotPushables;
	StrItemTr4 *VetPushables[1024];
	int VetPushablesIndex[1024];
	BYTE VetCollisionePushable[MAX_ITEMS];
	StrBaseClimbPush BaseClimbPushable;
	short IndicePushSpinto; // mentre lara spinge -1 se non sta spingendo
	
}BasePushablesFields;
typedef struct StrBasevehicles {
	int TotVeicoli;
	int VetIndiciVeicoli[MAX_VEICOLI];
}BaseVeicoliFields;

typedef struct StrAddDeadAnim {
	short Indice;
	WORD AnimIndex;
}AddDeadAnimFields;

typedef struct StrBaseDeadAnim {
	int TotDeadAnim;
	StrAddDeadAnim VetDeadAnim[MAX_DEAD_ANIM];
}BaseDeadAnimFields;

// struttura record per formattazione nuovo font
typedef struct StrFontSetting {
	WORD Status;    // 0x00
	WORD IndiceTex; // 0x02 indice a texture 256x256
	BYTE OrgX;      // 0x04 offset ripsetto a tex256x256per riquadro carattere
	BYTE OrgY;      // 0x05
	BYTE OffX;      // 0x06 spostamento da riquadro a zona pixel effettivi
	BYTE OffY;      // 0x07
	short BaseLineY; // 0x08 adattamento di cordy
	BYTE SizeX;  // 0x0A  dimensione effettiva di zona pixel
	BYTE SizeY;  // 0x0B
}FontSettingFields;  // 0x0C



typedef struct StrFontBaseSetting {
	BYTE CheckSum;
	short TotTextures;
	WORD RapportoSize;
	short TotCaratteri;
	WORD Flags;
	BYTE SizeFont;
	int TotListFont;
	StrFontSetting VetFontList[256];
}FontBaseSettingFields;


typedef struct StrIndiciFont {
	short IndiceRedirect;
	short IndiceTexture;
}IndiciFontFields;

typedef struct StrInfoScrollText {
	short TextOffset;  // numero di byte a partire da inizio stringa
					// se < 0 non ancora impostato
	short OrgY;		// coordinata dove e' stata stampata prima riga 
					// parzialemente visibile
	short OrgX;		// usato da scroll orizzontale
	short OffsetH;  // offset di testo per orgx ma usato solo per scroll orizzonale

}InfoScrollTextFields;

typedef struct StrRobaScrolling {
	StrInfoScrollText  CapoRigaOld;
	StrInfoScrollText  RigaInizio;  // prima riga parzialmente visibile
	StrInfoScrollText  PosizioneInizio; // primo carattere parzialemnte visibile
	char *pAdrStringa;
	int CordYIniziale;
	bool TestFinePrint; // testo sta uscendo da schermo
	DWORD ContaCaratteri; // numero di caratteri reali stampati
	int AltezzaRiga; 
	bool TestLastVisible; // anche ultima riga e' visibile
	int LastCordY; // ultima stampa cordy
	int FixedHeight; // altezza di riga usata come riferimento
	DWORD ContaRighe; // numero rige, la prima e' 0
	int LarghezzaCar; // larghezza massima possibile  W * 2
	int LastCordX; // maggiore cordx di prima riga
	char *ptr_NewStringa; // per saltare caratteri esterni a sizex
}RobaScrollingFields;

typedef struct StrParBar {
	int JumpPower; 
	int GiriCompleti;
	WORD ParallelBarFlags; //copiati da customize
	DWORD TotFrames;  //numero di frame in fase di rotazione
	short SpeedSlide; // muove left / right
	bool TestMostraBarra;

}ParBarFields;
// usata in funzione MoveLaraPosition
typedef struct StrMovePosition {
	int RelX;
	int RelY;
	int RelZ;
}MovePositionFields;
typedef struct StrLastPosAlign {
	DWORD CordX;
	int  CordY;
	DWORD CordZ;
	short HOrient;
	short VOrient;
	short ROrient;
}LastPosAlignFields;

typedef struct StrPosAndFacing {
	DWORD CordX;
	int CordY;
	DWORD CordZ;
	WORD Orient;
	short Room;
}PosAndFacingFields;

// strutturas per aggiustamento automatico di un'animazione
typedef struct StrAdjustAnim {
	bool TestAdjust; // solo se true c'e' adjust in corso
	short Animazione; // numero animazione
	short ItemIndex; // indice di item su cui si sta facendo aggiustamento
	StrItemTr4 *pItem; // item con cui interagisce
	short IdTestPosition; // id del test position usato
	WORD OldAnimazione;
	WORD OldFrame;

	short OrientIdeal; // e' l'orientamento effettivo da usare per il calcolo
	StrMovePosition DatiMovPos;
	StrLastPosAlign LastPosizione; // ultima posizione
}AdjustAnimFields;

typedef struct StrVersionHeader {
	WORD VetVersione[4];
	WORD Flags;	// VER_... flags

}VersionHeaderFields;

typedef struct StrSavegameInfos {
	WORD Flags; // FSAV_ ... flag per savegame
	WORD ShatterInizio; // primo slot statico per shatter
	WORD ShatterFine;  // ultimo slot statico per shatter
	WORD LaraStateId; // state id di lara
	short LaraVitality; // vitalita' di lara
	char Tr4Name[32]; // solonome di file .tr4 a cui si lega questo savegame
	short Veicolo; // VEC_ ... oppure -1 se non e' su un veicolo
	BYTE Copy_057_0F3[0x9d]; // copia prima parte savegame
	BYTE Copy_169_1AC[0x44]; // copia secondo blocco savegame
	WORD FlagsRoom; // flag di stanza dove si trova lara
	DWORD OffsetLara; // offset in inizio savegae di dove parte strutturalara
	BYTE SegretiTrovati_1FB; // bytre di segreti 
	BYTE NonUsato;
	DWORD Distanza;
	BYTE ByteAlign;
	WORD Reserved[114]; // ulteriori sviluppi futuri

}SavegameInfosFields;

typedef struct StrMiniShot {
	bool TestRGB; 
	DWORD ShotSizeX;
	DWORD ShotSizeY;
	bool TestSalvaMiniShot; // salva in savegame i mini shot
	BYTE *pMemMiniShot; //ptr memoria per minishot catturato all'ingresso di inventario
	DWORD SizeMemMiniShot; // dimensione dei dati di file bmp
	DWORD LastTimeScreenShot;
}MiniShotFields;

typedef struct StrCheatNG {
	BYTE LastChars[4];
	DWORD FrameCount;
}CheatNGFields;

typedef struct StrPreloadImage {
	WORD ImageId; // numero immagine
	HANDLE hBitMap; // handle del bitmap caricato
}PreloadImageFields;

typedef struct StrBasePreload {
	int TotPreload;
	StrPreloadImage VetPreload[MAX_IMAGES];
}BasePreloadFields;

typedef struct StrRecordSospensione {
	int NumeroCD;
	DWORD Posizione;
	int TestLoop;
	int NuovoCD;
	int NuovoTestLoop;
}RecordSospensioneFields;

typedef struct StrAudioSospeso {
	int CountSuspend; // incremntato di 1 per ogni sospensione e -1 per ogni riprendiaudio
	StrRecordSospensione VetSospeso[10];
}AudioSospesoFields;

typedef struct StrVetDiari {
	int TotDiari;
	StrBaseDiario VetBaseDiario[MAX_DIARI];
}VetDiariFields;
typedef struct StrHeaderSavegame {
	char NomeLivello[0x4b]; // nome del livello nel savegame
	int Numero;
	WORD Giorni;
	WORD Ore;
	WORD Minuti;
	WORD Secondi;
}HeaderSavegameFields;

typedef struct StrAdrMemory {
	union {
		BYTE *pByte;
		short *pShort;
		int *pLong;
	};
	char TipoSize; // B  S L
	char *pDescrizione;
}AdrMemoryFields;

// struttura per panello savegame
typedef struct StrPanelloSavegame {
	bool TestPresente;
	WORD FlagsSPF;   // SPF_...
	short IdImage;  // id image savegame (o -1 se assente)
	WORD TotSave;
	WORD TotVisibili;
	WORD TipoLayout; // SPL_...
	DWORD StartTime; // contatore perlampeggio
	bool TestLampeggio; // se true mostrare altrimenti no
	WORD IndiceStringa; // testo formattato
	RECT FrameListMicros; // valori in microunits originali
	bool TestInfoSavegames; // mostrareinfo
	int IndicePrimoVisibile;
	int TotCarLinea; // numero di caratteri di linea list
	int IndiceSelezionato;
	int IndiceOldSelezionato;
	int IndiceOldPrimoVisibile;

	bool TestLoad;
	bool TestInCorso;  //mostrato attualmente
	char *pFormatta; // testo di formattazione per info savegame
					// stringa originale non allocata e con place-folders
	char TitoloPanello[80]; // qui va copiata la string attuale
							// di "Load game" o "save game"
	StrBaseDiario MioDiario;
}PanelloSavegameFields;
typedef struct StrText80 {
	char Text[80];
}Text80Fields;

typedef struct StrTwoShorts {
	short s1;
	short s2;
}TwoShortsFields;

typedef struct StrFourBytes {
	BYTE b1;
	BYTE b2;
	BYTE b3;
	BYTE b4;
}FourBytesFields;

typedef struct StrGroupLongVar {
	union {
		int Long;
		union {
			short Short1;
			short Short2;
		};
		union {
			BYTE Byte1;
			BYTE Byte2;
			BYTE Byte3;
			BYTE Byte4;
		};
	};
}GroupLongVarFields;

typedef struct StrVariableNames {
	StrGroupLongVar Alfa;
	StrGroupLongVar Beta;
	StrGroupLongVar Delta;
	int Timer;

}VariableNamesFields;

typedef struct StrBloccoNumVar {
	union {
		int VetNumeriLong[4];
		short VetNumeriShort[8];
		BYTE VetNumeriByte[16];
		StrVariableNames Name;
	};
}BloccoNumVarFields;


// char confondere con dativariabili, questa struttura serve solo
// per variabili
typedef struct StrVariabiliGlobTRNG {
	// variabili numeriche

	StrBloccoNumVar NumWar;
	// variabili di testo
	StrText80 VetTextVar[4];


	// variabili store per savegame
	union {
		int VetStoreLong[16];
		short VetStoreShort[32];
		BYTE VetStoreByte[64];
	};

	// variabili speciali
	char LastInputText[80];
	int LastInputNumber;
	int CurrentValue;
	char TextBig[320];
	// valori per usi futuri
	DWORD VetExtra[20];

}VariabiliGlobTRNGFields;

// mantiene le setting originali che possono venire cambiate
// da funzioni trng
typedef struct StrSalvaSettings {
	bool TestCambiate; // usarto solo per setting usate da softfullscreen
	int Setting_VMode;
	int Setting_WindowX;
	int Setting_WindowY;
	int FlagsSettingRegistro;
	WORD DefMinFog;
	WORD DefMaxFog;
	BYTE DefVolumetricFX; // valore di setup
	BYTE DefBumpMapping; 
	int Reserved[80]; // to change when I modify this structure
}SalvaSettingsFields;
// struttura per record video list degli enum video mode di tomb4
typedef struct StrVideoList {
	DWORD SizeX;
	DWORD SizeY;
	DWORD Bits;
	BYTE  Dati[138];
}VideoListFields;


typedef struct StrBaseFog {
	short DefMinScript; // valori originali scritti in fogrange
	short DefMaxScript; // se -1 alloravalori originali
	short NowStartFog; // integer
	short NowEndFog;  // integer
	short NowFogBulbDistance;  // integer
	float FloatFogEnd;
	float FloatFogStart;
	bool TestHardFogEnabled;  // se false e' attualmene disattivata la fog hard
}BaseFogFields;

typedef struct StrBaseVarAll {
	StrVariabiliGlobTRNG Globals;
	StrBloccoNumVar Locals;

}BaseVarAllFields;
// struttura per emmroizare quando e' accaduto un evento in qualche punto
// per consentire di raccogliere gli eventi accaduti in vari punti
// del codice, e poi controllarli tutti in ControllaTriggerGlobali
typedef struct StrEventoNow {
	short GlobalTrigger; // trigger avventuo
	short Parameter;  // valore in parameter
}EventoNowFields;

typedef struct StrBaseEventiNow {
	int TotEventi;
	StrEventoNow VetEventi[MAX_EVENTI_NOW];
}BaseEventiNowFields;

typedef struct StrSospendiLog {
	bool TestSospendi;
	bool TestRilasciatoF9;
	int SalvaDebugCounter;
}SospendiLogFields;

typedef struct StrRecordSwitch
{
	WORD IdSwitch;
	WORD Flags;  // SWT_....
	WORD Variable; // place folder
	WORD TotIndici;
	short VetIndici[MAX_TRIGGER_GROUPS];

}RecordSwitchFields;

typedef struct StrBaseSwitch 
{
	WORD TotSwitch;
	StrRecordSwitch VetSwitch[MAX_SWITCH];
	short VetID[MAX_SWITCH*10];
}BaseSwitchFields;

typedef struct StrRecordFreeze
{
	WORD ItemIndex;
	WORD Tempo;  //0xffff = ininito
}RecordFreezeFields;

typedef struct StrBaseFreeze
{
	WORD TotFreeze;
	StrRecordFreeze VetFreeze[MAX_FREEZE];

}BaseFreezeFields;

typedef struct StrBaseMissing {
	bool TestScriptPresenti; // se entrambi i file script sono presenti
	char MexMissing[1024]; // avvisi file mancanti o NULL
	char MexLoadCamera[256];
	bool TestErrori; // errori 
	bool TestAvvisi;
	bool TestErroreLoadCamera;
	int Reserved[80]; // to change when I add new field to this structure
}BaseMissingFields;

typedef struct StrFloodRecord {
	WORD RoomAcqua;  // poi con +1 si trova la stanza sopra
	float OriginaleY; // valore originale di cordy di superfice acqua
	float PosYAcquaNow; // valore usato dopo load savegame
}FloodRecordFields;

typedef struct StrBaseFloodRooms {
	WORD TotFloods;
	StrFloodRecord VetFloods[MAX_FLOODS];
	float GapY;  // valore da sommare
	float OriginaleY; // ai vertici con questa coordinata.
	bool TestAcqua;
}BaseFloodRoomsFields;


// dati calcolati solo una volta ogni tot frames
// che riguardano possibilita' di attaccare 
// nota: 

typedef struct StrGlobFish {
	bool TestImpostato; // false = bisogna ricalcolare tutto
	WORD TempoRitardo; // calcolare solo quando questo raggiunge zero
	StrAbsBoxCollision BoxCollisionLara; // creato da UpdateLocust
	bool TestLOS; // lara puntabile da pivot
	GAME_VECTOR LastLaraPos; // ultima coordinate di lara quando si poteva puntare


}GlobFishFields;
typedef struct StrScriptStandBy 
{
	short IdStandBY;
	WORD TipoStandby;
	DWORD LimiteCicli;
	WORD Flags;
	short FirstText;
	WORD NumTexts;
	short AudioTrack;
	WORD VerticalOrient;
	short OrientSpeed;
	WORD Distanza;
	short IdTriggerGroupStart;
	short IdTriggerGroupEnd;

}ScriptStandByFields;

// dati per gestione di standby
typedef struct StrBaseStandBy
{
	int TotStandBY;
	short VetID[MAX_STANDBY*10];
	StrScriptStandBy VetStandBy[MAX_STANDBY];
	int IndiceSetCamera;
	WORD IdNow;
	bool PrimaEsecuzione;
	bool TestFlipEffect; // = true, attivato con flipeffect
	DWORD LimiteCicli; // INPUT raggiunto si avvia standby
	bool TestPresente; // se c'era comando in script  per standby
	bool TestIniziato; // siamo gia' in modo standby
	short OrientSpeed; // INPUT velocita' rotazione
	short VerticalSpeed; // velocita rotazione verticale
	short IncOrientSpeed;
	short DistanzaSpeed; // velocita' percambiare distanza
	short MaxDistanza;
	short MaxVAngle;
	short MaxHOrient;	// usato solo per portrait con flip horient
	short MaxHSpeed;
	short MinHSpeed;
	short MinDistanza;
	short MinVAngle;
	short MinHOrient;
	short IdTriggerGroupStart; // trigger group da chimare in avvio
	short IdTriggerGroupEnd; // trigger group da chiamare alla fine
	int TotStringhe;     // numero testi
	short AudioTrack; // traccia audio da suonare al posto al posto, o sopra suono attuale
	short IndicePrimaStringa;
	short VerticalOrient; // INPUT ma forse non lo uso
	WORD TipoStandby; // INPUT TSB_ valore per il tipo di movimenti
	WORD Flags;		 // INPUT FSB_..
	short Distanza;   // INPUT distanza da tenere da lara
	int OldIndiceSetCamera;
	bool OldTestSetCamera;
	bool OldDisableCombat;
	DWORD OldTastiDaBloccare;
	bool OldTestFreezeAll;
	int OldTimerScreen;
	short OldAriaDisponibile;
	WORD OldTempoFlare;
	WORD OldValOscilla;
	DWORD OldContatoreSchermo;
	int ContaStringhe; // 0,1,2 ecc
	DWORD ContaTempoStringa;
	StrSetCamera OldSetCamera; // dati di camera di gioco prima dello standby
	int Reserved[40]; // to change when I modify this structure
}BaseStandByFields;

typedef struct StrRobaKayak {

	StrBackupLara PosLaraPrimaKayak;
	WORD OldAnim;
	int OldFrame;
	bool TestDestra;
	DWORD StartFrame;
	StrMovePosition PosIdeale;
	short OrientIdeale;
	bool TestMistIniziata;
	short ClickPausa;
	int IntensitaMist;
}RobaKayakFields;

typedef struct StrBoatSinking {
	bool TestAffonda;
	bool TestGorgo;
	int CordY;
	short RoomNow;
	short RoomOld;
	short VOrient;
	bool TestFinito; // e' gia' affondata fino al punto di centroy


}BoatAffondaFields;

typedef struct StrRecordAssSlot {
	WORD MioSlot;   // Slot Id
	WORD TipoSlot;  // OBJ o altro valore Slot
}RecordAssSlotFields;

typedef struct StrBaseAssignSlot {
	short TotNewAssign;
	StrRecordAssSlot VetNewAssign[MAX_ASSIGN_SLOT];
}BaseAssignSlotFields;


typedef struct StrRollBoat {
	short Indice;
	short SpeedPitch;
	short SpeedSwing;
	short SoundSfx;
	WORD Flags; // FRB
	DWORD OldCordX;
	DWORD OldCordZ;
	BYTE DerivaTotCicli;
	char DerivaIncX;
	char DerivaIncZ;
}RollBoatFields;

typedef struct StrBaseRollBoat {
	WORD TotRollBoats;
	StrRollBoat VetRollBoats[MAX_ROLL_BOATS];
	// variabili per accesso rapido a presenza di rolling per 
	// rubberboat o motorboat
	bool TestRubberBoat;
	bool TestMotorBoat;
}BaseRollBoatFields;

#define MAX_TARA_ITEMS 14
// usata per tareare posizione di detector
typedef struct StrCordDetectors {
	int DetOrgY;		// 0
	int BaseTargetY;		// 1
	float SizeTestoX;	// 2
	float SizeTestoY;	// 3
	int BaseVLineX;		// 4
	int BaseVLineY;		// 5
	int GapVLineX;		// 6
	float GapVLineY;	// 7
	int BaseTextX;		// 8
	int BaseTextY;		// 9
	int CompassLineY;   // 10   13845
	int KeyPadX;		// 11   11056
	int KeyPadY;		// 12   17685
	int KeyPadTextY;    // 13   2304
	int IndiceAttivo; // valore da 0 a MAX_TARA_ITEMS-1
	DWORD LastCambioVar;
}CordDetectorsFields;

typedef struct StrAlignObjData {
	float RapportoScreen;
	int ScreenX;
	int ScreenY;
	int BaseVLineY; // detector radar VLineY
	int CompassLineY; // detector compass: VLineY
	int BaseTextY; // detector tutti TextY
	int BaseTargetY; // detector radar: targetY
	int KeyPadY;  // keypad OrgY
	int KeyPadTextY; // keypad TextY
	int DetOrgY;  // detector tutti: OrgY
}AlignObjDataFields;

typedef struct StrScaleItem {
	WORD IdScale;
	WORD Dynamic;
	short ItemIndex; 
	WORD Flags;   // fst_
	short BeginPerc;
	short FinalPerc;
	short IncPerc;
}ScaleItemFields;

typedef struct StrBaseScaleItem {
	WORD TotScale;
	StrScaleItem VetScaleItem[MAX_SCALE_ITEM];
	short VetIdScale[MAX_SCALE_ITEM*10];
}BaseScaleItemFields;

typedef struct StrShowSprite {
	WORD IdShowSprite;
	WORD Dynamic;
	WORD Flags;  // FSS_
	RECT Posizione;
	WORD SlotSprite;
	WORD IndiceSprite;
	COLORREF Colore;
	short GridX;
	short GridY;
	short ExtraValue;
}ShowSpriteFields;

typedef struct StrBaseShowSprite {
	WORD TotShowSprites;	
	StrShowSprite VetShowSprites[MAX_SHOW_SPRITES];
	short VetIdShowSprites[MAX_SHOW_SPRITES*10];
}BaseShowSpriteFields;

typedef struct StrPointFloat {
	float x;
	float y;
}PointFloatFields;

typedef struct StrTriangolo {
	StrPointFloat A;
	StrPointFloat B;
	StrPointFloat C;
}TriangoloFields;

typedef struct StrParamTriangle {
	short IdParam;
	StrTriangolo Tria;
	WORD Dynamic;
}ParamTriangleFields;

typedef struct StrBaseParamTriangles {
	WORD TotTriangles;
	StrParamTriangle VetTriangles[MAX_TRIANGLES];
	short VetIdTriangles[MAX_TRIANGLES*10];
}BaseParamTrianglesFields;

typedef struct StrQuad {
	short IdParam;
	StrTriangolo Tria1;
	StrTriangolo Tria2;
	WORD Dynamic;
}QuadFields;


typedef struct StrBaseQuads {
	WORD TotQuads;
	StrQuad VetQuads[MAX_QUADS];
	short VetIdQuads[MAX_QUADS*10];
}BaseQuadsFields;

typedef struct StrCircle {
	short IdParam;
	short CenterX;
	short CenterY;
	WORD Radius;
	WORD Dynamic;
}CircleFields;

typedef struct StrBaseCircle {
	WORD TotCircles;
	StrCircle VetCircle[MAX_CIRCLES];
	short VetIdCircle[MAX_CIRCLES*10];
}BaseCircleFields;

typedef struct StrParamWText {
	short IdParam;
	WORD Flags;
	WORD Durate;
	WORD ExtraParam;
	short WindowsFontId;
	RECT MyRect;
	WORD Dynamic;
}ParamWTextFields;

typedef struct StrBaseParamWText {
	int TotParamWText;
	StrParamWText VetParamWText[MAX_PARAM_WTEXT];
	short VetIds[MAX_PARAM_WTEXT*10];
}BaseParamWTextFields;

// mia struttura per ospitare dati minimi per calcolo collisione
// di questo oggetto (che poteva essere moveable o static) con un moveable
// usando funzione: CollideItemConCustom()
typedef struct StrCustomItem {
	DWORD CordX;	// 00
	int CordY;		// 04
	DWORD CordZ;	// 08
	short MinY;		// 0C
	short MaxY;		// 0E
	StrBoxCollisione *pBoxRel; // 10 (box relativo)
	short hOrient; 
	int ItemIndex; // o -1 se static
	short Slot; // o -1 se static
}CustomItemFields;

typedef struct StrVetItemCollision {
	bool TestAttiva;
	StrItemTr4 *pVeicolo;
	WORD TotMoveables;
	WORD VetMoveables[100];
	WORD TotStatics;
	StrPosizione OldPos;
	short OldSpeed; // precedente velocita' di veicolo
	StrCercaStatic VetStatics[100];
	StrAbsBoxCollision OutCollisioneBox; // usato per collisione veicoli
	int NewFloorY; // valore restituito da AnalisiCollVeicoli
}VetItemCollisionFields;

typedef struct StrShowMeshAbs {
	StrPos3d PosData;
	short Mesh;
}ShowMeshAbsFields;

typedef struct StrBaseShowMeshes {
	int TotMesh;
	StrShowMeshAbs VetMesh[MAX_SHOW_MESH];
}BaseShowMeshesFields;

typedef struct StrCheckPoint {
	short Orient;
	WORD Distance;
}CheckPointFields;

typedef struct StrEnvForEnemy {
	bool TestLara; // se = false allora e' nemico
	StrBoxCollisione EnemyBox; // relative collision box
	StrItemTr4 *pOggetto;
	int IndiceOggetto;
	DWORD FirstAnim;
	int TotExtraCheckPoints; // further point to check
	StrCheckPoint VetExtraCheckPoints[16];
}EnvForEnemyFields;

typedef struct StrRecordMemZone {
	DWORD OldAddress;  // original address in tomb4
	DWORD SizeOldMemory; 
	DWORD NewAddress;  // new address of allocated memory
	DWORD SizeNewMemory; 

}RecordMemZoneFields;

typedef struct StrRelocatedMem {
	DWORD Start; // new start address for this memory zone
	DWORD End; // new end address (excluded) for this memory zone
	DWORD TotItems; // number of items/recores that this zone is now able to host
}RelocatedFields;


typedef struct StrBaseRemapMemory {
	// here there is a direct access to old/new address for extended memory zone
	// in tomb4
	StrRelocatedMem Old_80DE80;  // 0
	StrRelocatedMem Old_80DEA0;  // 1
	StrRelocatedMem Old_4BFD70;  // 2
	StrRelocatedMem Old_533290;  // 3
	StrRelocatedMem Old_8012E0;  // 4
	StrRelocatedMem Old_5C0678;  // 5
	StrRelocatedMem Old_660B60;  // 6
	StrRelocatedMem Old_660998;  // 7
	StrRelocatedMem Old_670B60;  // 8
	StrRelocatedMem Old_52A220;  // 9
	StrRelocatedMem Old_7F6DA8;  // 10
	StrRelocatedMem Old_7F57C8;  // 11
	StrRelocatedMem Old_532C70;  // 12
	StrRelocatedMem Old_7FFE40;  // 13
	StrRelocatedMem Old_804740;  // 14
	StrRelocatedMem Old_7F7769;  // 15
	StrRelocatedMem Old_7F5960;  // 16
	StrRelocatedMem Old_4ACAC0;  // 17
	StrRelocatedMem Old_660678;  // 18  	// aggiungte 1.3.0.0
	StrRelocatedMem Old_5C0350;  // 19
	StrRelocatedMem Old_4B09B4;  // 20
	StrRelocatedMem Old_532B60;  // 21
	StrRelocatedMem Old_533270;  // 22
	StrRelocatedMem Old_533278;  // 23
	StrRelocatedMem Old_53327C;  // 24
	StrRelocatedMem Old_533284;  // 25
	StrRelocatedMem Old_533288;  // 26
	StrRelocatedMem Old_53328C;  // 27
	StrRelocatedMem Old_7FE1E0;  // 28
	StrRelocatedMem Old_7FE100;  // 29
	StrRelocatedMem Old_4AB9B8;  // 30
	StrRelocatedMem Old_4ABB90; // 31
	StrRelocatedMem Reserved[80-14]; // sosttiuire -0 con nuove aggiunte (ultima era con indice = 17)


}BaseRemapMemoryFields;

// struttura per gestione di triggergroup con flag TGROUP_SINGLE_SHOT_RESUMED
typedef struct StrSingleShotResumTG {
	int TotTGResumed; // id dei tg with flag resumed nello script 
	WORD VetTGResumed[MAX_TRIGGER_GROUPS];
	int TGEseguiti; // id dei triggergroup eseguiti nell'ultimo ciclo da F118
	WORD VetTGEseguiti[MAX_TRIGGER_GROUPS];

}SingleShotResumTGFields;




typedef struct StrRecordParamLgtn {
	short IdParamLgtn;
	short Flags;  // LGTN_
	short IndexSource;
	short IndexTarget;
	DWORD Color;
	short Intensity;
	WORD SoundEffect;
	short Size;
	short ParticleDurate;
	short Arg1;
	short Arg3;
	short IntervalMask;
	WORD Dynamic;
}RecordParamLgtnFields;
typedef struct StrPatch {
	DWORD Offset;
	bool TestInizio;
}PatchFields;

typedef struct StrTotElementi {
	DWORD Offset;
	char Tipo;
}TotElementiFields;

typedef struct StrParamPatch {
	DWORD Offset;
	char SizeType;  // 'D' (dword) 'W' word  'B' byte
	int Gap;    // relative value to add +-
}ParamPatchFields;

typedef struct StrTripla {
	int SizeX;
	int SizeY;
	int SizeZ;
}TriplaFields;

typedef struct StrFloatPatch {
	DWORD Offset;
	float ValDefault;
	bool TestFloat;
}FloatPatchFields;

// struttura sprite di tomb4
typedef struct StrSpriteTomb4 {
	WORD TexturePage;
	BYTE OrgX;
	BYTE OrgY;
	WORD SizeX;
	WORD SizeY;
	float x1;
	float y1;
	float x2;
	float y2;
}SpriteTomb4Fields;
typedef struct StrRiverRange {
	int TotTexture;
	WORD SettingFps;
	int IndiceRangeNG;
	WORD VetIndiciTail[50];
}RiverRangeFields;
typedef struct StrCheckOverflow {
	DWORD SizeMemoria;   // Dimensione complessiva di zona di memoria
	DWORD *pEndMem;   // indirizzo memoria da controllare (zona cuscinetto)
	int TotDword;   // numero massimo di dword di zona cuscinetto
	char Nome[64];  // nome per segnalare errore
	bool TestDinamica;  //true= allocata con memoria cuscinetto, false = memoria fissa
	float PercentualeUsata; // oppure -1 se non e'ancora stato calcolo
	DWORD SizeUsata; // byte usati (impostato quando viene calcolata percentuale)
}CheckOverflowFields;



// record  per salvare nomi custom degli slot
typedef struct StrNomeSlot {
	WORD Slot; // numero slot
	char NomeSlot[32];
}NomeSlotFields;

typedef struct BaseNomiSlot {
	short TotNomiSlot;
	char NomeWad[64]; // solo nome senza dir
	StrNomeSlot VetNomi[MAX_NOMI_SLOT];
}BaseNomiSlotFields;
// dimensione record 0x28 = 40 bytes
typedef struct StrEffettoTr4 {
	WORD Boh1;   // 00
	WORD Boh2;   // 02
	WORD Boh3;   // 04
	WORD Boh4;   // 06
	WORD Boh5;   // 08
	WORD Boh6;   // 0A
	WORD Boh7;   // 0C
	WORD Boh8;   // 0E
	WORD Boh9;   // 10
	short Room;  // 12
	short CodiceEffetto;  // 14
	short IndiceNext; //16
	short IndicePrevious; // 18
	WORD Boh10;  // 1A
	WORD Boh11;  // 1C
	WORD Boh12;  // 1E
	WORD Boh13;  // 20
	WORD Flag; // 22
	WORD Boh14; // 24
	WORD Boh15;  // 26

	
}EffettoTr4Fields;

typedef struct StrTailInfoTom {
	BYTE OrgX;		// 00
	WORD TopPixel;	// 01
	BYTE Flags;     // 03
	BYTE Mistery1;  // 04
	BYTE SizeX;     // 05
	BYTE Mistery2;  // 06
	BYTE SizeY;      // 07
}TailInfoTomFields;

typedef struct StrSoundSample {
	DWORD ObjSound;
	DWORD SoundPerSample;
	DWORD IndiceRel;
}SoundSampleFields;

typedef struct StrSnowStruc {
	WORD  Mistero1;
	WORD  PaginaTexture;
	WORD  Flags;
	float  x1;
	float  y1;
	float  x2;
	float  y2;
	float  Size;
}SnowStrucFields;

	
typedef struct StrDatiExtraAnimazioni {
	bool TestPresente;
	BYTE Tot_UV_Rotate; // numero di animation range con uv rotate
	WORD TotaleRangeNG; //numero originale di animation ranges
	WORD VetInfoRangeAnim[40]; // ogni valore contiene info su velocita frame
								// di ogni record animazione range
	WORD VetFromTex[40];
	WORD VetToTex[40];
	// usati solo per esport in headerng di file tom
	WORD SizeDefault; // 64 o 128

}DatiExtraAnimazioniFields;



// used for compute the origin of system strings of tomb4
typedef struct StrOffsetRanges {
	DWORD Start;
	DWORD End;
}OffsetRangesFields;



typedef struct StrBaseLightning {
	int TotParamLgtn;
	StrRecordParamLgtn VetRecordLgtn[MAX_PARAM_LIGHTNING];
	short VetId[MAX_PARAM_LIGHTNING*10];
}BaseLightningFields;

typedef struct StrBaseImgMonoScreen {
	bool TestBackGround; // se c'era in scropt comando "BackGround="
	bool TestImageMonoscreen;  // true= custom image to free
	int NumeroImage;  // numero, non ID
	int Cd_Track;   // -1 = no audio track
	WORD Flags;   // BKGDF_ flags
	short Parameter;
	bool TestLoop;   
	bool TestTrasparente;
}BaseImgMonoScreenFields;

typedef struct StrBaseImgBackGround {
	bool TestEnabled;
	bool TestAllocatedImage;
	int ImageNumber;
	WORD Flags; // BKGDF_ flags
	short Parameter;
	bool TestTransparent;
}BaseImgBackGround;

typedef struct StrRecNewError {
	char *pMex;
	int FrameTicks;
}RecNewErrorFields;


typedef struct StrBaseDgxErrors {
	int TotOld;
	char *pVetMexOld[MAX_DGX_OLD_ERRORS];
	int TotNew;
	StrRecNewError VetMexNew[MAX_DGX_NEW_ERRORS];
}BaseDgxErrorsFields;

typedef struct StrTiming {
	char *pLastMex;
	DWORD LastTime;
	DWORD NCicli;
	DWORD ValoreMedio;
}TimingFields;

typedef struct StrGlobalBinoculars {
	int LastYSextant;
	int LastXCompass;
	int MaxOscilla;
	int LastIncX;
	int MaxOscillaY;
	int LastIncY;
	int ValoreZoomProgresso; // viene usato sempre in codice tomb4
	float ZoomInc;
	float ZoomInc2; 
	float ZoomMax;
	DWORD OldTastiPremuti;
} GlobalBinocularsFields;

typedef struct SalvaSliderInput {
	bool TestUsed;
	DWORD Parameters[8];
}SalvaSliderInputFields;


typedef struct StrAnchoredBoat {
	WORD Slot;  // slot about posizione of frozen boat
	StrEnvPosition OldPosition;  // used with OCB_BOAT_ANCHORED ocb
}AnchoredBoatFields;

typedef struct StrMiniItemData {
	DWORD MeshVisibilityMask; // 08
	WORD StateIdCurrent;	    // 0E
	WORD StateIdNext;			// 10
	WORD StateIdAI;				// 12
	WORD AnimationNow;		    // 14
	WORD FrameNow;			    // 16
	short Room;					// 18
	short SpeedH;			// 1E
	short SpeedV;			// 20
	short Health;				// 22
	BYTE IdSprite1;				// 2e
	BYTE IdSprite2;				// 2f
	WORD Trasparency;			// 30
	DWORD  CordX;					// 40
	int  CordY;					// 44
	DWORD  CordZ;					// 48
	short OrientationV;  // 4c
	short OrientationH;  // 4e
	short OrientationT;		// 50
	DWORD FlagsMain;		// 15ea
	DWORD FlagsSwapMesh;	// 15ee
}MiniItemDataFields;

typedef struct StrSalvaDatiLara {
	StrMiniItemData Lara;
	int VetMeshLaraOffset[15];  // differenza tra indirizzo prima mesh di lara e valori impostati nelle varie mesh di lara
	int InventoryItemIndex; // item currently selected
	BYTE pLevelNow; 
	BYTE VetDrip[16];  // status gocce lara
	WORD FlagsLaraHands;
	WORD ObjInLaraHandsNow; 
	WORD ObjInLaraHandsNext; 
	WORD WeaponSelected; 
	WORD LaraLocationFlags;
	short VehicleIndex;
	short WeaponHolding;
	WORD Poison1;
	WORD Poison2;
	BYTE FlagsLara;
	BYTE FlagsLara2;
	int ZonaSave_9d_CordYDaPeloAcqua;
	int ObjectActiveIndex;
	WORD FlareLifeTime;
	int TimerScreen;
	int EarthQuake;
	short AirAvailable;
	int TestFlybyInProgress;
	WORD VetBloccoOrient[14];
	int Reserved[500]; // per futuri sviluppi
	// aggiungere dati per rotazione testa e busto
}SalvaDatiLaraFields;

// recording
typedef struct StrRecording {
	WORD Status;   // RECF_...
	WORD Version;  // first is 100 (1.00)
	StrSalvaDatiLara DatiLara;
	int IndexFrame; // for playing
	int TotFrames;
	DWORD VetFrames[MAX_DEMO_FRAMES];
}RecordingFields;

// data for fake managemnt of updateinput (directinput)
typedef struct StrMyDirectInput {
	DWORD LastCheckTime; // last ms when perfromed the check
	DWORD LastCheckFlags; // keyboard  input value read at least check
}MyDirectInputFields;


// struttura per demo
typedef struct StrDemo {
	WORD Flags; // DEMF_...
	short Parameter;
	short StringInfoTextIndex;
	short StringDemoLegendIndex;
	short WaitingTime;
	short LastIdPlayed; // or -1 se no demo yet played
	int TotDemoIDs;
	int SalvaSfxVolume; // per title
	int SalvaMusicVolume; // per title
	StrScriptOrganizer *pOrganizer;  // se c'e'un organizer linked with this demo (oppure NULL)
	DWORD LastInputTime; // in framecounts to check countdown
	WORD VetDemoIDs[MAX_DEMO_IDS];
	BYTE VetDemoPlayed[MAX_DEMO_IDS]; // only for title demo
	bool TestLoadAndPlay;  // impostato prima di caricare un livello (da title) dove eseguire subito uno o piu' demo
	bool TestDemoInProgress; // when a demo is in progress because launched from a Demo script command
	bool TestTornaATitle; // alla fine di un demo lanciato da title si ritorno a title
	DWORD CtrlSign; // when already loaded 0x615274F1 (only for title)
}DemoFields;

typedef struct StrCutsceneCamera {
	bool TestAllocata; // catturata una fixed camera (poi si dovra' ripristinarla)
	bool TestAttiva; // attualmente si sta usando cutscene camera
	bool TestFreeze; // sospesa esecxuzione
	WORD Flags; // FCT_...  
	DWORD FreezeEndFrame; // momento di fine freeze
	int IndexCamera; // indice di camera usata per cutscene
	int TargetIndex; // indice di target attuale
	StrCameraTr4 Original; // posizione originale di fixed camera (da ripristinare)
	StrCameraTr4 *pCamera; // puntatore a camera usata come cutscene camera
	int AngleViews;  // posizione rispetto a target (stessi valori di F381/382/383 triggers)
	int Distance; // distanza attuale da target
	int DistanceInc; // incremento per modificare distanza
	DWORD DistanceEndFrame; // se c'e' effetto zoom a tempo questo ci dice quando sara' terminato l'effetto
	int Height; // differenza Y rispetto a punto piu' alto di target
	int HeightInc; // valore da aggiungere ad ogni frame per effetto di movimento su /giu
	DWORD HeightEndFrame; // quando sara'finito l'effetto di movimento up/down in demo frames
	int Rotate;  // new degree to add to have a rotation around the target
	int RotateInc;  // change of Rotate increment
	DWORD RotateEndFrame; 

	DWORD LastPerformedFrame; // to avoid double performing in same frame number
	StrCameraTr4 CameraNow; // used only to save in savegaem position of cutscene camera and then restore it
	BYTE Reserved[41];

}CutsceneCameraFields;




typedef struct StrBaseCutscene {
	bool TestDemoIntroDone; //gia' eseguita prima esecuzione di cutscene intro 
	StrBaseSaveActors BaseSaveActors;
	StrCutsceneCamera BaseCamera;
	int LeadingActorIndex;
	StrTextActors TextLeading;
	int ExtraActorIndex;
	StrTextActors TextExtra;
	StrTextActors TextLara;
	int TotBasicCollision;
	short VetBasicCollisionIndices[MAX_BASIC_COLLISIONS]; // indices of moveable whom add basic collision
}BaseCutsceneFields;


typedef struct StrFlipMesh {
	WORD Slot;
	WORD Mesh;
}FlipMeshFields;

typedef struct StrBaseFlipMesh {
	int TotFlipMesh;
	StrFlipMesh VetFlipMesh[MAX_FLIP_MESH];
}BaseFlipMeshFields;

// to store new items created in last cycle
typedef struct StrCreatedItems {
	int TotNewItems;
	short VetNewItems[MAX_NEW_ITEMS];
}CreatedItemsFields;

typedef struct StrPluginNames {
	int PluginId;
	char Nome[80];
	DWORD TotUsato;
}PluginNamesFields;

typedef struct StrTablePluginIdFloor {
	int TotTable;
	BYTE VetTable[0x10000];
}TablePluginIdFloor;

// fille with data from Plugin= script commands
typedef struct StrRecPluginScript {
	WORD PluginId;
	char Name[80];
	int MainPluginSettings;  // MPS_.. flags
	short TotDisable;
	WORD VetDisable[256];
}RecPluginScriptFields;


typedef struct StrTableScriptPlugin {
	int TotPlugins;
	StrRecPluginScript VetPlugins[MAX_PLUGINS_NGLE];
}TableScriptPluginFields;

typedef struct StrWeather {
	WORD LevelFlags;  // for level flags about rain/snow  fngl_
	BYTE RainBuffer[SIZE_RAIN_BUFFER];
	BYTE SnowBuffer[SIZE_SNOW_BUFFER];
	DWORD ContaFrameSnow;
	float Rain_Float_1;
	float Rain_Float_2;  // spessore goccia
	float Rain_Float_4;  // inclinazione
	float Rain_Float_8;
	float Rain_Float_16;
	float Rain_Float_20480; // ancora non so cos'e'
	short Max_Rain;
	short Min_Rain;
	DWORD Splash_Rain;  // numero di splash per pioggia che tocca terreno
	short Max_Snow;
	short Min_Snow;
	float VettoreSizeSnow[32]; // 16 min max ranges
	int Reserved[100]; 
}WeatherFields;


typedef struct StrScriptLaraStartPos {
	bool TestPresent;
	short Room;
	short OCB;
}ScriptLaraStartPosFields;

typedef struct StrCollisionLast {
	int ItemIndex; // returned by IsCollidingWithSomeItem() function: index of moveable with whome there is collision or -1 if missing
	int StaticIndex; //  returned by IsCollidingWithSomeItem() function: index of static item with whome there is collision or -1 if missing
	int StaticIndexRoom; //  room of above static index
	StrAbsBoxCollision BoxItem;  // abs collision box of primary item
	StrAbsBoxCollision BoxSecondary; // abs collision box of secondary item (or static)
}CollisionLast;

// structure for miscellanous, different global variable
typedef struct StrGlobMisc {
	StrCollisionLast CollisionLast;
	int TotMaxAIRecords;
	int TotOldAIRecords;
	int TotItemOlds; // original number of moveables loaded from tr4 file
	int TotAsmTtngPatches; // number of numeric trng patches 
}GlobMiscFields;

typedef struct StrStaticMIP {
	short Slot; // main slot
	short DistanceB;
	short SlotB;
	short DistanceC;
	short SlotC;
}StaticMIPFields;

// structure to host all StaticMIP script command of current level section
typedef struct StrBaseStaticMIP {
	int TotStaticMIP;
	StrStaticMIP VetStaticMIP[MAX_STATIC_MIP];
	StrStaticMIP *pStaticNow;  // the further pointer to StaticMip command of current static (if the MainSlot is right)
}BaseStaticMIPFields;

// global structure with all data for tomb4 and trng

typedef struct StrGlobaliTomb4 {
	StrBaseRemapMemory BaseRemap; // remapped memory zones in tomb4
	DWORD FlagsLevel;   // valore FL_...
	StrPrefTomb MyPrefTomb;
	StrBaseFog BaseFog;
	StrCordDetectors BaseCordDetector;
	StrBoatSinking BoatSinking;
	StrBaseShowMeshes BaseShowMesh;
	StrSospendiLog BaseSospendiLog;
	StrBaseMissing BaseMissing;
	WORD *pIndiceFirstAnimBike;
	StrVetItemCollision BaseCollItem; // per collisione con veicoli
	StrBaseScaleItem BaseScaleParam;
	StrBasevehicles BaseVeicoli;
	StrBaseRollBoat BaseRollBoats;
	StrLaraHp BaseOldHpLara;
	StrRobaKayak BaseKayak;	
	bool TestFmvTitlePerformed;
	StrBaseShowSprite BaseShowSprites;
	StrBaseParamTriangles BaseTriangles;
	StrBaseQuads BaseQuads;
	StrBaseCircle BaseCircles;
	StrScriptLaraStartPos BaseLaraPosScript;
	bool TestExtraKayak;
	StrBaseAssignSlot BaseAssignSlot;
	BYTE DatiExtraKayak[0x30];
	DWORD SlowMotionDelay;
	bool TestFixLaraMovPos;
	bool TestDummyFalse; // conditional frag trigger
	bool TestDummyBlockForward;
	short OrientFixLaraMovPos;
	WORD VetSalvaFlagItems[120];
	int TotCombinabili;
	DWORD DummyX;
	int DummyY;
	DWORD DummyZ;
	short HealthLaraOld;
	WORD VetCombinabili[120];
	StrGlobFish GlobFish;
	DWORD LastTempoF12;
	bool TestObjectIsNotLara; // usato in TestTriggers per attiva con oggetti trigger normale
	StrBaseFloodRooms BaseFloodRooms;
	StrBaseFreeze BaseFreeze;
	WORD  HangForbidden;  // 0 se e' permesso, altrimnti bit NO_HANG_....
	WORD HangCounter; // se 0 azzera a inizio ciclo, altrimenti decremnta 
	int DebugModeCounter; // se > 0 mostra debug
	int TotBigNumbers;
	WORD VetBigNumbers[256];
	DWORD StartTimeHideDiagnostic;
	StrBaseStandBy BaseStandBy;
	StrSavegameInfos BaseSaveGameInfos;
	StrRecordFlip VetScanFlipEffects[64];
	StrBaseVarAll *pBaseVariableTRNG;
	StrBaseEventiNow BaseEventiNow;
	WORD  TotItemNoCollisions;
	short VetItemNoCollisions[MAX_COLL_DISABLED];
	StrSuoni Suoni;
	bool TestFogRange; // c'era un comando fogrange in lviello
	bool TestSetup; // tomb4' -setup
	StrBaseWindowsFonts BaseFonts;
	short VetRemapRooms[MAX_ROOMS];
	short LevelFarView; // valore preso da script
	short WorldFarView; // valore da script
	StrVetDiari BaseDiari;
	bool TestSoftFullScreen;
	StrBaseSwitch BaseSwitch;
	ANIMATIONINFO OldInfoAnimazione;
	bool TestCambiataAnim;
	StrPanelloSavegame *pBasePanello;
	StrBaseNG_Hub BaseHub_NG;
	StrSalvaSettings DefSettings;
	bool TestHubLara;
	bool TestHubLevel;
	bool TestAsSavegame; // se savegame o minihub
	StrShowImage BaseImages;
	bool TestFreezeAll;
	StrBasePrintText BaseParamPrint;
	bool TestFirstTime;
	StrBaseTestPosition BaseTestPosition;
	StrBaseScriptImages *pBaseScriptImages;
	StrBaseSetCamera BaseSetCamera;
	StrBasePreload BasePreloadImages;
	StrVersionHeader VersioneProject;
	StrCheatNG CheatNG;
	bool TestSetCamera;
	bool TestDisableCombatCamera;
	int IndiceSetCamera;
	StrVersionHeader BaseVersione;
	StrVersionHeader VersioneTr4;
	StrVersionHeader VersioneSavegame;
	StrMiniShot ScreenShot;
	StrAudioSospeso AudioSospeso;
	short TriggerGroupInProgress;  // usato per triggergroup lanciati da Animation
	StrBaseTriggerGroups *pBaseTriggerGroups;
	short ScrollingEndIndex;  // indice di stringa ng (senza 0x8000) appena terminata
	short ScrollingLastIndex; // indice di ng string con ultima riga visibile
	StrBaseItemGroup BaseItemGroup;	
	StrBaseColoraItem BaseColoraItem;
	short TestPosLastID;  // -1 o indice di ultimo testposition di script usato
	StrItemTr4 *TestPosLastItem; // item detected in ultima TestLaraPosition
	short TestPosLastIndex; // idice di item
	short TestPosOrientIdeal; // orientamento effetivo uato con test position ok
	DWORD TestPosXIdeal;	// usati solo con TPOS_ROUND_HORIENT
	DWORD TestPosZIdeal;	// ....
	bool TestFeetInQuickSand;
	StrAdjustAnim BaseAdjustAnim;
	char WindowNome[256];
	DWORD StartFromGame; // da inizio livello o da carica savegame in tick
	StrAdaptiveFarView BaseAdaptiveFar;
	StrPtrBars VetBarDefault[8];
	 StrBaseTimerOggetti BaseTimerOggetti;
	StrBaseMove BaseMoveItem;
	int TextCordXCarattere2;
	StrParBar ParallelBar;
	int TextCordYRiga2;
	StrBaseCollisione BaseCollisione;
	StrInfoChar VetFontInfos[256];
	float FontRapportoX;	// usato con flag FT_USE_FLOAT
	float FontRapportoY;
	float FontRemapLittleX;	// impostate a inizio print string
	float FontRemapLittleY; // per avere caratteri sempre ugualmente piccoli
	StrBaseTurbo BaseTurbo;
	StrBaseRotate BaseRotateItem;
	StrIndiciFont VetFontIndici[256];
	BYTE VetFontGraphics[256];
	bool TestFontNew;
	StrBaseSalvaStatic BaseSalvaStatic;
	StrBaseFPS BaseFPS;
	StrFontBaseSetting BaseFontBinary;
	StrBasePushables BasePushables;
	StrBaseColorRGB BaseColoriRGB;
	StrCercaStatic VetRemapStatics[6000+1]; 
	int TotScanFlipEffects;
	StrBaseImportFile BaseImportedFiles;
	StrEnemiesNotAimable BaseEnemiesNotAimable;
	WORD FlagsRoom;
	StrSFXSuonati BaseSfxPlayed;

	DWORD FlagsKeysOld;
	bool TestShootingInProgress;
	BYTE *pMemoriaTexture;
	int LastVerticalSpeed;
	StrTempRoomFlags TempRoomFlags;
	StrBassHandles BaseBassHandles;
	int TotScanActions;
	StrBaseDeadAnim BaseDeadAnim;
	StrSoundSettings SoundSettings;
	bool TestSecondoCrash;
	StrFlipMapRooms FlipMapRooms;
	StrBaseMovAdvance BaseMoveAdvance;
	StrBaseFlipSwap BaseMeshSwap;
	BaseNewObjects BaseIndiciNew;
	StrAnimDiag BaseDiagnosiAnim;
	StrAnimDiag BaseDiagnosiAnimSlot;
	StrBaseCustomize *pBaseCustomize;
	StrBaseCustomize *pBaseCustomizeDefault;
	BaseScriptOrganizer *pBaseOrganizer;
	StrItemTr4 *pTarget;
	WORD TestDisableFeatures;  // flags DF_...
	WORD OldDFPerCamera;  // flags DF_
	DWORD StatusNG;  // flags SNG_...
	StrBaseGlobalTriggers *pBaseGlobalTriggers;


	int TestNoDamageRollingBallIndex; // disattiva danni a lara con rolling ball
						// di valore indice
	StrBaseSalvaCollisioni  BaseSalvaCollisioni;
	StrBaseSalvaCollisioni  BaseSalvaOldCollisioni;
	bool TestDiagnosticaNow;  // attivato quando in watch.txt c'e' 
						// when = ON_FLAG_ATTIVO
	void *pAdrDiagnostica; // punta a memoria da controllare
					// quando tipo e' 
	int OperazioneNext;  // ON_...
	int OperazioneNext2;

	StrWideScreen BaseWideScreen;
	int OperazioneArg; // argomento per operazione next
	int OperazioneArg2; // usata per OperazioneNext2
	int OperazioneCount; // numero di tick prima di eseguire operazione
	StrBaseRemapTailInfo BaseRemapTail;
	StrBaseAnimTr4 BaseAnimTr4;
	StrBaseScriptEnvCondition  *pBaseEnvConditions;
	WORD TotPedane;
	StrBaseTexSequence BaseTexSequence;
	StrItemTr4 *VetPlatforms[1024];
	StrBaseEquipItem BaseEquipItem;
	StrGlobAddress *pAdr;
	WORD ClimbFlags;
	StrEnvPosition EnvPosition;
	StrHardwareB HardwareB;
	StrBaseParziali BaseTexParziali;
	StrDetector BaseDetector;
	int TotSizeDemoData;  // numero di suoni solo se tabella estesa
	StrScanAction VetScanActions[64];
	StrBaseEffects *pBaseEffects;
	int VetInventory[128];  // slot di item inventario	
	StrOldTrigger VetOldFlipEffects[MAX_OLD_FLIPEFFECT];
	int TotOldFlipEffects;
	StrOldTrigger VetOldActions[MAX_OLD_ACTIONS];
	int TotOldActions;
	StrFMV BaseFMV;
	HMODULE ModuloDll;
	short ScreenSizeX;  // dimensioni
	short ScreenSizeY; // di modo video
	StrOldTrigger VetOldCondizioni[MAX_OLD_CONDITIONS];
	int TotOldConditions;
	bool TestConditionFound; //basta che venga trovata una condizone nel ciclo game
	DWORD SaveTriggerButtons;  // per trigger condizione con extra buttons
	short ItemIndexCurrent;   // item che ha eseguito trigger attuale heavy
	short ItemIndexEnabledTrigger;  // ultimo item che ha eseguito` un trigger
	short ItemIndexLastMoved;  // ultimo item mosso
	short ItemIndexTgroup; // indice da usare per trigge group oppure -1
	short ItemIndexAnimCommand; // indice di item che ha eseguito un animcomamnd
	short IndiceItemCondizione; // item trovato in una condizione env positiva
								// on una collision globale
	short ItemIndexSelected; // indice stabilito con un trigger action
	short ItemIndexUsedByLara; // indice di oggetto con cui lara sta interagendo
	short CurrentSlotIndex; // usato per accedere a memoria slot
	short CurrentAnimationIndex; // usato per memoria di record animazione
	short CurrentInventoryIndex; // usato per memoria record inventario
	StrExtractNG HeaderNG_Savegame;
	StrExtractNG HeaderNG_Language;
	StrExtractNG HeaderNG_Script;
	WORD *pBaseFloorTriggerNow;
	bool TestBloccaCritical;
	int SalvaTestHeavy;
	bool TestSavegameConHeaderNG;
	short VetRemapObjects[6000];        // index=ngle index to get the tomb4 item
	short VetRemapInverseObjects[4096]; // index=tomb4 to get the ngle item
	bool TestTr4ConHeaderNG;
	StrDatiExtraAnimazioni TexAnimate;

	DWORD LastGameCommandTime;

	WORD *pVetSalvaSizeMesh;
	WORD TotDisabledMirrors;
	WORD VetDisabledMirrors[MAX_MIRRORS]; //numero stanza hidden
	char MexExtFMV[10];
	char LastTombMexage[1024];

	int LastTimerTrigger;
	StrBaseSalvaCords BaseSalvaCoordinate;
	WORD TotProgressiveActions;
	StrProgressiveAction VetProgressiveActions[MAX_ACTIONS];
	char SavegameLastName[256];
	int SavegameLastNumber;
	int KeysToSend;	//usano riferimenti CMD_..
	int KeysToStop;
	int KeysToStopBackup;  // valore precedente, usato per crossfade image

	bool TestTakeAwayWeapons;
	bool TestChangeTransparencyLara;
	DWORD TransparencyLaraIntensity;
	int KeySingleToSend; // usa riferimenti cmd_
	int KeysStopped; // valori che l'utente ha premuto ma sono stati bloccati
	BYTE VetKeysToStop[256]; // status di tasti singoli premuti ma bloccati
						// usato solo se keypad e' attivo

	bool TestNoUpdate; // se true impedisce ogni aggiornamento della superfice

	StrDatiXRain DatiRain;
	StrDatiXSnow DatiSnow;

	WORD TotExtraInfoRoom; // numero di stanze effettivamente presenti
	StrExtraInfoRoom VetExtraInfoRoom[0x400];
	int TotExtraStrings;
	StrExtraLangugage VetExtraStrings[0x400];  
	char LinguaNow[30];  // conterra' "italian" o "english" ecc.
	StrScriptOpzioni ScriptOptions; 
	StrScriptLevel *pScriptLevelNow;

	WORD FlagsLevelTr4;   // FLT_
	WORD Flags2LevelTr4;  // FLT2_
	StrPrintString PrintString;

	StrBaseDiagnostica *pDiagnostica;
	StrDamage DamageRoom;
	StrDamage DamageColdWater;
	StrDatiVariabili *pDatiVariabili; //level o lara
	StrDatiVariabili *pDatiVarLara; // solo lara
	StrJiga DatiJiga;
	StrBaseEnemyScript BaseEnemys;
	BaseAnimScript *pBaseAnimations;
	BaseMirrors BaseMirror;
	StrBaseStaticMIP BaseStaticMIP;
	StrBaseElevator BaseElevator;
	WORD Settings; // flag SET... di comando script Settings=
	StrKeyPad BaseKeyPad;

	bool TestDebuggerInProgress;

	short *pVehicleIndex;
	DWORD CordX;
	char *pMascheraSavegame;
	int  CordY;
	bool TestSuspendObjectShowing;
	WORD SlotSwitchKeyPad;
	DWORD CordZ;
	StrSlot *pSlotDefaultSprite;
	BaseAnimObjScript BaseAnimObj;
	StrEnvForEnemy BaseEnvEnemy; // extra check points when the env is for enemy
	StrSingleShotResumTG BaseTG_SingleShotResumed;
	StrBaseLightning BaseParamLightning;
	StrBaseImgMonoScreen BaseImgMonoScreen;  
	StrBaseImgBackGround BaseImgLoadingLevel;
	StrBaseImgBackGround BaseImgTitle;
	StrBaseImgBackGround BaseImgBinocular;
	StrBaseImgBackGround BaseImgLaserSight;
	COLORREF VetTextColors[9];  // colors used for print string with windowsfont (index to FC_ color constants)
	short  ColorWhiteStep;  // signed incrfement to change color FC_BIANCO_MOD (1), from white and black slowly
	short  ColorGradientNow; // from 0 to 255 to use for any gradient of pulsing white
	StrOffsetRanges VetStringOffsets[POFF_COUNTER]; // ranges of origin code for system strings 
	int TotStringOffset;  // number of ranges of above vector
	StrBaseParamWText BaseParamWText;
	StrBaseDgxErrors BaseDgxErrors;
	int TestStopNotifyThread;  // for sound audio track
	bool TestOldLaserSight; // in previous cycle it was enabled
	bool TestOldBinoculars;	 // in previous cycle it was enabled
	bool TestFirstLoadTitle; // true = showing load.bmp from boot strap game, otherwise 
							// coming back to title from some level
	bool TestWindowedModeRegister; // se nel registro era imposta modo windowed (se false=exlcusive full screen)
	StrDisablePushAway BaseDisablePushAway; // anim about when disable push away animation
	DWORD BackAdrOutputPolyList;  // to know from what zone has been called the outputpolylist
	StrBaseMyRect BaseParamMyRects;
	StrBaseInputBox BaseInputBoxes;
	bool TestStartDiary;  // required to show diary (waiting to be in main initpolylist()/outputpolylist() cycle)
	int DiaryIDToStart;   // id of diary to show 
	int DiaryPage;       // first page to show for diary
	DWORD LastAdrFloorData; // saved at begin of testtrigger, usato da catturaflipeffect
	GAME_VECTOR TargetBinoculars; // last point find before obstacle
	
	StrTiming Timing;  // used for performance analyses about timing
	int EmergencySettings; // ES_...
	bool TestForceSetup; 
	bool TestOverlapImage; // full screen image is currently drawn on screen
	StrGlobalBinoculars GlobBinoculars;
	SalvaSliderInput SliderMusic; // last input values , to fix problem in title custom image
	SalvaSliderInput SliderSfx;
	StrAnchoredBoat BoatAnchored; 
	DWORD LastTimeBinoculars; // to stop inventory when exiting from binoculars
	char *pMexLastTGCaller; // text about who called last time triggergroup() procedure (for diagnostic)
	StrItemTr4 *pItemAdrToIgnoreLOF;  // Adr structure to ignore in the objectonlos2() procedure (or NULL=0)
	StrRecording *pBaseDemo;  // to record or play demo
	StrMyDirectInput BaseDirectInputMine; // to temporize fake direct input outside of common game 
	StrDemo *pDemoTitle;
	StrDemo *pDemoLevel;
	StrDemo *pDemoNow;  // points to demolevel or demotitle or NULL if missing
	int DemoNumberLoaded; // current number of demo#.pak loaded in structure recording
	int DemoOnDisk;   // number of demo file in data folder
	WORD DemoOnDiskArray[1000]; // indices of demo#.pak files

	StrBaseCutscene *pBaseCutscene;  // data about cutscene and demos
	StrSalvaGlobFloor BaseSalvaFloor; // to save/restore some global variables affected from getheight/getceiling
	StrBaseSwapAnim *pBaseSwapAnim;
	StrBaseMemSwapAnim *pBaseMemSwapAnim; // to save/reload from savegame the last swapping to restore situation 
	StrBaseSpeechActor *pBaseSpeechActor; // to store PARAM_ACTOR_SPEECH
	bool TestEditingDemo; // if true demo is recording now or is playing after F10 key
	bool TestDisableDemoOrganizer; // if true while is playing or recording (in editing mode) the organizer will be ignore
	StrBaseFlipMesh BaseFlipMesh;
	StrCreatedItems BaseCreatedItems;
	int TotPluginNgle;
	StrPluginNames *pVetPluginNgle; // qui edevo cambiare definziione dimensione
	StrTablePluginIdFloor *pPluginFloorTable; // to discover the owner for each trigger in floordata
	StrTableScriptPlugin *pPluginScriptTable; // to discover the id of script commands (triggergroup, customize an parameters)

	StrAdrMemory *pVetMemorySavegame; // records about savegame memory  (ngle)
	StrAdrMemory *pVetItemMemory;     // records about item memory (ngle)
	StrAdrMemory *pVetCodeMemory;    // records about code memory (ngle)
	StrAdrMemory *pVetSlotMemory;      // records about slot memory (ngle)
	StrAdrMemory *pVetMemoryAnimation;  // records about animation memory (ngle)
	StrAdrMemory *pVetMemoryInventory;  // record about inventory memory (ngle)

	int *pVetMeshSwapSlot;
	int *pVetStandardSwapMesh;
	int *pVetSlotMeshMoveables;
	int TestOrizzonteVisibile;
	bool TestAlignmentInProgress;  // "true" there is a serie of calling to MoveLaraPosition() 
									// to align in front of ObjectActive
	StrGlobMisc *pMisc; // global miscellanous
	// check control value for 1.3.0.0 version
	DWORD CheckValue1_3_0_0;  // value will be 0x01234567

}GlobalFields;


// received when your plugin registers itself with trng
typedef struct StrTrngInfos {
	int IdMyPlugin;							// 00 (received) id of your plugin to use for each trng service
	StrGlobaliTomb4 *pGlobTomb4;			// 04 (received) address of StrGlobaliTomb4 in trng
	TYPE_RequireCallBack RequireCallBack;	// 08 (received) proc to require callback
	TYPE_SetNewPatch SetNewPatch;			// 0C (received) proc to set patch on tomb4.exe
	TYPE_Service Service;					// 10 (received) proc to require trng service
	void *pDirectCallBack;                  // 14 (sent) proc used from trng to call directly plugin code
	DWORD AdrTomb4Patcher;					// 18 (sent) Tomb4 offset of DWORD where to store che address of dll function MainPatcher
	void *pAdrDllPatcher;					// 1C (sent) Dll address of your MainPatcher function
	int  TestDebugMode;						// 20 (sent) to set 1 if debug version, 0 if release version
	DWORD MainPluginFlags;					// 24 (sent) MPS_ values set in Plugin= script command from user
	int TotPlugins;							// 28 total amout of loaded plugins (enclosed yours and tomb_nextgeneration.dll)
	char **pVetPluginNames;					// 2C vector with all plugin names (in format Plugin_Name )
	char *pMyPluginName;					// 30  the name of your plugin, only temporarily, then you find your
											//		plugin name in global variable TexMyPluginName[]
}TrngInfosFields;							

typedef struct StrLaraInfo {
	bool TestFreeHands;   // == true when she is NOT holding items in her hands, or  wall, ceiling, driving vehicle ect
	bool TestIsHoldingItem;  // == true when lara is holding weapons, torch or flare. In HoldedItem the HOLD_ type of item
	bool TestIsHoldingWeapon; // == true when lara is holding a weapon
	int  WeaponSelected;  // HOLD_ code (only weapons) for weapon that it will be drawn when player press SPACE bar. It doens't mean that lara has this weapon in her hands currently.
	int  HoldedItem;     // HOLD_ value but only for weapons, torch and flare (not special HOLD_ like vehicles or crowbar)
	bool TestIsHoldingCrowBar; // the crowbar has not seen as a common item, so you can detect its presence only from this Test value
	int  FlareTime;           // when the HoldedItem is HOLD_FLARE in FlareTime there will be the remaning time for light flare
	bool TestBurningTorch;   // == true when lara holds a burning torch
	bool TestIsDrivingVehicle;   // == true when lara is driving a vehicle
	int  SlotVehicle;	  // slot of vehicle lara is driving (or 0)
	int  IndexOfVehicle;  // index of vehicle (only on reading) that lara is driving (or -1 if it's missing)
	bool TestIsOnRope;    // lara is hanged on the rope with index = RopeIndex
	int  RopeIndex;		  // index of rope where lara is hanged
	bool TestIsOnPoleRope;  // == true when lara is hanged on pole-rope
	int  PoleRopeIndex;     // index of pole-rope item where lara is hanged (or -1)
	bool TestIsClimbing;    // == true lara is climbing a wall (note: to be hanged to the border of a wall is not climbing)
	bool TestIsMonkeying;   // == true lara is hanged on ceiling like a monky
	bool TestIsOnAllFour;   // == true lara is on all fours. Note: when lara is only knelt it's not on all fours, in that case use TestIsDucking
	bool TestIsDucking;		// == true when lara is knelt
	bool TestIsBurning;		// == true when lara is burning 
	bool TestIsImmortal;	// == true when lara is not killable
	bool TestIsPoisoned;	// == true when lara has been poisoned
	bool TestIsDripping;	// == true when lara is dripping  
	bool TestIsFalling;		// == true when lara is falling down or jumping
	bool TestIsRushing;		// == true when lara is using faster run. In RushValue you find the time from when she's rushing in game frames (max 120 frames, 4 seconds)
	bool TestIsUsingLaserSight;  // == true when lara is looking with laser-sigh
	bool TestIsUsingBinoculars;  // = true when lara is looking with binoculars
	DWORD SkipPhaseFlags;    // flag enumSKIP about current special game dynamics (binoculars, lasersight view, flycam camera ect)
	bool TestIsAimingEnemy;  // = true lara is in combat mode and is aiming an enemy. The structure pointer of this enemy will be stored in StrAimedEnemy field
	StrItemTr4 *  pStrAimedEnemy;  // pointer StrItemTr4 structure,  of baddy aimed by lara.
	bool TestIsPushingItem;   // = true when lara is pushing a pushable object. In PushableIndex the index of item, and in PushableSlot the slot of pushed item.
	bool TestIsPullingItem;   // = true when lara is pulling a pushable object. In PushableIndex the index of item, and in PushableSlot the slot of pulled item.
	int  PushableSlot;		// slot of pushed or pulled item if TestIsPushingItem or TestIsPullingItem is true
	int  PushableIndex;		// 	// index of pushed or pulled item if TestIsPushingItem or TestIsPullingItem is true
	bool TestEnvGround;		// lara is outside of water and is not using vehicles
	bool TestEnvFloatingOnWater;  // lara is floating on water surface. 
	bool TestEnvUnderwater;   // lara is totally under the water
	bool TestEnvLowWater;     // lara is walking in very low water (not floating)
	bool TestEnvSpecial;     // lara is dozy, on vehicle or (to verify) hanged to rope, polerope, ect.
	int  AirValue;			// air yet available, max value 1800
	int  RushValue;			// this value is the time, in frames (30th of seconds) from when lara is rushing. Max value =120 frames (4 seconds) and the it will decrease
	WORD OrientAligned; // more closed Orient to 8 perfect directions: hortogonal or diagonal
	int  OrientGap;   // difference between aligned orienting and real Lara's orienting
}LaraInfoFields;

typedef struct StrInput {
	DWORD GameCommandsRead; // CMD_ values to test with & .Only to read (test) input of game commands in main game cycle
	WORD *pGameCommandsWrite; // to write game commands, use | operator to add a CMD_ command
	DWORD InventoryCommandsRead; // input commands sent in inventory cycle
	BYTE *VetScanCode; // access with VetScanCode[ScanCode] > 0 = key enabled
}InputFields;

typedef struct StrScriptSettings {
	bool TestDiagnostic;  // == true when diagnostic is enabled
	char *pFMV_Extension;  // text of "FMV="	script command in [PCExtensions] section
	DWORD FlagsMain;        // SMAIN_... flags (option section) only for read 
	DWORD FlagsLevel;		// SLEV_ ... flags (level section) only for read
	DWORD DiagnosticEDGX;   // EDGX_ flags
	DWORD DiagnosticDGX;    // DGX_ flag types
}ScriptSettingsFields;

typedef struct StrGameInfo {
	bool TestVolumetric; // == true then volumetric fog is enabled
	int LevelIndex;     // index of current level. 0= title, 1 = first game level in script.dat, 2= second game level ect.
	DWORD FrameCounter;  // main timer in frames about game mode. its value will be updated 30 times for second
	int ScreenSizeX;    // width in pixels of game screen resolution
	int ScreenSizeY;    // height in pixels of game screen resolution 
	int RowHeight;     // about the height in pixel of (old) standard character. This value will be used to have a proportional drawing of 2d elements on screen
	int AudioTrack;    // number of CD track from audio folder
	bool TestAudioLooped; // AudioTrack is playing endless way
	bool TestExclusiveVideo; // full screen exclusive, no other windows over tr screen allowed
	StrBaseVarAll *pVar; // pointer to all trng variables in game
}GameInfoFields;

typedef struct StrGenericCustomize {
	WORD CustValue; // CUST_.. value 
	WORD NArguments; // number of arguments
	short *pVetArg; // pointer (dynamically allocaedte to free at begin of next level) with all arguments of this customize
}GenericCustomizeFields;

typedef struct StrBaseGenericCustomize {
	int TotCustomize;
	StrGenericCustomize *pVetCustomize; 
}BaseGenericCustomizefields;

typedef struct StrGenericParameters {
	WORD ParamValue; // PARAM_.. value 
	WORD NArguments; // number of arguments
	short *pVetArg; // pointer (dynamically allocaedte to free at begin of next level) with all arguments of this Parameters=
}GenericParametersFields;

typedef struct StrBaseGenericParameters {
	int TotParameters;
	StrGenericParameters *pVetParameters; 
}BaseGenericParametersfields;

typedef struct StrInfoItem {
	bool TestEnabled; // currently enabled
	bool TestSemiGod; // creature unkillable
	bool TestOnlyExplode; // creature killable only with explosive ammos (skeleton, mummy)
	bool TestCreature; // moveable is item with AI_ skills
	bool TestCollisions; // moveable has collisions
	bool TestDoor;  // moveable is a door, in this case when Testenabled = true the door is open, otherwise close
}InfoItemFields;

typedef struct StrInventoryData {
	StrInventoryItems *pItemPresence; //  presence or quantity of all inventory items 
	int *pChosenItemSlot; // slot of item chosen from inventory
	int *pRequiredItemSlot; // slot of inventory item required in game (like crowbar, or jeep's key)
	int ReservedFutureUse[40]; // place folders for new (further) inventory variables
} InfoInventoryFields;

typedef struct StrMemSelItems {
	short *pItemSelected; // points to index of current moveable item used for following memory variable operations
	short *pIventorySelected; //  points to index of current selected inventory record used for following memory variable operations
	short *pSlotSelected; //  points to index of current selected slot reccord used for following memory variable operations
	short *pAnimationSelected; //  points to index of current selected Animation record used for following memory variable triggers

}MemSelItemsFields;

typedef struct StrVariablesData {
	StrMemSelItems *pMemorySelected;  // point to structure with currently selected indices for all memory types
	StrBaseVarAll *pTrngVars;  // point to structure with all trng variables: global and local, 
							// numerical and testual, divided by groups in sub structures
	int Reserved[40];  // placefodler for future developments
}VariablesDataFields;

typedef struct StrFlipMap {
	int *pVetEnabledFlipMaps; // if flipmapindex is enabled pVetEnabledFlipMaps[FlipMapIndex] will be == 1, otherwise = 0
	int *pVetButtonFlipMaps;  // currently trigger activation flags already enabled for given flipmapindex (0x3E00 means all activation buttons)
							// Remind: the index used to access on above array, will be a FLIPMAP index and not a room index
	int TotFlipRooms;  // number of all rooms . 
	short *pVetFlipRooms; // array used to discover what main (not flipped) room is linked with FlipRoomIndex. 
						 // example if you have an index of a room (RoomIndex) and you wish know if this room is 
						// the flipped room of anther mainroom and what you use the code:
						// MainRoomIndex = pVetFlipRooms[RoomIndex];
						// if the room (whose you use index, in RoomIndex in above example) is NOT a flipped room, in MainRoomIndex there will be -1

}FlipMapFields;

// global structure with returned values by Get() locator
typedef struct StrGetLocator {
	StrItemTr4 *pLara;
	int  LaraIndex; 
	StrItemTr4 *pItem;
	StrMeshInfo *pStatic;
	StrRoomTr4 *pRoom;
	StrBoxCollisione *pCollItem;
	StrBoxCollisione *pCollStatic;
	StrBoxCollisione *pViewStatic;
	structStrDoorTr4 *pDoor;
	StrLaraInfo LaraInfo;
	StrGenericParameters *pParam;
	StrGenericCustomize *pCust;
	StrFlyByTr4 *pCameraFly;
	StrCameraTr4 *pCamera;
	StrAIDataTr4 *pAI;
	StrInput Input;
	char * pStringNG;
	StrLigthTr4 *pLight;
	char * pString;
	int BigNumber;
	StrProgressiveAction *pAction;
	StrScriptSettings ScriptSettings;
	COLORREF Color;
	StrGameInfo GameInfo;
	StrSlot *pSlot;
	StrInventoryItems *pInventory;
	StrAnimationTr4 *pAnimation;
	StrInfoItem InfoItem;
	StrInventoryData InventoryData;
	StrVariablesData Vars;
	StrFlipMap *pFlipMap;
}GetLocatorFields;

typedef struct StrLOFData {
	StrMeshInfo *pStaticFound; // if different than NULL, this is the static in the middle of Line Of Fire
	StrItemTr4* pItemFound;  // if different than NULL this is the moveable item in the middle of Line Of Fire
	StrPosizione PointFinal; // final point (x,y,z) in Line Of Fire
	WORD OrientingH; // direction between Source and Target item on X,Z plane
	WORD OrientingV; // angle on vertical axis. Valid value only if you set valid value for TolleranceV
	bool TestFreeLine; // if true there are NO obstacles in lof
	bool TestWall; // if true obstacle is a wall ( or ceiling, but no item)
	GAME_VECTOR Src;  // source point
	GAME_VECTOR Dest;  // target point
}LOFDataFields;

typedef struct StrDoubleIndices {
	WORD RoomIndex;
	WORD ObjIndex;
}DoubleIndicesFields;

typedef struct StrDgxFound {
		WORD FlagsDgx; // DGX_.. flags
		WORD DgxExtra; // EDGX_.. flags
		StrLogItem *pLogItem; // pointer to data of LogItem= script command
}DgxFoundFields;


typedef struct StrFind {
	int TotItems;
	WORD VetItems[1024];
	int TotStatics;
	StrDoubleIndices VetStatics[100];
	int TotLights;
	StrDoubleIndices VetLights[256];
	int TotAI;
	WORD VetAI[256];
	int TotCameraFix;
	WORD VetCameraFix[100];
	int TotCameraFly;
	WORD VetCameraFly[128];
	int TotProgAxnTrng;
	StrProgressiveAction *pVetProgAxnTrng[100];
	int TotProgAxnMine;
	StrProgressiveAction *pVetProgAxnMine[100];
	StrRecordEnemyScript *pEnemy;
	StrScriptImage *pImage;
	StrWindowsFont *pWindowsFont;
	StrDgxFound Dgx;
	short SlotAssigned; // or -1 if not found
}FindFields;

typedef struct StrSectorCoords {
	DWORD WestZ;   // lower Z coordinate (west side of the sector)
	DWORD EastZ;   // higher Z coordinate (east side of the sector)
	DWORD NorthX;  // lower X coordinate (north side of the sector)
	DWORD SouthX;  // higher X coordinate (south side of the sector)
	DWORD MiddleX;  // middle point of the sector
	DWORD MiddleZ;  // x and z coordinate
	int Radius; // distance from middle point of sector respect to the source point analysed in CheckFloor()
}SectorCoordsFields;


typedef struct StrFloorAnalyse {
	bool TestFullWall; // the point is inside wall: no other result will be meaningful
	bool TestGraySector; // the point is on gray sector forbidden to enemies
	bool TestClimb; // == true if there one wall with climb state around current sector, see ClimbStatus for more info
	bool TestMonkey;  // == true monkey ceiling over this sector
	bool TestDeath;  // == true death (lava) type on the floor of this sector
	WORD ClimbStatus;  // CLIMB_ values or 0 if missing climb walls in current sector
	int FloorHeight;  // Y Coordinate of floor (higher side)
	int CeilingHeight; // Y coordinatre of ceiling (lower side)
	int WaterDepth;  // Depth of the water in the vertical of input point
	int WaterHeight;  // y coordinate of water surface
	short RoomIndex; // the room where is the given point
	StrSectorCoords SectorCoords; // absolute coordinates of boundary and the middle point of the sector
	WORD *pFloor; // pointer to floor data about point, to use as input for other low-level functions
	int SlopeType; // SLOPE_ values to describe if the sector is flat or a slope and what kind
	short SlopeOrienting; // ORIENT_ values to describe the direction of the rise
	int SlopeClickGrade; // max difference in clicks between sides/corners
	int SlopeX;  // click difference about height on X axis on higher X values (south side on ngle view)
	int SlopeZ;  // click difference about height on Z axis on higher Z values (east side on ngle view)
}FloorAnalyseFields;
// #TAG_START_AUTO_ENUM_STUFF#
// ----------------  ENUM SHORTCUT FOR D3D_ -----------------
typedef struct StrEnumD3D {
	int NO_LIGHT;
	int SUN;
	int LIGHT;
	int SPOT_OR_EFFECT;
	int SHADOW;
	int FOG_BULB;
} StuffEnumD3D;

// ----------------  ENUM SHORTCUT FOR COLF_ -----------------
typedef struct StrEnumCOLF {
	int WINDOWS_COLOR;	// format 0x00RRGGBB windows functions requires this format
	int TOMB_COLOR;	// format 0x00BBGGRR tomb4 functions requirs this format
} StuffEnumCOLF;

// ----------------  ENUM SHORTCUT FOR FIL_ -----------------
typedef struct StrEnumFIL {
	int FROM_SAVEGAME;
	int FROM_NEW_LEVEL;	// level started from new game of title or with level jump and resethub
	int FROM_LEVEL_JUMP;	// when there is a level jump (note: if resethub was enabled, this flag will be missing)
	int PRESERVE_LARA;	// this flag is present: from savegame or from level jump (but no resethub)
	int PRESERVE_LEVEL;	// this flag is present: from savegame or from go and back for level jump and no resethub
	int FROM_LIKE_SAVEGAME;	// this flag is enabled if level is from savegame or from level jump with no reset hub and go and back from two levels.
} StuffEnumFIL;

// ----------------  ENUM SHORTCUT FOR SAVT_ -----------------
typedef struct StrEnumSAVT {
	int LOCAL_DATA;	// require to save only local data
	int GLOBAL_DATA;	// require to save only global data
	int COMPLETED;	// signal only to have completed the saving, now our callback can free temporary memory to store data to save
} StuffEnumSAVT;

// ----------------  ENUM SHORTCUT FOR CONV_ -----------------
typedef struct StrEnumCONV {
	int ItemIndexFromNgleToTomb;	// Input: Index= Ngle index. Output: returns the tomb4 corresponding index
	int ItemIndexFromTombToNgle;	// Input: Index= Tomb Index. Output: returns the corresponding ngle index
	int StaticIndexFromNgleToTomb;	// Input: Index= Ngle Static Index / pPointer = &ReturnedStaticIndex. Output: returns: Room Index  / *pPointer = (int) Tomb Static Index
	int StaticIndexFromTombToNgle;	// Input: Index=RoomIndex  / SecondaryIndex=StaticIndex. Output: returns ngle static index
	int RectFromMicroUnitsToPixels;	// input: pPointer = &RECT with microunits Output: changed value in rect pointed by pPointer
	int AnimIndexFromRelativeToAbs;	// input: Index=Relative anim index / pPointer: pointer to item structure. Output: returns Absolute animation index
	int AnimIndexFromAbsToRelative;	// input: Index=Absolute anim index / pPointer: pointer to item structure. Output: returnns Relative animation index
	int ItemFromStrItemTr4ToIndex;	// input: pPointer = StrItemTr4 *pItem. Output: returns item index of given structure
	int RoomIndexFromNgleToTomb;	// input: Index=NgleRoomIndex. Output: returns Tomb room index
	int RoomIndexFromTombToNgle;	// input: Index=TombRoomIndex, Output: returns Nlge room index
	int FrameIndexFromAbsToRelative;	// input: Index=AbsFrameIndex / SecondaryIndex=AbsAnimationIndex. Output: returns RelativeFrameIndex
	int FrameIndexFromRelativeToAbs;	// input: Index=RelFrameIndex / SecondaryIndex=AbsAnimationIndex. Output: returns AbsoluteFrameIndex
} StuffEnumCONV;

// ----------------  ENUM SHORTCUT FOR FL_ -----------------
typedef struct StrEnumFL {
	int HOLDS_FLARE_OR_TORCH;
	int UNKNOWN_02;	// never used in tomb4
	int SPECIAL_STATE;	// not clear the meaning
	int IS_BURNING;
	int ON_ALL_FOUR;	// not sure
	int SELF_ALIGNMENT;	// not sure
	int UNDER_MONKEY_SECTOR;
} StuffEnumFL;

// ----------------  ENUM SHORTCUT FOR RIB_ -----------------
typedef struct StrEnumRIB {
	int ONLY_DIGITS;	// ignores letters (ABCDEF ...Z) (by default accepts letters and digits)
	int ONLY_LETTERS;	// ignores digits (0123456789)  (by default accepts letters and digits)
	int HIDE_CARET;	// by default caret is visible
	int BLINK_CARET;	// perform a blinking of caret while it's waiting input
	int SOUND_ON_KEY;	// little sound for each typed character
	int PRINT_ONLY;	// ignore input and print newly the current buffer in given rect (works like OutText() only)
	int ALIGN_CENTER;	// center aligment in pRect boundary. (by default uses a LEFT align)
	int ONLY_CAPS;	// force capital letters. By default it will be typed lower or capital letters
	int PRINT_BIG_TEXT;	// start printing the "TextBig" trng variable
	int INPUT_BELOW_BIG_TEXT;	// require input in Y position after big text
	int ADD_SCANCODE_LIST;	// parses extra scan codes supplied in extra ng string 666
	int SHORTCUT_KEY;	// just typing a single (valid) key to have confirmed the input like with ENTER
	int HIDE_TEXT;	// not draw the inserted text but manage it as usual
} StuffEnumRIB;

// ----------------  ENUM SHORTCUT FOR WTF_ -----------------
typedef struct StrEnumWTF {
	int FLYING_TEXT;
	int PULSING_TEXT;
	int CHANGE_COLOR;
	int OVER_INVENTORY;
	int OVER_IMAGE;
	int OVER_FLYCAMERA;
	int OVER_FIXCAMERA;
	int OVER_BINOCULAR;
	int OVER_LASER_SIGHT;
} StuffEnumWTF;

// ----------------  ENUM SHORTCUT FOR FT_ -----------------
typedef struct StrEnumFT {
	int HALF_SIZEX;
	int HALF_SIZEY;
	int DOUBLE_SIZEX;
	int DOUBLE_SIZEY;
	int LITTLE_ALWAYS;	// caratteri forzati sempre a dimensione ok per keypad
	int USE_FLOAT;	// usa i valori di rapporto globali
	int NO_BORDERS;
	int SIZE_ATOMIC_CHAR;
} StuffEnumFT;

// ----------------  ENUM SHORTCUT FOR FTS_ -----------------
typedef struct StrEnumFTS {
	int ALIGN_LEFT;
	int STRETCH_TEXT;
	int BLINK;
	int ALIGN_RIGHT;
	int ALIGN_CENTER;
} StuffEnumFTS;

// ----------------  ENUM SHORTCUT FOR FC_ -----------------
typedef struct StrEnumFC {
	int WHITE_PULSE;
	int WHITE;
	int RED;
	int BLUE;
	int LIGHT_GRAY;
	int GOLD;
	int DARK_GRAY;
	int YELLOW;
} StuffEnumFC;

// ----------------  ENUM SHORTCUT FOR PRET_ -----------------
typedef struct StrEnumPRET {
	int OK;	// come back to game phase, the pause menu has been handled by this callback
	int GO_TO_TITLE;	// player selected "exit to title"
	int EXECUTE_ORIGINAL;	// perform original pause management
} StuffEnumPRET;

// ----------------  ENUM SHORTCUT FOR PHASE_ -----------------
typedef struct StrEnumPHASE {
	int INVENTORY_MAIN;	// main management of inventory
	int PAUSE_SCREEN;
	int LOAD_GAME_PANEL;
	int SAVE_GAME_PANEL;
	int OPTIONS;
	int OPTION_COMMANDS;
	int STATISTICS;
	int NEW_LEVEL;
	int TITLE_MENU;	// main screen of title menu
	int INVENTORY_EXAMINE;
} StuffEnumPHASE;

// ----------------  ENUM SHORTCUT FOR IRET_ -----------------
typedef struct StrEnumIRET {
	int OK;	// all fine, go on with common callback managemnt
	int SKIP_ORIGINAL;	// skip original management, valid only for CBT_FIRST callback
	int LOADED_GAME;	// valid only for CBT_REPLACE or CBT_AFTER callbacks.
} StuffEnumIRET;

// ----------------  ENUM SHORTCUT FOR CPU_ -----------------
typedef struct StrEnumCPU {
	int CARRY;
	int PARITY;
	int ZERO;
	int SIGN;
} StuffEnumCPU;

// ----------------  ENUM SHORTCUT FOR APPC_ -----------------
typedef struct StrEnumAPPC {
	int OK;	// patch it has been applied succesfully
	int WARN_PATCH_ALREADY_PERFORMED;	// same code patch performed two or more times same patch
	int WARN_PLUGIN_REMOVED_RELOCATION_OF_PROCEDURE;	// removed rilocation of procedure
	int WARN_PATCH_IN_A_RILOC_RESTORED_PROCEDURE;	// typed code in a rilocated and then restored procedure
	int WARN_PATCH_OVER_OLD_PATCH;	// patch wrote over another old patch
	int WARN_PATCH_OVER_CHANGED_ARGUMENTS;	// patch in code where some arguments have been previously changed
	int ERROR_PLUGIN_ID_UNREGISTERED;	// the supplied plugin id is wrong (unknown)
	int ERROR_PATCH_OVERLAPPED_TO_RUNTIME_PATCH;	// attempt to overwrite a run-time patch
	int ERROR_PATCH_IN_RILOCATED_PROCEDURE;	// patch is writing in an unplugged code where it was been removed (rilocated) a procedure
	int ERROR_NOT_FOUND_TOMB4_IMAGE;	// restore patch is not available for an internal error (this should never happen)
	int ERROR_RESTORE_ZONE_OUTSIDE_CODE_RANGE;	// the zone to restore is not inside of tomb4 code zone available for restoring (0x401000 - 0x4A6FFF)
	int ERROR_WRONG_PATCH_FOR_RUN_TIME;	// only parametric constant patches are admitted in run time mode
	int ERROR_PATCH_OUTSIDE_CODE_SEGMENT;	// it's not possible set code patch outside of code segment (0x401000 - 0x4A6FFF)
	int ERROR_PATCH_OVERLAPPED_TO_RESERVED_ZONE;	// a code patch tried to write over a reserved memory data
	int ERROR_RESERVED_ZONE_ALREADY_USED;	// the zone you required to reserved for your plugin had already used by other plugins
} StuffEnumAPPC;

// ----------------  ENUM SHORTCUT FOR CB_ -----------------
typedef struct StrEnumCB {
	int INIT_PROGRAM;	// DEFAULT. You have already it. Look for "cbInitProgram" procedure in Plugin_trng.cpp source
	int SAVING_GAME;	// DEFAULT. You have already it. Look for "cbSaveMyData" procedure in Plugin_trng.cpp source
	int LOADING_GAME;	// DEFAULT. You have already it. Look for "cbLoadMyData" procedure in Plugin_trng.cpp
	int INIT_GAME;	// DEFAULT. You have already it. Look for "cbInitGame" procedure in Plugin_trng.cpp source
	int INIT_LOAD_NEW_LEVEL;	// DEFAULT. You have already it. Look for "cbInitLoadNewLevel" procedure in Pluging_trng.cpp source
	int FLIPEFFECT_MINE;	// DEFAULT. You have already it. Look for "cbFlipEffectMine" procedure in Pluging_trng.cpp source
	int ACTION_MINE;	// DEFAULT. You have already it. Look for "cbActionMine" procedure in Pluging_trng.cpp source
	int CONDITION_MINE;	// DEFAULT. You have already it. Look for "cbConditionMine" procedure in Pluging_trng.cpp source
	int CUSTOMIZE_MINE;	// DEFAULT. You have already it. Look for "cbCustomizeMine" procedure in Pluging_trng.cpp source
	int PARAMETER_MINE;	// DEFAULT. You have already it. Look for "cbParametersMine" procedure in Pluging_trng.cpp source
	int CYCLE_BEGIN;	// DEFAULT. You have already it. Look for "cbCycleBegin" procedure in Plugin_trng.cpp
	int CYCLE_END;	// callback called at end of every game cycle
	int GLOBAL_TRIGGER;	// callback to give the result about a global trigger GT_... (yours or old of trng)
	int INIT_OBJECTS;	// it will be called just completed the loading of object slots from tr4 level. (CALL_VOID) You can use it to initialise your objects (moveables) or to change the settings of other moveables
	int PROGR_ACTION_MINE;	// DEFAULT: You have already it. Look for "cbProgrActionMine" procedure in Plugin_trng.cpp source
	int PROGR_ACTION_DRAW_MINE;	// callback to perform your progressive actions that requires a direct draw on screen (meshes, images, windows font texts, sprites)
	int INIT_LEVEL;	// DEFAULT: you have already it. Look for "cbInitLevel" procedure. it will be performed just a moment before entering in main game cycle. All items have already been initialized
	int COMPLETED_PROGR_ACTION;	// callback called when a trng progressive action has been just completed.
	int VEHICLE;	// callback for vehicle management
	int ASSIGN_SLOT_MINE;	// DEFAULT: You have already it. Look for "cbAssignSlotMine() procedure. It receives and stores all AssignSlot= data about your OBJ_ values
	int FMV_MANAGER;	// callback to replace playing fmv management
	int INPUT_MANAGER;	// callback to read in advance input command and, optionally remove them
	int SAVEGAME_MANAGER;	// callback for procedure to handle savegame list to load/save savegames
	int PAUSE_MANAGER;	// callback to handle pause menu phase. it works as a replace callback
	int STATISTICS_MANAGER;	// callback to the procedure to show statistics screen.
	int TITLE_MENU_MANAGER;	// callback for the menu of title level
	int WINDOWS_FONT_CREATE;	// callback to replace the creation of windows font for printing window text
	int WINDOWS_UNICODE_CONVERT;	// callback to convert from text of script to unicode text for windows text printing
	int WINDOWS_TEXT_PRINT;	// callback to replace print windows text trng function
	int DIAGNOSTIC;	// callback to add own diagnostic on screen or to monitor current trng diagnostic text
	int LARA_CONTROL;	// callback for main control procedure of lara that swap environment:
	int LARA_DRAW;	// callback for draw lara procedure
	int LARA_HAIR_DRAW;	// callback for draw hair procedure
	int LARA_HAIR_CONTROL;	// callback for control hair procedure
	int INVENTORY_MAIN;	// callback for procedure that shows inventory and handles it
	int INVENT_BACKGROUND_CREATE;	// callback for procedure ( CreateMonoScreen) that creates a backgroup (allocating resources if necessary)
	int INVENT_BACKGROUND_DRAW;	// callback for procedure (S_DisplayMonoScreen) that updates graphic for background
	int INVENT_BACKGROUND_QUIT;	// callback for procedure (FreeMonoScreen) that free futher resource allocated by BACKGROUND_CREATE
	int ANIMATE_LARA;	// callback for procedure AnimateLaran that update frames and change animations of lara
	int OPTIONS_MANAGER;	// callback to the procedure to show/edit options
	int FLIPEFFECT;	// callback for a tomb_nextgeneration (old, not yours) FLIPEFFECT trigger
	int ACTION;	// callback for a tomb_nextgeneration (old, not yours) ACTION trigger
	int CONDITION;	// callback for a tomb_nextgeneration (old, not yours) CONDITION trigger. WARNING: condition triggers have a particular management, see infos about this kind of callback
	int VEHICLE_CONTROL;	// calback for vehicles not handled by common slot control procedures
	int PROGR_ACTION;	// callback for a tomb_nextgeneration (old, not yours) PROGRESSIVE ACTION. Type the AZ_ constant to choose it.
	int NUMERIC_TRNG_PATCH;	// callback affects a given MOV AX , Number  / Call dword [TrngPatcher]  patch of trng (see tomb4 patched code source for their position and target)
	int SLOT_INITIALISE;	// callboack for Initialise procedure of given slot object
	int SLOT_CONTROL;	// callback for Control procedure of given slot object
	int SLOT_COLLISION;	// callback for Collision procedure of given slot object
	int SLOT_DRAW;	// callback for Draw procedure of given slot object
	int SLOT_FLOOR;	// callback for floor procedure of given slot object
	int SLOT_CEILING;	// callback fro ceiling procedure of given slot object
	int SLOT_DRAW_EXTRA;	// callback for drawextra procedure of given slot object
	int STATE_ID_LARA_CTRL;	// callback for control state-id lara, parameter the state-id
	int STATE_ID_LARA_COLLISION;	// callbacks for collision state-id lara, parameter the state-id
} StuffEnumCB;

// ----------------  ENUM SHORTCUT FOR SLOT_ -----------------
typedef struct StrEnumSLOT {
	int LARA;	// [0] slot
	int PISTOLS_ANIM;	// [1] slot
	int UZI_ANIM;	// [2] slot
	int SHOTGUN_ANIM;	// [3] slot
	int CROSSBOW_ANIM;	// [4] slot
	int GRENADE_GUN_ANIM;	// [5] slot
	int SIXSHOOTER_ANIM;	// [6] slot
	int FLARE_ANIM;	// [7] slot
	int LARA_SKIN;	// [8] slot
	int LARA_SKIN_JOINTS;	// [9] slot
	int LARA_SCREAM;	// [10] slot
	int LARA_CROSSBOW_LASER;	// [11] slot
	int LARA_REVOLVER_LASER;	// [12] slot
	int LARA_HOLSTERS;	// [13] slot
	int LARA_HOLSTERS_PISTOLS;	// [14] slot
	int LARA_HOLSTERS_UZIS;	// [15] slot
	int LARA_HOLSTERS_SIXSHOOTER;	// [16] slot
	int LARA_SPEECH_HEAD1;	// [17] slot
	int LARA_SPEECH_HEAD2;	// [18] slot
	int LARA_SPEECH_HEAD3;	// [19] slot
	int LARA_SPEECH_HEAD4;	// [20] slot
	int ACTOR1_SPEECH_HEAD1;	// [21] slot
	int ACTOR1_SPEECH_HEAD2;	// [22] slot
	int ACTOR2_SPEECH_HEAD1;	// [23] slot
	int ACTOR2_SPEECH_HEAD2;	// [24] slot
	int LARA_WATER_MESH;	// [25] slot
	int LARA_PETROL_MESH;	// [26] slot
	int LARA_DIRT_MESH;	// [27] slot
	int CROWBAR_ANIM;	// [28] slot
	int TORCH_ANIM;	// [29] slot
	int HAIR;	// [30] slot
	int MOTORBIKE;	// [31] slot
	int JEEP;	// [32] slot
	int VEHICLE_EXTRA;	// [33] slot
	int ENEMY_JEEP;	// [34] slot
	int SKELETON;	// [35] slot
	int SKELETON_MIP;	// [36] slot
	int GUIDE;	// [37] slot
	int GUIDE_MIP;	// [38] slot
	int VON_CROY;	// [39] slot
	int VON_CROY_MIP;	// [40] slot
	int BADDY_1;	// [41] slot
	int BADDY_1_MIP;	// [42] slot
	int BADDY_2;	// [43] slot
	int BADDY_2_MIP;	// [44] slot
	int SETHA;	// [45] slot
	int SETHA_MIP;	// [46] slot
	int MUMMY;	// [47] slot
	int MUMMY_MIP;	// [48] slot
	int SPHINX;	// [49] slot
	int SPHINX_MIP;	// [50] slot
	int CROCODILE;	// [51] slot
	int CROCODILE_MIP;	// [52] slot
	int HORSEMAN;	// [53] slot
	int HORSEMAN_MIP;	// [54] slot
	int SCORPION;	// [55] slot
	int SCORPION_MIP;	// [56] slot
	int JEAN_YVES;	// [57] slot
	int JEAN_YVES_MIP;	// [58] slot
	int TROOPS;	// [59] slot
	int TROOPS_MIP;	// [60] slot
	int KNIGHTS_TEMPLAR;	// [61] slot
	int KNIGHTS_TEMPLAR_MIP;	// [62] slot
	int MUTANT;	// [63] slot
	int MUTANT_MIP;	// [64] slot
	int HORSE;	// [65] slot
	int HORSE_MIP;	// [66] slot
	int BABOON_NORMAL;	// [67] slot
	int BABOON_NORMAL_MIP;	// [68] slot
	int BABOON_INV;	// [69] slot
	int BABOON_INV_MIP;	// [70] slot
	int BABOON_SILENT;	// [71] slot
	int BABOON_SILENT_MIP;	// [72] slot
	int WILD_BOAR;	// [73] slot
	int WILD_BOAR_MIP;	// [74] slot
	int HARPY;	// [75] slot
	int HARPY_MIP;	// [76] slot
	int DEMIGOD1;	// [77] slot
	int DEMIGOD1_MIP;	// [78] slot
	int DEMIGOD2;	// [79] slot
	int DEMIGOD2_MIP;	// [80] slot
	int DEMIGOD3;	// [81] slot
	int DEMIGOD3_MIP;	// [82] slot
	int LITTLE_BEETLE;	// [83] slot
	int BIG_BEETLE;	// [84] slot
	int BIG_BEETLE_MIP;	// [85] slot
	int WRAITH1;	// [86] slot
	int WRAITH2;	// [87] slot
	int WRAITH3;	// [88] slot
	int WRAITH4;	// [89] slot
	int BAT;	// [90] slot
	int DOG;	// [91] slot
	int DOG_MIP;	// [92] slot
	int HAMMERHEAD;	// [93] slot
	int HAMMERHEAD_MIP;	// [94] slot
	int SAS;	// [95] slot
	int SAS_MIP;	// [96] slot
	int SAS_DYING;	// [97] slot
	int SAS_DYING_MIP;	// [98] slot
	int SAS_CAPTAIN;	// [99] slot
	int SAS_CAPTAIN_MIP;	// [100] slot
	int SAS_DRAG_BLOKE;	// [101] slot
	int AHMET;	// [102] slot
	int AHMET_MIP;	// [103] slot
	int LARA_DOUBLE;	// [104] slot
	int LARA_DOUBLE_MIP;	// [105] slot
	int SMALL_SCORPION;	// [106] slot
	int LOCUST_EMITTER;	// [107] slot
	int GAME_PIECE1;	// [108] slot
	int GAME_PIECE2;	// [109] slot
	int GAME_PIECE3;	// [110] slot
	int ENEMY_PIECE;	// [111] slot
	int WHEEL_OF_FORTUNE;	// [112] slot
	int SCALES;	// [113] slot
	int DARTS;	// [114] slot
	int DART_EMITTER;	// [115] slot
	int HOMING_DART_EMITTER;	// [116] slot
	int FALLING_CEILING;	// [117] slot
	int FALLING_BLOCK;	// [118] slot
	int FALLING_BLOCK2;	// [119] slot
	int SMASHABLE_BIKE_WALL;	// [120] slot
	int SMASHABLE_BIKE_FLOOR;	// [121] slot
	int TRAPDOOR1;	// [122] slot
	int TRAPDOOR2;	// [123] slot
	int TRAPDOOR3;	// [124] slot
	int FLOOR_TRAPDOOR1;	// [125] slot
	int FLOOR_TRAPDOOR2;	// [126] slot
	int CEILING_TRAPDOOR1;	// [127] slot
	int CEILING_TRAPDOOR2;	// [128] slot
	int SCALING_TRAPDOOR;	// [129] slot
	int ROLLINGBALL;	// [130] slot
	int SPIKEY_FLOOR;	// [131] slot
	int TEETH_SPIKES;	// [132] slot
	int JOBY_SPIKES;	// [133] slot
	int SLICER_DICER;	// [134] slot
	int CHAIN;	// [135] slot
	int PLOUGH;	// [136] slot
	int STARGATE;	// [137] slot
	int HAMMER;	// [138] slot
	int BURNING_FLOOR;	// [139] slot
	int COG;	// [140] slot
	int SPIKEBALL;	// [141] slot
	int FLAME;	// [142] slot
	int FLAME_EMITTER;	// [143] slot
	int FLAME_EMITTER2;	// [144] slot
	int FLAME_EMITTER3;	// [145] slot
	int ROPE;	// [146] slot
	int FIREROPE;	// [147] slot
	int POLEROPE;	// [148] slot
	int ONEBLOCK_PLATFORM;	// [149] slot
	int TWOBLOCK_PLATFORM;	// [150] slot
	int RAISING_BLOCK1;	// [151] slot
	int RAISING_BLOCK2;	// [152] slot
	int EXPANDING_PLATFORM;	// [153] slot
	int SQUISHY_BLOCK1;	// [154] slot
	int SQUISHY_BLOCK2;	// [155] slot
	int PUSHABLE_OBJECT1;	// [156] slot
	int PUSHABLE_OBJECT2;	// [157] slot
	int PUSHABLE_OBJECT3;	// [158] slot
	int PUSHABLE_OBJECT4;	// [159] slot
	int PUSHABLE_OBJECT5;	// [160] slot
	int TRIPWIRE;	// [161] slot
	int SENTRY_GUN;	// [162] slot
	int MINE;	// [163] slot
	int MAPPER;	// [164] slot
	int OBELISK;	// [165] slot
	int FLOOR_4BLADE;	// [166] slot
	int ROOF_4BLADE;	// [167] slot
	int BIRD_BLADE;	// [168] slot
	int CATWALK_BLADE;	// [169] slot
	int MOVING_BLADE;	// [170] slot
	int PLINTH_BLADE;	// [171] slot
	int SETH_BLADE;	// [172] slot
	int LIGHTNING_CONDUCTOR;	// [173] slot
	int ELEMENT_PUZZLE;	// [174] slot
	int PUZZLE_ITEM1;	// [175] slot
	int PUZZLE_ITEM2;	// [176] slot
	int PUZZLE_ITEM3;	// [177] slot
	int PUZZLE_ITEM4;	// [178] slot
	int PUZZLE_ITEM5;	// [179] slot
	int PUZZLE_ITEM6;	// [180] slot
	int PUZZLE_ITEM7;	// [181] slot
	int PUZZLE_ITEM8;	// [182] slot
	int PUZZLE_ITEM9;	// [183] slot
	int PUZZLE_ITEM10;	// [184] slot
	int PUZZLE_ITEM11;	// [185] slot
	int PUZZLE_ITEM12;	// [186] slot
	int PUZZLE_ITEM1_COMBO1;	// [187] slot
	int PUZZLE_ITEM1_COMBO2;	// [188] slot
	int PUZZLE_ITEM2_COMBO1;	// [189] slot
	int PUZZLE_ITEM2_COMBO2;	// [190] slot
	int PUZZLE_ITEM3_COMBO1;	// [191] slot
	int PUZZLE_ITEM3_COMBO2;	// [192] slot
	int PUZZLE_ITEM4_COMBO1;	// [193] slot
	int PUZZLE_ITEM4_COMBO2;	// [194] slot
	int PUZZLE_ITEM5_COMBO1;	// [195] slot
	int PUZZLE_ITEM5_COMBO2;	// [196] slot
	int PUZZLE_ITEM6_COMBO1;	// [197] slot
	int PUZZLE_ITEM6_COMBO2;	// [198] slot
	int PUZZLE_ITEM7_COMBO1;	// [199] slot
	int PUZZLE_ITEM7_COMBO2;	// [200] slot
	int PUZZLE_ITEM8_COMBO1;	// [201] slot
	int PUZZLE_ITEM8_COMBO2;	// [202] slot
	int KEY_ITEM1;	// [203] slot
	int KEY_ITEM2;	// [204] slot
	int KEY_ITEM3;	// [205] slot
	int KEY_ITEM4;	// [206] slot
	int KEY_ITEM5;	// [207] slot
	int KEY_ITEM6;	// [208] slot
	int KEY_ITEM7;	// [209] slot
	int KEY_ITEM8;	// [210] slot
	int KEY_ITEM9;	// [211] slot
	int KEY_ITEM10;	// [212] slot
	int KEY_ITEM11;	// [213] slot
	int KEY_ITEM12;	// [214] slot
	int KEY_ITEM1_COMBO1;	// [215] slot
	int KEY_ITEM1_COMBO2;	// [216] slot
	int KEY_ITEM2_COMBO1;	// [217] slot
	int KEY_ITEM2_COMBO2;	// [218] slot
	int KEY_ITEM3_COMBO1;	// [219] slot
	int KEY_ITEM3_COMBO2;	// [220] slot
	int KEY_ITEM4_COMBO1;	// [221] slot
	int KEY_ITEM4_COMBO2;	// [222] slot
	int KEY_ITEM5_COMBO1;	// [223] slot
	int KEY_ITEM5_COMBO2;	// [224] slot
	int KEY_ITEM6_COMBO1;	// [225] slot
	int KEY_ITEM6_COMBO2;	// [226] slot
	int KEY_ITEM7_COMBO1;	// [227] slot
	int KEY_ITEM7_COMBO2;	// [228] slot
	int KEY_ITEM8_COMBO1;	// [229] slot
	int KEY_ITEM8_COMBO2;	// [230] slot
	int PICKUP_ITEM1;	// [231] slot
	int PICKUP_ITEM2;	// [232] slot
	int PICKUP_ITEM3;	// [233] slot
	int PICKUP_ITEM4;	// [234] slot
	int PICKUP_ITEM1_COMBO1;	// [235] slot
	int PICKUP_ITEM1_COMBO2;	// [236] slot
	int PICKUP_ITEM2_COMBO1;	// [237] slot
	int PICKUP_ITEM2_COMBO2;	// [238] slot
	int PICKUP_ITEM3_COMBO1;	// [239] slot
	int PICKUP_ITEM3_COMBO2;	// [240] slot
	int PICKUP_ITEM4_COMBO1;	// [241] slot
	int PICKUP_ITEM4_COMBO2;	// [242] slot
	int EXAMINE1;	// [243] slot
	int EXAMINE2;	// [244] slot
	int EXAMINE3;	// [245] slot
	int CROWBAR_ITEM;	// [246] slot
	int BURNING_TORCH_ITEM;	// [247] slot
	int CLOCKWORK_BEETLE;	// [248] slot
	int CLOCKWORK_BEETLE_COMBO1;	// [249] slot
	int CLOCKWORK_BEETLE_COMBO2;	// [250] slot
	int MINE_DETECTOR;	// [251] slot
	int QUEST_ITEM1;	// [252] slot
	int QUEST_ITEM2;	// [253] slot
	int QUEST_ITEM3;	// [254] slot
	int QUEST_ITEM4;	// [255] slot
	int QUEST_ITEM5;	// [256] slot
	int QUEST_ITEM6;	// [257] slot
	int MAP;	// [258] slot
	int SECRET_MAP;	// [259] slot
	int PUZZLE_HOLE1;	// [260] slot
	int PUZZLE_HOLE2;	// [261] slot
	int PUZZLE_HOLE3;	// [262] slot
	int PUZZLE_HOLE4;	// [263] slot
	int PUZZLE_HOLE5;	// [264] slot
	int PUZZLE_HOLE6;	// [265] slot
	int PUZZLE_HOLE7;	// [266] slot
	int PUZZLE_HOLE8;	// [267] slot
	int PUZZLE_HOLE9;	// [268] slot
	int PUZZLE_HOLE10;	// [269] slot
	int PUZZLE_HOLE11;	// [270] slot
	int PUZZLE_HOLE12;	// [271] slot
	int PUZZLE_DONE1;	// [272] slot
	int PUZZLE_DONE2;	// [273] slot
	int PUZZLE_DONE3;	// [274] slot
	int PUZZLE_DONE4;	// [275] slot
	int PUZZLE_DONE5;	// [276] slot
	int PUZZLE_DONE6;	// [277] slot
	int PUZZLE_DONE7;	// [278] slot
	int PUZZLE_DONE8;	// [279] slot
	int PUZZLE_DONE9;	// [280] slot
	int PUZZLE_DONE10;	// [281] slot
	int PUZZLE_DONE11;	// [282] slot
	int PUZZLE_DONE12;	// [283] slot
	int KEY_HOLE1;	// [284] slot
	int KEY_HOLE2;	// [285] slot
	int KEY_HOLE3;	// [286] slot
	int KEY_HOLE4;	// [287] slot
	int KEY_HOLE5;	// [288] slot
	int KEY_HOLE6;	// [289] slot
	int KEY_HOLE7;	// [290] slot
	int KEY_HOLE8;	// [291] slot
	int KEY_HOLE9;	// [292] slot
	int KEY_HOLE10;	// [293] slot
	int KEY_HOLE11;	// [294] slot
	int KEY_HOLE12;	// [295] slot
	int WATERSKIN1_EMPTY;	// [296] slot
	int WATERSKIN1_1;	// [297] slot
	int WATERSKIN1_2;	// [298] slot
	int WATERSKIN1_3;	// [299] slot
	int WATERSKIN2_EMPTY;	// [300] slot
	int WATERSKIN2_1;	// [301] slot
	int WATERSKIN2_2;	// [302] slot
	int WATERSKIN2_3;	// [303] slot
	int WATERSKIN2_4;	// [304] slot
	int WATERSKIN2_5;	// [305] slot
	int SWITCH_TYPE1;	// [306] slot
	int SWITCH_TYPE2;	// [307] slot
	int SWITCH_TYPE3;	// [308] slot
	int SWITCH_TYPE4;	// [309] slot
	int SWITCH_TYPE5;	// [310] slot
	int SWITCH_TYPE6;	// [311] slot
	int SWITCH_TYPE7;	// [312] slot
	int SWITCH_TYPE8;	// [313] slot
	int UNDERWATER_SWITCH1;	// [314] slot
	int UNDERWATER_SWITCH2;	// [315] slot
	int TURN_SWITCH;	// [316] slot
	int COG_SWITCH;	// [317] slot
	int LEVER_SWITCH;	// [318] slot
	int JUMP_SWITCH;	// [319] slot
	int CROWBAR_SWITCH;	// [320] slot
	int PULLEY;	// [321] slot
	int DOOR_TYPE1;	// [322] slot
	int DOOR_TYPE2;	// [323] slot
	int DOOR_TYPE3;	// [324] slot
	int DOOR_TYPE4;	// [325] slot
	int DOOR_TYPE5;	// [326] slot
	int DOOR_TYPE6;	// [327] slot
	int DOOR_TYPE7;	// [328] slot
	int DOOR_TYPE8;	// [329] slot
	int PUSHPULL_DOOR1;	// [330] slot
	int PUSHPULL_DOOR2;	// [331] slot
	int KICK_DOOR1;	// [332] slot
	int KICK_DOOR2;	// [333] slot
	int UNDERWATER_DOOR;	// [334] slot
	int DOUBLE_DOORS;	// [335] slot
	int BRIDGE_FLAT;	// [336] slot
	int BRIDGE_TILT1;	// [337] slot
	int BRIDGE_TILT2;	// [338] slot
	int SARCOPHAGUS;	// [339] slot
	int SEQUENCE_DOOR1;	// [340] slot
	int SEQUENCE_SWITCH1;	// [341] slot
	int SEQUENCE_SWITCH2;	// [342] slot
	int SEQUENCE_SWITCH3;	// [343] slot
	int SARCOPHAGUS_CUT;	// [344] slot
	int HORUS_STATUE;	// [345] slot
	int GOD_HEAD;	// [346] slot
	int SETH_DOOR;	// [347] slot
	int STATUE_PLINTH;	// [348] slot
	int PISTOLS_ITEM;	// [349] slot
	int PISTOLS_AMMO_ITEM;	// [350] slot
	int UZI_ITEM;	// [351] slot
	int UZI_AMMO_ITEM;	// [352] slot
	int SHOTGUN_ITEM;	// [353] slot
	int SHOTGUN_AMMO1_ITEM;	// [354] slot
	int SHOTGUN_AMMO2_ITEM;	// [355] slot
	int CROSSBOW_ITEM;	// [356] slot
	int CROSSBOW_AMMO1_ITEM;	// [357] slot
	int CROSSBOW_AMMO2_ITEM;	// [358] slot
	int CROSSBOW_AMMO3_ITEM;	// [359] slot
	int CROSSBOW_BOLT;	// [360] slot
	int GRENADE_GUN_ITEM;	// [361] slot
	int GRENADE_GUN_AMMO1_ITEM;	// [362] slot
	int GRENADE_GUN_AMMO2_ITEM;	// [363] slot
	int GRENADE_GUN_AMMO3_ITEM;	// [364] slot
	int GRENADE;	// [365] slot
	int SIXSHOOTER_ITEM;	// [366] slot
	int SIXSHOOTER_AMMO_ITEM;	// [367] slot
	int BIGMEDI_ITEM;	// [368] slot
	int SMALLMEDI_ITEM;	// [369] slot
	int LASERSIGHT_ITEM;	// [370] slot
	int BINOCULARS_ITEM;	// [371] slot
	int FLARE_ITEM;	// [372] slot
	int FLARE_INV_ITEM;	// [373] slot
	int DIARY_ITEM;	// [374] slot
	int COMPASS_ITEM;	// [375] slot
	int MEMCARD_LOAD_INV_ITEM;	// [376] slot
	int MEMCARD_SAVE_INV_ITEM;	// [377] slot
	int PC_LOAD_INV_ITEM;	// [378] slot
	int PC_SAVE_INV_ITEM;	// [379] slot
	int SMOKE_EMITTER_WHITE;	// [380] slot
	int SMOKE_EMITTER_BLACK;	// [381] slot
	int STEAM_EMITTER;	// [382] slot
	int EARTHQUAKE;	// [383] slot
	int BUBBLES;	// [384] slot
	int WATERFALLMIST;	// [385] slot
	int GUNSHELL;	// [386] slot
	int SHOTGUNSHELL;	// [387] slot
	int GUN_FLASH;	// [388] slot
	int BUTTERFLY;	// [389] slot
	int SPRINKLER;	// [390] slot
	int RED_LIGHT;	// [391] slot
	int GREEN_LIGHT;	// [392] slot
	int BLUE_LIGHT;	// [393] slot
	int AMBER_LIGHT;	// [394] slot
	int WHITE_LIGHT;	// [395] slot
	int BLINKING_LIGHT;	// [396] slot
	int LENS_FLARE;	// [397] slot
	int AI_GUARD;	// [398] slot
	int AI_AMBUSH;	// [399] slot
	int AI_PATROL1;	// [400] slot
	int AI_MODIFY;	// [401] slot
	int AI_FOLLOW;	// [402] slot
	int AI_PATROL2;	// [403] slot
	int AI_X1;	// [404] slot
	int AI_X2;	// [405] slot
	int LARA_START_POS;	// [406] slot
	int KILL_ALL_TRIGGERS;	// [407] slot
	int TRIGGER_TRIGGERER;	// [408] slot
	int SMASH_OBJECT1;	// [409] slot
	int SMASH_OBJECT2;	// [410] slot
	int SMASH_OBJECT3;	// [411] slot
	int SMASH_OBJECT4;	// [412] slot
	int SMASH_OBJECT5;	// [413] slot
	int SMASH_OBJECT6;	// [414] slot
	int SMASH_OBJECT7;	// [415] slot
	int SMASH_OBJECT8;	// [416] slot
	int MESHSWAP1;	// [417] slot
	int MESHSWAP2;	// [418] slot
	int MESHSWAP3;	// [419] slot
	int DEATH_SLIDE;	// [420] slot
	int BODY_PART;	// [421] slot
	int CAMERA_TARGET;	// [422] slot
	int WATERFALL1;	// [423] slot
	int WATERFALL2;	// [424] slot
	int WATERFALL3;	// [425] slot
	int PLANET_EFFECT;	// [426] slot
	int ANIMATING1;	// [427] slot
	int ANIMATING1_MIP;	// [428] slot
	int ANIMATING2;	// [429] slot
	int ANIMATING2_MIP;	// [430] slot
	int ANIMATING3;	// [431] slot
	int ANIMATING3_MIP;	// [432] slot
	int ANIMATING4;	// [433] slot
	int ANIMATING4_MIP;	// [434] slot
	int ANIMATING5;	// [435] slot
	int ANIMATING5_MIP;	// [436] slot
	int ANIMATING6;	// [437] slot
	int ANIMATING6_MIP;	// [438] slot
	int ANIMATING7;	// [439] slot
	int ANIMATING7_MIP;	// [440] slot
	int ANIMATING8;	// [441] slot
	int ANIMATING8_MIP;	// [442] slot
	int ANIMATING9;	// [443] slot
	int ANIMATING9_MIP;	// [444] slot
	int ANIMATING10;	// [445] slot
	int ANIMATING10_MIP;	// [446] slot
	int ANIMATING11;	// [447] slot
	int ANIMATING11_MIP;	// [448] slot
	int ANIMATING12;	// [449] slot
	int ANIMATING12_MIP;	// [450] slot
	int ANIMATING13;	// [451] slot
	int ANIMATING13_MIP;	// [452] slot
	int ANIMATING14;	// [453] slot
	int ANIMATING14_MIP;	// [454] slot
	int ANIMATING15;	// [455] slot
	int ANIMATING15_MIP;	// [456] slot
	int ANIMATING16;	// [457] slot
	int ANIMATING16_MIP;	// [458] slot
	int HORIZON;	// [459] slot
	int SKY_GRAPHICS;	// [460] slot
	int BINOCULAR_GRAPHICS;	// [461] slot
	int TARGET_GRAPHICS;	// [462] slot
	int DEFAULT_SPRITES;	// [463] slot
	int MISC_SPRITES;	// [464] slot
	int MOTOR_BOAT;	// [465] slot
	int MOTOR_BOAT_LARA;	// [466] slot
	int RUBBER_BOAT;	// [467] slot
	int RUBBER_BOAT_LARA;	// [468] slot
	int MOTORBIKE_LARA;	// [469] slot
	int FONT_GRAPHICS;	// [470] slot
	int PARALLEL_BARS;	// [471] slot
	int PANEL_BORDER;	// [472] slot
	int PANEL_MIDDLE;	// [473] slot
	int PANEL_CORNER;	// [474] slot
	int PANEL_DIAGONAL;	// [475] slot
	int PANEL_STRIP;	// [476] slot
	int PANEL_HALF_BORDER1;	// [477] slot
	int PANEL_HALF_BORDER2;	// [478] slot
	int PANEL_MIDDLE_CORNER;	// [479] slot
	int TIGHT_ROPE;	// [480] slot
	int LASER_HEAD;	// [481] slot
	int LASER_HEAD_BASE;	// [482] slot
	int LASER_HEAD_TENTACLE;	// [483] slot
	int HYDRA;	// [484] slot
	int HYDRA_MISSILE;	// [485] slot
	int ENEMY_SUB_MARINE;	// [486] slot
	int ENEMY_SUB_MARINE_MIP;	// [487] slot
	int SUB_MARINE_MISSILE;	// [488] slot
	int FROG_MAN;	// [489] slot
	int FROG_MAN_HARPOON;	// [490] slot
	int FISH_EMITTER;	// [491] slot
	int KAYAK;	// [492] slot
	int KAYAK_LARA;	// [493] slot
	int CUSTOM_SPRITES;	// [494] slot
	int BRIDGE_TILT3;	// [495] slot
	int BRIDGE_TILT4;	// [496] slot
	int BRIDGE_CUSTOM;	// [497] slot
	int ROBOT_CLEANER;	// [498] slot
	int ROBOT_STAR_WARS;	// [499] slot
	int MECH_WARRIOR;	// [500] slot
	int MECH_WARRIOR_LARA;	// [501] slot
	int UW_PROPULSOR;	// [502] slot
	int UW_PROPULSOR_LARA;	// [503] slot
	int MINE_CART;	// [504] slot
	int MINE_CART_LARA;	// [505] slot
	int NEW_SLOT5;	// [506] slot
	int NEW_SLOT6;	// [507] slot
	int NEW_SLOT7;	// [508] slot
	int NEW_SLOT8;	// [509] slot
	int NEW_SLOT9;	// [510] slot
	int NEW_SLOT10;	// [511] slot
	int NEW_SLOT11;	// [512] slot
	int NEW_SLOT12;	// [513] slot
	int NEW_SLOT13;	// [514] slot
	int NEW_SLOT14;	// [515] slot
	int NEW_SLOT15;	// [516] slot
	int NEW_SLOT16;	// [517] slot
	int NEW_SLOT17;	// [518] slot
	int NEW_SLOT18;	// [519] slot
	int NUMBER_OBJECTS;	// [520] slot
} StuffEnumSLOT;

// ----------------  ENUM SHORTCUT FOR GET_ -----------------
typedef struct StrEnumGET {
	int LARA;	// returns values in GET.pLara->  and GET.LaraIndex    INPUT: none
	int ITEM;	// returns value in GET.pItem->   (moveable object).INPUT: Index = index of moveable + (NGLE_INDEX if it is a ngle index)
	int STATIC;	// returns value in GET.pStatic->  (static object) INPUT: Index = (index of static + NGLE_INDEX) or (Index=Room), SecondaryIndex = static index if it was missing NGLE_INDEX, or unused
	int ROOM;	// returns value in GET.pRoom->   (room structure) INPUT: Index = index of room (no NGLE_INDEX, use the second number you see in room list of ngle program)
	int ITEM_COLL_BOX;	// returns value in GET.pCollItem->   (collision box of moveable item)  INPUT: same of GET_ITEM
	int STATIC_COLL_BOX;	// returns value in GET.pCollStatic->  (collision box of static item) INPUT: same input of GET_STATIC
	int STATIC_VIEW_BOX;	// returns value in GET.pViewStatic->  (view box of static item) INPUT: same input of GET_STATIC
	int DOOR_OF_ROOM;	// returns value in GET.pDoor->  (structure of "room" door) INPUT: Index = index of room, SecondaryIndex = index of door
	int INFO_LARA;	// returns values in GET.LaraInfo.  (structure with various infos about lara) INPUT: none
	int MY_PARAMETER_COMMAND;	// returns value in GET.pParam->   INPUT: Index= PARAM_ value, SecondaryIndex = (optiona) id (first field after PARAM_ value) of Parameters command or -1 if you wish omit this input value
	int MY_CUSTOMIZE_COMMAND;	// returns value in GET.pCust->  INPUT: Index = CUST_ value, SecondayIndex = (optional) value of first field after CUST_ value or -1 if you omit this input value
	int INPUT;	// returns value in GET.Input.  INPUT: none
	int STRINGNG;	// return value in GET.pStringNG  INPUT: Index= Index of ng string
	int STRING;	// return value in GET.pString INPUT: Index= Index of pc/psx string
	int BIG_NUMBER;	// return value in GET.BigNumber  INPUT: Index= index of number in Parameters=PARAM_BIG_NUMBERS script command. First number has index=0
	int SCRIPT_SETTINGS;	// return values in GET.ScriptSettings. INPUT: none
	int PROGRESSIVE_ACTION;	// return value in GET.pAction->  INPUT: none (creates a new progressive action record)
	int AI_ITEM;	// return value in GET.pAI->  INPUT: index = index of AI record in AI array (got from Find() function)
	int LIGHT;	// return value in GET.pLight->  INPUT: index = index of Room  SecondaryIndex=Index of light record of given room (got from Find() function)
	int CAMERA;	// return value in GET.pCamera-> INPUT: index = index of fixed camera got from Find() function
	int CAMERA_FLY;	// return value in GET.pCameraFly->  INPUT: index = index of flyby camera got from Find() function
	int COLOR_RGB;	// return value in GET.Color  INPUT: Index = Id of ColorRgb script command, SecondaryIndex = ColorFormat (enumCOLF_..)
	int GAME_INFO;	// return values in GET.GameInfo.  INPUT: none
	int SLOT;	// return value in GET.pSlot->  INPUT: index = SLOT_ value
	int INVENTORY;	// return values in GET.InventoryData.  and (old access) in GET.pInventory->  INPUT: none
	int ANIMATION;	// return values in GET.pAnimation->  INPUT: Index of absolute animation to locate
	int INFO_ITEM;	// return values in GET.InfoItem.  INPUT: Index of moveable item to analyse
	int VARIABLES;	// return values in GET.Vars struture. Used to access to trng variables and to have infos about selected items of memory zones
	int FLIPMAPS;	// return values in GET.pFlipMap struture. Used to access to all flipmap infos. No required input values
} StuffEnumGET;

// ----------------  ENUM SHORTCUT FOR SKIP_ -----------------
typedef struct StrEnumSKIP {
	int NONE;
	int FIXED_CAMERA;
	int FLY_CAMERA;
	int LOADING_LEVEL;
	int FADE;
	int TITLE_LEVEL;
	int GRAY_SCREEN;	// pause, inventory load/save game from the game
	int NO_VIEW_OGGETTI;
	int BINOCULARS;
	int LASER_SIGHT;
	int FULL_IMAGE;	// creata da trng
} StuffEnumSKIP;

// ----------------  ENUM SHORTCUT FOR FL2_ -----------------
typedef struct StrEnumFL2 {
	int OVER_MINE_TRIGGER;
	int IS_BURNING;
	int IS_DUCK;
	int IS_INVULNERABLE;
	int IS_USING_LASER_SIGHT_OR_BINOCULARS;
	int TORCH_IS_BURNING;
	int IS_CLIMBING;
} StuffEnumFL2;

// ----------------  ENUM SHORTCUT FOR HOLD_ -----------------
typedef struct StrEnumHOLD {
	int PISTOLS;
	int REVOLVER;
	int UZI;
	int SHOTGUN;
	int GRENADEGUN;
	int CROSSBOW;
	int FLARE;
	int OUT_TORCH;
	int FIRED_TORCH;
	int JEEP;
	int SIDECAR;
	int RUBBER_BOAT;
	int MOTOR_BOAT;
	int ROPE;
	int POLE;
	int ANY_TORCH;
	int KAYAK;
} StuffEnumHOLD;

// ----------------  ENUM SHORTCUT FOR LLF_ -----------------
typedef struct StrEnumLLF {
	int GROUND;	// lara outside water, no special situation
	int UNDERWATER;	// lara is underwater
	int FLOATING;	// lara is floating on the water surface
	int DOZY;	// lara has some special status: dozy e perhaps others
	int LOW_WATER;	// lara is walking on low water (no floating)
} StuffEnumLLF;

// ----------------  ENUM SHORTCUT FOR SCANF_ -----------------
typedef struct StrEnumSCANF {
	int HEAVY;	// Only for SCANF_FLOOR_DATA
	int TEMP_ONE_SHOT;	// internal setting to stop trigger until lara is yet standing on same sector
	int BUTTON_ONE_SHOT;	// one-shot button of the trigger. Only with SCANF_FLOOR_DATA
	int YET_TO_PERFORM;	// internal setting to remember this trigger has yet to be performed
	int SCRIPT_TRIGGER;	// trigger read from triggergroup of script.dat
	int DIRECT_CALL;	// trigger has been called from code in direct way
	int FLOOR_DATA;	// trigger has been read directly from floor data of tr4 file
	int ANIM_COMMAND;	// trigger has been called from an animcommand of some animation
} StuffEnumSCANF;

// ----------------  ENUM SHORTCUT FOR MESH_ -----------------
typedef struct StrEnumMESH {
	int BUTT;	// [0]
	int RIGHT_THIGH;	// [1]
	int RIGHT_CALF;	// [2]
	int RIGHT_FOOT;	// [3]
	int LEFT_THIGH;	// [4]
	int LEFT_CALF;	// [5]
	int LEFT_FOOT;	// [6]
	int CHEST;	// [7]
	int RIGHT_ARM;	// [8]
	int RIGHT_FOREARM;	// [9]
	int RIGHT_HAND;	// [10]
	int LEFT_ARM;	// [11]
	int LEFT_FOREARM;	// [12]
	int LEFT_HAND;	// [13]
	int HEAD;	// [14]
} StuffEnumMESH;

// ----------------  ENUM SHORTCUT FOR TRET_ -----------------
typedef struct StrEnumTRET {
	int PERFORM_ALWAYS;	// let enabled, the trigger, it will be continuosly performed until lara is over it
	int PERFORM_ONCE_AND_GO;	// disable temporary further executions until lara is yet on this sector
	int PERFORM_NEVER_MORE;	// disable forever this trigger on this sector (single-shot)
	int EXECUTE_ORIGINAL;	// used only with CBT_REPLACE to change idea and let execution of original code, in this way the CBT_REPLACE becomes dinamically a CBT_FIRST callback
} StuffEnumTRET;

// ----------------  ENUM SHORTCUT FOR CTRET_ -----------------
typedef struct StrEnumCTRET {
	int IS_TRUE;	// condition is true. se this value is missing then it will be false
	int EXTRA_PARAM;	// this condition trigger had an Extra parameter
	int ON_MOVEABLE;	// this condition trigger worked on a moveable
	int PERFORM_ALWAYS;	// peforming: Perform continuosly this condition
	int ONLY_ONCE_ON_TRUE;	// (suggested) Next peforming: No more performing until lara is over this sector and the condition was true
	int NEVER_MORE_ON_TRUE;	// Next peforming: No more performing forever if the condition was true
	int PERFORM_ONCE_AND_GO;	// No more performing until lara is over this sector, no matter if the conditon was true or false
	int EXECUTE_ORIGINAL;	// Only for CBT_REPLACE and CBT_AFTER callbacks.  it lets to original trng code the target to return a response.
} StuffEnumCTRET;

// ----------------  ENUM SHORTCUT FOR SRET_ -----------------
typedef struct StrEnumSRET {
	int OK;	// all fine: go on with common callback mangement
	int SKIP_ORIGINAL;	// valid only on CBT_FIRST call backs: the callback requires to skip the execution of tomb4 original code (becoming from CBT_FIRST to CBT_REPLACE)
	int SKIP_TRNG_CODE;	// valid only with CBT_FIRST call back on codes already managed by trng code (example: control object)
} StuffEnumSRET;

// ----------------  ENUM SHORTCUT FOR JOINT_ -----------------
typedef struct StrEnumJOINT {
	int LEFT_THIGH;
	int LEFT_KNEE;
	int LEFT_ANCKLE;
	int RIGHT_THIGH;
	int RIGHT_KNEE;
	int RIGHT_ANCKLE;
	int ABDOMEN;
	int NECK;
	int LEFT_SHOULDER;
	int LEFT_ELBOW;
	int LEFT_WRIST;
	int RIGHT_SHOULDER;
	int RIGHT_ELBOW;
	int RIGHT_WRIST;
} StuffEnumJOINT;

// ----------------  ENUM SHORTCUT FOR SC_ -----------------
typedef struct StrEnumSC {
	int NORMAL;
	int HALF_WIDTH;
	int HALF_HEIGHT;
	int HALF_SIZE;
	int DOUBLE_WIDTH;
	int DOUBLE_HEIGHT;
	int DOUBLE_SIZE;
} StuffEnumSC;

// ----------------  ENUM SHORTCUT FOR STRING_ -----------------
typedef struct StrEnumSTRING {
	int NG;
	int BOTTOM_CENTER;
	int TOP_CENTER;
	int CENTER_CENTER;
	int TOP_LEFT;
	int TOP_RIGHT;
	int BOTTOM_LEFT;
	int BOTTOM_RIGHT;
	int DOWN_DAMAGE_BAR;
	int DOWN_COLD_BAR;
	int DOWN_LEFT_BARS;
	int DOWN_RIGHT_BARS;
} StuffEnumSTRING;

// ----------------  ENUM SHORTCUT FOR ADD_ -----------------
typedef struct StrEnumADD {
	int FLAME;
	int SMOKE;
	int BLOOD;
	int MIST;
	int LIGHT_FLAT;
	int LIGHT_BLINK;
	int LIGHT_SPOT;
	int LIGHT_GLOVE;
} StuffEnumADD;

// ----------------  ENUM SHORTCUT FOR ENV_ -----------------
typedef struct StrEnumENV {
	int NO_BLOCK_IN_FRONT;
	int HANG_WITH_FEET;
	int HOLE_FLOOR_IN_FRONT;
	int MONKEY_CEILING;
	int CLIMB_WALL_IN_FRONT;
	int CLIMB_WALL_AT_RIGHT;
	int CLIMB_WALL_AT_LEFT;
	int HOLE_IN_FRONT_CEILING_CLIMB;
	int HOLE_BACK_CEILING_CLIMB;
	int NO_BLOCK_AT_RIGHT;
	int NO_BLOCK_AT_LEFT;
	int HOLE_FLOOR_AT_RIGHT;
	int HOLE_FLOOR_AT_LEFT;
	int HOLE_FLOOR_BACK;
	int NO_BLOCK_BACK;
	int CLIMB_WALL_BACK;
	int SUPPORT_IN_FRONT_WALL;
	int SUPPORT_IN_RIGHT_WALL;
	int SUPPORT_IN_LEFT_WALL;
	int SUPPORT_IN_BACK_WALL;
	int ITEM_EXTRA_IN_FRONT;
	int ITEM_EXTRA_OVER;
	int ITEM_EXTRA_UNDER;
	int MULT_CONDITION;
	int HANG_LEFT_IN_CORNER;
	int HANG_LEFT_OUT_CORNER;
	int HANG_RIGHT_IN_CORNER;
	int HANG_RIGHT_OUT_CORNER;
	int HANG_LEFT_SPACE;
	int HANG_RIGHT_SPACE;
	int DISTANCE_CEILING;
	int CLIMB_LEFT_IN_CORNER;
	int CLIMB_LEFT_OUT_CORNER;
	int CLIMB_RIGHT_IN_CORNER;
	int CLIMB_RIGHT_OUT_CORNER;
	int CLIMB_LEFT_SPACE;
	int CLIMB_RIGHT_SPACE;
	int MULT_OR_CONDITION;
	int DISTANCE_FLOOR;
	int FRAME_NUMBER;
	int VERTICAL_ORIENT;
	int ON_VEHICLE;
	int FREE_HANDS;
	int UNDERWATER;
	int FLOATING;
	int ONLAND;
	int IS_STILL;
	int ANIM_COMPLETE;
	int FLYING_UP;
	int FLYING_DOWN;
	int WALL_HOLE_IN_FRONT;
	int IN_LEFT_SIDE_SECTOR;
	int IN_RIGHT_SIDE_SECTOR;
	int ITEM_EXTRA_AT_LEFT;
	int ITEM_EXTRA_AT_RIGHT;
	int ITEM_TEST_POSITION;
	int HOLD_EXTRA_ITEM_IN_HANDS;
	int CONDITION_TRIGGER_GROUP;
	int ROOM_IS;
	int PLAYER_IS_SLEEPING;
	int PLAYER_WOKE_UP;
	int DISTANCE_NORTH_WALL;
	int DISTANCE_SOUTH_WALL;
	int DISTANCE_EAST_WALL;
	int DISTANCE_WEST_WALL;
	int LARA_IN_MICRO_STRIP;
	int NO_BOX_IN_FRONT;
	int NO_BOX_AT_LEFT;
	int NO_BOX_AT_RIGHT;
	int NO_BOX_BACK;
	int ENEMY_SEE_LARA;
	int FRAME_RANGE;
	int NON_TRUE;
	int POS_LEFT_CORNER;
	int POS_RIGHT_CORNER;
	int POS_CENTRAL;
	int POS_HORTOGONAL;
	int POS_IN_THE_MIDDLE;
	int POS_STRIP_3;
	int POS_STRIP_2;
	int POS_STRIP_1;
	int MASK_CONDITION;
	int MASK_FLAGS;
} StuffEnumENV;

// ----------------  ENUM SHORTCUT FOR CBT_ -----------------
typedef struct StrEnumCBT {
	int FIRST;	// your callback code it will be performed FIRST of orginal trng code, then it will be performed original trng code
	int AFTER;	// your callback code it will be performed AFTER that the original trng code has been performed
	int REPLACE;	// this callback will REPLACE totally original trng code.
	int ASM;	// this callback will be handled by your assembly code at low level.
	int PRESENT;	// do not use, reserved
	int LOOPED;	// performed togheter with original code, it works with management that has not a precise start and end but when it is in progress
	int ANY;	// do not use, reserved
} StuffEnumCBT;

// ----------------  ENUM SHORTCUT FOR FAN_ -----------------
typedef struct StrEnumFAN {
	int MASK_FPS_UV;
	int MASK_UVROTATE;
	int MASK_DELAY;
	int MASK_ANIM;
	int FRAMES;
	int P_FRAMES;
	int FULL_ROTATE;
	int RIVER_ROTATE;	// 0x8000 + 0x2000
	int HALF_ROTATE;
	int ANY_ROTATE;
	int SET_FREE_HANDS;
	int START_FROM_EXTRA_FRAME;
	int SET_NEUTRAL_STATE_ID;
	int KEYS_AS_SCANCODE;
	int DISABLE_PUSH_AWAY;
	int KEEP_NEXT_STATEID;
	int ENABLE_GRAVITY;
	int DISABLE_GRAVITY;
	int PERFORM_TRIGGER_GROUP;
	int SET_FREE_HANDS_TEMP;
	int SET_BUSY_HANDS;
	int ALIGN_TO_ENV_POS;
	int SET_ADDEFFECT;
	int RANDOM;
	int SET_LARA_PLACE;
} StuffEnumFAN;

// ----------------  ENUM SHORTCUT FOR FLH_ -----------------
typedef struct StrEnumFLH {
	int ALMOST_FREE_HANDS;	// no weapon or torch but she could hold the flare
	int IS_GRABBING;	// lara is grabbing something (torch, wall, ceiling but not weapons or flare)
	int IS_EXTRACTING;	// lara is extracting (or picking up) some weapon or torch (but not flare)
	int IS_THROWING;	// lara is throwing away current weapon or torch (not affected by flare)
	int HOLDS_ITEM;	// lara is holding some weapon or torch (but this is not affected by flare)
} StuffEnumFLH;

// ----------------  ENUM SHORTCUT FOR CMD_ -----------------
typedef struct StrEnumCMD {
	int ALL;	// 0 All keyboard commands
	int UP;	//  1: Up
	int DOWN;	// 2: Down
	int LEFT;	// 3: Left
	int RIGHT;	// 4: Right
	int DUCK;	// 5: Duck
	int DASH;	// 6: Dash
	int WALK;	// 7: Walk
	int JUMP;	// 8: Jump
	int ACTION;	// 9: Action
	int DRAW_WEAPON;	// 10: Draw Weapon
	int USE_FLARE;	// 11: Use Flare
	int LOOK;	// 12: Look
	int ROLL;	//  13: Roll
	int INVENTORY;	//  14: Inventory
	int STEP_LEFT;	// 15: Step Left
	int STEP_RIGHT;	// 16: Step Right
	int PAUSE;	//  17: Pause
	int SAVE_GAME;	// 18: Save the game (special)
	int LOAD_GAME;	// 19: Load the game (special)
	int WEAPON_KEYS;	//  20: Select weapon keys
	int ENTER;	// 21: Enter
} StuffEnumCMD;

// ----------------  ENUM SHORTCUT FOR SRV_ -----------------
typedef struct StrEnumSRV {
	int PERFORM_FLIPEFFECT;	// you can use the shortcut function: PerformFlipEffect() to use this service
	int PERFORM_ACTION;	// you can use the shortcut PerformActionTrigger() to use this service
	int PERFORM_CONDITION;	// you can use the shortcut PerformConditionTrigger() to use this service
	int PERFORM_TRIGGERGROUP_ID;	// Arg1=IdOfTriggerGroup
	int PERFORM_EXPORTED_TRIGGER;	// you can use shortcut function: PerformExportedTrigger() to use this service
	int CREATE_TRIGGERGROUP;	// TestDynamic and then a sequence of triple arg, {FirstWord, SecondWord, ThirdWord}
	int CREATE_ANIMATION;	// same fields of Animation command with END_LIST value at end
	int CREATE_ANIMATIONSLOT;	// same field of AnimationSlot command with END_LIST at end
	int CREATE_MULTENVCONDITION;	// TestDynamic and then same fields of MultEnvCondition= command
	int CREATE_TESTPOSITION;	// TestDynamic and then same fields of TestPosition command
	int CREATE_ADDEFFECT;	// TestDynamic and then same fields of AddEffect command
	int SetReservedDataZone;	// You pass the startOffset and number of bytes
	int F_ProporzioneDistanza;	//       (int Incremento, int Distanza);
	int F_EseguiTriggerGroup;
	int F_DetectedGlobalTriggerEvent;
	int F_InviaErroreLog;
	int CREATE_PARAM_COMMAND;	// creare Parameters=PARAM_.. command of trng owner to have an extra input
	int F_TestEnvCondition;
	int F_IsBoxSettore;
	int F_VerificaTestPosition;
	int F_CollideItemConCustom;
	int F_IsCollisioneConItems;
	int F_InviaLog;
	int F_EseguiAnimNemico;
	int F_CreateAIRecord;
	int F_DeleteAIRecord;
	int F_CreateNewMoveable;
	int F_DeleteNewMoveable;
	int CREATE_COLOR_RGB_COMMAND;	// arguments: TestDynamic and then Red, Green, Blue, it will return Id for new colorrgb
	int DeleteParamCommand;
	int DeleteTriggerGroup;
	int DeleteColorRgb;
	int DeleteMultEnvCondition;
	int DeleteTestPosition;
	int DeleteAddEffect;
	int F_DisableSaving;
	int F_AggiungiItemMosso;
	int F_CheckForStartMovePushable;
	int F_CheckForEndMovePushable;
	int F_ExplosionOnVehicle;
	int F_ConvertMicroUnits;
	int F_AllocateImage;
	int F_FreeImage;
	int F_AllocateTombHdc;
	int F_FreeTombHdc;
	int F_DrawSprite2D;
	int F_DrawMesh3D;
	int F_DrawObject2D;
	int F_PrintText;
	int F_ReadDxInput;
	int F_SuspendAudioTrack;
	int F_ResumeAudioTrack;
	int F_CreateWindowsFont;
	int F_FreeWindowsFont;
	int F_ReadInputBox;
	int F_ReadNumVariable;
	int F_WriteNumVariable;
	int F_ReadTextVariable;
	int F_WriteTextVariable;
	int F_ReadMemVariable;
	int F_WriteMemVariable;
} StuffEnumSRV;

// ----------------  ENUM SHORTCUT FOR SLEV_ -----------------
typedef struct StrEnumSLEV {
	int YOUNG_LARA;
	int WEATHER;
	int HORIZON;
	int LAYER1;
	int LAYER2;
	int STAR_FIELD;
	int LIGHTNING;
	int TRAIN;
	int PULSE;
	int COL_ADD_HORIZON;
	int RESET_HUB;
	int LENS_FLARE;
	int TIMER;
	int MIRROR;
	int REMOVE_AMULET;
	int NO_LEVEL;
} StuffEnumSLEV;

// ----------------  ENUM SHORTCUT FOR SMAIN_ -----------------
typedef struct StrEnumSMAIN {
	int FLY_CHEAT;
	int LOAD_SAVE;
	int TITLE;
	int PLAY_ANY_LEVEL;
	int DEMO_DISK;
} StuffEnumSMAIN;

// ----------------  ENUM SHORTCUT FOR EDGX_ -----------------
typedef struct StrEnumEDGX {
	int CONCISE_SCRIPT_LOG;
	int SLOW_MOTION;
	int SWAP_VIEW;
	int LARA_CORD_IN_LOG;
	int TRIGGER_TIMING;
	int RECORDING_DEMO;
	int CUTSCENE_LOG;
	int ANIMATION_SLOT;
} StuffEnumEDGX;

// ----------------  ENUM SHORTCUT FOR DGX_ -----------------
typedef struct StrEnumDGX {
	int LARA;
	int ANIMATION;
	int SFX_SOUNDS;
	int FAR_VIEW;
	int AUDIO_TRACKS;
	int CHEATS;
	int FOG;
	int COMMON_VARIABLES;
	int STORE_VARIABLES;
	int FLYBY;
	int TEXT_VARIABLES;
	int LOG_SCRIPT_COMMANDS;
	int WEAPON_ANIMATION;
	int ADJUSTMENT_MODE;
	int FPS;
	int ERRORS;
} StuffEnumDGX;

// ----------------  ENUM SHORTCUT FOR FROOM_ -----------------
typedef struct StrEnumFROOM {
	int COLD;
	int RAIN;
	int SNOW;
	int REFLECTION;
	int MIST;
	int NON_LENS_FLARE;
	int MISTERY;
	int OUTSIDE;
	int DAMAGE;
	int HORIZON;
	int QUICKSAND;
	int IS_FLIPROOM;
	int WATER;
} StuffEnumFROOM;

// ----------------  ENUM SHORTCUT FOR FIND_ -----------------
typedef struct StrEnumFIND {
	int ITEM;	// return values in FIND.VetItems and FIND.TotItems Looks for moveable items
	int STATIC;	// return values in FIND.VetStatics and FIND.TotStatics Looks for static items
	int AI;	// return values in FIND.VetAI and FIND.TotAI  Looks for AI_ items and LARA_START_POS items
	int LIGHT;	// return values in FIND.VetLigths and FIND.TotLights  look for lights, fog bulbs and light effects
	int CAMERA_FIXED;	// return values in FIND.VetCameraFix and FIND.TotCameraFix
	int CAMERA_FLY;	// return values in FIND.VetCameraFly  and FIND.TotCameraFly
	int ITEMS_NEARBY;	// returned values in FIND.VetItems and FIND.TotItems, looks for moveables closed to (x,y,z) point given as pPointer
	int ITEMS_SECTOR;	// returned values in FIND.VetItems and FIND.TotItems, looks for all moveable with pivot in sector where falls the point given as pPointer
	int STATICS_NEARBY;	// returned values in FIND.VetStatics and FIND.TotStatics, looks for statics closed to (x,y,z) point given as pPointer
	int STATICS_SECTOR;	// returned values in FIND.VetStatics and FIND.TotStatics, looks for all statics with pivot in sector where falls the point given as pPointer
	int ENEMY_COMMAND;	// returned value in FIND.pEnemy. Look for Enemy script command with Slot value
	int IMAGE_COMMAND;	// reurned value in FIND.pImage. Look for Image script command with given Id; Extra = IdImage
	int WINDOWS_FONT;	// returned value in FIND.pWindowsFont. Look for WindowsFont script command with IdWindowsFont; Extra = IdWindowsFont to look for
	int DIAGNOSTIC;	// returned value in FIND.Dgx. Look for Diagnostic, DiagnosticType and LogItem commands; no input required
	int PROGR_ACTION_TRNG;	// returned value in FIND.pVetProgAxnTrng[] FIND.TotProgAxnTrng input: SlotType= AZ_ action type, Extra = Index of "ItemIndex" field or -1 if ignore
	int PROGR_ACTION_MINE;	// returned values in FIND.pVetProgrAxnMine[] FIND.TotProgrAxnMine  input: SlotType = AXN_ action type, Extra = index of "ItemIndex" field or -1 if ignore
	int ASSIGN_SLOT_MINE;	// returned values in FIND.SlotAssigned  Input: SlotType= OBJ_Type to look for
} StuffEnumFIND;

// ----------------  ENUM SHORTCUT FOR SSLOT_ -----------------
typedef struct StrEnumSSLOT {
	int PLANT0;	// [0] slot
	int PLANT1;	// [1] slot
	int PLANT2;	// [2] slot
	int PLANT3;	// [3] slot
	int PLANT4;	// [4] slot
	int PLANT5;	// [5] slot
	int PLANT6;	// [6] slot
	int PLANT7;	// [7] slot
	int PLANT8;	// [8] slot
	int PLANT9;	// [9] slot
	int FURNITURE0;	// [10] slot
	int FURNITURE1;	// [11] slot
	int FURNITURE2;	// [12] slot
	int FURNITURE3;	// [13] slot
	int FURNITURE4;	// [14] slot
	int FURNITURE5;	// [15] slot
	int FURNITURE6;	// [16] slot
	int FURNITURE7;	// [17] slot
	int FURNITURE8;	// [18] slot
	int FURNITURE9;	// [19] slot
	int ROCK0;	// [20] slot
	int ROCK1;	// [21] slot
	int ROCK2;	// [22] slot
	int ROCK3;	// [23] slot
	int ROCK4;	// [24] slot
	int ROCK5;	// [25] slot
	int ROCK6;	// [26] slot
	int ROCK7;	// [27] slot
	int ROCK8;	// [28] slot
	int ROCK9;	// [29] slot
	int ARCHITECTURE0;	// [30] slot
	int ARCHITECTURE1;	// [31] slot
	int ARCHITECTURE2;	// [32] slot
	int ARCHITECTURE3;	// [33] slot
	int ARCHITECTURE4;	// [34] slot
	int ARCHITECTURE5;	// [35] slot
	int ARCHITECTURE6;	// [36] slot
	int ARCHITECTURE7;	// [37] slot
	int ARCHITECTURE8;	// [38] slot
	int ARCHITECTURE9;	// [39] slot
	int DEBRIS0;	// [40] slot
	int DEBRIS1;	// [41] slot
	int DEBRIS2;	// [42] slot
	int DEBRIS3;	// [43] slot
	int DEBRIS4;	// [44] slot
	int DEBRIS5;	// [45] slot
	int DEBRIS6;	// [46] slot
	int DEBRIS7;	// [47] slot
	int DEBRIS8;	// [48] slot
	int DEBRIS9;	// [49] slot
	int SHATTER0;	// [50] slot
	int SHATTER1;	// [51] slot
	int SHATTER2;	// [52] slot
	int SHATTER3;	// [53] slot
	int SHATTER4;	// [54] slot
	int SHATTER5;	// [55] slot
	int SHATTER6;	// [56] slot
	int SHATTER7;	// [57] slot
	int SHATTER8;	// [58] slot
	int SHATTER9;	// [59] slot
	int EXTRA00;	// [60] slot
	int EXTRA01;	// [61] slot
	int EXTRA02;	// [62] slot
	int EXTRA03;	// [63] slot
	int EXTRA04;	// [64] slot
	int EXTRA05;	// [65] slot
	int EXTRA06;	// [66] slot
	int EXTRA07;	// [67] slot
	int EXTRA08;	// [68] slot
	int EXTRA09;	// [69] slot
	int EXTRA10;	// [70] slot
	int EXTRA11;	// [71] slot
	int EXTRA12;	// [72] slot
	int EXTRA13;	// [73] slot
	int EXTRA14;	// [74] slot
	int EXTRA15;	// [75] slot
	int EXTRA16;	// [76] slot
	int EXTRA17;	// [77] slot
	int EXTRA18;	// [78] slot
	int EXTRA19;	// [79] slot
	int EXTRA20;	// [80] slot
	int EXTRA21;	// [81] slot
	int EXTRA22;	// [82] slot
	int EXTRA23;	// [83] slot
	int EXTRA24;	// [84] slot
	int EXTRA25;	// [85] slot
	int EXTRA26;	// [86] slot
	int EXTRA27;	// [87] slot
	int EXTRA28;	// [88] slot
	int EXTRA29;	// [89] slot
	int EXTRA30;	// [90] slot
	int EXTRA31;	// [91] slot
	int EXTRA32;	// [92] slot
	int EXTRA33;	// [93] slot
	int EXTRA34;	// [94] slot
	int EXTRA35;	// [95] slot
	int EXTRA36;	// [96] slot
	int EXTRA37;	// [97] slot
	int EXTRA38;	// [98] slot
	int EXTRA39;	// [99] slot
	int EXTRA40;	// [100] slot
	int EXTRA41;	// [101] slot
	int EXTRA42;	// [102] slot
	int EXTRA43;	// [103] slot
	int EXTRA44;	// [104] slot
	int EXTRA45;	// [105] slot
	int EXTRA46;	// [106] slot
	int EXTRA47;	// [107] slot
	int EXTRA48;	// [108] slot
	int EXTRA49;	// [109] slot
	int EXTRA50;	// [110] slot
	int EXTRA51;	// [111] slot
	int EXTRA52;	// [112] slot
	int EXTRA53;	// [113] slot
	int EXTRA54;	// [114] slot
	int EXTRA55;	// [115] slot
	int EXTRA56;	// [116] slot
	int EXTRA57;	// [117] slot
	int EXTRA58;	// [118] slot
	int EXTRA59;	// [119] slot
	int EXTRA60;	// [120] slot
	int EXTRA61;	// [121] slot
	int EXTRA62;	// [122] slot
	int EXTRA63;	// [123] slot
	int EXTRA64;	// [124] slot
	int EXTRA65;	// [125] slot
	int EXTRA66;	// [126] slot
	int EXTRA67;	// [127] slot
	int EXTRA68;	// [128] slot
	int EXTRA69;	// [129] slot
	int EXTRA70;	// [130] slot
	int EXTRA71;	// [131] slot
	int EXTRA72;	// [132] slot
	int EXTRA73;	// [133] slot
	int EXTRA74;	// [134] slot
	int EXTRA75;	// [135] slot
	int EXTRA76;	// [136] slot
	int EXTRA77;	// [137] slot
	int EXTRA78;	// [138] slot
	int EXTRA79;	// [139] slot
	int EXTRA80;	// [140] slot
	int EXTRA81;	// [141] slot
	int EXTRA82;	// [142] slot
	int EXTRA83;	// [143] slot
	int EXTRA84;	// [144] slot
	int EXTRA85;	// [145] slot
	int EXTRA86;	// [146] slot
	int EXTRA87;	// [147] slot
	int EXTRA88;	// [148] slot
	int EXTRA89;	// [149] slot
	int EXTRA90;	// [150] slot
	int EXTRA91;	// [151] slot
	int EXTRA92;	// [152] slot
	int EXTRA93;	// [153] slot
	int EXTRA94;	// [154] slot
	int EXTRA95;	// [155] slot
	int EXTRA96;	// [156] slot
	int EXTRA97;	// [157] slot
	int EXTRA98;	// [158] slot
	int EXTRA99;	// [159] slot
	int NUMBER_STATIC_OBJECTS;	// [160] slot
} StuffEnumSSLOT;

// ----------------  ENUM SHORTCUT FOR FWEAP_ -----------------
typedef struct StrEnumFWEAP {
	int PRESENT;	// weapon is present
	int LASERSIGHT;	// weapon has mounted the laser sight
	int AMMO_NORMAL;	// available normal ammos
	int AMMO_SUPER;	// for shotgun or grenadegun
	int AMMO_EXPLOSIVE;	// for crossbow, explosive while for grenadegun, flash ammo
} StuffEnumFWEAP;

// ----------------  ENUM SHORTCUT FOR CBEET_ -----------------
typedef struct StrEnumCBEET {
	int BEETLE_WITH_KEY;	// the beetle is present and has the key already mounted
	int KEY_SINGLE;	// is present the key but divided by beetle
	int BEETLE_SINGLE;	// is present the beetle but has not mounted the key
} StuffEnumCBEET;

// ----------------  ENUM SHORTCUT FOR WSKIN_ -----------------
typedef struct StrEnumWSKIN {
	int MISSING;
	int EMPTY_WATER_SKIN;	// it's present but it is empty
	int ONE_LITER;	// present with one liter
	int THREE_LITERS;	// present with three liters
	int FIVE_LITERS;	// present with five liters
} StuffEnumWSKIN;

// ----------------  ENUM SHORTCUT FOR COMBO_ -----------------
typedef struct StrEnumCOMBO {
	int ITEM1_FOR_PUZZLE_1;
	int ITEM2_FOR_PUZZLE_1;
	int ITEM1_FOR_PUZZLE_2;
	int ITEM2_FOR_PUZZLE_2;
	int ITEM1_FOR_PUZZLE_3;
	int ITEM2_FOR_PUZZLE_3;
	int ITEM1_FOR_PUZZLE_4;
	int ITEM2_FOR_PUZZLE_4;
	int ITEM1_FOR_PUZZLE_5;
	int ITEM2_FOR_PUZZLE_5;
	int ITEM1_FOR_PUZZLE_6;
	int ITEM2_FOR_PUZZLE_6;
	int ITEM1_FOR_PUZZLE_7;
	int ITEM2_FOR_PUZZLE_7;
	int ITEM1_FOR_PUZZLE_8;
	int ITEM2_FOR_PUZZLE_8;
} StuffEnumCOMBO;

// ----------------  ENUM SHORTCUT FOR KCOMBO_ -----------------
typedef struct StrEnumKCOMBO {
	int ITEM1_FOR_KEY_1;
	int ITEM2_FOR_KEY_1;
	int ITEM1_FOR_KEY_2;
	int ITEM2_FOR_KEY_2;
	int ITEM1_FOR_KEY_3;
	int ITEM2_FOR_KEY_3;
	int ITEM1_FOR_KEY_4;
	int ITEM2_FOR_KEY_4;
	int ITEM1_FOR_KEY_5;
	int ITEM2_FOR_KEY_5;
	int ITEM1_FOR_KEY_6;
	int ITEM2_FOR_KEY_6;
	int ITEM1_FOR_KEY_7;
	int ITEM2_FOR_KEY_7;
	int ITEM1_FOR_KEY_8;
	int ITEM2_FOR_KEY_8;
} StuffEnumKCOMBO;

// ----------------  ENUM SHORTCUT FOR EX16_ -----------------
typedef struct StrEnumEX16 {
	int EXIST_1;
	int EXIST_2;
	int EXIST_3;
	int EXIST_4;
	int EXIST_5;
	int EXIST_6;
	int EXIST_7;
	int EXIST_8;
	int EXIST_9;
	int EXIST_10;
	int EXIST_11;
	int EXIST_12;
	int EXIST_13;
	int EXIST_14;
	int EXIST_15;
	int EXIST_16;
} StuffEnumEX16;

// ----------------  ENUM SHORTCUT FOR PCOMBO_ -----------------
typedef struct StrEnumPCOMBO {
	int ITEM1_FOR_PICKUP_1;
	int ITEM2_FOR_PICKUP_1;
	int ITEM1_FOR_PICKUP_2;
	int ITEM2_FOR_PICKUP_2;
	int ITEM1_FOR_PICKUP_3;
	int ITEM2_FOR_PICKUP_3;
	int ITEM1_FOR_PICKUP_4;
	int ITEM2_FOR_PICKUP_4;
	int ITEM1_FOR_PICKUP_5;
	int ITEM2_FOR_PICKUP_5;
	int ITEM1_FOR_PICKUP_6;
	int ITEM2_FOR_PICKUP_6;
	int ITEM1_FOR_PICKUP_7;
	int ITEM2_FOR_PICKUP_7;
	int ITEM1_FOR_PICKUP_8;
	int ITEM2_FOR_PICKUP_8;
} StuffEnumPCOMBO;

// ----------------  ENUM SHORTCUT FOR DENV_ -----------------
typedef struct StrEnumDENV {
	int FLAG_FACE2FACE;
	int FLAG_FACE2BACK;
	int MASK_DISTANCE;
	int MASK_FLAGS;
} StuffEnumDENV;

// ----------------  ENUM SHORTCUT FOR SLOPE_ -----------------
typedef struct StrEnumSLOPE {
	int FLAT;	// no slope, all corners have same height
	int GENTLE_SLOPE;	// slope (1 or 2 clicks) on a single side where lara is able to walk
	int STEEP_SLOPE;	// slope (3 clicks or higher) on a single side where lara is not able to walk
	int GENTLE_CORNER;	// triangle slope (1 or 2 clicks) on a single corner where lara is able to walk
	int STEEP_CORNER;	// triangle slope (3 clicks or higher) on a single corner where lara is not able to walk
} StuffEnumSLOPE;

// ----------------  ENUM SHORTCUT FOR ORIENT_ -----------------
typedef struct StrEnumORIENT {
	int NORTH;	// 0xC000
	int SOUTH;	// 0x4000
	int EAST;	// 0x0000
	int WEST;	// 0x8000
	int SOUTH_EAST;	// 0x2000
	int NORTH_EAST;	// 0xE000
	int SOUTH_WEST;	// 0x6000
	int NORTH_WEST;	// 0xA000
} StuffEnumORIENT;

// ----------------  ENUM SHORTCUT FOR FSLOT_ -----------------
typedef struct StrEnumFSLOT {
	int NONE;	// used only to clear Flags field, to test presence of slot use (Flags & FSLOT_PRESENT)
	int PRESENT;	// object of this slot is present
	int AI_STANDARD;	// (not sure) enemy with traditional AI and movements (yes baddies, no wraith)
	int CHANGE_POS_ITEM;	// this item could change its position: it's necessary save its coordinate in savegame
	int MOVED_BY_ANIMATIONS;	// (not sure) moved whereby animations
	int SAVE_ALL_DATA;	// this item requires many data, other position, to be saved in savegame
	int AFFECT_LARA_AT_CONTACT;	// (not sure) lara could interacts with this item when she is closed to it
	int SFX_LOCAL_SOUND;	// the sounds of this moveable played in its local position (otherwise: global sound)
	int USE_COLLISION_BOX;	// item collisions will be checked whereby its collisional box
	int AMPHIBIOUS_CREATURE;	// creature is able to move underwater
	int HIT_BUT_NOT_HURT_BY_SHOTGUN;	// (not sure) shotgun ammo disturb them without hurt them (mummy and skeleton)
	int NO_DAMAGE_FOR_NO_EXPLOSIVE_AMMO;	// invulnerable at not explosive ammo
	int SAVE_MESH_MASK;	// this item could change visibility of its meshes: save also mesh visibility status
} StuffEnumFSLOT;

// ----------------  ENUM SHORTCUT FOR FITEM_ -----------------
typedef struct StrEnumFITEM {
	int NONE;	// used only to clear flags
	int ACTIVE;	// item is active (it will move) flag set after calling AddActiveItem()
	int CREATURE;	// Creatures are invisible until they have not been triggered
	int NOT_VISIBLE;	// the item was not visible at start
	int GRAVITY_AFFECTED;	// the item is falling down (or jumping up) and it's subject to gravity simulation, currently
	int FLAG_10;	// (let for backward compatibility but now it is sure its meaning: see the FITEM_ITEM_HAS_BEEN_HIT flag
	int ITEM_HAS_BEEN_HIT;	// the item has been hit: with weapons (but not grenade)
	int NOT_YET_ENABLED;	// the item has not yet been enabled (triggered)
	int KILLED_WITH_EXPLOSION;	// trng flag, added to remember that this enemy has been killed with an explosion
	int POISONED;	// enemy (or lara?) has been poisoned
	int AI_GUARD;	// enemy was over a AI_GUARD item
	int AI_AMBUSH;	// emeny was over a AI_AMBUSH item
	int AI_PATROL1;	// enemy was over a AI_PATROL1 (and perhaps AI_PATROL2)
	int AI_MODIFY;	// enemy was over a AI_MODIFY item
	int AI_FOLLOW;	// enemy was over a AI_FOLLOW item
	int THROWN_AMMO;	// (not sure) it used with visible ammo like greande or arrows of crossbow
} StuffEnumFITEM;

// ----------------  ENUM SHORTCUT FOR MEMT_ -----------------
typedef struct StrEnumMEMT {
	int SAVEGAME;
	int CODE;
	int ITEM;
	int SLOT;
	int ANIMATION;
	int INVENTORY;
} StuffEnumMEMT;


// #TAG_END_AUTO_ENUM_STUFF#

#pragma pack(2)

