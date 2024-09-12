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
    void DiagnosticBag::AddRange(const std::vector<Diagnostic>& Diagnostics) noexcept
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

    void DiagnosticBag::ReportInvalidCharacter(const TextLocation Location, const Char Character) noexcept
    {
        ReportError(Location, TEXT("Invalid character '"), Character, TEXT("'."));
    }

    void DiagnosticBag::ReportUnterminatedString(const TextLocation Location) noexcept
    {
        ReportError(Location, TEXT("Unterminated string literal."));
    }

    void DiagnosticBag::ReportInvalidDecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("Invalid decimal number '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidHexadecimal(const TextLocation Location, const StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("Invalid hexadecimal number '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidBinary(const TextLocation Location, const StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("Invalid binary number '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidOctal(const TextLocation Location, const StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("Invalid octal number '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportUnexpectedToken(
        const TextLocation Location,
        const SyntaxKind Kind,
        const SyntaxKind ExpectedKind
    ) noexcept
    {
        // Unexpected token 'Kind', Expected: 'ExpectedKind'.
        if (ExpectedKind == SyntaxKind::IdentifierToken)
        {
            ReportError(Location, TEXT("不写参数类型胆大包天😡"));
        }
        else
        {
            ReportError(
                Location,
                TEXT("Unexpected token '"),
                SyntaxFacts::ToString(Kind),
                TEXT("' "),
                TEXT("Expected: '"),
                SyntaxFacts::ToString(ExpectedKind),
                TEXT("'.")
            );
        }
    }

    void DiagnosticBag::ReportDiscardExpressionValue(const TextLocation Location) noexcept
    {
        ReportWarning(Location, TEXT("The result of the expression is discarded."));
    }

    void DiagnosticBag::ReportVariableAlreadyDeclared(const TextLocation Location, const StringView Name) noexcept
    {
        // Variable 'Name' is already declared, previous declaration at FileName:StartLine:StartCharacter.
        ReportError(
            Location,
            TEXT("Variable '"),
            Name,
            TEXT("' is already declared, previous declaration at "),
            Location.FileName(),
            TEXT(":"),
            Location.StartLine(),
            TEXT(":"),
            Location.StartCharacter()
        );
    }

    void DiagnosticBag::ReportUnreachableCode(const TextLocation Location) noexcept
    {
        ReportWarning(Location, TEXT("Unreachable code."));
    }

    void DiagnosticBag::ReportUnreachableCode(const SyntaxNode* Node) noexcept
    {
        switch (Node->Kind())
        {
            case SyntaxKind::BlockStatement:
            {
                auto Statements = static_cast<const BlockStatementSyntax*>(Node)->Statements;
                if (!Statements.empty())
                {
                    ReportUnreachableCode(Statements.front());
                }
                return;
            }

            case SyntaxKind::VariableDeclaration:
                ReportUnreachableCode(static_cast<const VariableDeclarationSyntax*>(Node)->Keyword->Location());
                return;
            case SyntaxKind::IfStatement:
                ReportUnreachableCode(static_cast<const IfStatementSyntax*>(Node)->IfKeyword->Location());
                return;
            case SyntaxKind::WhileStatement:
                ReportUnreachableCode(static_cast<const WhileStatementSyntax*>(Node)->WhileKeyword->Location());
                return;
            case SyntaxKind::DoWhileStatement:
                ReportUnreachableCode(static_cast<const DoWhileStatementSyntax*>(Node)->WhileKeyword->Location());
                return;
            case SyntaxKind::ForStatement:
                ReportUnreachableCode(static_cast<const ForStatementSyntax*>(Node)->Keyword->Location());
                return;
            case SyntaxKind::BreakStatement:
                ReportUnreachableCode(static_cast<const BreakStatementSyntax*>(Node)->Keyword->Location());
                return;
            case SyntaxKind::ContinueStatement:
                ReportUnreachableCode(static_cast<const ContinueStatementSyntax*>(Node)->Keyword->Location());
                return;
            case SyntaxKind::ReturnStatement:
                ReportUnreachableCode(static_cast<const ReturnStatementSyntax*>(Node)->ReturnKeyword->Location());
                return;
            case SyntaxKind::ExpressionStatement:
                ReportUnreachableCode(static_cast<const ExpressionStatementSyntax*>(Node)->Expression->Location());
                return;
            case SyntaxKind::CallExpression:
                ReportUnreachableCode(static_cast<const CallExpressionSyntax*>(Node)->Identifier->Location());
                return;
            default:
                break;
        }
    }
} // namespace Mamba
