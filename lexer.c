/* MIT License
 *
 * Copyright (c) Farid Farajli
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ctype.h>
#include <stdio.h>

#define LEN(arr) (sizeof (arr) / sizeof (arr[0]))

typedef struct {
	char *pos;
	size_t len;
} Token;

char *
skip_whitespace(const char *str)
{
	while (isspace(*str))
		str += 1;
	return (char *)str;
}

size_t
tokenize(char *str, Token *buf, size_t capacity)
{
	size_t bufpos = 0;

	while (bufpos != capacity && *str) {
		size_t toklen = 0;

		str = skip_whitespace(str);

		switch (*str) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			toklen = 1;
			goto new_token;
		}

		while (str[toklen] && !isspace(str[toklen]))
			toklen += 1;

	new_token:
		buf[bufpos].pos = str;
		buf[bufpos].len = toklen;

		str += toklen;
		bufpos += 1;
	}

	return bufpos;
}

int
main(void)
{
	char source[] = "+-*/%";
	Token buf[100];
	size_t token_count;

	token_count = tokenize(source, buf, LEN(buf));

	for (size_t i = 0; i != token_count; i++) {
		printf("token[%zu]: ", i);
		fwrite(buf[i].pos, buf[i].len, 1, stdout);
		puts("");
	}

	return 0;
}
