#pragma once
#include "../src/WinMain.h"

extern int isPaused;

class Time
{
private:
	unsigned int ms = 0;
	unsigned int total_ms = 0;
	unsigned int second = 0;
	unsigned int total_second = 0;
	unsigned int minute = 0;


public:
	double deltaTime_ms = 0;
	float deltaTime = 0;
	double currentTime = 0;
	double lastFrame = 0;

	void UpdateDeltaTime()
	{
		currentTime = static_cast<double>(GetNowCount());
		deltaTime_ms = (currentTime - lastFrame);
		deltaTime = static_cast<float>(deltaTime_ms / 1000);
		lastFrame = currentTime;
	}

	void UpdateTick()
	{
		UpdateDeltaTime();
		ms += static_cast<int>(deltaTime_ms);
		total_ms+= static_cast<int>(deltaTime_ms);
		if (ms > 1000)
		{
			ms = 0;
			total_second++;
			second++;
		}
		if (second > 60)
		{
			second = 0;
			minute++;
		}
	}

	int GetGameSecond()
	{
		return second;
	}
	double GetGameMS()
	{
		return ms;
	}
	int GetGameMinute()
	{
		return minute;
	}
	int GetTotalGameSecond()
	{
		return total_second;
	}
	double GetTotalGameMS()
	{
		return total_ms;
	}

};

extern Time time;

class Timer
{
public:
	//use New
	Timer( )
	{
	}

	~Timer()
	{
	}

	bool IsTimerSec(float _second)
	{
		Update();
		if (timer_sec >= _second) 
		{ return true; }
		return false;
	}

	void Update()
	{
		timer_ms += time.deltaTime_ms;
		if (timer_ms >= 100)
		{
			timer_sec+=0.1f;
		}
		if (timer_ms >= 1000)
		{
			timer_ms = 0;
		}
	}

	float GetTimerSec()
	{
		return timer_sec;
	}
		
	double GetTimerMS()
	{
		return timer_ms;
	}

	void Reset()
	{
		timer_ms = 0;
		timer_sec = 0;
	}

	void SimpleAnimIndex(int& index, int MAX_index, float _simpledelay = 2.0f)
	{
		if (timer_sec >= _simpledelay)
		{
			if (index >= MAX_index - 1)
			{
				index = 0;
			}
			else
			{
				index += 1;
			}
			timer_sec = 0;
		}
	}


private:
		double timer_ms = 0;
		float timer_sec = 0;
};


