#pragma once

#include "ExpressionSyntax.h"

#include <memory>

namespace Mamba
{
    // A expression that represents a literal value, empty values are disallowed
    class LiteralExpressionSyntax : public ExpressionSyntax
    {
    public:
        using Super = ExpressionSyntax;

        [[nodiscard]] LiteralExpressionSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> LiteralToken
        ) noexcept;

        [[nodiscard]] LiteralExpressionSyntax(
            const std::shared_ptr<const class SyntaxTree> SyntaxTree,
            const std::shared_ptr<const class SyntaxToken> LiteralToken,
            // use class in this elaborated-type-specifier is valid, but may cause linker error under Microsoft ABI
            // https://eel.is/c++draft/dcl.dcl#dcl.type.elab-7
            // https://zh.cppreference.com/w/cpp/language/elaborated_type_specifier
            const std::shared_ptr<const struct Literal> Value
        ) noexcept;

        SyntaxKind Kind() const noexcept override;
        std::vector<std::shared_ptr<const class SyntaxNode>> Children() const noexcept override;

        const std::shared_ptr<const class SyntaxToken> LiteralToken;
        const std::shared_ptr<const struct Literal> Value;
    };
} // namespace Mamba