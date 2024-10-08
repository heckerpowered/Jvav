#pragma once

#include <functional>

#include <fast_io.h>

#include "fast_io_core_impl/buffer_view.h"
#include "FunctionSignature.h"

namespace Mamba
{
    template<typename... T>
    concept Scannable = requires(T&&... Arguments) {
        fast_io::io::scan(std::forward<T>(Arguments)...);
    };

    template<typename T>
    class Scanner;

    template<typename ReturnType, typename... ArgumentTypes>
    class Scanner<ReturnType(ArgumentTypes...)>
    {
        std::function<ReturnType(ArgumentTypes...)> Function;

    public:
        template<typename T>
        Scanner(T&& Function) noexcept :
            Function(std::forward<T>(Function))
        {
        }

        void operator()() const noexcept
        {
            if constexpr (sizeof...(ArgumentTypes) == 0)
            {
                Function();
            }
            else
            {
                std::tuple<std::decay_t<ArgumentTypes>...> Arguments;
                [&]<std::size_t... Indices>(std::index_sequence<Indices...>) {
                    fast_io::io::scan(std::get<Indices>(Arguments)...);
                    Function(std::move(std::get<Indices>(Arguments)...));
                }(std::make_index_sequence<sizeof...(ArgumentTypes)>());
            }
        }

        template<typename InputStreamType>
        void operator()(InputStreamType&& InputStream) const noexcept
        {
            std::tuple<std::decay_t<ArgumentTypes>...> Arguments;
            [&]<std::size_t... Indices>(std::index_sequence<Indices...>) {
                fast_io::io::scan(std::forward<InputStreamType>(InputStream), std::get<Indices>(Arguments)...);
                Function(std::move(std::get<Indices>(Arguments)...));
            }(std::make_index_sequence<sizeof...(ArgumentTypes)>());
        }

        void operator()(fast_io::ibuffer_view Buffer) const noexcept
        {
            operator()<fast_io::ibuffer_view>(Buffer);
        }
    };

    template<typename ReturnType, typename... Args>
    Scanner(ReturnType (*)(Args...)) -> Scanner<ReturnType(Args...)>;

    template<typename FunctionType, typename Signature = FunctionSignature<decltype(&FunctionType::operator())>::Type>
    Scanner(FunctionType) -> Scanner<Signature>;

    struct PackagedScanner
    {
        template<typename T>
        [[nodiscard]] PackagedScanner(const Scanner<T>& Scanner) noexcept
        {
            Proxy = &Invoker<T>;
            RawScanner = &Scanner;
        }

        void operator()() const noexcept
        {
            Proxy(RawScanner);
        }

    private:
        template<typename T>
        static void Invoker(const void* RawScanner) noexcept
        {
            (*static_cast<const Scanner<T>*>(RawScanner))();
        }

        void (*Proxy)(const void*);
        const void* RawScanner;
    };
} // namespace Mamba