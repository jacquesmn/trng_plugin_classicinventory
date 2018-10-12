
// FOR_YOU: IMPORTANT WARNING: don't change or add text to this source because it will be
// replaced after new updates and in this way you could lose your adding.
// Use the "macros_mine.h" file to add your macros

// don't remove following line: it's a way to keep memory about current release of 
// your macros.h source
// #VERSION_UPDATE_INFO#=PU-4

// -------------- LEVEL BUILDERS: C++ MACROS ------------------------

// read help file "Plugin for level builders" to have a description of these macros

// CALL_CHECK(ProcName)
// this macro call the given ProcName and check if result is false. If it is false
// abort procedure with "return false"
// to use in the function: CreateMyCodePatches(), InitializeAll() and RequireMyCallBacks()
// to use only with function having no argument ()
#define CALL_CHECK(ProcName) \
	if (ProcName()==false) return false;

// SET_PATCH(ProcName)
// this macro call a function used to create a patch and verify it returns an error. In the case it did, it will return with false value

#define SET_PATCH(ProcName) \
	if (ProcName() < 0) return false;

#define GET_CALLBACK(CallBackCB, CBT_Flags, Index, MyProcToCall) \
	if (GetCallBack(CallBackCB, CBT_Flags, Index, &MyProcToCall) == false) return false;

#ifdef TRNG_BASE_VERSION

#define SHOW_DEBUG(x,y,z) if (GlobTomb4.DebugModeCounter) ShowMsgDebug(x,y,z)
#define SHOW_FULL_DEBUG(x,y,z) if (GlobTomb4.DebugModeCounter && !(GlobTomb4.pDiagnostica->DgxExtra & EDGX_CONCISE_SCRIPT_LOG)) ShowMsgDebug(x,y,z)

#else
#define SHOW_DEBUG(x,y,z) if (Trng.pGlobTomb4->DebugModeCounter) ShowMsgDebug(x,y,z)
#define SHOW_FULL_DEBUG(x,y,z) if (Trng.pGlobTomb4->DebugModeCounter && !(Trng.pGlobTomb4->pDiagnostica->DgxExtra & EDGX_CONCISE_SCRIPT_LOG)) ShowMsgDebug(x,y,z)

#endif


// ------------ ADVANCED: ASSEMBLY MACROS -------------------------
// SAVE_REGISTERS() macro to save the registers required from C++ conventions
// to use in first row of an asm procedure called directly by C++ functions
#define SAVE_REGISTERS \
	__asm push esi \
	__asm push edi \
	__asm push ebx \
	__asm push ebp

// RESTORE_REGISTERS() macro complementary of above SaveRegister, to restore the register previously saved
// to use before exiting from procedure (before of all the "retn" instructions)
#define RESTORE_REGISTERS \
	__asm pop ebp  \
	__asm pop ebx  \
	__asm pop edi  \
	__asm pop esi

// SAVE_MY_REGISTERS() macro to preserve the values of register of our assembly procedure (eax,ecx,edx) that
// could be changed by a C++ function we are calling
// you could use this macro first to insert the arguments of C function
#define SAVE_MY_REGISTERS \
	__asm push eax \
	__asm push ecx \
	__asm push edx

// RESTORE_MY_REGISTERS() macro to restore the values saved with SAVE_MY_REGISTERS() macro.
// you should use it after the instruction "add esp, ..." used after call Function to remove arguments
#define RESTORE_MY_REGISTERS \
	__asm pop edx \
	__asm pop ecx \
	__asm pop eax

// MOVE_OFFSET() This macro will be used to copy in the given register 
// the offset (address) of given subfield of Trng.pGlobTomb4
// The second argument  must be always "GlobalFields" 
// For instance:  MOVE_OFFSET(ecx, GlobalFields.CurrentSlotIndex)
// now in ecx there will be the address where there is the value
// of the index of selected slot (trng variables management)
#define MOVE_OFFSET(Registro, GlobalField) \
	__asm lea Registro, GlobalField \
	__asm add Registro, dword ptr [Trng.pGlobTomb4]

// MOVE_VALUE() this macro is alike the MOVE_OFFSET but in this case it moves directly the value
// of wished field in given register. Note: in this case it's very important use the correct
// type of register, with BYTE, WORD or dword size in according with the size of source field
// example, to move a value with byte size you can type:   MOVE_VALUE(bx, GlobalFields.CurrentSlotIndex)
// where we used a word register (bx) becauste the field has a word size
// remark: you cann't use EBx, Bx or BL as register
#define MOVE_VALUE(Register, GlobalField) \
	__asm  push ebx  \
	__asm  lea ebx, GlobalField \
	__asm  add ebx, dword ptr [Trng.pGlobTomb4] \
	__asm  mov Register, [ebx] \
	__asm  pop ebp
 

// MOVE_OFFSETField() this macro is alike than the MOVE_OFFSET() macro, but in this case it doesn't
// work on pGlobTomb4 structure, but it is able to manage any structure just supplying as second
// argument the pointer (address ) of that structure
#define MOVE_OFFSETField(Register, Address, Fields) \
	__asm mov Register, Address \
	__asm add Register, Fields


