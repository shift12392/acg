#pragma once
class GameTimer
{
private:
	double m_SecondsPerCount = 0.0f;
	double m_DeltaTime = 0.0f;
	bool m_bStoped = true;

	LARGE_INTEGER m_PrevCount = {};
	LARGE_INTEGER m_CurrentCount = {};
	LARGE_INTEGER m_BaseCount = {};
	LARGE_INTEGER m_PausedCount = {};          //暂停的总计数
	LARGE_INTEGER m_StopCount = {};

public:
	GameTimer();
	~GameTimer();

	void Reset();
	void Start();
	void Stop();
	double Tick();  // Call every frame.
};

