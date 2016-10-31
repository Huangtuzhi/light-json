#ifndef LIGHTJSON_H__
#define LIGHTJSON_H__

#include <stddef.h>

typedef enum {LIGHT_NULL, LIGHT_TRUE, LIGHT_FALSE, LIGHT_NUMBER, LIGHT_STRING, LIGHT_ARRAY, LIGHT_OBJECT} light_type;

typedef struct {
	union {
		struct {
			char* s;
			size_t len;
		}s; // customed string
		double n;
	}u;
	light_type type;
}light_value;

enum {
	LIGHT_PARSE_OK = 0,
	LIGHT_PARSE_EXPECT_VALUE,
	LIGHT_PARSE_INVALID_VALUE,
	LIGHT_PARSE_ROOT_NOT_SINGULAR,
	LIGHT_PARSE_MISS_QUOTATION_MARK,
	LIGHT_PARSE_INVALID_UNICODE_HEX
};

int light_parse(light_value* v, const char* json);

light_type light_get_type(const light_value* v);

double light_get_number(const light_value* v);


const char * light_get_string(const light_value* v);
size_t light_get_string_length(const light_value* v);
void light_set_string(light_value* v, const char* s, size_t len);

#endif
