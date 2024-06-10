#pragma once

#include "MambaCore.h"
#include "SyntaxKind.h"

#include <cstddef>
#include <vector>

MAMBA_NAMESPACE_BEGIN

class SyntaxFacts
{
    SyntaxFacts() = delete;
    ~SyntaxFacts() = delete;

public:
    [[nodiscard]] std::size_t GetUnaryOperatorPrecedence(const SyntaxKind Kind) noexcept;
    [[nodiscard]] std::size_t GetBinaryOperatorPrecedence(const SyntaxKind Kind) noexcept;
    [[nodiscard]] SyntaxKind GetKeywordKind(const StringView Text) noexcept;
    [[nodiscard]] std::vector<SyntaxKind> GetUnaryOperatorKinds() noexcept;
    [[nodiscard]] std::vector<SyntaxKind> GetBinaryOperatorKinds() noexcept;
    [[nodiscard]] StringView GetText(const SyntaxKind Kind) noexcept;
    [[nodiscard]] bool IsKeyword(const SyntaxKind Kind) noexcept;
    [[nodiscard]] bool IsToken(const SyntaxKind Kind) noexcept;
    [[nodiscard]] SyntaxKind GetBinaryOperatorOfAssignmentOperator(const SyntaxKind Kind) noexcept;
};

MAMBA_NAMESPACE_END