#pragma once

#include <functional>

namespace Cool {

class Action{
public:
	Action() = default;
	Action(std::function<void()> howToDo, std::function<void()> howToUndo)
		: _do(howToDo), _undo(howToUndo)
	{}
	~Action() = default;

	inline void Do() const { _do(); }
	inline void Undo() const { _undo(); }

private:
	std::function<void()> _do   = [](){};
	std::function<void()> _undo = [](){};
};

} // namespace Cool