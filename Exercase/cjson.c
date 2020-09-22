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

static unsigned char *ensure(printbuffer *const p, size_t needed) 
{
	unsigned char *newbuffer = NULL;
	size_t newsize = 0;

	if ((p == NULL) || (p->buffer == NULL)){
		return NULL;
	}
	if ((p->length >0) && (p->offset >= p->length)) {
		return NULL;
	}
	if (needed > INT_MAX) {
		return NULL;
	}
	//不需要扩容
	needed += p->offset + 1;
	if (needed <= p->length) {
		return p->buffer + p->offset;
	}
	if (p->noalloc) {
		return NULL;
	}

	//确定newsize大小 < INT_MAX
	if (needed > (INT_MAX / 2)) {
		if (needed <= INT_MAX) {
			newsize = INT_MAX;
		}else {
			return NULL;
		}
		
	}else {
		newsize = needed * 2;
	}
	
	//申请扩大内存
	if (p->hooks.reallocate != NULL) {
		newbuffer = (unsigned char *)p->hooks.reallocate(p->buffer, newsize);
		if (newbuffer == NULL) {
			p->hooks.deallocate(p->buffer);
			p->length = 0;
			p->buffer = NULL;
			return NULL;
		}
	}else {
		newbuffer = (unsigned char *)p->hooks.allocate(newsize);
		if (!newbuffer) {
			p->hooks.deallocate(p->buffer);
			p->length = 0;
			p->buffer = NULL;
			return NULL;
		}
		if (newbuffer) {
			memcpy(newbuffer, p->buffer, p->offset+1);
		}
		p->hooks.deallocate(p->buffer);
	}
	p->length = newsize;
	p->buffer = newbuffer;

	return newbuffer + p->offset;
	
	
}

static void update_offset(printbuffer *const buffer)
{
	const unsigned char *p_buffer = NULL;
	if ((buffer == NULL) || (buffer->buffer == NULL)) {
		return;
	}
	p_buffer = buffer->buffer + buffer->offset;
	buffer->offset += strlen((const char *)p_buffer);
}

static cJSON_bool print_number(const cJSON *item, printbuffer * const output_buffer)
{
	unsigned char *p_output = NULL;
	double d = item->valuedouble;
	int length = 0;
	size_t i = 0;
	unsigned char number_buffer[26] = {0};
	unsigned char decimal_point = get_decimal_point();
	double test = 0.0;

	if (output_buffer == NULL) {
		return false;
	}
	if ((d * 0) != 0) {
		length = sprintf((char*)number_buffer, "null");
	}else {
		length = sprintf((char*)number_buffer, "%1.15g", d);
		if ((sscanf((char*)number_buffer, "%1g", &test) != 1) || ((double)test != d)) {
			length = sprintf((char*)number_buffer, "%1.17g", d);
		}
	}
	if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1))) {
		return false;
	}
	p_output = ensure(optput_buffer, (size_t)length	 + sizeof(""));
	if (p_output == NULL) {
		return false;
	}

	for (i = 0; i < ((size_t)length); i++) {
		if (number_buffer[i] == decimal_point) {
			p_output[i] = '.';
			continue;
		}
		p_output[i] = number_buffer[i];
	}
	p_output[i] = '\0';
	output_buffer->offset += (size_t)length;

	return true;
	
}

//解析成4位的十六进制数
static unsigned parse_hex4(const unsigned char * const input)
{
	unsigned int hex = 0;
	size_t i = 0;
	for (i = 0; i < 4; i++) {
		if ((input[i] >= '0') && (input[i] <= '9')) {
			hex += (unsigned int)input[i] - '0';
		}else if ((input[i] >= 'A') && (input[i] <= 'F')){
			hex += (unsigned int)10 + input[i] - 'A';
		}else if ((input[i] >= 'a') && (input[i] <= 'f')) {
			hex += (unsigned int)10 + input[i] - 'a';
		}else {
			return 0;
		}
		if (i < 3) {
			hex <<= 4;
		}
	}
	return hex;
}

