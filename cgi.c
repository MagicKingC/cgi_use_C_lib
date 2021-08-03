#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



const struct Content_Type content_type_arr[CONENT_TYPE_NUM]={
    {FORM,"application/x-www-form-urlencoded"},
    {XHMLT,"application/xhtml+xml"},
    {XML,"application/xml"},
    {ATOM,"application/atom+xml"},
    {JSON,"application/json"},
    {PDF,"application/pdf"},
    {MSWORD,"application/msword"},
    {OCTET_STREAM,"application/octet-stream"},
};

//解析表单的数据
static int CGI_GetPortFormData(CGI_HANDLE *handle,const char * name,char *value);
//获取环境变量
static char * CGI_GetEnvData(const char *env){
    char *data_addr = NULL;
    char * env_data = NULL;
    env_data = getenv(env);

    if (env_data)
    {
        int lenght = CountStrLenght(env_data);
        if (lenght > 0)
        {
            data_addr = (char *)malloc(lenght);
            if (data_addr)
            {
                StrCopy(data_addr,env_data);
            }
            
        }
        
    }
    return data_addr;
}

//解析数据到数据链表
static int CGI_ExContext(CGI_HANDLE *handle, const char * data,int data_length,int data_list_class)
{
    //截取出表格数据
    if (data)
    {
        char *key;
        char *value;

        int lenght = 0;
        int key_length = 0;
        int value_length = 0;

        int IsFindKeyOrValue = 0;
    
       while ((data[lenght]) != '\0')
       {

           if (IsFindKeyOrValue == 0){//找key
                
                if (data[lenght] == '='){
                    //拷贝key
                    key = (char *)malloc(key_length+1);
                    if (key)
                    {
                        memcpy(key,data+(lenght-key_length),key_length);
                        key[key_length] = '\0';
                        IsFindKeyOrValue = 1;
                    }
                }
                 key_length++;
 
           }else if (IsFindKeyOrValue == 1){//找value
                
                if (data[lenght] == '&'||lenght == data_length-1){
                    int tem = 0;
                    if (lenght == data_length-1){
                        tem = 1;
                    }
                    value_length += tem;
                    //拷贝数值
                    value = (char *)malloc(value_length);
                    if (value)
                    {
                        
                        memcpy(value,data+(lenght-value_length+tem),value_length);
                        value[value_length]='\0';
                        value_length = 0;
                        IsFindKeyOrValue = 0;
                        //插入数据
                        CGI_LIST_NODE *Node = CreateCGIListNode(key,value);
                        if (data_list_class == PORT_FORM_LIST)
                        {
                            //port表单
                            InsertCGIList(handle->post_data_list,Node);
                        }else if (data_list_class == URL_QUERY_LIST)
                        {
                            //url数据
                             InsertCGIList(handle->url_query_list,Node);
                        }
                        free(value);
                    }

                    if (key)
                    {
                        key_length = 0;
                        free(key);
                    }
                }
                value_length++;
           }
           lenght++;
       }

       return SUCCESS;
    }
    return ERROR;
    
}

//获取请求类型
int GetRequest(CGI_HANDLE *handle){

    char *content_class = NULL;
    unsigned char num = 0;

    //获取cgi的环境变量
    char *env = NULL;
    char *content = NULL;
    char *quest_string = NULL;

    env = getenv("REQUEST_METHOD");         //请求类型
    content = getenv("CONTENT_TYPE");       //请求的内容类型
    quest_string = getenv("QUERY_STRING");  //获取请求发过来的信息，url后面的信息 

    if (!env)
    {
        return CGI_ERROR;
    }

    // CGI_DEBUG("HTTP_USER_AGENT:%s<br>",getenv("HTTP_USER_AGENT"));

    if(strcmp("POST",env) == 0){
        handle->request = POST;
        //获取表单的数据长度
        handle->port_form_lenght = atoi(getenv("CONTENT_LENGTH"));
        if (handle->port_form_lenght > 0)
        {
            handle->port_form = NULL;
            handle->port_form = (char *)malloc(handle->port_form_lenght+1);
            if (handle->port_form != NULL)
            {
                while(fgets(handle->port_form,handle->port_form_lenght+1,stdin) != NULL);
                handle->post_data_list = InitCGIList();
                CGI_ExContext(handle,handle->port_form,handle->port_form_lenght,PORT_FORM_LIST);//解析表格数据
            } 
        }
    }else if (strcmp("GET",env) == 0){
        handle->request = GET;
    }

    if (quest_string)
    {
        int query_len = strlen(quest_string) + 1;
        handle->url_query_data = NULL;
        handle->url_query_data = (char *)malloc(query_len);
        if (handle->url_query_data)
        {
            memcpy(handle->url_query_data,quest_string,query_len);
            handle->url_query_list = InitCGIList();
            CGI_ExContext(handle,handle->url_query_data,query_len-1,URL_QUERY_LIST);//解析表格数据
        }
        
    }

    if (content)
    {
        for ( num = 0; num < CONENT_TYPE_NUM; num++)
        {
            if (strcmp(content_type_arr[num].text,content)==0)
            {
                handle->content_type = content_type_arr[num].Type;
                return CGI_SUCCESS;
            }
        }
    }
    
    return CGI_SUCCESS;
}

//获取表单的数据
static int CGI_GetPortFormData(CGI_HANDLE *handle,const char * name,char *value){
    
    if (handle->post_data_list)
    {
       CGI_LIST_NODE *Node =  FindCGIList(handle->post_data_list,name);
       if (Node)
       {
           if (Node->value)
           {
                StrCopy(value,Node->value);
           }
           return CGI_FIND_DATA;
       }
    }
    return CGI_NO_FIND_DATA;
    
}

