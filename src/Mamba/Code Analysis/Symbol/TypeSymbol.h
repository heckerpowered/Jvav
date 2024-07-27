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

        [[nodiscard]] TypeSymbol(const std::shared_ptr<const String> Name) noexcept;
        virtual ~TypeSymbol() noexcept;

        SymbolKind Kind() const noexcept override;

        static const std::shared_ptr<const TypeSymbol> Int;
        static const std::shared_ptr<const TypeSymbol> Bool;
        static const std::shared_ptr<const TypeSymbol> String;
        static const std::shared_ptr<const TypeSymbol> Void;
    };
} // namespace Mamba