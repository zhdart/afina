#include "SimpleLRU.h"

namespace Afina {
namespace Backend {

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::PutNew(const std::string &key, const std::string &value) { //TODO (+-)
    size_t put_size = key.size() + value.size();
    if (put_size > _max_size) {
        return false;
    }
    while (_max_size - _size < put_size) {
        DeleteLast();
    }
    std::unique_ptr<lru_node> tmp_node{new lru_node{key, value}};
    if (_lru_tail != nullptr) {
        tmp_node->prev = _lru_tail;
        _lru_tail->next.swap(tmp_node);
        _lru_tail = _lru_tail->next.get();
    } else {
        _lru_head.swap(tmp_node);
        _lru_tail = _lru_head.get();
    }
    _lru_index.insert(std::make_pair(std::reference_wrapper<const std::string>(_lru_tail->key), std::reference_wrapper<lru_node>(*_lru_tail)));
    _size += put_size;
    return true;
}

bool SimpleLRU::PutOld(const std::string &key, const std::string &value) { //TODO убрать второй поиск
    auto tmp_iter = _lru_index.find(key);
    lru_node &tmp_node = tmp_iter->second;
    size_t put_size = value.size() - tmp_node.value.size();
    if (put_size > _max_size) {
        return false;
    }
    while(_size + put_size >= _max_size) {
        DeleteLast();
    }
    tmp_node.value = value;
    _size += put_size;
    return Update(tmp_node);
}

bool SimpleLRU::Put(const std::string &key, const std::string &value) { // ++
    if(_lru_index.find(key) != _lru_index.end()) {
        return PutOld(key, value);
    }
    return PutNew(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::PutIfAbsent(const std::string &key, const std::string &value) { // ++
    if (_lru_index.find(key)!=_lru_index.end()) {
        return false;
    }
    return PutNew(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Set(const std::string &key, const std::string &value) { // ++
    if (_lru_index.find(key) == _lru_index.end()) {
        return false;
    }
    return PutOld(key, value);
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Delete(const std::string &key) { //TODO
    if (_lru_head == nullptr) {
        return false;
    }
    auto tmp_iter = _lru_index.find(key);
    if (tmp_iter == _lru_index.end()) {
        return false;
    }
    auto tmp_ptr = tmp_iter->second.get();
    auto prev_ptr = tmp_ptr.prev;
    auto next_ptr = tmp_ptr.next;
    auto tmp_value = tmp_ptr.value;
    auto tmp_key = tmp_ptr.value;
    std::size_t deleted_memory = tmp_key.size() + tmp_value.size();
    if(tmp_ptr == _lru_head) {
        return DeleteLast();
    }
    prev_ptr.next = next_ptr;
    next_ptr.prev = prev_ptr;
    _lru_index.erase(tmp_iter);
    tmp_ptr.reset();
    _size -= deleted_memory;
    return true;
}

bool SimpleLRU::DeleteLast() { //TODO
    if (_lru_head == nullptr) {
        return false;
    }
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

bool SimpleLRU::Update(lru_node &new_node) { //TODO (+-)
    if (&new_node == _lru_tail) {
        return true;
    }
    if (&new_node == _lru_head.get()) {
        _lru_head.swap(new_node.next);
        _lru_head->prev = nullptr;
    } else {
        new_node.next->prev = new_node.prev;
        new_node.prev->next.swap(new_node.next);
    }
    _lru_tail->next.swap(new_node.next);
    new_node.prev = _lru_tail;
    _lru_tail = &new_node;
    return true;
}

// See MapBasedGlobalLockImpl.h
bool SimpleLRU::Get(const std::string &key, std::string &value) const { // ++
    if (_lru_head == nullptr) {
        return false;
    }
    auto tmp_iter = _lru_index.find(key);
    if (tmp_iter == _lru_index.end()) {
        return false;
    }
    value = tmp_iter->second.get().value;
    return Update(tmp_iter->second.get());
}

} // namespace Backend
} // namespace Afina
