/*
==========================================================================
cSound.cpp
==========================================================================
*/
#pragma warning ( disable : 4996 )
#include "Sound.h"
using namespace std;

Sound::Sound()
{
	m_OALData = NULL;           //data for the buffer
	m_OALFrequency = 0;     //frequency
	m_OALBitRate = 0;		  // Bit Rate
	m_OALLength = 0;		  // Length
	m_OALBuffer = 0;         // Buffer

}

Sound::~Sound()
{
	InputAction* iaS = Input::GetInputActionState("Mute");
	if (iaS)
		__unhook(&InputAction::InputActionChange, iaS, &Sound::Mute);
	cleanUp();
}

Sound::Sound(LPCSTR filename)
{
	m_OALData = NULL;           //data for the buffer
	m_OALFrequency = 0;     //frequency
	m_OALBitRate = 0;		  // Bit Rate
	m_OALLength = 0;		  // Length
	m_OALBuffer = 0;         // Buffer

	loadWAVFile(filename);
}

void Sound::loadWAVFile(LPCSTR filename)
{
	// Check for EAX 2.0 support
	m_OALbEAX = alIsExtensionPresent("EAX2.0");
	// Generate Buffers
	alGetError(); // clear error code

	//load the wave file
	alutLoadWAVFile((ALbyte *)filename, &m_OALFormat, (void **)&m_OALData, (ALsizei *)&m_OALBufferLen, &m_OALFrequency, &m_alLoop);

	//create a source
	alGenSources(1, &m_OALSource);

	//create  buffer
	alGenBuffers(1, &m_OALBuffer);

	//put the data into our sampleset buffer
	alBufferData(m_OALBuffer, m_OALFormat, m_OALData, m_OALBufferLen, m_OALFrequency);

	//assign the buffer to this source
	alSourcei(m_OALSource, AL_BUFFER, m_OALBuffer);

	//release the data
	alutUnloadWAV(m_OALFormat, m_OALData, m_OALBufferLen, m_OALFrequency);

	InputAction* iaS = Input::GetInputActionState("Mute");
	if (iaS)
		__hook(&InputAction::InputActionChange, iaS, &Sound::Mute);
}

void Sound::LoadWAVInfo(ifstream &filename, string &name, 	unsigned int &size)
{
	char chunk[4];
	filename.read((char *)&chunk, 4);
	filename.read((char *)&size, 4);

	name = string(chunk, 4);
}

void Sound::playAudio(ALboolean sndLoop)
{
	if (_muted) return;
	alSourcei(m_OALSource, sndLoop, AL_TRUE);

	//play
	alSourcePlay(m_OALSource);
}

void Sound::stopAudio()
{
	//to stop
	alSourceStop(m_OALSource);
}

void Sound::Mute(bool state)
{
	if (!state) return;

	_muted = !_muted;
	if (_muted)
	{
		alSourcef(m_OALSource, AL_GAIN, 0.0f);
	}
	else alSourcef(m_OALSource, AL_GAIN, 1.0f);
}

void Sound::cleanUp()
{
	int state;
	alGetSourcei(m_OALSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING || state != AL_STOPPED)
	{
		stopAudio();
	}

	alDeleteSources(1, &m_OALSource);

	//delete our buffer
	alDeleteBuffers(1, &m_OALBuffer);

	//Disable context
	alcMakeContextCurrent(NULL);
}

int Sound::GetState()
{
	int state;
	alGetSourcei(m_OALSource, AL_SOURCE_STATE, &state);
	return state;
}
