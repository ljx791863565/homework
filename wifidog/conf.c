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
	oGatewayID,
	oGatewayInterface,
	oGatewayAddress,
	oGatewayPort,
	ooAuthServerDeltaTraffic,
	oAuthServer,
	oAuthServHostname,
	oAuthServSSLAvailable,
	oAuthServSSLPort,
	oAuthServHTTPPort,
	oAuthServPath,
	oAuthServLoginScriptPathFragment,
	oAuthServPortalScriptPathFragment,
	oAuthServMsgScriptPathFragment,
	oAuthServPingScriptPathFragment,
	oAuthServAuthScriptPathFragment,
	oHTTPDMaxConn,
	oHTTPDName,
	oHTTPDRealm,
	oHTTPDUsername,
	oHTTPDPAssword,
	oClientTimeout,
	oCheckInterval,
	oWdctlSocket,
	oSyslogFacility,
	oFirewallRule,
	oFirewallRuleSet,
	oTrustedMacList,
	oPopularServers,
	oHtmlMessageFile,
	oProxyPort,
	oSSLPeerVerification,
	oSSLCertPath,
	oSSLAllowedCipherList,
	oSSLUseSNI,
} OpCodes;

static const struct {
	const char *name;
	OpCodes opcode;
} keywords[] = {
	{ "deltatraffic", oDeltaTraffic},
	{ "daemon", oDeamon},
	{ "debuglevel", oDebugLevel},
	{ "externalinterface", oExternalInterface},
	{ "gatewayid", oGatewayID},
	{ "gatewayinterface", oGatewayInterface},
	{ "gatewayaddress", oGatewayAddress},
	{ "gatewayport", oGatewayPort},
	{ "authserver", oAuthServer},
	{ "httpdmaxconn", oHTTPDMaxConn},
	{ "httpdname", oHTTPDName},
	{ "httpdrealm", oHTTPDRealm},
	{ "httpdusername", oHTTPDUsername},
	{ "httpdpassword", oHTTPDPAssword},
	{ "clienttimeout", oClientTimeout},
	{ "checkinterval", oCheckInterval},
	{ "syslogfacility", oSyslogFacility},
	{ "wdctlsocket", oWdctlSocket},
	{ "hostname", oAuthServHostname},
	{ "sslavailable", oAuthServSSLAvailable},
	{ "sslport", oAuthServSSLPort},
	{ "httpport", oAuthServHTTPPort},
	{ "path", oAuthServPath},
	{ "loginscriptpathfragment", oAuthServLoginScriptPathFragment},
	{ "portalscriptpathfragment", oAuthServPortalScriptPathFragment},
	{ "msgscriptpathfragment", oAuthServMsgScriptPathFragment},
	{ "pingscriptpathfragment", oAuthServPingScriptPathFragment},
	{ "authscriptpathfragment", oAuthServAuthScriptPathFragment},
	{ "firewallrule", oFirewallRule},
	{ "firewallruleset", oFirewallRuleSet},
	{ "trustedmaclist", oTrustedMacList},
	{ "popularservers", oPopularServers},
	{ "htmlmessagefile", oHtmlMessageFile},
	{ "proxyport", oProxyPort},
	{ "sslpeerverification", oSSLPeerVerification},
	{ "sslcertpath", oSSLCertPath},
	{ "sslallowedcipherlist", oSSLAllowedCipherList},
	{ "sslusesni", oSSLUseSNI},
	{ "NULL", oBadOption},
};

s_config *config_get_config(void)
{
	return &config;
}

void config_init(void)
{
	debug(LOG_DEBUG, "Setting default config parameters");
	config.configfile = safe_strdup(DEFAULT_CONFIGFILE);
	config.htmlmsgfile = safe_strdup(DEFAULT_HTMLMSGFILE);
	config.wdctl_sock = safe_strdup(DEFAULT_WDCTL_SOCK);
	config.internal_sock = safe_strdup(DEFAULT_INTERNAL_SOCK);
	config.deltatraffic = DEFAULT_DELTATRAAFFIC;
	config.daemon = -1;
	config.pidfile = NULL;
	config.external_interface = NULL;
	config.gw_id = DEFAULT_GATEWAYID;
	config.gw_interface = NULL;
	config.gw_address = NULL;
	config.gw_port = DEFAULT_GATEWAYPORT;
	config.auth_servers = NULL;
	config.httpdname = NULL;
	config.httpdmaxconn = DEFAULT_HTTPDMAXCONN;
	config.httpdrealm = DEFAULT_HTTPDNAME;
	config.httpdusername = NULL;
	config.httpdpassword = NULL;
	config.clienttimeout = DEFAULT_CLIENTTIMEOUT;
	config.checkinterval = DEFAULT_CHECKINTRTVAL;
	config.rulesets = NULL;
	config.trustedmaclist = NULL;
	config.popular_servers = NULL;
	config.proxy_port = 0;
	config.ssl_certs = safe_strdup(DEFAULT_AUTHSERVSSLCERTPATH);
	config.ssl_verify = DEFAULT_AUTHSERVSSLPEERVER;
	config.ssl_cipher_list = NULL;
	config.ssl_use_sni = DEFAULT_AUTHSERVSSLSNI;
	config.arp_table_path = safe_strdup(DEFAULT_ARPTABLE);
}

void config_init_override(void)
{
	if (config.daemon == -1) {
		config.daemon = DEFAULT_DEAMON;
	}
}

/*
 * 从config file 中读取到的参数名确定其对应的配置选项
 */
static OpCodes config_parse_token(const char *cp, const char *filename, int linenum)
{
	int i;
	for (i = 0; keywords[i],name; i++) {
		if (strcasecmp(cp, keywords[i],name) == 0) {
			return keywords[i].opcode;
		}
	}
	debug(LOG_ERR, "%s: line %d: Bad configuration option: %s", filename, linenum, cp);
	return oBadOption;
}

/*
 * 从config file中解析服务器信息
 */
static void parse_auth_server(FILE *file, const char *filename, int *linenum)
{
	char	*host = NULL,
			*path = NULL,
			*loginscriptpathfragment = NULL,
			*portalscriptpathfragment = NULL,
			*msgscriptpathfragment = NULL,
			*pingscriptpathfragment = NULL,
			*authscriptpathfragment = NULL,
			*line[BUFSIZE]

}
