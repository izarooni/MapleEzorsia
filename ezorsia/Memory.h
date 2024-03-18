#pragma once

class Memory
{
public:
	enum class ASM
	{
		nop = 0x90,
		jump = 0xE9,
		jump_short = 0xEB,
		jump_not_equal = 0x75,
		jump_le = 0x8E
	};

	static bool SetHook(bool attach, void** ptrTarget, void* ptrDetour);
	static void FillBytes(DWORD dwOriginAddress, unsigned char ucValue, int nCount);
	static void WriteString(DWORD dwOriginAddress, const char* sContent, size_t nSize);
	static void WriteByte(DWORD dwOriginAddress, unsigned char ucValue);
	static void WriteBytes(DWORD dwOriginAddress, ...);
	static void WriteShort(DWORD dwOriginAddress, unsigned short usValue);
	static void WriteInt(DWORD dwOriginAddress, unsigned int dwValue);
	static void CodeCave(void* ptrCodeCave, DWORD dwOriginAddress, int nNOPCount);
};

