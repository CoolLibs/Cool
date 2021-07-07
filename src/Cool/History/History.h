#pragma once

#include "Action.h"

namespace Cool {

class History {
public:
	History() = default;

	/**
	 * @brief Begins an undo group. An undo group contains actions that will all be done / undone at once as we move forward / backward in the history
	 * 
	 */
	void begin_undo_group();

	/**
	 * @brief Adds an action to the current undo group
	 * 
	 * @param action 
	 */
	void add_action(Action action);

	/**
	 * @brief Ends the current undo group and pushes it in the history
	 * 
	 */
	void end_undo_group();

	/**
	 * @brief Undoes one undo group. Does nothing if we are already at the beginning of the history
	 * 
	 */
	void move_backward();

	/**
	 * @brief Applies one undo group. Does nothing if we are already at the end of the history
	 * 
	 */
	void move_forward();

private:
	int nb_of_actions_between_this_and_previous_undo_group(int index);

private:
	std::vector<Action> _actions;
	std::vector<int> _cumul_nb_of_actions_to_get_to_this_undo_group;
	std::vector<Action> _tmp_action_buffer;
	int _index = -1;
	int _index_of_cumul_nb_of_actions = -1;
#ifdef DEBUG
	bool _an_undo_group_is_open = false;
#endif
/*
	Class invariant : We are always in the state _actions[_index] (and _index == -1 means that we are in the initial state, no action ever performed)
*/
};

} // namespace Cool