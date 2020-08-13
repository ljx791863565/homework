#ifndef _CONF_H_
#define _CONF_H_

/*==============================DEFINE=====================================*/
#ifndef SYSCONFDIR
#define DEFAULT_CONFIGFILE "/etc/wifidog.conf"
#define DEFAULT_HIMLMSGFILE "/etc/wifidog-msg.html"
#else
#define DEFAULT_CONFIGFILE SYSCONFDIR"/wifidog.conf"
#define DEFAULT_HIMLMSGFILE SYSCONFDIR"/wifidog-msg.html"
#endif

#define DEFAULT_DAEMON 1
#define DEFAULT_DEBUGLEVEL LOG_INFO
#define DEFAULT_HTTPDMAXCONN 10
#define DEFAULT_GATEWAYID NULL
#define DEFAULT_GATEWAYPORT 2060
#define DEFAULT_HTTPDNAME "WiFiDog"
#define DEFAULT_CLIENTTIMEOUT 5
#define DEFAULT_CHECLINTERVAL 60
#define DEFAULT_LOG_SYSLOG 0
#define DEFAULT_SYSLOG_FACILITY LOG_DAEMON
#define DEFAULT_WDCTL_SOCK "/tmp/wdctl.sock"
#define DEFAULT_INTERNAL_SOCK "tmp/wifidog.sock"
#define DEFAULT_AUTHSERVPORT 80
#define DEFAULT_AUTHSERVSSLPORT 443
#define DEFAULT_AUTHSERVSSLAVAILABLE 0
#define DEFAULT_AUTHSERVPATH "/wifidog/"
#define DEFAULT_AUTHSERVLOGINPATHFRAGMENT "login/?"
#define DEFAULT_AUTHSERVPORTALPATHFRAGMENT "portal/?"
#define DEFAULT_AUTHSERVMSGPATHFRAGMENT "gw_message.php"
#define DEFAULT_AUTHSERVPINGPATHFRAGMENT "ping/?"
#define DEFAULT_AUTHSERVAUTHPATHFRAGMENT "auth/?"
#define DEFAULT_AUTHSERVSSLCERTPATH "/etc/ssl/certs"
#define DEFAULT_AUTHSERVSSLPEERVER 1
#define DEFAULT_DELTATRAFFIC 0
#define DEFAULT_ARPTABLE "/proc/net/arp"
#define DEFAULT_AUTHSERVSSLSNI 0
#define FWRULESET_GLOBAL "global"
#define FWRULESET_VALIDATING_USERS "validating_users"
#define FWURLESET_KNOWN_USERS "known-users"
#define FWURLESET_AUTH_IS_DOWN "auth-is-down"
#define FWRULESET_UNKONWN_USERS "unknown-users"
#define FWURLESET_LOCKED_USERS "locked-users"
/*==============================Data Structre=====================================*/
extern pthread_mutex_t config_mutex;
//认证服务器信息结构
typedef struct _auth_serv_t {
	char *authserv_hostname;	//服务器主机名
	int authserv_http_port;		//服务器http端口号
	int authserv_ssl_port;		//服务器https端口号	
	char *authserv_use_ssl;		//是否使用SSL Secure Sockets Layer 安全套接字协议 在传输层与应用层之间对网络连接进行加密
	char *authserv_path;
	
	char *authserv_login_script_path_fragment;
	char *authserv_portal_script_path_fragment;
	char *authserv_msg_script_path_fragment;
	char *authserv_ping_script_path_fragment;
	char *authserv_auth_script_path_fragment;
	
	char *last_ip;
	struct _auth_serv_t *next;
} t_auth_serv;


//mac白名单链表结构
typedef struct _trusted_mac_t {
	char *mac;
	struct _trusted_mac_t *next;
} t_trusted_mac;

//firewall target enum
typedef enum {
	TARGET_DROP,
	TARGET_REJECT,
	TARGET_ACCEPT,
	TARGET_LOG,
	TARGET_ULOG
} t_firewall_target;

