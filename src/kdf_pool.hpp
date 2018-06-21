#pragma once

#include "uuid.hpp"
#include "kdf_engine.hpp"
#include "kdf_parameters.hpp"

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <utility>

namespace keepasspp {

// TODO: make threadsafe? what about globalness?
// TODO: add iterator? add
class KdfPool {
public:
    using key_type = Uuid;
    using mapped_type = std::shared_ptr<KdfEngine>;
    using value_type = std::pair<key_type, mapped_type>;
    using size_type = std::map<key_type, mapped_type>::size_type;

    static std::size_t size() noexcept;

    static void add(std::shared_ptr<KdfEngine> engine) noexcept;
    static mapped_type get(key_type key) noexcept;

    static KdfParameters getDefaultParameters() noexcept;
    // TODO: add operator[]?
private:
    inline static mapped_type defaultEngine;
    inline static std::map<key_type, mapped_type> pool{};

    static void ensureInitialized() noexcept;
};

}
