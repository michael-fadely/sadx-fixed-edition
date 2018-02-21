#pragma once

#include <random>

#include <SADXModLoader.h>

#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"

// Because ain't nobody got time for compiler warnings
#define _arrayptrandlength(data) data, (int)LengthOfArray(data)
#define _arraylengthandptr(data) (int)LengthOfArray(data), data

#define EXPORT __declspec(dllexport)

extern int SegaVoiceLanguage;
extern int VoiceLanguage_sel;
extern bool AlternateEggman;
extern float HorizontalResolution_float;
extern float VerticalResolution_float;