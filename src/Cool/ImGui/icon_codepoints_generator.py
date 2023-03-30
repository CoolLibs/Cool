
import json
from pathlib import Path


def icomoon():
    codepoint = 59648  # First codepoint in the font
    range = f"#define ICOMOON_RANGE_BEGIN {codepoint}\n"
    res = ""
    icons_reference_path = Path(__file__).parent.joinpath("IcoMoon-Free.json")
    with open(icons_reference_path, 'r') as file:
        data = json.loads(file.read())
        for entry in data:
            res += f'#define ICOMOON_{entry["name"].upper().replace("-", "_")} "{chr(codepoint)}"\n'
            codepoint += 1
    range += f"#define ICOMOON_RANGE_END {codepoint-1}\n"
    print("#pragma once\n\n" + range + res)


if __name__ == '__main__':
    icomoon()  # This will print to the terminal and you need to copy the output to a file. That is because I can't be bothered to figure out how to encode invalid Unicode codepoints to a file
