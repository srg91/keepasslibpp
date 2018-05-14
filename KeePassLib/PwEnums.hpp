#pragma once

/*
 * Compression algorithm specifiers
 */
enum class PwCompressionAlgorithm {
    // No compression.
    None,

    // GZip compression
    GZip,

    // TODO: Is we need this type?
    // Virtual field: currently known number of algorithms. Should not be used
    // by plugins or libraries -- it's used internally only.
    Count
};

