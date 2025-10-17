// #include <iostream>
// #include <print>
// #include "CoreAudio/CoreAudio.h"
// #include <AudioUnit/AudioUnit.h>           // AU types + AudioUnit APIs (incl. HAL Output AU)

// #include "AudioDevice.h"


#define checkErr( err) \
if(err) {\
	OSStatus error = static_cast<OSStatus>(err);\
		fprintf(stdout, "CAPlayThrough Error: %ld ->  %s:  %d\n",  (long)error,\
			   __FILE__, \
			   __LINE__\
			   );\
				   fflush(stdout);\
		return err; \
}         


// int getDeviceCount() 
// {
//     AudioObjectPropertyAddress propertyAddress;
//     propertyAddress.mSelector = kAudioHardwarePropertyDevices;
//     propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
//     propertyAddress.mElement = kAudioObjectPropertyElementMain;

//     UInt32 dataSize;

//     AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize);

//     int deviceCount = dataSize / sizeof(AudioObjectID);

//     return deviceCount;
// }

// int getDefaultInputDevice()
// {
//     AudioObjectPropertyAddress propertyAddress;
//     propertyAddress.mSelector = kAudioHardwarePropertyDefaultInputDevice;
//     propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
//     propertyAddress.mElement = kAudioObjectPropertyElementMain;

//     AudioObjectID id;
//     UInt32 dataSize = sizeof(AudioObjectID);
//     AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize, &id);

//     return id;
// }

// void printDeviceNames()
// {
//     auto deviceCount = getDeviceCount();

//     AudioObjectID deviceList[deviceCount];

//     UInt32 dataSize = sizeof(AudioObjectID) * deviceCount;

//     AudioObjectPropertyAddress propertyAddress;
//     propertyAddress.mSelector = kAudioHardwarePropertyDevices;
//     propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
//     propertyAddress.mElement = kAudioObjectPropertyElementMain;

//     AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &dataSize, &deviceList);



//     propertyAddress.mSelector = kAudioObjectPropertyName;
//     dataSize = sizeof(CFStringRef);
//     for(int i = 0; i < deviceCount; ++i)
//     {
//         CFStringRef name;
//         AudioObjectGetPropertyData(deviceList[i], &propertyAddress, 0, NULL, &dataSize, &name);
//         long length = CFStringGetLength(name);
//         char *mname = (char *)malloc(length * 3 + 1);
        
//         CFStringGetCString(name, mname, length * 3 + 1, CFStringGetSystemEncoding());

//         AudioObjectPropertyAddress theAddress;
//         theAddress.mScope = kAudioDevicePropertyScopeInput;
//         theAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
//         theAddress.mElement = kAudioObjectPropertyElementMain;
//         UInt32 propSize = sizeof(Float32);
//         UInt32 safetyOffset{0};

//         AudioObjectGetPropertyDataSize(deviceList[i], &theAddress, 0, NULL, &safetyOffset);
//         AudioBufferList *bufferList = (AudioBufferList *)malloc(safetyOffset);
//         AudioObjectGetPropertyData(deviceList[i], &theAddress, 0, NULL, &safetyOffset, bufferList);
//         bool hasInput = false;
//         for (UInt32 j = 0; j < bufferList->mNumberBuffers; j++) {
//             if (bufferList->mBuffers[j].mNumberChannels > 0) {  // ✅ Check CHANNELS, not size
//                 hasInput = true;
//                 break;
//             }
//         }


//         std::println("device: {} ({}) {}", deviceList[i], mname, hasInput);
//         // std::cout << "hej: " << mname << std::endl;
//         CFRelease( name );
//         free(mname);
//         free(bufferList);
//     }
// }

// void setInputDeviceAsCurrent(AudioDeviceID in)
// {
//     UInt32 size = sizeof(AudioDeviceID);
//     OSStatus err = noErr;

//     const AudioObjectPropertyAddress theAddress = {
//         .mSelector = kAudioHardwarePropertyDefaultInputDevice,
//         .mScope = kAudioObjectPropertyScopeGlobal,
//         .mElement = kAudioObjectPropertyElementMain
//     };

//     if(in == kAudioDeviceUnknown) {
//         err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &theAddress, 0, nullptr, &size, &in);
//         // checkErr(err);
//     }

// }


// int main() {
//     std::println("Hello Core Audio!");

//     AudioObjectShow(kAudioObjectSystemObject);

//     auto deviceCount = getDeviceCount();    
//     std::println("Device count: {}", deviceCount);


