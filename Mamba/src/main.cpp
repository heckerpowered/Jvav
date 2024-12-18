#include "Mamba.h"

using namespace Mamba;

// The unit test needs Mamba, the entry point of Mamba causes
// the multiple definition of main function.
#if !defined(MAMBA_TEST)

int main(int argc, char* argv[])
{
    return InitMamba(argc, argv);
}

#endif