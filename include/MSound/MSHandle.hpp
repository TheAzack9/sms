#ifndef MSHANDLE_HPP
#define MSHANDLE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

struct SeCategory {
	u32 mType;
	f32 _04;
	f32 _08;
	f32 _0C;
};

class MSHandle {
public:
	static SeCategory smSeCategory[];
	static f32 smACosPrm[];
	static f32 cPan_MaxAmp;
	static f32 cPan_CAdjust;
	static f32 cPan_CShift;
	static f32 cPan_HiSence_Dist;
	static f32 cMS_DistanceMax_Sence;
	static f32 cDol_0Rad;
	static f32 cDol_HalfRad;
	static f32 cDol_FullRad;

	virtual ~MSHandle() { }

	void setDistanceVolumeCommon(f32 volume, u8 param);
	void setSeDistancePitch(u8 param);
	void setSeDistanceVolume(u8 param);
	void setSeDistanceDolby(u8 param);
	void setSeDistancePan(u8 param);
	void setSeDistanceParameters();

	f32 calcVolume(f32 param1, f32 param2, f32 param3, u8 param4, u8 param5);
	f32 calcPan(const Vec& vec, f32 param1, f32 param2);
	f32 calcDolby(const Vec& vec, f32 param);
	f32 MSACos(f32 param);
};

#endif // MSHANDLE_HPP