//UTF-8, UTF-16, UTF-32 就是 Unicode 不同的实现
static unsigned char utf16_literal_to_utf8(
		const unsigned char * const input_pointer,
		const unsigned char *const input_end, 
		unsigned char **output_pointer)
{
	long unsigned int codepoint = 0;
	unsigned int first_code = 0;
	const unsigned char *first_sequence = input_pointer;
	unsigned char utf8_length = 0;
	unsigned char utf8_position = 0;
	unsigned char sequence_length = 0;
	unsigned char first_byte_mark = 0;

	if ((input_end - first_sequence) < 6) {
		goto fail;
	}
	first_code = parse_hex4(first_sequence +2);

	if ((first_code >= 0xD800) && (first_code <= 0xDBFF)) {
		const unsigned char *second_sequence = first_sequence + 6;
		unsigned int second_code = 0;
		sequence_length = 12;
		if ((input_end - second_sequence) < 6) {
			goto fail;
		}
		if ((second_sequence[0] != '\\') || (second_sequence[i] != 'u')) {
			goto fail;
		}
		second_code = parse_hex4(second_sequence + 2);
		if ((second_code < 0xDC00) || (second_code > 0xDFFF)) {
			goto fail;
		}
		codepoint = 0x10000 + (((first_code & 0x3FF) << 10) | (second_code & 0x3FF));
	}else {
		sequence_length = 6;
		codepoint = first_code;
	}

	if (codepoint < 0x80) {
		utf8_length = 1;
	}else if (codepoint < 0x800) {
		utf8_length = 2;
		first_byte_mark = 0xC0;
	}else if (codepoint < 0x10000) {
		utf8_length = 3;
		first_byte_mark = 0xE0;
	}else if (codepoint <= 0x10FFFF) {
		utf8_length = 4;
		first_byte_mark = 0xF0;
	}else {
		goto fail;
	}
	
	for (utf8_position = (unsigned char)(utf8_length -1); utf8_position > 0; utf8_position--) {
		(*output_pointer)[utf8_position] = (unsigned char)((codepoint | 0x80) & 0xBF);
		codepoint >>= 6;
	}
	if (utf8_length > 1) {
		(*output_pointer)[0] = (unsigned char)((codepoint | first_byte_mark) & 0xFF);
	}else {
		(*output_pointer)[0] = (unsigned char)((codepoint & 0x7F));
	}

	*output_pointer += utf8_length;
	return sequence_length;
	
fail:
	return 0;
}

static cJSON_bool parse_string(cJSON *const item, parse_buffer *const input_buffer)
{
	const unsigned char *input_pointer = buffer_at_offset(input_buffer) + 1;
	const unsigned char *input_end = buffer_at_offset(input_buffer)	+ 1;
	unsigned char *output_pointer = NULL;
	unsigned char *output = NULL;

	//不是"开头表示不是string
	if (buffer_at_offset(input_buffer)[0] != '\"') {
		goto fail;
	}
	{
		size_t allocation_length = 0;
		size_t skipped_bytes = 0;
		while (((size_t)(input_end - input_buffer->content) < input_buffer->length) && (*input_end != '\"')) {
			if (input_end[0] == '\\') {
				if ((size_t)(input_end + 1 - input_buffer->content) >= input_buffer->length) {
					goto fail;
				}
				skipped_bytes++;
				input_end++;
			}
			input_end++;
		}
		if (((size_t)(input_end - input_buffer->content) >= input_buffer->length) || (*input_end != '\"')) {
			goto fail;
		}
		allocation_length = (size_t)(input_end - buffer_at_offset(input_buffer)) - skipped_bytes;
		output = (unsigned char *)input_buffer->hooks.allocate(allocation_length + sizeof(""));
		if (output == NULL) {
			goto fail;
		}
	}
	output_pointer = output;
	while (input_pointer < input_end) {
		if (*input_pointer != '\\') {
			*output_pointer++ = *input_pointer++;
		}else {
			unsigned char sequence_length = 2;
			if ((input_end - input_pointer) < 1) {
				goto fail;
			}
			switch (input_pointer[1])
			{
				case 'b':
					*output_pointer++ = '\b';
					break;
				case 'f':
					*output_pointer++ = '\f';
					break;
				case 'n':
					*output_pointer++ = '\n';
					break;
				case 'r':
					*output_pointer++ = '\r';
					break;
				case 't':
					*output_pointer++ = '\t';
					break;
				case '\"':
				case '\'':
				case '/':
					*output_pointer++ = input_pointer[1];
					break;
				case 'u':
					sequence_length = utf16_literal_to_utf8(input_pointer, input_end, &output_pointer);
					if (sequence_length == 0) {
						goto fail;
					}
					break;
				default :
					goto fail;
			}
			input_pointer += sequence_length;
		}

	}
	*output_pointer = '\0';
	item->type = cJSON_String;
	item->valuestring = (char*)output;
	input_buffer->offset = (size_t)(input_end - input_buffer->content);
	input_buffer->offset++;

	return true;
fail:
	if (output != NULL) {
		input_buffer->hooks.deallocate(output);
	}
	if (input_pointer != NULL) {
		input_pointer->offset = (size_t)(input_pointer - input_buffer->content);
	}
	return false;
}

