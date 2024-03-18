#include "stdafx.h"
#include "Client.h"
#include "CashShop.h"
#include "LoginField.h"
#include "Logger.h"

int Client::m_nGameWidth = 1024;
int Client::m_nGameHeight = 768;
int nStatusBarY = Client::m_nGameHeight - 578;

#pragma region status bar
const DWORD dwStatusBarPosRetn = 0x008CFD5A;
__declspec(naked) void AdjustStatusBar() {
	__asm {
		push nStatusBarY
		push ebx // horizontal position; 0
		mov ecx, esi
		jmp dword ptr[dwStatusBarPosRetn]
	}
}
const DWORD dwStatusBarBackgroundPosRetn = 0x008D1F6A;
__declspec(naked) void AdjustStatusBarBG() {
	__asm {
		push nStatusBarY
		movsd
		push 0
		jmp dword ptr[dwStatusBarBackgroundPosRetn]
	}
}
const DWORD dwStatusBarInputPosRetn = 0x008D2185;
__declspec(naked) void AdjustStatusBarInput() {
	__asm {
		push nStatusBarY
		push edi
		lea ecx, [esi + 0x0CD0]
		jmp dword ptr[dwStatusBarInputPosRetn]
	}
}
#pragma endregion

const DWORD dwTranslateMessageRtn = 0x009E8092;
const DWORD dwTranslateMessageFix = 0x009E809F;
_declspec(naked) void ApplyCursorVectorPosScrollFix() {
	__asm {
		cmp eax, 0x20A // WM_MOUSEWHEEL
		mov edi, [ebp + 0x10]
		mov eax, [edi]
		je DontDraw
		jmp dword ptr[dwTranslateMessageRtn]
		DontDraw:
		jmp dword ptr[dwTranslateMessageFix]
	}
}

const DWORD dwCharacterHairSP = 0x005C958D;
const DWORD dwGetEquipDataPathRtn = 0x005C9505;
_declspec(naked) void ApplyHairRangeFix()
{
	__asm
	{
		cmp eax, 3
		jz fix_hair
		cmp eax, 4
		jz fix_hair
		cmp eax, 6
		jz fix_hair
		jmp original

		fix_hair:
		jmp dword ptr[dwCharacterHairSP]
		original:
		jmp dword ptr[dwGetEquipDataPathRtn]
	}
}

void Client::EnableNewIGCipher() {
	const int nCipherHash = m_nIGCipherHash;
	Memory::WriteInt(0x00A4A845 + 3, nCipherHash);
	Memory::WriteInt(0x00A4A8DA + 3, nCipherHash);
	Memory::WriteInt(0x00A4A9BF + 3, nCipherHash);
	Memory::WriteInt(0x00A4A942 + 3, nCipherHash);
	Memory::WriteInt(0x00A4A9F7 + 3, nCipherHash);
}

