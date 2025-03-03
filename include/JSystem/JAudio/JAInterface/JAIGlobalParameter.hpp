#ifndef JAIGLOBALPARAMETER_HPP
#define JAIGLOBALPARAMETER_HPP

#include <types.h>

class JAIGlobalParameter {
public:
	// Setter methods
	static void setParamSoundOutputMode(u32);
	static void setParamAudioSystemThreadPriority(u8);
	static void setParamAudioDvdThreadPriority(u8);
	static void setParamInitFileLoadSwitch(u8);
	static void setParamInitDataPointer(void*);
	static void setParamInterfaceHeapSize(u32);
	static void setParamSoundSceneMax(u32);
	static void setParamSeRegistMax(u32);
	static void setParamSeTrackMax(u32);
	static void setParamSeqPlayTrackMax(u32);
	static void setParamSeqControlBufferMax(u32);
	static void setParamStreamControlBufferMax(u32);
	static void setParamStreamParameterBufferMax(u32);
	static void setParamAutoHeapMax(u32);
	static void setParamStayHeapMax(u32);
	static void setParamInputGainDown(f32);
	static void setParamOutputGainUp(f32);
	static void setParamDistanceMax(f32);
	static void setParamMaxVolumeDistance(f32);
	static void setParamMinDistanceVolume(f32);
	static void setParamSeDistanceFxParameter(u16);
	static void setParamInitDataLoadOffFlag(bool);
	static void setParamStreamUseOffFlag(bool);
	static void setParamStreamDecodedBufferBlocks(u32);
	static void setParamStreamInsideBufferCut(bool);
	static void setParamAutoHeapRoomSize(u32);
	static void setParamStayHeapSize(u32);
	static void setParamSeDolbyCenterValue(u8);
	static void setParamSeDolbyFrontDistanceMax(f32);
	static void setParamSeDolbyBehindDistanceMax(f32);
	static void setParamInitDataFileName(char*);
	static void setParamSeInfoFileName(char*);
	static void setParamSeqInfoFileName(char*);
	static void setParamStreamInfoFileName(char*);
	static void setParamWavePath(char*);
	static void setParamSequenceArchivesPath(char*);
	static void setParamStreamPath(char*);
	static void setParamAudioResPath(char*);
	static void setParamSequenceArchivesFileName(char*);
	static void setParamSeqEntryCancelFlag(bool);
	static void setParamStreamEntryCancelFlag(bool);
	static void setParamDopplarMoveTime(u32);
	static void setParamDistanceParameterMoveTime(u8);
	static void setParamDummyPositionLifeTime(u32);
	static void setParamDummyPositionMax(u32);
	static void setParamSeqMuteVolumeSePlay(u8);
	static void setParamSeqMuteMoveSpeedSePlay(u32);
	static void setParamAudioCameraMax(u32);
	static void setParamSystemTrackMax(s32);
	static void setParamSystemRootTrackMax(s32);
	static void setParamPanDistanceMax(f32);
	static void setParamPanAngleParameter(f32);
	static void setParamDopplarParameter(f32);
	static void setParamSeDistanceWaitMax(u16);
	static void setParamSeDistancepitchMax(f32);

	// Getter methods
	static u8 getParamAudioSystemThreadPriority();
	static u8 getParamAudioDvdThreadPriority();
	static u8 getParamInitFileLoadSwitch();
	static void* getParamInitDataPointer();
	static u32 getParamInterfaceHeapSize();
	static u32 getParamSeCategoryMax();
	static u32 getParamSoundSceneMax();
	static u32 getParamSeRegistMax();
	static u32 getParamSeTrackMax();
	static u32 getParamSeqTrackMax();
	static u32 getParamSeqControlBufferMax();
	static u32 getParamStreamControlBufferMax();
	static u32 getParamStreamParameterBufferMax();
	static u32 getParamAutoHeapMax();
	static u32 getParamStayHeapMax();
	static u32 getParamSeqPlayTrackMax();
	static f32 getParamInputGainDown();
	static f32 getParamOutputGainUp();
	static f32 getParamDistanceMax();
	static f32 getParamMaxVolumeDistance();
	static f32 getParamMinDistanceVolume();
	static u32 getParamSoundOutputMode();
	static u16 getParamSeDistanceFxParameter();
	static bool getParamInitDataLoadOffFlag();
	static bool getParamStreamUseOffFlag();
	static u32 getParamAutoHeapRoomSize();
	static u32 getParamStayHeapSize();
	static u8 getParamSeDolbyCenterValue();
	static f32 getParamSeDolbyFrontDistanceMax();
	static f32 getParamSeDolbyBehindDistanceMax();
	static char* getParamInitDataFileName();
	static char* getParamSeInfoFileName();
	static char* getParamSeqInfoFileName();
	static char* getParamStreamInfoFileName();
	static char* getParamWavePath();
	static char* getParamSequenceArchivesPath();
	static char* getParamStreamPath();
	static char* getParamAudioResPath();
	static char* getParamSequenceArchivesFileName();
	static bool getParamSeqEntryCancelFlag();
	static bool getParamStreamEntryCancelFlag();
	static u32 getParamDopplarMoveTime();
	static u8 getParamDistanceParameterMoveTime();
	static u32 getParamDummyPositionLifeTime();
	static u32 getParamDummyPositionMax();
	static u8 getParamSeqMuteVolumeSePlay();
	static u32 getParamSeqMuteMoveSpeedSePlay();
	static u32 getParamAudioCameraMax();
	static s32 getParamSystemTrackMax();
	static s32 getParamSystemRootTrackMax();

	// Global parameter variables
	static u8 distanceParameterMoveTime;
	static u8 audioSystemThreadPriority;
	static u8 audioDvdThreadPriority;
	static u8 seqMuteVolumeSePlay;
	static u16 seDistanceFxParameter;
	static u32 soundSceneMax;
	static u32 seRegistMax;
	static u32 seTrackMax;
	static u32 seqTrackMax;
	static u32 seqPlayTrackMax;
	static u32 seqControlBufferMax;
	static u32 streamControlBufferMax;
	static u32 streamParameterBufferMax;
	static u32 autoHeapMax;
	static u32 stayHeapMax;
	static u32 autoHeapRoomSize;
	static u32 stayHeapSize;
	static char* seInfoFileName;
	static char* seqInfoFileName;
	static char* streamInfoFileName;
	static char* initDataFileName;
	static char* wavePath;
	static char* sequenceArchivesPath;
	static char* streamPath;
	static char* sequenceArchivesFileName;
	static f32 inputGainDown;
	static f32 outputGainUp;
	static f32 distanceMax;
	static f32 maxVolumeDistance;
	static f32 seDolbyCenterValue;
	static f32 seDolbyFrontDistanceMax;
	static f32 seDolbyBehindDistanceMax;
	static u32 dopplarMoveTime;
	static u32 dummyPositionLifeTime;
	static u32 dummyPositionMax;
	static u32 seqMuteMoveSpeedSePlay;
	static u32 audioCameraMax;
	static s32 systemTrackMax;
	static s32 systemRootTrackMax;
	static f32 panDistanceMax;
	static f32 panDistance2Max;
	static f32 panAngleParameter;
	static f32 panAngleParameter2;
	static f32 dopplarParameter;
	static u16 seDistanceWaitMax;
	static f32 seDistancepitchMax;
	static u16 seDefaultFx;
	static u32 interfaceHeapSize;
	static char* audioResPath;
	static f32 minDistanceVolume;
};

#endif // JAIGLOBALPARAMETER_HPP
