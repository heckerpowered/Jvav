#include "MambaCore.h"

MAMBA_NAMESPACE_BEGIN

enum class SyntaxKind
{
    // Tokens
    BadToken,
    EndOfFileToken,
    NumberToken,
    StringToken,
    PlusToken,
    MinusToken,
    StarToken,
    SlashToken
};

MAMBA_NAMESPACE_END