# coroutine
A lightweight, cross-platforming coroutine library.
<p align="left">
    <a href="">
        <img src="https://img.shields.io/badge/OS-linux%20mac%20windows-orange.svg">
    </a>
    <a href="https://isocpp.org/">
        <img src="https://img.shields.io/badge/language-C%2B%2B11-blue.svg">
    </a>
    <a href="https://travis-ci.org/felixguendling/cista">
        <img src="https://travis-ci.org/felixguendling/cista.svg?branch=master">
    </a>
</p>

# test
```
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

```

# output:
## Two coroutines alternate in parallel in the main thread, no context switching.
```
test start
test 1 => 0
test 2 => 0
test 1 => 1
test 2 => 1
test 1 => 2
test 2 => 2
test 1 => 3
test 2 => 3
test 1 => 4
test 2 => 4
test end
```
