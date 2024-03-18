#pragma once
class Client
{
public:
	static void ApplyMods();
	static void EnableNewIGCipher();
	static void UpdateLogin();
	static void UpdateResolution();

	static const int m_nIGCipherHash = 0xC65053F2;
	static int m_nGameHeight;
	static int m_nGameWidth;
};