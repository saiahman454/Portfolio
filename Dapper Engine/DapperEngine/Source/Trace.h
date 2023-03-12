//------------------------------------------------------------------------------
//
// File Name:	Trace.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ISystem.h"
#include <vector>
#include <fstream>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class Trace : public ISystem
    {
    public:
        Trace();
        ~Trace()override;

        const std::string & Name()const override;

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        operator std::ofstream&();

        std::ofstream& GetStream();

    private:
        const std::string name = "Trace";
        std::vector<const char*> messages;
        std::ofstream tracelog;


        // Inherited via ISystem
        virtual bool UpdateWhilePaused() const override;

    };//End of Trace class

} // End Of Dapper Namespace