void Client::UpdateResolution() {
	nStatusBarY = m_nGameHeight - 578;

	LoginField::UpdateResolution();
	CashShop::UpdateResolution(m_nGameWidth, m_nGameHeight);

	Memory::CodeCave(AdjustStatusBar, 0x008CFD55, 5);
	Memory::CodeCave(AdjustStatusBarBG, 0x008D1F65, 5);
	Memory::CodeCave(AdjustStatusBarInput, 0x008D217C, 9);

	Memory::WriteInt(0x009F7B1D + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x009F7B23 + 1, m_nGameWidth);	//push 800 ; CWvsApp::InitializeGr2D
	Memory::WriteInt(0x0059A15D + 2, floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x0059A169 + 2, floor(-m_nGameWidth / 2));	//push -400 ; CInputSystem::SetCursorVectorPos
	Memory::WriteInt(0x0059AC22 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0059AC09 + 1, m_nGameWidth);	//mov ecx,800 ; CInputSystem::UpdateMouse
	Memory::WriteInt(0x0059A8B1 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0059A898 + 1, m_nGameWidth);	//mov eax,800 ; CInputSystem::SetCursorPos
	Memory::WriteInt(0x009DFCF0 + 3, m_nGameHeight);//lea eax,[esi+eax-600]
	Memory::WriteInt(0x009DFE68 + 3, m_nGameWidth);	//lea eax,[ecx+eax-800]
	Memory::WriteInt(0x009E2E85 + 1, floor(-m_nGameHeight / 2));	//push -300
	Memory::WriteInt(0x009E2E8B + 1, floor(-m_nGameWidth / 2));	//push -400 ;

	Memory::WriteInt(0x008F32DF + 1, m_nGameHeight); //mov eax,599 ; CUIToolTip::MakeLayer
	Memory::WriteInt(0x008F32CC + 1, m_nGameWidth); //mov eax,799 ; CUIToolTip::MakeLayer

	Memory::WriteInt(0x007B2EA0 + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::ShowToolTip
	Memory::WriteInt(0x007B3087 + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::FindIcon
	Memory::WriteInt(0x007B2C97 + 2, m_nGameHeight / 2 - 23);	//sub ebx,277 ; Skill icon buff y-pos
	Memory::WriteInt(0x007B2CB5 + 3, m_nGameWidth / 2 - 3);	//lea eax,[eax+esi+397] ; Skill icon buff x-pos
	Memory::WriteInt(0x007B2DA0 + 2, m_nGameHeight / 2 - 23);	//sub ebx,277 ; Skill icon cooltime y-pos
	Memory::WriteInt(0x007B2DBE + 3, m_nGameWidth / 2 - 3);	//lea eax,[eax+esi+397] ; Skill icon cooltime x-pos

	Memory::WriteInt(0x008D1793 + 1, m_nGameHeight + 1);//add eax,533
	Memory::WriteInt(0x008DF782 + 2, m_nGameHeight + 1);//add esi,533
	Memory::WriteInt(0x008D179A + 1, 799); //push 647
	Memory::WriteInt(0x008DF7F8 + 1, 799); //push 647
	Memory::WriteInt(0x008DE8EE + 2, (-m_nGameWidth + 32) / 2); //lea edi,[eax-647]
	Memory::WriteInt(0x008DE8E5 + 2, -m_nGameWidth + 228); //lea ebx,[eax-427]

	Memory::WriteInt(0x0043717B + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00437181 + 1, m_nGameWidth);	//mov esi,800 ; CreateWnd
	Memory::WriteInt(0x0053808B + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x00538091 + 1, m_nGameWidth);	//push 800 ; RelMove?
	Memory::WriteInt(0x004CC160 + 1, m_nGameWidth);	//mov [ebp-16],800 ; CreateWnd
	Memory::WriteInt(0x004CC2C5 + 2, m_nGameHeight);//cmp ecx,600
	Memory::WriteInt(0x004CC2B0 + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x004D59B2 + 1, m_nGameHeight);//mov eax,800
	Memory::WriteInt(0x004D599D + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x0085F36C + 2, m_nGameWidth);	//cmp edx,800
	Memory::WriteInt(0x0085F374 + 1, m_nGameWidth - 80);	//mov ecx,720 ; CreateDlg
	Memory::WriteInt(0x008EBC58 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x008EBC3C + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x009966B5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009966CA + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009966D2 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg
	Memory::WriteInt(0x009A3E7F + 1, m_nGameHeight);//mov edx,600
	Memory::WriteInt(0x009A3E72 + 1, m_nGameWidth);	//mov edx,800 ; CreateDlg
	Memory::WriteInt(0x0045B898 + 1, m_nGameHeight - 25);	//push 575
	Memory::WriteInt(0x0045B97E + 1, m_nGameWidth);	//push 800 ; RelMove?
	Memory::WriteInt(0x004D9BD1 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x004D9C37 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x005386F0 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x005386F5 + 1, m_nGameWidth);	//push 800 ; CField::DrawFearEffect
	Memory::WriteInt(0x0055B808 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0055B80D + 1, m_nGameWidth);	//mov edi,800
	Memory::WriteInt(0x0055B884 + 1, m_nGameWidth);	//push 600 ; RelMove?
	Memory::WriteInt(0x007E15BE + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x007E16B9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007E16BE + 1, m_nGameWidth);	//push 800 ; CWnd::GetCanvas
	Memory::WriteInt(0x008AA266 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x008AA26B + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x009F6E99 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x009F6EA0 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x005A8B46 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A8B56 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005A9B42 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A9B52 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005AADAA + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005AADBA + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ABC65 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ABC75 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ACB29 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ACB39 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C187E + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C188E + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C2D62 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C2D72 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x007CF48F + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x007CF49D + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008A12F4 + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x008A1302 + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x0062FC4A + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FC4F + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x0062FE63 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FE68 + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x007F257E + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007F258F + 1, m_nGameWidth);	//push 800 ; CWnd::CreateWnd
	Memory::WriteInt(0x0062F9C6 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062F9CB + 1, m_nGameWidth);	//push 800; (UI/Logo/Wizet)
	Memory::WriteInt(0x0062F104 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0062F109 + 1, m_nGameWidth);	//mov eax,800 ; Rectangle
	Memory::WriteInt(0x006406D5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006406C3 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064050A + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006404F8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640618 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640690 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064061D + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064064B + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640606 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064067E + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640639 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064043E + 1, floor(m_nGameWidth / 2));	//mov edi,400
	Memory::WriteInt(0x00640443 + 1, floor(m_nGameHeight / 2));	//mov esi,300
	Memory::WriteInt(0x00640626 + 1, floor(m_nGameWidth / 2));	//add eax,400 ; bunch of modulus stuff
	Memory::WriteInt(0x00641A19 + 3, m_nGameHeight);//mov [ebp+28],600
	Memory::WriteInt(0x00641A12 + 3, m_nGameWidth);	//mov [ebp+32],800 ; idk
	Memory::WriteInt(0x00641B38 + 3, m_nGameHeight);//mov [ebp-32],600
	Memory::WriteInt(0x00641B2E + 3, m_nGameWidth);	//mov [ebp-36],800 ; CAnimationDisplayer::SetCenterOrigin
	Memory::WriteInt(0x0046B85C + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0046B86A + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x009994D8 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009994ED + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009994F5 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg
	Memory::WriteInt(0x00641FC8 + 1, floor(m_nGameHeight / 2));	//add eax,300  ; VRRight
	Memory::WriteInt(0x0064208F + 1, floor(m_nGameHeight / 2));	//sub eax,300
	Memory::WriteInt(0x00641F61 + 1, floor(m_nGameWidth / 2));	//mov ebc,400 ;  VRTop
	Memory::WriteInt(0x006CD842 + 1, floor(m_nGameWidth / 2));	//push 400 ; RelMove?
	Memory::WriteInt(0x0059A0A2 + 6, floor(m_nGameHeight / 2));	//mov [ebx+2364],300 ; CMapLoadable::RestoreViewRange
	Memory::WriteInt(0x0059A09C + 2, floor(m_nGameWidth / 2));	//mov [esi],400	; CInputSystem::LoadCursorState
	Memory::WriteInt(0x0080546C + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00805459 + 1, m_nGameWidth);	//mov edx,800 ; CUIEventAlarm::CreateEventAlarm
	Memory::WriteInt(0x008CFD4B + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008CFD50 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x0053836D + 1, floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x00538373 + 1, floor(-m_nGameWidth / 2));	//push -400	; RelMove?
	Memory::WriteInt(0x0055BB2F + 1, floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x0055BB35 + 1, floor(-m_nGameWidth / 2));	//push -400 ; RelMove?
	Memory::WriteInt(0x005F481E + 1, floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x005F4824 + 1, floor(-m_nGameWidth / 2));	//push -400 ; RelMove?
	Memory::WriteInt(0x004372B1 + 1, floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x004372B6 + 1, floor(-m_nGameWidth / 2));	//push -400 ; RelMove?
	Memory::WriteInt(0x006CE3AB + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x007E1CB7 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D82F5 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D82FA + 1, m_nGameWidth);	//push 800 ; CreateWnd?
	Memory::WriteInt(0x00935870 + 1, floor(m_nGameHeight / 2));	//push 300
	Memory::WriteInt(0x0093586B + 1, m_nGameWidth);	// push 800 ; RelMove? (Skills)
	Memory::WriteInt(0x009DFD5C + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009DFED2 + 1, m_nGameHeight);//mov ecx,600	; IWzVector2D::RelMove
	Memory::WriteInt(0x009F6ADD + 1, floor(m_nGameHeight / 2)); //push 300 ; MapleStoryClass
	Memory::WriteInt(0x006D50D8 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074BAA9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B951 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B4A2 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B3B7 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x006421B3 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM
	Memory::WriteInt(0x0060411C + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x005E3FA0 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0059EB49 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM
	Memory::WriteInt(0x008D247B + 1, m_nGameHeight - 33);	//push 567 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008DEB93 + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008DEE2F + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008D2765 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D29B4 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D8BFE + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D937E + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D9AC9 + 1, m_nGameHeight - 19);	//push  
	Memory::WriteInt(0x008D1D50 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1D55 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D1FF4 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1FF9 + 1, m_nGameWidth);	//push 800 ; CUIStatusBar
	Memory::WriteInt(0x0062F5DF + 1, m_nGameHeight);	//push 600
	Memory::WriteInt(0x0062F5E4 + 1, m_nGameWidth);	//push 800 ; (UI/Logo/Nexon)
	Memory::WriteInt(0x004EDB89 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x004EDB78 + 1, m_nGameHeight);	//mov ecx,600 ; CreateWnd
	Memory::WriteInt(0x004EDABF + 1, m_nGameHeight); 	//mov eax,600 ; CDialog::CreateDlg_0
	Memory::WriteInt(0x004EDAD8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009F7079, m_nGameHeight);		// dd 600
	Memory::WriteInt(0x009F707E, m_nGameWidth);		// dd 800
	Memory::WriteInt(0x00BE2738, floor(m_nGameWidth / 2));	// dd 400 ; CUIEventAlarm__CreateEventAlarm
	Memory::WriteInt(0x00BE2DF4, floor(m_nGameHeight / 2));	// dd 300
	Memory::WriteInt(0x00BE2DF0, floor(m_nGameWidth / 2));	// dd 400 ; CUtilDlgEx__CreateUtilDlgEx
	Memory::WriteInt(0x00640656 + 2, floor(-m_nGameWidth / 2));	//add edi,-400 ;
	Memory::WriteInt(0x00641048 + 1, floor(-m_nGameHeight / 2));	//mov esi,-300
	Memory::WriteInt(0x00641050 + 1, floor(-m_nGameWidth / 2));	//mov esi,-400 ;
	Memory::WriteInt(0x006CE4C6 + 1, floor(-m_nGameWidth / 2));	//push -400 ;
	Memory::WriteInt(0x005F64DE + 1, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x0093519A + 1, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00954433 + 1, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00981555 + 1, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00981F7A + 2, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00A448B0 + 2, floor(-m_nGameHeight / 2));	//push -300 ; CWvsPhysicalSpace2D::Load
	Memory::WriteInt(0x005F6627 + 1, floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x0066BACE + 2, floor(-m_nGameWidth / 2));	//and ecx,-400
	Memory::WriteInt(0x009B76BD + 3, floor(-m_nGameHeight / 2));	//push -300
	Memory::WriteInt(0x009B76CB + 3, floor(m_nGameHeight / 2));	//push 300

	// ui status bar approximately 80px in height, add extra spacing for padding
	// exp/meso feed, bottom right
	Memory::WriteInt(0x0089B639 + 1, m_nGameHeight - 100);	//Origin Y
	Memory::WriteInt(0x0089B6F7 + 1, m_nGameWidth - 300);	//Origin X
	Memory::WriteInt(0x0089B796 + 1, m_nGameHeight - 100);	//Origin Y (fixes reset)
	Memory::WriteInt(0x0089BA03 + 1, m_nGameWidth - 300);	//Origin X (fixes reset)

	// 710 (width to menu button) - (82 (width of context menu) / 2) 
	Memory::WriteInt(0x00849E3F + 1, 710 - 87 / 2);	// Menu 
	// 140 (height of context menu) + 73 (height of status bar)
	Memory::WriteInt(0x00849E39 + 1, m_nGameHeight - (140 + 73));	// Menu Y

	// 766 (width to menu button) - (87 (width of context menu) / 2)
	Memory::WriteInt(0x0084A5BD + 1, 766 - 87 / 2);	// ShortCut Menu X
	// 247 (height of context menu) + 73 (height of status bar)
	Memory::WriteInt(0x0084A5B7 + 1, m_nGameHeight - (247 + 73));	// ShortCut Y

	// CFadeWnd__SetOption
	const int yOffset[] = { // original y = 508
		0x00523BB1,// Buddy CH
		0x00523359,// Party Add
		0x00522E65,// Buddy Add
		0x0052378B,// Guild Inv
		0x0052397D,// Alliance
		0x0052307E,// Miniroom Trade Req
		0x005243DB,// Family
		0x00523595,// Quest
		0x00522C73,// Messenger
		0x00523154,// Memo
		0x00523DA5,// Delivery
		0x00523FA3,// New Year Card
		0x0052418C,// Stat Change?
	};
	const int xOffset[] = { // original x = 464
		0x00523BC5,// Buddy CH
		0x0052336D,// Party Add
		0x00522E79,// Buddy Add ; move left ~43px
		0x0052379F,// Guild Inv
		0x00523991,// Alliance
		0x00523092,// Miniroom Trade Req
		0x005243EF,// Family
		0x005235A9,// Quest
		0x00522C87,// Messenger
		0x0052315C,// Memo
		0x00523DC5,// Delivery
		0x00523FB7,// New Year Card
		0x005241A0,// Stat Change?
	};

	//get array length of yOffset
	const int yOffsetLength = std::size(yOffset);
	for (int i = 0; i < yOffsetLength; i++) {
		Memory::WriteInt(yOffset[i] + 1, 610);
		Memory::WriteInt(xOffset[i] + 1, 463);
	}

	Memory::WriteInt(0x0055BB2F + 1, floor(-m_nGameHeight / 2));	// LimitedView::Init Y
	Memory::WriteInt(0x0055BB35 + 1, floor(-m_nGameWidth / 2));	// LimitedView::Init X

	Memory::WriteInt(0x0055B808 + 1, m_nGameHeight);	// LimitedView::Init H
	Memory::WriteInt(0x0055B80D + 1, m_nGameWidth);	// LimitedView::Init W

	Memory::WriteByte(0x0055BEEE, 127); // LimitedView::DrawViewRange Clear Y
	Memory::WriteInt(0x0055BEE8, floor(m_nGameWidth / 2 - 316 / 2)); // LimitedView::DrawViewRange X

	Memory::WriteByte(0x0055C081, 127); // // LimitedView::DrawViewRange Y
	Memory::WriteInt(0x0055C087, floor(m_nGameWidth / 2 - 316 / 2)); // LimitedView::DrawViewRange Clear X

	// megaphone x position adjustment
	Memory::WriteInt(0x0045B417 + 1, m_nGameWidth - 220);
	Memory::WriteInt(0x0045B97E + 1, m_nGameWidth + 100); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone
	Memory::WriteInt(0x0045A5CB + 1, m_nGameWidth); //push 800 ; CAvatarMegaphone ; CreateWnd


	if (m_nGameHeight != 600 || m_nGameWidth != 800) {
		Memory::WriteInt(0x005F481E + 1, m_nGameHeight);//push -300
		Memory::WriteInt(0x005F4824 + 1, m_nGameWidth); //push -400
		Logger::Info("[Client] Custom resolution found, hiding login screen book frame");
	}
}

void Client::ApplyMods() {
	Memory::CodeCave(ApplyCursorVectorPosScrollFix, 0x009E808D, 5);
	Logger::Info("[Client] Applying scroll wheel fix");

	Memory::CodeCave(ApplyHairRangeFix, 0x005C94FC, 8);
	Logger::Info("[Client] Applying hair range fix");

	// force window to be created in windowed mode
	Memory::WriteInt(0x009F7A9B + 1, 0);

	// CLogo start-up animation
	Memory::FillBytes(0x0062EE54, 0x90, 21);
	// Typable PIN
	//Memory::FillBytes(0x004CA8BA, 0x90, 2);

	// how fast the character takes to rest (lower is faster); default -30
	//Memory::WriteByte(0x0045230D, -127)

	// Genderless equips
	//Memory::FillBytes(0x00460AED, (BYTE)Memory::ASM::nop, 2);

	// Player emotion delay (milliseconds)
	//Memory::WriteInt(0x00A244AF, 100);

	// disable SP_5394_UI_UIWINDOWIMG_SKILLUP_BACKGRD popup
	//Memory::FillBytes(0x00A20006, (BYTE)Memory::ASM::nop, 2);

	// CUIStatusBar::ChatLogAdd simple max line count
	Memory::WriteByte(0x008DB387 + 3, 0x7F);

	// CActionMan::SweepCache m_tLastSweepCache delay (milliseconds, default 60000)
	Memory::WriteInt(0x00411BE2, 10000);
	// CActionMan:SweepCache tCacheExpire (milliseconds, default 300000)
	Memory::WriteInt(0x00411CBD + 1, 30000);
	Memory::WriteInt(0x0041201A + 2, 30000);
	Memory::WriteInt(0x00412388 + 2, 30000);
	Memory::WriteInt(0x00411D70 + 2, 30000);
	Memory::WriteInt(0x00411E13 + 2, 30000);
	Memory::WriteInt(0x00411EC5 + 2, 30000);
	Memory::WriteInt(0x00411F68 + 2, 30000);
	Memory::WriteInt(0x004120BD + 2, 30000);
	Memory::WriteInt(0x00412125 + 1, 30000);
	Memory::WriteInt(0x00412282 + 2, 30000);
	Memory::WriteInt(0x00412303 + 2, 30000);
}