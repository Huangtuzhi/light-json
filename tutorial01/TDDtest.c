#include "lightjson.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
	do {\
		test_count++;\
		if (equality)\
			test_pass++;\
		else {\
			fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
			main_ret = 1;\
		}\
	} while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect == actual), expect, actual, "%d")

static void test_parse_null() {
	light_value v;
	v.type = LIGHT_FALSE;
	EXPECT_EQ_INT(LIGHT_PARSE_OK, light_parse(&v, "null"));
	EXPECT_EQ_INT(LIGHT_NULL, light_get_type(&v));
}

static void test_parse_true() {
	light_value v;
	v.type = LIGHT_NULL;
	EXPECT_EQ_INT(LIGHT_PARSE_OK, light_parse(&v, "  true"));
	EXPECT_EQ_INT(LIGHT_TRUE, light_get_type(&v));
}

static void test_parse_false() {
	light_value v;
	v.type = LIGHT_NULL;
	EXPECT_EQ_INT(LIGHT_PARSE_OK, light_parse(&v, "false "));
	EXPECT_EQ_INT(LIGHT_FALSE, light_get_type(&v));
}

static void test_parse() {
	test_parse_null();
	test_parse_true();
	test_parse_false();
}

int main()
{
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}
