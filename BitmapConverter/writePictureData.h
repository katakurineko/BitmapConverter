#pragma once
#include<stdio.h>

#include"pixelDataRGB.h"

#include "bitmapInfoHeader.h"

void writePictureData(FILE*, bitmapInfoHeader*, pixelDataRGB*);