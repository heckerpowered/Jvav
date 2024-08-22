#include <concepts>
#include <ranges>

namespace Mamba
{
    template<typename ElemT, typename StepFunctorT, typename IsEndFunctorT>
    class RecursiveView : public std::ranges::view_interface<RecursiveView<ElemT, StepFunctorT, IsEndFunctorT>>
    {
        class Iterator
        {
            ElemT CurrentElem{};
            const RecursiveView* Parent{}; 

        public:
            constexpr Iterator() requires std::default_initializable<ElemT> = default;
            Iterator() requires std::default_initializable<ElemT> && std::default_initializable<StepFunctorT> = default;

            constexpr Iterator(ElemT Elem, const RecursiveView* Parent)
            : CurrentElem{ std::move(Elem) }
            , Parent{ std::move(Parent) }
            {}

            constexpr bool IsEnd()const
            {
                return Parent->IsEndFunctor(CurrentElem);
            }

            ElemT operator*()const
            {
                return CurrentElem;
            }

            Iterator& operator++()
            {
                CurrentElem = Parent->StepFunctor(CurrentElem);
                return *this;
            }

            Iterator operator++(int)
            {
                auto Copy = *this;
                ++*this;
                return Copy;
            }
        };

        class Sentinel
        {
        public:
            constexpr Sentinel() = default;

            constexpr friend bool operator==(const class Iterator& Iterator, const class Sentinel&)
            {
                return Iterator.IsEnd();
            }            
        };

        ElemT FirstElem{};
        StepFunctorT StepFunctor{};
        IsEndFunctorT IsEndFunctor{};

    public:
        RecursiveView() requires std::default_initializable<ElemT> && std::default_initializable<StepFunctorT> = default;
        
        constexpr RecursiveView(ElemT FirstElem, StepFunctorT StepFunctor, IsEndFunctorT IsEndFunctor) noexcept
        : FirstElem{ std::move(FirstElem) }
        , StepFunctor{ std::move(StepFunctor) }
        , IsEndFunctor{ std::move(IsEndFunctor) }
        {}

        constexpr Iterator begin()const noexcept(std::is_nothrow_copy_constructible_v<ElemT>)
        {
            return { FirstElem, this };
        }

        constexpr Sentinel end()const noexcept
        {
            return {};
        }
    };

    template<typename ElemT, typename StepFunctorT, typename IsEndFunctorT>
    RecursiveView(ElemT, StepFunctorT, IsEndFunctorT) -> RecursiveView<ElemT, StepFunctorT, IsEndFunctorT>;

    template<typename ElemT, typename StepFunctorT, typename IsEndFunctorT>
    constexpr auto Recursive(ElemT&& FirstElem, StepFunctorT&& StepFunctor, IsEndFunctorT&& IsEndFunctor)
    {
        return RecursiveView{ 
            std::forward<ElemT>(FirstElem), 
            std::forward<StepFunctorT>(StepFunctor),
            std::forward<IsEndFunctorT>(IsEndFunctor) 
        };
    }
}