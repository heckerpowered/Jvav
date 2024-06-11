#pragma once

#include "MambaCore.h"
#include "TextLocation.h"

#include <memory>
#include <vector>

MAMBA_NAMESPACE_BEGIN

class DiagnosticBag : public std::vector<std::shared_ptr<const class Diagnostic>>
{
public:
    using Super = std::vector<const class Diagnostic>;

    void AddRange(const std::vector<std::shared_ptr<const class Diagnostic>>& Diagnostics) noexcept;

private:
    void ReportError(const TextLocation Location, const std::shared_ptr<const String>& Message);
    void ReportWarning(const TextLocation Location, const std::shared_ptr<const String>& Message);
    void ReportInformation(const TextLocation Location, const std::shared_ptr<const String>& Message);

    void ReportInvalidCharacter(const TextLocation Location, const Char Character);
    void ReportUnterminatedString(const TextLocation Location);
};

MAMBA_NAMESPACE_END