#pragma once

/**
 * @brief Based on https://youtu.be/ejF6qqohp3M?t=2896
 *
 *
 */

namespace Cool {

// template<typename T>
using T = int;
class Registry {
public:
    struct Id {
        size_t id;
        Id() = default;

    private:
        friend class Registry;
        explicit Id(size_t id)
            : id(id)
        {
        }
    };

    [[nodiscard]] auto emplace(T&& value) -> Id
    {
        list_.emplace_back(id_for_next_value_, std::move(value));
        ++nb_non_empty_slots_;
        return Id{id_for_next_value_.id++};
    }

    [[nodiscard]] auto size() -> size_t
    {
        return nb_non_empty_slots_;
    }

    T* get(Id id)
    {
        auto p = std::lower_bound(
            list_.begin(), list_.end(), id,
            [](const auto& a, const auto& b) { return a.first.id < b.id; }
        );
        if (p == list_.end() || p->first.id != id.id || !p->second)
        {
            return nullptr;
        }
        else
        {
            return &p->second.value();
        }
    }

    void remove(Id id)
    {
        auto p = std::lower_bound(
            list_.begin(), list_.end(), id,
            [](const auto& a, const auto& b) { return a.first.id < b.id; }
        );
        if (p == list_.end() || p->first.id != id.id || !p->second)
            return;

        p->second.reset();
        --nb_non_empty_slots_;

        if (nb_non_empty_slots_ < list_.size() / 2)
        {
            list_.erase(
                std::remove_if(list_.begin(), list_.end(), [](const auto& e) { return !e.second; }),
                list_.end()
            );
        }
    }

private:
    std::vector<std::pair<Id, std::optional<T>>> list_;
    size_t                                       nb_non_empty_slots_ = 0;
    Id                                           id_for_next_value_{0};
};

} // namespace Cool

#include <doctest/doctest.h>
#if DEBUG
TEST_CASE("Registry")
{
    Cool::Registry registry;
    CHECK(registry.size() == 0);
    auto id1 = registry.emplace(1);
    CHECK(registry.size() == 1);
    auto id2 = registry.emplace(2);
    CHECK(registry.size() == 2);
    // _registry.remove(0);
    // _registry.remove(Registry::Id{0});
    registry.remove(id1);
    CHECK(registry.size() == 1);
    auto* p1 = registry.get(id1);
    CHECK(p1 == nullptr);
    auto* p2 = registry.get(id2);
    CHECK(p2 != nullptr);
    CHECK(*p2 == 2);
}
#endif