#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "lightjson.h"

#define EXPECT(c, ch) do { assert(*c->json == (ch)); c->json++; } while(0)
#define light_init(v) do { (v)->type = LIGHT_NULL; } while (0)
#define PUTC(c, ch) do { *(char*)light_context_push(c, sizeof(char)) = ch; } while(0)

#ifndef LIGHT_PARSE_STACK_INIT_SIZE
#define LIGHT_PARSE_STACK_INIT_SIZE 256
#endif

typedef struct {
	const char* json;
	char * stack;
	size_t size, top;
}light_context;

static void* light_context_push(light_context* c, size_t size) {
	void* ret;
	assert(size > 0);
	if (c->top + size >= c->size) {
		if (c->size == 0)
			c->size = LIGHT_PARSE_STACK_INIT_SIZE;
		while (c->top + size >= c->size)
			c->size += c->size >> 1;
		c->stack = (char*)realloc(c->stack, c->size);
	}
	ret = c->stack + c->top;
	c->top += size;
	return ret;
}

static void* light_context_pop(light_context* c, size_t size) {
	assert(c->top >= size);
	return c->stack + (c->top -= size);
}


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

static int light_parse_number(light_context* c, light_value*v) {
	char* end;
	v->u.n = strtod(c->json, &end);
	if (c->json == end)
		return LIGHT_PARSE_INVALID_VALUE;
	c->json = end;
	v->type = LIGHT_NUMBER;
	return LIGHT_PARSE_OK;
}

static int light_parse_string(light_context* c, light_value* v) {
	size_t head = c->top;
	size_t len;
	const char* p;
	EXPECT(c, '\"');
	p = c->json;
	for (;;) {
		char ch = *p++;
		switch (ch) {
			case '\"':
				len = c->top - head;
			    light_set_string(v, (const char*)light_context_pop(c, len), len);
				c->json = p;
				return LIGHT_PARSE_OK;
			case '\0':
				c->top = head;
				return LIGHT_PARSE_MISS_QUOTATION_MARK;
			default:
				PUTC(c, ch);
		}
	}
}

static int light_parse_value(light_context* c, light_value* v) {
	switch(*c->json) {
		case 'n': return light_parse_null(c, v);
		case 't': return light_parse_true(c, v);
		case 'f': return light_parse_false(c, v);
		case '\0': return LIGHT_PARSE_EXPECT_VALUE;
		default: return light_parse_number(c, v);
		case '\"': return light_parse_string(c, v);
	}
}

int light_parse(light_value* v, const char* json) {
	int ret;
	light_context c;
	assert(v!= NULL);
	
	c.json = json;
	c.stack = NULL;
	c.size = c.top = 0;
	light_init(v);
	light_parse_whitespace(&c);
	if ((ret = light_parse_value(&c, v)) == LIGHT_PARSE_OK) {
		light_parse_whitespace(&c);
		if (*c.json != '\0')
			ret = LIGHT_PARSE_ROOT_NOT_SINGULAR;
	}

	assert(c.top == 0);
	free(c.stack);
	return ret;
}

light_type light_get_type(const light_value* v) {
	assert(v != NULL);
	return v->type;
}

double light_get_number(const light_value* v) {
	assert(v != NULL && v->type == LIGHT_NUMBER);
	return v->u.n;
}

void light_free(light_value* v) {
	assert(v != NULL);
	if (v->type == LIGHT_STRING)
		free(v->u.s.s);
	v->type = LIGHT_NULL;
}

void light_set_string(light_value* v, const char* s, size_t len) {
	assert(v != NULL && (s != NULL || len == 0));
	light_free(v);
	v->u.s.s = (char*)malloc(len + 1);
	memcpy(v->u.s.s, s, len);
	v->u.s.s[len] = '\0';
	v->u.s.len = len;
	v->type = LIGHT_STRING;
}

const char* light_get_string(const light_value* v) {
	assert(v != NULL);
	return v->u.s.s;
}

size_t light_get_string_length(const light_value* v) {
	assert(v != NULL);
	return v->u.s.len;
}


