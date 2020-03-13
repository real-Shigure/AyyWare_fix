/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init() override;
	void Draw() override;
	void Move(CUserCmd* pCmd, bool& bSendPacket) override;
private:
	void AutoJump(CUserCmd* pCmd);
	void AutoJumpSMAC(CUserCmd* pCmd);
	void EdgeJump(CUserCmd* pCmd);
	void Walkbot(CUserCmd* pCmd);
	void PerfectStrafe(CUserCmd* pCmd);
	void LegitStrafe(CUserCmd* pCmd);
	void RageStrafe(CUserCmd* pCmd);
	void ChatSpamInterwebz();
	void ChatSpamName();
	void ChatSpamDisperseName();
	void ChatSpamRegular();
	void ChatSpamReports();
	void ChatSpamCallout();
	//void ChatSpamAchievement();
	//void NameDispersion();
	//void ChatSpamAchievement();
	void Fakelag(CUserCmd* pCmd, bool& bSendPacket);

	//std::string GetNameChange();

	/*void ChatSpam();
	void Namechanger();

	std::string GetRegularSpam();
	std::string GetCustomSpam();
	std::string GetPlayerSpam();
	std::string GetSinglePlayerSpam();

	std::string GetNameChange();
	std::string GetNameCalloutChange();*/
};
