#ifndef MSCALC_HPP
#define MSCALC_HPP

#include <dolphin/types.h>

namespace MSCalc {

inline u32 getCategory(u32 sound_id)
{
	u32 uVar1 = sound_id >> 30;
	u32 uVar2 = sound_id >> 12 & 0xF;

	if (uVar1 == 0)
		return uVar2;

	if (uVar1 == 2)
		return 0x10;

	if (uVar1 == 3)
		return 0x11;

	return 0xffffffff;
}

} // namespace MSCalc

#endif // MSCALC_HPP
