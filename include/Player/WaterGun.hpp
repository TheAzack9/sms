#ifndef WATERGUN_HPP
#define WATERGUN_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Player/NozzleBase.hpp>
#include <Player/NozzleTrigger.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Player/Yoshi.hpp>
#include <Player/MarioMain.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <System/MarioGamePad.hpp>

struct NozzleJointData {
	u8 flags; // TODO: This is likely an enum. 0x1 is used, 0x4 is disabled.
	u8 jointIndex;
};

struct NozzleData {
	/* 0x00 */ u32 _00; // Number of water streams emitted by this nozzle
	/* 0x04 */ u32 _04; // Unused padding
	/* 0x08 */ const char* mHelmetPath; // Path to optional helmet model (used
	                                    // by underwater nozzle)
	/* 0x0C */ const char* mPath;       // Path to base nozzle model directory
	/* 0x10 */ const char* mBmdPath;    // Full path to nozzle's BMD model file
	/* 0x14 */ u8 mNumEmitters; // Number of water emitter points on the nozzle
	/* 0x15 */ u8 _15;          // Unknown/padding
	/* 0x16 */ NozzleJointData mJoints[3]; // Array of joint data for the nozzle
};

// Data structure containing model paths and configuration for each FLUDD nozzle
// type
class TNozzleBmdData {
public:
	u8 getEmitterCount(int index) const { return mNozzles[index].mNumEmitters; }

	u8 getFlags(int index, int jointIndex) const
	{
		return mNozzles[index].mJoints[jointIndex].flags;
	}

	u8 getJointIndex(int index, int jointIndex) const
	{
		return mNozzles[index].mJoints[jointIndex].jointIndex;
	}

	NozzleData mNozzles[6];
};
// Define the global variable in .data section
extern const TNozzleBmdData nozzleBmdData;

class TWaterGun {
public:
	typedef s8 TNozzleType;
	enum { Spray = 0, Rocket, Underwater, Yoshi, Hover, Turbo };

	TWaterGun(TMario* mario);

	virtual void perform(u32, JDrama::TGraphics*);

	void changeBackup();
	void calcAnimation(JDrama::TGraphics*);
	void changeNozzle(TNozzleType, bool);
	bool damage();
	void emit();
	TNozzleBase* getCurrentNozzle() const;
	MtxPtr getEmitMtx(int);
	void getEmitPosDirSpeed(int, JGeometry::TVec3<f32>* pos,
	                        JGeometry::TVec3<f32>* dir,
	                        JGeometry::TVec3<f32>* speed);
	MtxPtr getNozzleMtx();
	f32 getPressure();
	f32 getPressureMax();
	void init();
	void initInLoadAfter();
	bool isEmitting();
	bool isPressureOn();
	void movement();
	void rotateProp(f32);
	void setAmountToRate(f32);
	void setBaseTRMtx(Mtx);
	bool suck();
	void startDashEffect();
	void endDashEffect();
	void getMarioUpperStatus();

	inline MtxPtr getYoshiMtx()
	{
		TYoshi* yoshi = (TYoshi*)this->mMario->yoshi;
		return yoshi->mActor->unk4->getAnmMtx(yoshi->mJoint);
	}

	void decWater() { }

	void checkMax()
	{
		s32 maxWater = this->getCurrentNozzle()->mEmitParams.mAmountMax.get();
		if (this->mCurrentWater > maxWater) {
			this->mCurrentWater = maxWater;
		}
	}

	bool isYoshiNozzle() const { return this->mCurrentNozzle == (s8)Yoshi; }

	bool hasWater() const { return this->mCurrentWater > 0; }

	u32 getWaterGunAnmId();

	J3DModel* getModel() { return this->mFluddModel->unk4; }

	void setEmitPt();

	void finalDrawInitialize();
	void entryAll();
	void createGunBody();

	// Fabricated
	// TODO: Probably not in Watergun
	void startSound(u32 id, const Vec* pos)
	{
		MSoundSESystem::MSoundSE::startSoundActor(id, pos, 0, nullptr, 0, 4);
	}

	// Fabricated
	// TODO: Probably not in Watergun
	inline void playSoundWithInfo(u32 id, const Vec* pos, u32 _unk, f32 _unk2)
	{
		if (gpMSound->gateCheck(id)) {
			MSoundSESystem::MSoundSE::startSoundActorWithInfo(
			    id, pos, nullptr, _unk2, _unk, 0, nullptr, 0, 4);
		}
	}

	// Fabricated
	inline bool hasFlag(u16 flag)
	{
		bool hasFlag;
		if ((this->mFlags & flag) != 0) {
			hasFlag = true;
		} else {
			hasFlag = false;
		}
		return hasFlag;
	}

	// Fabricated
	// TODO: Possibly in each nozzle class?
	inline void playSprayNozzleSound(u32 id, f32 val)
	{
		JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
		playSoundWithInfo(id, emitPos, 0, val);
	}

	// Fabricated
	// TODO: This is definitely not the structure
	// Figure out something better for sound funcs
	inline void playSound2(u32 id)
	{
		JGeometry::TVec3<f32>* emitPos = &mEmitPos[0];
		trySound(id, emitPos);
	}

