#include<iostream>
#include "sys.h"
#include "NetServer.h"

using namespace std;




bool Sys::Initialize()
{
	cout<<"system start"<<endl;
	return true;
}  
size_t ThreadNum = 4;

void Sys::Run()
{
	CNetServer netServer(8000, ThreadNum);
    netServer.Run();

	cout<<"system end"<<endl;
	system("pause");
}  

bool Sys::Deintitialize()
{
	cout<<"system end"<<endl;
	return true;
}