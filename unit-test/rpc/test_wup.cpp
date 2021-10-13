
#include "hello_test.h"

TEST_F(HelloTest, wupSync)
{
	forEach([&](){
		checkWup();
	});
}
