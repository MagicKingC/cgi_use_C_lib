# cgi-
在Linux环境下，用标准c写的一套cgi库
（注意：假如使用过程发现有什么问题希望能够指出，谢谢

下面是cgi库中的函数，这里不一一介绍，主要介绍两个函数，
CGI_HandleCreate句柄初始化函数，该函数需要传递两个参数，一个是CGI_HANDLE
另一个是head为http的头部内容一般为：content-type:text/html
CGI_HANDLE的主要包括以下内容：
struct CGI_DATA
{
    int request;                
    int content_type;           
    int port_form_lenght;       //表格数据长度
    char *port_form;            //表格参数
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

CGI_HandleClose函数，该函数主要用于释放申请的资源，是和CGI_HandleCreate配套使用，如果在结束之后使用该函数会导致申请的内存无法释放

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

