#include "AudioClipLoader.hpp"

/*
 * [riff]
 * FileTypeBlocId = 0 - 4
 * FileSize = 4 -8
 * FileFormatId = 8 - 12
 * [fmt]
 * FormatBlocID = 12 - 16
 * BlocSize = 16 - 20,
 * AudioFormat = 20 - 22,
 * NbrChannels = 22 - 24
 * freq / samp = 24 - 28
 * BytePerSec = 28 - 32
 * BytePerBloc = 32 - 34,
 * BitsPerSample = 34 - 36
 * [data]
 * DataBlocId = 36 - 40,
 * DataSize = 40 - 44,
 * SampledData = 44 + datasize
 */
AudioClipLoader::AudioClipLoader(const std::vector<uint8_t>& wavBytes) {
    if (wavBytes.size() <= HeaderSize) // header is 44 bytes
        return;

    int channelStartByte = 22;
    int sampleRateStartByte = 24;
    int bitsPerSampleStartByte = 34;
    this->Channels = ToInt16(wavBytes, channelStartByte);
    this->SamplingRate = ToInt32(wavBytes, sampleRateStartByte);
    this->BitsPerSample = ToInt16(wavBytes, bitsPerSampleStartByte);
    this->BytesPerSample = BitsPerSample / 8;
}

int AudioClipLoader::ToInt16(const std::vector<uint8_t>& bytes, const int& start) const
{
    int16_t value;
    std::memcpy(&value, bytes.data() + start, sizeof(int16_t));
    return value;
}

int AudioClipLoader::ToInt32(const std::vector<uint8_t>& bytes, const int& start) const
{
    int32_t value;
    std::memcpy(&value, bytes.data() + start, sizeof(int32_t));
    return value;
}

int AudioClipLoader::GetSamplingRate() const {
    return this->SamplingRate;
}

int AudioClipLoader::GetBitsPerSample() const {
    return this->BitsPerSample;
}

int AudioClipLoader::GetBytesPerSample() const {
    return this->BytesPerSample;
}

int AudioClipLoader::GetChannels() const {
    return this-> Channels;
}

AudioClip* AudioClipLoader::CreateAudioClip(const std::vector<uint8_t>& wavBytes, const std::string& clipName) const {
    if (this->GetChannels() <= 0 || this->GetSamplingRate() <= 0 || this->GetBitsPerSample() <= 0 || this->GetBytesPerSample() <= 0 || wavBytes <= DataOffset)
        return nullptr;

    int dataSize = wavBytes.size() - DataOffset;
    int totalSamples = dataSize / this->GetBytesPerSample();
    int sampleCount = totalSamples / this->GetChannels();
    if (sampleCount <= 0)
        return nullptr;

    Array<float>* samples = ConvertPCMToFloat(wavBytes, sampleCount);
    if (!samples)
        return nullptr;

    AudioClip* audioClip = AudioClip::Create(clipName, sampleCount, this->GetChannels(), this->GetSamplingRate(), false);
    audioClip->SetData(samples, 0);
    return audioClip;
}

Array<float>* AudioClipLoader::ConvertPCMToFloat(const std::vector<uint8_t>& PCM, const int& sampleCount) const {
    Array<float>* samples = Array<float>::Create(sampleCount * this->GetChannels());
    int bitsPerSample = this->GetBitsPerSample();

    // unity requires -1.0 -> 1.0 float range. pcm is int, following is converting.
    if (bitsPerSample == 8) {
        for (int i = 0; i < sampleCount * this->GetChannels(); i++) {
            int byteIndex = DataOffset + i;
            if (byteIndex >= (int)PCM.size()) {
                (*samples)[i] = 0.0f;
                continue;
            }
            (*samples)[i] = std::clamp((PCM[byteIndex] - 128.f) / 128.0f, -1.0f, 1.0f);
        }
    }
    else if (bitsPerSample == 16){
        for (int i = 0; i < sampleCount * this->GetChannels(); i++) {
            int byteIndex = DataOffset + i * 2;
            if (byteIndex + 1 >= (int)PCM.size()) {
                (*samples)[i] = 0.0f;
                continue;
            }

            int16_t raw;
            std::memcpy(&raw, PCM.data() + byteIndex, sizeof(int16_t));
            (*samples)[i] = std::clamp(raw / 32768.0f, -1.0f, 1.0f);
        }
    }
    else
        return nullptr;

    return samples;
}
