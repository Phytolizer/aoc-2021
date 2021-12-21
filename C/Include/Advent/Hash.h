#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAX_LOAD_DEFAULT 0.75

uint64_t Hash_FNV1a(const void* data, size_t len);

#define HASH_PROTOTYPES_(HashType, ValueType)                                                                          \
  void HashType##_init(HashType* h);                                                                                   \
  void HashType##_deinit(HashType* h);                                                                                 \
  void HashType##_insert(HashType* h, HashType##_key key, ValueType value);                                            \
  ValueType* HashType##_lookup(HashType* h, HashType##_key key)

#define HASH_DECLARE(HashType, ValueType)                                                                              \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    void* key;                                                                                                         \
    size_t len;                                                                                                        \
  } HashType##_key;                                                                                                    \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    bool present;                                                                                                      \
    HashType##_key key;                                                                                                \
    ValueType value;                                                                                                   \
  } HashType##_bucket;                                                                                                 \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    bool keyEnabled;                                                                                                   \
    void (*keyFunc)(void*);                                                                                            \
    bool valueEnabled;                                                                                                 \
    void (*valueFunc)(ValueType);                                                                                      \
  } HashType##_destructor_;                                                                                            \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    HashType##_bucket* buckets;                                                                                        \
    size_t bucketCount;                                                                                                \
    size_t capacity;                                                                                                   \
  } HashType;                                                                                                          \
  HASH_PROTOTYPES_(HashType, ValueType)

#define HASH_DESTRUCTOR_NONE(HashType)                                                                                 \
  ((HashType##_destructor_){                                                                                           \
      .keyEnabled = false,                                                                                             \
      .valueEnabled = false,                                                                                           \
  })

#define HASH_DESTRUCTOR_KEY(HashType, KeyDestructor)                                                                   \
  ((HashType##_destructor_){                                                                                           \
      .keyEnabled = true,                                                                                              \
      .keyFunc = KeyDestructor,                                                                                        \
      .valueEnabled = false,                                                                                           \
  })

#define HASH_DESTRUCTOR_VALUE(HashType, ValueDestructor)                                                               \
  ((HashType##_destructor_){                                                                                           \
      .keyEnabled = false,                                                                                             \
      .valueEnabled = true,                                                                                            \
      .valueFunc = ValueDestructor,                                                                                    \
  })

#define HASH_DESTRUCTOR_BOTH(HashType, KeyDestructor, ValueDestructor)                                                 \
  ((HashType##_destructor_){                                                                                           \
      .keyEnabled = true,                                                                                              \
      .keyFunc = KeyDestructor,                                                                                        \
      .valueEnabled = true,                                                                                            \
      .valueFunc = ValueDestructor,                                                                                    \
  })

#define HASH_IMPL_INIT_(HashType)                                                                                      \
  void HashType##_init(HashType* h)                                                                                    \
  {                                                                                                                    \
    memset(h, 0, sizeof(HashType));                                                                                    \
  }

#define HASH_IMPL_DEINIT_(HashType, Destructor)                                                                        \
  void HashType##_deinit(HashType* h)                                                                                  \
  {                                                                                                                    \
    static const HashType##_destructor_ dtor_ = Destructor;                                                            \
    if (dtor_.keyEnabled || dtor_.valueEnabled)                                                                        \
    {                                                                                                                  \
      for (size_t i = 0; h->capacity; ++i)                                                                             \
      {                                                                                                                \
        if (h->buckets[i].present)                                                                                     \
        {                                                                                                              \
          if (dtor_.keyEnabled)                                                                                        \
          {                                                                                                            \
            dtor_.keyFunc(h->buckets[i].key.key);                                                                      \
          }                                                                                                            \
          if (dtor_.valueEnabled)                                                                                      \
          {                                                                                                            \
            dtor_.valueFunc(h->buckets[i].value);                                                                      \
          }                                                                                                            \
        }                                                                                                              \
      }                                                                                                                \
    }                                                                                                                  \
    free(h->buckets);                                                                                                  \
    HashType##_init(h);                                                                                                \
  }

