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

#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect == actual), expect, actual, "%f")

#define TEST_NUMBER(expect, json)\
	do {\
		light_value v;\
		EXPECT_EQ_INT(LIGHT_PARSE_OK, light_parse(&v, json));\
		EXPECT_EQ_INT(LIGHT_NUMBER, light_get_type(&v));\
		EXPECT_EQ_DOUBLE(expect, light_get_number(&v));\
	} while(0)

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

static void test_parse_number() {
	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(-1E+10, "-1E+10");
}

static void test_parse() {
	//test_parse_null();
	//test_parse_true();
	//test_parse_false();
	test_parse_number();
}

int main()
{
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}
