#ifndef __Common_Util_h__
#define __Common_Util_h__

#include <map>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>

#if __cplusplus >= 201103L
#define USE_CXX11_ATOMIC
#include <atomic>
#elif defined(IOS)
#define USE_IOS_ATOMIC
#include <stdatomic.h>
#endif

using namespace std;

#ifndef __LINK_TOOLS
#define __LINK_TOOLS

#define def_link_node(struct_name)\
struct {\
    struct_name *prev;\
    struct_name *next;\
}

#define def_link_entity(struct_name)\
struct {\
    struct_name *head;\
    struct_name *tail;\
    int node_num;\
}

#define link_node_init(entity)\
do {\
    (entity).prev = 0;\
    (entity).next = 0;\
} while (0)

#define link_init(entity)\
do {\
    (entity).head = 0;\
    (entity).tail = 0;\
    (entity).node_num = 0;\
}while(0)

#define link_swap(entity1,entity2,struct_name)\
do {\
    struct_name *_node;int _node_num;\
    _node = (entity1).head;\
    (entity1).head = (entity2).head;\
    (entity2).head = _node;\
    _node = (entity1).tail;\
    (entity1).tail = (entity2).tail;\
    (entity2).tail = _node;\
    _node_num = (entity1).node_num;\
    (entity1).node_num = (entity2).node_num;\
    (entity2).node_num = _node_num;\
}while(0)

#define link_head(entity)               ((entity).head)
#define link_tail(entity)               ((entity).tail)
#define link_node_num(entity)           ((entity).node_num)

#define link_next(node, node_name)      ((node)->node_name.next)
#define link_prev(node, node_name)      ((node)->node_name.prev)

#define link_add_head(entity, node, node_name)\
do {\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    (node)->node_name.prev = 0;\
    (node)->node_name.next = (entity).head;\
     if ((entity).head)\
         (entity).head->node_name.prev = (node);\
     else\
         (entity).tail = (node);\
     (entity).head = (node);\
     (entity).node_num ++;\
}while(0)

#define link_add_tail(entity, node, node_name)\
do {\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    (node)->node_name.next = 0;\
    (node)->node_name.prev = (entity).tail;\
    if ((entity).tail)\
        (entity).tail->node_name.next = (node);\
    else\
        (entity).head = (node);\
    (entity).tail = (node);\
    (entity).node_num ++;\
}while(0)

