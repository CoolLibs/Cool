#pragma once
#include <ser20/archives/json.hpp>
#include <ser20/types/polymorphic.hpp>
#include "Node.h"

#define COOL_REGISTER_NODE_TYPE(type)                                                             \
    SER20_REGISTER_TYPE(Cool::Node::Model<type>);                                      /*NOLINT*/ \
    SER20_REGISTER_POLYMORPHIC_RELATION(Cool::Node::Concept, Cool::Node::Model<type>); /*NOLINT*/
