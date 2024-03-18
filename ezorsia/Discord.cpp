#include "stdafx.h"
#include "discord_rpc.h"
#include "Discord.h"

#include <ctime>

LPCSTR Discord::m_sApplicationID = "1219212688136208474";
time_t Discord::m_nStartTimestamp = time(nullptr);

void Discord::StartThread() {
	CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Login), 0, 0, 0);
}

void Discord::Login() {
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(m_sApplicationID, &handlers, 1, nullptr);
	Logger::Info("[Discord] Application initialized");
	while (true) {
		Update();
		Sleep(1000 * 60);
	}
}

void Discord::Update() {
	DiscordRichPresence rp;
	memset(&rp, 0, sizeof(rp));

	rp.startTimestamp = m_nStartTimestamp;
	rp.largeImageKey = "maplestory";
	rp.largeImageText = "";
	rp.smallImageKey = "";
	rp.smallImageText = "";
	rp.details = "MapleStory";
	rp.state = "";
	Discord_UpdatePresence(&rp);

	Logger::Info("[Discord] Presence updated");
}