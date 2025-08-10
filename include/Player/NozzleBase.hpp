#ifndef NOZZLEBASE_HPP
#define NOZZLEBASE_HPP

#include <Dolphin/types.h>
#include <JSystem/JGeometry.hpp>
#include "System/ParamInst.hpp"
#include "System/Params.hpp"
#include <M3DUtil/MActor.hpp>
#include <System/MarioGamePad.hpp>

#include <JSystem/JDrama/JDRNameRef.hpp>

// struct TWaterEmitInfo : public TParams {
//     TWaterEmitInfo(const char *prm);

//     TParamT<s32> mNum;
//     TParamT<s16> mAlive;
//     TParamT<s16> mAttack;
//     TParamT<JGeometry::TVec3<f32>> mDir;
//     TParamT<JGeometry::TVec3<f32>> mPos;
//     TParamT<JGeometry::TVec3<f32>> mV;
//     TParamT<f32> mDirTremble;
//     TParamT<f32> mPow;
//     TParamT<f32> mPowTremble;
//     TParamT<f32> mSize;
//     TParamT<f32> mSizeTremble;
//     TParamT<f32> mHitRadius;
//     TParamT<f32> mHitHeight;
//     TParamT<s32> mFlag;
//     TParamT<s16> mType;

// private:
//     TParamT<s16> __padding;
// };

class TWaterGun;
// class TMarioControllerWork;

class TNozzleBase {
public:
	struct TEmitParams : public TParams {
		TEmitParams(const char* prm)
		    : TParams(prm)
		    , PARAM_INIT(mRocketType, 0)
		    , PARAM_INIT(mNum, 1.0f)
		    , PARAM_INIT(mAttack, 1)
		    , PARAM_INIT(mDirTremble, 0.0099999998)
		    , PARAM_INIT(mEmitPow, 40.0)
		    , PARAM_INIT(mEmitCtrl, 1.0f)
		    , PARAM_INIT(mPowTremble, 1.0f)
		    , PARAM_INIT(mSize, 40.0f)
		    , PARAM_INIT(mSizeTremble, 16.0f)
		    , PARAM_INIT(mAmountMax, 0x834)
		    , PARAM_INIT(mReactionPow, 0.0f)
		    , PARAM_INIT(mReactionY, 0.0f)
		    , PARAM_INIT(mDecRate, 0)
		    , PARAM_INIT(mTriggerRate, 0x100)
		    , PARAM_INIT(mDamageLoss, 0xfa)
		    , PARAM_INIT(mSuckRate, 0.1f)
		    , PARAM_INIT(mHitRadius, 50.0f)
		    , PARAM_INIT(mHitHeight, 80.0f)
		    , PARAM_INIT(mLAngleBase, 0x1000)
		    , PARAM_INIT(mLAngleNormal, 12000)
		    , PARAM_INIT(mLAngleSquat, 12000)
		    , PARAM_INIT(mLAngleMin, -0x2000)
		    , PARAM_INIT(mLAngleMax, 0x2000)
		    , PARAM_INIT(mLAngleChase, 0.1f)
		    , PARAM_INIT(mSizeMinPressure, 0.0f)
		    , PARAM_INIT(mSizeMaxPressure, 1.0f)
		    , PARAM_INIT(mNumMin, 1.0f)
		    , PARAM_INIT(mAttackMin, 1)
		    , PARAM_INIT(mDirTrembleMin, 0.0099999998)
		    , PARAM_INIT(mEmitPowMin, 40.0f)
		    , PARAM_INIT(mSizeMin, 40.0f)
		    , PARAM_INIT(mMotorPowMin, 5.0f)
		    , PARAM_INIT(mMotorPowMax, 25.0f)
		    , PARAM_INIT(mReactionPowMin, 0.0f)
		    , PARAM_INIT(mInsidePressureDec, 100.0f)
		    , PARAM_INIT(mInsidePressureMax, 4500.0f)
		    , PARAM_INIT(mTriggerTime, 1)
		    , PARAM_INIT(mType, 0)
		    , PARAM_INIT(mSideAngleMaxSide, 0x4000)
		    , PARAM_INIT(mSideAngleMaxFront, 0x4000)
		    , PARAM_INIT(mSideAngleMaxBack, 0x2000)
		    , PARAM_INIT(mRButtonMult, 10000.0)
		    , PARAM_INIT(mEmitPowScale, 10.0f)

		{
		}

		TParamRT<u8> mRocketType;
		TParamRT<f32> mNum;
		TParamRT<u16> mAttack;
		TParamRT<f32> mDirTremble;
		TParamRT<f32> mEmitPow;
		TParamRT<f32> mEmitCtrl;
		TParamRT<f32> mPowTremble;
		TParamRT<f32> mSize;
		TParamRT<f32> mSizeTremble;
		TParamRT<s32> mAmountMax;
		TParamRT<f32> mReactionPow;
		TParamRT<f32> mReactionY;
		TParamRT<s16> mDecRate;
		TParamRT<s16> mTriggerRate;
		TParamRT<s32> mDamageLoss;
		TParamRT<f32> mSuckRate;
		TParamRT<f32> mHitRadius;
		TParamRT<f32> mHitHeight;
		TParamRT<s16> mLAngleBase;
		TParamRT<s16> mLAngleNormal;
		TParamRT<s16> mLAngleSquat;
		TParamRT<s16> mLAngleMin;
		TParamRT<s16> mLAngleMax;
		TParamRT<s16> mLAngleChase;
		TParamRT<f32> mSizeMinPressure;
		TParamRT<f32> mSizeMaxPressure;
		TParamRT<f32> mNumMin;
		TParamRT<s16> mAttackMin;
		TParamRT<f32> mDirTrembleMin;
		TParamRT<f32> mEmitPowMin;
		TParamRT<f32> mSizeMin;
		TParamRT<f32> mMotorPowMin;
		TParamRT<f32> mMotorPowMax;
		TParamRT<f32> mReactionPowMin;
		TParamRT<f32> mInsidePressureDec;
		TParamRT<f32> mInsidePressureMax;
		TParamRT<s16> mTriggerTime;
		TParamRT<s16> mType;
		TParamRT<s16> mSideAngleMaxSide;
		TParamRT<s16> mSideAngleMaxFront;
		TParamRT<s16> mSideAngleMaxBack;
		TParamRT<f32> mRButtonMult;
		TParamRT<f32> mEmitPowScale;
	} mEmitParams; // 0x0000

	TNozzleBase(const char* name, const char* prm, TWaterGun* fludd);

	virtual void init();
	virtual inline s32 getNozzleKind() const { return 0; }
	virtual inline s16 getGunAngle() { return this->mGunAngle; }
	virtual inline s16 getWaistAngle() { return this->mWaistAngle; }
	virtual void movement(const TMarioControllerWork&);
	virtual void emitCommon(int, u32* /* WaterEmitInfo*/);
	virtual void emit(int);
	virtual void animation(int);

	void calcGunAngle(const void*);

	TWaterGun* mFludd; // 0x0368
	u16 mAnimState;
	s16 mGunAngle;
	s16 mWaistAngle;
	s16 _372;
	f32 _374;
	f32 _378;
	f32 _37C;
	MActor* mActor;
}; // 0x384

#endif
