#include "iostream"
#include "Coroutine.h"

void test1()
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "test 1 => " << i << std::endl;
        coroutine::yield();
    }


}

void test2()
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "test 2 => " << i << std::endl;
        coroutine::yield();
    }
}

void test()
{
    size_t co1 = coroutine::create(test1);
    size_t co2 = coroutine::create(test2);
    std::cout << "test start" << std::endl;
    while (coroutine::getStatus(co1) != coroutine::kCoroutineStatus::Dead && coroutine::getStatus(co2) != coroutine::kCoroutineStatus::Dead)
    {
		coroutine::resume(co1);
		coroutine::resume(co2);
    }

    coroutine::destroy(co1);
    coroutine::destroy(co2);
    std::cout << "test end" << std::endl;
}

int main()
{
    test();
	system("pause");
	return 1;
}
