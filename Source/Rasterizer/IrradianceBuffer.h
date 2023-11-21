﻿#pragma once
#include <Core.h>
#include "HDRColor.h"
#include "LDRColor.h"
#include "Point2.h"

namespace core
{
    //  The origin of the coordinate is the topleft corner.
    struct FIrradianceBuffer
    {
    private:
        FHDRColor * * HDRData;
        FLDRColor * * LDRData;
        uint8 * LDRRawData;
        int32 Width;
        int32 Height;

    public:
        FIrradianceBuffer(int32 InWidth, int32 InHeight);
        void Clear(const FHDRColor& Color);
        void FillUpHorizontal(int32 Y, int32 StartX, int32 EndX, const FHDRColor& Color);
        void ToneMaping();
        uint8 * GetRawLDRData() const;
        ~FIrradianceBuffer();
    };
}
