#include "test_markdown_formatting.h"
#include "Cool/ImGui/markdown.h"

namespace Cool {

void test_markdown_formatting()
{
    ImGuiExtras::markdown(R"markdown(
# H1
## H2
### H3

*emphasis*
**strong emphasis**

Normal text
  Indent level 1
    Indent level 2
      Indent level 3
Normal text

Normal text
  * Unordered List level 1
    * Unordered List level 2
      * Unordered List level 3
      * Unordered List level 3
  * Unordered List level 1
Normal text

[Link](https://coollibs.github.io/)

![image](image identifier e.g. filename)

***
___

)markdown");
}

} // namespace Cool