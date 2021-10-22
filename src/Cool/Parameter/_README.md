The concept of a parameter is a value that you can modify through the UI, query throughout the different systems, create presets for, track changes in an history, and so on.

Basically, this is a value that has meaning for the user and we want to provide as many utilities around it as possible.

## A common interface

We currently provide an ```IParameter``` interface. It is not strictly required since the polymorphism is achieved through templates rather than subtyping (inheritance), but it allows us to benefit from the amazing ```override``` keyword and also make sure we don't forget to implement any required method.

This approach could probably be replaced or at least complemented with C++20's concepts.

## Parameters.h

This file simply provides all the different parameters : Parameter::Float, Parameter::Color and so on.
This is what you need to include to create parameters

## AnyParameter.h

```AnyParameter``` is a std::variant that can be any of the parameter types. This is what is used in order to achieve polymorphism in ```ParameterDynamicList```