#include<iostream>
using namespace std;

struct Node
{
    int data;
    Node* next;  // 指向下一个节点
};

int main()
{
    // 创建链表：1 -> 2 -> 3
    Node node1 = {1, nullptr};
    Node node2 = {2, nullptr};
    Node node3 = {3, nullptr};
    
    node1.next = &node2;
    node2.next = &node3;
    
    Node* head = &node1;
    
    // 访问方式 1：使用 ->
    cout << "head->data = " << head->data << endl;          // 1
    cout << "head->next->data = " << head->next->data << endl;  // 2
    
    // 访问方式 2：使用 *
    cout << "(*head).data = " << (*head).data << endl;      // 1
    cout << "(*(*head).next).data = " << (*(*head).next).data << endl;  // 2
    
    // 遍历链表
    Node* current = head;
    while(current != nullptr)
    {
        cout << current->data << " ";  // 1 2 3
        current = current->next;
    }
    cout << endl;  // 添加换行符
    
    return 0;
}