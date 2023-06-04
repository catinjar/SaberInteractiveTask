#pragma once

#include <string>

struct ListNode
{
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

class List
{
public:
    ~List();

    void Serialize(FILE* file) const;
    void Deserialize(FILE* file);

    ListNode* Add(const char* data);
    void Clear();
    void Print() const;

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int count = 0;
};