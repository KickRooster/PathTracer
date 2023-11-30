﻿#include "Game.h"

#include <algorithm>

#include "../Components/StaticMeshComponent.h"
#include "../Rasterizer/Vertex.h"
#include "Camera.h"
#include "../Rasterizer/Driver.h"

namespace core
{
    void UGame::Initialize(int32 ViewportWidth, int32 ViewportHeight)
    {
        CurrentWorld = std::make_unique<UWorld>();

        //  Clockwise.
        FVertex Vertex0(-20.0, 20, 0, 1.0);
        FVertex Vertex1(20, 20.0, 0, 1.0);
        FVertex Vertex2(20.0, -20.0, 0, 1.0);
        FVertex Vertex3(-20.0, -20.0, 0, 1.0);
        FVertex Vertex4(0, -40.0, 0, 1.0);
        FVertex Vertex5(40, 0, 0, 1.0);
        std::unique_ptr<UStaticMeshComponent> StaticMeshComponent = std::make_unique<UStaticMeshComponent>();
        StaticMeshComponent->AddVertex(Vertex0);
        StaticMeshComponent->AddVertex(Vertex1);
        StaticMeshComponent->AddVertex(Vertex2);
        StaticMeshComponent->AddVertex(Vertex3);
        StaticMeshComponent->AddVertex(Vertex4);
        StaticMeshComponent->AddVertex(Vertex5);
        StaticMeshComponent->AddIndex(0);
        StaticMeshComponent->AddIndex(1);
        StaticMeshComponent->AddIndex(2);
        StaticMeshComponent->AddIndex(2);
        StaticMeshComponent->AddIndex(3);
        StaticMeshComponent->AddIndex(0);
        StaticMeshComponent->AddIndex(3);
        StaticMeshComponent->AddIndex(2);
        StaticMeshComponent->AddIndex(4);
        StaticMeshComponent->AddIndex(1);
        StaticMeshComponent->AddIndex(5);
        StaticMeshComponent->AddIndex(2);

        std::unique_ptr<UActor> TriangleActor = std::make_unique<UActor>();
        TriangleActor->RegisterStaticMeshComponent(std::move(StaticMeshComponent));
        CurrentWorld->AddBasicActor(std::move(TriangleActor));

        std::unique_ptr<UCamera> CameraActor = std::make_unique<UCamera>();
        CameraActor->Aspect = static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight);
        CameraActor->ZNear = 0.1f;
        CameraActor->ZFar = 1000.0f;
        CameraActor->FieldOfViewY = Degree2Radian(120.0f);
        CameraActor->Position = FVector3(0, 0, -40);
        CurrentWorld->AddCameraActor(std::move(CameraActor));

        UDriver::Instance()->SetViewport(0, 0, ViewportWidth, ViewportHeight);
        IrradianceBuffer = std::make_unique<FIrradianceBuffer>(ViewportWidth, ViewportHeight);
    }

    void UGame::Tick(const FJoystick& Joystick, float DeltaTime)
    {
        CurrentWorld->Tick(Joystick, DeltaTime);
    }

    void UGame::Render()
    {
        IrradianceBuffer->Clear(FHDRColor(0, 0.2f, 0.4f, 1.0f));
        CurrentWorld->Render(*IrradianceBuffer);
        IrradianceBuffer->ToneMaping();
    }

    uint8 * UGame::GetLDRData() const
    {
        return IrradianceBuffer->GetRawLDRData();
    }

    void UGame::ShutDown()
    {
    }
}