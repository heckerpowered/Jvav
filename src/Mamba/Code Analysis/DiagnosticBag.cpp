#include "DiagnosticBag.h"
#include "BlockStatementSyntax.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "ContinueStatementSyntax.h"
#include "Diagnostic.h"
#include "DoWhileStatementSyntax.h"
#include "ExpressionStatementSyntax.h"
#include "ForStatementSyntax.h"
#include "IfStatementSyntax.h"
#include "MambaCore.h"
#include "ReturnStatementSyntax.h"
#include "SyntaxFacts.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "TextLocation.h"
#include "VariableDeclarationSyntax.h"
#include "WhileStatementSyntax.h"

#include <vector>

namespace Mamba
{
    void DiagnosticBag::AddRange(const std::vector<std::shared_ptr<const Diagnostic>>& Diagnostics) noexcept
    {
#if __cpp_lib_containers_ranges == 202202L
        append_range(Diagnostics);
#else
        for (auto&& Diagnostic : Diagnostics)
        {
            emplace_back(std::forward<decltype(Diagnostic)>(Diagnostic));
        }
#endif
    }

    void DiagnosticBag::ReportError(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept
    {
        emplace_back(
            Hatcher([&]
                    { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Error, Location, Message); })
        );
    }

    void DiagnosticBag::ReportWarning(const TextLocation Location, const std::shared_ptr<const String> Message) noexcept
    {
        emplace_back(
            Hatcher([&]
                    { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Warning, Location, Message); })
        );
    }

    void DiagnosticBag::ReportInformation(
        const TextLocation Location,
        const std::shared_ptr<const String> Message
    ) noexcept
    {
        emplace_back(Hatcher(
            [&]
            { return std::make_shared<const Diagnostic>(DiagnosticSeverity::Information, Location, Message); }
        ));
    }

    void DiagnosticBag::ReportInvalidCharacter(const TextLocation Location, const Char Character) noexcept
    {
        const auto Message = std::make_shared<const String>(
            Hatcher([&]
                    { return Concat(TEXT("Invalid character '"), Character, TEXT("'.")); })
        );
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportUnterminatedString(const TextLocation Location) noexcept
    {
        const auto Message = std::make_shared<const String>(TEXT("Unterminated string literal."));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidDecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid decimal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidHexadecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid hexadecimal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidBinary(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid binary number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportInvalidOctal(const TextLocation Location, const StringView Literal) noexcept
    {
        const auto Message =
            std::make_shared<const String>(Concat(TEXT("Invalid octal number '"), Literal, TEXT("'.")));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportUnexpectedToken(
        const TextLocation Location,
        const SyntaxKind Kind,
        const SyntaxKind ExpectedKind
    ) noexcept
    {
        // Unexpected token 'Kind', Expected: 'ExpectedKind'.
        const auto Message = std::make_shared<const String>(Concat(
            TEXT("Unexpected token '"),
            SyntaxFacts::GetText(Kind),
            TEXT("'"),
            TEXT("Expected: '"),
            SyntaxFacts::ToString(ExpectedKind),
            TEXT("'.")
        ));
        ReportError(Location, Message);
    }

    void DiagnosticBag::ReportDiscardExpressionValue(const TextLocation Location) noexcept
    {
        const auto Message = std::make_shared<const String>(TEXT("The result of the expression is discarded."));
        ReportWarning(Location, Message);
    }

    void DiagnosticBag::ReportVariableAlreadyDeclared(const TextLocation Location, const StringView Name) noexcept
    {
        // Variable 'Name' is already declared, previous declaration at FileName:StartLine:StartCharacter.
        const auto Message = std::make_shared<const String>(Concat(
            TEXT("Variable '"),
            Name,
            TEXT("' is already declared, previous declaration at "),
            *Location.FileName(),
            TEXT(":"),
            Location.StartLine(),
            TEXT(":"),
            Location.StartCharacter()
        ));
    }

    void DiagnosticBag::ReportUnreachableCode(const TextLocation Location) noexcept
    {
        const auto Message = std::make_shared<const String>(TEXT("Unreachable code."));
        ReportWarning(Location, Message);
    }

    void DiagnosticBag::ReportUnreachableCode(const std::shared_ptr<const SyntaxNode> Node) noexcept
    {
        switch (Node->Kind())
        {
            case SyntaxKind::BlockStatement:
            {
                const auto Statements = std::static_pointer_cast<const BlockStatementSyntax>(Node)->Statements;
                if (!Statements.empty())
                {
                    ReportUnreachableCode(Statements.front());
                }
                return;
            }

            case SyntaxKind::VariableDeclaration:
                ReportUnreachableCode(std::static_pointer_cast<const VariableDeclarationSyntax>(Node)->Keyword->Location());
                return;
            case SyntaxKind::IfStatement:
                ReportUnreachableCode(std::static_pointer_cast<const IfStatementSyntax>(Node)->IfKeyword->Location());
                return;
            case SyntaxKind::WhileStatement:
                ReportUnreachableCode(std::static_pointer_cast<const WhileStatementSyntax>(Node)->WhileKeyword->Location());
                return;
            case SyntaxKind::DoWhileStatement:
                ReportUnreachableCode(std::static_pointer_cast<const DoWhileStatementSyntax>(Node)->WhileKeyword->Location());
                return;
            case SyntaxKind::ForStatement:
                ReportUnreachableCode(std::static_pointer_cast<const ForStatementSyntax>(Node)->Keyword->Location());
                return;
            case SyntaxKind::BreakStatement:
                ReportUnreachableCode(std::static_pointer_cast<const BreakStatementSyntax>(Node)->Keyword->Location());
                return;
            case SyntaxKind::ContinueStatement:
                ReportUnreachableCode(std::static_pointer_cast<const ContinueStatementSyntax>(Node)->Keyword->Location());
                return;
            case SyntaxKind::ReturnStatement:
                ReportUnreachableCode(std::static_pointer_cast<const ReturnStatementSyntax>(Node)->ReturnKeyword->Location());
                return;
            case SyntaxKind::ExpressionStatement:
                ReportUnreachableCode(std::static_pointer_cast<const ExpressionStatementSyntax>(Node)->Expression->Location());
                return;
            case SyntaxKind::CallExpression:
                ReportUnreachableCode(std::static_pointer_cast<const CallExpressionSyntax>(Node)->Identifier->Location());
                return;
            default:
                break;
        }
    }
} // namespace Mamba
