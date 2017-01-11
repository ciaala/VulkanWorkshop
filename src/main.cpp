#include <iostream>
#include "engine/vlk/ApplicationContext.h"

using namespace std;
using namespace vlk;

int main(int argc, char **argv) {
    ApplicationContext applicationContext;
    applicationContext.init();
    applicationContext.join();
    return 0;
}