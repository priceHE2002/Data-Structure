#include <climits>
#include <iostream>

const int MaxLen = 255;

 
struct HString{
    char *ch;
    int length;
};

bool InitHString(HString &S, int maxLen = MaxLen)
{
    S.ch = new char[MaxLen];
    if(S.ch == nullptr)
    {
        std::cout<<"因内存分配失败，初始化失败"<<std::endl;
        return false;
    }
    S.length = 0;
    return true;
}
void DestroyHString(HString &S)
{
    if(S.ch != nullptr)
    {
        delete[] S.ch;
        S.ch = nullptr;
        S.length = 0;
    }
}
//赋值操作，把串S赋值为字符串T
bool StrAssign(HString &S, char T[], int tLength)
{
     if(S.ch == nullptr || T == nullptr || tLength < 0)
     {
        return false;
     }
     if(tLength >= MaxLen)
     {
        return false;
     }
     for (int i =0;i <= tLength; ++i)
     {
        S.ch[i] = T[i];
     }
     S.ch[tLength] = '\0';
     return true;
}

HString StrCopy(HString &S)
{
    HString T;
    if(!InitHString(T))
    {
        T.ch = nullptr;
        T.length = 0;
        return T;
    }
    if(S.ch == nullptr || S.length < 0)
    {
        T.ch[0] = '\0';
        T.length = 0;
        return T;
    }
    for(int i = 0; i <= S.length; i++)  // 包含'\0'
    {
        T.ch[i] = S.ch[i];
    }
    T.length = S.length;
    return T;
}
//判空
bool StrEmpty(HString S)
{
    if(S.ch == nullptr)
    {
        return true;
    }
    else {
        return false;
    }
}
//字符串的比较,S>T返回1，S<T返回-1；S=T返回0
int StrCompare(HString S, HString T)
{
    // 先比较长度
    if(S.length > T.length)
    {
        return 1;
    }
    else if (S.length < T.length) 
    {
        return -1;
    }
      
    // 长度相同，逐字符比较
    for(int i = 0; i < S.length; i++) 
    {
        if(S.ch[i] > T.ch[i])
        {
            return 1;
        }
        else if(S.ch[i] < T.ch[i])
        {
            return -1;
        }
        // 字符相等，继续比较下一个
    }
    
    // 所有字符都相等
    return 0;
}
//求子串，用Sub返回串S从第pos个字符起长度为len的子串
bool SubString(HString &Sub, HString S, int pos, int len)
{
    // 参数合法性检查
    if (S.ch == nullptr || pos < 1 || len < 0 || pos + len - 1 > S.length)
    {
        return false;
    }
    
    if (Sub.ch != nullptr)
    {
        delete[] Sub.ch;
        Sub.ch = nullptr;
    }
    
    Sub.ch = new char[len + 1];  // +1 用于存储结束符
    if (Sub.ch == nullptr)
    {
        return false;
    }
    Sub.length = len;

    for (int i = 0; i < len; i++)
    {
        Sub.ch[i] = S.ch[pos - 1 + i];
    }

    return true;
}
//定位操作。若主串S中存在与串T值相同的子串，则返回它在主串S中的第一次出现的位置；否则函数值返回0
int Index(HString S, HString T)
{   
    //参数合法性检查
    if (S.ch == nullptr || T.ch ==nullptr || S.length < T.length || T.length < 1)
    {
        return 0;
    }

    for(int i = 0; i <= S.length - T.length; i++)
    {
        if(S.ch[i] == T.ch[0])
        {
            int j = 0;
            while(j < T.length && S.ch[i+j] == T.ch[j])
            {
                j++;
            }
            if(j == T.length)
            {
                return i + 1;
            }
        }
    }
    return 0;
}