#define NOMINMAX
#include <iostream>
#include <algorithm>
#include <limits>
#include <span>
#include <string>
#include <bitset>
#include <vector>
#include "asiodrivers.h"
#include "asio.h"
#include <ctype.h>

extern AsioDrivers *asioDrivers;
bool loadAsioDriver(char *name);

class AudioDeviceAsio;
static AudioDeviceAsio *audioDeviceAsioPtr{nullptr};

class AudioStreamProperties
{
public:
    long totalNumInputChannels;
    long totalNumOutputChannels;
    std::vector<std::string> inputChannelNames;
    std::vector<std::string> outputChannelNames;
    std::bitset<32> activeInputChannels;
    std::bitset<32> activeOutputChannels;
    std::string driverName;
};

class AudioDeviceAsio
{
    public:
    AudioDeviceAsio() {
        callbacks.asioMessage = &asioMessage;
        callbacks.bufferSwitch = &bufferSwitch;
        callbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
        callbacks.sampleRateDidChange = &sampleRateDidChange;

        audioDeviceAsioPtr = this;
    };

    ~AudioDeviceAsio() {};

    static AudioStreamProperties probeDriver(std::string& driver)
    {
        std::cout << "probeDriver: " << driver << std::endl;
        auto status = loadAsioDriver(&driver[0]);
        if(status == 0)
        {
            std::cout << "probeDriver: Failed to load ASIO driver" << std::endl;
        }
        else
        {
            std::cout << "probeDriver: Successfully loaded ASIO driver" << std::endl;
        }
        AudioStreamProperties audioStreamProperties;
        audioStreamProperties.driverName = driver;

        ASIODriverInfo driverInfo;
        if(ASIOInit(&driverInfo) == ASE_OK)
        {
            std::cout << driverInfo.asioVersion << std::endl;
            std::cout << driverInfo.driverVersion << std::endl;
            std::cout << driverInfo.name << std::endl;
            std::cout << driverInfo.errorMessage << std::endl;
        }

        if(ASIOGetChannels(&audioStreamProperties.totalNumInputChannels, &audioStreamProperties.totalNumOutputChannels) == ASE_OK)
        {
            // std::cout << "numInputChannels = " << inputChannels << std::endl;
            // std::cout << "numOutputChannels = " << outputChannels << std::endl;
        }

        std::vector<ASIOChannelInfo> channelInfos;
        channelInfos.resize(audioStreamProperties.totalNumInputChannels + audioStreamProperties.totalNumOutputChannels);

        // Get the input channel names
        for(int i = 0; i < audioStreamProperties.totalNumInputChannels; ++i)
        {
            channelInfos.at(i).channel = i;
            channelInfos.at(i).isInput = ASIOTrue;

            if(ASIOGetChannelInfo(&channelInfos.at(i)) != ASE_OK)
            {
                std::cout << "Not able to get input channel info" << std::endl;
            }
            audioStreamProperties.inputChannelNames.push_back(channelInfos.at(i).name);
        }

        // Get the output channel names
        for(int i = 0; i < audioStreamProperties.totalNumOutputChannels; ++i)
        {
            channelInfos.at(i).channel = i;
            channelInfos.at(i).isInput = ASIOFalse;

            if(ASIOGetChannelInfo(&channelInfos.at(i)) != ASE_OK)
            {
                std::cout << "Not able to get output channel info" << std::endl;
            }
            audioStreamProperties.outputChannelNames.push_back(channelInfos.at(i).name);
        }

        if(ASIOExit() == ASE_OK)
        {
            std::cout << "Exit ASIO successfully" << std::endl;
        }
        else
        {
            std::cout << "Exit ASIO failed" << std::endl;
        }

        return audioStreamProperties;
    }

