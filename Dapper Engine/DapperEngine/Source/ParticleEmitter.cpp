//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitter.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ParticleEmitter.h"
#include "Utility.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "TextureManager.h"
#include "glm/vec3.hpp"
#include "imgui.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/common.hpp>

// Dapper Namespace!
namespace Dapper
{
  ParticleEmitter::ParticleEmitter(Engine* engine) : theEngine(engine), parentID(-1),
    paused(false)
  {
  }

  ParticleEmitter::~ParticleEmitter()
  {
  }

  void ParticleEmitter::Init()
  {
    decrementTimer = info.timer;
  }

  void ParticleEmitter::Update(float dt)
  {
    for (unsigned int i = 0; i < burst_particles.size(); ++i)
    {
      Particle& p = burst_particles[i];
      p.Life -= dt; // reduce life
      if (p.Life > 0.0f)
      {	// particle is alive, thus update
        p.Velocity = glm::vec2(p.Velocity.x, p.Velocity.y + (burst_info.gravity_value * dt));
        p.Position -= glm::vec3(p.Velocity.x * dt, p.Velocity.y * dt, p.Position.z);
        if (burst_info.wavey)
        {
          float last_frame = sinf(p.sin_wave * 20);
          p.sin_wave += dt;
          p.Position.x += sinf(p.sin_wave * 20) - last_frame;
          if (p.sin_wave > 100)
          {
            p.sin_wave = 0;
          }
        }
        p.Color.a -= dt * fadeRate; //Fade
      }
      else
      {
        std::swap(p, burst_particles.back());
        burst_particles.pop_back();
      }
    }
    //info.wavey = true;
    //info.gravity_value = -75;
    for (unsigned int i = 0; i < trail_particles.size(); ++i)
    {
      Particle& p = trail_particles[i];
      p.Life -= dt; // reduce life
      if (p.Life > 0.0f)
      {	// particle is alive, thus update
        p.Velocity = glm::vec2(p.Velocity.x, p.Velocity.y + (info.gravity_value * dt));
        p.Position -= glm::vec3(p.Velocity.x * dt, p.Velocity.y * dt, p.Position.z);
        if (info.wavey)
        {
          float last_frame = sinf(p.sin_wave * 6);
          p.sin_wave += dt;
          p.Position.x += (sinf(p.sin_wave * 6) - last_frame) * 5;
          if (p.sin_wave > 100)
          {
            p.sin_wave = 0;
          }
        }
        p.Color.a -= dt * fadeRate; //Fade
      }
      else
      {
        std::swap(p, trail_particles.back());
        trail_particles.pop_back();
      }
    }
    decrementTimer -= dt;

    if (info.burst == false)
    {
      if (decrementTimer < 0)
      {
        Particle newBoy;
        makeParticle(&newBoy);
        trail_particles.push_back(newBoy);
        decrementTimer = info.timer;
      }
    }
  }

  void ParticleEmitter::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;

    auto daObject = jsonVal.GetObject();
    const auto& texure_obj = daObject["Texture"];
    if (texure_obj.IsArray())
    {
      const auto& arr = texure_obj.GetArray();
      for (int i = 0; i < arr.Size(); ++i)
      {
        auto textureData = deserial.Deserialize("Texture", arr[i]);
        textureID.push_back(textureData.second);
      }

    }
    else
    {
      auto textureData = deserial.Deserialize("Texture", texure_obj);
      textureID.push_back(textureData.second);
    }

