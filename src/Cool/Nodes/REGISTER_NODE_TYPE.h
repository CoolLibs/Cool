#pragma once
#include "Node.h"

#if COOL_SERIALIZATION
#define COOL_REGISTER_NODE_TYPE(type)                                                              \
    CEREAL_REGISTER_TYPE(Cool::Node::Model<type>);                                      /*NOLINT*/ \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::Node::Concept, Cool::Node::Model<type>); /*NOLINT*/
#else
#define COOL_REGISTER_NODE_TYPE(type)
#endif