---
name: hadi-style
description: Apply Hadi Hajieghrary's coding and academic-writing conventions when editing C++, Python, Markdown/prose, LaTeX, or BibTeX in the Cpp-Code and Rule_Book repositories — including IEEE conference papers (ICRA/IROS), the geometric-control PhD thesis, and related research notes. Code side covers project layout, naming (PascalCase types, camel/snake callables, trailing-underscore privates), interface/implementation separation, RAII and resource ownership, labeled error messages and console output (***** title ***** delimiters), explicit loops, coarse-grained std::thread / ThreadPoolExecutor parallelism, vendored third-party code with license citations, and co-located validation. Academic side covers IEEE section template (I. INTRODUCTION → … → V. CONCLUSIONS with A./B./C. subsections), numbered equations and ≜/:= definitions, Theorem N.N (Name) blocks, first-use abbreviation policy, "We …" first-person-plural voice, Fig. N: full-sentence captions with subfigure labels, the end-of-intro contribution paragraph, "In this paper we addressed…" conclusion template, IEEE numbered references, and a quirks-to-fix list (Riemannian spelling, doubled articles, translation-y phrasings, closed-loop hyphenation). Use any time the task involves authoring or modifying source files, headers, README/docs, LaTeX, .bib, or paper/thesis prose in these repos.
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

## 12. Academic writing (papers and thesis)

Sections 1–11 cover code, README, and inline-comment writing. Academic writing — IEEE conference papers (ICRA, IROS, …), the geometric-control PhD thesis, and longer research notes in `Rule_Book` — follows a tighter template derived from the patterns visible across the published work.

### 12.1 Section structure

**Conference paper** (IROS, ICRA, etc.):
```
I.   INTRODUCTION           — motivation + literature woven in; ends with contribution paragraph
II.  <Background / Preliminaries>
III. <Method / Approach>
       A. <Sub-problem 1>
       B. <Sub-problem 2>
       C. Controller Synthesis (or analogous)
IV.  RESULTS
       A. Simulation Results
       B. Experimental Results
V.   CONCLUSIONS (and Future Work)
References [1]…[N]
```
- Roman numerals for top sections, capital letters for subsections.
- Related work is **woven into the introduction**, not its own section.

**Thesis / monograph**:
```
Chapter 1: Introduction
    1.1 Literature Review
    1.2 Contributions of This Research
    1.3 Organization of this Document
Chapter 2: Mathematical Preliminaries
    2.1 …
Chapter N: Concluding Remarks
    N.1 Suggested Future Work
Bibliography
```
- Decimal numbering for sections (`1.1`, `1.2`, …).
- Always keep §1.3 "Organization of this Document" — a roadmap paragraph mapping every later chapter.

### 12.2 Voice and tense

- First-person **plural** for the body: *"We present…"*, *"We consider…"*, *"We are interested in…"*, *"We address…"*. Even single-author papers use "we".
- First-person **singular** only in Acknowledgments and "Suggested Future Work" of the thesis: *"I would like to suggest…"*.
- Chapter introductions take **future tense**: *"In Chapter 3 we will examine…"*.
- Passive for results (*"It can be shown that…"*) mixed with active for design choices (*"We define the tracking errors as `e_x(t) ≜ x(t) − x_r(t)`"*).

### 12.3 Math and equation conventions

- Numbered, displayed equations using `(N)` right-aligned. Reference in-text as `(8)`, `from (12)`, `given by (3.2)` (the thesis uses chapter-prefixed numbers).
- Definitions: `≜` or `:=` (`q_i ≜ [x_i, y_i, θ_i]^T`); reserve `=` for equalities.
- Vectors and matrices italic with explicit `^T` for transpose; subscripts for indices (`q_i`, `θ_i`); hats / tildes / bars for estimates and transforms (`μ̃_{x̃_i}`, `Ṽ_{still_i}`).
- **Theorem / Lemma / Proposition** as labeled blocks with a parenthetical name when classical: `Theorem 2.4.1 (Frobenius Theorem). A regular distribution is integrable if and only if it is involutive. [17]`. Cite the source in brackets when restating a known result.
- One equation per concept. Display dense derivations; don't bury them inline.

### 12.4 Figures and captions

