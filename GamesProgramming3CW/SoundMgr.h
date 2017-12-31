/*
==========================================================================
cSoundMgr.h
==========================================================================
*/

#ifndef _SOUNDMGR_H
#define _SOUNDMGR_H

// OpenGL Headers
#include "Common.h"
#include "Sound.h"


using namespace std;

class SoundMgr
{
private:
	static SoundMgr* pInstance;
	ALCcontext *m_OALContext;
	ALCdevice *m_OALDevice;

protected:
	SoundMgr();
	~SoundMgr();
	map <LPCSTR, Sound*> gameSnds;
	void createContext();

public:
	void add(LPCSTR sndName, LPCSTR fileName);
	Sound* getSnd(LPCSTR sndName);
	void deleteSnd();
	static SoundMgr* getInstance();
};
#endif