#pragma once

#include "Colors.h"
#include "Diagnostic.h"
#include "fast_io_core_impl/codecvt/general.h"
#include <concepts>
#include <fast_io.h>

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

    constexpr std::string_view SeverityString(DiagnosticSeverity Severity) noexcept
    {
        switch (Severity)
        {
            case DiagnosticSeverity::Error:
                return "error";
            case DiagnosticSeverity::Warning:
                return "warning";
            case DiagnosticSeverity::Information:
                return "info";
            default:
                std::unreachable();
        }
    }

    template<bool WithColor = true>
    constexpr std::string SourceLocation(const Diagnostic& Diagnostic) noexcept
    {
        // <FileName>:<StartLine>:<StartCharacter>: <Severity>: <Message>
        if constexpr (WithColor)
        {
            auto SeverityColor = DiagnosticColor(Diagnostic.Severity);
            auto Severity = SeverityString(Diagnostic.Severity);

            return fast_io::concat(
                fast_io::mnp::code_cvt(Diagnostic.Location.FileName()),
                ":",
                Diagnostic.LineNumber(),
                ":",
                Diagnostic.Location.RelativeStartCharacter(),
                ": ",
                Color(Severity, SeverityColor),
                Color(": ", SeverityColor),
                fast_io::mnp::code_cvt(Diagnostic.Message)
            );
        }

        auto Severity = SeverityString(Diagnostic.Severity);
        return fast_io::concat(
            fast_io::mnp::code_cvt(Diagnostic.Location.FileName()),
            ":",
            Diagnostic.LineNumber(),
            ":",
            Diagnostic.Location.RelativeStartCharacter(),
            ": ",
            Severity,
            ": ",
            fast_io::mnp::code_cvt(Diagnostic.Message)
        );
    }

    constexpr std::string LineView(const Diagnostic& Diagnostic) noexcept
    {
        // <LineNumber> | <LineView>
        return fast_io::concat("  ", Diagnostic.LineNumber(), " | ", fast_io::mnp::code_cvt(Diagnostic.LineView()));
    }

    template<bool WithColor = true, bool Fun = true>
    constexpr std::string LocationGuide(const Diagnostic& Diagnostic) noexcept
    {
        constexpr std::string_view PointerCharacter = Fun ? "🤓👆" : "^";
        constexpr auto PointerCharacterSize = Fun ? 2 : 1;

        constexpr auto IndentCharacter = ' ';

        // 5 = indent (2 spaces) + " | "
        auto BaseIndent = fast_io::concat(Diagnostic.LineNumber()).length() + 5;
        auto ExtraIndent = Diagnostic.Location.RelativeStartCharacter();
        auto Length = Diagnostic.Location.View.size();
        auto Indent = std::string(BaseIndent + ExtraIndent - (Fun ? 2 : 0), IndentCharacter);

        if constexpr (Fun)
        {
            constexpr auto WaveCharacter = U'👆';
            auto WaveLength = Length <= PointerCharacterSize ? 0 : Length - PointerCharacterSize;
            if (WaveLength == 0)
            {
                return fast_io::concat(
                    Indent,
                    PointerCharacter
                );
            }

            auto WaveString = std::u32string(WaveCharacter, WaveLength);
            return fast_io::concat(
                Indent,
                PointerCharacter,
                fast_io::mnp::code_cvt(WaveString)
            );
        }

        constexpr auto WaveCharacter = '^';
        auto WaveString = std::string(WaveCharacter, Length - PointerCharacter.size());

        if constexpr (WithColor)
        {
            return fast_io::concat(
                Indent,
                Color(PointerCharacter, Colors::BrightForegroundGreen),
                Color(WaveString, Colors::BrightForegroundGreen)
            );
        }

        return fast_io::concat(
            Indent,
            PointerCharacter,
            WaveString
        );
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

        /**
        auto SeverityColor = DiagnosticColor(Diagnostic.Severity);
        auto Severity = SeverityString(Diagnostic.Severity);

        println(
            out,
            fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), // Color(fast_io::mnp::code_cvt(Diagnostic.Location.FileName()), Colors::BrightForegroundWhite),
            ":",                                                    // Color(":", Colors::BrightForegroundWhite),
            Diagnostic.Location.StartLine(),                        // Color(Diagnostic.Location.StartLine(), Colors::BrightForegroundWhite),
            ":",                                                    // Color(":", Colors::BrightForegroundWhite),
            Diagnostic.Location.StartCharacter(),                   // Color(Diagnostic.Location.StartCharacter(), Colors::BrightForegroundWhite),
            ": ",
            Color(Severity, SeverityColor),
            Color(": ", SeverityColor),
            fast_io::mnp::code_cvt(Diagnostic.Message) // Color(fast_io::mnp::code_cvt(Diagnostic.Message), Colors::BrightForegroundWhite)
        );

        auto Text = Diagnostic.Location.Text;
        auto LineIndex = Diagnostic.Location.StartLine();
        auto Line = Text.Lines()[LineIndex];
        auto LineView = Text.SubView(Line.Start, Line.Length);
        println(out, "  ", LineIndex + 1, " | ", fast_io::mnp::code_cvt(LineView));

        auto Indent = fast_io::concat(LineIndex + 1).length() + 5;

        for (auto Index [[maybe_unused]] : std::views::iota(0uz, Indent))
        {
            print(out, " ");
        }

        auto RelativeBegin = Diagnostic.Location.StartCharacter() - Line.Start;
        for (auto Index [[maybe_unused]] : std::views::iota(0uz, RelativeBegin - 2))
        {
            print(out, " ");
        }

        print(out, Color("🤓👆", Colors::BrightForegroundGreen));
        // for (auto Index [[maybe_unused]] : std::views::iota(1uz, Length))
        // {
        //   print(out, Color("~", Colors::BrightForegroundGreen));
        //}
**/
    }
} // namespace Mamba