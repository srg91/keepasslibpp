#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>


class VariantDictionary {
    const uint16_t VdVersion = 0x0100;
    const uint16_t VdmCritical = 0xff00;
    const uint16_t VdmInfo = 0x00ff;

//    std::unordered_map<std::string, object? void* ?
    enum class VdType : int8_t {
        None = 0x00,
        UInt32 = 0x04,
        Bool = 0x08,
        Int32 = 0x0C,
        Int64 = 0x0D,
        String = 0x18,
        ByteArray = 0x42,
    };

    struct Variant {
        VdType type;

        union value {
            uint32_t UInt32;
            bool Bool;
            int32_t Int32;
            int64_t Int64;
            std::string String;
            // TODO: ByteArray???
            std::vector<uint8_t> ByteArray;
            ~Value();
        };
    };

public:

};





