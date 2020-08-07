#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "common.h"
#include "safe.h"
#include "debug.h"
#include "http.h"
#include "auth.h"
#include "firewall.h"
#include "util.h"

static s_config config;

pthread_mutex_t config_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef enum {
	oBadOption,
	oDaemon,
	oDebugLevel,
	oExternalInterface,
} OpCodes;

static const struct {
	const char *name;
	OpCodes opcode;
} keywords[] = {
	{ "daemon", oDeamon},
	{ "debuglevel", oDebugLevel},
	{ "externalinterface", oExternalInterface},
	{ "NULL", oBadOption},
};

s_config *config_get_config(void)
{
	return &config;
}

void config_init(void)
{
	debug(LOG_DEBUG, "Setting default config parameters");
	config.external_interface = NULL;
	config.daemon = 1;
}