//解析url后面的查询数据
int CGI_GetURLQueryValue(CGI_HANDLE *handle,const char * name,char *value)
{
    if (handle->url_query_list)
    {
       CGI_LIST_NODE *Node =  FindCGIList(handle->url_query_list,name);
       if (Node)
       {
           if (Node->value)
           {
                StrCopy(value,Node->value);
           }
           return CGI_FIND_DATA;
       }
    }
    return CGI_NO_FIND_DATA;
}

//获取服务器IP地址
char * CGI_GetServiceIPAddress(CGI_HANDLE *handle)
{
    handle->service_addr = NULL;
    handle->service_addr = CGI_GetEnvData("SERVER_NAME");
    return handle->service_addr;
}

//获取服务器端口号
int CGI_GetServicePort(CGI_HANDLE *handle)
{
    handle->service_port =atoi(getenv("SERVER_PORT"));
    return handle->service_port;
}

//获取服务器类型
char *CGI_GetServiceSoftWare(CGI_HANDLE *handle)
{
    handle->service_software = NULL;
    handle->service_software = CGI_GetEnvData("SERVER_SOFTWARE");
    return handle->service_software;
}

//获取客户端IP地址
char * CGI_GetClientIPAddress(CGI_HANDLE *handle)
{
    handle->client_addr = NULL;
    handle->client_addr = CGI_GetEnvData("REMOTE_ADDR");
    return handle->client_addr;
}

//获取客户端主机名
char * CGI_GetClientHost(CGI_HANDLE *handle)
{
    handle->client_host = NULL;
    handle->client_host = CGI_GetEnvData("REMOTE_HOST");
    return handle->client_host;
}

//获取CGI版本
char *CGI_GetVerSion(CGI_HANDLE *handle)
{
    handle->cgi_version = NULL;
    handle->cgi_version = CGI_GetEnvData("GATEWAY_INTERFACE");
    return handle->cgi_version;
}

//获取CGI脚本路径
char *CGI_GetCGIPath(CGI_HANDLE *handle)
{
    handle->cgi_path = NULL;
    handle->cgi_path = CGI_GetEnvData("SCRIPT_NAME");
    return handle->cgi_path;
}

//服务器的http协议
char *CGI_GetServiceHttpProtocol(CGI_HANDLE *handle)
{
    handle->cgi_path = NULL;
    handle->cgi_path = CGI_GetEnvData("SERVER_PROTOCOL");
    return handle->cgi_path;
}

//获取浏览器的信息
char * CGI_GetHTTPUserAgent(CGI_HANDLE *handle)
{
    handle->http_user_agent = NULL;
    handle->http_user_agent = CGI_GetEnvData("HTTP_USER_AGENT");
    return handle->http_user_agent;
}

//复选框
int CGI_CheckboxStatus(CGI_HANDLE *handle,char *name){
    char name_arr[1];
    if (handle->request == POST)
    {
        if(CGI_GetPortFormData(handle,name,name_arr) == CGI_FIND_DATA){
            return CGI_CHECK;
        }
    }else{
        if(CGI_GetURLQueryValue(handle,name,name_arr) == CGI_FIND_DATA){
            return CGI_CHECK;
        }
    }
    return CGI_UNCHECK;
}

//输入框
int CGI_GetInputValue(CGI_HANDLE *handle,char *name,char *data)
{
    if (handle->request == POST)
    {
        if(CGI_GetPortFormData(handle,name,data) == CGI_FIND_DATA){
            return CGI_SUCCESS;
        }
    }else{
        if(CGI_GetURLQueryValue(handle,name,data) == CGI_FIND_DATA){
            return CGI_SUCCESS;
        }
    }
    return CGI_ERROR;

}

//单选框
int CGI_RadioButtonsValue(CGI_HANDLE *handle,char *name,char *value)
{
    if (handle->request == POST)
    {
        if(CGI_GetPortFormData(handle,name,value) == CGI_FIND_DATA){
            return CGI_SUCCESS;
        }
    }else{
        if(CGI_GetURLQueryValue(handle,name,value) == CGI_FIND_DATA){
            return CGI_SUCCESS;
        }
    }
    return CGI_ERROR;
}

//下拉框
int CGI_SelectValue(CGI_HANDLE *handle,char *name,char *value)
{
    if(CGI_GetPortFormData(handle,name,value) == CGI_FIND_DATA){
        return CGI_SUCCESS;
    }
    return CGI_ERROR;
}

//创建cgi句柄
void CGI_HandleCreate(CGI_HANDLE *handle,char *head)
{

    memset(handle,0,sizeof(CGI_HANDLE));
    fprintf(stdout,"%s",head);//
    
}

//关闭cgi句柄
void CGI_HandleClose(CGI_HANDLE *handle)
{

    if (handle->port_form)
    {
        free(handle->port_form);
    }

    if (handle->url_query_data)
    {
        free(handle->url_query_data);
    }

    if (handle->service_addr)
    {
        free(handle->service_addr);
    }
    
    if (handle->service_software)
    {
        free(handle->service_software);
    }

    if (handle->client_addr)
    {
        free(handle->client_addr);
    }

    if (handle->client_host)
    {
        free(handle->client_addr);
    }

    if (handle->http_user_agent)
    {
        free(handle->http_user_agent);
    }

    if (handle->cgi_version)
    {
        free(handle->cgi_version);
    }

     if (handle->cgi_path)
    {
        free(handle->cgi_path);
    }

        if (handle->http_protocol)
    {
        free(handle->http_protocol);
    }

    if (handle->post_data_list)
    {
        DestoryCGIList(handle->post_data_list);
    }

    if (handle->url_query_list)
    {
        DestoryCGIList(handle->url_query_list);
    }
    
}

//信息调试函数
void CGI_DEBUG(char *format,...)
{

    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}

