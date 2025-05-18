/*****************************************************************************
 * charstring.h: 
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

#ifndef __OUITILS_CHARSTRING_H__
#define __OUITILS_CHARSTRING_H__

typedef struct string_s string_t;
struct string_s
{
	const char *data;
	size_t length;
	size_t size;
	char *ddata;
};

#define STRING_REF(string) string, sizeof(string)-1
#define STRING_INFO(string) string.data, string.length
#define STRING_DCL(string) {.data=string, .size=sizeof(string), .length=sizeof(string)-1}
string_t *string_create(size_t size);
void string_debug(string_t *str, const char *name);
int string_store(string_t *str, const char *pointer, size_t length);
int string_cmp(const string_t *str, const char *cmp, size_t length);
int string_contain(const string_t *str, const char *cmp, size_t length, const char sep);
int string_is(const string_t *str1, const string_t *str2);
int string_startwith(const string_t *str1, const string_t *str2);
int string_printf(string_t *str, void *fmt,...);
int string_fgetline(string_t *str, FILE *f);
int string_cpy(string_t *str, const char *source, size_t length);
int string_empty(const string_t *str);
const char *string_toc(const string_t *str);
size_t string_length(const string_t *str);
size_t string_size(const string_t *str);
int string_split(string_t *str, char sep, ...);
size_t string_slice(string_t *str, int start, int length);
int string_dup(string_t *dst, string_t *src);
void string_cleansafe(string_t *str);
void string_destroy(string_t *str);

#endif
