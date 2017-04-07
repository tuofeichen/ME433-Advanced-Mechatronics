#include <math.h>
#include <iostream>

#define PI 3.1415

void generateSin(int res, char* out)
{
    int i = 0;
            
    for (i = 0; i < res; i++)
    {
        *out++= 127+128*sin(2*PI/res*i);//
    }
    
    return; 
}
void generateTrigArray(int res, char* out)
{
    int i = 0;
            
    for (i = 0; i < res; i++)
    {
        *out++= 255*i/res; //
    }
    
    return; 
};


using namespace std;
int main()
{
	char arr[100];
	char trig[100];
	generateTrigArray(100,trig);
	generateSin(100,arr);
	cout <<"sine wave is " << endl;
	for (int i = 0;i<100;i++)
	{
		bitset<8> x(trig[i]);
		cout << x  <<" , " << endl ;
	}
	
}