//防火墙规则结构
typedef struct _firewall_rule_t {
	t_firewall_target target;
	char *protocol;			//协议相关 tcp udp...
	char *port;				//阻塞/放行的端口
	char *mask;				
	int mask_is_ipset;		
	struct _firewall_rule_t *next;
}t_firewall_rule;

//设置防火墙规则结构
typedef struct _firewall_ruleset_t {
	char *name;
	t_firewall_rule *rules;
	struct _firewall_ruleset_t *next;
} t_firewall_ruleset;

typedef struct _popular_server_t {
	char *hostname;
	struct _popular_server_t *next;
} t_popular_server;


//wifidog 基础配置结构
typedef struct {
	char *configfile;		//配置文件名
	char *htmlmsgfile;		//用于消息的HTML文件名
	char *wdctl_sock;
	char internal_sock;
	int deltarraffic;		//在每次Auth操作后重置每个用户的流量（输出和传入）值
	int daemon;				//如果daemon > 0, 使用daemon mode 即守护进程
	char *pidfile;
	char *external_interface;	//防火墙规则下的外网接口名
	char *gw_id;			//发送到服务器的网关ID 即可以是br-lan的mac
	char *gw_interface;		//接受连接的网络接口
	char *gw_address;		//web IP地址	如192.168.1.1
	char *gw_port;			//web 运行的端口 如80

	t_auth_serv *auth_servers;
	t_firewall_ruleset *rulesets;
	t_trusted_mac *trustedmaclist;
	t_popular_server *popular_servers;

	char *httpdname;		//web服务器返回时回复的请求名
	int httpdmaxconn;		//由libhttpd使用

	char *httpdrealm;		//用于HTTP认证
	char *httpdusername;	//用于HTTP身份验证的用户名
	char *httpdpassword;	//用于HTTP身份认密码

	int clienttimeout;		//向服务器auth counters的时间间隔
	int checkinterval;		//向服务器auth logout的时间间隔

	//代理服务器端口 0表示禁用 
	//代理网络用户去取得网络信息 代理服务器是介于浏览器和Web服务器之间的另一台服务器
	//有了它之后，浏览器不是直接到Web服务器去取回网页而是向代理服务器发出请求
	//信号会先送到代理服务器，由代理服务器来取回浏览器所需要的信息并传送给你的浏览器
	//将远程之前所存取过的档案、网页数据，在近端的proxy内复制一份备份
	//若联机设定有指定proxy的话，则当每次在连结到网页时，都会先检查之前是否有其它人上过同样的网站
	//有的话就可以直接传回数据，不必连接到外面
	int proxy_port;			

	char *ssl_certs;		//auth服务器SSL证书路径
	int ssl_verify;			//是否启用auth服务器证书认证 0不启用 1 启用 类似boolean值
	char *ssl_cipher_list;	//允许使用SSL密码链表 可选
	int ssl_use_sni;		//是否启用Auth服务器进行服务器名称指示，TLS扩展 boolean值

	char *arp_table_path;	//格式化 ARP表的路径 如/proc/net/arp
} s_config;

/*==============================API=====================================*/

s_config *config_get_config(void);

void config_init(void);

//用命令行覆盖 config 初始化
void config_init_override(void);

//读取配置文件信息
void config_read(const char *filename);

//判断配置是否有效
void config_validate(void);

t_auth_serv *get_auth_server(void);

//将服务器信息转存到list底部
void mark_auth_server_bad(t_auth_serv *as);

//firewall relu set
t_firewall_rule *get_ruleset(const char *);

#define LOCK_CONFIG() do {\
	debug(LOG_DEBUG, "Locking config"); \
	pthread_mutex_lock(&config_mutex);\
	debug(LOG_DEBUG, "Config locked");\
} while (0)

#define UNLOCK_CONFIG() do {\
	debug(LOG_DEBUG, "Unlocking config"); \
	pthread_mutex_unlock(&config_mutex);\
	debug(LOG_DEBUG, "Config unlocked");\
} while (0)
#endif //_CONF_H_