//     auto defaultInputDevice = getDefaultInputDevice();    
//     std::println("Default input device: {}", defaultInputDevice);

//     printDeviceNames();
   


//     std::println("Hello CoreAudio AUHAL");

//     AudioComponent comp;
//     AudioComponentDescription desc;

//     desc.componentType = kAudioUnitType_Output;
//     desc.componentSubType = kAudioUnitSubType_HALOutput;

//     desc.componentManufacturer = kAudioUnitManufacturer_Apple;
//     desc.componentFlags = 0;
//     desc.componentFlagsMask = 0;

//     comp = AudioComponentFindNext(NULL, &desc);
//     if(comp == NULL)
//     {
//         std::println("Error: AudioComponentFindNext");
//         return -1;
//     }

// 	AudioUnit inputUnit;
//     OSStatus err = noErr;
//     err = AudioComponentInstanceNew(comp, &inputUnit);
//     checkErr(err);

//        // THIRD: Initialize AFTER configuration
//     err = AudioUnitInitialize(inputUnit);
//     checkErr(err);

//     // FIRST: Enable I/O BEFORE setting device
//     UInt32 enable = 1;
//     err = AudioUnitSetProperty(inputUnit,
//                                kAudioOutputUnitProperty_EnableIO,
//                                kAudioUnitScope_Input,
//                                1, // input element
//                                &enable,
//                                sizeof(enable));
//     checkErr(err);

//     enable = 0;
//     err = AudioUnitSetProperty(inputUnit,
//                                kAudioOutputUnitProperty_EnableIO,
//                                kAudioUnitScope_Output,
//                                0, // output element
//                                &enable,
//                                sizeof(enable));
//     checkErr(err);

//     // SECOND: Set the input device (element 1)
//     AudioDeviceID inputDeviceID = 126;  // Jabra microphone
//     err = AudioUnitSetProperty(inputUnit,
//                                kAudioOutputUnitProperty_CurrentDevice,
//                                kAudioUnitScope_Global,
//                                0,  // element 1 for INPUT device
//                                &inputDeviceID,
//                                sizeof(inputDeviceID));
//     checkErr(err);

 


//     return 0;
// }
#include <print>
#include <vector>
#include <iostream>
#include <AudioUnit/AudioUnit.h>           // AU types + AudioUnit APIs (incl. HAL Output AU)
#include "AudioDevice.h"
#include "utilities/CARingBuffer.h"
#include "utilities/CAStreamBasicDescription.h"

class CoreAudio {

    public:
    OSStatus SetupInputAUHAL(AudioDeviceID in) {
        std::println("SetupInputAUHAL {}", in);

        OSStatus err = noErr;

        AudioComponent comp;
        AudioComponentDescription desc;

        desc.componentType = kAudioUnitType_Output;
    	desc.componentSubType = kAudioUnitSubType_HALOutput;
        desc.componentManufacturer = kAudioUnitManufacturer_Apple;
        desc.componentFlags = 0;
        desc.componentFlagsMask = 0;

        comp = AudioComponentFindNext(NULL, &desc);
        if(!comp)
            exit(-1);

        err = AudioComponentInstanceNew(comp, &mInputUnit);
        checkErr(err);

        err = AudioUnitInitialize(mInputUnit);
        checkErr(err);

        err = EnableIO();
        checkErr(err);

        err = SetInputDeviceAsCurrent(in);
        checkErr(err);

        err = CallbackSetup();
        checkErr(err);

        err = AudioUnitInitialize(mInputUnit);

        return err;
    }

    OSStatus EnableIO() {
        OSStatus err = noErr;
        UInt32 enableIO;

        enableIO = 1;
        err = AudioUnitSetProperty(mInputUnit,
                                kAudioOutputUnitProperty_EnableIO,
                                kAudioUnitScope_Input,
                                1,
                                &enableIO,
                            sizeof(enableIO));

        checkErr(err);
        enableIO = 0;
        err = AudioUnitSetProperty(mInputUnit,
                                kAudioOutputUnitProperty_EnableIO,
                                    kAudioUnitScope_Output,
                                    0,
                                    &enableIO,
                                    sizeof(enableIO));
        return err;
    }

