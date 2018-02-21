#include "stdafx.h"
#include <SADXModLoader.h>
#include <random>

// TODO: Adjust for longer Japanese Big voice clips.

int CharacterVoice = 0;
int VoiceLanguage_sel = 1;
bool AlternateEggman   = false;

void PlaySegaSonicTeamVoice_init()
{
	std::random_device r;
	std::mt19937 mt(r());
	std::uniform_int_distribution<int> voice(0, 7);
	std::uniform_int_distribution<int> eggman(0, 1);
	CharacterVoice = voice(mt);
	AlternateEggman = eggman(mt) == 1;
	if (SegaVoiceLanguage == 2)
	{
		VoiceLanguage_sel = 0;
		if (CharacterVoice == 5)
		{
			WriteData<1>((char*)0x0042CC9C, 0xFF);
			WriteData<1>((char*)0x0042CD0E, 0xFF);
		}
	}
}

// SEGA, Sonic Team
static int voices[8][2] =
{
	{ 1994, 1995 },	// Sonic
	{ 1996, 1997 },	// Tails
	{ 1998, 1999 },	// Knuckles
	{ 2000, 2001 },	// Amy
	{ 2002, 2003 },	// Big
	{ 2004, 2005 },	// Gamma
	{ 2006, 2008 },	// Eggman
	{ 2010, 2011 }	// Tikal
};

static int GetVoiceNumber(int value)
{
	int result = voices[CharacterVoice][value];
	return (CharacterVoice == 6 && AlternateEggman) ? ++result : result;
}

static void PlaySegaSonicTeamVoice()
{
	if (SegaLogo_Frames == 1 && SoundManager_ptr == nullptr)
	{
		SoundManager_Load(LoadObject((LoadObj)0, 1, SoundManager_Load));
	}

	if (SegaLogo_Frames != 30)
	{
		return;
	}

	if (SoundManager_ptr != nullptr)
	{
		switch (SegaLogo_Mode)
		{
		case 1:
			VoiceLanguage = VoiceLanguage_sel;
			PlayVoice(GetVoiceNumber(0));
			SoundManager_ptr->MainSub(SoundManager_ptr);
			break;

		case 16:
			PlayVoice(GetVoiceNumber(1));
			SoundManager_ptr->MainSub(SoundManager_ptr);
			VoiceLanguage = 1;
			break;

		default:
			break;
		}
	}
	else
	{
		PrintDebug("Error initializing Sound Manager.\n");
	}
}


void __declspec(naked) PlaySegaSonicTeamVoice_asm()
{
	__asm
	{
		call DisplayLogoScreen
		add esp, 4
		call PlaySegaSonicTeamVoice
		xor eax, eax
		retn
	}
}
