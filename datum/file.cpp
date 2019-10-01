#include <string>
#include <winerror.h>
#include <Windows.h>

#include "datum/file.hpp"
#include "datum/flonum.hpp"
#include "datum/string.hpp"
#include "datum/char.hpp"

using namespace WarGrey::SCADA;

/************************************************************************************************/
bool WarGrey::SCADA::char_end_of_word(char ch) {
	return ch == space;
}

bool WarGrey::SCADA::char_end_of_line(char ch) {
	return (ch == linefeed) || (ch == carriage_return);
}

bool WarGrey::SCADA::char_end_of_field(char ch) {
	return ch == comma;
}

/************************************************************************************************/
int WarGrey::SCADA::peek_char(std::filebuf& src) {
	return src.sgetc();
}

int WarGrey::SCADA::read_char(std::filebuf& src) {
	discard_space(src);

	return src.sbumpc();
}

std::string WarGrey::SCADA::read_text(std::filebuf& src, bool (*end_of_text)(char)) {
	std::string str;
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if (end_of_text(ch)) {
			src.sungetc();
			break;
		}

		str.push_back(ch);
	}

	return str;
}

Platform::String^ WarGrey::SCADA::read_wtext(std::filebuf& src, bool (*end_of_text)(char)) {
	return make_wstring(read_text(src, end_of_text));
}

Platform::String^ WarGrey::SCADA::read_wgb18030(std::filebuf& src, bool (*end_of_text)(char)) {
	Platform::String^ wstr = nullptr;
	std::string str;
	bool gb18030 = false;
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if (end_of_text(ch)) {
			src.sungetc();
			break;
		}

		if ((!gb18030) && (!((0 <= ch) && (ch < 0x80)))) { // ch is actually unsigned int
			gb18030 = true;
		}

		str.push_back(ch);
	}

	if (gb18030) { // https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
		wchar_t wpool[1024]; // DIG only allows 50 chars.
		size_t msize = str.length();
		int wsize = MultiByteToWideChar(54936, 0, str.c_str(), int(msize), wpool, int(sizeof(wpool) / sizeof(wchar_t)));

		if (wsize > 0) {
			wstr = ref new Platform::String(wpool, (unsigned int)wsize);
		} else {
			switch (GetLastError()) {
			case ERROR_INSUFFICIENT_BUFFER: wstr = "insufficient buffer"; break;
			case ERROR_INVALID_FLAGS: wstr = "invalid flags"; break;
			case ERROR_INVALID_PARAMETER: wstr = "invalid parameters"; break;
			case ERROR_NO_UNICODE_TRANSLATION: wstr = "no unicode translation"; break;
			default: wstr = "unknown error occured"; break;
			}
		}
	} else {
		wstr = make_wstring(str);
	}

	return wstr;
}

unsigned long long WarGrey::SCADA::read_natural(std::filebuf& src) {
	unsigned long long n = 0;
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			src.sungetc();
			break;
		}

		n = n * 10 + (ch - zero);
	}

	return n;
}

long long WarGrey::SCADA::read_integer(std::filebuf& src) {
	long long n = 0;
	long long sign = 1;
	char ch;

	discard_space(src);

	if (src.sgetc() == minus) {
		sign = -1;
		src.snextc();
	}

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			src.sungetc();
			break;
		}

		n = n * 10 + (ch - zero);
	}

	return n * sign;
}

double WarGrey::SCADA::read_flonum(std::filebuf& src) {
	double flonum = flnan;
	double i_acc = 10.0;
	double f_acc = 1.0;
	double sign = 1.0;
	char ch;

	discard_space(src);

	if (src.sgetc() == minus) {
		sign = -1.0;
		src.snextc();
	}

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			if ((ch != dot) || (f_acc != 1.0)) {
				src.sungetc();
				break;
			} else {
				i_acc = 1.0;
				f_acc = 0.1;
				continue;
			}
		}

		if (std::isnan(flonum)) {
			flonum = 0.0;
		}

		flonum = flonum * i_acc + double(ch - zero) * f_acc;

		if (f_acc != 1.0) {
			f_acc *= 0.1;
		}
	}

	return flonum * sign;
}

float WarGrey::SCADA::read_single_flonum(std::filebuf& src) {
	return float(read_flonum(src));
}

void WarGrey::SCADA::discard_space(std::filebuf& src) {
	char ch;

	while ((ch = src.sbumpc()) != EOF) {
		if (!char_end_of_word(ch)) {
			src.sungetc();
			break;
		}
	}
}

void WarGrey::SCADA::discard_newline(std::filebuf& src) {
	char ch;
	
	while ((ch = src.sbumpc()) != EOF) {
		if (!char_end_of_line(ch)) {
			src.sungetc();
			break;
		}
	}
}

void WarGrey::SCADA::discard_this_line(std::filebuf& src) {
	char ch;

	while ((ch = src.sbumpc()) != EOF) {
		if (char_end_of_line(ch)) {
			discard_newline(src);
			break;
		}
	}
}