- Caption prefix: `Fig. N:` (paper) or `Figure C.N:` (thesis).
- Captions are **complete sentences**, usually two: one descriptive, one interpretive. Example: *"Fig. 3: Simulation results for the proposed kinematic closed-loop control of two boats hauling a floated load. After a transient state both boats turn to move parallel to the load."*
- Subfigures: `(a)`, `(b)`, `(c)`, `(d)` with short sub-captions like `(a) Trajectory`, `(b) Bearings (attitude)`.
- Reference in-text as `Fig. N` or `see Fig. 1`.

### 12.5 Abbreviations

Define on first use, abbreviation thereafter — without exception:
- *autonomous surface vehicles (ASVs)*
- *Markov Decision Process (MDP)*
- *Multi-Robot Coherent Structure Testbed (mCoSTe)*
- *Regional Ocean Model System (ROMS)*
- *Robot Operating System (ROS)*

If the abbreviation never gets reused, drop it and spell out each occurrence.

### 12.6 End-of-introduction contribution paragraph

Every paper's introduction ends with an explicit "what we do differently" paragraph. Standard openers:
- *"Different from existing approaches, we …"*
- *"In this work, we build upon our existing work [N] and present …"*
- *"Different from our previous work, we explicitly …"*

Pair with a one-line preview of the result: *"Our results suggest that the proposed strategy is robust in the presence of disturbances and model uncertainties."*

### 12.7 Conclusion template

Closing section is short and predictable:
1. Opener sentence: *"In this paper we addressed the cooperative transport problem for a team of ASVs towing a buoyant load."*
2. Recap of the contribution in 3–5 sentences.
3. Validation result in one sentence.
4. **Future Work** — bullet list or short paragraphs of next directions.

### 12.8 References

IEEE numbered style. In-text `[N]` or `Author et al. in [N]`. Entry format:
```
[N] A. Author, B. Author, and C. Author, "Title in title case," Venue Abbrev., vol. X, no. Y, pp. A-B, Year.
```
Conference papers use the conference name; journals use the abbreviated journal title with vol/no/pp.

## 13. Linguistic quirks to fix on sight

These recur across the existing work — correct them when editing, don't propagate them:

| Quirk | Correction |
|---|---|
| `Reimannian` | **Riemannian** (proper noun, after Bernhard Riemann) |
| Doubled articles: `the the controller`, `the the load` | Drop the duplicate |
| `investigates the fundamental concepts` (in past-tense context) | `investigated` |
| Missing articles / wrong number: `for a class of constrained system` | `for a class of constrained systems` |
| Stacked translation-y phrasings: *"To embolden the significance of the solution this research proposes"* | Rewrite for directness: *"To demonstrate the value of the proposed method, we …"* |
| `the agents are subjected to various holonomic and nonholonomic constraints` | `the agents are subject to holonomic and non-holonomic constraints` |
| `nonholonomic` vs `non-holonomic` (both appear) | Standardize on **non-holonomic** |
| Echoed phrasing: `arise from arise from` | Remove the repetition |
| `close loop` | `closed-loop` (hyphenated, adjectival) |
| Tense slipping between past/present in the same paragraph | One tense per paragraph — present for methodology, past for completed experiments |

If the user explicitly asks to preserve their voice verbatim, do so — but flag the quirk so it's a conscious choice, not drift.

## One-page checklist

Run through this before considering any new file/function/section done.

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
| **Academic prose?** | IEEE template, "we" voice, abbreviations defined on first use, `Fig. N:` full-sentence captions, equations numbered with `(N)`, contribution paragraph at end of intro, conclusion + future work, no quirks from §13. |

## When this skill applies

- Writing or editing `.cpp`, `.hpp`, `.h`, `.py`, `.md`, `.tex`, `.bib`, `README*`, or docstrings in `Cpp-Code` or `Rule_Book`.
- Drafting or revising IEEE-style conference papers, dissertation chapters, and longer-form research notes that live in (or will move to) those repos.
- Reviewing diffs in those repos — run each item against the one-page checklist.
- Generating new modules, scripts, paper sections, or examples for those repos.

When the user's instruction conflicts with these conventions, the user wins — but flag the conflict so it's a conscious decision, not drift.
