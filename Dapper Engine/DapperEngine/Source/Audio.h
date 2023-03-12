//------------------------------------------------------------------------------
//
// File Name:	Audio.h
// Author(s):	Gavin Lu
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "fmod_studio.hpp"         // Sound
#include <unordered_map>
#include "ISystem.h"
#include <string>
#include "rapidjson/document.h"
#include "Engine.h"
#include "IJsonWriter.h"
#include "Trace.h"
#include <string_view>
#include <memory>
#include <glm/vec2.hpp>

// Dapper Namespace!
namespace Dapper{

  class Listener {

  public:

    Listener(FMOD::Studio::System* s);

    void Update(glm::vec2 pos, glm::vec2 vel);

  private:
    FMOD_3D_ATTRIBUTES attributes = { {0} };
    FMOD::Studio::System* studio_system;
    // Will need internal id if multiple listeners

  };

  class Sound {
  public:

    Sound(FMOD::Studio::EventInstance* e);

    ~Sound();

    void Play();
    void SetVolume(float vol);
    void SetPaused(bool paused);
    void SetPitch(float pitch);
    void Stop();
    float GetVolume();
    bool GetPaused();
    float GetPitch();
    void Update(glm::vec2 pos, glm::vec2 vel);
    void SetParameter(const char* name, float value);
    float GetParameter(const char* name);

  private:

    FMOD::Studio::EventInstance* event;
    FMOD_3D_ATTRIBUTES attributes = {{0}};

  };

  class Bus {

  public:

    Bus();

    Bus(FMOD::Studio::Bus* b);

    void StopAll();
    void SetPaused(bool paused);
    void SetVolume(float vol);
    void SetMute(bool muted);
    void SetPitch(float pitch);
    float GetVolume();
    bool GetPaused();
    float GetPitch();
    bool GetMute();

  private:
    FMOD::Studio::Bus* bus;
    FMOD::ChannelGroup* channelgroup = nullptr;

  };

  class Audio : public ISystem
  {

  public:
    Audio(Engine& engine);
    ~Audio() override;

    const std::string& Name()const override;

    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    void Shutdown();
    FMOD::Sound* Load(const char* filePath, bool isMusic);
    void Play(FMOD::Sound* sound, std::string name);
    void SetVolume(std::string name, float volume);
    inline void GetVolume(std::string name, float* volume);
    inline void SetPitch(std::string name, float pitch);
    inline void GetPitch(std::string name, float* pitch);
    void ToggleMute(std::string name);
    void TogglePause(std::string name);
    void Stop(std::string name);
    void WriteAllAudioObjects(IJsonWriter& writer);
    inline FMOD::Sound* GetSound(std::string name);
    Bus* GetBus(std::string name);

    std::shared_ptr<Sound> GetSound(const std::string_view event_name) const;

    inline FMOD::ChannelGroup* GetChannel(std::string name);

    // FMOD::ChannelGroup* GetChannel1()const; Old Get Function

  private:

    Engine& my_engine;

    FMOD::System* core_system = nullptr;
    FMOD::Studio::System* studio_system = nullptr;

    const std::string name = "Audio";
    FMOD::Channel* channel1 = nullptr;

    std::unordered_map<std::string, FMOD::Sound*> sound_list;
    std::unordered_map<std::string, FMOD::ChannelGroup*> channel_list;

    std::unordered_map<std::string, FMOD::Studio::EventDescription*> event_list;
    std::unordered_map<std::string, FMOD::Studio::Bank*> bank_list;
    std::unordered_map<std::string, Bus> bus_list;

    // std::unordered_map<std::string, FMOD::ChannelGroup*> group_list;

    // Inherited via ISystem
    virtual bool UpdateWhilePaused()const override;

    void LoadGUIDFile();

  };

  std::pair<std::string_view, int> AudioDeserialize(Engine& engine, const rapidjson::Value& val);
  std::pair<std::string_view, int> AudioArrayDeserialize(Engine& engine, const rapidjson::Value& val);

} // End Of Dapper Namespace
