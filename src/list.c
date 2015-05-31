#include "list.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

struct list_t
{
    const struct list_interface_t interface;
    char * data;
    unsigned int length;
    unsigned int element_size;
};


static list_t * LIST_ctor (struct list_interface_t * interface,
    unsigned int length);
static void LIST_dtor (list_t * list);
static unsigned int LIST_get_length (list_t * list);
static void LIST_add (list_t * list, void * element);
static void list_copy (list_t * destination, list_t const * const source);
static unsigned int LIST_get_size (struct list_interface_t * list);
static void LIST_pop (list_t * list, void * element);
static struct list_interface_t * new_interface (void);
static struct list_t * new_list (void);

static struct list_t null_list =
{
    .interface = {
        .ctor = LIST_ctor,
        .dtor = LIST_dtor,
        .get_length = LIST_get_length,
        .add = LIST_add,
        .pop = LIST_pop
    },
    .data = NULL,
    .length = 0,
    .element_size = 0
};

struct list_interface_t const * const LIST_factory_ctor(
    unsigned int element_size)
{
    struct list_t * list = new_list();
    list->element_size = element_size;
    return (struct list_interface_t *)list;
}

static list_t * LIST_ctor(struct list_interface_t * interface,
    unsigned int length)
{
    struct list_t * list = (struct list_t *)interface;
    unsigned int element_size = list->element_size;
    struct list_t * _new_list = new_list();
    _new_list->data = malloc(element_size * length);
    _new_list->element_size = element_size;

    return list;
}

static void LIST_dtor(list_t * list)
{
    if (    ( NULL      != list)
        &&  (&null_list != list))
    {
        if (NULL != list->data)
        {
            free(list->data);
        }
        free(list);
    }
}

static unsigned int LIST_get_length(list_t * list)
{
    unsigned int ret_val = 0;
    if (NULL != list)
    {
        ret_val = list->length;
    }
    return ret_val;
}

static void LIST_add(list_t * list, void * element)
{
    if (    (NULL != list)
        &&  (NULL != element))
    {
        unsigned int new_length = list->length + 1;
        char * new_data = malloc(new_length * list->element_size);
        char * last_element = &new_data[list->length * list->element_size];

        memcpy(new_data, list->data, list->length * list->element_size);
        memcpy(last_element, element, list->element_size);
        free(list->data);
        list->length = new_length;
        list->data = new_data;
    }
}

static void list_copy(struct list_t * destination, list_t const * const source)
{
    memcpy( destination,
            source,
            sizeof(struct list_t));
}

static void LIST_pop(list_t * list, void * element)
{
    if (    (NULL != list)
        &&  (NULL != element))
    {
        unsigned int new_length = list->length - 1; 
        char * new_data = malloc(new_length * list->element_size);
        char * last_element =
            &list->data[new_length * list->element_size];

        memcpy(element, last_element, list->element_size);
        memcpy(new_data, list->data, new_length * list->element_size);
        free(list->data);
        list->data = new_data;
        list->length = new_length;
    }
}

static struct list_t * new_list(void)
{
    struct list_t * list = malloc(sizeof(struct list_t));
    list_copy(list, &null_list);
    return list;
}

