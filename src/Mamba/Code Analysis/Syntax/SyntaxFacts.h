#pragma once

#include "MambaCore.h"
#include "SyntaxKind.h"

#include <cstddef>
#include <vector>

namespace Mamba
{
    class SyntaxFacts
    {
        SyntaxFacts() = delete;
        ~SyntaxFacts() = delete;

    public:
        [[nodiscard]] static std::size_t GetUnaryOperatorPrecedence(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static std::size_t GetBinaryOperatorPrecedence(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static SyntaxKind GetKeywordKind(const StringView Text) noexcept;
        [[nodiscard]] static std::vector<SyntaxKind> GetUnaryOperatorKinds() noexcept;
        [[nodiscard]] static std::vector<SyntaxKind> GetBinaryOperatorKinds() noexcept;
        [[nodiscard]] static String GetText(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static String ToString(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static bool IsKeyword(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static bool IsToken(const SyntaxKind Kind) noexcept;
        [[nodiscard]] static SyntaxKind GetBinaryOperatorOfAssignmentOperator(const SyntaxKind Kind) noexcept;
    };
} // namespace Mamba