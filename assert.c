#ifdef DEBUG
void _Assert(char *, unsigned);
#define ASSERT(f)			\
if(f)						\
	NULL;					\
else						\
	_Assert(__FILE__, __LINE__)\
#else 
#define ASSERT(f)			NULL 
#endif



