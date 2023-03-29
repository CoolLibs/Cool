#pragma once
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>
#include "Node.h"

#define COOL_REGISTER_NODE_TYPE(type)                                                              \
    CEREAL_REGISTER_TYPE(Cool::Node::Model<type>);                                      /*NOLINT*/ \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::Node::Concept, Cool::Node::Model<type>); /*NOLINT*/
