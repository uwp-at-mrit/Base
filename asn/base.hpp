#pragma once

#include <string>

#include "datum/natural.hpp"

#include "asn/identifier.hpp"

namespace WarGrey::DTPM {
    typedef std::basic_string<unsigned char> octets;

    WarGrey::DTPM::octets asn_length_to_octets(size_t length);
    size_t asn_octets_to_length(WarGrey::DTPM::octets& length, size_t* offset = nullptr);

    bool asn_primitive_predicate(WarGrey::DTPM::ASNPrimitive type, WarGrey::DTPM::octets& content, size_t offset = 0);
    WarGrey::DTPM::octets asn_octets_box(uint8 tag, WarGrey::DTPM::octets& content, size_t size);
    size_t asn_octets_unbox(WarGrey::DTPM::octets& basn, size_t* offset);

    // NOTE: `asn_octets_to_xxx` does not check the tag, please ensure that the octet is really what it should be.  
    WarGrey::DTPM::octets asn_fixnum_to_octets(long long integer);
    long long asn_octets_to_fixnum(WarGrey::DTPM::octets& bint, size_t* offset = nullptr);

    WarGrey::DTPM::octets asn_natural_to_octets(WarGrey::DTPM::Natural& integer);
    WarGrey::DTPM::Natural asn_octets_to_natural(WarGrey::DTPM::octets& bint, size_t* offset = nullptr);

    WarGrey::DTPM::octets asn_real_to_octets(double integer);
    double asn_octets_to_real(WarGrey::DTPM::octets& bint, size_t* offset = nullptr);
}