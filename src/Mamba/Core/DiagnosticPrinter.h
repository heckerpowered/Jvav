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

    template<std::integral char_type>
    constexpr void print_define(fast_io::io_reserve_type_t<char_type, Diagnostic>, auto out [[maybe_unused]], const Diagnostic& Diagnostic) noexcept
    {
        using namespace fast_io::io;

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

        auto Length = Diagnostic.Location.Text.Length();
        for (auto Index [[maybe_unused]] : std::views::iota(0uz, Indent))
        {
            print(out, " ");
        }

        auto RelativeBegin = Diagnostic.Location.StartCharacter() - Line.Start;
        for (auto Index [[maybe_unused]] : std::views::iota(0uz, RelativeBegin - 1))
        {
            print(out, " ");
        }

        print(out, Color("☝️🤓", Colors::BrightForegroundGreen));
        // for (auto Index [[maybe_unused]] : std::views::iota(1uz, Length))
        // {
        //   print(out, Color("~", Colors::BrightForegroundGreen));
        //}
    }
} // namespace Mamba