#pragma once

#include "MambaCore.h"

#include "DiagnosticBag.h"
#include "SourceText.h"
#include "SourceTextInfo.h"

namespace Mamba
{
    class Compiler
    {
        std::vector<SourceText> SourceTexts;
        DiagnosticBag Diagnostics;

        void PrivateAddSourceFile(const std::string_view FileName) noexcept;

        void ReportNoInputFiles() noexcept;

    public:
        template<std::integral CharType>
        void AddSourceFile(const std::basic_string_view<CharType> FileName) noexcept
        {
            if constexpr (std::is_same_v<CharType, char>)
            {
                PrivateAddSourceFile(FileName);
            }
            else
            {
                PrivateAddSourceFile(fast_io::concat(fast_io::mnp::code_cvt(FileName)));
            }
        }

        void Compile() noexcept
        {
            if (SourceTexts.empty())
            {
                ReportNoInputFiles();
                return;
            }
        }
    };
} // namespace Mamba