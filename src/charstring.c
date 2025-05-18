/*****************************************************************************
 * charstring.c: string management
 * this file is part of https://github.com/ouistiti-project/
 *****************************************************************************
 * Copyright (C) 2025-2027
 *
 * Authors: Marc Chalain <marc.chalain@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "log.h"
#include "charstring.h"

#define MAX_STRING 1024
#define EREJECT -1
#define ESUCCESS 0

string_t *string_create(size_t size)
{
	string_t *str = calloc(1, sizeof(*str) + size);
	str->ddata = (void *)str + sizeof(*str);
	str->data = str->ddata;
	str->size = size;
	return str;
}

void string_debug(string_t *str, const char *name)
{
	dbg("string: %s %.*s", name, (int)(str->length & INT_MAX), str->data);
}

size_t string_length(const string_t *str)
{
	if (str->data && str->length == (size_t) -1)
		((string_t*)str)->length = strnlen(str->data, MAX_STRING);
	return str->length;
}

size_t string_size(const string_t *str)
{
	return str->size;
}

int string_store(string_t *str, const char *pointer, size_t length)
{
	if (str->ddata)
	{
		return string_cpy(str, pointer, length);
	}
	str->data = pointer;
	/// set length and check if value is -1
	str->length = length;
	str->length = string_length(str);
	str->size = str->length + 1;
	if (str->data == NULL)
	{
		str->length = 0;
		str->size = 0;
	}
	return ESUCCESS;
}

int string_cmp(const string_t *str, const char *cmp, size_t length)
{
	if (cmp == NULL)
		return -1;
	if ((length != (size_t) -1) && (length != str->length))
		return (length - str->length);
	return strncasecmp(str->data, cmp, str->length);
}

#if 1
int string_contain(const string_t *stack, const char *nail, size_t length, const char sep)
{
	int ret = -1;
	if (nail == NULL)
		return -1;
	if (length == (size_t) -1)
		length = strnlen(nail, stack->length);
	if (nail[length - 1] == '*')
		length--;
	const char *offset = stack->data;
	while (offset && offset[0] != '\0')
	{
		if (!strncasecmp(offset, nail, length))
		{
			if (nail[length] == '*')
				ret = 0;
			/// a string_t may not be a null terminated array
			if (((offset + length) == (stack->data + stack->length)) || offset[length] == sep)
				ret = 0;
		}
		if (ret == 0)
			break;
		offset = strchr(offset, sep);
		if (offset)
			offset++;
	}
	return ret;
}
#else
int string_contain(const string_t *stack, const char *nailorig, size_t length, const char sep)
{
	int ret = -1;
	const char *nail = nailorig;
	if (nail == NULL)
		return -1;
	if (length == (size_t) -1)
		length = strnlen(nail, stack->length);
	if (nail[length - 1] == '*')
		length--;
	const char *offset = stack->data;
	while (offset && offset[0] != '\0')
	{
		int wildcard = 0;
		if (offset[0] == '*')
			wildcard = 1;
		do
		{
			if (wildcard)
			{
				while (*nail != offset[1] && nail < nailorig + length) nail++;
				offset++;
			}
			if (length - (nail - nailorig) == 0)
				break;
			if (!strncasecmp(offset, nail, length - (nail - nailorig)))
			{
				if (nail[length] == '*')
					ret = 0;
				/// a string_t may not be a null terminated array
				if (((offset + length) == (stack->data + stack->length)) || offset[length] == sep)
					ret = 0;
			}
		} while (ret);
		if (ret == 0)
			break;
		offset = strchr(offset, sep);
		nail = nailorig;
		wildcard = 0;
		if (offset)
			offset++;
	}
	return ret;
}
#endif

int string_split(string_t *str, char sep, ...)
{
	int ret = 0;
#ifdef USE_STDARG
	va_list ap;
	va_start(ap, sep);
	for (size_t index = 0; index < str->length; index++)
	{
		string_t *arg = va_arg(ap, string_t *);
		if (arg == NULL || ret > 10) ///10 for max elements
			break;
		ret++;
		arg->data = &str->data[index];
		while (arg->data[arg->length] != sep &&
				arg->length < (str->length - (arg->data - str->data)))
			arg->length++;
		index += arg->length;
		if (arg->ddata)
		{
			string_cpy(arg, arg->data, arg->length);
			arg->data = arg->ddata;
		}
	}
	va_end(ap);
#endif
	return ret;
}

int string_is(const string_t *str1, const string_t *str2)
{
	if ((str1 == NULL) || (str2 == NULL))
		return 0;
	if ((str1->length != str2->length))
		return 0;
	if (!strncasecmp(str1->data, str2->data, str1->length))
		return 1;
	return 0;
}

int string_startwith(const string_t *str1, const string_t *str2)
{
	if ((str1 == NULL) || (str2 == NULL))
		return 0;
	if ((str1->length < str2->length))
		return 0;
	if (!strncasecmp(str1->data, str2->data, str2->length))
		return 1;
	return 0;
}

int string_empty(const string_t *str)
{
	return ! (str->data != NULL && str->data[0] != '\0' && str->length > 0);
}

int string_cpy(string_t *str, const char *source, size_t length)
{
	if (str->ddata == NULL)
		return EREJECT;
	if ((length == (size_t) -1) || (length > INT_MAX))
		str->length = snprintf(str->ddata, str->size, "%s", source);
	else
		str->length = snprintf(str->ddata, str->size, "%.*s", (int)length, source);
	if (str->length == str->size)
		return EREJECT;
	return ESUCCESS;
}

int string_printf(string_t *str, void *fmt,...)
{
	if (str->ddata != NULL)
	{
#ifdef USE_STDARG
		va_list ap;
		va_start(ap, fmt);
		str->length = vsnprintf(str->ddata, str->size, fmt, ap);
		va_end(ap);
		if (str->length < str->size)
			return ESUCCESS;
#endif
	}
	return EREJECT;
}

int string_dup(string_t *dst, string_t *src)
{
	if (string_empty(src))
		return EREJECT;
	dst->ddata = strndup(src->data, src->length);
	dst->data = dst->ddata;
	dst->size = src->length + 1;
	dst->length = src->length;
	return ESUCCESS;
}

size_t string_slice(string_t *str, int start, int length)
{
	if (start > 0)
	{
		str->data += start;
		str->length -= start;
	}
	if (length > 0 && length < str->length)
		str->length = length;
	return str->length;
}

int string_fgetline(string_t *str, FILE *file)
{
	if (str->ddata == NULL)
		return EREJECT;
	size_t length = 0;
#if 1
	do
	{
		char c = fgetc(file);
		if (c == EOF || c == '\n')
			break;
		str->ddata[length++] = c;
	} while (length < str->size);
#elif 0
	while (((str->ddata[length] = fgetc(file)) != EOF) &&
			(str->ddata[length] != '\n') &&
			(length < str->size)) length++;
#else
	ssize_t ret = getline(&str->ddata, &str->size, file);
	if (ret == -1)
		return EREJECT;
#endif
	if (str->ddata[length] == '\n')
		length--;
	if (length == 0)
		return EREJECT;
	str->length = length;
	return ESUCCESS;
}

const char *string_toc(const string_t *str)
{
	return str->data;
}

void string_cleansafe(string_t *str)
{
	volatile char *p = str->ddata;
	if (p == NULL)
	{
		warn("string: clean safe a static string");
		str->length = 0;
		return;
	}
	while (str->length--)
	{
		*p++ = (char)random();
	}
}

void string_destroy(string_t *str)
{
	if (str->ddata && ((void*)str->ddata != ((void *)str + sizeof(*str))))
		free(str->ddata);
	str->ddata = NULL;
	str->data = NULL;
	str->length = 0;
	str->size = 0;
	free(str);
}
