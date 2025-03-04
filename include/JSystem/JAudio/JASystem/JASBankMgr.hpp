#ifndef JASBANKMGR_HPP
#define JASBANKMGR_HPP

#include <JSystem/JAudio/JASystem/JASBank.hpp>
#include <types.h>

namespace JASystem {

class TChannel;
class TChannelMgr;

namespace BankMgr {
	void init(int bankCount);
	void registBank(int bankIndex, TBank* bank);
	void registBankBNK(int bankIndex, void* bankData);
	TBank* getBank(int bankIndex);
	u16 getPhysicalNumber(u16 virtualNumber);
	void setVir2PhyTable(u32 tableAddr, int size);
	void assignWaveBank(int bankIndex, int waveBankIndex);
	f32 clamp01(f32 value);
	void noteOn(TChannelMgr* channelMgr, int bankIndex, int programIndex,
	            u8 note, u8 velocity, u32 params);
	void noteOnOsc(TChannelMgr* channelMgr, int bankIndex, u8 note, u8 velocity,
	               u32 params);
	void gateOn(TChannel* channel, u8 note, u8 velocity, u32 params);
	u32 getUsedHeapSize();

	extern f32 OSC_ENV[];
	extern u32 OSC_RELEASE_TABLE[];
	extern u32 sTableSize;
	extern TBank* sBankArray;
	extern u16* sVir2PhyTable;
} // namespace BankMgr

} // namespace JASystem

#endif // JASBANKMGR_HPP
