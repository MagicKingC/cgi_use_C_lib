#include "cgi_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int CountStrLenght(const char *string){
    int length = 0;

    while (string[length] != '\0')
    {
        length++;
    }

    return length;
}

void StrCopy(char *des,const char *src)
{
    while ((*(des++) = *(src++)) != '\0');
}

int StrCmp(char *des,const char *src){

    int des_lenght = CountStrLenght(des);
    int src_lenght = CountStrLenght(src);
    int num;
    int cmp = 0;
    if (des_lenght > src_lenght)
    {
        for ( num = 0; num < des_lenght; num++)
        {
            if (des[num] == src[num])
            {
                cmp ++;
                if (cmp == des_lenght)
                {
                    return 0;
                }
                
            }else{
                return 1;
            }
        }
    }else{
        for ( num = 0; num < src_lenght; num++)
        {
            if (des[num] == src[num])
            {
                cmp ++;
                if (cmp == src_lenght)
                {
                    return 0;
                }
                
            }else{
                return 1;
            }
        }
    }
    return 1;
}

//初始化列表
CGI_LINK * InitCGIList()
{
    CGI_LINK *HeardNode  = NULL;

    HeardNode = (CGI_LINK *)malloc(sizeof(CGI_LINK*));

    if (HeardNode)
    {
        HeardNode->NodeNum = 0;
        HeardNode->next = NULL;
    }

    return HeardNode;
}

//插入列表
int InsertCGIList(CGI_LINK *HeardNode,CGI_LIST_NODE *Node)
{
    if (Node&&HeardNode)
    {
        HeardNode->NodeNum++;
        Node->next = HeardNode->next;
        HeardNode->next = Node;
        return SUCCESS;
    }
    return ERROR;
}

//创建节点
CGI_LIST_NODE *CreateCGIListNode(const char *key,const char *value){
    CGI_LIST_NODE *Node = NULL;
    Node = (CGI_LIST_NODE *)malloc(sizeof(CGI_LIST_NODE));
    if (Node)
    {
        int key_lenght = CountStrLenght(key);
        int value_lenght = CountStrLenght(value);

        if (key_lenght > 0)
        {
            
            Node->key = (char *)malloc(key_lenght);
            if (Node->key)
            {
                StrCopy(Node->key,key);
            }

        }else{
            Node->key = (char *)malloc(2);
            StrCopy(Node->key," ");
        }
        

        if (value_lenght > 0)
        {
            Node->value = (char *)malloc(value_lenght);
            StrCopy(Node->value,value);
        }else{
            Node->value = (char *)malloc(2);
            StrCopy(Node->value," ");
        }

        Node->next = NULL;
    }
    return Node;
}



//销毁节点
void  DestoryCGIListNode(CGI_LIST_NODE *Node){
    if (Node)
    {
        if (Node->key)
        {
            free(Node->key);
        }
        if (Node->value)
        {
            free(Node->value);
        }
        free(Node);
    }
    
}

//查找列表数据
CGI_LIST_NODE * FindCGIList(CGI_LINK *HeardNode,const char *key)
{
    CGI_LIST_NODE *Node = NULL;
    if (HeardNode)
    {
        Node = HeardNode->next;
        
        while (Node)
        {
            if (StrCmp(Node->key,key) == 0)
            {
                return Node;
            }
            Node = Node->next;
        }
        
    }
    return NULL;
    //return *Node;
    
}

//释放列表
void DestoryCGIList(CGI_LINK *HeardNode){
    CGI_LIST_NODE *Node = NULL;
    if (HeardNode)
    {
        Node = HeardNode->next;
        while (Node)
        {
            DestoryCGIListNode(Node);
            Node = Node->next;
        }
        free(HeardNode);
    }
    
}

