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


//获取请求类型
int GetRequest(CGI_HANDLE *handle){

    char *content_class = NULL;
    unsigned char num = 0;

    int form_lenght = 0;//表单长度

    //获取cgi的环境变量
    char *env = NULL;
    char *content = NULL;
    char *quest_string = NULL;

    env = getenv("REQUEST_METHOD");         //请求类型
    content = getenv("CONTENT_TYPE");       //请求的内容类型
    quest_string = getenv("QUERY_STRING");  //获取请求发过来的信息，url后面的信息


    // fprintf(stdout, "<H2>quest_string: %s</H2>",quest_string);


    if (!env)
    {
        return CGI_ERROR;
    }

    if(strcmp("POST",env) == 0){
        handle->request = POST;
        //获取表单的数据长度
        form_lenght = atoi(getenv("CONTENT_LENGTH"));

        handle->post_form = NULL;

        handle->post_form = (char *)malloc(form_lenght+1);

        if (handle->post_form != NULL)
        {
             while(fgets(handle->post_form,form_lenght+1,stdin) != NULL);
        }
        

    }else if (strcmp("GET",env) == 0){
        handle->request = GET;
    }

    if (quest_string)
    {
        int query_len = strlen(quest_string) + 1;
        handle->url_query_data = NULL;
        handle->url_query_data = (char *)malloc(query_len);
        if (!handle->url_query_data)
        {
           memcpy(handle->url_query_data,quest_string,query_len);
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
//初始化表单
//解析表单的数据
int CGI_ExportFormContext(CGI_HANDLE *handle,const char * key,char *value){
    int lenght = -1;
    if (handle->post_form)
    {
        char *token;
        char *split;
        token = strtok(handle->post_form, "&");
        while( token != NULL ) {
            //获取字段
            split = strtok(token, "=");
            if (strcmp(split,key) == 0)
            {
                lenght = 0;
                value = strtok(NULL, "=");
                lenght = strlen(value);
                // if (split)
                // {
                //      while ((*value++ = split[lenght]) != '\0')
                //     {
                //         lenght++;
                //     }
                // }
            
                return lenght;
            }
            
            token = strtok(NULL, "&");
        }

    }
    return lenght;
    
}

//解析url后面的查询数据
int CGI_URLQueryValue(CGI_HANDLE *handle,char *key,char *value)
{
    int lenght = -1;
    if (handle->url_query_data)
    {
        char *token;
        char *split;
        token = strtok(handle->url_query_data, "&");
        while( token != NULL ) {
            //获取字段
            split = strtok(token, "=");
            if (strcmp(split,key) == 0)
            {
                lenght = 0;
                value = strtok(NULL, "=");
                lenght = strlen(value);
                // split = strtok(NULL, "=");
                // if (split)
                // {
                //      while ((*value++ = split[lenght]) != '\0')
                //     {
                //         lenght++;
                //     }
                // }
            
                return lenght;
            }
            
            token = strtok(NULL, "&");
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
}

//错误信息
void CGI_DEBUG(char *format,...)
{

    va_list args;
    va_start(args,format);
    vprintf(format,args);
    va_end(args);
}


// fprintf(cgiOut, "<a href='.html'>返回</a>"); 
// fprintf(cgiOut, "<meta http-equiv=\"refresh\" content=\"1;url=../main%d.html\">", sto_no[0]);
