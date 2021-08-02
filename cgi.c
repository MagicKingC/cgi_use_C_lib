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
                        CGI_DEBUG("key:%s -- value:%s -- value_length:%d<br>",key,value,value_length);
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

    if(strcmp("POST",env) == 0){
        handle->request = POST;
        //获取表单的数据长度
        handle->post_form_lenght = atoi(getenv("CONTENT_LENGTH"));
        handle->post_form = NULL;
        handle->post_form = (char *)malloc(handle->post_form_lenght+1);
        if (handle->post_form != NULL)
        {
            while(fgets(handle->post_form,handle->post_form_lenght+1,stdin) != NULL);
            handle->post_data_list = InitCGIList();
            CGI_ExContext(handle,handle->post_form,handle->post_form_lenght,PORT_FORM_LIST);//解析表格数据
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


//解析表单的数据
int CGI_GetPortFormData(CGI_HANDLE *handle,const char * key,char *value){
    int lenght = -1;
    
    if (handle->post_data_list)
    {
       CGI_LIST_NODE *Node =  FindCGIList(handle->post_data_list,key);
       if (Node)
       {
           lenght = 0;
           if (Node->value)
           {
                StrCopy(value,Node->value);
                lenght = CountStrLenght(Node->value);
           }
           return lenght;
       }
    }
    return lenght;
    
}

//解析url后面的查询数据
int CGI_GetURLQueryValue(CGI_HANDLE *handle,const char * key,char *value)
{
    int lenght = -1;
    
    if (handle->url_query_list)
    {
       CGI_LIST_NODE *Node =  FindCGIList(handle->url_query_list,key);
       if (Node)
       {
           lenght = 0;
           if (Node->value)
           {
                StrCopy(value,Node->value);
                lenght = CountStrLenght(Node->value);
           }
           return lenght;
       }
    }
    return lenght;
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

    if (handle->post_form)
    {
        free(handle->post_form);
    }

    if (handle->url_query_data)
    {
        free(handle->url_query_data);
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

//错误信息
void CGI_DEBUG(char *format,...)
{

    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}

