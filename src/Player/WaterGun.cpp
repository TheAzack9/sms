#include <Player/Watergun.hpp>
#include <Player/NozzleTrigger.hpp>
#include <Player/NozzleBase.hpp>

#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Player/MarioMain.hpp>
#include <Player/Yoshi.hpp>

#include <M3DUtil/MActor.hpp>

#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

#include <gpMarDirector.hpp>

#include <System/StageUtil.hpp>

#include <MarioUtil/MathUtil.hpp>

// Define the global variable in .data section
const TNozzleBmdData nozzleBmdData
    = { { { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/normal_wg",               // mPath
	        "/mario/watergun2/normal_wg/normal_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                          // _00
	        0,                                          // _04
	        nullptr,                                    // mHelmetPath
	        "/mario/watergun2/rocket_wg",               // mPath
	        "/mario/watergun2/rocket_wg/rocket_wg.bmd", // mBmdPath
	        1,                                          // mNumEmitters
	        2,                                          // _15
	        { { 2, 1 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        "/mario/bmd/wg_hel_diver.bmd",            // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/dummy_wg",              // mPath
	        "/mario/watergun2/dummy_wg/dummy_wg.bmd", // mBmdPath
	        1,                                        // mNumEmitters
	        2,                                        // _15
	        { { 3, 0 }, { 4, 0 }, { 4, 0 } } },
	      { 0,                                        // _00
	        0,                                        // _04
	        nullptr,                                  // mHelmetPath
	        "/mario/watergun2/hover_wg",              // mPath
	        "/mario/watergun2/hover_wg/hover_wg.bmd", // mBmdPath
	        2,                                        // mNumEmitters
	        0xc,                                      // _15
	        { { 1, 0 }, { 1, 0 }, { 4, 0 } } },
	      { 0,                                      // _00
	        0,                                      // _04
	        nullptr,                                // mHelmetPath
	        "/mario/watergun2/back_wg",             // mPath
	        "/mario/watergun2/back_wg/back_wg.bmd", // mBmdPath
	        1,                                      // mNumEmitters
	        2,                                      // _15
	        { { 1, 0 }, { 4, 0 }, { 4, 0 } } } } };

TWaterGun::TWaterGun(TMario* mario)
    : mMario(mario)
    , mNozzleDeform("spray", "spray.prm", this)
    , mNozzleDeformBomb("bomb", "bomb.prm", this)
    , mNozzleRocket("rocket", "rocket.prm", this)
    , mNozzleUnderWater("underwater", "underwater.prm", this)
    , mNozzleYoshiDeform("yoshi", "yoshi.prm", this)
    , mNozzleYoshiDeformBomb("yoshibomb", "yoshibomb.prm", this)
    , mNozzleHover("hover", "hover.prm", this)
    , mNozzleTurbo("turbo", "turbo.prm", this)
    , mWatergunParams("watergun.prm")
{
	mNozzleList[0] = &mNozzleDeform;
	mNozzleList[1] = &mNozzleDeformBomb;
	mNozzleList[2] = &mNozzleRocket;
	mNozzleList[3] = &mNozzleUnderWater;
	mNozzleList[4] = &mNozzleYoshiDeform;
	mNozzleList[5] = &mNozzleYoshiDeformBomb;
	mNozzleList[6] = &mNozzleHover;
	mNozzleList[7] = &mNozzleTurbo;
}

s32 TNozzleTrigger::getNozzleKind() const { return 1; }

// TODO: Nozzle deform

void TWaterGun::init() { }

void TWaterGun::calcAnimation(JDrama::TGraphics* graphics) { }

void TWaterGun::changeNozzle(TNozzleType type, bool immediate) { }

#pragma dont_inline on
MtxPtr TWaterGun::getEmitMtx(int jointIndex)
{
	volatile u32 unused2[24]; // TODO: A lot of stack space, possibly a lot of
	                          // inlined functions.
	MtxPtr result;
	if (!this->mMario->onYoshi()) {
		result = this->getYoshiMtx();
	} else {
		// This entire block is likely an inlined function.
		u8 currentNozzle = this->mCurrentNozzle;
		s8 flag          = nozzleBmdData.getFlags(currentNozzle, jointIndex);
		if (flag < 3) {
			return this->getCurrentNozzle()->mActor->unk4->getAnmMtx(
			    nozzleBmdData.getJointIndex(this->mCurrentNozzle, jointIndex));
		} else if (flag == 3) {
			return getYoshiMtx();
		}
	}
	return result;
}
#pragma dont_inline off

void TWaterGun::initInLoadAfter() { }

bool TWaterGun::isEmitting() { return false; }

void TWaterGun::movement()
{
	volatile u32 unused2[69]; // TODO: A lot of stack space, possibly a lot of

	bool canSpray; // Not sure if this is correct variable name
	if (mCurrentWater == 0) {
		canSpray = false;
	} else {
		s32 kind = getCurrentNozzle()->getNozzleKind();
		if (kind == 1) {
			TNozzleTrigger* triggerNozzle
			    = (TNozzleTrigger*)mNozzleList[mCurrentNozzle];
			if (triggerNozzle->mSprayState == TNozzleTrigger::ACTIVE) {
				canSpray = true;
			} else {
				canSpray = false;
			}
		} else if (getCurrentNozzle()->_378 > 0.0f) {
			canSpray = true;
		} else {
			canSpray = false;
		}
	}

	if (!canSpray) {
		_1cc2 = 0;
		_1cc4 = 0;
	}

	mNozzleSpeedY
	    += (_1cc2 - mNozzleSpeedY) * mWatergunParams.mChangeSpeed.get();
	mNozzleSpeedZ
	    += (_1cc4 - mNozzleSpeedZ) * mWatergunParams.mChangeSpeed.get();

	TNozzleBase* currentNozzle = this->getCurrentNozzle();

	rotateProp(currentNozzle->_378);

	// They do the same thing again?... This is the exact same code as
	// rotateProp
	if (mCurrentNozzle == 5) {
		_1cd2 += mNozzleList[mCurrentNozzle]->_378
		         * mWatergunParams.mNozzleAngleYSpeed.get();
		_1cd2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < _1cd2) {
			_1cd2 = mWatergunParams.mHoverRotMax.get();
		}
		_1cd0 = _1cd0 + _1cd2;
	} else {
		_1cd2 = 0;
		_1cd0 = 0;
	}

	// Yoshi nozzle
	if (mCurrentNozzle == 3) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (SMS_isDivingMap()) {
		mCurrentWater = getCurrentNozzle()->mEmitParams.mAmountMax.get();
	}

	if (mCurrentNozzle == 3) {
		_1cec = 0.0f;
	}

	// Nozzle swapping
	if (_1d00 != 0.0f) {
		f32 unk           = mFluddSwitchTween;
		f32 sum           = unk + _1d00;
		mFluddSwitchTween = sum;
		if ((unk < 0.5f) && (0.5f <= sum)) {
			u8 curNozzle        = mCurrentNozzle;
			s32 currentWater    = mCurrentWater;
			u8 secondNozzle     = mSecondNozzle;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			if (secondNozzle != 0) {
				mSecondNozzle = secondNozzle;
			}
			mCurrentNozzle = secondNozzle;

			currentNozzle = this->getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			if (secondNozzle == 3) {
				mCurrentWater
				    = mMario->yoshi->_11[0]; // TODO: Proper yoshi stuff
			} else {
				mCurrentWater = waterPercentage
				                * currentNozzle->mEmitParams.mAmountMax.get();
			}
		}
		if ((sum < 0.5f) && (0.5f <= unk)) {
			f32 currentWater    = (f32)mCurrentWater;
			f32 maxWater        = currentNozzle->mEmitParams.mAmountMax.get();
			f32 waterPercentage = currentWater / maxWater;

			mCurrentNozzle = 0;

			currentNozzle = this->getCurrentNozzle();
			currentNozzle->init(); // TODO: 2 vtable entry

			mCurrentWater
			    = waterPercentage * currentNozzle->mEmitParams.mAmountMax.get();
		}

		if (mFluddSwitchTween < 0.0) {
			mFluddSwitchTween = 0.0f;
			_1d00             = 0.0f;
		}
		if (1.0f < mFluddSwitchTween) {
			mFluddSwitchTween = 1.0f;
			_1d00             = 0.0f;
		}
	}
	currentNozzle->animation(mCurrentNozzle);
}

void TWaterGun::setBaseTRMtx(Mtx mtx)
{
	volatile u32 unused1[10];
	f32 initialAngle = mtx[1][0];
	if (initialAngle < 0.0f) {
		initialAngle = -initialAngle;
	}

	// Seemingly some adjustment of fluddpack angle
	f32 baseAngle = _1d06;
	f32 angleDiff = _1d04 - _1d06;

	s16 angle = initialAngle * angleDiff + baseAngle;

	Mtx result;
	Vec unused2;
	MsMtxSetRotRPH(result, 0.0f, 0.0f, 0.005493164f * angle);

	PSMTXConcat(mtx, result, result);
	PSMTXCopy(result, mFluddModel->unk4->mBaseMtx);
}

void TWaterGun::setAmountToRate(f32 rate)
{
	volatile u32 unused2[7]; // TODO: possibly inlined function
	if (this->mCurrentNozzle == 3) {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		s32 amountMax       = currentNozzle->mEmitParams.mAmountMax.get();
		this->mCurrentWater = amountMax;
	} else {
		TNozzleBase* currentNozzle = this->getCurrentNozzle();
		this->mCurrentWater
		    = rate * currentNozzle->mEmitParams.mAmountMax.get();
	}
}

TNozzleBase::TNozzleBase(const char* name, const char* prm, TWaterGun* fludd)
    : mEmitParams(prm)
    , mFludd(fludd)
{
	mEmitParams.load(prm);
	this->mAnimState = 2;
	this->mGunAngle  = 0;
	this->_372       = 0;
	this->_378       = 0.0f;
	this->_37C       = 0.0f;
}

void TNozzleBase::init()
{

	this->mAnimState = 2;
	this->mGunAngle  = 0;
	this->_372       = 0;
	this->_378       = 0.0f;
	this->_37C       = 0.0f;
}

// TODO: This has a lot of inline functions, find them and update them properly
void TNozzleBase::animation(int param_1)
{

	if (param_1 != 2) {
		return;
	}

	if (0.0f < this->mFludd->_1d00) {
		this->mAnimState = 4;
	}

	if (this->mFludd->_1d00 < 0.0f) {
		this->mAnimState = 3;
	}

	switch (this->mAnimState) {
	case 0: {

		if (!this->mActor->checkCurBckFromIndex(4)) {
			this->mActor->setBckFromIndex(4);
		}
		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (ctrl->mCurrentFrame <= (ctrl->mEndFrame - 0.1)
		    && (ctrl->mFlags & 3) == 0) {
			return;
		}
		this->mAnimState = 1;
		break;
	}
	case 1: {

		if (!this->mActor->checkCurBckFromIndex(2)) {
			this->mActor->setBckFromIndex(2);
		}

		TWaterGun* fludd     = this->mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->mGameState;
			if (gameState != 3 && gameState != 4) {
				updateAnimation = true;
				if (gameState != 1 && gameState != 2) {
					updateAnimation = true;
				}

				if (!updateAnimation) {
					updateAnimation = true;
					u32 nozzleKind = fludd->getCurrentNozzle()->getNozzleKind();
					if (nozzleKind == 1) {
						TNozzleTrigger* trigger
						    = (TNozzleTrigger*)fludd->getCurrentNozzle();
						if (trigger->mSprayState == 1) {
							updateAnimation = true;
						} else {
							updateAnimation = false;
						}

					} else if (fludd->getCurrentNozzle()->_378 <= 0.0f) {
						updateAnimation = false;
					} else {
						updateAnimation = true;
					}
				}
			}
		}

		if (updateAnimation) {
			return;
		}

		this->mAnimState = 2;
		break;
	}
	case 2: {

		if (!this->mActor->checkCurBckFromIndex(3)) {
			this->mActor->setBckFromIndex(3);
		}
		TWaterGun* fludd     = this->mFludd;
		bool updateAnimation = false;
		if (fludd->mCurrentWater == 0) {
			updateAnimation = false;
		} else {
			u8 gameState = gpMarDirector->mGameState;
			if (gameState != 3 && gameState != 4) {
				updateAnimation = true;
				if (gameState != 1 && gameState != 2) {
					updateAnimation = true;
				}

				if (!updateAnimation) {
					updateAnimation = true;
					u32 nozzleKind = fludd->getCurrentNozzle()->getNozzleKind();
					if (nozzleKind == 1) {
						TNozzleTrigger* trigger
						    = (TNozzleTrigger*)fludd->getCurrentNozzle();
						if (trigger->mSprayState == 1) {
							updateAnimation = true;
						} else {
							updateAnimation = false;
						}

					} else if (fludd->getCurrentNozzle()->_378 <= 0.0f) {
						updateAnimation = false;
					} else {
						updateAnimation = true;
					}
				}
			}
		}

		if (updateAnimation) {
			this->mAnimState = 0;
		}
		break;
	}

	case 3: {

		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (!this->mActor->checkCurBckFromIndex(1)) {
			this->mActor->setBckFromIndex(1);
		}

		// Use external tween value
		ctrl->mCurrentFrame = this->mFludd->mFluddSwitchTween * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;
		break;
	}
	case 4: {

		J3DFrameCtrl* ctrl = this->mActor->getFrameCtrl(MActor::BCK);
		if (!this->mActor->checkCurBckFromIndex(0)) {
			this->mActor->setBckFromIndex(0);
		}

		// Use external tween value
		ctrl->mCurrentFrame = this->mFludd->mFluddSwitchTween * ctrl->mEndFrame;
		ctrl->mSpeed        = 0.0f;

		if (this->mFludd->mFluddSwitchTween < 1.0f) {
			return;
		}
		this->mAnimState = 0;
		break;
	}
	}
}