#define link_remove_head(entity, node_name)\
do {\
    assert((entity).head);\
    assert((entity).head->node_name.prev == 0);\
    void **head_next = (void **)&((entity).head->node_name.next);\
    (entity).head = (entity).head->node_name.next;\
    *head_next = 0;\
    if ((entity).head)\
        (entity).head->node_name.prev = 0;\
    else\
        (entity).tail = 0;\
    assert((entity).node_num > 0);\
    (entity).node_num --;\
    assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_remove_tail(entity, node_name)\
do {\
    assert((entity).tail);\
    assert((entity).head->node_name.next == 0);\
    void **tail_prev = (void **)&((entity).head->node_name.prev);\
    (entity).tail = (entity).tail->node_name.prev;\
    *tail_prev = 0;\
    if ((entity).tail)\
        (entity).tail->node_name.next = 0;\
    else\
        (entity).head = 0;\
    assert((entity).node_num > 0);\
    (entity).node_num --;\
    assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_remove_node(entity, node, node_name)\
do {\
    if ((node)->node_name.prev)\
    {\
        assert((node)->node_name.prev->node_name.next == node);\
        (node)->node_name.prev->node_name.next = (node)->node_name.next;\
    }\
    else\
    {\
        assert((entity).head == (node));\
        (entity).head = (node)->node_name.next;\
    }\
    if ((node)->node_name.next)\
    {\
        assert((node)->node_name.next->node_name.prev == node);\
        (node)->node_name.next->node_name.prev = (node)->node_name.prev;\
    }\
    else\
    {\
        assert((entity).tail == (node));\
        (entity).tail = (node)->node_name.prev;\
    }\
    (node)->node_name.prev = 0;\
    (node)->node_name.next = 0;\
    assert((entity).node_num > 0);\
    (entity).node_num --;\
    assert((entity).node_num>0||((entity).head==0&&(entity).tail==0));\
    assert((entity).node_num>1||((entity).head==(entity).tail));\
}while(0)

#define link_insert_prev(entity, next_node, node, node_name)\
do {\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    assert(next_node);\
    (node)->node_name.next = (next_node);\
    (node)->node_name.prev = (next_node)->node_name.prev;\
    if ((next_node)->node_name.prev)\
    {\
        (next_node)->node_name.prev->node_name.next = (node);\
    }\
    else\
    {\
        assert((entity).head == (next_node));\
        (entity).head = (node);\
    }\
    (next_node)->node_name.prev = (node);\
    (entity).node_num ++;\
}while(0)

#define link_insert_next(entity, prev_node, node, node_name)\
do {\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    assert(prev_node);\
    (node)->node_name.prev = (prev_node);\
    (node)->node_name.next = (prev_node)->node_name.next;\
    if ((prev_node)->node_name.next)\
    {\
        (prev_node)->node_name.next->node_name.prev = (node);\
    }\
    else\
    {\
        assert((entity).tail == (prev_node));\
        (entity).tail = (node);\
    }\
    (prev_node)->node_name.next = (node);\
    (entity).node_num ++;\
}while(0)

#define link_no_entity_remove_node(node, node_name)\
do {\
    if ((node)->node_name.prev)\
        (node)->node_name.prev->node_name.next = (node)->node_name.next;\
    if ((node)->node_name.next)\
        (node)->node_name.next->node_name.prev = (node)->node_name.prev;\
    (node)->node_name.next = 0;\
    (node)->node_name.prev = 0;\
}while(0)

#define link_no_entity_insert_prev(next_node, node, node_name)\
do {\
    assert(next_node);\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    (node)->node_name.next = (next_node);\
    (node)->node_name.prev = (next_node)->node_name.prev;\
    if ((next_node)->node_name.prev)\
        (next_node)->node_name.prev->node_name.next = (node);\
    (next_node)->node_name.prev = (node);\
}while(0)

#define link_no_entity_insert_next(prev_node, node, node_name)\
do {\
    assert(prev_node);\
    assert((node)->node_name.prev == 0);\
    assert((node)->node_name.next == 0);\
    (node)->node_name.prev = (prev_node);\
    (node)->node_name.next = (prev_node)->node_name.next;\
    if ((prev_node)->node_name.next)\
        (prev_node)->node_name.next->node_name.prev = (node);\
    (prev_node)->node_name.next = (node);\
}while(0)
#endif
/**
 * @}
 */

/**
 * @defgroup Map数据结构定义和操作宏定义
 * @{
 *
 */
#ifndef __MAP_TOOLS
#define __MAP_TOOLS

typedef int (*fn_hash_key)(char *key);
typedef int (*fn_cmp_key)(char *key1, char *key2);

#define def_map_node(struct_name)\
struct {\
    struct_name *next;\
    int hash_key;\
}

#define def_map_entity(struct_name)\
struct {\
    struct_name **map;\
    int map_size;\
    int key_offset;\
    fn_hash_key hash_key;\
    fn_cmp_key cmp_key;\
    int node_num;\
}

#define map_init(entity, init_size, struct_name, key_name, func_hash_key, func_cmp_key)\
do {\
    (entity).map = (struct_name **)malloc(init_size*sizeof(void *));\
    memset((entity).map, 0, init_size*sizeof(void *));\
    (entity).map_size = init_size;\
    (entity).key_offset = (int)((char *)&((struct_name*)1)->key_name - (char *)1);\
    (entity).hash_key = (fn_hash_key)func_hash_key;\
    (entity).cmp_key = (fn_cmp_key)func_cmp_key;\
    (entity).node_num = 0;\
}while(0)

#define map_uninit(entity)\
do {\
    if ((entity).map) {\
        free((entity).map);\
        (entity).map = 0;\
    }\
}while(0)

#define map_node_num(entity)                ((entity).node_num)

#define entity_node_key(entity, node)       ((char *)(node) + (entity).key_offset)

#define entity_hash_key1(entity, node)      ((entity).hash_key(entity_node_key(entity, node))&0x7FFFFFFF)
#define entity_hash_key2(entity, key)       ((entity).hash_key((char *)&key)&0x7FFFFFFF)
#define entity_cmp_key(entity, node, key)   ((entity).cmp_key(entity_node_key(entity, node), (char *)&key))

#define map_search(entity, key, node, map_name)\
do {\
    int _hash_key = entity_hash_key2(entity, key);\
    int _map_idx = _hash_key%(entity).map_size;\
    node = (entity).map[_map_idx];\
    while(node)\
    {\
        assert(((node)->map_name.hash_key%(entity).map_size) == _map_idx);\
        if (entity_cmp_key(entity, node, key)==0)\
            break;\
        node = (node)->map_name.next;\
    }\
}while(0)

#define map_add_node(entity, node, map_name)\
do {\
    int _hash_key = entity_hash_key1(entity, node);\
    int _map_idx = _hash_key%(entity).map_size;\
    (node)->map_name.hash_key = _hash_key;\
    (node)->map_name.next =  (entity).map[_map_idx];\
    (entity).map[_map_idx] = node;\
    (entity).node_num ++;\
}while(0)

#define map_remove_node(entity, node, struct_name, map_name)\
do {\
    struct_name *_node;\
    int _map_idx = (node)->map_name.hash_key%(entity).map_size;\
    _node = (entity).map[_map_idx];\
    if (_node == node)\
    {\
        (entity).map[_map_idx] = (node)->map_name.next;\
        (node)->map_name.next = 0;\
        (entity).node_num --;\
        break;\
    }\
    while (1)\
    {\
        if (!_node)\
        {\
            assert(0);\
            break;\
        }\
        if ((_node)->map_name.next == node)\
        {\
            (_node)->map_name.next = (node)->map_name.next;\
            (node)->map_name.next = 0;\
            (entity).node_num --;\
            break;\
        }\
        _node = (_node)->map_name.next;\
    }\
}while(0)

#define map_update_size(entity, struct_name, map_name)\
do {\
    struct_name **_map;\
    int _map_size, _map_idx, _i;\
    if ((entity).node_num <= (entity).map_size*2)\
        break;\
    _map_size = (entity).map_size*4;\
    _map = (struct_name **)malloc(_map_size*sizeof(void *));\
    if (!_map) break;\
    memset(_map, 0, _map_size*sizeof(void *));\
    for (_i=0; _i< (entity).map_size; _i++)\
    {\
        struct_name *_node1, *_node2 = (entity).map[_i];\
        while (_node2)\
        {\
            _node1 = _node2;\
            _node2 = _node2->map_name.next;\
            _map_idx = _node1->map_name.hash_key%_map_size;\
            _node1->map_name.next =  _map[_map_idx];\
            _map[_map_idx] = _node1;\
        }\
    }\
    free((entity).map);\
    (entity).map = _map;\
    (entity).map_size = _map_size;\
}while(0);
#endif

#endif
