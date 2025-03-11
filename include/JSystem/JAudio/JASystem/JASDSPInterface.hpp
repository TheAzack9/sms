#ifndef JASDSPINTERFACE_HPP
#define JASDSPINTERFACE_HPP

#include <dolphin/types.h>

namespace JASystem {

namespace Driver {
	struct Wave_;
}

namespace DSPInterface {
	struct FxlineConfig_;

	class DSPBuffer {
	public:
		void flushChannel();
		void allocInit();
		void initFilter();
		void setAutoMixer(u16, u8, u8, u8, u8);
		void setMixerVolumeOnly(u8, s16);
		void setPitch(u16);
		void setPauseFlag(u8);
		void setMixerVolume(u8, s16, u8);
		void setIIRFilterParam(s16*);
		void setFIR8FilterParam(s16*);
		void setFilterMode(u16);
		void setDistFilter(s16);
		void setWaveInfo(Driver::Wave_*, u32);
		void setOscInfo(u32);
		void setBusConnect(u8, u8);
		void initAutoMixer();
		void setMixerInitDelayMax(u8);
		void setMixerInitVolume(u8, s16, u8);
		void playStart();
		void playStop();
		void updateAMVolume(u16);
		void updateAMPan(u8, u8);
		void updateAMFX(u8);
		void setPitchIndirect(f32, f32);
		void cacheChannel();

	public: // copypasta from TWW, probably wrong
		/* 0x00 */ u16 unk0;
		/* 0x02 */ u16 unk2;
		/* 0x04 */ u16 unk4;
		/* 0x06 */ u8 unk6[0x8 - 0x6];
		/* 0x08 */ u16 unk8;
		/* 0x0A */ u8 unkA[0xc - 0xa];
		/* 0x0C */ u16 unkC;
		/* 0x0E */ u16 unkE;
		/* 0x10 */ u16 unk10[1][4];
		/* 0x18 */ u8 unk18[0x50 - 0x18];
		/* 0x50 */ u16 unk50;
		/* 0x52 */ u16 unk52;
		/* 0x54 */ short unk54;
		/* 0x56 */ u16 unk56;
		/* 0x58 */ u16 unk58;
		/* 0x5A */ u8 unk5A[0x60 - 0x5a];
		/* 0x60 */ short unk60;
		/* 0x62 */ u8 unk62[0x64 - 0x62];
		/* 0x64 */ short unk64;
		/* 0x66 */ short unk66;
		/* 0x68 */ int unk68;
		/* 0x6C */ u32 unk6C;
		/* 0x70 */ u8 unk70[0x74 - 0x70];
		/* 0x74 */ u32 unk74;
		/* 0x78 */ short unk78[4];
		/* 0x80 */ short unk80[20];
		/* 0xA8 */ short unkA8[4];
		/* 0xB8 */ u16 unkB0[16];
		/* 0xD0 */ u8 unkD0[0x100 - 0xd0];
		/* 0x100 */ u16 unk100;
		/* 0x102 */ u16 unk102;
		/* 0x104 */ short unk104;
		/* 0x106 */ short unk106;
		/* 0x108 */ short unk108;
		/* 0x10A */ u16 unk10A;
		/* 0x10C */ u32 unk10C;
		/* 0x110 */ u32 unk110;
		/* 0x114 */ u32 unk114;
		/* 0x118 */ u32 unk118;
		/* 0x11C */ int unk11C;
		/* 0x120 */ short unk120[8];
		/* 0x130 */ u8 unk130[0x148 - 0x130];
		/* 0x148 */ short unk148[4];
		/* 0x150 */ short unk150;
		/* 0x152 */ u8 unk152[0x180 - 0x152];
	};

	class FXBuffer {
	public:
		void setFXLine(s16*, FxlineConfig_*);
		void changeFXLineParam(u8, u32);
	};

	extern u32 SEND_TABLE;
	extern u16 JAS_DSP_PREFIX;
	extern u32 CH_BUF;
	extern u32 FX_BUF;

	DSPBuffer* getDSPHandle(u8);
	DSPBuffer* getDSPHandleNc(u8);
	FXBuffer* getFXHandle(u8);
	FXBuffer* getFXHandleNc(u8);
	void setFilterTable(s16*, s16*, u32);
	void flushBuffer();
	void flushChannelAll();
	void cacheChannelAll();
	void invalChannelAll();
	void clearBuffer();
	void setupBuffer();
	void initBuffer();
} // namespace DSPInterface

} // namespace JASystem

#endif // JASDSPINTERFACE_HPP
