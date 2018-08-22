#include "buffer.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#if defined HAVE_STDINT_H
# include <stdint.h>
#elif defined HAVE_INTTYPES_H
# include <inttypes.h>
#endif

static const char hex_chars[] = "0123456789abcdef";


/**
 * init the buffer
 *
 */

buffer* buffer_init(void) {
	buffer *b;

	b = malloc(sizeof(*b));
	force_assert(b);

	b->ptr = NULL;
	b->size = 0;
	b->used = 0;

	return b;
}

buffer *buffer_init_buffer(buffer *src) {
	buffer *b = buffer_init();
	buffer_copy_string_buffer(b, src);
	return b;
}

/**
 * free the buffer
 *
 */

void buffer_free(buffer *b) {
	if (!b) return;

	free(b->ptr);
	free(b);
}

void buffer_reset(buffer *b) {
	if (!b) return;

	/* limit don't reuse buffer larger than ... bytes */
	if (b->size > BUFFER_MAX_REUSE_SIZE) {
		free(b->ptr);
		b->ptr = NULL;
		b->size = 0;
	} else if (b->size) {
		b->ptr[0] = '\0';
	}

	b->used = 0;
}


/**
 *
 * allocate (if neccessary) enough space for 'size' bytes and
 * set the 'used' counter to 0
 *
 */

#define BUFFER_PIECE_SIZE 64

int buffer_prepare_copy(buffer *b, size_t size) {
	if (!b) return -1;

	if ((0 == b->size) ||
	    (size > b->size)) {
		if (b->size) free(b->ptr);

		b->size = size;

		/* always allocate a multiply of BUFFER_PIECE_SIZE */
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);

		b->ptr = malloc(b->size);
		force_assert(b->ptr);
	}
	b->used = 0;
	return 0;
}

/**
 *
 * increase the internal buffer (if neccessary) to append another 'size' byte
 * ->used isn't changed
 *
 */

int buffer_prepare_append(buffer *b, size_t size) {
	if (!b) return -1;

	if (0 == b->size) {
		b->size = size;

		/* always allocate a multiply of BUFFER_PIECE_SIZE */
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);

		b->ptr = malloc(b->size);
		b->used = 0;
		force_assert(b->ptr);
	} else if (b->used + size > b->size) {
		b->size += size;

		/* always allocate a multiply of BUFFER_PIECE_SIZE */
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);

		b->ptr = realloc(b->ptr, b->size);
		force_assert(b->ptr);
	}
	return 0;
}

int buffer_copy_string(buffer *b, const char *s) {
	size_t s_len;

	if (!s || !b) return -1;

	s_len = strlen(s) + 1;
	buffer_prepare_copy(b, s_len);

	memcpy(b->ptr, s, s_len);
	b->used = s_len;

	return 0;
}

int buffer_copy_string_len(buffer *b, const char *s, size_t s_len) {
	if (!s || !b) return -1;
#if 0
	/* removed optimization as we have to keep the empty string
	 * in some cases for the config handling
	 *
	 * url.access-deny = ( "" )
	 */
	if (s_len == 0) return 0;
#endif
	buffer_prepare_copy(b, s_len + 1);

	memcpy(b->ptr, s, s_len);
	b->ptr[s_len] = '\0';
	b->used = s_len + 1;

	return 0;
}

int buffer_copy_string_buffer(buffer *b, const buffer *src) {
	if (!src) return -1;

	if (src->used == 0) {
		buffer_reset(b);
		return 0;
	}
	return buffer_copy_string_len(b, src->ptr, src->used - 1);
}

int buffer_append_string(buffer *b, const char *s) {
	size_t s_len;

	if (!s || !b) return -1;

	s_len = strlen(s);
	buffer_prepare_append(b, s_len + 1);
	if (b->used == 0)
		b->used++;

	memcpy(b->ptr + b->used - 1, s, s_len + 1);
	b->used += s_len;

	return 0;
}

