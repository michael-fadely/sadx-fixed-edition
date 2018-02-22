#pragma once

void PlaySegaSonicTeamVoice_init();
void PlaySegaSonicTeamVoice_asm();

enum class SegaVoiceLanguage_t
{
	Off,
	English,
	Japanese
};

extern int CharacterVoice;
extern SegaVoiceLanguage_t SegaVoiceLanguage;
extern int VoiceLanguage_sel;
extern bool AlternateEggman;
