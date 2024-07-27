#include "BoundLiteralExpression.h"

#include <memory>

#include "BoundConstant.h"
#include "Literal.h"
#include "TypeSymbol.h"

using namespace Mamba;

std::shared_ptr<const TypeSymbol> GetType(const std::shared_ptr<const Literal> Value) noexcept
{
    switch (Value->Type)
    {
        case LiteralType::String:
            return TypeSymbol::String;
        case LiteralType::SignedInt:
            return TypeSymbol::Int;
        case LiteralType::Boolean:
            return TypeSymbol::Bool;

        case LiteralType::Character:
        case LiteralType::UnsignedByte:
        case LiteralType::UnsignedShort:
        case LiteralType::UnsignedInt:
        case LiteralType::UnsignedLong:
        case LiteralType::SignedByte:
        case LiteralType::SignedShort:
        case LiteralType::SignedLong:
        case LiteralType::Double:
        case LiteralType::Float:
        case LiteralType::Empty:
            return TypeSymbol::Void;
    }
}

BoundLiteralExpression::BoundLiteralExpression(
    const std::shared_ptr<const SyntaxNode> Syntax,
    const std::shared_ptr<const Literal> Value
) noexcept :
    Super(Syntax), PrivateType(GetType(Value)), Value(Value)
{
}

BoundNodeKind BoundLiteralExpression::Kind() const noexcept
{
    return BoundNodeKind::LiteralExpression;
}

std::shared_ptr<const TypeSymbol> BoundLiteralExpression::Type() const noexcept
{
    return PrivateType;
}

std::shared_ptr<const BoundConstant> BoundLiteralExpression::ConstantValue() const noexcept
{
    return std::make_shared<BoundConstant>(Value);
}
