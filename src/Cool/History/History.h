#pragma once

#include "internal/Action.h"

namespace Cool {

class History {
public:
	History() = default;
	~History() = default;

	/// <summary>
	/// Begins an undo group. An undo group contains actions that will all be done / undone at once as we move forward / backward in the history.
	/// </summary>
	void begin_undo_group();

	/// <summary>
	/// Adds an action to the current undo group.
	/// </summary>
	/// <param name="action"></param>
	void add_action(Action action);

	/// <summary>
	/// Ends the current undo group and pushes it in the history.
	/// </summary>
	void end_undo_group();

	/// <summary>
	/// Undoes one undo group. Does nothing if we already are at the beginning of the history.
	/// </summary>
	void move_backward();

	/// <summary>
	/// Applies one undo group. Does nothing if we already are at the end of the history.
	/// </summary>
	void move_forward();

private:
	int nbOfActionsBetweenThisAndPreviousUndoGroup(int index);

private:
	std::vector<Action> m_actions;
	std::vector<int> m_cumulNbOfActionsToGetToThisUndoGroup;
	std::vector<Action> m_tmpActionBuffer;
	int m_index = -1;
	int m_indexOfCumulfNbOfActions = -1;
#ifndef NDEBUG
	bool _undo_group_open = false;
#endif
/*
	We are always in the state m_actions[m_index] (m_index == -1 means that we are in the initial state, no action ever performed)
*/
};

} // namespace Cool