    numOfParticles = daObject["Number"].GetInt();
    lifeOfParticles = daObject["Life"].GetFloat();
    fadeRate = daObject["FadeRate"].GetFloat();
    info.minSpeed = daObject["MinSpeed"].GetFloat();
    info.maxSpeed = daObject["MaxSpeed"].GetFloat();
    info.minAngle = daObject["MinSpreadAngle"].GetFloat();
    info.maxAngle = daObject["MaxSpreadAngle"].GetFloat();
    info.direction = daObject["Direction"].GetFloat();
    info.burst = daObject["Burst"].GetBool();
    info.timer = daObject["Timer"].GetFloat();
    if (daObject.HasMember("Gravity"))
    {
      info.gravity_value = daObject["Gravity"].GetFloat();
    }
    if (daObject.HasMember("Wavey"))
    {
      info.wavey = daObject["Wavey"].GetBool();
    }
    if (daObject.HasMember("Offset"))
    {
      auto jarr = daObject["Offset"].GetArray();
      info.offset.x = jarr[0].GetFloat();
      info.offset.y = jarr[1].GetFloat();
    }
    if (daObject.HasMember("Size"))
    {
      auto jarr = daObject["Size"].GetArray();
      info.size.x = jarr[0].GetFloat();
      info.size.y = jarr[1].GetFloat();
    }
    if (daObject.HasMember("Type"))
    {
      type = emmiter_type(daObject["Type"].GetInt());
    }
  }

  void ParticleEmitter::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("ParticleEmitter");
    writer.StartObject();

    //Write the texture's name, given the texture ID
    TextureManager* theTextureManager = dynamic_cast<TextureManager*>
      (theEngine->Get("TextureManager"));
      writer.WriteKey("Texture");
      writer.StartArray();
    for (int i = 0; i < textureID.size(); ++i)
    {
      Texture texture = (*theTextureManager)[textureID[i]];
      writer.WriteJustString(texture.GetFileLocation().c_str());
    }
    writer.EndArray();
    writer.WriteInt("Number", numOfParticles);
    writer.WriteFloat("Life", lifeOfParticles);
    writer.WriteFloat("FadeRate", fadeRate);
    writer.WriteFloat("MinSpeed", info.minSpeed);
    writer.WriteFloat("MaxSpeed", info.maxSpeed);
    writer.WriteFloat("MinSpreadAngle", info.minAngle);
    writer.WriteFloat("MaxSpreadAngle", info.maxAngle);
    writer.WriteFloat("Direction", info.direction);
    writer.WriteBool("Burst", info.burst);//behavior
    writer.WriteFloat("Timer", info.timer);
    writer.WriteFloat("Gravity", info.gravity_value);
    writer.WriteBool("Wavey", info.wavey);
    writer.WriteVec2("Offset", info.offset.x, info.offset.y);
    writer.WriteVec2("Size", info.size.x, info.size.y);
    writer.WriteInt("Type", static_cast<int>(type));

    writer.EndObject();
  }

  void ParticleEmitter::ImGuiDisplay(float scale)
  {
    ImGui::PushItemWidth(250 * scale);
    ImGui::InputFloat("Life Time", &lifeOfParticles, 1);
    ImGui::InputFloat("Fade Rate", &fadeRate, 1);
    ImGui::InputInt("Number of Partcles", &numOfParticles, 1);
    ImGui::InputFloat("MinSpeed", &info.minSpeed, 1);
    ImGui::InputFloat("MaxSpeed", &info.maxSpeed, 1);
    ImGui::InputFloat("MinSpreadAngle", &info.minAngle, 1);
    ImGui::InputFloat("MaxSpreadAngle", &info.maxAngle, 1);
    ImGui::InputFloat("Direction", &info.direction, 1);
    ImGui::InputFloat("Timer", &info.timer, 1);
    ImGui::InputFloat("Gravity", &info.gravity_value, 1);
    ImGui::InputFloat2("Offset", &info.offset[0]);
    ImGui::InputFloat2("Size", &info.size[0]);
    //No Bool in GUI use Checkbox
    ImGui::Checkbox("Burst", &info.burst);
    ImGui::Checkbox("Wavey", &info.wavey);
    ImGui::PopItemWidth();
  }

  const int ParticleEmitter::GetTextureId() const
  {
    int index = RandomRange(0, textureID.size() - 1);
    return textureID[index];
  }

  void ParticleEmitter::ResetParticles()
  {
    Transform* theTransform =
      GetComponentFromParent<Transform>(*theEngine, parentID, "Transform");

    if (burst_info.burst == true)
    {
      burst_particles.resize(numOfParticles);
      for (Particle& particle : burst_particles)
      {
        makeBurstParticle(&particle);
      }
    }

  }

  void ParticleEmitter::SetBurst(bool value)
  {
    info.burst = value;
  }

  ParticleInfo ParticleEmitter::CopyInfo()
  {
    return info;
  }

  void ParticleEmitter::SetInfo(ParticleInfo& information)
  {
    info = information;
  }

  void ParticleEmitter::SetBurstInfo(ParticleInfo information)
  {
    burst_info = information;
  }



  void ParticleEmitter::makeParticle(Particle* theParticle) const
  {
    Transform* theTransform =
      GetComponentFromParent<Transform>(*theEngine, parentID, "Transform");

    float random = ((rand() % 100) - 50) / 10.0f;
    float randomAngle = RandomRange(info.minAngle, info.maxAngle);
    glm::vec2 angle = glm::rotate(glm::vec2(1, 0),
      glm::radians(randomAngle + (info.direction + theTransform->GetRotation())));

    float speed = RandomRange(info.minSpeed, info.maxSpeed);
    theParticle->size += RandomRange(-10, 10);
    theParticle->textuere_id = textureID[RandomRange(0, textureID.size() - 1)];

    //float rColor = ((rand() % 100) / 100.0f);
    //float gColor = ((rand() % 100) / 100.0f);
    //float bColor = ((rand() % 100) / 100.0f);
    //spawn at emitors location
    switch (type)
    {
    case emmiter_type::point:
    {
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(info.offset.x, info.offset.y, -100.0f); //offset
      break;
    }
    case emmiter_type::circle:
    {
      float raduis = glm::sqrt(RandomRange(0, 1)) * info.size.x;
      float theta = glm::radians(RandomRange(0, 360));
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(info.offset.x + (raduis * cos(theta)), info.offset.y + (raduis * sin(theta)), -100.0f);
      break;
    }
    case emmiter_type::square:
    {
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(info.offset.x + RandomRange(info.size.x / -2, info.size.x / 2),
          info.offset.y + RandomRange(info.size.y / -2, info.size.y / 2), -100.0f); //offset
      break;
    }
    }

    //theParticle->Color = glm::vec4(rColor, gColor, bColor, 1.0f);
    theParticle->Color = glm::vec4(1.0f);
    theParticle->Life = lifeOfParticles;
    theParticle->Velocity = -angle * speed;//just works =(
  }

  void ParticleEmitter::makeBurstParticle(Particle* theParticle) const
  {
    Transform* theTransform =
      GetComponentFromParent<Transform>(*theEngine, parentID, "Transform");

    float random = ((rand() % 100) - 50) / 10.0f;
    float randomAngle = RandomRange(burst_info.minAngle, burst_info.maxAngle);
    glm::vec2 angle = glm::rotate(glm::vec2(1, 0),
      glm::radians(randomAngle + (burst_info.direction + theTransform->GetRotation())));

    float speed = RandomRange(burst_info.minSpeed, burst_info.maxSpeed);
    theParticle->size += RandomRange(-10, 10);
    theParticle->textuere_id = textureID[RandomRange(0, (textureID.size() - 1))];
    //float rColor = ((rand() % 100) / 100.0f);
    //float gColor = ((rand() % 100) / 100.0f);
    //float bColor = ((rand() % 100) / 100.0f);
    //spawn at emitors location
    switch (type)
    {
    case emmiter_type::point:
    {
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(burst_info.offset.x, burst_info.offset.y, -100.0f); //offset
      break;
    }
    case emmiter_type::circle:
    {
      float raduis = glm::sqrt(RandomRange(0, 1)) * burst_info.size.x;
      float theta = glm::radians(RandomRange(0, 360));
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(burst_info.offset.x + (raduis * cos(theta)), burst_info.offset.y + (raduis * sin(theta)), -100.0f);
      break;
    }
    case emmiter_type::square:
    {
      theParticle->Position = theTransform->GetTranslation()
        + glm::vec3(burst_info.offset.x + RandomRange(burst_info.size.x / -2, burst_info.size.x / 2),
          burst_info.offset.y + RandomRange(burst_info.size.y / -2, burst_info.size.y / 2), -100.0f); //offset
      break;
    }
    }

    //theParticle->Color = glm::vec4(rColor, gColor, bColor, 1.0f);
    theParticle->Color = glm::vec4(1.0f);
    theParticle->Life = lifeOfParticles;
    theParticle->Velocity = -angle * speed;//just works =(
  }






} // End Of Dapper Namespace