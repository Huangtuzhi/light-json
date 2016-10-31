#include <assert.h>
#include <stdlib.h>
#include "lightjson.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
	const char* json;
}light_context;

// omit the space
static void light_parse_whitespace(light_context* c) {
	const char* p = c->json;
	while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
		p++;
	c->json = p;
}

static int light_parse_null(light_context* c, light_value* v) {
	EXPECT(c, 'n');
	if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
		return LIGHT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type =LIGHT_NULL;
	return LIGHT_PARSE_OK;
}

static int light_parse_true(light_context* c, light_value* v) {
	EXPECT(c, 't');
	if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
		return LIGHT_PARSE_INVALID_VALUE;
	c->json += 3;
	v->type =LIGHT_TRUE;
	return LIGHT_PARSE_OK;
}

static int light_parse_false(light_context* c, light_value* v) {
	EXPECT(c, 'f');
	if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
		return LIGHT_PARSE_INVALID_VALUE;
	c->json += 4;
	v->type =LIGHT_FALSE;
	return LIGHT_PARSE_OK;
}

static int light_parse_value(light_context* c, light_value* v) {
	switch(*c->json) {
		case 'n': return light_parse_null(c, v);
		case 't': return light_parse_true(c, v);
		case 'f': return light_parse_false(c, v);
		case '\0': return LIGHT_PARSE_EXPECT_VALUE;
		default: return LIGHT_PARSE_INVALID_VALUE;
	}
}

int light_parse(light_value* v, const char* json) {
	int ret;
	light_context c;
	assert(v!= NULL);
	c.json = json;
	v->type = LIGHT_NULL;
	light_parse_whitespace(&c);
	if ((ret = light_parse_value(&c, v)) == LIGHT_PARSE_OK) {
		light_parse_whitespace(&c);
		if (*c.json != '\0')
			ret = LIGHT_PARSE_ROOT_NOT_SINGULAR;
	}

	return ret;
}

light_type light_get_type(const light_value* v) {
	assert(v != NULL);
	return v->type;
}
