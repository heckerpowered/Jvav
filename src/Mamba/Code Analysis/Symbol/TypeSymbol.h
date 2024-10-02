#pragma once

#include <memory>

#include "MambaCore.h"

#include "Symbol.h"

namespace Mamba
{
    class TypeSymbol : public Symbol
    {
    public:
        using Super = Symbol;

        [[nodiscard]] TypeSymbol(StringView Name) noexcept;
        virtual ~TypeSymbol() noexcept = default;

        SymbolKind Kind() const noexcept override;

        static const TypeSymbol Int;
        static const TypeSymbol Bool;
        static const TypeSymbol String;
        static const TypeSymbol Void;
        static const TypeSymbol Double;
        static const TypeSymbol Error;

        static bool IsBuiltInType(const TypeSymbol* Type) noexcept;
    };
} // namespace Mamba