    OSStatus SetInputDeviceAsCurrent(AudioDeviceID in) {
        UInt32 size = sizeof(AudioDeviceID);
        OSStatus err = noErr;

        AudioObjectPropertyAddress theAddress = {
            .mSelector = kAudioHardwarePropertyDefaultInputDevice,
            .mScope = kAudioObjectPropertyScopeGlobal,
            .mElement = kAudioObjectPropertyElementMain
        };

        if(in == kAudioDeviceUnknown) {
            err = AudioObjectGetPropertyData(kAudioObjectSystemObject, &theAddress, 0, nullptr, &size, &in);
            checkErr(err);
        }

        mInputDevice.Init(in, true);

        err = AudioUnitSetProperty(mInputUnit,
                                    kAudioOutputUnitProperty_CurrentDevice,
                                    kAudioUnitScope_Global,
                                    0,
                                    &mInputDevice.mID,
                                sizeof(mInputDevice.mID));

        checkErr(err);
        return err;
    }

    OSStatus CallbackSetup() {
        OSStatus err = noErr;
        AURenderCallbackStruct input;

        input.inputProc = InputCallBack;
        input.inputProcRefCon = this;

        err = AudioUnitSetProperty(mInputUnit,
                                kAudioOutputUnitProperty_SetInputCallback,
                                kAudioUnitScope_Global,
                                0,
                                &input,
                                sizeof(input));

        checkErr(err);
        return err;
    }

    static OSStatus InputCallBack(void *inRefCon,
                            AudioUnitRenderActionFlags *ioActionsFlags,
                            const AudioTimeStamp *inTimeStamp,
                            UInt32 inBusNumber,
                            UInt32 inNumberFrames,
                            AudioBufferList * ioData)
    {
        OSStatus err = noErr;

        CoreAudio *This = (CoreAudio*)inRefCon;

        if(This->mFirstInputTime < 0.)
            This->mFirstInputTime = inTimeStamp->mSampleTime;

        // AudioUnitRender MUST use element 1 for input AUHAL
        err = AudioUnitRender(This->mInputUnit,
                                ioActionsFlags,
                                inTimeStamp,
                                1,  // element 1 for input
                                inNumberFrames,
                                This->mInputBuffer);

        checkErr(err);

        if(!err) {
            err = This->mBuffer->Store(This->mInputBuffer, Float64(inNumberFrames), SInt64(inTimeStamp->mSampleTime));
        }

        return err;
    }

    static OSStatus OutputCallback(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData)
    {
        CoreAudio *This = (CoreAudio*)inRefCon;
        
        // Check if input has started yet
        if (This->mFirstInputTime < 0.) {
            // Input hasn't run yet -> silence
            for (UInt32 i = 0; i < ioData->mNumberBuffers; i++) {
                memset(ioData->mBuffers[i].mData, 0, ioData->mBuffers[i].mDataByteSize);
            }
            return noErr;
        }
        
        // Fetch from ring buffer into ioData
        This->mBuffer->Fetch(ioData, inNumberFrames, inTimeStamp->mSampleTime);
        
        return noErr;
    }

    void probeDevices() {
        uint32_t propSize;

        AudioObjectPropertyAddress theAddress = {
            .mSelector = kAudioHardwarePropertyDevices,
            .mScope = kAudioObjectPropertyScopeGlobal,
            .mElement = kAudioObjectPropertyElementMain
        };

        AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &theAddress, 0, nullptr, &propSize);
        int nDevices = propSize / sizeof(AudioDeviceID);
        std::vector<AudioDeviceID> deviceIDs(nDevices);
        AudioObjectGetPropertyData(kAudioObjectSystemObject, &theAddress, 0, nullptr, &propSize, deviceIDs.data());

        // auto inputDevices = coreAudio.GetInputDevices();
        // auto outputDevices = coreAudio.GetOutputDevices();
        
        std::println("Input devices:");
        for(int i = 0; i < nDevices; ++i) {
            AudioDevice inputDev(deviceIDs[i], true);
            if(inputDev.CountChannels() > 0) {
                // inputDevices.push_back(inputDev);
                std::println("{}: {}", inputDev.mID, inputDev.GetName());
            }
        }
        std::println();
        std::println("Output devices");
        for(int i = 0; i < nDevices; ++i) {
            AudioDevice outputDev(deviceIDs[i], false);
            if(outputDev.CountChannels() > 0) {
                // outputDevices.push_back(outputDev);
                std::println("{}: {}", outputDev.mID, outputDev.GetName());
            }
        }

        // for(int i = 0; i < inputDevices.size(); ++i) {
        //     std::println("{}: {}", i, inputDevices.at(i).GetName());
        // }

