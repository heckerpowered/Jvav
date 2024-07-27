#include "BoundConstant.h"

#include "Literal.h"

using namespace Mamba;

BoundConstant::BoundConstant(const std::shared_ptr<const struct Literal> Value) noexcept : Value(Value) {}

const LiteralValue* BoundConstant::operator->() const noexcept
{
    return &Value->Value;
}