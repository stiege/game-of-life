#ifndef list_H
#define list_H

typedef struct list_t list_t;

struct list_interface_t
{
    list_t * (* ctor)(struct list_interface_t * interface,
        unsigned int length);
    void (* dtor)(list_t * list);
    unsigned int (* get_length)(list_t * list);
    void (* add)(list_t * list, void * element);
    void (* pop)(list_t * list, void * element);
};

/*
    Default factory constructor and destructor
*/
struct list_interface_t const * const LIST_factory_ctor (
    unsigned int element_size);

void LIST_factory_dtor (struct list_interface_t const * const interface);

#endif // list_H
