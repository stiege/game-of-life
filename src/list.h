#ifndef list_H
#define list_H

#include "stdbool.h"
typedef struct list_t list_t;

struct list_interface_t
{
    list_t * (* ctor)(struct list_interface_t * interface);
    void (* dtor)(list_t * list);
    unsigned int (* get_length)(list_t * list);
    void (* add)(list_t * list, void const * const element);
    bool (* pop)(list_t * list, void * element);
    void (* sort)(list_t * list,
        int (* compare)(void const * element_a, void const * element_b));
    void (* iterate)(list_t * list,
        void (* action)(void * element, void * args), void * args);
};

/*
    Default factory constructor and destructor
*/
struct list_interface_t const * const LIST_factory_ctor (
    unsigned int element_size);

void LIST_factory_dtor (struct list_interface_t const * const interface);

#endif // list_H
