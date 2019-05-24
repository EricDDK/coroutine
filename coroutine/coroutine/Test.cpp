#include "iostream"
#include "Coroutine.h"

COROUTINE_NAMESPACE_USE

void test1()
{

}

void test2(int a)
{

}

void test()
{
	Coroutine c;
	size_t r1 = c.create(test1);
	//size_t r2 = Coroutine::create(std::bind(test1, 2));

	/*std::cout << "00" << std::endl;
	Coroutine::resume(r1);*/


}

int main()
{
	
	system("pause");
	return 1;
}