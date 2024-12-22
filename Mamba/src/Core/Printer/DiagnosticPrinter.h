#pragma once

#include <concepts>
#include <fast_io.h>

#include "Colors.h"
#include "Diagnostic.h"
#include "fast_io_dsal/string.h"
#include "MambaCore.h"

namespace Mamba
{
    constexpr Colors DiagnosticColor(DiagnosticSeverity Severity) noexcept
    {
        switch (Severity)
        {
            case DiagnosticSeverity::Error:
                return Colors::BrightForegroundRed;
            case DiagnosticSeverity::Warning:
                return Colors::BrightForegroundYellow;
            case DiagnosticSeverity::Information:
                return Colors::BrightForegroundCyan;
            default:
                std::unreachable();
        }
    }

    constexpr fast_io::string_view SeverityString(DiagnosticSeverity Severity) noexcept
    {
        switch (Severity)
        {
            case DiagnosticSeverity::Error:
                return "错误";
            case DiagnosticSeverity::Warning:
                return "警告";
            case DiagnosticSeverity::Information:
                return "信息";
            default:
                std::unreachable();
        }
    }

    template<bool WithColor = true>
    constexpr fast_io::string SourceLocation(const Diagnostic& Diagnostic) noexcept
    {
        // <FileName>:<StartLine>:<StartCharacter>: <Severity>: <Message>
        if constexpr (WithColor)
        {
            auto SeverityColor = DiagnosticColor(Diagnostic.Severity);
            auto Severity = SeverityString(Diagnostic.Severity);

            return fast_io::concat_fast_io(
                Color(fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), Colors::Ignore, true, false),
                ":",
                Diagnostic.LineNumber(),
                ":",
                Diagnostic.Location.RelativeStartCharacter() + 1, // 0-based to 1-based
                ": ",
                Color(Severity, SeverityColor),
                Color(": ", SeverityColor),
                fast_io::mnp::code_cvt(Diagnostic.Message)
            );
        }

        auto Severity = SeverityString(Diagnostic.Severity);
        return fast_io::concat_fast_io(
            fast_io::mnp::code_cvt(Diagnostic.Location.FileName()),
            ":",
            Diagnostic.LineNumber(),
            ":",
            Diagnostic.Location.RelativeStartCharacter() + 1, // 0-based to 1-based
            ": ",
            Severity,
            ": ",
            fast_io::mnp::code_cvt(Diagnostic.Message)
        );
    }

    constexpr fast_io::string LineView(const Diagnostic& Diagnostic) noexcept
    {
        // <LineNumber> | <LineView>
        return fast_io::concat_fast_io("  ", Diagnostic.LineNumber(), " | ", fast_io::mnp::code_cvt(Diagnostic.LineView()));
    }

    template<bool WithColor = true, bool Fun = true>
    constexpr fast_io::string LocationGuide(const Diagnostic& Diagnostic) noexcept
    {
        constexpr std::string_view PointerCharacter = Fun ? "🤓👆" : "^";
        constexpr std::string_view WaveCharacter = Fun ? "👆" : "~";
        constexpr std::string_view IndentCharacter = " ";

        constexpr auto PointerCharacterSize = Fun ? 2 : 1;

        auto PrefixIndentLength = fast_io::concat_fast_io(Diagnostic.LineNumber()).size() + 3; // + 3 for spaces before '|'
        auto ContentIndentLength = Diagnostic.Location.RelativeStartCharacter() + 1;           // + 1 for space after '|'

        auto PrefixIndent = std::views::repeat(IndentCharacter, PrefixIndentLength) | std::views::join | std::ranges::to<std::string>();
        auto ContentIndent = std::views::repeat(IndentCharacter, ContentIndentLength) | std::views::join | std::ranges::to<std::string>();

        auto Length = Diagnostic.Location.View.size();
        auto WaveLength = Length <= PointerCharacterSize ? 0 : Length - PointerCharacterSize;
        auto Wave = std::views::repeat(WaveCharacter, WaveLength) | std::views::join | std::ranges::to<std::string>();

        if constexpr (WithColor)
        {
            return fast_io::concat_fast_io(PrefixIndent, "|", ContentIndent, Color(PointerCharacter, Colors::BrightForegroundGreen), Color(Wave, Colors::BrightForegroundGreen));
        }

        return fast_io::concat_fast_io(PrefixIndent, "|", ContentIndent, PointerCharacter, Wave);
    }

    template<std::integral char_type>
    constexpr void print_define(fast_io::io_reserve_type_t<char_type, Diagnostic>, auto out [[maybe_unused]], const Diagnostic& Diagnostic) noexcept
    {
        using namespace fast_io::io;

        print(
            out,
            SourceLocation(Diagnostic),
            "\n",
            LineView(Diagnostic),
            "\n",
            LocationGuide<true, false>(Diagnostic)
        );
    }
} // namespace Mamba