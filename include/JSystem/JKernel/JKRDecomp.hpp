#ifndef JKR_DECOMP_HPP
#define JKR_DECOMP_HPP

#include <JSystem/JKernel/JKRThread.hpp>
#include <JSystem/JKernel/JKREnum.hpp>

class JKRAMCommand;
class JKRDecompCommand {
public:
	typedef void (*AsyncCallback)(u32);

	JKRDecompCommand();
	~JKRDecompCommand();

public:
	/* 0x00 */ u32 field_0x0;
	/* 0x04 */ u8* mSrcBuffer;
	/* 0x08 */ u8* mDstBuffer;
	/* 0x0C */ u32 mSrcLength;
	/* 0x10 */ u32 mDstLength;
	/* 0x14 */ AsyncCallback mCallback;
	/* 0x18 */ JKRDecompCommand* mThis;
	/* 0x1C */ OSMessageQueue* field_0x1c;
	/* 0x20 */ s32 field_0x20;
	/* 0x24 */ JKRAMCommand* mAMCommand;
	/* 0x28 */ OSMessageQueue mMessageQueue;
	/* 0x48 */ OSMessage mMessage;
};

#define JKRDECOMP_SYNC_BLOCKING     0
#define JKRDECOMP_SYNC_NON_BLOCKING 1

class JKRDecomp : public JKRThread {
private:
	JKRDecomp(s32);
	virtual ~JKRDecomp();

	/* vt[03] */ virtual void* run(); /* override */

public:
	static JKRDecomp* create(s32);
	static JKRDecompCommand* prepareCommand(u8*, u8*, u32, u32,
	                                        JKRDecompCommand::AsyncCallback);
	static void sendCommand(JKRDecompCommand*);
	static bool sync(JKRDecompCommand*, int);
	static JKRDecompCommand* orderAsync(u8*, u8*, u32, u32,
	                                    JKRDecompCommand::AsyncCallback);
	static bool orderSync(u8*, u8*, u32, u32);
	static void decode(u8*, u8*, u32, u32);
	static void decodeSZP(u8*, u8*, u32, u32);
	static void decodeSZS(u8*, u8*, u32, u32);
	static JKRCompression checkCompressed(u8*);

	static JKRDecomp* sDecompObject;
	static OSMessage sMessageBuffer[4];
	static OSMessageQueue sMessageQueue;
};

inline void JKRDecompress(u8* srcBuffer, u8* dstBuffer, u32 srcLength,
                          u32 dstLength)
{
	JKRDecomp::orderSync(srcBuffer, dstBuffer, srcLength, dstLength);
}

inline JKRDecomp* JKRCreateDecompManager(s32 priority)
{
	return JKRDecomp::create(priority);
}

inline JKRCompression JKRCheckCompressed(u8* pBuf)
{
	return JKRDecomp::checkCompressed(pBuf);
}

// TODO: return type MIGHT be wrong here
inline u32 JKRDecompExpandSize(u8* pBuf)
{
	return (pBuf[4] << 24) | (pBuf[5] << 16) | (pBuf[6] << 8) | pBuf[7];
}

#endif
