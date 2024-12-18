#pragma once

#include <vector>

#include "Diagnostic.h"
#include "SyntaxKind.h"
#include "SyntaxNode.h"
#include "SyntaxToken.h"
#include "TextLocation.h"
#include "TypeSymbol.h"

namespace Mamba
{
    class DiagnosticBag : public std::vector<struct Diagnostic>
    {
    public:
        using Super = std::vector<struct Diagnostic>;

    private:
        template<typename... T>
        void ReportError(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Error, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportWarning(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

        template<typename... T>
        void ReportInformation(TextLocation Location, T&&... Args) noexcept
        {
            emplace_back(DiagnosticSeverity::Warning, Location, Concat(std::forward<T>(Args)...));
        }

    public:
        /**
         * Report a invalid character.
         * e.g. "Invalid character 'x'."
         */
        void ReportInvalidCharacter(TextLocation Location, Char Character) noexcept;

        /**
         * Report a string literal is unterminated.
         * e.g. "Unterminated string literal."
         */
        void ReportUnterminatedString(TextLocation Location) noexcept;

        /**
         * Report a invalid decimal literal.
         * e.g. "Invalid decimal literal 'x'."
         */
        void ReportInvalidDecimal(TextLocation Location, StringView Literal) noexcept;

        /**
         * Report a invalid hexadecimal literal.
         * e.g. "Invalid hexadecimal literal 'x'."
         */
        void ReportInvalidHexadecimal(TextLocation Location, StringView Literal) noexcept;

        /**
         * Report a invalid binary literal.
         * e.g. "Invalid binary literal 'x'."
         */
        void ReportInvalidBinary(TextLocation Location, StringView Literal) noexcept;

        /**
         * Report a invalid octal literal.
         * e.g. "Invalid octal literal 'x'."
         */
        void ReportInvalidOctal(TextLocation Location, StringView Literal) noexcept;

        /**
         * Report a unexpected token, specialize information based on expected type.
         * e.g. "Unexpected Token 'x', Expected: 'y'"
         * e.g. "There should be an identifier." (When expected identifier token)
         */
        void ReportUnexpectedToken(TextLocation Location, SyntaxKind Kind, SyntaxKind ExpectedKind) noexcept;

        /**
         * Report that the value of the expression is discarded, usually found in expression statements.
         * e.g. "the expression value is discarded"
         */
        void ReportDiscardExpressionValue(TextLocation Location) noexcept;

        /**
         * Report error that a variable is already declared when declare a variable.
         * e.g. "Variable 'x' is already declared at here: <line number>:<start character>"
         */
        void ReportVariableAlreadyDeclared(TextLocation Location, StringView Name) noexcept;

        /**
         * Report the code in the specified location is unreachable.
         * e.g. "Unreachable code."
         */
        void ReportUnreachableCode(TextLocation Location) noexcept;

        /**
         * Report the code is unreachable, the location to be reported is depends on the node.
         * When the node is a block statement, report the first statement, if there's no statements
         * in the block statement, this function do nothing. For all other situations, report the whole
         * expression or statement.
         */
        void ReportUnreachableCode(const SyntaxNode* Node) noexcept;

        /**
         * Report error for undeclared identifier is used.
         * e.g. "Undeclared identifier 'x'"
         */
        void ReportUndeclaredIdentifier(TextLocation Location, StringView Name) noexcept;

        /**
         * Report error for ambiguous identifier, cannot select the best candidate in the overload set.
         */
        void ReportAmbiguousIdentifier(TextLocation Location, StringView Name) noexcept;

        /**
         * Report error for unexpected type, expected type and actual type.
         * e.g. "Expected type 'int', but got type 'float'."
         */
        void ReportTypeMismatch(TextLocation Location, const TypeSymbol& ExpectedType, const TypeSymbol& ActualType) noexcept;

        /**
         * Report error unary expression, which the operator is not defined for the given type.
         * e.g. "Undefined unary operator '-' for type 'string'."
         */
        void ReportUndefinedUnaryOperator(TextLocation Location, const SyntaxToken* OperatorToken, const TypeSymbol& OperandType) noexcept;

        /**
         * Report error binary expression, which the operator is not defined for the given types.
         * e.g. "Undefined binary operator '+' for types 'int' and 'float'."
         */
        void ReportUndefinedBinaryOperator(TextLocation Location, const TypeSymbol& LeftType, const SyntaxToken* OperatorToken, const TypeSymbol& RightType) noexcept;

        /**
         * Report error for trying to assign to an immutable variable.
         * e.g. "Cannot assign to variable 'x' because it is immutable."
         */
        void ReportVariableImmutable(TextLocation Location, StringView Name) noexcept;

        /**
         * Report error for function call with wrong argument count, expected count and actual count,
         * e.g. "Expected 2 arguments, but got 3."
         */
        void ReportArgumentCountMismatch(TextLocation Location, std::size_t ExpectedCount, std::size_t ActualCount) noexcept;
    };

} // namespace Mamba