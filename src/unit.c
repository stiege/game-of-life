#include "unit.h"
#include "stdlib.h"

static unit_t * UNIT_ctor(struct unit_interface_t * unit_interface,
    enum unit_type unit);
static cell_list_t * UNIT_get_cell_list(unit_t * p_block_unit);

static struct unit_interface_t null_unit_interface = 
{
    .ctor = UNIT_ctor,
    .get_cell_list = UNIT_get_cell_list
};

void UNIT_init(void)
{

}

struct unit_interface_t * UNIT_factory_ctor(
    struct unit_interface_t * unit_interface)
{
    return &null_unit_interface;
}

static unit_t * UNIT_ctor(struct unit_interface_t * unit_interface,
    enum unit_type unit)
{
    return NULL;
}

static cell_list_t * UNIT_get_cell_list(unit_t * p_block_unit)
{
    return NULL;
}
