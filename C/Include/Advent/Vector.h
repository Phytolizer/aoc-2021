#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define VECTOR_PROTOTYPES_(VectorType, DataType)                                                                       \
  void VectorType##_init(VectorType* v);                                                                               \
  void VectorType##_deinit(VectorType* v);                                                                             \
  void VectorType##_push(VectorType* v, DataType value)

#define VECTOR_DECLARE(VectorType, DataType)                                                                           \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    DataType* data;                                                                                                    \
    size_t length;                                                                                                     \
    size_t capacity;                                                                                                   \
  } VectorType;                                                                                                        \
  typedef struct                                                                                                       \
  {                                                                                                                    \
    bool enabled;                                                                                                      \
    void (*func)(DataType value);                                                                                      \
  } VectorType##_destructor_;                                                                                          \
  VECTOR_PROTOTYPES_(VectorType, DataType)

#define VECTOR_DESTRUCTOR(VectorType, Function)                                                                        \
  ((VectorType##_destructor_){                                                                                         \
      .enabled = true,                                                                                                 \
      .func = Function,                                                                                                \
  })

#define VECTOR_DESTRUCTOR_NONE(VectorType)                                                                             \
  ((VectorType##_destructor_){                                                                                         \
      .enabled = false,                                                                                                \
  })

#define VECTOR_IMPL_INIT_(VectorType)                                                                                  \
  void VectorType##_init(VectorType* v)                                                                                \
  {                                                                                                                    \
    memset(v, 0, sizeof(VectorType));                                                                                  \
  }

#define VECTOR_IMPL_DEINIT_(VectorType, DataType, Destructor)                                                          \
  void VectorType##_deinit(VectorType* v)                                                                              \
  {                                                                                                                    \
    static const VectorType##_destructor_ dtor_ = Destructor;                                                          \
    if (dtor_.enabled)                                                                                                 \
    {                                                                                                                  \
      for (size_t i = 0; i < v->length; ++i)                                                                           \
      {                                                                                                                \
        dtor_.func(v->data[i]);                                                                                        \
      }                                                                                                                \
    }                                                                                                                  \
    free(v->data);                                                                                                     \
    VectorType##_init(v);                                                                                              \
  }

#define VECTOR_IMPL_PUSH_(VectorType, DataType)                                                                        \
  void VectorType##_push(VectorType* v, DataType value)                                                                \
  {                                                                                                                    \
    if (v->length == v->capacity)                                                                                      \
    {                                                                                                                  \
      v->capacity = v->capacity ? v->capacity * 2 : 1;                                                                 \
      v->data = realloc(v->data, v->capacity * sizeof(DataType));                                                      \
    }                                                                                                                  \
    v->data[v->length++] = value;                                                                                      \
  }

#define VECTOR_IMPLEMENT(VectorType, DataType, Destructor)                                                             \
  VECTOR_IMPL_INIT_(VectorType)                                                                                        \
  VECTOR_IMPL_DEINIT_(VectorType, DataType, Destructor)                                                                \
  VECTOR_IMPL_PUSH_(VectorType, DataType)                                                                              \
  static bool VectorType##_IS_IMPLEMENTED_ = true
