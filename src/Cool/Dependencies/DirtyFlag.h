#pragma once

namespace Cool {

class DirtyFlag {
public:
    auto is_dirty() const -> bool { return *_is_dirty; }
    auto is_clean() const -> bool { return !is_dirty(); }

    void set_dirty() const { *_is_dirty = true; }
    void set_clean() const { *_is_dirty = false; }

private:
    std::shared_ptr<bool> _is_dirty{std::make_shared<bool>(true)}; // shared_ptr to make sure the address stays stable in memory and we can reference it in Commands stored in the History + survive through serialization

private:
    friend class ser20::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            ser20::make_nvp("Is dirty", _is_dirty)
        );
    }
    template<class Archive>
    void load(Archive& archive)
    {
        archive(_is_dirty);
        set_dirty(); // Always need to rerender after loading from file
    }
};

} // namespace Cool