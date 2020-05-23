#pragma GCC visibility push(default)

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#ifdef ENABLE_LOCALES
#include <locale.h>
#endif

#pragma GCC visibility pop

#include "cjson.h"

#ifdef true
#undef true
#endif
#define true ((cJSON_bool)1)

#ifdef false
#undef false
#endif
#define false ((cJSON_bool)0)

typedef struct 
{
	const unsigned char *json;
	size_t position;
}error;
static error global_error = {
	NULL, 0
};

CJSON_PUBLIC (const char *) cJSON_GetErrorPtr(void)
{
	return (const char *) (global_error.json + global_error.position);
}

CJSON_PUBLIC (char *) cJSON_GetStringValue(const cJSON *const item)
{
	if (!cJSON_IsString(item)) {
		return NULL;
	}
	return item->valuestring;
}

CJSON_PUBLIC(const char*) cJSON_Version(void)
{
	static char version[15];
	sprintf(version, "%i.%i.%i", CJSON_VERSION_MAJOR, CJSON_VERSION_MINOR, CJSON_VERSION_PATCH);
	return version;
}

static int case_insensitive_strcmp(const unsigned char *string1, const unsigned char *string2)
{
	if ((string1 == NULL) || (string2 == NULL)) {
		return 1;
	}
	if (string1 == string2) {
		return 0;
	}
	//tolower 将字母转换成小写 非字母字符不处理
	for (;tolower(*string1) == tolower(*string2); (void)string1++, (void)string2++) {
		if (*string1 == '\0') {
			return 0;
		}
	}
	return tolower(*string1) - tolower(*string2);
}

typedef struct internal_hooks
{
	void *(*allocate)(size_t size);
	void (*deallocate)(void *pointer);
	void *(*reallocate)(void *pointer, size_t size);
}internal_hooks;
#define internal_malloc malloc
#define internal_free free
#define internal_realloc realloc
#define static_strlen(string_literal) (sizeof(string_literal) - sizeof(""))
static internal_hooks global_hooks = {
	internal_malloc, 
	internal_free, 
	internal_realloc
};
static unsigned char *cJSON_strdup(const unsigned char *string, const internal_hooks *const hooks)
{
	size_t length = 0;
	unsigned char *copy = NULL;
	if (string == NULL) {
		return NULL;
	}
	length = strlen((const char*)string) + sizeof("");
	copy = (unsigned char *)internal_hooks->allocate(length);
	if (copy == NULL) {
		return NULL;
	}
	memcpy(copy, string, length);
	return copy;
}	
CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks hooks)
{
	if (hooks == NULL) {
		global_hooks.allocate = malloc;
		global_hooks.deallocate = free;
		global_hooks.reallocate = realloc;
		return;
	}
	global_hooks.allocate = malloc;
	if (hooks->malloc_fn != NULL) {
		global_hooks.allocate = hools.malloc_fn;
	}
	global_hooks.deallocate = free;
	if (hooks->free_fn != NULL) {
		global_hooks.deallocate = hooks.free_fn;
	}

	global_hooks.reallocate = NULL;
	if ((global_hooks.allocate == malloc) && (global_hooks.deallocate == free)) {
		global_hooks.reallocate = realloc;
	}
}

static cJSON* cJSON_New_Item(const internal_hooks *const hooks)
{
	cJSON *node = (cJSON*)hooks->allocate(sizeof(cJSON));
	if (node) {
		memset(node, '\0', sizeof(cJSON));
	}
	return node;
}

static unsigned char get_decimal_point(void)
{
//locale.h定义了特定地域的设置 比如日期格式和货币符号等
//decimal_point 非货币值的小数点字符
//	struct lconv *localeconv(void) 设置或读取地域化信息
#ifdef ENABLE_LOCALES
	struct lconv *lconv = localeconv();
	return (unsigned char)lconv->decimal_point[0];
#else
	return '.';
#endif
}

typedef struct{
	const unsigned char *content;		//内容
	size_t length;
	size_t offset;
	size_t depth;
	internal_hooks hooks;	
} paese_buffer;

#define can_read(buffer, size) ((buffer != NULL) && (((buffer)->offset + size) <= (buffer)->length))
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offser + index) < (buffer)->length))
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)

