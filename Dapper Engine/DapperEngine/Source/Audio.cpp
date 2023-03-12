//------------------------------------------------------------------------------
//
// File Name:	Audio.cpp
// Author(s):	Gavin Lu
// 
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

// Disables FMOD File warning
#pragma warning(disable : 26812)

#include "Audio.h"
#include "fmod_errors.h"
#include <iostream> // cout
#include "Utility.h"

// FMOD_ERRCHECK checks the return of a given fmod function, which will tell us if something went wrong.

#define LIVEUPDATE 0

#define FMOD_ERRCHECK(function) \
result = function;\
if (result != FMOD_OK) \
{ \
 \
  std::cout << "FMOD Error: " << FMOD_ErrorString(result) << "\n"; \
}
// Dapper Namespace!
namespace Dapper {

  static FMOD_RESULT result;

  /*--------------------------------------
  Description: Constructor
  Return: Void
  ---------------------------------------*/
  Audio::Audio(Engine& engine) :
    my_engine(engine)
  {
  }
  /*--------------------------------------
  Description: Temporary Deconstructor
  Return: Void
  ---------------------------------------*/
  Audio::~Audio()
  {
  }

  /*--------------------------------------
  Description: Initializes FMOD system for the class.
  Return: Void
  ---------------------------------------*/
  void Audio::Initialize()
  {

    // Inits system
    FMOD_ERRCHECK(FMOD::Studio::System::create(&studio_system));
    FMOD_ERRCHECK(studio_system->getCoreSystem(&core_system));
    FMOD_ERRCHECK(core_system->setSoftwareFormat(0, FMOD_SPEAKERMODE_STEREO, 0));

    // When you submit, turn live update off IMPORTANT IMPORTANT IMPORTANT

#if LIVEUPDATE

    FMOD_ERRCHECK(studio_system->initialize(1024, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));

#else

    FMOD_ERRCHECK(studio_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

#endif
    Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(my_engine, "Deserializer");
    deserial.RegisterFunction("Audio", AudioDeserialize);
    deserial.RegisterFunction("Audios", AudioArrayDeserialize);

    LoadGUIDFile();

  }

  

  /*--------------------------------------
  Description: Empty Render function so I don't get yelled at by ISystems!
  Return: Void
  ---------------------------------------*/
  void Audio::Render()
  {
    // I don't have a render :(
  }

  /*--------------------------------------
  Description: Updates the FMOD System!
  Params: float dt ¡V Time derivative!
  Return: Void
  ---------------------------------------*/
  void Audio::Update(float dt)
  {
    // Unused param dt
    dt;

    // Update Audio and ERRCHECKS
    FMOD_ERRCHECK(studio_system->update());

  }

  /*--------------------------------------
  Description: Shuts down all sounds and the audio system itself
  Return: Void
  ---------------------------------------*/
  void Audio::Shutdown()
  {

    // If soundList is not empty
    if (!sound_list.empty())
    {

      // Range based for loop, it goes through every pair in soundList and releases it.
      for (auto pair : sound_list)
      {

        FMOD::Sound* sound = pair.second;
        FMOD_ERRCHECK(sound->release());

      }

    }

    // Shuts down the System. Will release an error if we never tried init-ing the audio system
    // I don't think that will be an issue though.
    FMOD_ERRCHECK(studio_system->release());

  }

  /*--------------------------------------
  Description: Loads audio based on filebath and if it's music, it'll be created differently.
  Params:
    const char* filePath ¡V audio filePath
    bool isMusic - bool for if the audio is music or not.
  Return: FMOD::Sound* - Sound pointer for storage.
  ---------------------------------------*/

  FMOD::Sound* Audio::Load(const char* filePath, bool isMusic)
  {
    FMOD::Sound* sound; // Sound for sound storage
    FMOD::ChannelGroup* channelgroup1 = nullptr;
    
    if (GetSound(filePath) != nullptr)
      return nullptr;

    if (isMusic)
    {
      // If music
      FMOD_ERRCHECK(core_system->createStream(filePath, FMOD_LOOP_NORMAL, 0, &sound));
    }
    else {
      // If not music
      FMOD_ERRCHECK(core_system->createSound(filePath, FMOD_DEFAULT, 0, &sound));
    }

    FMOD_ERRCHECK(core_system->createChannelGroup(filePath, &channelgroup1));

    // Inserts it into the unordered map
    // Perhaps an idea for having the channel ID as the pair?

    sound_list.insert({ filePath , sound });
    channel_list.insert({ filePath, channelgroup1 });

    // Returns Sound*, though we are storing sound in the map..maybe we don't need to return anything?
    return sound;

  }

  /*--------------------------------------
  Description: Returns Class name for identity purposes
  Return: std::string - class name, aka Audio
  ---------------------------------------*/
  const std::string& Audio::Name() const
  {
    return name;
  }

  /*--------------------------------------
  Description: Plays audio depending on param
  Params:
    FMOD::Sound* sound ¡V sound we are playing.
    FMOD::Channel* channel - channel we are playing in.
  Return: Void
  ---------------------------------------*/
  void Audio::Play(FMOD::Sound* sound, std::string name)
  {

    bool isPlaying;     // Result for isPlaying

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores isPlaying via pointer...for some reason.
    // Invalid Object handle used occurrs the line directly below me.
    FMOD_ERRCHECK(channel->isPlaying(&isPlaying));

    if (!isPlaying)
    {

      // Plays sound
      FMOD_ERRCHECK(core_system->playSound(sound, channel, false, &channel1));

    }

  }

  /*--------------------------------------
  Description: Sets volume for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are setting volume for.
    float volume - volume we are setting it to.
  Return: Void
  ---------------------------------------*/
  void Audio::SetVolume(std::string name, float Volume)
  {

    FMOD::ChannelGroup* channel = GetChannel(name);

    FMOD_ERRCHECK(channel->setVolume(Volume));

  }

  /*--------------------------------------
  Description: Gets volume for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are getting volume for.
    float* volume - volume we are storing it in.
  Return: Void
  ---------------------------------------*/
  void Audio::GetVolume(std::string name, float* volume)
  {

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores volume via pointer...for some reason.
    FMOD_ERRCHECK(channel->getVolume(volume));

  }

  /*--------------------------------------
  Description: Sets pitch for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are setting pitch for.
    float pitch - pitch we are setting it to.
  Return: Void
  ---------------------------------------*/
  void Audio::SetPitch(std::string name, float pitch)
  {

    FMOD::ChannelGroup* channel = GetChannel(name);

    FMOD_ERRCHECK(channel->setPitch(pitch));

  }

  /*--------------------------------------
  Description: Gets pitch for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are getting pitch for.
    float* volume - pitch we are storing it in.
  Return: Void
  ---------------------------------------*/
  void Audio::GetPitch(std::string name, float* pitch)
  {

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores pitch via pointer...for some reason.
    FMOD_ERRCHECK(channel->getPitch(pitch));

  }

  /*--------------------------------------
  Description: Toggles Mute for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are toggling mute in.
  Return: Void
  ---------------------------------------*/
  void Audio::ToggleMute(std::string name)
  {

    bool muted;         // Result for getMute

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores mute via pointer...for some reason.
    FMOD_ERRCHECK(channel->getMute(&muted));

    if (muted)
    {

      FMOD_ERRCHECK(channel->setMute(false));
      return;

    }

    FMOD_ERRCHECK(channel->setMute(true));

  }

  /*--------------------------------------
  Description: Toggles pause for a channel
  Params:
    FMOD::Channel* channel ¡V The channel we are toggling pause in.
  Return: Void
  ---------------------------------------*/
  void Audio::TogglePause(std::string name)
  {

    bool paused;        // Result for getPaused

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores pause via pointer...for some reason.
    FMOD_ERRCHECK(channel->getPaused(&paused));

    if (paused)
    {

      FMOD_ERRCHECK(channel->setPaused(false));
      return;

    }

    FMOD_ERRCHECK(channel->setPaused(true));

  }

  /*--------------------------------------
  Description: Stops audio depending on param
  Params:
    FMOD::Channel* channel - channel we are stopping the sound in.
  Return: Void
  ---------------------------------------*/
  void Audio::Stop(std::string name)
  {

    bool isPlaying;     // Result for isPlaying

    FMOD::ChannelGroup* channel = GetChannel(name);

    // Stores playing via pointer...for some reason.
    FMOD_ERRCHECK(channel->isPlaying(&isPlaying));

    if (isPlaying)
    {

      FMOD_ERRCHECK(channel->stop());

    }

  }

  /*--------------------------------------
  Description: Gets sound through sound path name
  Params:
    std::string name - Filepath of the sound you're trying to get.
  Return: Pointer to sound. Will return null if it doesn't exist.
  ---------------------------------------*/
  FMOD::Sound* Audio::GetSound(std::string name)
  {
    auto iter = sound_list.find(name);
    if (iter != sound_list.end())
    {
      return iter->second;
    }
    return nullptr;
  }

  Bus* Audio::GetBus(std::string name)
  {
     return &bus_list.find(name)->second;
  }

  std::shared_ptr<Sound> Audio::GetSound(const std::string_view event_name) const
  {
    const std::string event = event_name.data();

    auto e = event_list.find(event);

    if (e != event_list.end())
    {
      FMOD::Studio::EventInstance* temp;
      e->second->createInstance(&temp);
      return std::make_shared<Sound>(temp);

    }

    return nullptr;

  }

  /*--------------------------------------
  Description: Gets ChannelGroup through sound path name
  Params:
    std::string name - Filepath of the sound you're trying to get.
  Return: Pointer to sound. Will return null if it doesn't exist.
  ---------------------------------------*/
  FMOD::ChannelGroup* Audio::GetChannel(std::string name)
  {
    auto iter = channel_list.find(name);
    if (iter != channel_list.end())
    {
      return iter->second;
    }
    return nullptr;
  }

  /*--------------------------------------
  Description: Whether if this should be updating while paused
  Return: Always true
  ---------------------------------------*/
  bool Audio::UpdateWhilePaused()const
  {
    return true;
  }

  void Audio::LoadGUIDFile()
  {

    std::string file_line;

    std::ifstream stream("./Assets/FMOD Banks/GUIDs.txt");

    size_t type_index;
    size_t name_index;

    std::string name;
    
    if (stream)
    {

      while (!stream.eof())
      {

        std::getline(stream, file_line);

        // Find
        type_index = file_line.find("} ");

        // Check if legal
        if (type_index == std::string::npos)
          continue;

        // Move 2 forwards
        type_index += 2;

        // Read until you reach semi colon

        name_index = file_line.find(":", type_index);

        // Check if legal
        assert(name_index != std::string::npos);

        // Hold name
        name = file_line.substr(type_index, name_index - type_index);

        // Check

        if (name == "event")
        {

          name = file_line.substr(type_index);
          FMOD::Studio::EventDescription* temp;
          FMOD_ERRCHECK(studio_system->getEvent(name.c_str(), &temp));
          event_list[file_line.substr(name_index + 1)] = temp;

        }
        else if (name == "bus")
        {

          name = file_line.substr(type_index);
          FMOD::Studio::Bus* temp;
          FMOD_ERRCHECK(studio_system->getBus(name.c_str(), &temp));
          bus_list[file_line.substr(name_index + 1)] = temp;

        }
        else if (name == "bank")
        {
          
          name = "./Assets/FMOD Banks/Desktop" + file_line.substr(name_index + 1) + ".bank";
          FMOD::Studio::Bank* temp;
          FMOD_ERRCHECK(studio_system->loadBankFile(name.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &temp));
          bank_list[file_line.substr(name_index + 1)] = temp;

        }
        else if (name == "vca")
        {

          // Don't know how to do dis yet

        }
        else if (name == "parameter")
        {

          // Don't know how to do dis yet

        }
      }

    }

  }

  /*--------------------------------------
  Description: Writing audio objects in json file
  Params:
    IJsonWriter& writer - Writer for writing
  Return: void
  ---------------------------------------*/
  void Audio::WriteAllAudioObjects(IJsonWriter& writer)
  {

    if (sound_list.size() > 1)
    {
      writer.WriteKey("Audios");
      writer.StartArray();
      // Loop through here, and then write, you don't need another for loop in the write function, as there are no components in the game object.
      for (auto i : sound_list)
      {
        writer.StartObject();
        writer.WriteString("Filepath", i.first.c_str());
        writer.WriteBool("Music", i.second);
        writer.EndObject();
      }
      writer.EndArray();
    }
  }

  /*--------------------------------------
  Description: Deserializing one audio
  Params:
    Engine& engine - engine for getting audio system.
    rapidjson::Value& val - value gotten from json
  Return: pair of name for writing and -1.
  ---------------------------------------*/
  std::pair<std::string_view, int> AudioDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
    auto json_obj = val.GetObject();
    std::string name = json_obj["Filepath"].GetString();
    bool ismus = json_obj["Music"].GetBool();
    audio.Load(name.c_str(), ismus);
    return{ name, -1 };
  }

