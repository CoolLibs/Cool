#include "Editor.h"
#include "EditorImpl.h"

namespace Cool {

NodesEditor::NodesEditor()
    : _pimpl{std::make_unique<NodesEditorImpl>()}
{
}

} // namespace Cool