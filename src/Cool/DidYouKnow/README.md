- You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.
- Drag from window menu button (upper-left button) to undock an entire node (all windows).
- Ctrl+click on a slider to input a value manually (and it also allows you to bypass the bounds of the slider !)
- Double-click on title bar to collapse window.
- double-click on lower right corner to auto fit window to its contents.
- TAB/SHIFT+TAB to cycle through keyboard editable fields.
- Text editor:
 - Hold SHIFT or use mouse to select text.
 - CTRL+Left/Right to word jump.
 - CTRL+Shift+Left/Right to select words.
 - CTRL+A or Double-Click to select all.
 - CTRL+X,CTRL+C,CTRL+V to use OS clipboard/
 - CTRL+Z,CTRL+Y to undo/redo.
 - ESCAPE to revert text to its original value.
- Double-click on title bar to collapse window.
- CTRL+Tab to select a window.
    ImGui::BulletText("While inputing text:\n");
    ImGui::Indent();
    ImGui::BulletText("CTRL+Left/Right to word jump.");
    ImGui::BulletText("CTRL+A or double-click to select all.");
    ImGui::BulletText("CTRL+X/C/V to use clipboard cut/copy/paste.");
    ImGui::BulletText("CTRL+Z,CTRL+Y to undo/redo.");
    ImGui::BulletText("ESCAPE to revert.");
    ImGui::Unindent();
    ImGui::BulletText("With keyboard navigation enabled:");
    ImGui::Indent();
    ImGui::BulletText("Arrow keys to navigate.");
    ImGui::BulletText("Space or enter to activate a widget.");
    ImGui::BulletText("Return to input text into a widget.");
    ImGui::BulletText("Escape to deactivate a widget, close popup, exit child window.");
    ImGui::BulletText("Alt to jump to the menu layer of a window.");