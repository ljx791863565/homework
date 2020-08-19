#ifndef _CJSON_H_
#define _CJSON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#define cJSON_Invalid (0)
#define cJSON_False (1 << 0)
#define cJSON_True	(1 << 1)
#define cJSON_NULL	(1 << 2)
#define cJSON_Number (1 << 3)
#define cJSON_String (1 << 4)
#define cJSON_Array	(1 << 5)
#define cJSON_Object (1 << 6)
#define cJSON_Raw	(1 << 7)

#define cJSON_IsReference 256
#define cJSON_StringIsConst 512

#define CJSON_VERSION_MAJOR 1
#define CJSON_VERSION_MINOR 7
#define CJSON_VERSION_PATCH 12

/*
 * default 可见性是默认的符号链接可见性
 * 如果我们不指定visibility 属性，那么默认就使用默认的可见性。
 * 默认可见性的对象与函数可以直接在其他模块中引用，包括在动态链接库中 ，它属于一个正常，完整的外部连接。
 */
#define CJSON_PUNLIC(type) __attribute__((visibility("default")))type
typedef struct cJSON
{
	struct cJSON *next;
	struct cJSON *prev;
	struct cJSON *child;
	
	//对象的类型
	int type;		
	
	//如果 type == cJSON_String 则是字符串
	char *Valuestring;
	int valueint;
	double valuedouble;

	//对象名称
	char *string
}cJSON;

typedef struct cJSON_Hooks
{
	void *(*malloc_fn)(size_t sz);
	void (*free_fn)(void *ptr);
}cJSON_Hooks;

typedef int cJSON_bool;

#ifndef CJSON_NESTING_LIMIT
#define CJSON_NESTING_LIMIT 1000

CJSON_PUBLIC(const char*) cJSON_Version(void);
CJSON_PUBLIC(void) cJSON_InitHooks(cJSON_Hooks hooks);


CJSON_PUBLIC(cJSON*) cJSON_Parse(const char *value);
CJSON_PUBLIC(cJSON*) cJSON_ParseWithOpts(const char *value, const char **return_parse_end, 
		cJSON_bool require_null_terminated);

CJSON_PUBLIC(char *) cJSON_Print(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintUnformatted(const cJSON *item);
CJSON_PUBLIC(char *) cJSON_PrintBuffered(const cJSON* item, int prebuffer, cJSON_bool fmt);
CJSON_PUBLIC(cJSON_bool) cJSON_PrintPreallocated(cJSON *item, char *buffer, const int length
		const cJSON_bool format);

CJSON_PUBLIC(void) cJSON_Delete(cJSON *item);

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

cJSON_PUBLIC(double) cJSON_SetNumberHelper(cJSON *object, double number);

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
