// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
//   ______  ______   ______   __         //
//  /\  == \/\  __ \ /\  __ \ /\ \        //
//  \ \  _-/\ \ \/\ \\ \ \/\ \\ \ \____   //
//   \ \_\   \ \_____\\ \_____\\ \_____\  //
//    \/_/    \/_____/ \/_____/ \/_____/  //
//                                        //
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// utils/pool.h

#ifndef UTILS_POOL_H
#define UTILS_POOL_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    enum
    {
        POOL_FLAGS_NONE    = 0x0,
        POOL_FLAGS_ASSERT  = 1 << 0,
        POOL_FLAGS_EXPAND  = 1 << 1,
        POOL_FLAGS_RECYCLE = 1 << 2,
    };

#define POOL_DECLARE(type, name)                             \
    void   name##_pool_new(size_t capacity, uint32_t flags); \
    void   name##_pool_delete(void);                         \
    void   name##_pool_reserve(size_t capacity);             \
    void   name##_pool_insert(type item);                    \
    void   name##_pool_remove(uint32_t index);               \
    type*  name##_pool_begin(void);                          \
    type*  name##_pool_end(void);                            \
    size_t name##_pool_count(void);

#define POOL_DEFINE(type, name)                                                                                 \
    static struct                                                                                               \
    {                                                                                                           \
        uint32_t flags;                                                                                         \
                                                                                                                \
        type* data;                                                                                             \
                                                                                                                \
        size_t start;                                                                                           \
        size_t count;                                                                                           \
        size_t capacity;                                                                                        \
    } name##_pool;                                                                                              \
                                                                                                                \
    void name##_pool_new(size_t capacity, uint32_t flags)                                                       \
    {                                                                                                           \
        assert(capacity > 0);                                                                                   \
        assert((flags & (POOL_FLAGS_EXPAND | POOL_FLAGS_RECYCLE)) != (POOL_FLAGS_EXPAND | POOL_FLAGS_RECYCLE)); \
                                                                                                                \
        name##_pool.data     = (type*)malloc(capacity * sizeof(type));                                          \
        name##_pool.start    = 0;                                                                               \
        name##_pool.count    = 0;                                                                               \
        name##_pool.capacity = capacity;                                                                        \
        name##_pool.flags    = flags;                                                                           \
                                                                                                                \
        assert(name##_pool.data != NULL);                                                                       \
    }                                                                                                           \
                                                                                                                \
    void name##_pool_delete(void)                                                                               \
    {                                                                                                           \
        free(name##_pool.data);                                                                                 \
                                                                                                                \
        name##_pool.start    = 0;                                                                               \
        name##_pool.count    = 0;                                                                               \
        name##_pool.capacity = 0;                                                                               \
    }                                                                                                           \
                                                                                                                \
    void name##_pool_reserve(size_t capacity)                                                                   \
    {                                                                                                           \
        type* prev = name##_pool.data;                                                                          \
        type* curr = (type*)malloc(capacity * sizeof(type));                                                    \
                                                                                                                \
        assert(curr != NULL);                                                                                   \
                                                                                                                \
        for (size_t i = 0; i < name##_pool.count; ++i)                                                          \
        {                                                                                                       \
            curr[i] = prev[(name##_pool.start + i) % name##_pool.capacity];                                     \
        }                                                                                                       \
                                                                                                                \
        name##_pool.data     = curr;                                                                            \
        name##_pool.start    = 0;                                                                               \
        name##_pool.capacity = capacity;                                                                        \
                                                                                                                \
        free(prev);                                                                                             \
    }                                                                                                           \
                                                                                                                \
    void name##_pool_insert(type item)                                                                          \
    {                                                                                                           \
        if ((name##_pool.flags & POOL_FLAGS_ASSERT) == POOL_FLAGS_ASSERT)                                       \
        {                                                                                                       \
            assert(name##_pool.count + 1 <= name##_pool.capacity);                                              \
        }                                                                                                       \
                                                                                                                \
        if (name##_pool.count >= name##_pool.capacity)                                                          \
        {                                                                                                       \
            if ((name##_pool.flags & POOL_FLAGS_EXPAND) == POOL_FLAGS_EXPAND)                                   \
            {                                                                                                   \
                name##_pool_reserve(name##_pool.capacity * 2);                                                  \
                name##_pool_insert(item);                                                                       \
            }                                                                                                   \
                                                                                                                \
            if ((name##_pool.flags & POOL_FLAGS_RECYCLE) == POOL_FLAGS_RECYCLE)                                 \
            {                                                                                                   \
                name##_pool.data[name##_pool.start] = item;                                                     \
                name##_pool.start                   = (name##_pool.start + 1) % name##_pool.count;              \
            }                                                                                                   \
                                                                                                                \
            return;                                                                                             \
        }                                                                                                       \
                                                                                                                \
        name##_pool.data[name##_pool.count++] = item;                                                           \
    }                                                                                                           \
                                                                                                                \
    void name##_pool_remove(uint32_t index)                                                                     \
    {                                                                                                           \
        assert(name##_pool.count > 0);                                                                          \
                                                                                                                \
        name##_pool.data[index] = name##_pool.data[name##_pool.count - 1];                                      \
        --name##_pool.count;                                                                                    \
    }                                                                                                           \
                                                                                                                \
    type* name##_pool_begin(void) { return &name##_pool.data[0]; }                                              \
                                                                                                                \
    type* name##_pool_end(void) { return &name##_pool.data[name##_pool.count]; }                                \
                                                                                                                \
    size_t name##_pool_count(void) { return name##_pool.count; }

#define POOL_LOOP_FORWARD(type, name, var) for (type* var = name##_pool_begin(); var != name##_pool_end(); ++var)
#define POOL_LOOP_REVERSE(type, name, var) for (type* var = name##_pool_end() - 1; var >= name##_pool_begin(); --var)

#ifdef __cplusplus
}
#endif

#endif  // UTILS_POOL_H
