#pragma once
class Discord
{
public:
	static void StartThread();
	static void Login();
	static void Update();

	static LPCSTR m_sApplicationID;
	static time_t m_nStartTimestamp;
};

