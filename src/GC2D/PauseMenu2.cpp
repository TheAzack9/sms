#include "GC2D/CardSave.hpp"
#include "JSystem/JDrama/JDRNameRefGen.hpp"
#include "JSystem/JGeometry/JGVec3.hpp"
#include "JSystem/JUtility/JUTRect.hpp"
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/MessageUtil.hpp>
#include <System/FlagManager.hpp>
// #include "gpMarDirector.hpp"
#include <JSystem/J2D/J2DTextBox.hpp>
#include <System/Application.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DPicture.hpp>

TPauseMenu2::TPauseMenu2(const char* name)
    : JDrama::TViewObj(name),
      unk10(5),
      unk14(nullptr),
      unk18(nullptr),
      unk1C(nullptr),
      unkD4(nullptr),
      unkD8(nullptr),
      unkDC(nullptr),
      unkE0(0),
      unkE8(0.0f),
      unkEC(0.0f),
      unkF8(0),
      unkFC(6),
      unk100(3),
      unk102(0x81),
      unk104(3),
      unk108(0),
      unk109(0),
      unk110(nullptr),
      unk114(0)
{

}

void TPauseMenu2::load(JSUMemoryInputStream &inputStream) {
    JDrama::TNameRef::load(inputStream);

    JKRArchive *volume = (JKRArchive*)JKRFileLoader::getVolume("game_6");
    unk14 = new J2DSetScreen("pause_1.blo", volume);
    unk14->setCullBack(GX_CULL_BACK);

    if(gpApplication.mCurrArea.unk0 < 2) {
        unk104 = 2;
    }
    unk18 = unk14->search('mask');
    unk1C = unk14->search('t_0');

    for(int i = 0; i < 5; ++i) {
        unk20[i] = unk14->search(i + 'pa00');
    }

    for(int i = 0; i < 3; ++i) {
        unk98[i] = unk14->search(i + 'tx_1');
        if(unk104 == 2) {
            // TODO: Verify, complete guess
            // Might be setting alpha to 0
            unk98[i]->setConnectParent(false);
        }

        unk98[i]->hide();
    }

    unkD4 = (J2DTextBox*)unk14->search('map');
    unkD4->setFont(gpSystemFont);

    unkDC = (J2DTextBox*)unk14->search('task');
    SMSMakeTextBuffer(unkDC, 0x80);
    unkDC->setFont(gpSystemFont);

    unkD8 = unk14->search('brek');
    // SMS_getShineStage(gpMarDirector->unk7C);
    s32 currentStage = TFlagManager::getInstance()->getFlag(0x40003);
    void *stageNames = JKRFileLoader::getGlbResource("/common/2d/stagename.bmg");
    const char *stageName = SMSGetMessageData(stageNames, currentStage);
    unkD4->setString(stageName);

    // TODO: Scenario name, requires stuff in MarDirector
}

void TPauseMenu2::loadAfter() {
    unk118 = JDrama::TNameRefGen::getInstance()->search<TCardSave>("データセーブ");
    // TODO: Some loop stuff here

    // TODO: Inlined subtype or unrolled for loop
    unk34[0].x1 = unk20[0]->mBounds.x1;
    unk34[0].y1 = unk20[0]->mBounds.y1;
    unk34[0].x2 = unk20[0]->mBounds.x2;
    unk34[0].y2 = unk20[0]->mBounds.y2;
    unk84[0] = unk20[0]->mRotation;

    unk34[1].x1 = unk20[1]->mBounds.x1;
    unk34[1].y1 = unk20[1]->mBounds.y1;
    unk34[1].x2 = unk20[1]->mBounds.x2;
    unk34[1].y2 = unk20[1]->mBounds.y2;
    unk84[1] = unk20[1]->mRotation;

    unk34[2].x1 = unk20[2]->mBounds.x1;
    unk34[2].y1 = unk20[2]->mBounds.y1;
    unk34[2].x2 = unk20[2]->mBounds.x2;
    unk34[2].y2 = unk20[2]->mBounds.y2;
    unk84[2] = unk20[2]->mRotation;

    unk34[3].x1 = unk20[3]->mBounds.x1;
    unk34[3].y1 = unk20[3]->mBounds.y1;
    unk34[3].x2 = unk20[3]->mBounds.x2;
    unk34[3].y2 = unk20[3]->mBounds.y2;
    unk84[3] = unk20[3]->mRotation;

    unk34[4].x1 = unk20[4]->mBounds.x1;
    unk34[4].y1 = unk20[4]->mBounds.y1;
    unk34[4].x2 = unk20[4]->mBounds.x2;
    unk34[4].y2 = unk20[4]->mBounds.y2;
    unk84[4] = unk20[4]->mRotation;

    unkF0 = unk18->mAlpha;
    unkF4 = unkF0 / 45.0f;
    unkF8 = unk98[0]->mRotation;

    // TODO: Some weird stuff here
}

void TPauseMenu2::appearWindow() {

}

void TPauseMenu2::disappearWindow() {

}

void TPauseMenu2::perform(u32 flags, JDrama::TGraphics* graphics) {

}

u8 TPauseMenu2::getNextState() {
    u8 result = 2;
    if(unk109 != 0) {
        switch(unkE0) {
            case 0: {
                if(unk10 == 5) {
                    result = 0;
                }
                break;
            }
            case 1: {
                if(unk10 == 5) {
                    result = unk118->getNextState();
                }
                break;
            }
            case 2: {
                if(unk10 == 5) {
                    result = 5;
                }
                break;
            }
            default: {
                result = 0;
                break;
            }
        }
    } else {
        if((unk108 != 0) && unk10 == 5) {
            result = 0;
        }
    }

    return result;
}

void TPauseMenu2::setDrawStart() {

}

void TPauseMenu2::setDrawEnd() {

}

void TPauseMenu2::drawAppearPane(J2DPicture *picture, f32 unk1, JUTRect &rect, f32 unk2) {
    JUTRect bounds;

    if(unk2 >= 0.0f) {
        if(unk1 < 20.0f) {
            if(!picture->mVisible) {
                picture->mVisible = true;
                picture->mAlpha = 0;
            }

            if(unk1 == 2.0f) {
                bounds.copy(picture->mGlobalBounds);

                Vec unk1 = {bounds.x2 - bounds.x1, bounds.y2 - bounds.y1, 0.0f};
                // TODO: Used in a createEmitter
                //         JPAEmitterManager::createEmitter
                //          (System::gpEmitterManager4D2,&local_58,0x1f9,(JPACallBackBase *)0x0, (JPACallBackBase2 *)0x0);
            }

            picture->mRotation = unk1;
        }

    }
}
