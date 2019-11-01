#pragma once
#include<stdio.h>

#include "bitmapInfoHeader.h"

#include"pixelDataRGB.h"

pixelDataRGB* getPictureData(FILE*, bitmapInfoHeader*);