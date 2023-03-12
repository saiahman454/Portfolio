//------------------------------------------------------------------------------
//
// File Name:	PlayerController.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "PlayerController.h"
#include "Input.h"
#include "Transform.h"
#include "Physics.h"
#include "Utility.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ParticleEmitter.h"
#include "Health.h"
#include "SquareCollider.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/vec3.hpp"
#include "SceneManager.h"
#include "Sprite.h"
#include "AnimationSpine.h"
#include "CollectibleBehavior.h"
#include "CameraAnchor.h"
#include <cmath>
#include "BGMController.h"


// Dapper Namespace!
namespace Dapper {
    PlayerController::PlayerController(int id, Engine& engineParam) :
        IStateMachine(PlayerStates::PlayerInvalid, PlayerStates::PlayerIdle),
        my_ID(id), engine(engineParam),
        speed(20)
    {
    }

    PlayerController::~PlayerController()
    {
    }

    void PlayerController::OnExit()
    {

        switch (GetState())
        {
        case PlayerStates::PlayerIdle:
            break;
        case PlayerStates::PlayerMove:
            break;
        }

    }

    void PlayerController::OnInit()
    {

        const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
        movesound = audio.GetSound("/SFX/Player/Player_Move");

        // If the states get too complicated we can separate the states into classes and
        // have it do state pointers instead.

        switch (GetState())
        {
        case PlayerStates::PlayerIdle:
            //std::cout << "Entering PlayerIdle" << std::endl;
            break;
        case PlayerStates::PlayerMove:
            //std::cout << "Entering PlayerMove" << std::endl;
            break;
        }

        SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
        if (scene_man.IsSceneLoaded("Level1"))
        {
            current_scene = "Level1";
        }
        else if (scene_man.IsSceneLoaded("Level2"))
        {
            current_scene = "Level2";
        }
        else if (scene_man.IsSceneLoaded("Level3"))
        {
            current_scene = "Level3";
        }
        else
        {
            current_scene = "Invalid";
        }

    }

