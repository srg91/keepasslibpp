#include "kdf_engine_aes.hpp"
#include "kdf_engine_argon.hpp"
#include "kdf_parameters.hpp"
#include "kdf_pool.hpp"

#include <iterator>
#include <memory>

namespace keepasspp {

std::size_t KdfPool::size() noexcept {
    KdfPool::ensureInitialized();
    return std::size(KdfPool::pool);
}

void KdfPool::add(std::shared_ptr<KdfEngine> engine) noexcept {
    if (!KdfPool::defaultEngine) defaultEngine = engine;
    KdfPool::pool[engine->getUuid()] = engine;
}

KdfPool::mapped_type KdfPool::get(KdfPool::key_type key) noexcept {
    KdfPool::ensureInitialized();

    if (KdfPool::pool.find(key) != std::end(KdfPool::pool)) {
        return KdfPool::pool.at(key);
    }
    return nullptr;
}

KdfParameters KdfPool::getDefaultParameters() noexcept {
    KdfPool::ensureInitialized();
    return defaultEngine->getDefaultParameters();
}

void KdfPool::ensureInitialized() noexcept {
    if (std::size(KdfPool::pool) > 0) return;

    KdfPool::add(std::make_shared<KdfEngineAes>());
    KdfPool::add(std::make_shared<KdfEngineArgon2>());
}

}