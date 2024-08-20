#include "BoundLiteralExpression.h"

#include <memory>

#include "BoundConstant.h"
#include "Literal.h"
#include "TypeSymbol.h"

using namespace Mamba;

std::shared_ptr<const TypeSymbol> GetType(const std::shared_ptr<const Literal> Value) noexcept
{
    static constexpr auto TableElem = []<size_t I>()
    {
        using Type = std::variant_alternative_t<I, decltype(Value->Value)>;
        if constexpr(std::same_as<Type, String>)
        {
            return &TypeSymbol::String;
        }
        else if constexpr(std::same_as<Type, std::int32_t>)
        {
            return &TypeSymbol::Int;
        }
        else if constexpr(std::same_as<Type, bool>)
        {
            return &TypeSymbol::Bool;
        }
        else
        {
            return &TypeSymbol::Void;
        }
    };

    static constexpr auto Table = []<size_t...I>(std::index_sequence<I...>)
    {
        return std::array{ TableElem.operator()<I>()... };
    }(std::make_index_sequence<std::variant_size_v<decltype(Value->Value)>>{});

    return *Table[Value->Value.index()];
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