    int openStream(AudioStreamProperties &audioStreamProperties) {
        mAudioStreamProperties = audioStreamProperties;

        auto status = loadAsioDriver(&audioStreamProperties.driverName[0]);
        if(status == 0)
        {
            std::cout << "Failed to load ASIO driver" << std::endl;
        }
        else
        {
            std::cout << "Successfully loaded ASIO driver" << std::endl;
        }

        if(ASIOInit(&driverInfo) == ASE_OK)
        {
            std::cout << driverInfo.asioVersion << std::endl;
            std::cout << driverInfo.driverVersion << std::endl;
            std::cout << driverInfo.name << std::endl;
            std::cout << driverInfo.errorMessage << std::endl;
        }

        if(ASIOGetChannels(&inputChannels, &outputChannels) == ASE_OK)
        {
            std::cout << "numInputChannels = " << inputChannels << std::endl;
            std::cout << "numOutputChannels = " << outputChannels << std::endl;
        }

        if(ASIOGetLatencies(&inputLatency, &outputLatency) == ASE_OK)
        {
            std::cout << "inputLatency = " << inputLatency << std::endl;
            std::cout << "outputLatency = " << outputLatency << std::endl;
        }

        if(ASIOGetBufferSize(&minSize, &maxSize, &preferredSize, &granularity) == ASE_OK)
        {
            std::cout << "minSize = " << minSize << std::endl;
            std::cout << "maxSize = " << maxSize << std::endl;
            std::cout << "preferredSize = " << preferredSize << std::endl;
            std::cout << "granularity = " << granularity << std::endl;
        }

        if(ASIOGetSampleRate(&sampleRate) == ASE_OK)
        {
            std::cout << "currentRate = " << sampleRate << std::endl;
        }

        if(ASIOOutputReady() == ASE_OK)
        {
            postOutput = true;
            std::cout << "Output is ready" << std::endl;
        }
        else
        {
            postOutput = false;
        }

        std::cout << "After OuputReady" << std::endl;

        // ASIOClockSource clocks;
        // long numSources;
        // if(ASIOGetClockSources(&clocks, &numSources) == ASE_OK)
        // {
        //     std::cout << "clocks.index = " << clocks.index << std::endl;
        //     std::cout << "clocks.associatedChannel = " << clocks.associatedChannel << std::endl;
        //     std::cout << "clocks.associatedGroup = " << clocks.associatedGroup << std::endl;
        //     std::cout << "clocks.isCurrentSource = " << clocks.isCurrentSource << std::endl;
        //     std::cout << "clocks.name = " << clocks.name << std::endl;
        //     std::cout << "numSources = " << numSources << std::endl;
        // }

        // ASIOSamples sPos;
        // ASIOTimeStamp tStamp;
        // if(ASIOGetSamplePosition(&sPos, &tStamp) == ASE_OK)
        // {
        //     std::cout << "sPos = " << sPos.lo + sPos.hi * 4294967296. << std::endl;
        //     std::cout << "tStamp = " << tStamp.lo + tStamp.hi * 4294967296. << std::endl;

        // }


        inBuffers.resize(inputChannels);
        outBuffers.resize(outputChannels);
        bufferInfos.resize(inputChannels + outputChannels);
        channelInfos.resize(inputChannels + outputChannels);

        for(int i = 0; i < inputChannels; ++i)
        {
            if(audioStreamProperties.activeInputChannels.test(i))
            {
                auto& bufferInfo = bufferInfos.at(i);
                bufferInfo.isInput = ASIOTrue;
                bufferInfo.channelNum = i;
                bufferInfo.buffers[0] = bufferInfo.buffers[1] = nullptr;
            }
        }

        for(int i = 0; i < outputChannels; ++i)
        {
            if(audioStreamProperties.activeOutputChannels.test(i))
            {
                auto& bufferInfo = bufferInfos.at(audioStreamProperties.activeInputChannels.count()  + i);
                bufferInfo.isInput = ASIOFalse;
                bufferInfo.channelNum = i;
                bufferInfo.buffers[0] = bufferInfo.buffers[1] = nullptr;
            }
        }

        // Get the input channel names
        for(int i = 0; i < inputChannels; ++i)
        {
            channelInfos.at(i).channel = i;
            channelInfos.at(i).isInput = ASIOTrue;

            if(ASIOGetChannelInfo(&channelInfos.at(i)) != ASE_OK)
            {
                std::cout << "Not able to get input channel info" << std::endl;
            }
            inputChannelNames.push_back(channelInfos.at(i).name);
        }

        // Get the output channel names
        for(int i = 0; i < outputChannels; ++i)
        {
            channelInfos.at(i).channel = i;
            channelInfos.at(i).isInput = ASIOFalse;

            if(ASIOGetChannelInfo(&channelInfos.at(i)) != ASE_OK)
            {
                std::cout << "Not able to get output channel info" << std::endl;
            }
            outputChannelNames.push_back(channelInfos.at(i).name);
        }

        // Get the sample type
        // ASIOSampleType sampleType = channelInfos[0].type;

        // switch (sampleType)
        // {
        // case ASIOSTInt16LSB:
        //     normalizeToFloat = []()
        //     break;
        // case ASIOSTInt24LSB:
        //     break;
        // case ASIOSTInt32LSB:
        //     break;
        // case ASIOSTFloat32LSB:
        //     break;
        // case ASIOSTFloat64LSB:
        //     break;
        // case ASIOSTInt16MSB: 
        //     break;
        // case ASIOSTInt24MSB:
        //     break;
        // case ASIOSTInt32MSB:
        //     break;
        // case ASIOSTFloat32MSB:
        //     break;
        // case ASIOSTFloat64MSB:
        //     break;
        // default:
        //     break;
        // }




        // init end
        auto totalBuffers = mAudioStreamProperties.activeInputChannels.count() + mAudioStreamProperties.activeOutputChannels.count();
        std::cout << "Total Buffers = " << totalBuffers << std::endl;
        scratchBuffer.resize(totalBuffers * preferredSize);

        auto result = ASIOCreateBuffers(bufferInfos.data(), totalBuffers, preferredSize, &callbacks);
        if(result == ASE_OK)
        {
            std::cout << "Create buffers worked" << std::endl; 
        }
        else
        {
            std::cout << "Something went wrong with creating buffers: " << result << std::endl;
        }

        int n{0};
        for(int i = 0; i < inputChannels; ++i)
        {
            if(mAudioStreamProperties.activeInputChannels.test(i))
            {
                inBuffers[n] = scratchBuffer.data() + (preferredSize * n); 
                ++n;
            }   
        }

        n = 0;
        for(int i = 0; i < outputChannels; ++i)
        {
            if(mAudioStreamProperties.activeOutputChannels.test(i))
            {
                outBuffers[n] = scratchBuffer.data() + (preferredSize * (mAudioStreamProperties.activeInputChannels.count() + n));
                ++n;
            }
        }

        return 0;
    }

