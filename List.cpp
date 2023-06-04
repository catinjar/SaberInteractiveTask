#include "List.h"

#include <map>
#include <iostream>


List::~List()
{
    Clear();
}


void List::Serialize(FILE* file) const
{
    fwrite(&count, sizeof(int), 1, file);

    // Use hash map to store random links in linear time
    // Could write uintptr_t's and serialize without a hash map
    // But it would make saved data incompatible on platforms with different architectures and pointer sizes (e.g. 32-bit and 64-bit)
    std::map<ListNode*, int32_t> nodesMap;
    int32_t nodeIndex = 0;

    // Write string data
    ListNode* current = head;
    while (current)
    {
        const size_t length = current->data.length();
        fwrite(&length, sizeof(length), 1, file);
        fwrite(current->data.c_str(), sizeof(char), length, file);

        nodesMap[current] = nodeIndex++;
        current = current->next;
    }

    // Write random links data
    current = head;
    while (current)
    {
        if (current->rand)
            fwrite(&nodesMap[current->rand], sizeof(int32_t), 1, file);
        else
            fputc(-1, file);

        current = current->next;
    }
}


void List::Deserialize(FILE* file)
{
    Clear();

    int32_t nodesCount;
    fread(&nodesCount, sizeof(int), 1, file);

    std::map<int32_t, ListNode*> nodesMap;

    // Read string data and add nodes
    for (int32_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
    {
        size_t length;
        fread(&length, sizeof(length), 1, file);

        // In real-life code I would optimize this string allocation
        // Do we know max string length?
        // Can we use more effective custom allocator to avoid copying and fragmentation?
        char* data = new char[length + 1];
        fread(data, sizeof(char), length, file);
        data[length] = 0;

        ListNode* node = Add(data);
        nodesMap[nodeIndex] = node;

        delete[] data;
    }

    // Read random links data
    ListNode* current = head;
    while (current)
    {
        int32_t randIndex;
        fread(&randIndex, sizeof(randIndex), 1, file);

        if (randIndex != -1)
            current->rand = nodesMap[randIndex];

        current = current->next;
    }
}


ListNode* List::Add(const char* data)
{
    // Would be neat to use some kind of linear allocator here
    // to store nodes linearly in memory and avoid cache misses while iterating through the list
    // It would also make random links serialization trivial (node index would be just the distance between head and current node pointers)
    ListNode* node = new ListNode();

    if (data)
        node->data = data;

    if (!head)
    {
        head = node;
        tail = node;
    }
    else
    {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }

    // Set random link
    const uint32_t randomIndex = std::rand() % (count + 1);
    uint32_t index = 0;

    ListNode* current = head;
    while (current)
    {
        if (index == randomIndex)
        {
            node->rand = current;
            break;
        }

        index++;
        current = current->next;
    }

    count++;
    return node;
}


void List::Clear()
{
    ListNode* current = head;

    while (current != nullptr)
    {
        const ListNode* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}


void List::Print() const
{
    if (!head)
    {
        std::cout << "Empty" << std::endl;
        return;
    }

    ListNode* current = head;
    while (current != nullptr)
    {
        std::cout << "Data: " << current->data << " Random: " << (current->rand ? current->rand->data : "null") << std::endl;
        current = current->next;
    }
}
