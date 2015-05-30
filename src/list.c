#include "list.h"
#include "stdlib.h"
#include "string.h"

struct list_t
{
    const struct list_interface_t * interface;
    char * data;
    unsigned int length;
    unsigned int element_size;
};


static unsigned int element_size = 0;

static list_t * LIST_ctor(unsigned int length);
static void LIST_dtor(list_t * list);
static unsigned int LIST_get_length(list_t * list);
static void LIST_add(list_t * list, void * element);
static void list_copy(list_t * destination, list_t * source);
static unsigned int LIST_get_size(struct list_interface_t * list);

const struct list_interface_t default_list_interface = 
{
    .ctor = LIST_ctor,
    .dtor = LIST_dtor,
    .get_length = LIST_get_length,
    .add_element = LIST_add
};

static const struct list_t null_list =
{
    .interface = &default_list_interface,
    .data = NULL,
    .length = 0,
    .element_size = 0
};

struct list_interface_t const * const LIST_factory_ctor(
    unsigned int element_size)
{
    return (struct list_interface_t *)&null_list;
}

static list_t * LIST_ctor(unsigned int length)
{
    list_t * list = malloc(sizeof(list_t));
    list->data = malloc(element_size * length);
    list->element_size = element_size;

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
        char * new_data = malloc((new_length) * list->element_size);
        char * last_element = &new_data[new_length - 1];

        memcpy(last_element, element, list->element_size);
        free(list->data);
        list->data = new_data;
    }
}

static void list_copy(list_t * destination, list_t * source)
{
    memcpy( destination->data,
            source->data,
            source->element_size * source->length);
    destination->element_size = source->element_size;
    destination->length = source->length;
}



