#ifndef LIGHTJSON_H__
#define LIGHTJSON_H__

typedef enum {LIGHT_NULL, LIGHT_TRUE, LIGHT_FALSE, LIGHT_NUMBER, LIGHT_STRING, LIGHT_ARRAY, LIGHT_OBJECT} light_type;

typedef struct {
	light_type type;
	double n;
}light_value;

enum {
	LIGHT_PARSE_OK = 0,
	LIGHT_PARSE_EXPECT_VALUE,
	LIGHT_PARSE_INVALID_VALUE,
	LIGHT_PARSE_ROOT_NOT_SINGULAR
};

int light_parse(light_value* v, const char* json);

light_type light_get_type(const light_value* v);

double light_get_number(const light_value* v);

#endif