    int closeStream()
    {
        if(ASIODisposeBuffers() == ASE_OK)
        {
            std::cout << "Successfully disposed ASIO buffers" << std::endl;
        }

        if(ASIOExit() == ASE_OK)
        {
            std::cout << "Successfully exit ASIO" << std::endl;
        }
        return 0;
    }
    int startStream()
    {
        if(ASIOStart() == ASE_OK)
        {
            std::cout << "ASIO Driver started successfully" << std::endl;
        }
        
        return 0;
    }
    int stopStream()
    {
        if(ASIOStop() == ASE_OK)
        {
            std::cout << "Audio stream successfully stopped" << std::endl;
        }
        return 0;
    }

    std::vector<std::string>& getInputChannelNames() { return inputChannelNames; }
    std::vector<std::string>& getOutputChannelNames() { return outputChannelNames; }
    std::vector<float*> inBuffers;
    std::vector<float*> outBuffers;
    std::vector<float> scratchBuffer;





    private:
    // Callbacks
    static void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess)
    {
        std::cout << "Callback process bufferSwitch" << std::endl;
    }

    static void sampleRateDidChange(ASIOSampleRate sRate)
    {

    }

    static long asioMessage(long selector, long value, void* message, double *opt)
    {
        int returnValue{0};
        switch (selector)
        {
        case kAsioSelectorSupported:
            if(value == kAsioResetRequest || value == kAsioEngineVersion || value == kAsioResetRequest
                || value == kAsioLatenciesChanged || value == kAsioSupportsInputMonitor || value == kAsioOverload) 
                returnValue = 1;
            break;
        case kAsioEngineVersion:
            std::cout << "AsioEngineVersion" << std::endl;
            returnValue = 2;
            break;
        case kAsioResetRequest:
            std::cout << "AsioResetRequest" << std::endl;
            returnValue = 1;
            break;
        case kAsioBufferSizeChange:
            std::cout << "AsioBufferSizeChange" << std::endl;
            break;
        case kAsioResyncRequest:
            std::cout << "AsioResyncRequest" << std::endl;
            break;
        case kAsioLatenciesChanged:
            std::cout << "AsioLatenciesChanged" << std::endl;
            break;
        case kAsioSupportsTimeInfo:
            std::cout << "AsioSupportsTimeInfo" << std::endl;
            returnValue = 1;
            break;
        case kAsioSupportsTimeCode:
            std::cout << "AsioSupportsTimeCode" << std::endl;
            break;
        case kAsioMMCCommand:
            std::cout << "AsioMMCCommand" << std::endl;
            break;
        case kAsioSupportsInputMonitor:
            std::cout << "AsioSupportsInputMonitor" << std::endl;
            break;
        case kAsioSupportsInputGain:
            std::cout << "AsioSupportsInputGain" << std::endl;
            break;
        case kAsioSupportsOutputGain:
            std::cout << "AsioSupportsOutputGain" << std::endl;
            break;
        case kAsioSupportsOutputMeter:
            std::cout << "AsioSupportsOutputMeter" << std::endl;
            break;
        case kAsioOverload:
            std::cout << "AsioOverload" << std::endl;
            break;
        
        
        default:
            break;
        }
        return returnValue;
    }

    static void  normalizeToFloat(const void* src, std::span<float> dest, int bitDepth)
    {
        double factor;
        switch (bitDepth)
        {
        case 16:
            factor = 32768.0;
            break;
        case 24:
            factor = 8388608.0;
            break;
        case 32:
            factor = 2147483648.0;
            break;
        default:
            break;
        }

        const double scale = 1.0 / 0x7FFFFFFF;  // Normalization factor
        const int32_t* srcSamples = static_cast<const int32_t*>(src);
    
        for (size_t i = 0; i < dest.size(); ++i) {
            // Directly access the int32_t value and scale it
            dest[i] = static_cast<float>(srcSamples[i] * scale);
        }
    }

    static void  convertToInt(std::span<float> src, void* dest, int bitDepth)
    {
        double factor;
        switch (bitDepth)
        {
        case 16:
            factor = 32768.0;
            break;
        case 24:
            factor = 8388608.0;
            break;
        case 32:
            factor = 2147483648.0;
            break;
        default:
            break;
        }

        const double maxVal = 0x7FFFFFFF;  // Maximum value for int32_t
        int32_t* destSamples = reinterpret_cast<int32_t*>(dest);
    
        for (int i = 0; i < src.size(); ++i) {
            // Scale the float value to the int32_t range and clamp it
            double scaledValue = std::clamp(src[i] * maxVal, -maxVal, maxVal);
    
            // Round to the nearest integer
            destSamples[i] = static_cast<int32_t>(std::round(scaledValue));
        }
    }

    static ASIOTime* bufferSwitchTimeInfo(ASIOTime *params, long doubleBufferIndex, ASIOBool directProcess)
    {
        auto bufferSize = audioDeviceAsioPtr->preferredSize;

        auto numActiveInputChannels = audioDeviceAsioPtr->mAudioStreamProperties.activeInputChannels.count();
        auto numActiveOutputChannels = audioDeviceAsioPtr->mAudioStreamProperties.activeOutputChannels.count();

        // Only read the first channel and assume all other channel has the same format

        for(int i = 0; i < numActiveInputChannels; ++i)
        {
            // convert to float (-1.0 and 1.0) somewhere here
            ASIOSampleType sampleType = audioDeviceAsioPtr->channelInfos[audioDeviceAsioPtr->bufferInfos[i].channelNum].type;
            std::span<float> dest(audioDeviceAsioPtr->inBuffers[i], bufferSize);
    
            int bitDepth;
            switch (sampleType)
            {
            case ASIOSTInt16LSB:
            case ASIOSTInt16MSB: 
                bitDepth = 16;
                // normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, 16);
                break;
            case ASIOSTInt24LSB:
            case ASIOSTInt24MSB:
                bitDepth = 24;
                // normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, 24);
                break;
            case ASIOSTInt32LSB:
            case ASIOSTInt32MSB:
                bitDepth = 32;
                // normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, 32);
                break;
            case ASIOSTFloat32LSB:
            case ASIOSTFloat32MSB:
                // If input format already is float, just copy the buffer
                // std::memcpy(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex], bufferSize * 4);
                break;
            case ASIOSTFloat64LSB:
            case ASIOSTFloat64MSB:
                // TODO: Handle 64-bit float input format
                break;
            default:
                break;
            }
            normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, bitDepth);

            // normalizeToFloat here

            // std::memcpy(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex], bufferSize * 4);
        }

        auto channels = std::max(numActiveInputChannels, numActiveOutputChannels);


        audioDeviceAsioPtr->process(audioDeviceAsioPtr->inBuffers, audioDeviceAsioPtr->outBuffers, channels, bufferSize);

        for(int i = 0; i < numActiveOutputChannels; ++i)
        {
            // convert back to int somewhere here
            ASIOSampleType sampleType = audioDeviceAsioPtr->channelInfos[audioDeviceAsioPtr->bufferInfos[numActiveInputChannels + i].channelNum].type;
            int bytesToCopy{0};
            std::span<float> src(audioDeviceAsioPtr->outBuffers[i], bufferSize);
    
            switch (sampleType)
            {
            case ASIOSTInt16LSB:
            case ASIOSTInt16MSB: 
                // normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, 16);
                break;
            case ASIOSTInt24LSB:
            case ASIOSTInt24MSB:
                // normalizeToFloat(static_cast<std::byte*>(audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex]), dest, 24);
                break;
            case ASIOSTInt32LSB:
            case ASIOSTInt32MSB:
                // std::memcpy(audioDeviceAsioPtr->bufferInfos[numActiveInputChannels + i].buffers[doubleBufferIndex], audioDeviceAsioPtr->inBuffers[i], bufferSize * 4);
                // JUCEconvertFromFloat(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[numActiveInputChannels + i].buffers[doubleBufferIndex], bufferSize);

                convertToInt(src, audioDeviceAsioPtr->bufferInfos[numActiveInputChannels + i].buffers[doubleBufferIndex], 32);
                // std::memcpy(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex], bufferSize * 4);
                break;
            case ASIOSTFloat32LSB:
            case ASIOSTFloat32MSB:
                // If input format already is float, just copy the buffer
                // std::memcpy(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex], bufferSize * 4);
                break;
            case ASIOSTFloat64LSB:
            case ASIOSTFloat64MSB:
                // TODO: Handle 64-bit float input format
                break;
            default:
                break;
            }
            // normalizeToFloat here

            // std::memcpy(audioDeviceAsioPtr->inBuffers[i], audioDeviceAsioPtr->bufferInfos[i].buffers[doubleBufferIndex], bufferSize * 4);


            // std::memcpy(audioDeviceAsioPtr->bufferInfos[numActiveInputChannels + i].buffers[doubleBufferIndex], audioDeviceAsioPtr->outBuffers[i], bufferSize * 4);
        }

        if(audioDeviceAsioPtr->postOutput)
        {
            ASIOOutputReady();
        }

        return 0;
    }

    void process(std::vector<float*> in, std::vector<float*> out, int numChannels, int bufferSize)
    {
        for(int i = 0; i < numChannels; ++i)
        {
            // float sample = in.at(i)[0];
            // float test = -1.0;

            // memcpy(out[i], &test, sizeof(float));
            memcpy(out[i], in[0], bufferSize * 4);
        }
        // std::memcpy(out[0], in[0], size);
    }

    int mActiveInputChannel;
    int mActiveOutputChannel;
    std::vector<std::string> inputChannelNames;
    std::vector<std::string> outputChannelNames;
    AudioStreamProperties mAudioStreamProperties;
    
    // ASIOInit()
    ASIODriverInfo driverInfo;
    // ASIOGetChannels()
    long           inputChannels;
    long           outputChannels;
    // ASIOGetBufferSize()
    long           minSize;
    long           maxSize;
    long           preferredSize;
    long           granularity;
    // ASIOGetSampleRate()
    ASIOSampleRate sampleRate;
    // ASIOOutputReady()
    bool           postOutput;
    // ASIOGetLatencies ()
    long           inputLatency;
    long           outputLatency;
    // ASIOCreateBuffers ()
    long inputBuffers;	// becomes number of actual created input buffers
    long outputBuffers;	// becomes number of actual created output buffers
    std::vector<ASIOBufferInfo> bufferInfos; // buffer info's
    // ASIOGetChannelInfo()
    std::vector<ASIOChannelInfo> channelInfos; // channel info's
    // The above two arrays share the same indexing, as the data in them are linked together
    // Information from ASIOGetSamplePosition()
    // data is converted to double floats for easier use, however 64 bit integer can be used, too
    double         nanoSeconds;
    double         samples;
    double         tcSamples;	// time code samples
    // bufferSwitchTimeInfo()
    ASIOTime       tInfo;       // time info state
    unsigned long  sysRefTime;      // system reference time, when bufferSwitch() was called
    // Signal the end of processing in this example
    bool           stopped;
    ASIOCallbacks callbacks;
};