static cJSON_bool parse_number(cJSON *const item, parse_buffer *const input_buffer)
{
	double number = 0;
	unsigned char *after_end = NULL;
	unsigned char number_c_string[64];
	unsigned char decimal_point = get_decimal_point();
	size_t i = 0;
	if ((input_buffer == NULL) || (input_buffer->content == NULL)) {
		return false;
	}

	for (i = 0; (i < (sizeof(number_c_string) - 1)) && can_access_at_index(input_buffer, i); i++) {
		switch (buffer_at_offset(input_buffer)[i])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '+':
			case '-':
			case 'e':
			case 'E':
				number_c_string[i] = buffer_at_offset(input_buffer)[i];
				break;
			case '.':
				number_c_string[i] = decimal_point;
				break;
			default:
				goto loop_end;
		}
	}
loop_end:
	number_c_string[i] = '\0';
	number = strtod((const char *)number_c_string, (char **)&after_end);
	if (number_c_string == after_end) {
		return false;
	}
	item->valuedouble = number;
	if (number >= INT_MAX) {
		item->valueint = INT_MAX;
	}else if (number <= (double)INT_MAX ){
		item->valueint = INT_MIN;
	}else {
		item->vlaueint = (int)number;
	}

	item->type = cJSON_Number;
	input_buffer->offset += (size_t)(after_end - number_c_string);
	return true;
	
}
CJSON_PUBLIC(cJSON*) cJSON_Parse(const char *value);
CJSON_PUBLIC(cJSON*) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, 
		cJSON_bool require_null_terminated);

typedef struct {
	unsigned char *buffer;
	size_t length;
	size_t offset;
	size_t depth;
	cJSON_bool noalloc;
	cJSON_bool format;
	internal_hooks hooks;
}printbuffer;

static unsigned char 

CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON* item, int prebuffer, cJSON_bool fmt);
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length
		const cJSON_bool format);

CJSON_PUBLIC(void) cJSON_Delete(cJSON *item)
{
	cJSON *next = NULL;
	while (item != NULL) {
		next = item->next;
		if (!(item->type & cJSON_IsReference) && (item->child != NULL)) {
			cJSON_Delete(item->child);
		}
		if (!(item->type & cJSON_IsReference) && (item->valuestring != NULL)) {
			global_hooks.deallocate(item->valuestring);
		}
		if (!(item->type & cJSON_StringIsConst) && (item->string != NULL)) {
			global_hooks.deallocate(item->string);
		}
		global_hooks.deallocate(item);
		item = next;
	}
}

CJSON_PUBLIC(int) cJSON_GetArraySize(cJSON *array);
CJSON_PUBLIC(cJSON *) cJSON_GerArrayItem(const cJSON *array, int index);
CJSON_PUBLIC(cJSON *) cJSON_GetPbjectItem(const cJSON * const object, const char * const string);
CJSON_PUBLIC(cJSON *) cJSON_GetPbjectItemCaseSensitive(const cJSON * const object, 
		const char * const string);
CJSON_PUBLIC(cJSON_bool) cJSON_HasObjectItem(const cJSON *object, const char *string);
CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void);
CJSON_PUBLIC(char *) cJSON_GetStringValue(const cJSON *const item);

