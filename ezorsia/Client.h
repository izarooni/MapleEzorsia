#pragma once
class Client
{
public:
	static void EnableNewIGCipher();
	static void UpdateResolution();
	static void UpdateLogin();
	static const int m_nIGCipherHash = 0XC65053F2;
	static int m_nGameHeight;
	static int m_nGameWidth;
};