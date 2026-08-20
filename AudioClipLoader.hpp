#pragma once
#include <BNMResolve.hpp>
#include <string>
#include <vector>

// more info on WAV files at: https://en.wikipedia.org/wiki/WAV, essential for making a project like this.
class AudioClipLoader{
public:
    AudioClipLoader(const std::vector<uint8_t>& wavBytes);

    AudioClip* CreateAudioClip(const std::vector<uint8_t>& wavBytes, const std::string& clipName) const;

    int GetSamplingRate() const;
    int GetChannels() const;
    int GetBitsPerSample() const;
    int GetBytesPerSample() const;
private:
    int Channels = 0;
    int SamplingRate = 0;
    int BitsPerSample = 0;
    int BytesPerSample = 0; // bits / 8
    static constexpr int DataOffset = 44; // header is 44 bytes so data starts after the first 44 bytes
    static constexpr int HeaderSize = 44; // also 44, just readability sake.
    int ToInt16(const std::vector<uint8_t>& bytes, const int& start) const;
    int ToInt32(const std::vector<uint8_t>& bytes, const int& start) const;
    Array<float>* ConvertPCMToFloat(const std::vector<uint8_t>& PCM, const int& sampleCount) const;
};