void TNozzleBase::movement(const TMarioControllerWork& controllerWork)
{
	if (this->mFludd->mCurrentWater > 0) { }
}

inline void TWaterGun::setEmitPt()
{
	int index;
	int offset;
	// TODO: Register r27 and r28 are swapped on some instructions. I cannot
	// figure out how to swap them correctly. Very likely some inline setter.
	// Below looks ugly, but closest i could get.
	for (index = 0, offset = 0;
	     index < nozzleBmdData.getEmitterCount(this->mCurrentNozzle);
	     index += 1, offset += 0xc) {
		MtxPtr p1 = this->getEmitMtx(index);
		if (p1 != nullptr) {
			mEmitPos[index].set(p1[0][3], p1[1][3], p1[2][3]);
		}
	}
}

void TWaterGun::perform(u32 flags, JDrama::TGraphics* graphics)
{
	// TODO: Missing stack space
	volatile u32 unused2[22];

	if ((flags & 0x1) != 0) {
		if ((this->mFlags & 0x10) != 0) {
			this->mCurrentWater = 0;
		}
		this->movement();
	}

	if ((flags & 0x2) != 0) {
		this->calcAnimation(graphics);
	}

	this->mFluddModel->perform(flags, graphics);

	if ((flags & 0x2) != 0) {
		MActor* p2 = this->getCurrentNozzle()->mActor;
		if (p2 != nullptr) {
			PSMTXCopy(this->getModel()->getAnmMtx(this->mCurFluddTransformIdx),
			          p2->unk4->mBaseMtx);
		}

		setEmitPt();
	}

	MActor* p2 = this->getCurrentNozzle()->mActor;
	if (p2 != nullptr) {
		p2->perform(flags, graphics);
	}
}

