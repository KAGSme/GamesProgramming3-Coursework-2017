#include "TimeDay.h"



TimeDay::TimeDay(Font* f)
{
	timeHour = 12;
	timeMinute = 0;
	timeScale = 30;
	font = f;
}


TimeDay::~TimeDay()
{
}

void TimeDay::Update(float deltaTime)
{
	timeMinute += (deltaTime * timeScale);

	if (timeMinute > 59.99)
	{
		timeMinute = 0;
		timeHour++;
		if (timeHour > 23) timeHour = 0;
	}
	font->Render(GetTimeString(), { 0, 5, 90, 50 });
}

void TimeDay::OnRender(Camera * camera)
{
}

void TimeDay::SetTimeScale(float tScale)
{
	timeScale = tScale;
}

void TimeDay::SetTime(int hour, float minute = 0)
{
	timeHour = hour;
	timeMinute = minute;
	if (timeHour < 0 || timeMinute < 0) 
	{
		timeHour = 0;
		timeMinute = 0;
	}
}

string TimeDay::GetTimeString()
{
	stringstream ss;
	ss << setw(2) << timeHour << " :" << ss.fill('0') << setw(2) << int(timeMinute);
	return ss.str();
}

