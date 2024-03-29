﻿#include "Game.h"
#include "../Math/MathGlobal.h"
#include "../Components/StaticMeshComponent.h"
#include "../Components/ShadingComponent.h"
#include "../Rasterizer/Vertex.h"
#include "Camera.h"
#include "../Rasterizer/Driver.h"
#include "../Others/AssetManager.h"

namespace core
{
    void UGame::Initialize(int32 ViewportWidth, int32 ViewportHeight)
    {
        CurrentWorld = std::make_unique<UWorld>();

        FVertex Vertex0(FVector4(-20.0, -20, 0, 1.0), FHDRColor::Red, FVector2(0, 1.0f));
        FVertex Vertex1(FVector4(-20.0, 20.0, 0, 1.0), FHDRColor::Green, FVector2(0, 0));
        FVertex Vertex2(FVector4(20.0, 20.0, 0, 1.0), FHDRColor::Blue, FVector2(1.0, 0));
        FVertex Vertex3(FVector4(20.0, -20.0, 0, 1.0), FHDRColor::Yellow, FVector2(1.0, 1.0));

        std::unique_ptr<UStaticMeshComponent> StaticMeshComponent = std::make_unique<UStaticMeshComponent>();
        StaticMeshComponent->AddVertex(Vertex0);
        StaticMeshComponent->AddVertex(Vertex1);
        StaticMeshComponent->AddVertex(Vertex2);
        StaticMeshComponent->AddVertex(Vertex3);

        //  Clockwise.
        StaticMeshComponent->AddIndex(0);
        StaticMeshComponent->AddIndex(1);
        StaticMeshComponent->AddIndex(2);

        StaticMeshComponent->AddIndex(0);
        StaticMeshComponent->AddIndex(2);
        StaticMeshComponent->AddIndex(3);

        std::unique_ptr<FTexture> AlbedoTexture = std::make_unique<FTexture>();
        UAssetManager::Instance()->LoadTexture("book_pbrt.png", AlbedoTexture.get());

        std::unique_ptr<UShadingComponent> ShadingComponent = std::make_unique<UShadingComponent>();
        ShadingComponent->SetAlbedo(std::move(AlbedoTexture));

        std::unique_ptr<UActor> TriangleActor = std::make_unique<UActor>();
        TriangleActor->RegisterStaticMeshComponent(std::move(StaticMeshComponent));
        TriangleActor->RegisterShadingComponent(std::move(ShadingComponent));
        CurrentWorld->AddBasicActor(std::move(TriangleActor));

        std::unique_ptr<UCamera> CameraActor = std::make_unique<UCamera>();
        CameraActor->Aspect = static_cast<float>(ViewportWidth) / static_cast<float>(ViewportHeight);
        CameraActor->ZNear = 0.1f;
        CameraActor->ZFar = 1000.0f;
        CameraActor->FieldOfViewY = Degree2Radian(90.0f);
        CameraActor->Position = FVector3(0, 0, -40);
        CurrentWorld->AddCameraActor(std::move(CameraActor));

        UDriver::Instance()->SetViewport(0, 0, ViewportWidth, ViewportHeight);
        IrradianceBuffer = std::make_unique<FIrradianceBuffer>(ViewportWidth, ViewportHeight);
    }

    void UGame::Tick(const FJoystick& Joystick, float DeltaTime)
    {
        CurrentWorld->Tick(Joystick, DeltaTime);
    }

    void UGame::Render(bool bPerspectiveCorrectInterpolation)
    {
        IrradianceBuffer->Clear(FHDRColor(0, 0.2f, 0.4f, 1.0f));
        CurrentWorld->Render(bPerspectiveCorrectInterpolation, *IrradianceBuffer);
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
