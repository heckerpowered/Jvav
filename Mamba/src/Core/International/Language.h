#pragma once

#include <string_view>

#include <fast_io.h>

namespace Mamba
{
    struct LanguageIntermediateRepresentation
    {
        std::string_view Name;
        std::string_view Content;
        std::size_t InjectPointCount;
    };

    struct Language
    {
        std::string_view LanguageCode;
    };

    class LanguageManager
    {
    public:
        static constexpr std::string_view LanguagesFolder = "Languages/";

        static Language LoadLanguage(std::string_view LanguageCode) noexcept
        {
            auto LanguageFile = fast_io::concat("Languages/", LanguageCode);
            auto FileLoader = fast_io::native_file_loader(LanguageFile);
        }
    };
} // namespace Mamba