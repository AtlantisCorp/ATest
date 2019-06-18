# ATest: the very simple unit test
This library is a very tiny library to launch tests for any C/C++ functions. Basically, you 
create your units for the functions you want to test with `ATest::make_unit`, you group them
into some `UnitGroup` and then a big `Test` class to englobates everything.

### Example 

```c++
#include <ATest/ATTest.h>
#include <ATest/ATUnitGroup.h>

using namespace ATest;

int big_function(int, const char*, int);

int main() {
  try {
    Test my_test("hello");
    my_test.addUnit(make_unit(4, big_function, 3, "hello", 6));
    
    my_test.run();
    my_test.thow_error();
    
  } catch(const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
``` 

### Custom compareason functions
ATest supports any compareason function between two values of the same type. You just have to create a derived class from
`ATest::Comparator` and you're done, as in this example: 

```c++
template < typename T >
struct custom_compareason : public Comparator < T > {
  bool compare(T rhs, T lhs) const { return rhs + lhs == 8; }
};
```
