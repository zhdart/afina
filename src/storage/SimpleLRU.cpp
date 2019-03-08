#include "SimpleLRU.h"

namespace Afina {
namespace Backend {

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::PutNew(const std::string &key, const std::string &value) { //TODO
    const size_t insert_memory = key.size() + value.size();
    if (insert_memory > _max_size)
        return false;
    while (_max_size - _size < insert_memory) {
        if (_lru_head == nullptr)
            return false;
        DeleteLast();
        _size -= insert_memory;
    }

}

bool SimpleLRU::PutOld(const std::string &key, const std::string &value) { //TODO
    return false;
}

bool SimpleLRU::Put(const std::string &key, const std::string &value) {
    if(_lru_index.find(key) != _lru_index.end())
        return PutOld(key, value);
    return PutNew(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::PutIfAbsent(const std::string &key, const std::string &value) {
    if (_lru_index.find(key)!=_lru_index.end())
        return false;
    return PutNew(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Set(const std::string &key, const std::string &value) {
    if (_lru_index.find(key) == _lru_index.end())
        return false;
    return PutOld(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Delete(const std::string &key) { //TODO (+-)
    if (_lru_head == nullptr)
        return false;
    auto tmp_iter = _lru_index.find(key);
    auto tmp_ptr = tmp_iter->second.get();
    if (tmp_ptr == _lru_index.end())
        return false;
    auto prev_ptr = tmp_ptr.prev;
    auto next_ptr = tmp_ptr.next;
    auto tmp_value = tmp_ptr->value;
    auto tmp_key = tmp_ptr->value;
    std::size_t deleted_memory = tmp_key.size() + tmp_value.size();
    if(tmp_ptr == _lru_head)
        return DeleteLast();
    prev_ptr.next = next_ptr;
    next_ptr.prev = prev_ptr;
    _lru_index.erase(tmp_iter);
    tmp_ptr.reset();
    _size -= deleted_memory;
    return true;
}

bool SimpleLRU::DeleteLast() { //TODO (+-)
    if (_lru_head == nullptr)
        return false;
    auto key = _lru_head->key;
    auto value = _lru_head->value;
    auto tmp_ptr = _lru_head;
    std::size_t deleted_memory = key.size() + value.size();
    _lru_head = tmp_ptr->next;
    _lru_index.erase(_lru_head->key);
    tmp_ptr.reset();
    _size -= deleted_memory;
    return true;
}

bool SimpleLRU::Update(const std::string &key) { //TODO
    return false;
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Get(const std::string &key, std::string &value) const { //TODO
    if (_lru_head == nullptr)
        return false;
    auto tmp_iter = _lru_index.find(key);
    if (tmp_iter == _lru_index.end())
        return false;
    value = tmp_iter->second.get().value;
    Update(key); // TODO
    return true;
}

} // namespace Backend
} // namespace Afina
