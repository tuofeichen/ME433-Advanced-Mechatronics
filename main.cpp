#include <iostream>
#include <stdio.h>

int len(char* arr);
using namespace std;
int main()
{
 char hello [100];
 sprintf(hello,"hello world");
 cout << "length of hw is " << len(hello) << endl;
 return 0;
 
}

int len(char* arr)
{
 return strlen(arr);
}
