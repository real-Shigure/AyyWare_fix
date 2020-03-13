/*
Syn's AyyWare Framework 2015
*/

#pragma once

#include "Hacks.h"

// All misc visuals (Crosshairs etc) go in this class

class CVisuals : public CHack
{
public:
	void Init() override;
	void Draw() override;
	void Move(CUserCmd* pCmd, bool& bSendPacket) override;
private:
	void DrawCrosshair();
	void DrawRecoilCrosshair();
	void DrawCenterRadar();
};
