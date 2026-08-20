# LibSoundboard

A minimal Unity Il2CPP Soundboard library. Made for the public, without the slop.
---

## Why
Other similar projects felt bloated, and filled with AI slop. This exists to just do its job.

## Dependencies
- **BNMResolve** - For access to Unity APIs
- Able to build a .so
- Prior C++ knowledge

## Format Support
Currently only **WAV** files are supported. More support may be coming in the future!
WAV data can be directly embedded as a `std::vector<uint8_t>`, read from a file, or any other method of storing, as long as you convert it to a `std::vector<uint8_t>` its good!

## Demo
```cpp
  std::string clipName = "Test"
  std::vector<uint8_t> sound = { 0x52, 0x49, 0x46, 0x46, 0x46, ... }
  auto audioClipLoader = AudioClipLoader(sound);
    auto* clip = audioClipLoader.CreateAudioClip(sound, clipName);
    if (!clip) {
        return;
    }
    // play clip
    // Note: you have to obtain a valid recorder.
    recorder->SetSourceType(Recorder::InputSourceType::AudioClipSource);
    recorder->SetAudioClip(clip);
    recorder->SetLoopAudioClip(true);
    recorder->SetDebugEcho(true);
    recorder->RestartRecording();
```

## Support
If you have an issue with the library code itself, open a Github issue, otherwise I will **NOT** help.
