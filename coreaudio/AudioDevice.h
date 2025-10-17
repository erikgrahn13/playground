#pragma once

#include <string_view>
#include "CoreAudio/CoreAudio.h"

#ifndef verify_noerr
    #define verify_noerr(errorCode) do { OSStatus __err = (errorCode); if (__err != noErr) { fprintf(stderr, "Error %d at %s:%d\n", (int)__err, __FILE__, __LINE__); } } while (0)
#endif

class AudioDevice {
    public:
    AudioDevice();
    AudioDevice(AudioDeviceID audioDeviceID, bool isInput);

    void Init(AudioDeviceID audioDeviceID, bool isInput);
    bool Valid();
    void SetBufferSize(uint32_t size);
    int CountChannels();
    std::string GetName();
    // AudioDeviceID GetAudioDeviceID() { return mID; }

    AudioDeviceID mID;
    private:
    bool mIsInput;
    Float32 mSafetyOffset;      // Changed from int to Float32
    UInt32 mBufferSizeFrames;   // Changed from int to UInt32
    AudioStreamBasicDescription mFormat;
    // std::string mName;
};