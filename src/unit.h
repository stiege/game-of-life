#ifndef unit_H
#define unit_H

#include "cell.h"

enum unit_type
{
    BLOCK_UNIT
};

typedef struct unit_t unit_t;

struct unit_interface_t
{
    unit_t * (* ctor)(
        struct unit_interface_t * unit_interface, enum unit_type);
    cell_list_t * (* get_cell_list)();
};


void UNIT_init(void);
struct unit_interface_t * UNIT_factory_ctor(
    struct unit_interface_t * unit_interface);

#endif // unit_H
