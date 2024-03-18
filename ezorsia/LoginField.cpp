#include "stdafx.h"
#include "LoginField.h"
#include "Client.h"

#pragma region login
const DWORD dwLoginCreateDlgRtn = 0x006203F6;

__declspec(naked) void PositionLoginDlg()
{
	__asm {
		push 0x000000B4
		push 400
		push - 48 // y
		push - 185 // x
		jmp dword ptr[dwLoginCreateDlgRtn]
	}
}

const DWORD dwLoginUsernameRtn = 0x006209B1;

__declspec(naked) void PositionLoginUsername()
{
	__asm {
		push 0x0F
		push 0x00000084
		push 127 // y
		push 0 // x
		jmp dword ptr[dwLoginUsernameRtn]
	}
}

const DWORD dwLoginPasswordRtn = 0x00620A15;

__declspec(naked) void PositionLoginPassword()
{
	__asm {
		push 0x0F
		push 0x78
		push 127 // y
		push 272 // x
		jmp dword ptr[dwLoginPasswordRtn]
	}
}
#pragma endregion

void LoginField::UpdateResolution()
{
	//Memory::CodeCave(PositionLoginDlg, 0x006203E8, 14);
	//Memory::CodeCave(PositionLoginUsername, 0x006209A6, 11);
	//Memory::CodeCave(PositionLoginPassword, 0x00620A0D, 8);

	//Memory::WriteInt(0x0062094F + 3, 0xFFF8FAFF);	// ARGB value ; Input background color
	//Memory::WriteByte(0x00620930 + 3, 1);		// bool: true=black, false=white ; Input font Color
	//Memory::WriteInt(0x00620644 + 1, -127);	// x-pos ; login button
	//Memory::WriteInt(0x00620917 + 1, -127);	// x-pos ; quit button
	//Memory::WriteByte(0x006206BE + 1, -127);	// x-pos ; save id button

	Memory::WriteInt(0x00620735 + 1, -127); // x-pos ; find id button 
	Memory::WriteInt(0x006207AF + 1, -127); // x-pos ; find password button
	Memory::WriteByte(0x006208A0 + 1, -127); // x-pos ; web button
	Memory::WriteByte(0x00620829 + 1, -127); // x-pos ; register button

	// Login error notice dialogue
	Memory::WriteInt(0x0060F79B + 1, (Client::m_nGameHeight / 2) - (324 / 2)); // animate x
	Memory::WriteInt(0x0060F7A4 + 1, (Client::m_nGameHeight / 2) - (324 / 2)); // x
	Memory::WriteInt(0x0060F7AC + 1, (Client::m_nGameWidth / 2) - (204 / 2)); // animate y
	Memory::WriteInt(0x0060F7B5 + 1, (Client::m_nGameWidth / 2) - (204 / 2)); // y

	//Memory::WriteInt(0x00604121 + 1, -(Client::m_nGameHeight + 590)); // Character Y; FFFFFB3E (217)
	//Memory::WriteInt(0x00604126 + 1, -(Client::m_nGameWidth - 1248)); // Character X; 0FFFFFEDE (121)
}
