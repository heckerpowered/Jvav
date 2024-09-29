#include "DiagnosticBag.h"
#include "BlockStatementSyntax.h"
#include "BreakStatementSyntax.h"
#include "CallExpressionSyntax.h"
#include "ContinueStatementSyntax.h"
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

#include <random>
#include <source_location>
#include <vector>

namespace Mamba
{
    void DiagnosticBag::ReportInvalidCharacter(TextLocation Location, Char Character) noexcept
    {
        ReportError(Location, TEXT("无效字符 '"), fast_io::mnp::chvw(Character), TEXT("'."));
    }

    void DiagnosticBag::ReportUnterminatedString(TextLocation Location) noexcept
    {
        ReportError(Location, TEXT("未结束的字符串字面量"));
    }

    void DiagnosticBag::ReportInvalidDecimal(TextLocation Location, StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("无效十进制字面量 '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidHexadecimal(TextLocation Location, StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("无效十六进制字面量 '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidBinary(TextLocation Location, StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("无效二进制字面量 '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportInvalidOctal(TextLocation Location, StringView Literal) noexcept
    {
        ReportError(Location, Concat(TEXT("无效八进制字面量 '"), Literal, TEXT("'.")));
    }

    void DiagnosticBag::ReportUnexpectedToken(TextLocation Location, SyntaxKind Kind, SyntaxKind ExpectedKind) noexcept
    {
        // Unexpected token 'Kind', Expected: 'ExpectedKind'.
        if (ExpectedKind == SyntaxKind::IdentifierToken)
        {
            ReportError(Location, TEXT("此处应有标识符"));
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

    void DiagnosticBag::ReportDiscardExpressionValue(TextLocation Location) noexcept
    {
        ReportWarning(Location, TEXT("表达式的结果被忽略"));
    }

    void DiagnosticBag::ReportVariableAlreadyDeclared(TextLocation Location, StringView Name) noexcept
    {
        // Variable 'Name' is already declared, previous declaration at FileName:StartLine:StartCharacter.
        ReportError(
            Location,
            TEXT("变量 '"),
            Name,
            TEXT("' 已在此处声明过: "),
            Location.FileName(),
            TEXT(":"),
            Location.StartLine(),
            TEXT(":"),
            Location.RelativeStartCharacter()
        );
    }

    void DiagnosticBag::ReportUnreachableCode(const TextLocation Location) noexcept
    {
        auto RandomDevice = std::random_device();
        auto Distribution = std::uniform_int_distribution<int>(0, 6);

        switch (Distribution(RandomDevice))
        {
            case 0:
                ReportWarning(Location, TEXT("现在流行收无效代码税"));
                break;
            case 1:
                ReportWarning(Location, TEXT("不可达的代码"));
                break;
            case 2:
                ReportWarning(Location, TEXT("这段代码永远不会被执行"));
                break;
            case 3:
                ReportWarning(Location, TEXT("关爱空巢代码"));
                break;
            case 4:
                ReportWarning(Location, TEXT("你猜我会不会把这段代码优化掉"));
                break;
            case 5:
                ReportWarning(Location, TEXT("b站搜索mq白, 问问他这段代码有什么问题"));
                break;
            case 6:
                ReportWarning(Location, TEXT("b代码把👴气笑了"));
                break;
            default:
            {
                auto Distribution = std::uniform_int_distribution<int>(0, 1);
                switch (Distribution(RandomDevice))
                {
                    case 0:
                        InternalCompilerError(std::source_location::current(), "谁在用Intel? 害得我编译器ICE了");
                    case 1:
                        InternalCompilerError(std::source_location::current(), "谁在用AMD? 害得我编译器ICE了");
                }
            }
        }
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
                ReportUnreachableCode(static_cast<const VariableDeclarationSyntax*>(Node)->Location());
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

    void DiagnosticBag::ReportUndeclaredIdentifier(TextLocation Location, StringView Name) noexcept
    {
        ReportError(Location, TEXT("未声明的标识符 '"), Name, TEXT("'."));
    }

    void DiagnosticBag::ReportAmbiguousIdentifier(TextLocation Location, StringView Name) noexcept
    {
        ReportError(Location, TEXT("标识符有歧义 '"), Name, TEXT("'."));
    }

    void DiagnosticBag::ReportTypeMismatch(TextLocation Location, const TypeSymbol& ExpectedType, const TypeSymbol& ActualType) noexcept
    {
        ReportError(Location, Concat(TEXT("此处需要"), ExpectedType.Name(), TEXT("类型，实际类型: '"), ActualType.Name(), TEXT(", 无法进行隐式转换")));
    }
} // namespace Mamba
