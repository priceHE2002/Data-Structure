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