    void PlayerController::OnUpdate(float dt)
    {

        // Update invincibility
        if (invin_timer > 0)
        {
            invin_timer -= dt;
        }
        else
        {
            invincibility_flag = false;
            invin_timer = 0;
        }
        if (!invincibility_flag)
        {
            Sprite& sprite = *GetComponentFromParent<Sprite>(engine, parent_ID, "Sprite");
            sprite.SetTint(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
        if (invincible)
        {
            Sprite& sprite = *GetComponentFromParent<Sprite>(engine, parent_ID, "Sprite");
            sprite.SetTint(glm::vec4(0.4f, 0.4f, 1.0f, 1.0f));
        }

        if (!engine.IsGamePaused())
        {



            Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
            const std::vector<keyboard_key> keys = input.GetAllKeys();

            input_direction = { 0,0 };

            // Input direction logic
            if (keys[GLFW_KEY_W] == keyboard_key::KEY_DOWN)
            {
                input_direction.y = 1;
            }
            if (keys[GLFW_KEY_D] == keyboard_key::KEY_DOWN)
            {
                input_direction.x = 1;
            }
            if (keys[GLFW_KEY_S] == keyboard_key::KEY_DOWN)
            {
                if (input_direction.y == 1)
                    input_direction.y = 0;
                else
                    input_direction.y = -1;
            }
            if (keys[GLFW_KEY_A] == keyboard_key::KEY_DOWN)
            {
                if (input_direction.x == 1)
                    input_direction.x = 0;
                else
                    input_direction.x = -1;
            }
            if (input_direction != glm::vec2{ 0,0 })
            {
                input_direction = glm::normalize(input_direction);


                // Actual Velocity calculation
                if (input_direction != -move_direction)
                {

                    acc += (acceleration_multiplier_per_sec * dt);

                }
                else {

                    Decelerate(dt);

                }

            }
            else {

                Decelerate(dt);

            }

            Physics& physics = *GetComponentFromParent<Physics>(engine, parent_ID, "Physics");
            Transform& transform = *GetComponentFromParent<Transform>(engine, parent_ID, "Transform");

            AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(engine, parent_ID, "AnimationSpine");
            spine::TrackEntry* current_animation;
            switch (GetState())
            {
            case PlayerStates::PlayerIdle:
            {

                float rotationRawAngle = atan2(new_move_direction.y, new_move_direction.x);
                rotationRawAngle = glm::degrees(rotationRawAngle);

                if (std::abs(rotationRawAngle) > 90)
                {
                    SpriteFlip(physics, transform);
                }
                transform.setRotation(rotationRawAngle);

                // Play idle animation
                spine::TrackEntry* current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
                if (!current_animation || current_animation->getAnimation()->getName() == "Swim")
                {
                    animation.GetInstance()->GetAnimationState()->setAnimation(0, "Idle", true);
                }
                animation.GetInstance()->GetAnimationState()->addAnimation(0, "Idle", true, 0);

                if ((keys[GLFW_KEY_W] == keyboard_key::KEY_DOWN) || (keys[GLFW_KEY_A] == keyboard_key::KEY_DOWN) ||
                    (keys[GLFW_KEY_S] == keyboard_key::KEY_DOWN) || (keys[GLFW_KEY_D] == keyboard_key::KEY_DOWN ||
                        keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN
                        || keys[GLFW_KEY_E] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_Q] == keyboard_key::KEY_DOWN ||
                        keys[GLFW_KEY_T] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_Y] == keyboard_key::KEY_DOWN))
                    SetNextState(PlayerStates::PlayerMove);

                vel = (speed * acc);

                break;
            }
            case PlayerStates::PlayerMove:

                UpdateRotation(dt);

                // Play swim animation- idle and swim will constantly switch between each other, unless player is hurt or dashing
                current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
                if (!current_animation || current_animation->getAnimation()->getName() == "Idle")
                {
                    animation.GetInstance()->GetAnimationState()->setAnimation(0, "Swim", true);
                }
                animation.GetInstance()->GetAnimationState()->addAnimation(0, "Swim", true, 0);

                vel = (speed * acc);

                if (vel > max_speed)
                {
                    // Separate rotation and movement logic
                    vel = std::lerp(vel, max_speed, 1);
                    acc = vel / speed;

                }
                else if (vel > dash_max_speed)
                {

                    vel = dash_max_speed;
                    acc = vel / speed;

                }

                if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_TRIGGERED && can_dash == true)
                {
                    // Set next state to be dash
                    SetNextState(PlayerStates::PlayerDash);

                    dashing = !dashing;

                    // Play the dash animation
                    animation.GetInstance()->GetAnimationState()->setAnimation(0, "Dash", false);
                    animation.GetInstance()->GetAnimationState()->addAnimation(0, "Swim", true, 0);
                    can_dash = false;

                    // Play dash sound
                    const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                    auto sound = audio.GetSound(dash_fileName);
                    sound->Play();

                    dash_max_speed = max_speed + dash_force;
                    vel += dash_force;
                }


                // physics.SetAccelration(speed * acc);

                if (!((keys[GLFW_KEY_W] == keyboard_key::KEY_DOWN) || (keys[GLFW_KEY_A] == keyboard_key::KEY_DOWN) || (keys[GLFW_KEY_S] == keyboard_key::KEY_DOWN)
                    || (keys[GLFW_KEY_D] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN) || GetState() == PlayerStates::PlayerCutScene))
                    SetNextState(PlayerStates::PlayerIdle);
                break;

            case PlayerStates::PlayerDash:

                UpdateRotation(dt);

                if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN)
                {

                    // Keep dashing
                    held_dash_time += dt * dash_base_lasting_time * (max_dash_multiplier - 1);

                }

                invoke_timer += dt;

                if (invoke_timer >= dash_base_lasting_time + held_dash_time)
                {

                    dash_max_speed = max_speed + dash_force;
                    vel += dash_force;

                    invoke_timer = 0;

                    dashing = !dashing;

                    SetNextState(PlayerStates::PlayerMove);

                }
                break;
            case PlayerStates::PlayerCutScene:

                if (!breachplayed)
                {

                    const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                    auto sound = audio.GetSound("/SFX/Player/Player_Breaching");
                    sound->Play();
                    breachplayed = true;

                }

                current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
                if (!current_animation || current_animation->getAnimation()->getName() != "WaterBreach")
                {
                    animation.GetInstance()->GetAnimationState()->setAnimation(0, "WaterBreach", false);
                    ParticleEmitter& particle = *GetComponentFromParent<ParticleEmitter>(engine, parent_ID, "ParticleEmitter");
                    particle.SetBurst(true);
                }

                if (current_animation->isComplete())
                {
                    SetNextState(PlayerStates::PlayerIdle);
                    stop_moving = false;
                    breachplayed = false;


                    SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                    BGMController& bgmcontroller = *(GetSystemFromEngine<BGMController*>(engine, "BGMController"));
                    if (scene_man.IsSceneLoaded("Level1"))
                    {
                        // Win Sound
                        const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                        auto sound = audio.GetSound(fileName);
                        sound->Play();
                        scene_man.FadeToScene(current_scene, "LevelPassed1", ConditionCheck::none);
                        bgmcontroller.CollectedAll(0);
                        //scene_man.LoadScene("LevelPassed1");
                        break; //Adam Early out
                    }
                    if (scene_man.IsSceneLoaded("Level2"))
                    {
                        // Win Sound
                        const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                        auto sound = audio.GetSound(fileName);
                        sound->Play();
                        scene_man.FadeToScene(current_scene, "LevelPassed2", ConditionCheck::none);
                        bgmcontroller.CollectedAll(0);
                        break; //Adam Early out
                    }
                    if (scene_man.IsSceneLoaded("Level3"))
                    {
                        // Win Sound
                        const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                        auto sound = audio.GetSound(fileName);
                        sound->Play();
                        scene_man.FadeToScene(current_scene, "WinGame", ConditionCheck::none);
                        bgmcontroller.CollectedAll(0);
                        break; //Adam Early out
                    }
                }
                break;

            case PlayerStates::PlayerDead:

                stop_moving = true;
                invincible = true;

                Sprite& sprite = *GetComponentFromParent<Sprite>(engine, parent_ID, "Sprite");
                sprite.SetTint(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

                physics.SetVelocity(glm::vec2(0.0f, 30.0f));

                // Delete camera anchor
                GameObjectManager& gameobject_manager =
                    *GetSystemFromEngine<GameObjectManager*>(engine, "GameObjectManager");
                GameObject& gameobject = gameobject_manager[parent_ID];
                CameraAnchor& camera_anchor = *GetComponentFromParent<CameraAnchor>(engine, parent_ID, "CameraAnchor");
                camera_anchor.SetFollowing(false);

                physics.SetAccelration({ 0,0 });
                transform.setRotation(10.0f);
                transform.setScale(glm::vec2(std::abs(transform.GetScale().x), std::abs(transform.GetScale().y)));

                current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
                if (!current_animation || current_animation->getAnimation()->getName() != "Death")
                {
                    animation.GetInstance()->GetAnimationState()->setAnimation(0, "Death", false);
                }
                if (current_animation->isComplete())
                {
                    death_timer -= dt;

                    if (death_timer <= 0.0f)
                    {
                        SceneManager& state =
                            *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

                        // Lose Sound
                        const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                        auto sound = audio.GetSound(lose_fileName);
                        sound->Play();
                        //state.UnloadAllScenes();
                        state.FadeToScene(current_scene, "Lose", ConditionCheck::none);
                        invincible = true;
                        return; //Adam Early out
                    }
                }

                break;
            }

            //Shoot out particles from player
            if (keys[GLFW_KEY_P] == keyboard_key::KEY_TRIGGERED)
            {
                ParticleEmitter& particleEmitter = *GetComponentFromParent<ParticleEmitter>
                    (engine, parent_ID, "ParticleEmitter");
                particleEmitter.ResetParticles();
            }

            // Toggle invincibility
            if (keys[GLFW_KEY_I] == keyboard_key::KEY_TRIGGERED)
            {
                invincible = !invincible;
            }

            // Toggle Wall Clip
            if (keys[GLFW_KEY_U] == keyboard_key::KEY_TRIGGERED)
            {
                SquareCollider& theCollider = *GetComponentFromParent<SquareCollider>(engine, parent_ID, "SquareCollider");
                theCollider.SetGhostCollider(!theCollider.IsGhostCollider());
            }

            //Decrease Health
            if (keys[GLFW_KEY_MINUS] == keyboard_key::KEY_TRIGGERED)
            {
                Health& theHealth = *GetComponentFromParent<Health>
                    (engine, parent_ID, "Health");
                theHealth.DecreaseHealth();
            }
            //Increase Health
            if (keys[GLFW_KEY_EQUAL] == keyboard_key::KEY_TRIGGERED)
            {
                Health& theHealth = *GetComponentFromParent<Health>
                    (engine, parent_ID, "Health");
                theHealth.IncreaseHealth();
            }

            //Level swapping using more than once crashes currently --ADAM
            if (keys[GLFW_KEY_1] == keyboard_key::KEY_TRIGGERED)
            {
                SceneManager& state =
                    *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                if (state.IsSceneLoaded("Level1") == true)
                    return;
                state.UnloadAllScenes();
                state.LoadScene("Level1");
            }
            if (keys[GLFW_KEY_2] == keyboard_key::KEY_TRIGGERED)
            {
                SceneManager& state =
                    *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                if (state.IsSceneLoaded("Level2") == true)
                    return;
                state.UnloadAllScenes();
                state.LoadScene("Level2");
            }
            if (keys[GLFW_KEY_3] == keyboard_key::KEY_TRIGGERED)
            {
                SceneManager& state =
                    *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                if (state.IsSceneLoaded("Level3") == true)
                    return;
                state.UnloadAllScenes();
                state.LoadScene("Level3");
            }
            if (keys[GLFW_KEY_M] == keyboard_key::KEY_TRIGGERED)
            {
                Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
                Dapper::Bus* busbio = audio.GetBus("/");
                bool boi = busbio->GetMute();
                busbio->SetMute(!boi);
            }
            if (keys[GLFW_KEY_L] == keyboard_key::KEY_TRIGGERED)
            {
                SceneManager& state =
                    *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                state.UnloadAllScenes();
                state.LoadScene("Lose");
            }
            if (keys[GLFW_KEY_K] == keyboard_key::KEY_TRIGGERED)
            {
                SceneManager& state =
                    *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
                state.UnloadAllScenes();
                state.LoadScene("WinGame");
            }

            if (current_scene == "Level1")
            {
                if (transform.GetTranslation().x >= 10112.0f && transform.GetTranslation().y >= 1290.0f)
                {
                    transform.setRotation(0);
                    physics.SetAccelration({ 0,0 });
                    physics.SetVelocity({ 0,0 });
                    stop_moving = true;
                    SetNextState(PlayerStates::PlayerCutScene);
                }
            }
            else if (current_scene == "Level2")
            {
                if (transform.GetTranslation().x >= 12448.0f && transform.GetTranslation().y >= 576.0f)
                {
                    transform.setRotation(0);
                    physics.SetAccelration({ 0,0 });
                    physics.SetVelocity({ 0,0 });
                    stop_moving = true;
                    SetNextState(PlayerStates::PlayerCutScene);

                }
            }
            else if (current_scene == "Level3")
            {
                if (transform.GetTranslation().x >= 4530.0f && transform.GetTranslation().y >= 754.0f)
                {
                    transform.setRotation(0);
                    physics.SetAccelration({ 0,0 });
                    physics.SetVelocity({ 0,0 });
                    stop_moving = true;
                    SetNextState(PlayerStates::PlayerCutScene);
                }
            }

            ParticleEmitter& particle = *GetComponentFromParent<ParticleEmitter>(engine, parent_ID, "ParticleEmitter");
            float speed_emit = glm::length(physics.GetVelocity());
            if (speed_emit == 0)
            {
                speed_emit = 0.8;
            }
            else
            {
                speed_emit = 60 / speed_emit;
            }
            particle.setTimer(speed_emit);

            // Music test

            if (keys[GLFW_KEY_J] == keyboard_key::KEY_TRIGGERED)
            {
                const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");

                auto d = audio.GetSound("/SFX/UI/UI_Select");

                d->Play();

            }


            glm::vec2 velocity = physics.GetVelocity();
            if (velocity.x == 0 && velocity.y == 0)
            {
                move_direction = { 0,0 };
            }
            else
            {
                move_direction = glm::normalize(velocity);
            }


            // Dash cooldown handling
            if (invoked_timer == 0 && can_dash == false)
            {
                can_dash = true;

                held_dash_time = 0;
            }

            // Constantly setting velocity, which overrides bounce mechanics if you stay in this state.
            if (!stop_moving)
            {
                physics.SetVelocity(vel * new_move_direction);
                // Audio Breach Set
                // const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");

                //auto d = audio.GetSound("/SFX/Player/Player_WallBreak");
                //d->SetParameter("speed", vel);

            }
        }
    }

    void PlayerController::Init()
    {

        SquareCollider& collider = *GetComponentFromParent<SquareCollider>(engine, parent_ID, "SquareCollider");
        std::function<void(IComponent*)> func = std::bind(&PlayerController::ResolutionFunction, this, std::placeholders::_1);
        collider.RegisterOnChange(func);

    }

    int PlayerController::GetParentID() const
    {
        return parent_ID;
    }

    bool PlayerController::GetInvincible()
    {
        return invincible;
    }

    bool PlayerController::GetInvincibleFlag()
    {
        return invincibility_flag;
    }

    float PlayerController::GetPlayerVel()
    {
        return vel;
    }

    void PlayerController::SetParentID(int ID)
    {
        parent_ID = ID;
    }

    const std::string& PlayerController::GetName() const
    {
        return name;
    }

    void PlayerController::Decelerate(float dt)
    {

        acc += (acceleration_multiplier_per_sec * dt) * -(deceleration_multiplier);
        if (acc < 0)
            acc = 0;

    }
    // This needs fixing, also need a flip function maybe, or just set scale to -x
    void PlayerController::UpdateRotation(float dt)
    {
        if (GetState() != PlayerStates::PlayerCutScene)
        {

            Transform& trans = *GetComponentFromParent<Transform>(engine, parent_ID, "Transform");
            Physics& physics = *GetComponentFromParent<Physics>(engine, parent_ID, "Physics");

            if (dashing)
            {

                new_move_direction = glm::normalize(glm::lerp(new_move_direction, input_direction, dash_drift_speed * dt));

            }
            else {

                new_move_direction = glm::normalize(glm::lerp(new_move_direction, input_direction, drift_speed * dt));

            }

            float rotationRawAngle = atan2(new_move_direction.y, new_move_direction.x);
            rotationRawAngle = glm::degrees(rotationRawAngle);

            if (std::abs(rotationRawAngle) > 90)
            {
                SpriteFlip(physics, trans);
            }
            trans.setRotation(rotationRawAngle);

        }
    }

    void PlayerController::ResolutionFunction(IComponent* object)
    {
        //if mass is not 0 it is a enemy
        //make particles shoot out
        int theParentId = object->GetParentID();
        Physics& physics = *GetComponentFromParent<Physics>(engine, theParentId, "Physics");
        Health& theHealth =
            *GetComponentFromParent<Health>(engine, parent_ID, "Health");
        SceneManager& state =
            *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
        GameObjectManager& gameobject_manager =
            *GetSystemFromEngine<GameObjectManager*>(engine, "GameObjectManager");
        GameObject& gameobject = gameobject_manager[theParentId];

        Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
        const std::vector<keyboard_key> keys = input.GetAllKeys();

        //for collecting health bubbles
        if (gameobject.GetFlag() == "FloatingHealth")
        {
            theHealth.IncreaseHealth();
            // Emit particles at the health's location
            // Emit particles at the player's position
            ParticleEmitter& particleEmitter =
                *GetComponentFromParent<ParticleEmitter>(engine, theParentId, "ParticleEmitter");

            if (&particleEmitter)
            {
                ParticleInfo  info(0, 360, 50, 100, 90, true, 1.0f, -75, true, { 0,0 }, { 0,0 });
                particleEmitter.SetBurstInfo(info);
                particleEmitter.ResetParticles();

            }
        }
        else if (gameobject.GetFlag() == "Tile")
        {
            // WARNNING SPAMS AUDIO LAGS OUT GAME
            /* Play bounce sound
            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(tile_fileName);
            sound->Play();
            */

        }
        else if (gameobject.GetFlag() == "Collectible")
        {
            auto& collectableBehavior = *GetComponentFromParent<CollectibleBehavior>(engine, gameobject.GetID(), "CollectibleBehavior");

            if (collectableBehavior.IsCollectable())
            {
                // Send message to UI behavior to increase number of collectibles
                collectableBehavior.Collect();
                on_collection.SendMessage();

            }
        }

        // Upon enemy collision, update properly
        if (invin_timer == 0)
        {
            if (!invincible)
            {
                // Enemy Collision
                if (gameobject.GetFlag() == "Enemy")
                {
                    invincibility_flag = true;
                    // Decrement the health and check for lose state
                    theHealth.DecreaseHealth();
                    // Play the hurt animation
                    AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(engine, parent_ID, "AnimationSpine");

                    spine::TrackEntry* current_animation_swim = animation.GetInstance()->GetAnimationState()->getCurrent(0);
                    //animation.GetInstance()->GetAnimationState()->clearTracks();
                    // Get the name of the enemy and check if it is jellyfish
                    std::string object_name = gameobject.GetName();
                    std::string core_name = object_name.substr(0, 14);
                    if (object_name.substr(0, 14) == "JellyfishEnemy")
                    {
                        animation.GetInstance()->GetAnimationState()->setAnimation(0, "Shock", false);
                    }
                    else
                    {
                        animation.GetInstance()->GetAnimationState()->setAnimation(0, "Hurt", false);
                    }

                    if (theHealth.GetHealth() <= 0)
                    {
                        SetNextState(PlayerStates::PlayerDead);
                    }

                    // Emit particles at the player's position
                    ParticleEmitter& particleEmitter =
                        *GetComponentFromParent<ParticleEmitter>(engine, parent_ID, "ParticleEmitter");

                    ParticleInfo info(0, 360, 50, 100, 90, true, 1.0f, -75, true, { 0,0 }, { 0,0 });
                    particleEmitter.SetBurstInfo(info);
                    particleEmitter.ResetParticles();

                    invin_timer = invin_duration;

                }
            }
        }

    }

    void PlayerController::SpriteFlip(Physics& physics, Transform& transform)
    {
        glm::vec2 new_scale = { transform.GetScale().x, -transform.GetScale().y };
        transform.setScale(new_scale);
    }

    void PlayerController::RegisterMessage(std::function<void()> function)
    {
        on_collection.RegisterMessage(function);
    }

    void PlayerController::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {

        deserial;
        // The instantaneous acceleration which a player starts at, IE when they start moving.
        auto obj = jsonVal.GetObject();
        auto spe = obj["Speed"].GetFloat();
        this->speed = spe;

        auto maxspe = obj["MaxSpeed"].GetFloat();
        this->max_speed = maxspe;

        auto dashmax = obj["DashMaxSpeed"].GetFloat();
        this->dash_max_speed = dashmax;

        auto accel_mult = obj["AccelerationMultiplier"].GetFloat();
        this->acceleration_multiplier_per_sec = accel_mult;

        auto decel = obj["DecelerationMultiplier"].GetFloat();
        this->deceleration_multiplier = decel;

        auto drift = obj["DriftSpeed"].GetFloat();
        this->drift_speed = drift;

        auto dashdrift = obj["DashDriftSpeed"].GetFloat();
        this->dash_drift_speed = dashdrift;

        auto dashmaxmult = obj["MaxDashMultiplier"].GetFloat();
        this->max_dash_multiplier = dashmaxmult;

        auto lastingtime = obj["DashBaseLastingTime"].GetFloat();
        this->dash_base_lasting_time = lastingtime;

        auto dashh = obj["DashForce"].GetFloat();
        this->dash_force = dashh;

        auto dname = obj["DashFileName"].GetString();
        this->dash_fileName = dname;

        auto dvol = obj["DashVolume"].GetFloat();
        this->dash_volume = dvol;

        auto bname = obj["BounceFileName"].GetString();
        this->tile_fileName = bname;

        auto bvol = obj["BounceVolume"].GetFloat();
        this->tile_volume = bvol;

        auto fname = obj["WinFileName"].GetString();
        this->fileName = fname;

        auto vol = obj["WinVolume"].GetFloat();
        this->volume = vol;

        auto flname = obj["LoseFileName"].GetString();
        this->lose_fileName = flname;

        auto lvol = obj["LoseVolume"].GetFloat();
        this->lose_volume = lvol;

        auto mname = obj["MoveSoundFileName"].GetString();
        this->move_fileName = mname;

        auto mvol = obj["MoveSoundVolume"].GetFloat();
        this->move_volume = mvol;

        auto level_x = obj["LevelEndTriggerX"].GetFloat();
        this->level_end_trigger_x = level_x;

    }

    void PlayerController::Write(IJsonWriter& writer) const
    {
        writer.WriteKey("PlayerController");
        writer.StartObject();
        writer.WriteFloat("Speed", speed);
        writer.WriteFloat("MaxSpeed", max_speed);
        writer.WriteFloat("DashMaxSpeed", dash_max_speed);
        writer.WriteFloat("AccelerationMultiplier", acceleration_multiplier_per_sec);
        writer.WriteFloat("DecelerationMultiplier", deceleration_multiplier);
        writer.WriteFloat("DriftSpeed", drift_speed);
        writer.WriteFloat("DashDriftSpeed", dash_drift_speed);
        writer.WriteFloat("MaxDashMultiplier", max_dash_multiplier);
        writer.WriteFloat("DashBaseLastingTime", dash_base_lasting_time);
        writer.WriteFloat("DashForce", dash_force);
        writer.WriteString("DashFileName", dash_fileName.c_str());
        writer.WriteFloat("DashVolume", dash_volume);
        writer.WriteString("BounceFileName", tile_fileName.c_str());
        writer.WriteFloat("BounceVolume", tile_volume);
        writer.WriteString("WinFileName", fileName.c_str());
        writer.WriteFloat("WinVolume", volume);
        writer.WriteString("LoseFileName", lose_fileName.c_str());
        writer.WriteFloat("LoseVolume", lose_volume);
        writer.WriteString("MoveSoundFileName", move_fileName.c_str());
        writer.WriteFloat("MoveSoundVolume", move_volume);
        writer.WriteFloat("LevelEndTriggerX", level_end_trigger_x);
        writer.EndObject();
    }

    void PlayerController::ImGuiDisplay(float scale)
    {
        scale;
        ImGui::InputFloat("Speed", &speed, 10.0f);
        ImGui::InputFloat("MaxSpeed", &max_speed, 100.0f);
        ImGui::InputFloat("DashMaxSpeed", &dash_max_speed, 100.0f);
        ImGui::InputFloat("AccelerationMultiplayer", &acceleration_multiplier_per_sec, 10.0f);
        ImGui::InputFloat("DecelerationMultiplier", &deceleration_multiplier, 10.0f);
        ImGui::InputFloat("DriftSpeed", &drift_speed, 10.0f);
        ImGui::InputFloat("DashDriftSpeed", &dash_drift_speed, 10.0f);
        ImGui::InputFloat("MaxDashMultiplier", &max_dash_multiplier, 10.0f);
        ImGui::InputFloat("DashBaseLastingTime", &dash_base_lasting_time, 10.0f);
        ImGui::InputFloat("DashForce", &dash_force, 100.0f);
        ImGui::InputFloat("Acceleration", &acc, 10.0f);
        ImGui::Checkbox("Invincible", &invincible);
        // dash sound

        if (ImGui::InputFloat("DashVolume", &dash_volume, 0.05f)) {

            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(dash_fileName);
            sound->SetVolume(dash_volume);

        }
        // Tile collision

        if (ImGui::InputFloat("BounceVolume", &tile_volume, 0.05f)) {

            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(tile_fileName);
            sound->SetVolume(tile_volume);

        }
        // win

        if (ImGui::InputFloat("WinVolume", &volume, 0.05f)) {

            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(fileName);
            sound->SetVolume(volume);

        }

        // lose

        if (ImGui::InputFloat("LoseVolume", &lose_volume, 0.05f)) {

            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(lose_fileName);
            sound->SetVolume(lose_volume);

        }

        // move

        if (ImGui::InputFloat("MoveSoundVolume", &move_volume, 0.05f)) {

            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(move_fileName);
            sound->SetVolume(move_volume);

        }

    }

    void PlayerController::SetID(int ID)
    {
        my_ID = ID;
    }

    const int PlayerController::GetID() const
    {
        return my_ID;
    }

    bool PlayerController::UpdateWhilePaused() const
    {
        return false;
    }

    //void PlayerController::RegisterFunction(Messenger<MeshManager&>::func function)
    //{
    //    on_collection.RegisterMessage(function);
    //}

} // End Of Dapper Namespace