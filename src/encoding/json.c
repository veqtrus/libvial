/*
Copyright (c) 2020 Pavlos Georgiou

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at
https://www.boost.org/LICENSE_1_0.txt
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <vial/encoding/json.h>

static void dispose_array(struct vial_json_array *array)
{
	struct vial_json *item;
	vial_vector_foreach(item, array->vec)
		vial_json_leave(item);
	vial_vector_dispose(array->vec);
}

static void dispose_object(struct vial_json_object *object)
{
	vial_map_dispose(object->map);
}

static void dispose_key(char **p_key)
{
	free(*p_key);
}

struct vial_json vial_json_create_str(const char *value)
{
	struct vial_json result;
	const size_t size = strlen(value) + 1;
	result.type = VIAL_JSON_STRING;
	result.value.as_string = vial_sharedptr_malloc(size);
	memcpy(result.value.as_string, value, size);
	return result;
}

struct vial_json vial_json_create_array(void)
{
	struct vial_json result;
	result.type = VIAL_JSON_ARRAY;
	result.value.as_array = vial_sharedptr_make(sizeof(*result.value.as_array), (vial_dispose_f) dispose_array);
	vial_vector_init(result.value.as_array->vec);
	return result;
}

static int key_comp(const char **l, const char **r)
{
	return strcmp(*l, *r);
}

struct vial_json vial_json_create_object(void)
{
	struct vial_json result;
	result.type = VIAL_JSON_OBJECT;
	result.value.as_object = vial_sharedptr_make(sizeof(*result.value.as_object), (vial_dispose_f) dispose_object);
	vial_map_init(result.value.as_object->map, (vial_comp_f) key_comp);
	result.value.as_object->map.dispose_key = (vial_dispose_f) dispose_key;
	result.value.as_object->map.dispose_value = (vial_dispose_f) vial_json_leave;
	return result;
}

void vial_json_push_move(struct vial_json self, struct vial_json item)
{
	if (self.type == VIAL_JSON_ARRAY)
		vial_vector_add(self.value.as_array->vec, item);
}

void vial_json_put_move(struct vial_json self, const char *key, struct vial_json item)
{
	if (self.type != VIAL_JSON_OBJECT)
		return;
	vial_map_put(self.value.as_object->map, vial_strdup(key), item);
}

struct vial_json vial_json_at(struct vial_json self, size_t idx)
{
	if (self.type == VIAL_JSON_ARRAY && idx < self.value.as_array->vec.size)
		return vial_vector_at(self.value.as_array->vec, idx);
	return vial_json_create_invalid();
}

struct vial_json vial_json_get(struct vial_json self, const char *key)
{
	struct vial_json *result;
	if (self.type == VIAL_JSON_OBJECT) {
		result = vial_map_get(self.value.as_object->map, key);
		return result != NULL ? *result : vial_json_create_invalid();
	}
	return vial_json_create_invalid();
}

static void skip_whitespace(const char **p_str)
{
	const char *c = *p_str;
	while (*c == ' ' || *c == '\n' || *c == '\r' || *c == '\t')
		c++;
	*p_str = c;
}

static struct vial_json parse_number(const char **p_str)
{
	double value;
	const char *c = *p_str;
	int ret, count;
	skip_whitespace(&c);
	ret = sscanf(c, "%lf%n", &value, &count);
	if (ret < 1 || count < 1)
		return vial_json_create_invalid();
	c += count;
	skip_whitespace(&c);
	*p_str = c;
	return vial_json_create_num(value);
}

static bool parse_string(const char **p_str, struct vial_string *str_out)
{
	const char *c = *p_str;
	vial_string_init(str_out, NULL);
	skip_whitespace(&c);
	if (*c != '\"')
		return false;
	for (c++; *c != '\"'; c++) {
		if (*c == '\\') {
			switch (c[1]) {
			case '\"':
				vial_string_push(str_out, '\"');
				break;
			case '\'':
				vial_string_push(str_out, '\'');
				break;
			case '\\':
				vial_string_push(str_out, '\\');
				break;
			case 'b':
				vial_string_push(str_out, '\b');
				break;
			case 'f':
				vial_string_push(str_out, '\f');
				break;
			case 'n':
				vial_string_push(str_out, '\n');
				break;
			case 'r':
				vial_string_push(str_out, '\r');
				break;
			case 't':
				vial_string_push(str_out, '\t');
				break;
			default:
				vial_string_clear(str_out);
				return false;
			}
			c++;
		} else if (*c >= 0 && *c < 32) {
			vial_string_clear(str_out);
			return false;
		} else {
			vial_string_push(str_out, *c);
		}
	}
	c++;
	skip_whitespace(&c);
	*p_str = c;
	return true;
}

static struct vial_json parse_value(const char **p_str, int level)
{
	struct vial_json value, tmp_value;
	struct vial_string tmp_str;
	const char *c = *p_str;
	value.type = VIAL_JSON_INVALID;
	if (level > 1000)
		return value;
	skip_whitespace(&c);
	switch (*c) {
	case 'n':
	case 'f':
	case 't':
		if (vial_cstr_starts(c, "null")) {
			c += 4;
			value.type = VIAL_JSON_NULL;
		} else if (vial_cstr_starts(c, "false")) {
			c += 5;
			value.type = VIAL_JSON_FALSE;
		} else if (vial_cstr_starts(c, "true")) {
			c += 4;
			value.type = VIAL_JSON_TRUE;
		}
		break;
	case '\"':
		if (parse_string(&c, &tmp_str)) {
			value = vial_json_create_str(vial_string_cstr(&tmp_str));
			vial_string_clear(&tmp_str);
		}
		break;
	case '[':
		c++;
		skip_whitespace(&c);
		value = vial_json_create_array();
		if (*c == ']') {
			c++;
			break;
		}
		while (*c != ']') {
			tmp_value = parse_value(&c, level + 1);
			vial_json_push_move(value, tmp_value);
			if (tmp_value.type == VIAL_JSON_INVALID)
				goto parse_error;
			if (*c == ',')
				c++;
			skip_whitespace(&c);
		}
		c++;
		break;
	case '{':
		c++;
		skip_whitespace(&c);
		value = vial_json_create_object();
		if (*c == '}') {
			c++;
			break;
		}
		while (*c != '}') {
			if (!parse_string(&c, &tmp_str))
				goto parse_error;
			if (*c != ':') {
				vial_string_clear(&tmp_str);
				goto parse_error;
			}
			c++;
			tmp_value = parse_value(&c, level + 1);
			vial_json_put_move(value, vial_string_cstr(&tmp_str), tmp_value);
			vial_string_clear(&tmp_str);
			if (tmp_value.type == VIAL_JSON_INVALID)
				goto parse_error;
			if (*c == ',')
				c++;
			skip_whitespace(&c);
		}
		c++;
		break;
	default:
		value = parse_number(&c);
		if (value.type == VIAL_JSON_INVALID)
			goto parse_error;
		break;
	}
	skip_whitespace(&c);
	*p_str = c;
	return value;
parse_error:
	vial_json_leave(&value);
	value.type = VIAL_JSON_INVALID;
	return value;
}

struct vial_json vial_json_decode(const char *str)
{
	return parse_value(&str, 0);
}

static void append_number(struct vial_string *result, double value)
{
	char buf[64];
	buf[0] = 0;
	snprintf(buf, sizeof(buf), "%g", value);
	vial_string_append_cstr(result, buf);
}

static void append_string(struct vial_string *result, const char *value)
{
	vial_string_push(result, '\"');
	for (; *value; value++) {
		switch (*value) {
		case '\"':
			vial_string_append_cstr(result, "\\\"");
			break;
		case '\\':
			vial_string_append_cstr(result, "\\\\");
			break;
		case '\b':
			vial_string_append_cstr(result, "\\b");
			break;
		case '\f':
			vial_string_append_cstr(result, "\\f");
			break;
		case '\n':
			vial_string_append_cstr(result, "\\n");
			break;
		case '\r':
			vial_string_append_cstr(result, "\\r");
			break;
		case '\t':
			vial_string_append_cstr(result, "\\t");
			break;
		default:
			vial_string_push(result, *value);
			break;
		}
	}
	vial_string_push(result, '\"');
}

static void append_indent(struct vial_string *result, const char *indent, int level)
{
	if (indent == NULL)
		return;
	vial_string_push(result, '\n');
	while (level --> 0)
		vial_string_append_cstr(result, indent);
}

struct object_iter_ctx {
	size_t i;
	struct vial_string *result;
	const char *indent;
	int level;
};

static void append_value(struct vial_string *result, struct vial_json *value, const char *indent, int level);

static void append_object_iterator(const char **p_key, struct vial_json *value, struct object_iter_ctx *ctx)
{
	if (ctx->i != 0)
		vial_string_push(ctx->result, ',');
	append_indent(ctx->result, ctx->indent, ctx->level);
	append_string(ctx->result, *p_key);
	vial_string_push(ctx->result, ':');
	if (ctx->indent != NULL)
		vial_string_push(ctx->result, ' ');
	append_value(ctx->result, value, ctx->indent, ctx->level);
	ctx->i++;
}

static void append_value(struct vial_string *result, struct vial_json *value, const char *indent, int level)
{
	size_t i, len;
	struct object_iter_ctx obj_iter_ctx;
	switch (value->type) {
	case VIAL_JSON_NULL:
		vial_string_append_cstr(result, "null");
		break;
	case VIAL_JSON_FALSE:
		vial_string_append_cstr(result, "false");
		break;
	case VIAL_JSON_TRUE:
		vial_string_append_cstr(result, "true");
		break;
	case VIAL_JSON_NUMBER:
		append_number(result, value->value.as_number);
		break;
	case VIAL_JSON_STRING:
		append_string(result, value->value.as_string);
		break;
	case VIAL_JSON_ARRAY:
		vial_string_push(result, '[');
		len = value->value.as_array->vec.size;
		for (i = 0; i < len; i++) {
			if (i != 0)
				vial_string_push(result, ',');
			append_indent(result, indent, level + 1);
			append_value(result, &vial_vector_at(value->value.as_array->vec, i), indent, level + 1);
		}
		append_indent(result, indent, level);
		vial_string_push(result, ']');
		break;
	case VIAL_JSON_OBJECT:
		vial_string_push(result, '{');
		obj_iter_ctx.i = 0;
		obj_iter_ctx.result = result;
		obj_iter_ctx.indent = indent;
		obj_iter_ctx.level = level + 1;
		vial_map_foreach(value->value.as_object->map, (vial_biconsumer_f) append_object_iterator, &obj_iter_ctx);
		append_indent(result, indent, level);
		vial_string_push(result, '}');
		break;
	default:
		break;
	}
}

struct vial_string vial_json_encode(struct vial_json self, const char *indent)
{
	struct vial_string result;
	vial_string_init(&result, NULL);
	append_value(&result, &self, indent, 0);
	return result;
}
