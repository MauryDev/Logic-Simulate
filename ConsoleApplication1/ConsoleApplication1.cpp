#include <iostream>
#include "Logic.h"

const char* GetTF(bool v)
{
    return v ? "TRUE" : "FALSE";
}
int main()
{
    Logic mylogic;

    auto input1 = mylogic.AddInput(false);
    auto input2 = mylogic.AddInput(true);
    auto input3 = mylogic.AddInput(true);

    auto output1 = mylogic.AddOutput();
    auto or_ = mylogic.AddOr();

    mylogic.AddES(input1, or_);
    mylogic.AddES(input2, or_);
    mylogic.AddES(input3, or_);

    mylogic.AddES(or_, output1);

    mylogic.Run();

    auto outputobj = mylogic.Get(output1);
    std::cout << GetTF(outputobj->Value);
}