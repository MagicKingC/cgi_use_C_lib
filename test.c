#include <stdio.h>
#include <stdlib.h>
#include "cgi/cgi.h"

int main(int argc,char **argv)
{
    char user[10]={0};
    char getdata[10]={0};
    char text[10]={0};
    CGI_HANDLE cgi_handle;
    CGI_HandleCreate(&cgi_handle,"content-type:text/html\n\n");

    int req = GetRequest(&cgi_handle);
    if (cgi_handle.request == POST)
    {
        CGI_DEBUG("post<br>");
    }else  if (cgi_handle.request == GET){
        CGI_DEBUG("get<br>");
    }else{
        CGI_DEBUG("faile<br>");
    }

    //输入框
    CGI_GetInputValue(&cgi_handle,"user",user);
    CGI_DEBUG("user:%s<br>",user);
    CGI_GetInputValue(&cgi_handle,"user1",user);
    CGI_DEBUG("user1:%s<br>",user);
    CGI_GetInputValue(&cgi_handle,"user2",user);
    CGI_DEBUG("user2:%s<br>",user);
    CGI_GetInputValue(&cgi_handle,"user",user);
    CGI_DEBUG("user:%s<br>",user);

    //复选框
    if (CGI_CheckboxStatus(&cgi_handle,"running") == CGI_CHECK)
    {
        CGI_DEBUG("running: is check<br>");
    }else{
        CGI_DEBUG("running: is not check<br>");
    }
    
    if (CGI_CheckboxStatus(&cgi_handle,"reading") == CGI_CHECK)
    {
        CGI_DEBUG("reading: is check<br>");
    }else{
        CGI_DEBUG("reading: is not check<br>");
    }

    if (CGI_CheckboxStatus(&cgi_handle,"shopping") == CGI_CHECK)
    {
        CGI_DEBUG("shopping: is check<br>");
    }else{
        CGI_DEBUG("shopping: is not check<br>");
    }

    
    CGI_RadioButtonsValue(&cgi_handle,"sex",text);
    CGI_DEBUG("sex_value:%s<br>",text);
    
    CGI_SelectValue(&cgi_handle,"cars",text);
    CGI_DEBUG("select_value:%s<br>",text);

    CGI_GetURLQueryValue(&cgi_handle,"data",getdata);
    CGI_DEBUG("getdata:%s<br>",getdata);

    CGI_DEBUG("CGI_GetServiceIPAddress:%s<br>",CGI_GetServiceIPAddress(&cgi_handle));
    CGI_DEBUG("CGI_GetServicePort:%d<br>",CGI_GetServicePort(&cgi_handle));
    CGI_DEBUG("CGI_GetServiceSoftWare:%s<br>",CGI_GetServiceSoftWare(&cgi_handle));
    CGI_DEBUG("CGI_GetClientIPAddress:%s<br>",CGI_GetClientIPAddress(&cgi_handle));
    CGI_DEBUG("CGI_GetHTTPUserAgent:%s<br>",CGI_GetHTTPUserAgent(&cgi_handle));
    CGI_DEBUG("CGI_GetVerSion:%s<br>",CGI_GetVerSion(&cgi_handle));
    CGI_DEBUG("CGI_GetCGIPath:%s<br>",CGI_GetCGIPath(&cgi_handle));
    CGI_DEBUG("CGI_GetServiceHttpProtocol:%s<br>",CGI_GetServiceHttpProtocol(&cgi_handle));


    CGI_HandleClose(&cgi_handle);


    return 0;
}

