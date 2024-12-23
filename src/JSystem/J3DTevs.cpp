#include <JSystem/J3D/J3DTevs.hpp>

typedef void IDK;
IDK loadCullMode(u8);
IDK loadColorChanNum(u8);
IDK loadTexGenNum(u32);
IDK loadTexNo(u32, const u16&);
IDK loadTevKColorSel(u32, u8);
IDK loadTevKAlphaSel(u32, u8);
IDK loadTevStageNum(u8);
IDK loadIndTexStageNum(u8);
IDK loadDither(u8);

struct J3DNBTScale;
IDK loadNBTScale(J3DNBTScale&);
IDK makeTexCoordTable();
IDK makeAlphaCmpTable();
IDK makeZModeTable();
IDK makeTevSwapTable();

u8 GXTexMode0Ids[8]  = { 0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3 };
u8 GXTexMode1Ids[8]  = { 0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7 };
u8 GXTexImage0Ids[8] = { 0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB };
u8 GXTexImage1Ids[8] = { 0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF };
u8 GXTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3 };
u8 GXTexImage3Ids[8] = { 0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7 };
u8 GXTexTlutIds[8]   = { 0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB };
