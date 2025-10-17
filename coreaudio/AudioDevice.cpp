#include "AudioDevice.h"
#include <vector>

AudioDevice::AudioDevice()
    : mID(kAudioDeviceUnknown)
    , mIsInput(false)
    , mSafetyOffset(0)
    , mBufferSizeFrames(0)
{
}

AudioDevice::AudioDevice(AudioDeviceID audioDeviceID, bool isInput)
    : mID(kAudioDeviceUnknown)
    , mIsInput(false)
    , mSafetyOffset(0)
    , mBufferSizeFrames(0)
{
    Init(audioDeviceID, isInput);
}

void AudioDevice::Init(AudioDeviceID audioDeviceID, bool isInput)
{
    mID = audioDeviceID;
    mIsInput = isInput;

    if(mID == kAudioDeviceUnknown)
        return;

    UInt32 propSize = sizeof(Float32);

    AudioObjectPropertyAddress theAddress = {
        .mSelector = kAudioDevicePropertySafetyOffset,
        .mScope = mIsInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
        .mElement = 0 // channel
    };

    verify_noerr(AudioObjectGetPropertyData(mID, &theAddress, 0, nullptr, &propSize, &mSafetyOffset));

    propSize = sizeof(UInt32);
    theAddress.mSelector = kAudioDevicePropertyBufferFrameSize;

    verify_noerr(AudioObjectGetPropertyData(mID, &theAddress, 0, nullptr, &propSize, &mBufferSizeFrames));

    propSize = sizeof(AudioStreamBasicDescription);
    theAddress.mSelector = kAudioDevicePropertyStreamFormat;

    verify_noerr(AudioObjectGetPropertyData(mID, &theAddress, 0,  nullptr, &propSize, &mFormat));
}

void AudioDevice::SetBufferSize(uint32_t size) 
{
    uint32_t propSize = sizeof(uint32_t);

    AudioObjectPropertyAddress theAddress = {
        .mSelector = kAudioDevicePropertyBufferFrameSize,
        .mScope = mIsInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
        .mElement = 0
    };

    verify_noerr(AudioObjectSetPropertyData(mID, &theAddress, 0, nullptr, propSize, &size));
    verify_noerr(AudioObjectGetPropertyData(mID, &theAddress, 0, nullptr, &propSize, &mBufferSizeFrames));
}

int AudioDevice::CountChannels()
{
    int32_t err;
    uint32_t propSize;
    int result = 0;

    AudioObjectPropertyAddress theAddress = {
        .mSelector = kAudioDevicePropertyStreamConfiguration,
        .mScope = mIsInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
        .mElement = 0
    };

    err = AudioObjectGetPropertyDataSize(mID, &theAddress, 0, nullptr, &propSize);
	if (err) 
        return 0;

    std::vector<std::byte> buffer(propSize);
    auto* bufferList = reinterpret_cast<AudioBufferList*>(buffer.data());
    err = AudioObjectGetPropertyData(mID, &theAddress, 0, nullptr, &propSize, bufferList);

    if(!err) {
        for(uint32_t i = 0; i < bufferList->mNumberBuffers; ++i) {
            result += bufferList->mBuffers[i].mNumberChannels;
        }
    }

    return result;
}

std::string AudioDevice::GetName()
{
    AudioObjectPropertyAddress theAddress = {
        .mSelector = kAudioDevicePropertyDeviceName,
        .mScope = mIsInput ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput,
        .mElement = 0
    };

    std::string name(256, '\0');
    uint32_t maxLen = name.size();
    verify_noerr(AudioObjectGetPropertyData(mID, &theAddress, 0, nullptr,  &maxLen, name.data()));
    name.resize(std::strlen(name.c_str()));
    
    return name;

}