#include "unity.h"
#include "list.h"

struct list_interface_t * list_interface = NULL;

struct test_t
{
    int a;
    int b;
};

void setUp(void)
{
    list_interface = (struct list_interface_t *)LIST_factory_ctor(
        sizeof(struct test_t));
    TEST_ASSERT_NOT_NULL(list_interface);
    TEST_ASSERT_NOT_NULL(list_interface->ctor);
}

void tearDown(void)
{
}

void test_can_add_and_pop(void)
{
    list_t * test_list = list_interface->ctor(list_interface,0);
    struct test_t first_element = {.a=3,.b=4};
    list_interface->add(test_list, &first_element);

    struct test_t received;
    list_interface->pop(test_list, &received);

    TEST_ASSERT_EQUAL_MEMORY(&first_element, &received, sizeof(struct test_t));
    list_interface->dtor(test_list);
}