#define HASH_IMPL_FIND_BUCKET_(HashType)                                                                               \
  HashType##_bucket* HashType##_findBucket_(HashType##_bucket* buckets, size_t capacity, HashType##_key key)           \
  {                                                                                                                    \
    if (capacity == 0)                                                                                                 \
    {                                                                                                                  \
      return NULL;                                                                                                     \
    }                                                                                                                  \
    uint64_t h = Hash_FNV1a(key.key, key.len) % capacity;                                                              \
    while (buckets[h].present &&                                                                                       \
           !(buckets[h].key.len == key.len && memcmp(buckets[h].key.key, key.key, key.len) == 0))                      \
    {                                                                                                                  \
      h = (h + 1) % capacity;                                                                                          \
    }                                                                                                                  \
    return &buckets[h];                                                                                                \
  }

#define HASH_IMPL_REHASH_(HashType)                                                                                    \
  void HashType##_rehash_(HashType* h, size_t newCapacity)                                                             \
  {                                                                                                                    \
    HashType##_bucket* oldBuckets = h->buckets;                                                                        \
    h->buckets = calloc(newCapacity, sizeof(HashType##_bucket));                                                       \
    for (size_t i = 0; i < h->capacity; ++i)                                                                           \
    {                                                                                                                  \
      if (oldBuckets[i].present)                                                                                       \
      {                                                                                                                \
        HashType##_bucket* newBucket = HashType##_findBucket_(h->buckets, newCapacity, oldBuckets[i].key);             \
        newBucket->present = true;                                                                                     \
        newBucket->key = oldBuckets[i].key;                                                                            \
        newBucket->value = oldBuckets[i].value;                                                                        \
      }                                                                                                                \
    }                                                                                                                  \
    h->capacity = newCapacity;                                                                                         \
    free(oldBuckets);                                                                                                  \
  }

#define HASH_IMPL_INSERT_(HashType, ValueType, MaxLoad)                                                                \
  void HashType##_insert(HashType* h, HashType##_key key, ValueType value)                                             \
  {                                                                                                                    \
    if (h->capacity == 0 || (double)h->bucketCount / h->capacity > MaxLoad)                                            \
    {                                                                                                                  \
      HashType##_rehash_(h, h->capacity ? h->capacity * 2 : 8);                                                        \
    }                                                                                                                  \
    HashType##_bucket* bucket = HashType##_findBucket_(h->buckets, h->capacity, key);                                  \
    bucket->present = true;                                                                                            \
    bucket->key = key;                                                                                                 \
    bucket->value = value;                                                                                             \
    ++h->bucketCount;                                                                                                  \
  }

#define HASH_IMPL_LOOKUP_(HashType, ValueType)                                                                         \
  ValueType* HashType##_lookup(HashType* h, HashType##_key key)                                                        \
  {                                                                                                                    \
    HashType##_bucket* bucket = HashType##_findBucket_(h->buckets, h->capacity, key);                                  \
    if (!bucket || !bucket->present)                                                                                   \
    {                                                                                                                  \
      return NULL;                                                                                                     \
    }                                                                                                                  \
    return &bucket->value;                                                                                             \
  }

#define HASH_IMPLEMENT(HashType, ValueType, Destructor, MaxLoad)                                                       \
  HASH_IMPL_INIT_(HashType)                                                                                            \
  HASH_IMPL_DEINIT_(HashType, Destructor)                                                                              \
  HASH_IMPL_FIND_BUCKET_(HashType)                                                                                     \
  HASH_IMPL_REHASH_(HashType)                                                                                          \
  HASH_IMPL_INSERT_(HashType, ValueType, MaxLoad)                                                                      \
  HASH_IMPL_LOOKUP_(HashType, ValueType)                                                                               \
  static const bool HashType##_IS_IMPLEMENTED_ = true
