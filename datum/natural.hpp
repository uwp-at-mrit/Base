#pragma once

#include "datum/bytes.hpp"

namespace WarGrey::SCADA {
	private enum class Fixnum { Uint16, Uint32, Uint64 };

	private class Natural {
	public:
		~Natural() noexcept;
		
		Natural();
		Natural(unsigned long long n);

		Natural(bytes& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(std::string& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(std::wstring& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(Platform::String^ nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, bytes& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, std::string& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, std::wstring& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, Platform::String^ nstr, size_t nstart = 0, size_t nend = 0);

		template<typename BYTE, size_t N>
		Natural(const BYTE(&ns)[N], size_t nstart = 0, size_t nend = N)
			: Natural((const BYTE*)ns, nstart, nend) {}

		template<typename BYTE, size_t N>
		Natural(uint8 base, const BYTE(&ns)[N], size_t nstart = 0, size_t nend = N)
			: Natural(base, (const BYTE*)ns, nstart, nend) {}

		template<typename BYTE>
		Natural(const BYTE ns[], size_t nstart, size_t nend)
			: Natural((uint8)0U, ns, nstart, nend) {}

		template<typename BYTE>
		Natural(uint8 base, const BYTE ns[], size_t nstart, size_t nend) : natural(nullptr), capacity(0U), payload(0U) {
			switch (base) {
			case 16: this->from_base16(ns, nstart, nend); break;
			case 10: this->from_base10(ns, nstart, nend); break;
			case 8:  this->from_base8(ns, nstart, nend); break;
			default: this->from_memory(ns, nstart, nend);
			}
		}

	public:
		Natural(const WarGrey::SCADA::Natural& n);
		Natural(WarGrey::SCADA::Natural&& n);

		WarGrey::SCADA::Natural& operator=(const WarGrey::SCADA::Natural& n);
		WarGrey::SCADA::Natural& operator=(WarGrey::SCADA::Natural&& n);

	public: // NOTE: C++20 has operator<=>
		int compare(unsigned long long rhs) const;
		int compare(const WarGrey::SCADA::Natural& rhs) const;

		friend inline bool operator<(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) < 0); }
		friend inline bool operator<(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) < 0); }
		friend inline bool operator<(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) > 0); }
		friend inline bool operator==(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) == 0); }
		friend inline bool operator==(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) == 0); }
		friend inline bool operator==(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) == 0); }

		friend inline bool operator>(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) > 0); }
		friend inline bool operator>(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) > 0); }
		friend inline bool operator>(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) < 0); }
		friend inline bool operator<=(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) <= 0); }
		friend inline bool operator<=(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) <= 0); }
		friend inline bool operator<=(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) >= 0); }
		friend inline bool operator>=(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) >= 0); }
		friend inline bool operator>=(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) >= 0); }
		friend inline bool operator>=(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) <= 0); }
		friend inline bool operator!=(const WarGrey::SCADA::Natural& lhs, const WarGrey::SCADA::Natural& rhs) { return (lhs.compare(rhs) != 0); }
		friend inline bool operator!=(const WarGrey::SCADA::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) != 0); }
		friend inline bool operator!=(unsigned long long lhs, const WarGrey::SCADA::Natural& rhs) { return (rhs.compare(lhs) != 0); }

	public:
		WarGrey::SCADA::Natural& operator++();
		inline WarGrey::SCADA::Natural operator++(int postfix) { Natural snapshot(*this); this->operator++(); return snapshot; }

		WarGrey::SCADA::Natural& operator--();
		inline WarGrey::SCADA::Natural operator--(int postfix) { Natural snapshot(*this); this->operator--(); return snapshot; }

		WarGrey::SCADA::Natural& operator+=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator+=(const WarGrey::SCADA::Natural& rhs);

		WarGrey::SCADA::Natural& operator-=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator-=(const WarGrey::SCADA::Natural& rhs);
		
		WarGrey::SCADA::Natural& operator*=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator*=(const WarGrey::SCADA::Natural& rhs);

		inline WarGrey::SCADA::Natural& operator/=(const WarGrey::SCADA::Natural& rhs) { return this->quotient_remainder(rhs, nullptr); }
		inline WarGrey::SCADA::Natural& operator%=(const WarGrey::SCADA::Natural& rhs) { this->quotient_remainder(rhs, this); return (*this); };

		friend inline WarGrey::SCADA::Natural operator+(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs += rhs; }
		friend inline WarGrey::SCADA::Natural operator+(unsigned long long lhs, WarGrey::SCADA::Natural rhs) { return rhs += lhs; }
		friend inline WarGrey::SCADA::Natural operator+(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs += rhs; }

		// TODO: compiler will cast the number into Natural when encountered `n - Natural`;
		friend inline WarGrey::SCADA::Natural operator-(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs -= rhs; }
		friend inline WarGrey::SCADA::Natural operator-(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs -= rhs; }

		friend inline WarGrey::SCADA::Natural operator*(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs *= rhs; }
		friend inline WarGrey::SCADA::Natural operator*(unsigned long long lhs, WarGrey::SCADA::Natural rhs) { return rhs *= lhs; }
		friend inline WarGrey::SCADA::Natural operator*(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs *= rhs; }

		friend inline WarGrey::SCADA::Natural operator/(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs /= rhs; }
		friend inline WarGrey::SCADA::Natural operator%(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs %= rhs; }

	public:
		WarGrey::SCADA::Natural& expt(unsigned long long e);
		WarGrey::SCADA::Natural& expt(const WarGrey::SCADA::Natural& e);

		WarGrey::SCADA::Natural& quotient_remainder(const WarGrey::SCADA::Natural& divider, Natural* remainder = nullptr);

		friend inline WarGrey::SCADA::Natural expt(WarGrey::SCADA::Natural b, unsigned long long e) { return b.expt(e); }
		friend inline WarGrey::SCADA::Natural expt(unsigned long long b, WarGrey::SCADA::Natural e) { return Natural(b).expt(e); }
		friend inline WarGrey::SCADA::Natural expt(WarGrey::SCADA::Natural b, const WarGrey::SCADA::Natural& e) { return b.expt(e); }

	public:
		WarGrey::SCADA::Natural operator~();

		WarGrey::SCADA::Natural& operator<<=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator>>=(unsigned long long rhs);

		WarGrey::SCADA::Natural& operator&=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator&=(const WarGrey::SCADA::Natural& rhs);
		WarGrey::SCADA::Natural& operator|=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator|=(const WarGrey::SCADA::Natural& rhs);
		WarGrey::SCADA::Natural& operator^=(unsigned long long rhs);
		WarGrey::SCADA::Natural& operator^=(const WarGrey::SCADA::Natural& rhs);

		friend inline WarGrey::SCADA::Natural operator<<(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs <<= rhs; }
		friend inline WarGrey::SCADA::Natural operator>>(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs >>= rhs; }

		friend inline WarGrey::SCADA::Natural operator&(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs &= rhs; }
		friend inline WarGrey::SCADA::Natural operator&(unsigned long long lhs, WarGrey::SCADA::Natural rhs) { return rhs &= lhs; }
		friend inline WarGrey::SCADA::Natural operator&(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs &= rhs; }
		friend inline WarGrey::SCADA::Natural operator|(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs |= rhs; }
		friend inline WarGrey::SCADA::Natural operator|(unsigned long long lhs, WarGrey::SCADA::Natural rhs) { return rhs |= lhs; }
		friend inline WarGrey::SCADA::Natural operator|(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs |= rhs; }
		friend inline WarGrey::SCADA::Natural operator^(WarGrey::SCADA::Natural lhs, unsigned long long rhs) { return lhs ^= rhs; }
		friend inline WarGrey::SCADA::Natural operator^(unsigned long long lhs, WarGrey::SCADA::Natural rhs) { return rhs ^= lhs; }
		friend inline WarGrey::SCADA::Natural operator^(WarGrey::SCADA::Natural lhs, const WarGrey::SCADA::Natural& rhs) { return lhs ^= rhs; }

		bool is_bit_set(unsigned long long m);
		WarGrey::SCADA::Natural bit_field(unsigned long long start, unsigned long long end);

	public:
		uint8& operator[] (int idx);
		uint16 fixnum16_ref(int idx, size_t offset = 0U);
		uint32 fixnum32_ref(int idx, size_t offset = 0U);
		uint64 fixnum64_ref(int idx, size_t offset = 0U);

	public:
		bool is_zero() const;
		bool is_one() const;
		bool is_fixnum() const;
		bool is_odd() const;
		bool is_even() const;

	public:
		size_t length() const;
		size_t integer_length() const;
		size_t fixnum_count(WarGrey::SCADA::Fixnum type = Fixnum::Uint64) const;

	public:
		size_t expand(size_t size);

	public:
		bytes to_bytes() const;
		bytes to_hexstring() const;

	private:
		Natural(void* null, long long capacity);
		void from_memory(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_memory(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base16(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base16(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base10(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base10(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base8(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base8(const uint16 nchars[], size_t nstart, size_t nend);

	private:
		void add_digit(uint8 digit);
		void times_digit(uint8 digit);
		uint8 division_normalize(Natural* divisor);

	private:
		void on_moved();
		void bzero();
		void skip_leading_zeros(size_t payload);
		void decrease_from_slot(size_t slot);
		uint8* malloc(size_t size);
		void recalloc(size_t new_size, uint8 initial = '\0', size_t shift = 0U);
		
	private:
		uint8* natural;
		size_t capacity;
		size_t payload;
	};
}
