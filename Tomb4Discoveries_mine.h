
// FOR_YOU: here you can type your discoveries about tomb4 procedures
// if you mean use the C language to call them, you can create the
// prototypes of them like it has been done in the "DefTomb4Funct.h" file
// and "functions.h" file.
// While if you wish use only assembly code, you can type here
// the #define ADR_NOME_PROC 0x404040  to remember and use in the asm
// code all addresses of the tomb4 procedures

// TYPE_HERE: prototype (c++ language), mnemonic constants for 
// addresses (assembly)
typedef BOOL(WINAPI *TYPE_BASS_SetEAXParameters) (int env, float vol, float decay, float damp);
typedef BOOL(WINAPI *TYPE_BASS_ChannelSetFX) (DWORD handle, DWORD type, int priority);
typedef void(__cdecl *TYPE_DoFlareInHand) (int flare_age);
typedef void(__cdecl *TYPE_S_DisplayMonoScreen) ();
typedef void(__cdecl *TYPE_phd_LookAt) (int srcX, int srcY, int srcZ, int destX, int destY, int destZ, short roll);
typedef void(__cdecl *TYPE_phd_GetVectorAngles) (int distX, int distY, int distZ, short* dest);
typedef void(__cdecl *TYPE_phd_GenerateW2V) (StrPos3d* viewpos);
typedef void(__cdecl *TYPE_phd_TranslateRel) (int x, int y, int z);
typedef void(__cdecl *TYPE_phd_pushUnitMatrix) ();
typedef void(__cdecl *TYPE_AddFootprint) (StrItemTr4 *pItem);
typedef void(__cdecl *TYPE_phd_RotX) (WORD Orient);
typedef void(__cdecl *TYPE_phd_RotZ) (WORD Orient);
typedef void(__cdecl *TYPE_phd_RotYXZpack) (int rot);
typedef void(__cdecl *TYPE_gar_RotYXZsuperpack) (short **pprot, int skip);
typedef void(__cdecl *TYPE_DrawGameInfo) (int timed);
typedef void(__cdecl *TYPE_DrawPickups) (int timed);
typedef void(__cdecl *TYPE_InitialisePickUpDisplay) ();
typedef void(__cdecl *TYPE_S_DrawPickup) (int slot);
typedef void(__cdecl *TYPE_phd_PutPolygons) (StrMeshTr4 *mesh, int clip);
typedef void(__cdecl *TYPE_phd_PutPolygonsInv) (StrMeshTr4 *mesh, int clip);
typedef void(__cdecl *TYPE_S_PrintShadow) (short size, short *pFrame, StrItemTr4 *pItem, int unknown);
typedef void(__cdecl *TYPE_DrawRooms) (short currentRoom);
typedef void(__cdecl *TYPE_DrawAnimatingItem) (StrItemTr4 *pItem);
typedef void(__cdecl *TYPE_CalculateObjectLighting) (StrItemTr4 *pItem, short *pFrame);
typedef void(__cdecl *TYPE_CalculateObjectLightingLara) ();
typedef void(__cdecl *TYPE_GetLaraHandAbsPosition) (StrMovePosition *pPos, int joint);
typedef void(__cdecl *TYPE_UpdateTextPulse) ();
typedef void(__cdecl *TYPE_use_current_item) ();
typedef int(__cdecl *TYPE_ShowSaveScreen) (int loading_or_saving, bool clear_screen);
typedef void(__cdecl *TYPE_DoStatScreen) ();
typedef void(__cdecl *TYPE_DoOptions) ();
typedef void(__cdecl *TYPE_S_PauseMenu) ();
typedef void(__cdecl *TYPE_InitWaterTable) ();
typedef void(__cdecl *TYPE_AnimateLara) (StrItemTr4 *pLara);
typedef void(__cdecl *TYPE_DrawLara) (StrItemTr4 *pLara, int unknown);
typedef void(__cdecl *TYPE_PlayLevel) (int level_index, int audio_index);
typedef void(__cdecl *TYPE_ShowInventory) ();

// discovered by AODfan
typedef int(__cdecl* TYPE_phd_atan) (int x, int y);
typedef void(__cdecl* TYPE_AlertAllGuards) (short ItemIndex);
typedef void(__cdecl* TYPE_CreatureTilt) (StrItemTr4 *item, short angle);
typedef void(__cdecl* TYPE_CreatureJoint) (StrItemTr4 *item, short joint, short required);
typedef int(__cdecl* TYPE_CreatureAnimation) (short ItemIndex, short angle, short tilt);
typedef short(__cdecl* TYPE_CreatureTurn) (StrItemTr4 *item, short maximum_turn);
typedef void(__cdecl* TYPE_CreatureUnderwater) (StrItemTr4 *item, int depth);
typedef void(__cdecl* TYPE_CreatureFloat) (short ItemIndex);
typedef int(__cdecl* TYPE_CreatureCreature) (short ItemIndex);
typedef int(__cdecl* TYPE_ValidBox) (StrItemTr4 *item, short zone_number, short box_number);
typedef void(__cdecl* TYPE_EscapeBox) (StrItemTr4 *item, StrItemTr4 *enemy, short box_number);
typedef void(__cdecl* TYPE_InitialiseCreature) (short ItemIndex);
typedef int(__cdecl* TYPE_CreatureVault) (short ItemIndex, short angle, int vault, int shift);
typedef void(__cdecl* TYPE_CreatureKill) (StrItemTr4 *item, int kill_anim, int kill_state, int lara_kill_state);
typedef void(__cdecl* TYPE_AlertNearbyGuards) (StrItemTr4 *item);
typedef void(__cdecl* TYPE_AdjustStopperFlag) (StrItemTr4 *item, long dir, long set);
typedef short(__cdecl* TYPE_CreateEffect) (short room_num);
typedef short(__cdecl* TYPE_GunShot) (int x, int y, int z, short speed, short yrot, short room_number); // PHD_ANGLE yrot = short?
typedef short(__cdecl* TYPE_GunHit) (int x, int y, int z, short speed, short yrot, short room_number);
typedef short(__cdecl* TYPE_GunMiss) (int x, int y, int z, short speed, short yrot, short room_number);

// discovered by ChocolateFan
typedef void(__cdecl *TYPE_SeedRandomControl) (int seed);
typedef void(__cdecl *TYPE_DrawHealthBar) (int Percentuale);
typedef void(__cdecl* TYPE_DrawAirBar) (int Percentuale);
typedef void(__cdecl *TYPE_SayNo) ();