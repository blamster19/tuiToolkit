# tuiToolkit
tuiToolkit is a learning project with the goal of creating a simple and good looking windowed text user interface. The toolkit is written in C11.
## Usage
Include the header file in your project.
The toolkit currently relies on ncurses; to compile properly you have to use `-lncursesw` flag. Also make sure you have `libncurses5-dev` installed in your system.

Detailed instructions and function descriptions are in the header file.
## Features
The library features:
- windows with optional titlebars, borders and per-window color theme
- widgets:
  - label
  - button
  - checkbox
  - list
### TODO
1. near-future
- [ ] proper deallocation upon window ending
- [ ] widget focus controlled through parent window
- [ ] window focus controlled through screen
- [ ] widget feedback
- [ ] responsiveness
- [ ] more widgets
3. future
- [ ] rewrite the toolkit without ncurses dependency
- [ ] dialog boxes

The project is first and foremost an educational endeavor and as such I don't guarantee ever finishing those goals. If you feel like it, feel free to fork.
