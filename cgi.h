#ifndef _CGI_H_
#define _CGI_H_


#define CONENT_TYPE_NUM 8

enum REQUESTCLASS{
    CGI_ERROR  = 0, //错误请求
    CGI_SUCCESS,    //成功
    GET,            //get请求
    POST,            //post请求
};

enum CONTENT_TYPE{
    XHMLT     = 0,  //xhtml
    XML,            //xml
    ATOM,           //atom
    JSON,           //json
    PDF,            //pdf
    MSWORD,         //word
    OCTET_STREAM,   //二进制流数据
    FORM,           //表单 
    NO_TYPE,        //不知名类型             
};


struct CGI_DATA
{
    int request;
    int content_type;
    char *post_form;
    char *url_query_data;
};

struct Content_Type
{
    int  Type;
    char text[60];
};

typedef struct CGI_DATA CGI_HANDLE;


//获取请求类型
int GetRequest(CGI_HANDLE *handle);

//解析表单的数据
int CGI_ExportFormContext(CGI_HANDLE *handle,const char * key,char *value);
//解析url后面的查询数据
int CGI_URLQueryValue(CGI_HANDLE *handle,char *key,char *value);

//句柄
void CGI_HandleCreate(CGI_HANDLE *handle,char *head);
void CGI_HandleClose(CGI_HANDLE *handle);


#endif
