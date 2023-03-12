//------------------------------------------------------------------------------
//
// File Name:	NameHere.h
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
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class NameHere : public ISystem
    {
    public:
        NameHere();
        ~NameHere()override;

        const std::string & Name()const override;

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

    private:
        const std::string name = "NameHere";

    };//End of NameHere class

} // End Of Dapper Namespace