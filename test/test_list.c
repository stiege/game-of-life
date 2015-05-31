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
    /*LIFO list*/
    list_t * test_list = list_interface->ctor(list_interface,0);

    struct test_t first_element = {.a=3,.b=4};
    struct test_t second_element = {.a=12,.b=24};
    list_interface->add(test_list, &first_element);
    list_interface->add(test_list, &second_element);

    struct test_t received_first;
    struct test_t received_second;
    list_interface->pop(test_list, &received_first);
    list_interface->pop(test_list, &received_second);

    TEST_ASSERT_EQUAL_MEMORY(&second_element, &received_first,
        sizeof(struct test_t));
    TEST_ASSERT_EQUAL_MEMORY(&first_element, &received_second,
        sizeof(struct test_t));

    list_interface->dtor(test_list);
}

void test_empty_pop_does_nothing(void)
{
    list_t * test_list = list_interface->ctor(list_interface,0);
    struct test_t element = {.a=3,.b=4};
    TEST_ASSERT_FALSE(list_interface->pop(test_list, &element));
    struct test_t expected = {.a=3,.b=4};
    TEST_ASSERT_EQUAL_MEMORY(&expected, &element,
    sizeof(struct test_t));
}
