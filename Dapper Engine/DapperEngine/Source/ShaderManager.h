//------------------------------------------------------------------------------
//
// File Name:	ShaderManager.h
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
#include "Shader.h"
#include "Engine.h"

//------------------------------------------------------------------------------
// Dapper Namespace!
namespace Dapper 
{
    class ShaderManager : public ITypeManager<Shader>
    {
    public:
        ShaderManager(Engine& engine);
        ~ShaderManager()override;

        inline const std::string& Name()const override { return name; }

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        inline Shader& operator[](int index) { return shader_list[index]; }

        int GetNew();

        Shader& operator[](std::string shader_name);

        std::pair<Shader*, int> HasShader(std::string shader_name);
    private:
        const std::string name = "ShaderManager";
        std::vector<Shader> shader_list;
        Engine& the_engine;


        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of TextureManager class

    std::pair<std::string_view, int> ShaderDeserialize(Engine& engine, const rapidjson::Value& val);

} // End Of Dapper Namespace