        // std::println();
        // for(int i = 0; i < outputDevices.size(); ++i) {
        //     std::println("{}: {}", i, outputDevices.at(i).GetName());
        // }

    }

    OSStatus SetupOutputAUHAL(AudioDeviceID out){
        OSStatus err = noErr;
        std::println("SetupOutputAUHAL {}", out);


        AudioComponentDescription outDesc;

        outDesc.componentType = kAudioUnitType_Output;
        outDesc.componentSubType = kAudioUnitSubType_HALOutput;
        outDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
        outDesc.componentFlags = 0;
        outDesc.componentFlagsMask = 0;

        AudioComponent comp = AudioComponentFindNext(NULL, &outDesc);
        err = AudioComponentInstanceNew(comp, &mOutputUnit);
        checkErr(err);

        //     err = AudioUnitInitialize(mOutputUnit);
        // checkErr(err);

        // 2. Enable OUTPUT, disable INPUT (opposite of input unit)
        UInt32 enableIO = 1;
        err = AudioUnitSetProperty(mOutputUnit,
                                kAudioOutputUnitProperty_EnableIO,
                                kAudioUnitScope_Output,
                                0,  // output element
                                &enableIO,
                                sizeof(enableIO));
        checkErr(err);

        enableIO = 0;
        err = AudioUnitSetProperty(mOutputUnit,
                                kAudioOutputUnitProperty_EnableIO,
                                kAudioUnitScope_Input,
                                1,  // input element
                                &enableIO,
                                sizeof(enableIO));
        checkErr(err);

        // 3. Set output device
        mOutputDevice.Init(out, false);
        err = AudioUnitSetProperty(mOutputUnit,
                                kAudioOutputUnitProperty_CurrentDevice,
                                kAudioUnitScope_Global,
                                0,
                                &mOutputDevice.mID,
                                sizeof(mOutputDevice.mID));
        checkErr(err);

        // 4. Set RENDER callback (not input callback)
        AURenderCallbackStruct output;
        output.inputProc = OutputCallback;
        output.inputProcRefCon = this;
        
        err = AudioUnitSetProperty(mOutputUnit,
                                kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Global,
                                0,
                                &output,
                                sizeof(output));
        checkErr(err);

        // 5. Initialize
        err = AudioUnitInitialize(mOutputUnit);
        checkErr(err);
        std::println("Output initialized");
        return err;
    }

    OSStatus SetupBuffers() {
        OSStatus err = noErr;
        UInt32 bufferSizeFrames, bufferSizeBytes, propSize;
        CAStreamBasicDescription asbd, asbd_dev1_in, asbd_dev2_out;			
        Float64 inputRate = 0, outputRate;

        // Get the size of the IO buffers
        UInt32 propertySize = sizeof(bufferSizeFrames);
        err = AudioUnitGetProperty(mInputUnit, kAudioDevicePropertyBufferFrameSize, kAudioUnitScope_Global, 0, &bufferSizeFrames, &propertySize);
        checkErr(err);
        bufferSizeBytes = bufferSizeFrames * sizeof(Float32);
        std::println("sizes {} : {}", bufferSizeFrames, bufferSizeBytes);

        // Get INPUT device format (Input scope, element 1)
        propertySize = sizeof(asbd_dev1_in);
        err = AudioUnitGetProperty(mInputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 1, &asbd_dev1_in, &propertySize);
        checkErr(err);
        std::println("====Input DEVICE stream format (Input scope, element 1)");
        asbd_dev1_in.Print();

        // Get what format the input unit WILL output (Output scope, element 1)
        propertySize = sizeof(asbd);
        err = AudioUnitGetProperty(mInputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &asbd, &propertySize);
        checkErr(err);
        std::println("====Input unit OUTPUT format (Output scope, element 1)");
        asbd.Print();

        // Get output device format
        propertySize = sizeof(asbd_dev2_out);
        err = AudioUnitGetProperty(mOutputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &asbd_dev2_out, &propertySize);
        checkErr(err);
        std::println("====Output (Device) stream format");
        asbd_dev2_out.Print();

        // Use minimum channel count
        asbd.mChannelsPerFrame = std::min(asbd_dev1_in.mChannelsPerFrame, asbd_dev2_out.mChannelsPerFrame);
        std::println("Info: Input Device channel count={}\t Output Device channel count={}", 
                    asbd_dev1_in.mChannelsPerFrame, asbd_dev2_out.mChannelsPerFrame);

        // Get and set sample rates
        propertySize = sizeof(Float64);
        AudioObjectPropertyAddress theAddress = { 
            .mSelector = kAudioDevicePropertyNominalSampleRate,
            .mScope = kAudioObjectPropertyScopeGlobal,
            .mElement = kAudioObjectPropertyElementMain };
                                              
        err = AudioObjectGetPropertyData(mInputDevice.mID, &theAddress, 0, nullptr, &propertySize, &inputRate);
        checkErr(err);

        err = AudioObjectGetPropertyData(mOutputDevice.mID, &theAddress, 0, nullptr, &propertySize, &outputRate);
        checkErr(err);

        // Check if rates match
        if (inputRate != outputRate) {
            std::println("Error: Sample rates don't match! Input: {} Hz, Output: {} Hz", 
                        inputRate, outputRate);
            return -1;
        }

        asbd.mSampleRate = inputRate;
        propertySize = sizeof(asbd);

        // SET the format on the input unit's output
        err = AudioUnitSetProperty(mInputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 1, &asbd, propertySize);
        checkErr(err);

        // SET the format on the output unit's input
        err = AudioUnitSetProperty(mOutputUnit, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &asbd, propertySize);
        checkErr(err);

        // Allocate input buffer
        propSize = offsetof(AudioBufferList, mBuffers[0]) + (sizeof(AudioBuffer) * asbd.mChannelsPerFrame);
        mInputBuffer = (AudioBufferList *)malloc(propSize);
        mInputBuffer->mNumberBuffers = asbd.mChannelsPerFrame;
        
        for(UInt32 i = 0; i < mInputBuffer->mNumberBuffers; i++) {
            mInputBuffer->mBuffers[i].mNumberChannels = 1;
            mInputBuffer->mBuffers[i].mDataByteSize = bufferSizeBytes;
            mInputBuffer->mBuffers[i].mData = malloc(bufferSizeBytes);
        }

        // Allocate ring buffer with correct bytes per frame
        mBuffer = new CARingBuffer();
        mBuffer->Allocate(asbd.mChannelsPerFrame, asbd.mBytesPerFrame, bufferSizeFrames * 20);

        std::println("Buffer setup complete:");
        std::println("  Sample rate: {} Hz", inputRate);
        std::println("  Channels: {}", asbd.mChannelsPerFrame);
        std::println("  Bytes per frame: {}", asbd.mBytesPerFrame);
        std::println("  Buffer size: {} frames ({} bytes)", bufferSizeFrames, bufferSizeBytes);

        return err;
    }

    OSStatus Start() {
        OSStatus err = noErr;
        err = AudioOutputUnitStart(mInputUnit);
        checkErr(err);

        err = AudioOutputUnitStart(mOutputUnit);
        checkErr(err);
        
        return err;
    }

    OSStatus Destroy() {
        OSStatus err = noErr;
        err = AudioOutputUnitStop(mInputUnit);
        checkErr(err);

        err = AudioOutputUnitStop(mOutputUnit);
        checkErr(err);
        
        return err;
    }

    // std::vector<AudioDevice>& GetInputDevices() { return mInputDevices; }
    // std::vector<AudioDevice>& GetOutputDevices() { return mOoutputDevices; }
    private:
    // std::vector<AudioDevice> mInputDevices;
    // std::vector<AudioDevice> mOoutputDevices;
    AudioDevice mInputDevice;
    AudioDevice mOutputDevice;
    AudioUnit mInputUnit;
    AudioUnit mOutputUnit;
    Float64 mFirstInputTime = -1.0;  // Initialize to -1
    AudioBufferList *mInputBuffer;
    CARingBuffer *mBuffer;
};

int main()
{
    std::println("Hello CoreAudio AUHAL");

    CoreAudio coreAudio;
    coreAudio.probeDevices();

    std::print("Choose input driver: ");
    AudioDeviceID inputDriver;
    std::cin >> inputDriver;

    std::print("Choose output driver: ");
    AudioDeviceID outputDriver;
    std::cin >> outputDriver;

    std::println("Input: {}\tOutput: {}", inputDriver, outputDriver);

    OSStatus err = noErr;
    err = coreAudio.SetupInputAUHAL(inputDriver);
    std::println("Input successfull");
    checkErr(err);
    // Output device
    coreAudio.SetupOutputAUHAL(outputDriver);

    err = coreAudio.SetupBuffers();
    checkErr(err);

    coreAudio.Start();

    std::println("Audio started - press Enter to stop");
    std::cin.ignore();  // Clear the newline left by std::cin >>
    std::cin.get();     // Wait for user to press Enter

    // Cleanup:
    coreAudio.Destroy();

    return 0;

}