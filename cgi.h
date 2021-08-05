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

enum _FIND_DATA{
    CGI_NO_FIND_DATA = -1,
    CGI_FIND_DATA,
};

//element
enum ELEMENT_DATA{
    CGI_UNCHECK = 0,
    CGI_CHECK,
};

////////////////////////////
struct CGI_DATA
{
    int request;                
    int content_type;           
    int http_data_lenght;       //数据长度
    char *port_form;            //表格参数
    char *json_data;            //json数据
    char *url_query_data;       //url的参数
    char *service_addr;         //服务器ip地址
    char *service_software;     //服务器的软件名
    char *client_addr;          //客户端ip地址
    char *client_host;          //客户端ip地址
    char *http_user_agent;      //浏览器信息
    char *cgi_version;          //cgi版本
    char *cgi_path;             //cgi脚本路径
    char *http_protocol;        //服务器http协议
    int service_port;
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
//解析url后面的查询数据
int CGI_GetURLQueryValue(CGI_HANDLE *handle,const char * name,char *value);
//获取服务器IP地址
char * CGI_GetServiceIPAddress(CGI_HANDLE *handle);
//获取服务器端口号
int CGI_GetServicePort(CGI_HANDLE *handle);
//获取服务器类型
char *CGI_GetServiceSoftWare(CGI_HANDLE *handle);
//获取客户端IP地址
char * CGI_GetClientIPAddress(CGI_HANDLE *handle);
//获取客户端主机名
char * CGI_GetClientHost(CGI_HANDLE *handle);
//获取浏览器的信息
char * CGI_GetHTTPUserAgent(CGI_HANDLE *handle);
//获取CGI版本
char *CGI_GetVerSion(CGI_HANDLE *handle);
//获取CGI脚本路径
char *CGI_GetCGIPath(CGI_HANDLE *handle);
//服务器的http协议
char *CGI_GetServiceHttpProtocol(CGI_HANDLE *handle);
//复选框
int CGI_CheckboxStatus(CGI_HANDLE *handle,char *name);
//输入框
int CGI_GetInputValue(CGI_HANDLE *handle,char *name,char *data);
//单选框
int CGI_RadioButtonsValue(CGI_HANDLE *handle,char *name,char *value);
//下拉框
int CGI_SelectValue(CGI_HANDLE *handle,char *name,char *value);
//句柄
void CGI_HandleCreate(CGI_HANDLE *handle,char *head);
void CGI_HandleClose(CGI_HANDLE *handle);


#endif
