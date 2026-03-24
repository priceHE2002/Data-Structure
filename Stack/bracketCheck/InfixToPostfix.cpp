#include <iostream>
#include <cstring>
using namespace std;

// 链栈结构
typedef struct LinkNode{
    char data;
    LinkNode *next;
}*LinkStack;

//初始化链栈
bool InitLinkStack(LinkStack &S)
{
    S = new LinkNode;
    if(S == nullptr)
    {
        return false;
    }
    S->next = nullptr;
    return true;
}

//判断栈是否为空
bool isStackEmpty(LinkStack S)
{
    return S->next == nullptr;
}

//入栈
bool Push(LinkStack &S, char a)
{
    LinkNode *newNode = new LinkNode;
    if(newNode == nullptr)
    {
        return false;
    }
    newNode->data = a;
    newNode->next = S->next;
    S->next = newNode;
    return true;
}

//出栈
bool Pop(LinkStack &S, char &a)
{
    if(S->next == nullptr)
    {
        return false;
    }
    LinkNode *p = S->next;
    a = p->data;
    S->next = p->next;
    delete p;
    return true;
}

//获取栈顶元素
bool GetTop(LinkStack S, char &a)
{
    if(S->next == nullptr)
    {
        return false;
    }
    a = S->next->data;
    return true;
}

//销毁链栈
void DestroyStack(LinkStack &S) {
    char temp;
    while (!isStackEmpty(S)) {
        Pop(S, temp);
    }
    delete S;
    S = nullptr;
}

// 判断是否是运算符
bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 获取运算符优先级
int getPriority(char op)
{
    switch(op)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// 中缀表达式转后缀表达式
void InfixToPostfix(char infix[], char postfix[])
{
    LinkStack S;
    InitLinkStack(S);
    
    int i = 0;  // infix 的索引
    int j = 0;  // postfix 的索引
    
    while(infix[i] != '\0')
    {
        char c = infix[i];
        
        // 1. 遇到操作数（数字或字母），直接加入后缀表达式
        if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            // 如果是数字，需要处理多位数的情况
            if(c >= '0' && c <= '9')
            {
                // 复制所有连续的数字
                while(infix[i] >= '0' && infix[i] <= '9')
                {
                    postfix[j++] = infix[i++];
                }
                postfix[j++] = ' ';  // 添加空格分隔符
                i--;  // 回退一位，因为外层循环会 i++
            }
            else
            {
                // 字母直接添加
                postfix[j++] = c;
            }
        }
        // 2. 遇到左括号，直接入栈
        else if(c == '(')
        {
            Push(S, c);
        }
        // 3. 遇到右括号，弹出栈内运算符直到左括号
        else if(c == ')')
        {
            char top;
            while(GetTop(S, top) && top != '(')
            {
                Pop(S, top);
                postfix[j++] = top;
            }
            Pop(S, top);  // 弹出左括号，但不加入后缀表达式
        }
        // 4. 遇到运算符
        else if(isOperator(c))
        {
            char top;
            // 弹出优先级高于或等于当前运算符的所有运算符
            while(!isStackEmpty(S) && GetTop(S, top) && top != '(' && getPriority(top) >= getPriority(c))
            {
                Pop(S, top);
                postfix[j++] = top;
            }
            Push(S, c);  // 当前运算符入栈
        }
        
        i++;
    }
    
    // 5. 将栈中剩余运算符依次弹出
    char top;
    while(!isStackEmpty(S))
    {
        Pop(S, top);
        postfix[j++] = top;
    }
    
    postfix[j] = '\0';  // 添加字符串结束符
    DestroyStack(S);
}

int main()
{
    // 测试用例 1：简单表达式
    char infix1[] = "a+b";
    char postfix1[100];
    InfixToPostfix(infix1, postfix1);
    cout << "中缀表达式：" << infix1 << endl;
    cout << "后缀表达式：" << postfix1 << endl << endl;
    
    // 测试用例 2：带括号
    char infix2[] = "(a+b)*c";
    char postfix2[100];
    InfixToPostfix(infix2, postfix2);
    cout << "中缀表达式：" << infix2 << endl;
    cout << "后缀表达式：" << postfix2 << endl << endl;
    
    // 测试用例 3：复杂表达式
    char infix3[] = "a+b*c";
    char postfix3[100];
    InfixToPostfix(infix3, postfix3);
    cout << "中缀表达式：" << infix3 << endl;
    cout << "后缀表达式：" << postfix3 << endl << endl;
    
    // 测试用例 4：多个括号
    char infix4[] = "(a+b)*(c-d)";
    char postfix4[100];
    InfixToPostfix(infix4, postfix4);
    cout << "中缀表达式：" << infix4 << endl;
    cout << "后缀表达式：" << postfix4 << endl << endl;
    
    // 测试用例 5：图片中的例子（简化版）
    char infix5[] = "((15/(7-(1+1)))*3)-(2+(1+1))";
    char postfix5[100];
    InfixToPostfix(infix5, postfix5);
    cout << "中缀表达式：" << infix5 << endl;
    cout << "后缀表达式：" << postfix5 << endl << endl;
    
    return 0;
}