TNozzleBase* TWaterGun::getCurrentNozzle() const
{
	return mNozzleList[mCurrentNozzle];
}

bool TWaterGun::isPressureOn()
{
	volatile u32 unused2[6];
	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)this->getCurrentNozzle();
		if (triggerNozzle->mTriggerFill > 0.0f) {
			return true;
		}
	}
	return false;
}

f32 TWaterGun::getPressure()
{
	// TODO: Missing stack space
	volatile u32 unused2[5];
	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		TNozzleTrigger* triggerNozzle
		    = (TNozzleTrigger*)this->getCurrentNozzle();
		return triggerNozzle->mTriggerFill;
	}
	return 0.0f;
}

f32 TWaterGun::getPressureMax()
{
	// TODO: Missing stack space
	volatile u32 unused2[6];

	if (this->getCurrentNozzle()->getNozzleKind() == 1) {
		return this->getCurrentNozzle()->mEmitParams.mInsidePressureMax.get();
	}

	return 0.0f;
}
MtxPtr TWaterGun::getNozzleMtx()
{
	return this->mFluddModel->unk4->getAnmMtx(this->mCurFluddTransformIdx);
}

void TWaterGun::getEmitPosDirSpeed(int index, JGeometry::TVec3<f32>* pos,
                                   JGeometry::TVec3<f32>* dir,
                                   JGeometry::TVec3<f32>* speed)
{
	// TODO: Fix unused stack space
	volatile u32 unused2[6];

	MtxPtr nozzleEmitMtx = getEmitMtx(index);
	pos->set(mEmitPos[index]);

	if (nozzleEmitMtx != nullptr) {
		dir->x = nozzleEmitMtx[0][0];
		dir->y = nozzleEmitMtx[1][0];
		dir->z = nozzleEmitMtx[2][0];
	} else {
		dir->set(0.0f, 0.0f, 1.0f);
	}

	speed->x = mMario->vel.x * 0.125f;
	speed->y = 0.0f;
	speed->z = mMario->vel.z * 0.125f;
}
void TWaterGun::rotateProp(f32 rotation)
{
	if (mCurrentNozzle == 5) {
		_1cd2 += rotation * mWatergunParams.mNozzleAngleYSpeed.get();
		_1cd2 *= mWatergunParams.mNozzleAngleYBrake.get();
		if (mWatergunParams.mHoverRotMax.get() < _1cd2) {
			_1cd2 = mWatergunParams.mHoverRotMax.get();
		}
		_1cd0 = _1cd0 + _1cd2;
	} else {
		_1cd2 = 0;
		_1cd0 = 0;
	}
}

