﻿#pragma once
#include <Core.h>
#include "Actor.h"
#include "Camera.h"
#include <vector>
#include "../Rasterizer/IrradianceBuffer.h"

namespace core
{
    class UWorld
    {
        std::unique_ptr<UCamera> DefaultCamera;
        std::vector<std::unique_ptr<UActor>> BasicActorList;

    public:
        void AddCameraActor(std::unique_ptr<UCamera> CameraActor);
        void AddBasicActor(std::unique_ptr<UActor> BasicActor);
        void Tick(float DeltaTime);
        void Render(FIrradianceBuffer& IrradianceBuffer);
    };
}
