#include "Mamba.h"

using namespace Mamba;

constexpr auto JvavFile = R"(

)";

int main(int argc, char* argv[])
{
    return InitMamba(argc, argv) == -1;
}