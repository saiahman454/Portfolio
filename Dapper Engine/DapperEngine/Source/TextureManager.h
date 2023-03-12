//------------------------------------------------------------------------------
//
// File Name:	TextureManager.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ITypeManager.h"
#include "Texture.h"
#include "Engine.h"
#include "IDManager.h"
#include "Messenger.h"

//------------------------------------------------------------------------------
// Dapper Namespace!
namespace Dapper 
{
    class TextureManager : public ITypeManager<Texture>
    {
    public:
        TextureManager(Engine& engine);
        ~TextureManager()override;

        inline const std::string& Name()const override { return name; }

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        inline Texture& operator[](int id) { return texture_list[ID_manager[id]]; }

        Texture& operator[](std::string texture_name);

        int GetNew();

        std::pair<Texture*, int> HasTexture(std::string texture_name);

        void LoadNewTexture(std::string texture_name);

        inline int Size() { return (int)texture_list.size(); }

        int Search(std::string search_name);

        void RegisterFunction(Messenger<TextureManager&>::func function);

    private:
        const std::string name = "TextureManager";
        std::vector<Texture> texture_list;
        Engine& the_engine;
        IDManager ID_manager;
        Messenger<TextureManager&> on_reallocate;


        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of TextureManager class

    std::pair<std::string_view, int> TextureDeserialize(Engine& engine, const rapidjson::Value& val);

} // End Of Dapper Namespace