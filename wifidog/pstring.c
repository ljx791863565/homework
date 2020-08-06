#include <string.h>
#include <stdlib.h>

#include "pstring.h"
#include "safe.h"
#include "common.h"

pstr_t *pstr_new(void)
{
	pstr_t new;
	new = (pstr_t)safe_malloc(sizeof(pstr_t));
	new->len = 0;
	new->size = BUFSIZE;
	new->buf = (char *)safe_malloc(BUFSIZE);

	return new;
}

char *pstr_to_string(pstr_t *pstr)
{
	char *ret = pstr->buf;
	free(pstr);
	return ret;
}

static void _pstr_grow(pstr_t pstr)
{
	pstr->buf = (char *)safe_realloc((void *)pstr->buf, (pstr->size + BUFSIZE));
	pstr->size += BUFSIZE;
}

void pstr_cat(pstr_t *pstr, const char *string)
{
	size_t len = strlen(string);
	while ( (pstr->len + len + 1) > pstr->size ) {
		_pstr_grow(pstr);
	}
	strncat((pstr->buf + pstr->len), string, (pstr->size - pstr->len -1));
	pstr->len += len;
}

int pstr_append_sprintf(pstr_t *pstr, const char *fmt, ...)
{
	va_list ap;
	char *str;
	int retval;

	va_start(ap, fmt);
	retval = safe_vsfprintf(&str, fmt, ap);
	va_end(ap);
	if (retval >= 0) {
		pstr_cat(pstr, str);
		free(str);
	}
	return retval;
}
