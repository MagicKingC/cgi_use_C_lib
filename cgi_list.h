#ifndef CGI_LIST_H
#define CGI_LIST_H

struct CGI_PORTFORM_CONTEXT
{
    char *key;      //查询key
    char *value;    //值
};

//初始化列表
void InitCGIList();
//插入列表
void InsertCGIList();
//查找列表数据
void FindCGIList();

#endif
