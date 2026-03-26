
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