/*
==================================================================================
cSoundMgr.cpp
==================================================================================
*/

#include "SoundMgr.h"

SoundMgr* SoundMgr::pInstance = NULL;

/*
=================================================================================
Constructor
=================================================================================
*/
SoundMgr::SoundMgr()
{
	createContext();
}

/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
SoundMgr* SoundMgr::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new SoundMgr();
	}
	return SoundMgr::pInstance;
}

void SoundMgr::createContext()
{
	m_OALDevice = alcOpenDevice(NULL);
	if (m_OALDevice)
	{
		//Create a context
		m_OALContext = alcCreateContext(m_OALDevice, NULL);

		//Set active context
		alcMakeContextCurrent(m_OALContext);
	}
}

void SoundMgr::add(LPCSTR sndName, LPCSTR fileName)
{
	if (!getSnd(sndName))
	{
		Sound * newSnd = new Sound();
		newSnd->loadWAVFile(fileName);
		gameSnds.insert(make_pair(sndName, newSnd));
	}
}

Sound* SoundMgr::getSnd(LPCSTR sndName)
{
	map<LPCSTR, Sound*>::iterator snd = gameSnds.find(sndName);
	if (snd != gameSnds.end())
	{
		return snd->second;
	}
	else
	{
		return NULL;
	}
}

void SoundMgr::deleteSnd()
{
	for (map<LPCSTR, Sound*>::iterator snd = gameSnds.begin(); snd != gameSnds.end(); ++snd)
	{
		delete snd->second;
	}
}


SoundMgr::~SoundMgr()
{
	m_OALContext = alcGetCurrentContext();

	//Get device for active context
	m_OALDevice = alcGetContextsDevice(m_OALContext);

	//Release context(s)
	alcDestroyContext(m_OALContext);

	//Close device
	alcCloseDevice(m_OALDevice);

}
