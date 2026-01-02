# VSCode C++ Development Tools

A VSCode C++ development environment for Linux, richly populated with
quality-oriented tools.


## Motivation

VSCode users know that the application is highly expandable and flexible. They
also know that discovering all the tools and techniques to configure VSCode is
not easy. This repository presents a highly enriched VSCode environment for
quality-oriented C++ development on Linux.

A C++ statistics accumulator class helps to showcase the environment's
features. The class is similar to Boost's statistics accumulator, without the
Boost dependency. The code is not the point of this repository. It serves to
demonstrate the many additions required to turn VSCode in to a rich C++ IDE.
Those many additions are the point of this repository. It might be used as
a template for new VSCode C++ projects.

Developers using other IDEs, or even the command-line, might be interested
in pieces from this project. For example, the custom build tasks for mutation
testing and code sanitizers are powered by scripts easily transferred to other
C++ development environments.


## Overview

The developmment enviroonment is provided via two directories:

- .devcontainer

  A VSCode containerized development installlation based on Ubuntu.
  The developer works in the environment under their own account.

- .vscode

  VSCode settings, CMake variants, and custom tasks for C++ development.

The directories complement each other. The .vscode files provide settings
for the .devcontainer tools.

The resulting VSCode session has many integrated tools available from the
IDE's GUI:

- CMake tools with configuration pre-sets and build-target selection
- Auto-format on save (C++ and CMake files)
- Inline linter hints and git blame
- Test explorer and code coverage annotation
- Mutation testing
- Valgrind reports and code sanitizer reports

Read on below for detailed instructions in using the environment.

![VSCode screenshot showing the C++ developer environment](docs/VSCode.jpeg)


## Use The Environment

These instructions assume you are working on Linux with VSCode, git, and
docker installed. They also assume you have a GitHub account with a registered
SSH key so you can clone this repository and open it in VSCode.

```
git clone git@github.com:ian-fisher-developer/vscode-cpp-dev-tools.git

cd vscode-cpp-dev-tools

code .
```

VSCode recognizes the .devcontainer directory and offers to re-open the project
in the container. Or, click on `Open a Remote Window` (at the very left of the
status bar) and choose `Reopen in Container`. Note that, for the first time
only, it may take quite a while for VSCode to create the docker images.

When the DevContainer session is ready, follow these steps to exercise the
environment's C++ development features.

- Configure the build

  The CMake Tools extension has a build-kit selector in the VSCode status bar.
  Choose GCC 11.4.0. The extension runs the CMake configure step to the
  build/build-vscode-cmake-tools directory.

- Build all and use the test explorer

  Choose the `all` build target (in the status bar selector) and build it to
  create the library, the unit-test program, and the example programs.
  Open the test explorer side pane to view and run the unit tests.

  The test explorer has controls to run all the tests or a single test. Or, set
  `Start Continuous Run` (at the top of the test explorer) to automatically run
  the tests when they are rebuilt. Note that in the screen-shot above, the test
  explorer is moved to VSCode's secondary side bar, on the right.

- Try the code editor

  Open src/lib/StatisticsAccumulator.cpp from the file explorer.

  The code editor has git blame for the current line. Mouse-over the brief
  inline description for a more detailed popup.

  Note the "squiggle" suggestions from the linter, clang-tidy. Mouse-over the
  deliberate misnaming of variable 'dVal' for more information and a "fixit"
  for this violation of the project's code naming conventions.

  The clangd extension from the LLVM project provides a full C++ code model,
  enabling many code editing helpers such as code completion options. Note
  that VSCode tends to install GitHub CoPilot by default. The LLM's slop
  suggestions get in the way. Turn off GitHub CoPilot to return to accurate
  information from the code model.

  Ctrl-S auto-formats C++ and CMake files according to the project standards.

- Examine code coverage

  Switch from `all` to the `statistics_coverage_report` target and build it.

  Turn on the Coverage Gutters extension's `Watch` function (in the status bar)
  to add coverage annotation to the C++ code editor in the form of colored bars
  by the line numbers.

  Select `Coverage Gutters: Preview Coverage Report` from the editor's context
  menu to browse the [HTML report][1].

- Test the tests

  A custom build task provides [mutation testing][2] for the project, helping
  find gaps in the tests, even when code coverage is all green.
  Select `Run Build Task` from VSCode's `Terminal` menu to create either a
  [full mutation testing report][3], or an analysis for only the branch diffs.

- Look for memory leaks

  A custom build task runs the unit-test program under valgrind and logs the
  results to a [valgrind report][4].

- Use the compiler's code sanitizers

  Two custom build tasks generate reports from the compiler's
  [address sanitizer][5] and [undefined-behavior sanitizer][6].
  Each task builds the project's code with the corresponding sanitizer flag,
  then runs the unit-test program. The instrumented code writes to a log file
  if it finds any issues.


[1]: https://ian-fisher-developer.github.io/vscode-cpp-dev-tools/statistics_coverage_report/coverage_report.html
     "HTML code coverage report for the project unit-test program"
[2]: https://en.wikipedia.org/wiki/Mutation_testing
     "Wikipedia description of mutation testing"
[3]: https://ian-fisher-developer.github.io/vscode-cpp-dev-tools/statistics_test_mull_no-diff-filter.txt
     "Full mutation testing report for the project unit-test program"
[4]: https://ian-fisher-developer.github.io/vscode-cpp-dev-tools/statistics_test_valgrind.txt
     "Valgrind report for the project unit-test program"
[5]: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fsanitize_003daddress
     "GCC address sanitizer description"
[6]: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fsanitize_003dundefined
     "GCC undefined-behavior sanitizer description"
