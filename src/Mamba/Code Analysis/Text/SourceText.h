#pragma once

#include "MambaCore.h"

#include <memory>
#include <vector>

MAMBA_NAMESPACE_BEGIN

class SourceText : public std::enable_shared_from_this<SourceText>
{
public:
    const std::shared_ptr<const String> Text;
    const std::shared_ptr<const String> FileName;
    const std::vector<std::shared_ptr<const class TextLine>> Lines;

    [[nodiscard]] static SourceText From(const std::shared_ptr<const String> Text,
                                         const std::shared_ptr<const String> FileName = {}) noexcept;

private:
    [[nodiscard]] SourceText(const std::shared_ptr<const String>& Text,
                             const std::shared_ptr<const String>& FileName = {}) noexcept;

    [[nodiscard]] static std::vector<std::shared_ptr<const class TextLine>> ParseLines(const SourceText& SourceText,
                                                                                       const String& Text) noexcept;

    static void AddLine(std::vector<std::shared_ptr<const class TextLine>>& Result,
                        const SourceText& SourceText,
                        const std::size_t Position,
                        const std::size_t LineStart,
                        const std::size_t LineBreakWidth) noexcept;

    [[nodiscard]] static std::size_t GetLineBreakWidth(const String& Text, const std::size_t Position) noexcept;

public:
    [[nodiscard]] auto&& operator[](this auto&& Self, const std::size_t Position) noexcept
    {
        return (*std::forward<decltype(Self)>(Self).Text)[Position];
    }

    [[nodiscard]] std::size_t Length() const noexcept;

    [[nodiscard]] std::size_t GetLineIndex(const std::size_t Position) const noexcept;

    [[nodiscard]] std::shared_ptr<const String> ToString() const noexcept;

    [[nodiscard]] std::shared_ptr<String> ToString(const std::size_t Start, const std::size_t Length) const noexcept;

    [[nodiscard]] std::shared_ptr<const String> ToString(const class TextSpan Span) const noexcept;

    [[nodiscard]] StringView ToView(const std::size_t Start, const std::size_t Length) const noexcept;

    [[nodiscard]] StringView ToView(const class TextSpan Span) const noexcept;
};

MAMBA_NAMESPACE_END