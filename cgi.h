#ifndef _CGI_H_
#define _CGI_H_

#include "cgi_list.h"

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

enum _LIST_CLASS_{
    NO_LIST = -1,
    PORT_FORM_LIST = 0,
    URL_QUERY_LIST,
};

struct CGI_DATA
{
    int request;
    int content_type;
    int post_form_lenght;
    char *post_form;
    char *url_query_data;
    CGI_LINK *post_data_list;
    CGI_LINK *url_query_list;

};

struct Content_Type
{
    int  Type;
    char text[60];
};

typedef struct CGI_DATA CGI_HANDLE;

//调试函数
void CGI_DEBUG(char *format,...);
//获取请求类型
int GetRequest(CGI_HANDLE *handle);
//解析表单的数据
int CGI_GetPortFormData(CGI_HANDLE *handle,const char * key,char *value);
//解析url后面的查询数据
int CGI_GetURLQueryValue(CGI_HANDLE *handle,const char * key,char *value);
//句柄
void CGI_HandleCreate(CGI_HANDLE *handle,char *head);
void CGI_HandleClose(CGI_HANDLE *handle);


#endif
