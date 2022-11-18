

Clang format src
```
find src -iname *.h -o -iname *.cpp | xargs clang-format -i -style="{IndentWidth: 4, TabWidth: 4}"
```

