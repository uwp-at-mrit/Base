#pragma once

namespace WarGrey::SCADA {
	static const char linefeed = 0x0A;
	static const char carriage_return = 0x0D;
	static const char space = 0x20;
	static const char zero = 0x30;
	static const char nine = 0x39;
	static const char minus = 0x2D;
	static const char dot = 0x2E;

	static const wchar_t wlinefeed = (wchar_t)(linefeed);
	static const wchar_t wcarriage_return = (wchar_t)(carriage_return);
	static const wchar_t wspace = (wchar_t)(space);
	static const wchar_t wzero = (wchar_t)(zero);
	static const wchar_t wnine = (wchar_t)(nine);
	static const wchar_t wminus = (wchar_t)(minus);
	static const wchar_t wdot = (wchar_t)(dot);
}