// void * GET_ROOM_OFFSET(RoomIndex, RoomField) macro to get the offset of a given field and given
//													roomindex. You have to use RoomFields in second
//													argument to set the wished field name
//													example:
//													GET_ROOM_OFFSET(3, RoomFields.Z_SizeSectors)
//													set in eax register the address of Z_SizeSectors field
//													of Room with index = 3
#define GET_ROOM_OFFSET(RoomIndex, RoomField) \
	__asm push ecx  \
    __asm push edx  \
	__asm lea edx, RoomField \
	__asm push edx \
	__asm push RoomIndex  \
	__asm call GetRoomOff  \
	__asm add esp,8  \
	__asm pop edx \
	__asm pop ecx


// void * GET_ITEM_OFFSET(ItemIndex, ItemFields) macro to get the offset of a given field and given
//													itemindex. You have to use ItemFields in second
//													argument to set the wished field name, 
//													example:
//													GET_ITEM_OFFSET(4, ItemFields.CordY)
//													set in eax register the address of Cordy field
//													of moveable item with index = 4

#define GET_ITEM_OFFSET(ItemIndex, ItemField) \
	__asm push ecx  \
    __asm push edx  \
	__asm lea edx, ItemField \
	__asm push edx \
	__asm push ItemIndex  \
	__asm call GetItemOff  \
	__asm add esp,8  \
	__asm pop edx \
	__asm pop ecx		

//GET_STATIC_OFFSET(StaticIndex, RoomIndex, StaticField)  macro to get the offset of a give Static item
//														with a given StaticIndex, RoomIndex and field
//														example
//														GET_STATIC_OFFSET(2, 5, StaticFields.OCB)
//														set in eax register the address of OCB field
#define GET_STATIC_OFFSET(StaticIndex, RoomIndex, StaticField) \
	__asm push ecx \
	__asm push edx \
	__asm lea eax, StaticField \
	__asm push eax \
	__asm push RoomIndex \
	__asm push StaticIndex \
	__asm call GetStaticOff \
	__asm add esp, 0ch \
	__asm pop edx \
	__asm pop ecx
	// OutRoomIndex, OutStaticIndex, NgleStaticIndex
//											macro to convert the static index you find in NGLE program
	//										or you read from some script command

#define CONVERT_STATIC_INDEX(OutStaticIndex, OutRoomIndex, NgleStaticIndex) \
	__asm  SAVE_MY_REGISTERS \
	__asm  push NgleStaticIndex \
	__asm  call ConvNgleStaticIndex \
	__asm  add esp, 4 \
	__asm  mov word ptr [TempStaticIndex], ax \
	__asm  rol eax, 16  \
	__asm  mov word ptr [TempRoomIndex], ax \
	__asm  RESTORE_MY_REGISTERS \
	__asm  movzx OutRoomIndex, word ptr [TempRoomIndex] \
	__asm  movzx OutStaticIndex, word ptr [TempStaticIndex]




	

// MOVE_ARG() Read the value of the given argument (First=1, second=2, 
// third =3 ect.) and copy it in the given registry. 
// To use in the first rows
// of an asm function called by a c++ code with argument passage. 
// note: you should use this macro first of SAVE_REGISTERS() macro
// but in this case, don't use registers: esi, edi, ebx, or ebp
// usage:
//	MOVE_ARG(eax,1) 
//  MOVE_ARG(ecx,2)
// copy in eax the value of first argument, and in ecx the second.
#define MOVE_ARG(Register, ArgNumber) \
	__asm mov Register, dword ptr [ESP+4*ArgNumber]

// ESP_MEM(Displacement)
// to use ONLY inside of C++ function to handle callback to numerical trng patches
// it used to reach memory, pointed by pRegisters->pESP, adding to it a displacement offset, in assembly style
// It access to memory always as (int), so if you wish a different type you'll have to cast
// the value adding a casting at left of these macro: for instance:
//  (StrItemTr4 *) ESP_MEM(0x40)
#define ESP_MEM(Offset) \
	(* ((int*) &pRegisters->pESP[Offset]))

#define IF_TCMD(Flags) \
	if ((Flags & TGROUP_COMMAND) == TGROUP_COMMAND)


// BEGIN_ASM_PROC(ProcName) Used to contains assembly code.
// From the following row you can type assemply code, and at end
// you have to use the END_ASM_PROC to complete the assembly procedure
// example:
// BEGIN_ASM_PROC(MoveObjUp)
//		mov eax, dword ptr [LastYValue]
//      .... (other asm code)
//    
//      sub eax, 1
//		retn  
// END_ASM_PROC
// then you can call the above asm procedure from other asm code 
// in common way:
// call MoveObjUp

#define BEGIN_ASM_PROC(ProcName) \
__declspec(naked) int ProcName(void) { \
	__asm { 

// END_ASM_PROC
// used to close the asm procedure that begun with a BEGIN_ASM_PROC 
#define END_ASM_PROC \
	} \
}

// BEGIN_ASM
// differently than begin_asm_proc, the BEGIN_ASM macro introduce an asm portion of code
// within a C++ function. You can not use BEGIN_ASM alone, i.e outside of C++
// code, so neither you can use begin_asm_proc within a C++ code.
// This macro saves and restores all registers. This operation is necessary
// when you type asm code within a C++ procedure. Anwyay, since there
// are push/pop operations you can not use the "ret" instruction within 
// an BEGIN_ASM  / END_ASM block
#define BEGIN_ASM \
__asm {  \
__asm	pushad

// END_ASM
// used to close an asm block within of c++ function, started with BEGIN_ASM macro
#define END_ASM \
__asm	popad \
	}