int buffer_append_string_rfill(buffer *b, const char *s, size_t maxlen) {
	size_t s_len;

	if (!s || !b) return -1;

	s_len = strlen(s);
	if (s_len > maxlen)  s_len = maxlen;
	buffer_prepare_append(b, maxlen + 1);
	if (b->used == 0)
		b->used++;

	memcpy(b->ptr + b->used - 1, s, s_len);
	if (maxlen > s_len) {
		memset(b->ptr + b->used - 1 + s_len, ' ', maxlen - s_len);
	}

	b->used += maxlen;
	b->ptr[b->used - 1] = '\0';
	return 0;
}

/**
 * append a string to the end of the buffer
 *
 * the resulting buffer is terminated with a '\0'
 * s is treated as a un-terminated string (a \0 is handled a normal character)
 *
 * @param b a buffer
 * @param s the string
 * @param s_len size of the string (without the terminating \0)
 */

int buffer_append_string_len(buffer *b, const char *s, size_t s_len) {
	if (!s || !b) return -1;
	if (s_len == 0) return 0;

	buffer_prepare_append(b, s_len + 1);
	if (b->used == 0)
		b->used++;

	memcpy(b->ptr + b->used - 1, s, s_len);
	b->used += s_len;
	b->ptr[b->used - 1] = '\0';

	return 0;
}

int buffer_append_string_buffer(buffer *b, const buffer *src) {
	if (!src) return -1;
	if (src->used == 0) return 0;

	return buffer_append_string_len(b, src->ptr, src->used - 1);
}

int buffer_append_memory(buffer *b, const char *s, size_t s_len) {
	if (!s || !b) return -1;
	if (s_len == 0) return 0;

	buffer_prepare_append(b, s_len);
	memcpy(b->ptr + b->used, s, s_len);
	b->used += s_len;

	return 0;
}

int buffer_copy_memory(buffer *b, const char *s, size_t s_len) {
	if (!s || !b) return -1;

	b->used = 0;

	return buffer_append_memory(b, s, s_len);
}

int buffer_append_long_hex(buffer *b, unsigned long value) {
	char *buf;
	int shift = 0;
	unsigned long copy = value;

	while (copy) {
		copy >>= 4;
		shift++;
	}
	if (shift == 0)
		shift++;
	if (shift & 0x01)
		shift++;

	buffer_prepare_append(b, shift + 1);
	if (b->used == 0)
		b->used++;
	buf = b->ptr + (b->used - 1);
	b->used += shift;

	shift <<= 2;
	while (shift > 0) {
		shift -= 4;
		*(buf++) = hex_chars[(value >> shift) & 0x0F];
	}
	*buf = '\0';

	return 0;
}

int LI_ltostr(char *buf, long val) {
	char swap;
	char *end;
	int len = 1;

	if (val < 0) {
		len++;
		*(buf++) = '-';
		val = -val;
	}

	end = buf;
	while (val > 9) {
		*(end++) = '0' + (val % 10);
		val = val / 10;
	}
	*(end) = '0' + val;
	*(end + 1) = '\0';
	len += end - buf;

	while (buf < end) {
		swap = *end;
		*end = *buf;
		*buf = swap;

		buf++;
		end--;
	}

	return len;
}

int buffer_append_long(buffer *b, long val) {
	if (!b) return -1;

	buffer_prepare_append(b, 32);
	if (b->used == 0)
		b->used++;

	b->used += LI_ltostr(b->ptr + (b->used - 1), val);
	return 0;
}

int buffer_copy_long(buffer *b, long val) {
	if (!b) return -1;

	b->used = 0;
	return buffer_append_long(b, val);
}

