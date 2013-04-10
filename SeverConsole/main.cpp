#include <iostream>
#include "Sys.h"
#include"boost/asio.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace std;

int main()
{
	Sys::Initialize();

	Sys::Run();
}