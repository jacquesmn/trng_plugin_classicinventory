TYPE_BASS_SetEAXParameters SetEAXParameters;
TYPE_BASS_ChannelSetFX ChannelSetFX;
TYPE_DoFlareInHand DoFlareInHand = (TYPE_DoFlareInHand)0x42F270;
TYPE_S_DisplayMonoScreen S_DisplayMonoScreen = (TYPE_S_DisplayMonoScreen)0x4786C0;
TYPE_phd_LookAt phd_LookAt = (TYPE_phd_LookAt)0x48DD90;
TYPE_phd_GetVectorAngles phd_GetVectorAngles = (TYPE_phd_GetVectorAngles)0x48E710;
TYPE_phd_GenerateW2V phd_GenerateW2V = (TYPE_phd_GenerateW2V)0x48DA40;
TYPE_phd_TranslateRel phd_TranslateRel = (TYPE_phd_TranslateRel)0x48DFD0;
TYPE_phd_pushUnitMatrix phd_pushUnitMatrix = (TYPE_phd_pushUnitMatrix)0x48DF90;
TYPE_AddFootprint AddFootprint = (TYPE_AddFootprint)0x437790;
TYPE_phd_RotX phd_RotX = (TYPE_phd_RotX)0x48E080;
TYPE_phd_RotZ phd_RotZ = (TYPE_phd_RotZ)0x48E1E0;
TYPE_gar_RotYXZsuperpack gar_RotYXZsuperpack = (TYPE_gar_RotYXZsuperpack)0x44FE00;
TYPE_phd_RotYXZpack phd_RotYXZpack = (TYPE_phd_RotYXZpack)0x48E290;
TYPE_DrawGameInfo DrawGameInfo = (TYPE_DrawGameInfo)0x4521C0;
TYPE_DrawPickups DrawPickups = (TYPE_DrawPickups)0x452480;
TYPE_InitialisePickUpDisplay InitialisePickUpDisplay = (TYPE_InitialisePickUpDisplay)0x452440;
TYPE_S_DrawPickup S_DrawPickup = (TYPE_S_DrawPickup)0x47D8F0;
TYPE_phd_PutPolygons phd_PutPolygons = (TYPE_phd_PutPolygons)0x47BB30;
TYPE_phd_PutPolygonsInv phd_PutPolygonsInv = (TYPE_phd_PutPolygonsInv)0x47C950;
TYPE_S_PrintShadow S_PrintShadow = (TYPE_S_PrintShadow)0x485480;
TYPE_DrawRooms DrawRooms = (TYPE_DrawRooms)0x44E2C0;
TYPE_DrawAnimatingItem DrawAnimatingItem = (TYPE_DrawAnimatingItem)0x44F600;
TYPE_CalculateObjectLighting CalculateObjectLighting = (TYPE_CalculateObjectLighting)0x450240;
TYPE_CalculateObjectLightingLara CalculateObjectLightingLara = (TYPE_CalculateObjectLightingLara)0x450340;
TYPE_GetLaraHandAbsPosition GetLaraHandAbsPosition = (TYPE_GetLaraHandAbsPosition)0x41D890;
TYPE_UpdateTextPulse UpdateTextPulse = (TYPE_UpdateTextPulse)0x462AE0;
TYPE_use_current_item use_current_item = (TYPE_use_current_item)0x43E090;
TYPE_ShowSaveScreen ShowSaveScreen = (TYPE_ShowSaveScreen)0x47AE70;
TYPE_DoStatScreen DoStatScreen = (TYPE_DoStatScreen)0x47A830;
TYPE_DoOptions DoOptions = (TYPE_DoOptions)0x479300;
TYPE_S_PauseMenu S_PauseMenu = (TYPE_S_PauseMenu)0x47ADB0;
TYPE_InitWaterTable InitWaterTable = (TYPE_InitWaterTable)0x475390;

// discovered by AODfan
TYPE_phd_atan phd_atan = (TYPE_phd_atan)0x48DE90;
TYPE_AlertAllGuards AlertAllGuards = (TYPE_AlertAllGuards)0x441D10;
TYPE_CreatureTilt CreatureTilt = (TYPE_CreatureTilt)0x4416B0;
TYPE_CreatureJoint CreatureJoint = (TYPE_CreatureJoint)0x4416F0;
TYPE_CreatureAnimation CreatureAnimation = (TYPE_CreatureAnimation)0x4409F0;
TYPE_CreatureTurn CreatureTurn = (TYPE_CreatureTurn)0x441430;
TYPE_CreatureUnderwater CreatureUnderwater = (TYPE_CreatureUnderwater)0x441850;
TYPE_CreatureFloat CreatureFloat = (TYPE_CreatureFloat)0x441750;
TYPE_CreatureCreature CreatureCreature = (TYPE_CreatureCreature)0x440220; // Not sure
TYPE_ValidBox ValidBox = (TYPE_ValidBox)0x43FB20;
TYPE_EscapeBox EscapeBox = (TYPE_EscapeBox)0x43FA70;
TYPE_InitialiseCreature InitialiseCreature = (TYPE_InitialiseCreature)0x43F330;
TYPE_CreatureVault CreatureVault = (TYPE_CreatureVault)0x4419A0; // 0x441940 <- this one seems to crash the game
TYPE_CreatureKill CreatureKill = (TYPE_CreatureKill)0x4419A0; // not sure - might be CreatureVault
TYPE_AlertNearbyGuards AlertNearbyGuards = (TYPE_AlertNearbyGuards)0x441D90;
TYPE_AdjustStopperFlag AdjustStopperFlag = (TYPE_AdjustStopperFlag)0x442400; // ?
TYPE_CreateEffect CreateEffect = (TYPE_CreateEffect)0x453DD0;
TYPE_GunShot GunShot = (TYPE_GunShot)0x40AF90; // this one is weird - definitely check
TYPE_GunHit GunHit = (TYPE_GunHit)0x40AFA0;
TYPE_GunMiss GunMiss = (TYPE_GunMiss)0x40B060;

// discovered by ChocolateFan
TYPE_SeedRandomControl SeedRandomControl = (TYPE_SeedRandomControl)0x48EB90;