#if !defined(SIZEOF_LONG) || (SIZEOF_LONG != SIZEOF_OFF_T)
int buffer_append_off_t(buffer *b, off_t val) {
	char swap;
	char *end;
	char *start;
	int len = 1;

	if (!b) return -1;

	buffer_prepare_append(b, 32);
	if (b->used == 0)
		b->used++;

	start = b->ptr + (b->used - 1);
	if (val < 0) {
		len++;
		*(start++) = '-';
		val = -val;
	}

	end = start;
	while (val > 9) {
		*(end++) = '0' + (val % 10);
		val = val / 10;
	}
	*(end) = '0' + val;
	*(end + 1) = '\0';
	len += end - start;

	while (start < end) {
		swap   = *end;
		*end   = *start;
		*start = swap;

		start++;
		end--;
	}

	b->used += len;
	return 0;
}

int buffer_copy_off_t(buffer *b, off_t val) {
	if (!b) return -1;

	b->used = 0;
	return buffer_append_off_t(b, val);
}
#endif /* !defined(SIZEOF_LONG) || (SIZEOF_LONG != SIZEOF_OFF_T) */

char int2hex(char c) {
	return hex_chars[(c & 0x0F)];
}

/* converts hex char (0-9, A-Z, a-z) to decimal.
 * returns 0xFF on invalid input.
 */
char hex2int(unsigned char hex) {
	hex = hex - '0';
	if (hex > 9) {
		hex = (hex + '0' - 1) | 0x20;
		hex = hex - 'a' + 11;
	}
	if (hex > 15)
		hex = 0xFF;

	return hex;
}


/**
 * init the buffer
 *
 */

buffer_array* buffer_array_init(void) {
	buffer_array *b;

	b = malloc(sizeof(*b));

	force_assert(b);
	b->ptr = NULL;
	b->size = 0;
	b->used = 0;

	return b;
}

void buffer_array_reset(buffer_array *b) {
	size_t i;

	if (!b) return;

	/* if they are too large, reduce them */
	for (i = 0; i < b->used; i++) {
		buffer_reset(b->ptr[i]);
	}

	b->used = 0;
}


/**
 * free the buffer_array
 *
 */

void buffer_array_free(buffer_array *b) {
	size_t i;
	if (!b) return;

	for (i = 0; i < b->size; i++) {
		if (b->ptr[i]) buffer_free(b->ptr[i]);
	}
	free(b->ptr);
	free(b);
}

buffer *buffer_array_append_get_buffer(buffer_array *b) {
	size_t i;

	if (b->size == 0) {
		b->size = 16;
		b->ptr = malloc(sizeof(*b->ptr) * b->size);
		force_assert(b->ptr);
		for (i = 0; i < b->size; i++) {
			b->ptr[i] = NULL;
		}
	} else if (b->size == b->used) {
		b->size += 16;
		b->ptr = realloc(b->ptr, sizeof(*b->ptr) * b->size);
		force_assert(b->ptr);
		for (i = b->used; i < b->size; i++) {
			b->ptr[i] = NULL;
		}
	}

	if (b->ptr[b->used] == NULL) {
		b->ptr[b->used] = buffer_init();
	}

	b->ptr[b->used]->used = 0;

	return b->ptr[b->used++];
}


char * buffer_search_string_len(buffer *b, const char *needle, size_t len) {
	size_t i;
	if (len == 0) return NULL;
	if (needle == NULL) return NULL;

	if (b->used < len) return NULL;

	for(i = 0; i < b->used - len; i++) {
		if (0 == memcmp(b->ptr + i, needle, len)) {
			return b->ptr + i;
		}
	}

	return NULL;
}

buffer *buffer_init_string(const char *str) {
	buffer *b = buffer_init();

	buffer_copy_string(b, str);

	return b;
}

int buffer_is_empty(buffer *b) {
	if (!b) return 1;
	return (b->used == 0);
}

/**
 * check if two buffer contain the same data
 *
 * HISTORY: this function was pretty much optimized, but didn't handled
 * alignment properly.
 */

int buffer_is_equal(buffer *a, buffer *b) {
	if (a->used != b->used) return 0;
	if (a->used == 0) return 1;

	return (0 == strcmp(a->ptr, b->ptr));
}

