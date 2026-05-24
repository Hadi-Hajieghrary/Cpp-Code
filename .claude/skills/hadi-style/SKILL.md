---
name: hadi-style
description: Apply Hadi Hajieghrary's coding and documentation conventions when writing or editing C++, Python, or Markdown/prose in the Cpp-Code and Rule_Book repositories. Covers project layout, naming (PascalCase types, camel/snake callables, trailing-underscore privates), interface/implementation separation, RAII and resource ownership, labeled error messages and console output (***** title ***** delimiters), explicit loops over hidden abstractions, coarse-grained std::thread / ThreadPoolExecutor parallelism, vendored third-party code with license citations, co-located validation/demo functions, and terse-at-call-site / verbose-in-implementation comment style. Use this skill any time the task involves authoring or modifying source files, headers, README/docs, or examples in these repos.
---

# hadi-style — coding & documentation conventions

Use these rules when writing or editing any artifact (C++, Python, Markdown, docstrings) in `Cpp-Code` or `Rule_Book`. The anchors below point to canonical examples already in the tree; consult them before inventing a new pattern.

## 1. Project organization

One self-contained idea per directory. Each project has its own entrypoint, private logic in `src/`, public surface in `include/`, and metadata at the root.

| Layer | C++ (this repo) | Python equivalent | Docs equivalent |
|---|---|---|---|
| Top-level | One dir per project | One package per concern | One dir per topic |
| Logic | `src/*.cpp` | `src/module/*.py` | Section files |
| Interface | `include/*.hpp` | `__init__.py`, `*.pyi`, `Protocol` | Index / API reference |
| Entrypoint | `main.cpp` | `__main__.py` or `if __name__ == "__main__":` | `README.md` at root |
| Build/config | `CMakeLists.txt` | `pyproject.toml` | `mkdocs.yml` / frontmatter |

Rules:
- Entrypoint is a thin orchestration layer — call into logic defined elsewhere. See `neural_network/main.cpp` and `drawing_fractal/main.cpp`.
- Shared utilities are allowed; sprawling shared monoliths are not.

## 2. Naming

| Thing | C++ | Python | Docs |
|---|---|---|---|
| Type / class | `PascalCase` (`MultiLayerPerceptron`, `Bitmap`, `Color`) | `PascalCase` | Title-case headings |
| Function / method | `lowerCamelCase` (`getOutputs`, `setWeight`) | `snake_case` (`get_outputs`) | Lowercase prose verbs |
| Private member | trailing underscore `weights_`, `bias_` | leading underscore `_weights` | not visible |
| File / module | `snake_case` (`perceptron.cpp`) | `snake_case` | `snake-case` / `kebab-case` URLs |
| Constants | `UPPER_SNAKE` via `constexpr` | `UPPER_SNAKE` | bold or code in prose |

Pick one convention per file and hold it. Don't mirror inconsistencies seen elsewhere in the repo — propagate the **right** convention forward when editing.

## 3. Interface vs. implementation

Declare the contract separately from how it's fulfilled.

- **C++**: header declares; `.cpp` defines. Keep `include/` minimal and stable.
- **Python**: type-hinted signatures in the body, expose only the public API via `__init__.py`; use `.pyi` or `typing.Protocol` for larger libraries.
- **Docs**: one-line summary → details. In API docs, signature + one-line docstring is the "header"; the extended description is the "implementation".

## 4. Resource ownership

The creator cleans up. Make ownership explicit.

- **C++**: prefer RAII. Existing examples wrap `new[]` with destructors (`Canvas::~Canvas`, `Bitmap::~Bitmap`); follow that pattern for new types and prefer `std::unique_ptr`/`std::vector` for new code even though older code uses raw `new`/`delete[]`.
- **Python**: use context managers (`with open(...)`). Implement `__enter__`/`__exit__` for custom owners. Prefer dataclasses over raw dicts when lifetime matters.
- **Docs**: link external sources; don't push the search burden onto the reader.

## 5. Error handling

Report errors close to where they occur, with the offending value included.

- **C++**: `throw std::out_of_range(...)` with a formatted message (the repo uses `sprintf` into a buffer). Keep one level of formality per module.
- **Python**: typed exceptions, `raise ValueError(f"expected {expected}, got {actual}")`. Define a module-specific exception class only when callers need to catch it specifically. Never use bare `except:`.
- **Docs**: surface limitations and edge cases in a "Notes" or "Caution" callout, not buried in prose.

## 6. I/O and output

Label every line of output. Match the output's structure to the data's structure.

Canonical pattern in `neural_network/main.cpp:21-46`:
```cpp
void Print(double** matrix, const unsigned int row, const unsigned int col, const char* title = "") {
    std::cout << "***** " << title << " *****" << std::endl;
    // ...
    std::cout << "===== " << title << " =====" << std::endl;
}
```