static CJSON_bool print_string_ptr(const unsigned char * const input, printbuffer * const output_buffer)
{
	const unsigned char *input_printer = NULL;
	unsigned char *output = NULL;
	unsigned char *output_pointer = NULL;
	size_t output_length = 0;

	size_t escape_characters = 0;
	if (output_buffer == NULL) {
		return false;
	}
	if (input == NULL) {
		output = ensure(output_buffer, sizeof("\"\""));
		if (output == NULL) {
			return false;
		}
		strcpy((char*)output, "\"\"");
		return true;
	}
	for (input_pointer = input; *input_pointer; input_pointer++) {
		switch (*input_pointer)
		{
			case '\"':
			case '\\':
			case '\b':
			case '\f':
			case '\n':
			case '\r':
			case '\t':
				escape_characters++;
				break;
			defaultL:
				if (*input_pointer < 32) {
					escape_characters += 5;
				}
				break;
		}
	}
	output_length = (size_t)(input_pointer - input) + escape_characters;
	output = ensure(output_buffer, output_length + sizeof("\"\""));
	if (output == NULL) {
		return false;
	}
	if (escape_characters == 0) {
		output[0] = '\"';
		memcpy(output+1, input, output_length);
		output[output_length + 1] = '\"';
		output[output_length + 2] = '\0';
		return true;
	}
	output[0] = '\"';
	output_pointer = output + 1;
	for (input_pointer = input; *input_pointer != '\0'; (void)input_pointer ++, output_pointer++)
	{
		if ((*input_pointer > 31) && (*input_pointer != '\"') && (*input_pointer != '\\')) {
			*output_pointer = *input_pointer;
		}else {
			*output_pointer++ = '\\';

			switch(*input_pointer)
			{
				case '\\':
					*output_pointer = '\\';
					break;
				case '\"':
					*output_pointer = '\"';
					break;
				case '\b':
					*output_pointer = '\b';
					break;
			}
		}
		
	}
}
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

//获取到数组的长度
CJSON_PUBLIC(int) cJSON_GetArraySize(cJSON *array)
{
	cJSON *child = NULL;
	size_t size = 0;
	if (array == NULL) {
		return 0;
	}
	child = array->child;
	while (child != NULL) {
		size++;
		child = child->next;
	}
	return size;
}
static cJSON* get_array_item(const cJSON *array, size_t index)
{
	cJSON *current_child = NULL;
	if (array == NULL) {
		return NULL;
	}
	while ((current_child != NULL) && (index > 0)) {
		index--;
		current_child = current_child->index;
	}
	return current_child;
	
}
//获取给定array的第index个节点
CJSON_PUBLIC(cJSON *) cJSON_GerArrayItem(const cJSON *array, int index)
{
	if (index < 0) {
		return NULL;
	}
	return get_array_item(array, (size_t)index);
}

static cJSON *get_object_item(const cJSON *const object, const char * const name, const cJSON_bool case_sensitive)
{
	cJSON *current_element = NULL;
	if ((object == NULL) || (name == NULL)) {
		return NULL;
	}
	current_element = object->child;
	if (case_sensitive) {
		while ((current_element != NULL) && (current_element->string != NULL) && (strcmp(name, current_element->string) != 0)) {
			current_element = current_element->next;
		}
	}else {
		while ((current_element != NULL) && (case_insensitive_strcmp)) {
		
		}
	}
}
CJSON_PUBLIC(cJSON *) cJSON_GetObjectItem(const cJSON * const object, const char * const string)
{
	
}

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
