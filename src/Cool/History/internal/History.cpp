#include "../History.h"

namespace Cool {

void History::begin_undo_group() {
	m_tmpActionBuffer.resize(0);
#ifndef NDEBUG
	_undo_group_open = true;
#endif
}

void History::end_undo_group() {
	if (m_tmpActionBuffer.size() > 0) {
		//
		m_cumulNbOfActionsToGetToThisUndoGroup.resize(m_indexOfCumulfNbOfActions + 1);
		m_cumulNbOfActionsToGetToThisUndoGroup.push_back(m_tmpActionBuffer.size()+ (m_indexOfCumulfNbOfActions>-1 ? m_cumulNbOfActionsToGetToThisUndoGroup[m_indexOfCumulfNbOfActions] : 0));
		m_indexOfCumulfNbOfActions++;
		//
		m_actions.resize(m_indexOfCumulfNbOfActions>0 ? m_cumulNbOfActionsToGetToThisUndoGroup[m_indexOfCumulfNbOfActions-1] : 0);
		for (size_t i = 0; i < m_tmpActionBuffer.size(); ++i) {
			m_actions.push_back(m_tmpActionBuffer[i]);
			m_index++;
		}
	}
#ifndef NDEBUG
	else {
		Log::Warn("Empty undo group");
	}
	_undo_group_open = false;
#endif
}

void History::add_action(Action action) {
	assert(_undo_group_open && "add_action() must be called between a call to begin_undo_group() and a call to end_undo_group()");
	m_tmpActionBuffer.push_back(action);
}

void History::move_backward() {
	if (m_indexOfCumulfNbOfActions > -1) {
		for (int i = 0; i < nbOfActionsBetweenThisAndPreviousUndoGroup(m_indexOfCumulfNbOfActions); ++i) {
			m_actions[m_index].Undo();
			m_index--;
		}
		m_indexOfCumulfNbOfActions--;
	}
}

void History::move_forward() {
	if (m_indexOfCumulfNbOfActions < static_cast<int>(m_cumulNbOfActionsToGetToThisUndoGroup.size() - 1)) { // cast to an int because size_t is an unsigned type and it causes a bug when m_indexOfCumulfNbOfActions == -1
		for (int i = 0; i < nbOfActionsBetweenThisAndPreviousUndoGroup(m_indexOfCumulfNbOfActions+1); ++i) {
			m_index++;
			m_actions[m_index].Do();
		}
		m_indexOfCumulfNbOfActions++;
	}
}

int History::nbOfActionsBetweenThisAndPreviousUndoGroup(int index){
	return m_cumulNbOfActionsToGetToThisUndoGroup[index] - (index > 0 ? m_cumulNbOfActionsToGetToThisUndoGroup[index-1] : 0);
}

} // namespace Cool