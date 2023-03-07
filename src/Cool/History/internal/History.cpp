#include "../History.h"

#if !defined(__GNUC__)
#pragma warning(push)
#pragma warning(disable : 4267)
#else
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif
#endif

namespace Cool {

void History::begin_undo_group()
{
    _tmp_action_buffer.resize(0);
#if DEBUG
    _an_undo_group_is_open = true;
#endif
}

void History::end_undo_group()
{
    if (!_tmp_action_buffer.empty())
    {
        //
        _cumul_nb_of_actions_to_get_to_this_undo_group.resize(_index_of_cumul_nb_of_actions + 1);
        _cumul_nb_of_actions_to_get_to_this_undo_group.push_back(_tmp_action_buffer.size() + (_index_of_cumul_nb_of_actions > -1 ? _cumul_nb_of_actions_to_get_to_this_undo_group[_index_of_cumul_nb_of_actions] : 0));
        _index_of_cumul_nb_of_actions++;
        //
        _actions.resize(_index_of_cumul_nb_of_actions > 0 ? _cumul_nb_of_actions_to_get_to_this_undo_group[_index_of_cumul_nb_of_actions - 1] : 0);
        for (const auto& action : _tmp_action_buffer)
        {
            _actions.push_back(action);
            _index++;
        }
    }
#if DEBUG
    else
    {
        Log::Debug::warning("History::end_undo_group", "Empty undo group");
    }
    _an_undo_group_is_open = false;
#endif
}

void History::add_action(Action action)
{
#if DEBUG // On some compilers asserts are compiled even when not in DEBUG
    assert(_an_undo_group_is_open && "add_action() must be called between a call to begin_undo_group() and a call to end_undo_group()");
#endif
    _tmp_action_buffer.push_back(action);
}

void History::move_backward()
{
    if (_index_of_cumul_nb_of_actions > -1)
    {
        for (int i = 0; i < nb_of_actions_between_this_and_previous_undo_group(_index_of_cumul_nb_of_actions); ++i)
        {
            _actions[_index].revert();
            _index--;
        }
        _index_of_cumul_nb_of_actions--;
    }
}

void History::move_forward()
{
    if (_index_of_cumul_nb_of_actions < static_cast<int>(_cumul_nb_of_actions_to_get_to_this_undo_group.size() - 1)) // cast to an int because size_t is an unsigned type and it causes a bug when _index_of_cumul_nb_of_actions == -1
    {
        for (int i = 0; i < nb_of_actions_between_this_and_previous_undo_group(_index_of_cumul_nb_of_actions + 1); ++i)
        {
            _index++;
            _actions[_index].apply();
        }
        _index_of_cumul_nb_of_actions++;
    }
}

int History::nb_of_actions_between_this_and_previous_undo_group(int index)
{
    return _cumul_nb_of_actions_to_get_to_this_undo_group[index] - (index > 0 ? _cumul_nb_of_actions_to_get_to_this_undo_group[index - 1] : 0);
}

} // namespace Cool

#if !defined(__GNUC__)
#pragma warning(pop)
#endif