- **C++ / Python console output**: every demo line gets a stage or key. Use the `***** title *****` / `===== title =====` delimiters when dumping structured data.
- **Python production code**: prefer `logging` over `print`. For demos, `print(f"[{stage}] {label}: {value}")` matches the spirit.
- **Docs**: headings, bold terms, explicit names — don't rely on context to identify what's being described.

## 7. Algorithms and loops

Write algorithms plainly. Resist premature abstraction.

- **C++**: the repo implements Mandelbrot iteration, particle physics, and backprop as raw `for` loops over arrays. No STL algorithms, no Boost. Match that style for new code in this repo.
- **Python**: prefer explicit `for i, val in enumerate(...)` over deep comprehensions or `functools.reduce`. Reach for `numpy`/`scipy` only when numerics actually demand it.
- **Docs**: numbered lists for procedures; prose for explanation. Don't compress multi-step algorithms into a single dense paragraph.

## 8. Parallelism / concurrency

Partition work into independent regions and run them. Keep parallelism visible.

Canonical pattern in `drawing_fractal/main.cpp:151-160`:
```cpp
thread t0(DrawMandelBrot, mandel_brot_fractal, quarters[0]);
thread t1(DrawMandelBrot, mandel_brot_fractal, quarters[1]);
thread t2(DrawMandelBrot, mandel_brot_fractal, quarters[2]);
thread t3(DrawMandelBrot, mandel_brot_fractal, quarters[3]);
```

- Four fixed `std::thread`s, each owning a quarter of the image. No thread pool, no task system.
- **Python**: `concurrent.futures.ThreadPoolExecutor` or `multiprocessing.Pool` for the same coarse partitioning. Keep partition logic separate from worker logic.
- **Docs**: document the unit of work, who owns shared state, and how results are merged.

## 9. Third-party / external code

Vendor it in a clearly named subdirectory. Namespace or prefix it.

- **C++**: see `include/mnist/` with `namespace mnist { }` and a preserved license header (`mnist_reader.hpp`, `mnist_reader_common.hpp`). Eigen plugins live in `include/` with a `_list.h` suffix.
- **Python**: put vendored code in `vendor/` or `third_party/`; re-export only what's used via `from vendor.x import foo`. Keep the LICENSE file adjacent.
- **Docs**: cite source, license, and version. Minimum: "Adapted from <author>'s <project>, <license>."

## 10. Validation / testing

Validation lives next to the code and runs from `main`. Output is human-readable and labeled.

- **C++**: existing demo functions (`Test_Perceptron`, `Test_TrainingMLP`) live in `main.cpp` and print labeled blocks. If a framework is needed, prefer Catch2 (single-header) over gtest — it matches the lightweight style.
- **Python**: `pytest`. `test_perceptron.py` next to `perceptron.py`, or in a parallel `tests/` tree. Mirror the `***** label *****` spirit with descriptive test names.
- **Docs**: every documented API gets a quick-start example. The example is the minimum viable test of the documentation.

## 11. Documentation writing style

Terse at the call site, verbose in the implementation.

- Method names self-document (`randomInitializeWeights`, `saveCurrentFrame`). Don't add comments that restate the name.
- Inline `// TODO -` for known gaps (e.g. `neural_network/main.cpp:330-331`). Don't externalize a TODO to a tracker unless it's beyond the scope of a single fix.
- Benchmark / perf data goes inline as a comment in code; in docs, put it in a table or callout — not buried in a sentence.
- Commented-out code is acceptable as a record of alternatives (`drawing_fractal/main.cpp:134-161, 181-202` preserves both sequential and parallel variants). When you do this, add a one-line comment saying **why** one is preferred.

## One-page checklist

Run through this before considering any new file/function/doc done.

| Question | Pass criterion |
|---|---|
| Isolated? | One directory, one purpose, one entrypoint. |
| Interface separate from implementation? | Header / signature / summary first; details second. |
| Names consistent within the file? | PascalCase types, camel/snake callables, trailing-underscore privates. |
| Ownership explicit? | RAII wrapper / context manager / clear create+destroy pairing. |
| Errors labeled? | Typed exception or labeled message including the problematic value. |
| Outputs labeled? | Every print/log line has a stage or key. |
| Algorithms explicit? | Plain loops; hidden indirection only when perf demands it. |
| External material attributed? | Vendor subdir, namespace, license citation. |
| Validation present? | Demo function or example exercising the main path. |

## When this skill applies

- Writing or editing any `.cpp`, `.hpp`, `.h`, `.py`, `.md`, `README*`, or docstring in `Cpp-Code` or `Rule_Book`.
- Reviewing diffs in those repos — check each item against the one-page checklist.
- Generating new modules, scripts, or examples that will live in those repos.

When the user's instruction conflicts with these conventions, the user wins — but flag the conflict so it's a conscious decision, not drift.
