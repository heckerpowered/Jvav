#include "BoundLiteralExpression.h"
#include "MambaCore.h"
#include "TypeSymbol.h"

#include <cstdint>
#include <fast_io.h>
#include <limits>
#include <source_location>
#include <utility>

using namespace Mamba;

BoundLiteralExpression::BoundLiteralExpression(
    const SyntaxNode* Syntax,
    Literal Value
) noexcept :
    Super(Syntax), Value(Value)
{
}

BoundNodeKind BoundLiteralExpression::Kind() const noexcept
{
    return BoundNodeKind::LiteralExpression;
}

const TypeSymbol* BoundLiteralExpression::Type() const noexcept
{
    return std::visit(
        []<typename T>(T) {
            if constexpr (std::is_same_v<T, LiteralType::String>)
            {
                return &TypeSymbol::String;
            }
            else if constexpr (std::is_same_v<T, LiteralType::Number>)
            {
                return &TypeSymbol::Int;
            }
            else if constexpr (std::is_same_v<T, LiteralType::Boolean>)
            {
                return &TypeSymbol::Bool;
            }
            else if constexpr (std::is_same_v<T, LiteralType::FloatingPoint>)
            {
                return &TypeSymbol::Double;
            }
            else if constexpr (std::is_same_v<T, LiteralType::Empty>)
            {
                InternalCompilerError(std::source_location::current(), "Literal expression with no empty type.");
                return &TypeSymbol::Void;
            }

            std::unreachable();
        },
        Value.Value
    );
}

Constant BoundLiteralExpression::ConstantValue() const noexcept
{
    return std::visit(
        []<typename T>(T&& Value) -> Constant {
            if constexpr (std::is_same_v<std::decay_t<T>, LiteralType::Number>)
            {
                if (Value <= static_cast<std::uint64_t>(std::numeric_limits<std::int32_t>::max()))
                {
                    return static_cast<std::int32_t>(Value);
                }
            }

            return Value;
        },
        Value.Value
    );
}
