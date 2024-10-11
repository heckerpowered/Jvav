#pragma once

namespace Mamba
{
    template<typename T>
    struct FunctionSignature;

    template<typename ReturnType, typename... Args>
    struct FunctionSignature<ReturnType (*)(Args...)>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename... Args>
    struct FunctionSignature<ReturnType (*)(Args...) noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...)>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) volatile>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const volatile>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...)&>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const&>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const volatile&>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) volatile noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const volatile noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) & noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const & noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) volatile & noexcept>
    {
        using Type = ReturnType(Args...);
    };

    template<typename ReturnType, typename Class, typename... Args>
    struct FunctionSignature<ReturnType (Class::*)(Args...) const volatile & noexcept>
    {
        using Type = ReturnType(Args...);
    };
} // namespace Mamba