#include "BoundNode.h"

using namespace Mamba;

BoundNode::BoundNode(const std::shared_ptr<const class SyntaxNode> Syntax) noexcept : Syntax(Syntax) {}