int main() {

    int32_t maxInt = std::numeric_limits<int32_t>::max();
    float a = static_cast<float>(std::numeric_limits<int32_t>::min()) / static_cast<float>(maxInt);

    


    std::cout << "maxInt = " << maxInt << std::endl;
    std::cout << "a = " << a << std::endl;


    // return 0;
    std::vector<std::string> names(16, std::string(32, '\0'));
    std::vector<char*> namePtrs;
    int numDrivers;

    for(auto &name : names)
    {
        namePtrs.push_back(&name[0]);
    }

    {
        // Needs an ASIO driver instance to be able to query the available drivers
        AsioDrivers tmpDriver; 
        numDrivers = tmpDriver.getDriverNames(namePtrs.data(), 16);
    }

    for(int i = 0; i < numDrivers; ++i)
    {
        std::cout << i + 1 << ".\t" << names[i] << std::endl;
    }

    int chooseDriver;
    std::cout << "Choose driver: ";
    std::cin >> chooseDriver;

    auto audioStreamProperties = AudioDeviceAsio::probeDriver(names[chooseDriver - 1]);

    std::cout << "==================== INPUT CHANNELS ====================" << std::endl;
    for(int i = 0; i < audioStreamProperties.inputChannelNames.size(); ++i)
    {
        std::cout << i + 1 << ": " << audioStreamProperties.inputChannelNames.at(i) << std::endl;
    }

    std::cout << "==================== OUTPUT CHANNELS ====================" << std::endl;
    for(int i = 0; i < audioStreamProperties.outputChannelNames.size(); ++i)
    {
        std::cout << i + 1 << ": " << audioStreamProperties.outputChannelNames.at(i) << std::endl;
    }


    std::cout << "Select active input channels: " << std::endl;
    
    int numInput;
    while(std::cin >> numInput)
    {
        try
        {
            audioStreamProperties.activeInputChannels.set(numInput - 1);
        }
        catch(const std::exception& e)
        {
            std::cerr << "ERROR: Not possible to activate given channels"  << std::endl;;

        }
        
        if (std::cin.peek() == '\n') // Stop when Enter is pressed
            break;
    }

    std::cout << "Select active output channels: " << std::endl;
    int numOutput;
    while(std::cin >> numOutput)
    {
        try
        {
            audioStreamProperties.activeOutputChannels.set(numOutput - 1);
        }
        catch(const std::exception& e)
        {
            std::cerr << "ERROR: Not possible to activate given channels"  << std::endl;;
        }
        
        if (std::cin.peek() == '\n') // Stop when Enter is pressed
            break;
    }

    std::cout << "Active input channels: " << audioStreamProperties.activeInputChannels << std::endl;
    std::cout << "Active output channels: " << audioStreamProperties.activeOutputChannels << std::endl;


    auto device = std::make_shared<AudioDeviceAsio>();
    device->openStream(audioStreamProperties);
    device->startStream();

    while(1)
    {

    }

    device->stopStream();
    device->closeStream();

    return 0;
}
