#include "unity.h"
#include "list.h"
#include "malloc/malloc.h"

#define LENGTH_OF(a) (sizeof(a)/sizeof(a[0]))

struct list_interface_t * list_interface = NULL;

struct test_t
{
    int a;
    int b;
};

struct test_t const test_array[] =
{
    [0] = {
        .a = 0,
        .b = 0
    },
    [1] = {
        .a = 2,
        .b = 2
    },
    [2] = {
        .a = 1,
        .b = 1
    }
};

struct test_t const sorted_test_array[] =
{
    [0] = {
        .a = 0,
        .b = 0
    },
    [1] = {
        .a = 1,
        .b = 1
    },
    [2] = {
        .a = 2,
        .b = 2
    }
};

static bool mem_leak_workaround;
struct mstats mdata_before;

static int sort (void const * _x, void const * _y);
static void iterate (void * element, void * args);

void setUp(void)
{
    mem_leak_workaround = false;
    mdata_before = mstats();

    list_interface = (struct list_interface_t *)LIST_factory_ctor(
        sizeof(struct test_t));
    TEST_ASSERT_NOT_NULL(list_interface);
    TEST_ASSERT_NOT_NULL(list_interface->ctor);
}

void tearDown(void)
{
    LIST_factory_dtor(list_interface);
    if(!mem_leak_workaround)
    {
        TEST_ASSERT_EQUAL_INT(mdata_before.chunks_used, mstats().chunks_used);
    }
}

void test_memory_leak()
{
    mem_leak_workaround = true;
    list_t * test_list = list_interface->ctor(list_interface);
    list_interface->dtor(test_list);
}

void test_can_add_and_pop(void)
{
    /*LIFO list*/
    list_t * test_list = list_interface->ctor(list_interface);

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
    list_t * test_list = list_interface->ctor(list_interface);
    struct test_t element = {.a=3,.b=4};
    TEST_ASSERT_FALSE(list_interface->pop(test_list, &element));
    struct test_t expected = {.a=3,.b=4};
    TEST_ASSERT_EQUAL_MEMORY(&expected, &element,
    sizeof(struct test_t));
    list_interface->dtor(test_list);
}

void test_list_can_sort(void)
{
    /*"Largest" as defined by sort_function is popped first*/
    list_t * test_list = list_interface->ctor(list_interface);

    struct test_t received;
    for (int i = 0; i < LENGTH_OF(test_array); ++i)
    {
        list_interface->add(test_list, &test_array[i]);
    }

    list_interface->sort(test_list, sort);

    for (unsigned int i = LENGTH_OF(test_array) - 1; i > 0 ; --i)
    {
        list_interface->pop(test_list, &received);
        TEST_ASSERT_EQUAL_MEMORY(&sorted_test_array[i], &received,
            sizeof(struct test_t));
    }

    list_interface->dtor(test_list);
}

void test_can_iterate(void)
{
    /*"Largest" as defined by sort_function is popped first*/
    list_t * test_list = list_interface->ctor(list_interface);

    /*Put array elements into test list*/
    for (int i = 0; i < LENGTH_OF(test_array); ++i)
    {
        list_interface->add(test_list, &test_array[i]);
    }

    /*iterate over list elements*/
    list_interface->iterate(test_list, iterate, NULL);

    /*Pop from list and check all have been modified by iterate function*/
    struct test_t received_element = {0};
    struct test_t expected_element = {0};
    for (int i = LENGTH_OF(test_array) - 1; i > 0; --i)
    {
        list_interface->pop(test_list, &received_element);
        expected_element = test_array[i];
        expected_element.a += 1;
        TEST_ASSERT_EQUAL_MEMORY(&expected_element,
            &received_element,
            sizeof(struct test_t));
    }

    list_interface->dtor(test_list);
}

void test_multiple_lists(void)
{
    list_t * first_list = list_interface->ctor(list_interface);
    list_t * second_list = list_interface->ctor(list_interface);

    for (int i = 0; i < LENGTH_OF(test_array); ++i)
    {
        list_interface->add(first_list, &test_array[i]);
        list_interface->add(second_list, &sorted_test_array[i]);
    }

    struct test_t first_list_element = {0};
    struct test_t second_list_element = {0};
    for (int i = LENGTH_OF(test_array) - 1; i > 0 ; --i)
    {
        TEST_ASSERT(list_interface->pop(first_list, &first_list_element));
        TEST_ASSERT(list_interface->pop(second_list, &second_list_element));
        TEST_ASSERT_EQUAL_MEMORY(&test_array[i],
            &first_list_element,
            sizeof(struct test_t));
        TEST_ASSERT_EQUAL_MEMORY(&sorted_test_array[i],
            &second_list_element,
            sizeof(struct test_t));
    }

    list_interface->dtor(first_list);
    list_interface->dtor(second_list);

}

static int sort(void const * _x, void const * _y)
{
    struct test_t const * x = _x;
    struct test_t const * y = _y;

    int ret_val = 0;
    if (0 != (x->b - y->b))
    {
        ret_val = (x->b - y->b);
    }
    else
    {
        ret_val = (x->a - y->a);
    }
    return ret_val;
}

static void iterate(void * _element, void * args)
{
    struct test_t * element = _element;
    element->a += 1;
}
