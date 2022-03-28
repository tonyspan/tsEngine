#pragma once

namespace tsEngine
{
	enum class MouseCode
	{
		LMB = 1, MMB = 2, RMB = 3
	};

	static const char* MouseCodeName(MouseCode mouseCode) { return mouseCode == MouseCode::LMB ? "LMB" : mouseCode == MouseCode::RMB ? "RMB" : "MMB"; }
}