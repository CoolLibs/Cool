#pragma once
#include <list>

namespace Cool {

template<typename T>
class CircularBuffer {
public:
    using iterator = typename std::list<T>::iterator;

    explicit CircularBuffer(size_t max_size)
        : _max_size{max_size}
    {}

    void push_back(T const& t) { push_back_impl(t); }
    void push_back(T&& t) { push_back_impl(std::move(t)); }
    void push_front(T const& t) { push_front_impl(t); }
    void push_front(T&& t) { push_front_impl(std::move(t)); }

    [[nodiscard]] auto size() const -> size_t { return _container.size(); }
    [[nodiscard]] auto max_size() const -> size_t { return _max_size; }

    auto begin() { return _container.begin(); }
    auto begin() const { return _container.begin(); }
    auto end() { return _container.end(); }
    auto end() const { return _container.end(); }

    [[nodiscard]] auto is_empty() const -> bool { return _container.empty(); }

    auto underlying_container() -> std::list<T>& { return _container; }
    auto underlying_container() const -> std::list<T> const& { return _container; }

private:
    template<typename Tref>
    void push_back_impl(Tref&& t)
    {
        _container.push_back(std::forward<Tref>(t));
        shrink_front();
    }
    template<typename Tref>
    void push_front_impl(Tref&& t)
    {
        _container.push_front(std::forward<Tref>(t));
        shrink_back();
    }

    void shrink_front()
    {
        while (_container.size() > _max_size)
            _container.pop_front();
    }

    void shrink_back()
    {
        while (_container.size() > _max_size)
            _container.pop_back();
    }

private:
    std::list<T> _container;
    size_t       _max_size;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Container", _container),
            ser20::make_nvp("Max size", _max_size)
        );
    }
};

} // namespace Cool
