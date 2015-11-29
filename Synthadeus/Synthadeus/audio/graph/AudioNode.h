////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//   Audio Node                                                               //
//   Everett Moser                                                            //
//   11-23-15                                                                 //
//                                                                            //
//   Base class for a node in the audio graph                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CFMaths.h"
#include "AudioPlaybackPosition.h"
#include "AudioDefines.h"
#define POTENTIAL_NULL(x, a, d) (((x) != NULL)? (x)->a : (d))
#include "Error.h"

class AudioNode : public AudioPlaybackPosition
{
protected:
	float bufferL[AUDIO_BUFFER_SIZE];
	float bufferR[AUDIO_BUFFER_SIZE];
	int bufferSize;

	// this LCM calculation is done with this specific order of operations to avoid integer overflow (very common)
	static inline int LCM(int A, int B) { int maxA = A, maxB = B; if (maxA < 1) maxA = 1; if (maxB < 1) maxB = 1; return (maxA / GCD(A, B)) * maxB; }
	static int GCD(int A, int B);

public:
	inline AudioNode() : AudioPlaybackPosition() { }
	inline int getBufferSize() { return bufferSize; }
	inline float* getBufferL() { return bufferL; }
	inline float* getBufferR() { return bufferR; }
	inline float getBufferValueL(int pos) { return bufferL[pos % bufferSize]; }
	inline float getBufferValueR(int pos) { return bufferR[pos % bufferSize]; }
	inline float getBufferAtPositionL() { return bufferL[getPositionL() % bufferSize]; }
	inline float getBufferAtPositionR() { return bufferR[getPositionR() % bufferSize]; }
	virtual void recalculate() = 0;
	float lerpValueL(float t);
	float lerpValueR(float t);
};
