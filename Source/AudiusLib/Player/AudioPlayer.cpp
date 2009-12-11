#include "Precompiled.h"
#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(void)
{
}

AudioPlayer::~AudioPlayer(void)
{
}

void AudioPlayer::initialise()
{
	Logger::writeToLog(T("Initializing player..."));
}

// Singleton impl
juce_ImplementSingleton(AudioPlayer)
