#pragma once

#include <random>

#include <SADXModLoader.h>
#include <IniFile.hpp>
#include <Trampoline.h>

#include "ItemBox.h"
#include "PlaySegaSonicTeamVoice.h"
#include "PerfectChaosFixes.h"

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target())
#define TARGET_STATIC(name) ((decltype(name##_r)*)name##_t.Target())
#define EXPORT __declspec(dllexport)