	s32 getSuckRate()
	{
		return (s32)((f32)this->mCurrentPressure
		             * this->getCurrentNozzle()->mEmitParams.mSuckRate.get());
	}

	s32 getCurrentWater() { return this->mCurrentWater; }
	s32 getMaxWater()
	{
		return this->getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	bool isMaxWater() { return this->getCurrentWater() >= this->getMaxWater(); }

	void trySound(u32 id, JGeometry::TVec3<f32>* pos)
	{
		MSound* sound = gpMSound;
		if (sound->gateCheck(id)) {
			this->startSound(id, pos);
		}
	}

	void incWater(s32 amount)
	{
		this->mCurrentWater += amount;

		s32 currentWater = this->getCurrentWater();
		s32 maxWater     = this->getMaxWater();
		if (currentWater > maxWater) {
			mCurrentWater = maxWater;
		}

		if (!isMaxWater()) {
			JGeometry::TVec3<f32>* emitPos = &this->mEmitPos[0];
			trySound(0xf, emitPos);
		}
	}

	TNozzleBase* getNozzle(u8 index) { return this->mNozzleList[index]; }

	void triggerPressureMovement(const TMarioControllerWork&);

	u16 mFlags;                            // 0x0000
	TMario* mMario;                        // 0x0008
	TNozzleBase mNozzleDeform;             // 0x000C
	TNozzleTrigger mNozzleDeformBomb;      // 0x0390
	TNozzleTrigger mNozzleRocket;          // 0x0720
	TNozzleBase mNozzleUnderWater;         // 0x0AB0
	TNozzleBase mNozzleYoshiDeform;        // 0x0E34
	TNozzleTrigger mNozzleYoshiDeformBomb; // 0x11B8
	TNozzleTrigger mNozzleHover;           // 0x1548
	TNozzleTrigger mNozzleTurbo;           // 0x18D8
	TNozzleBase* mNozzleList[6];           // 0x1C68
	s32 mCurrentWater;                     // 0x1C80
	u8 mCurrentNozzle;                     // 0x1C84
	u8 mSecondNozzle;                      // 0x1C85
	bool mIsEmitWater;                     // 0x1C86
	u8 _13;                                // 0x1C87
	u32 _14;                               // 0x1C88
	u8 mCurrentPressure;                   // 0x1c8c
	u8 mPreviousPressure;                  // 0x1c8d
	u8 _16;                                // 0x1c8e
	u8 _17;                                // 0x1c8f
	JGeometry::TVec3<f32> mEmitPos[4];     // 0x1C90
	s16 _24;                               // 0x1CC0
	s16 _1cc2;                             // 0x1CC2
	s16 _1cc4;                             // 0x1CC4
	s16 _1cc6;                             // 0x1CC6
	f32 mNozzleSpeedY;                     // 0x1CC8
	f32 mNozzleSpeedZ;                     // 0x1CCC
	s16 _1cd0;                             // 0x1CD0
	s16 _1cd2;                             // 0x1CD2
	MActor* mFluddModel;                   // 0x1CD4 // MActor*
	u8 mCurFluddTransformIdx;              // 0x1CD8
	u8 _27;                                // 0x1CD9
	u16 _28;                               // 0x1CDA
	f32 _25;                               // 0x1CDC
	f32 _26;                               // 0x1CE0
	u32 _1ce4;                             // 0x1CE4
	u32 _1ce8;                             // 0x1CE8
	f32 _1cec;                             // 0x1CEC
	JGeometry::TVec3<f32> mGeometry4;      // 0x1CF0
	f32 mFluddSwitchTween;                 // 0x1CFC
	f32 _1d00;                             // 0x1D00
	s16 _1d04;                             // 0x1D04
	s16 _1d06;                             // 0x1D06
	u32 _18;                               // 0x1D08
	u32 mEmitInfo;                         // 0x1D0C // TWaterEmitInfo
	u32 _19;                               // 0x1D10

	struct TWatergunParams : public TParams {
		TWatergunParams(const char* prm)
		    : TParams(prm)
		    , PARAM_INIT(mRocketHeight, 1500.0f)
		    , PARAM_INIT(mHoverHeight, 160.0f)
		    , PARAM_INIT(mLAngleNormal, 60.0f)
		    , PARAM_INIT(mNozzleAngleYSpeed, 1.0f)
		    , PARAM_INIT(mNozzleAngleYBrake, 0.995f)
		    , PARAM_INIT(mHoverRotMax, 0x2000)
		    , PARAM_INIT(mNozzleAngleYSpeedMax, 0x2000)
		    , PARAM_INIT(mChangeSpeed, 0.05f)
		{
		}
		TParamT<f32> mRocketHeight;
		TParamT<f32> mHoverHeight;
		TParamT<f32> mLAngleNormal;
		TParamT<f32> mNozzleAngleYSpeed;
		TParamT<f32> mNozzleAngleYBrake;
		TParamT<s16> mHoverRotMax;
		TParamT<f32> mNozzleAngleYSpeedMax;
		TParamT<f32> mChangeSpeed;
	} mWatergunParams; // 0x1d14
	f32 _1dbc;         // 0x1DBC
	f32 _1dc0;         // 0x1DC0
	f32 _1dc4;         // 0x1DC4
	f32 _1dc8;         // 0x1DC8
	f32 _1dcc;         // 0x1DCC
};

#endif