void TWaterGun::triggerPressureMovement(
    const TMarioControllerWork& controllerWork)
{
	mCurrentPressure = controllerWork.mAnalogL * 150.0f;

	TNozzleBase* currentNozzle = getCurrentNozzle();
	currentNozzle->movement(controllerWork);

	if (mCurrentPressure > mPreviousPressure) {
		mPreviousPressure = mCurrentPressure;
	} else if (mPreviousPressure != 0) {
		mPreviousPressure -= 1;
	} else {
		mPreviousPressure = 0;
	}
}
void TWaterGun::emit()
{
	// TODO: Missing stack space
	volatile u32 unused1[25];

	if (!mMario->hasAttribute(HAS_HELMET_FLW_CAMERA)
	    && mMario->hasAttribute(IS_SHALLOW_WATER | IS_WATER)) {
		// I can imagine this also being an inline function that checks if
		// the emit point is below the water height, but i will leave it for
		// now. TODO.
		MtxPtr nozzleEmitMtx;
		if ((nozzleEmitMtx = getEmitMtx(0)) != nullptr) {
			if (nozzleEmitMtx[1][3] < mMario->mWaterHeight + 20.0f) {
				return;
			}
		}
	}

	if (!mMario->onYoshi()) {
		if (this->_1cec > 0.0f) {
			return;
		}
	}

	// TODO: Probably an enum
	if (this->hasFlag(0x4)) {
		// TODO: Probably inline function?
		this->mFlags &= ~0x4;
		return;
	}

	u8 currentNozzleId         = mCurrentNozzle;
	TNozzleBase* currentNozzle = getNozzle(currentNozzleId);
	for (int i = 0; i < nozzleBmdData.getEmitterCount(currentNozzleId); ++i) {
		currentNozzle->emit(i);
	}
	if (mCurrentWater > 0) {
		switch (currentNozzleId) {
		case 0: {
			playSprayNozzleSound(0x24, getCurrentNozzle()->_374);
		}
		case 3:
		case 5: {
			playSprayNozzleSound(0x0, getCurrentNozzle()->_378);
			break;
		}
		case 2: {
			playSound2(0x18);
		} break;
		case 1:
			break;
		case 4:
			if (mIsEmitWater) {
				playSound2(0x18);
			}
			break;
		}
	}
}
bool TWaterGun::suck()
{
	// TODO: Missing stack space
	volatile u32 unused1[7];
	if (this->isYoshiNozzle()) {
		return false;
	} else {
		s32 suckRate = this->getSuckRate();
		if (suckRate > 0) {
			this->incWater(suckRate);
			return true;
		}
	}
	return false;
}
bool TWaterGun::damage()
{
	if (this->hasWater()) {
		TNozzleBase* nozzle = this->getCurrentNozzle();

		this->mCurrentWater -= nozzle->mEmitParams.mDamageLoss.value;

		if (this->mCurrentWater < 0) {
			this->mCurrentWater = 0;
		}
		return true;
	}
	return false;
}

void TWaterGun::changeBackup()
{
	volatile u32 unused2[7]; // TODO: A lot of stack space, possibly a lot of
	if (this->mFluddSwitchTween == 0.0f) {
		if (gpMSound->gateCheck(0x812)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x812, 0, nullptr, 0);
		}
		this->_1d00 = this->_1dcc;
	}

	if (this->mFluddSwitchTween == 1.0f) {
		if (gpMSound->gateCheck(0x811)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x811, 0, nullptr, 0);
		}
		this->_1d00 = -this->_1dcc;
	}
}