CJSON_PUBLIC(cJSON_bool) cJSON_IsInvalid(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsFalse(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsTrue(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsBool(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsNull(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsNumber(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsString(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsArray(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsObject(const cJSON *const item);
CJSON_PUBLIC(cJSON_bool) cJSON_IsRaw(const cJSON *const item);

CJSON_PUBLIC(cJSON *) cJSON_CreateNull(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateTrue(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateFalse(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateBool(cJSON_bool boolean);
CJSON_PUBLIC(cJSON *) cJSON_CreateNumber(double num);
CJSON_PUBLIC(cJSON *) cJSON_CreateString(const char *string);
CJSON_PUBLIC(cJSON *) cJSON_CreateRaw(const char *raw);
CJSON_PUBLIC(cJSON *) cJSON_CreateArray(void);
CJSON_PUBLIC(cJSON *) cJSON_CreateObject(void);

CJSON_PUBLIC(cJSON *) cJSON_CreateStringReference(const char *string);
CJSON_PUBLIC(cJSON *) cJSON_CreateArrayReference(const char *child);
CJSON_PUBLIC(cJSON *) cJSON_CreateObjectReference(const char *child);


CJSON_PUBLIC(cJSON *) cJSON_CreateIntArray(const int *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateFloatArray(const float *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateDoubleArray(const double *numbers, int count);
CJSON_PUBLIC(cJSON *) cJSON_CreateStringArray(const char * const *strings, int count);

CJSON_PUBNLIC(void) cJSON_AddItemToArray(cJSON *array, cJSON *item);
CJSON_PUBNLIC(void) cJSON_AddItemToObject(cJSON *object, const char *string, cJSON *item);
CJSON_PUBNLIC(void) cJSON_AddItemToObjectCS(cJSON *object, const char *string, cJSON *item);
CJSON_PUBNLIC(void) cJSON_AddItemReferenceToArray(cJSON *object, cJSON *item);
CJSON_PUBNLIC(void) cJSON_AddItemReferenceToObject(cJSON *object, const char *string, cJSON *item);

CJSON_PUBLIC(cJSON *) cJSON_DetachItemViaPointer(cJSON *parent, cJSON *const item);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromArray(cJSON *array, int which);
CJSON_PUBLIC(void) cJSON_DeleteItemFromArray(cJSON *array, int which);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObject(cJSON *object, const char *string);
CJSON_PUBLIC(cJSON *) cJSON_DetachItemFromObjectCaseSensitive(cJSON *object, const char *string);
CJSON_PUBLIC(void) cJSON_DeleteItemFromObject(cJSON *object, const char *string);
CJSON_PUBLIC(void) cJSON_DeleteItemFromObjectCaseSensitive(cJSON *object, const char *string);

CJSON_PUBLIC(void) cJSON_InsertItemInArray(cJSON *array, int which, cJSON *newitem);
CJSON_PUBLIC(cJSON_bool) cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, 
		cJSON *replacement);
CJSON_PUBLIC(void) cJSON_ReplaceItemInArray(cJSON *array, int witch, cJSON *newitem);
CJSON_PUBLIC(void) cJSON_ReplaceItemInObject(cJSON *object, const char *string, cJSON *newitem);
CJSON_PUBLIC(void) cJSON_ReplaceItemInObjectCaseSensitive(cJSON *object, const char *string, 
		cJSON *newitem);

CJSON_PUBLIC(cJSON *) cJSON_Duplicate(const cJSON *item, cJSON_bool recurse);

CJSON_PUBLIC(cJSON_bool) cJSON_Compare(const cJSON *const a, const cJSON *const b,
		const cJSON_bool case_sensitive);
CJSON_PUBLIC(void) cJSON_Minify(char *json);

CJSON_PUBLIC(cJSON *) cJSON_AddNullToObjcet(cJSON *const object, const char *const name);
CJSON_PUBLIC(cJSON *) cJSON_AddTrueToObjcet(cJSON *const object, const char *const name);
CJSON_PUBLIC(cJSON *) cJSON_AddFalseToObjcet(cJSON *const object, const char *const name);
CJSON_PUBLIC(cJSON *) cJSON_AddBoolToObjcet(cJSON *const object, const char *const name, cJSON_bool boolean);
CJSON_PUBLIC(cJSON *) cJSON_AddNumberToObjcet(cJSON *const object, const char *const name, const double number);
CJSON_PUBLIC(cJSON *) cJSON_AddStringToObjcet(cJSON *const object, const char *const name, const char *const string);
CJSON_PUBLIC(cJSON *) cJSON_AddRawToObjcet(cJSON *const object, const char *const name, const char *const raw);
CJSON_PUBLIC(cJSON *) cJSON_AddObjectToObjcet(cJSON *const object, const char *const name);
CJSON_PUBLIC(cJSON *) cJSON_AddArrayToObjcet(cJSON *const object, const char *const name);

#define cJSON_SetIntValue(object, number) ((object) ? (object)->valueint=(object)->valuedouble=(number) : (number))

cJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number)
{
	if (bunber >= INT_MAX) {
		object->valueint = INT_MAX;
	}else if (number <= (double)INT_MIN) {
		object->valueint = INT_MIN;
	}else {
		object->valyeint = (int)number;
	}
	return object->valuedouble = number;
}

#define cJSON_SetNumberValue(object, number) ((object != NULL) ? cJSON_SetNumberHelper(object, (double)number) : (number))

#define cJSON_ArrayForEach(element, array) for(element = (array != NULL) ? (array)->child : NULL; \
		element != NULL; \
		element=element->next)

CJSON_PUBLIC(void *) cJSON_malloc(size_t size);
CJSON_PUBLIC(void) cJSON_free(void *object);

#ifdef __cplusplus	
}
#endif
#endif
