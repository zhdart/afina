#ifndef AFINA_STORAGE_THREAD_SAFE_SIMPLE_LRU_H
#define AFINA_STORAGE_THREAD_SAFE_SIMPLE_LRU_H

#include <map>
#include <mutex>
#include <string>

#include "SimpleLRU.h"

namespace Afina {
namespace Backend {

/**
 * # SimpleLRU thread safe version
 *
 *
 */
class ThreadSafeSimplLRU : public SimpleLRU {
public:
    ThreadSafeSimplLRU(size_t max_size = 1024) : SimpleLRU(max_size) {}
    ~ThreadSafeSimplLRU() {}

    /*
    bool PutNew(const std::string &key, const std::string &value) {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::PutNew(key, value);
    }


    bool PutOld(const std::string &key, const std::string &value) {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::PutOld(key, value);
    }

    bool DeleteLast() {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::DeleteLast();

    bool Update(lru_node &new_node) {

     }

    }


*/
    // see SimpleLRU.h
    bool Put(const std::string &key, const std::string &value) override {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::Put(key, value);
    }

    // see SimpleLRU.h
    bool PutIfAbsent(const std::string &key, const std::string &value) override {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::PutIfAbsent(key, value);
    }

    // see SimpleLRU.h
    bool Set(const std::string &key, const std::string &value) override {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::Set(key, value);
    }

    // see SimpleLRU.h
    bool Delete(const std::string &key) override {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::Delete(key);
    }

    // see SimpleLRU.h
    bool Get(const std::string &key, std::string &value) override {
        //std::lock_guard<std::mutex> lock(_mutex);
        return SimpleLRU::Get(key, value);
    }

private:
    //std::mutex _mutex;
};

} // namespace Backend
} // namespace Afina

#endif // AFINA_STORAGE_THREAD_SAFE_SIMPLE_LRU_H
