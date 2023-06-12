#include <iostream>
#include <fstream>
#include "List.h"


/*
 * Вяткин Денис Евгеньевич
 * 04.06.2023. ~4 часа
 */


// Support any signed number (32-bit by default).
template<class T = int32_t>
void PrintSignedBinary(T number)
{
    static_assert(std::is_signed_v<T> == true, "This function supports only signed number types");

    const size_t bits = sizeof(T) * CHAR_BIT;

    for (int32_t bitIndex = bits - 1; bitIndex >= 0; --bitIndex)
    {
        if (number >> bitIndex & 1)
            std::cout << "1";
        else
            std::cout << "0";

        // Group bits by 4-bit groups for readability.
        if (bitIndex % 4 == 0)
            std::cout << " ";
    }

    std::cout << std::endl;
}


void RemoveDups(char* str)
{
    if (!str)
        return;

    char* readPtr = str;
    char* writePtr = str;

    while (*readPtr)
    {
        if (readPtr == str || *readPtr != *(readPtr - 1))
        {
            *writePtr = *readPtr;
            ++writePtr;
        }

        ++readPtr;
    }

    *writePtr = 0;
}


void TestPrintSignedBinary()
{
    std::cout << "Task 1:" << std::endl;

    PrintSignedBinary(0);
    PrintSignedBinary(1);
    PrintSignedBinary(-1);
    PrintSignedBinary(6);
    PrintSignedBinary(-6);
    PrintSignedBinary(INT32_MAX);
    PrintSignedBinary(INT32_MIN);
    PrintSignedBinary<long long>(LONG_MAX);
    PrintSignedBinary<long long>(LONG_MIN);
    PrintSignedBinary<short>(SHRT_MAX);
    PrintSignedBinary<short>(SHRT_MIN);
}


void TestRemoveDups()
{
    std::cout << "Task 2:" << std::endl;

    char data1[] = "AAA BBB AAA";
    RemoveDups(data1);
    std::cout << data1 << std::endl;

    char data2[] = "";
    RemoveDups(data2);
    std::cout << data2 << std::endl;

    char data3[] = "ABCDEFG";
    RemoveDups(data3);
    std::cout << data3 << std::endl;

    char data4[] = "AAAAAAAAA";
    RemoveDups(data4);
    std::cout << data4 << std::endl;
}


void TestList()
{
    std::cout << "Task 3:" << std::endl;

    // For random links generation
    // Didn't use modern C++ random number generators for simplicity
    std::srand(std::time(nullptr));

    List list;
    list.Add("Node 1");
    list.Add("Node 2");
    list.Add("Node 3");
    list.Add("Node 4");
    list.Add("Node 5");

    FILE* file;

    fopen_s(&file, "test.bin", "wb");
    list.Serialize(file);
    fclose(file);

    list.Print();
    list.Clear();

    fopen_s(&file, "test.bin", "rb");
    list.Deserialize(file);
    fclose(file);

    std::cout << std::endl;
    list.Print();
}


int main()
{
    TestPrintSignedBinary();
    std::cout << std::endl;

    TestRemoveDups();
    std::cout << std::endl;

    TestList();
}