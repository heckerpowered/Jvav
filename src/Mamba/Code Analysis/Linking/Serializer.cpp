#include "Serializer.h"
#include "fast_io.h"
#include "fast_io_core_impl/integers/impl.h"

using namespace Mamba;

void Serializer::Write(const std::span<const std::byte> DataView) noexcept
{
#if __cpp_lib_containers_ranges == 202202L
    Data.append_range(DataView);
#else
    const auto PreviousSize = Data.size();
    Data.resize(PreviousSize + DataView.size());
    std::memcpy(Data.data() + PreviousSize, DataView.data(), DataView.size());
#endif
}

void Serializer::Write(const StringView String) noexcept
{
    Write(String.size());

    const auto First = reinterpret_cast<const std::byte*>(String.data());
    const auto Last = reinterpret_cast<const std::byte*>(String.data() + String.size());
    Write(std::span<const std::byte>(First, Last));
}
