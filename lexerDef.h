/*PUSHKARA RAVINDRA 2011A7PS150P
 *VIRAJ PRABHU	    2011A7PS044P
 * BATCH-1*/
/*LEXERDEF_H*/
typedef char *buffer;
typedef int buffersize;
typedef struct _tokenInfo tokenInfo;
typedef union _data data;
union _data
{
	int num;
	float rnum;
	char *str;
};
struct _tokenInfo
{
	char *token;
	data dat;
	int line;
};