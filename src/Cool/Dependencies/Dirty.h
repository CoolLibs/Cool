#pragma once

#include <reg/reg.hpp>
#include <utility>

namespace Cool {

namespace internal {
struct IsDirty {
    bool is_dirty = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(cereal::make_nvp("Is Dirty", is_dirty));
#else
        (void)archive;
#endif
    }
};

inline auto to_string(IsDirty is_dirty) -> std::string
{
    return is_dirty.is_dirty ? "Dirty" : "Clean";
}
} // namespace internal

class DirtyFlag {
public:
    DirtyFlag() = default; // For serialization :( Remove whenever possible

private:
    friend class DirtyManager_Ref;
    friend class DirtyFlagSetter_Ref;
    friend class DirtyFlagFactory_Ref;
    friend class IsDirty_Ref;

    explicit DirtyFlag(reg::SharedId<internal::IsDirty> id)
        : id{std::move(id)}
    {
    }

    reg::SharedId<internal::IsDirty> id;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(cereal::make_nvp("ID", id));
#else
        (void)archive;
#endif
    }
};

using DirtyRegistry = reg::Registry<internal::IsDirty>;

class DirtyFlagFactory_Ref {
public:
    explicit DirtyFlagFactory_Ref(DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    auto make()
    {
        return DirtyFlag{_registry.get().create_shared({})};
    }

private:
    std::reference_wrapper<DirtyRegistry> _registry;
};

class DirtyFlagSetter_Ref {
public:
    explicit DirtyFlagSetter_Ref(DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    void operator()(const DirtyFlag& flag, bool is_dirty)
    {
        _registry.get().set(flag.id.raw(), {is_dirty});
    }

private:
    std::reference_wrapper<DirtyRegistry> _registry;
};

class SetDirty_Ref {
public:
    explicit SetDirty_Ref(DirtyRegistry& registry)
        : _setter{registry}
    {
    }

    void operator()(const DirtyFlag& flag)
    {
        _setter(flag, true);
    }

private:
    DirtyFlagSetter_Ref _setter;
};

class SetClean_Ref {
public:
    explicit SetClean_Ref(DirtyRegistry& registry)
        : _setter{registry}
    {
    }

    void operator()(const DirtyFlag& flag)
    {
        _setter(flag, false);
    }

private:
    DirtyFlagSetter_Ref _setter;
};

class IsDirty_Ref {
public:
    explicit IsDirty_Ref(const DirtyRegistry& registry)
        : _registry{registry}
    {
    }

    auto operator()(const DirtyFlag& flag) const -> bool
    {
        const auto dirty = _registry.get().get(flag.id.raw());
        if (!dirty.has_value())
        {
            throw std::runtime_error{"DirtyFlag has been deleted but someone still had a handle to it!"};
        }
        return dirty->is_dirty;
    }

private:
    std::reference_wrapper<const DirtyRegistry> _registry;
};

} // namespace Cool
