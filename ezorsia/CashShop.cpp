#include "stdafx.h"
#include "CashShop.h"

// calculation is relative to the sizing of the native v83 cash shop base background.
// if the background is customized, you will probably have to change a lot of values.
int responsive_x, responsive_y;

#pragma region cash shop updates
DWORD dwStorageWallReturn = 0x00469436;
__declspec(naked) void StorageWall() {
	__asm {
		push 104
		push 255
		jmp dword ptr[dwStorageWallReturn]
	}
}

DWORD dwPosPreviewBGReturn = 0x004AB120;
__declspec(naked) void PosPreviewBG() {
	__asm {
		mov eax, responsive_y
		add eax, 40 // offset from the top of the base background
		push eax

		mov eax, responsive_x
		add eax, 24 // offset from the left of the base background
		push eax

		lea eax, [ebp + 0x8]
		jmp dword ptr[dwPosPreviewBGReturn]
	}
}

DWORD dwPosTopLeftReturn = 0x00469426;
__declspec(naked) void PosTopLeft() {
	__asm {
		mov eax, responsive_y
		add eax, 1 // asset has 1px blank space at the top
		push eax

		push responsive_x

		lea ecx, [esi + 0x00000554]
		jmp dword ptr[dwPosTopLeftReturn]
	}
}

DWORD dwPosItemsListReturn = 0x00469499;
__declspec(naked) void PosItemsList() {
	__asm {
		mov eax, responsive_y
		add eax, 95 // offset from the top of the base background
		push eax

		mov eax, responsive_x
		add eax, 277 // offset from the left of the base background
		push eax

		jmp dword ptr[dwPosItemsListReturn]
	}
}

DWORD dwPosBtSearchReturn = 0x004694F3;
__declspec(naked) void PosBtSearch() {
	__asm {
		mov eax, responsive_y
		add eax, 98 // offset from the top of the base background
		push eax

		mov eax, responsive_x
		// 691 = 800 (width of base background) - 109 (width of the button + margin)
		add eax, 691
		push eax
		jmp dword ptr[dwPosBtSearchReturn]
	}
}

DWORD dwPosCashStorageReturn = 0x00469443;
__declspec(naked) void PosCashStorage() {
	__asm {
		mov eax, responsive_x
		push eax
		lea ecx, [esi + 0x000009A4]
		jmp dword ptr[dwPosCashStorageReturn]
	}
}

DWORD dwPosUserInventoryReturn = 0x00469462;
__declspec(naked) void PosUserInventory() {
	__asm {
		push responsive_x
		lea ecx, [esi + 0x00000F40]
		jmp dword ptr[dwPosUserInventoryReturn]
	}
}

DWORD dwNavBarReturn = 0x00469479;
_declspec(naked) void PosNavBar()
{
	__asm {
		mov eax, responsive_y
		add eax, 17
		push eax

		mov eax, responsive_x
		add eax, 274
		push eax

		jmp dword ptr[dwNavBarReturn]
	}
}
#pragma endregion

void CashShop::UpdateResolution(int game_width, int game_height) {
	// calculate the top-left origin of the base background using the native base background dimensions
	responsive_x = (game_width - 800) / 2;
	responsive_y = (game_height - 600) / 2;

	Logger::Info("[CashShp] Responsive X: %d, Y: %d", responsive_x, responsive_y);

	Memory::CodeCave(StorageWall, 0x0046942F, 7);
	Memory::CodeCave(PosPreviewBG, 0x004AB119, 7);
	Memory::CodeCave(PosTopLeft, 0x0046941E, 8);
	Memory::CodeCave(PosItemsList, 0x00469492, 7);
	Memory::CodeCave(PosBtSearch, 0x004694EC, 7);
	Memory::CodeCave(PosCashStorage, 0x0046943B, 8);
	Memory::CodeCave(PosUserInventory, 0x0046945B, 7);
	Memory::CodeCave(PosNavBar, 0x00469472, 7);

	Memory::WriteInt(0x004AB10F + 1, 175);	// Preview Background Wall S
	Memory::WriteInt(0x004AB114 + 1, 225);	// Preview Background Wall E
	Memory::WriteInt(0x00469414 + 1, 310);	// 3 Buttons, Chatbar, IGN Wall S
	Memory::WriteInt(0x00469419 + 1, 255);	// 3 Buttons, Chatbar, IGN Wall E
	Memory::WriteInt(0x00469488 + 1, 426);	// Items List Wall Y
	Memory::WriteInt(0x0046948D + 1, 412);	// Items List Wall X
	Memory::WriteByte(0x004BA74B + 2, 53);	// BtBuy Y
	Memory::WriteByte(0x004BA75F + 2, 78);	// BtBuy X
	Memory::WriteByte(0x004BA7F1 + 2, 53);	// BtGift Y
	Memory::WriteByte(0x004BA7F9 + 2, 118);	// BtGift X
	Memory::WriteByte(0x004BA8EB + 2, 53);	// BtReserve Y
	Memory::WriteInt(0x004BA8F3 + 2, 158);	// BtReserve X
	Memory::WriteInt(0x0046944C + 1, 163);	// Item Inv, Item Tabs Wall S
	Memory::WriteInt(0x00469451 + 1, 255);	// Item Inv, Item Tabs Wall E

	// 157 = offset from the top of the base background
	Memory::WriteInt(0x004694AF + 1, responsive_y + 157);	// "Hot" Items Y
	// 690 = offset from the left of the base background
	Memory::WriteInt(0x004694BA + 1, responsive_x + 690);	// "Hot" Items X

	// NX values & 4 buttons (charge, check cash, code and exit) 547x64
	Memory::WriteInt(0x004694CF + 1, responsive_y + (600 - 70));	//Y
	Memory::WriteInt(0x004694D4 + 1, responsive_x + (800 - 546));	//X

	// CCSWnd_Locker::OnCreate; storage
	// 281 = offset from bottom of base background + 96 = height of the storage
	Memory::WriteInt(0x00469436 + 1, responsive_y + 600 - 281);	// Origin Y

	// CCSWnd_Inventory::OnCreate; inventory
	// 600 = height of the base background, 175 = height of the inventory including margin and base background border offset
	Memory::WriteInt(0x00469456 + 1, responsive_y + 600 - 175);	// Origin Y
	Logger::Info("[CashShp] Origins of interactive elements adjusted");
}