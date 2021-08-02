#ifndef CGI_LIST_H
#define CGI_LIST_H

#define SUCCESS     0
#define ERROR       -1

typedef struct _CGI_LIST_NODE_
{
    char *key;      //查询key
    char *value;    //值
    struct _CGI_LIST_NODE_ *next;
}CGI_LIST_NODE;

typedef struct _CGI_HEARD_NODE_
{
    int NodeNum;
    struct _CGI_LIST_NODE_ *next;
}CGI_LINK;


int CountStrLenght(const char *string);
void StrCopy(char *des,const char *src);
int StrCmp(char *des,const char *src);

//初始化列表
CGI_LINK *InitCGIList();
//插入列表
int InsertCGIList(CGI_LINK *HeardNode,CGI_LIST_NODE *Node);
//查找列表数据
CGI_LIST_NODE * FindCGIList(CGI_LINK *HeardNode,const char *key);
//释放列表
void DestoryCGIList(CGI_LINK *HeardNode);
//创建节点
CGI_LIST_NODE *CreateCGIListNode(const char *key,const char *value);
//销毁节点
void  DestoryCGIListNode(CGI_LIST_NODE *Node);

#endif