  /*--------------------------------------
  Description: Deserializing an array of audios.
  Params:
    Engine& engine - engine for getting audio system.
    rapidjson::Value& val - value gotten from json
  Return: pair of name + s for writing and -1.
  ---------------------------------------*/
  std::pair<std::string_view, int> AudioArrayDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
    auto obj_array = val.GetArray();
    int len = obj_array.Size();
    for (int i = 0; i < len; ++i)
    {
      auto json_obj = obj_array[i].GetObj();
      std::string name = json_obj["Filepath"].GetString();
      bool ismus = json_obj["Music"].GetBool();
      audio.Load(name.c_str(), ismus);
    }
    return { "Audios", -1 };

  }

  // Sound things

  Sound::Sound(FMOD::Studio::EventInstance* e) : event(e)
  {
  }

  Sound::~Sound()
  {

    event->release();

  }

  void Sound::Play()
  {

    FMOD_ERRCHECK(event->start());

  }

  void Sound::SetVolume(float vol)
  {
    FMOD_ERRCHECK(event->setVolume(vol));

  }

  void Sound::SetPaused(bool paused)
  {
    FMOD_ERRCHECK(event->setPaused(paused));
  }

  void Sound::SetPitch(float pitch)
  {
    FMOD_ERRCHECK(event->setPitch(pitch));
  }

  void Sound::Stop()
  {
    FMOD_ERRCHECK(event->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
  }

  float Sound::GetVolume()
  {
    float vol;
    event->getVolume(&vol);
    return vol;
  }

  bool Sound::GetPaused()
  {
    bool temp;
    event->getPaused(&temp);
    return temp;
  }

  float Sound::GetPitch()
  {
    float pitch;
    event->getPitch(&pitch);
    return pitch;
  }

  void Sound::Update(glm::vec2 pos, glm::vec2 vel)
  {

    attributes.position = FMOD_VECTOR(pos.x, pos.y, 0);
    attributes.velocity = FMOD_VECTOR(vel.x, vel.y, 0);

  }

  void Sound::SetParameter(const char* name, float value)
  {

    event->setParameterByName(name, value);

  }

  float Sound::GetParameter(const char* name)
  {
    float value;
    event->getParameterByName(name, &value);
    return value;
  }

  Bus::Bus()
  {
  }

  Bus::Bus(FMOD::Studio::Bus* b) : bus(b)
  {
  }

  void Bus::StopAll()
  {

    FMOD_ERRCHECK(bus->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT));

  }

  void Bus::SetPaused(bool paused)
  {

    FMOD_ERRCHECK(bus->setPaused(paused));

  }

  void Bus::SetVolume(float vol)
  {

    FMOD_ERRCHECK(bus->setVolume(vol));

  }

  void Bus::SetMute(bool muted)
  {
    FMOD_ERRCHECK(bus->setMute(muted));

  }

  void Bus::SetPitch(float pitch)
  {

    FMOD_ERRCHECK(bus->getChannelGroup(&channelgroup));
    FMOD_ERRCHECK(channelgroup->setPitch(pitch));

  }

  float Bus::GetVolume()
  {

    float vol;
    FMOD_ERRCHECK(bus->getVolume(&vol));
    return vol;

  }

  bool Bus::GetPaused()
  {

    bool paused;
    FMOD_ERRCHECK(bus->getPaused(&paused));
    return paused;

  }

  float Bus::GetPitch()
  {

    FMOD_ERRCHECK(bus->getChannelGroup(&channelgroup));
    float pitch;
    FMOD_ERRCHECK(channelgroup->getPitch(&pitch));
    return pitch;

  }

  bool Bus::GetMute()
  {
    bool muted;
    FMOD_ERRCHECK(bus->getMute(&muted));
    return muted;
  }


  Listener::Listener(FMOD::Studio::System* s) : studio_system(s)
  {
  }

  void Listener::Update(glm::vec2 pos, glm::vec2 vel)
  {

    attributes.position = FMOD_VECTOR(pos.x, pos.y, 0);
    attributes.velocity = FMOD_VECTOR(vel.x, vel.y, 0);
    studio_system->setListenerAttributes(0, &attributes);

  }

} // End Of Dapper Namespace

