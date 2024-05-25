#pragma once

#include "VertexCore.h"
#include <memory>

_VERTEX_BEGIN

class SourceText : public std::enable_shared_from_this<SourceText> {
  public:
    const std::shared_ptr<const String> Text;
    const std::shared_ptr<const String> FileName;
    const Vector<std::shared_ptr<const class TextLine>> Lines;

    [[nodiscard]] static SourceText From(const std::shared_ptr<const String> &Text,
                                         const std::shared_ptr<const String> &FileName = {}) noexcept;

  private:
    [[nodiscard]] SourceText(const std::shared_ptr<const String> &Text,
                             const std::shared_ptr<const String> &FileName = {}) noexcept;

    [[nodiscard]] static Vector<std::shared_ptr<const class TextLine>> ParseLines(const SourceText &SourceText,
                                                                                  const String &Text) noexcept;

    static void AddLine(Vector<std::shared_ptr<const class TextLine>> &Result, const SourceText &SourceText,
                        const std::size_t Position, const std::size_t LineStart,
                        const std::size_t LineBreakWidth) noexcept;

    [[nodiscard]] static std::size_t GetLineBreakWidth(const String &Text, const std::size_t Position) noexcept;

  public:
    [[nodiscard]] auto &&operator[](this auto &&Self, const std::size_t Position) noexcept;

    [[nodiscard]] std::size_t Length() const noexcept;

    [[nodiscard]] std::size_t GetLineIndex(const std::size_t Position) const noexcept;

    [[nodiscard]] std::shared_ptr<const String> ToString() const noexcept;

    [[nodiscard]] std::shared_ptr<String> ToString(const std::size_t Start, const std::size_t Length) const noexcept;

    [[nodiscard]] std::shared_ptr<const String> ToString(const class TextSpan &Span) const noexcept;
};
_VERTEX_END