int buffer_is_equal_string(buffer *a, const char *s, size_t b_len) {
	buffer b;

	b.ptr = (char *)s;
	b.used = b_len + 1;

	return buffer_is_equal(a, &b);
}

/* buffer_is_equal_caseless_string(b, CONST_STR_LEN("value")) */
int buffer_is_equal_caseless_string(buffer *a, const char *s, size_t b_len) {
	if (a->used != b_len + 1) return 0;

	return (0 == strcasecmp(a->ptr, s));
}

int buffer_caseless_compare(const char *a, size_t a_len, const char *b, size_t b_len) {
	size_t const len = (a_len < b_len) ? a_len : b_len;
	size_t i;

	for (i = 0; i < len; ++i) {
		unsigned char ca = a[i], cb = b[i];
		if (ca == cb) continue;

		/* always lowercase for transitive results */
#if 1
		if (ca >= 'A' && ca <= 'Z') ca |= 32;
		if (cb >= 'A' && cb <= 'Z') cb |= 32;
#else
		/* try to produce code without branching (jumps) */
		ca |= ((unsigned char)(ca - (unsigned char)'A') <= (unsigned char)('Z' - 'A')) ? 32 : 0;
		cb |= ((unsigned char)(cb - (unsigned char)'A') <= (unsigned char)('Z' - 'A')) ? 32 : 0;
#endif

		if (ca == cb) continue;
		return ca - cb;
	}
	if (a_len == b_len) return 0;
	return a_len - b_len;
}

/**
 * check if the rightmost bytes of the string are equal.
 *
 *
 */

int buffer_is_equal_right_len(buffer *b1, buffer *b2, size_t len) {
	/* no, len -> equal */
	if (len == 0) return 1;

	/* len > 0, but empty buffers -> not equal */
	if (b1->used == 0 || b2->used == 0) return 0;

	/* buffers too small -> not equal */
	if (b1->used - 1 < len || b2->used - 1 < len) return 0;

	if (0 == strncmp(b1->ptr + b1->used - 1 - len,
			 b2->ptr + b2->used - 1 - len, len)) {
		return 1;
	}

	return 0;
}

int buffer_copy_string_hex(buffer *b, const char *in, size_t in_len) {
	size_t i;

	/* BO protection */
	if (in_len * 2 < in_len) return -1;

	buffer_prepare_copy(b, in_len * 2 + 1);

	for (i = 0; i < in_len; i++) {
		b->ptr[b->used++] = hex_chars[(in[i] >> 4) & 0x0F];
		b->ptr[b->used++] = hex_chars[in[i] & 0x0F];
	}
	b->ptr[b->used++] = '\0';

	return 0;
}

int light_isdigit(int c) {
	return (c >= '0' && c <= '9');
}

int light_isxdigit(int c) {
	if (light_isdigit(c)) return 1;

	c |= 32;
	return (c >= 'a' && c <= 'f');
}

int light_isalpha(int c) {
	c |= 32;
	return (c >= 'a' && c <= 'z');
}

int light_isalnum(int c) {
	return light_isdigit(c) || light_isalpha(c);
}

int buffer_to_lower(buffer *b) {
	char *c;

	if (b->used == 0) return 0;

	for (c = b->ptr; *c; c++) {
		if (*c >= 'A' && *c <= 'Z') {
			*c |= 32;
		}
	}

	return 0;
}


int buffer_to_upper(buffer *b) {
	char *c;

	if (b->used == 0) return 0;

	for (c = b->ptr; *c; c++) {
		if (*c >= 'a' && *c <= 'z') {
			*c &= ~32;
		}
	}

	return 0;
}

void log_failed_assert(const char *filename, unsigned int line, const char *msg) {
        /* can't use buffer here; could lead to recursive assertions */
        fprintf(stderr, "%s.%d: %s\n", filename, line, msg);
        fflush(stderr);
        abort();
}
