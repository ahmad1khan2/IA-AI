# IA-AI

A console loan-application chatbot in C++. It holds a conversation with an
applicant, works out which of four loan products they need, collects and
validates what that product requires, records the application, and can fall back
to open-ended small talk when the applicant says something off-script.

SE1001 project, FAST NUCES Lahore.

---

## What it does

**Four loan products**, each with its own inputs, rules, and dialogue path:

| Product  | Handled by                        |
| -------- | --------------------------------- |
| Home     | `HomeState` / `home.cpp`          |
| Car      | `CarState` / `Car.cpp`            |
| Scooter  | `ScooterState` / `Scooter.cpp`    |
| Personal | `PersonalState` / `Personal.cpp`  |

Each product owns its own logic and its own data file (`Home.txt`, `Car.txt`,
`Scooter.txt`, `personal.txt`), so changing the terms on one loan type cannot
disturb the other three.

**CNIC verification.** Applicant identity is collected and validated through a
dedicated state (`CNICState`, `cnic.cpp`) before an application is accepted, so
a malformed identity number never reaches the application record.

**Persisted applications.** Completed applications are written to
`applications.txt` through a shared file-handling layer, so a session's output
survives the session.

**General conversation.** When input doesn't belong to any loan flow,
`GeneralChatHandler` takes over: it tokenizes the input and scores it against a
two-speaker dialogue corpus (`human_chat_corpus.txt`, `Utterances.txt`) using
**IoU — intersection over union over token sets** — returning the corpus line
with the highest overlap. The bot therefore degrades into plausible small talk
instead of dead-ending on unrecognized input.

---

## How it is built

The conversation is a **state machine**, not a script. `ChatState` defines the
interface every conversational state implements; `StateManager` owns the
current state and the transitions between them; `MainState` is the entry point
that routes an applicant toward a product, a general chat, or identity
collection.

```
StateManager
  └── ChatState (interface)
        ├── MainState            entry point and routing
        ├── GeneralChatState     open-ended chat, IoU corpus matching
        ├── CNICState            identity collection and validation
        ├── HomeState
        ├── CarState
        ├── ScooterState
        └── PersonalState
```

Because states are self-contained and registered with the manager, the number of
distinct routes through a conversation is a product of the transitions rather
than a fixed set of paths, and adding a fifth loan product means adding a state
— not editing the four that exist.

Supporting modules: `applicant` (the applicant model), `loan.h` (shared loan
definitions), `fileHandling` (all reads and writes), `LenderInterface` and
`interface` (presentation).

---

## Repository layout

```
IA-AI/
├── Chat-Bot/
│   ├── Chat-Bot.sln              Visual Studio solution
│   ├── Chat-Bot.vcxproj
│   ├── StateManager.{h,cpp}      state machine core
│   ├── ChatState.h               state interface
│   ├── MainState.{h,cpp}
│   ├── GeneralChat{,State}.{h,cpp}
│   ├── CNICState.{h,cpp}  cnic.{h,cpp}
│   ├── Home{,State}.{h,cpp}  Car{,State}.{h,cpp}
│   ├── Scooter{,State}.{h,cpp}  Personal{,State}.{h,cpp}
│   ├── applicant.{h,cpp}  loan.h
│   ├── fileHandling.{h,cpp}
│   ├── LenderInterface.cpp  interface.cpp
│   ├── data/
│   └── *.txt                     loan data, corpus, applications
└── ISE Project - Increment 1.docx
```

---

## Build and run

The project ships as a Visual Studio solution:

```
Open Chat-Bot/Chat-Bot.sln in Visual Studio, then Build → Build Solution (Ctrl+Shift+B)
and Debug → Start Without Debugging (Ctrl+F5).
```

Run from the `Chat-Bot/` directory so the `.txt` data files resolve.

---

## Contributors

- [@ahmad1khan2](https://github.com/ahmad1khan2)
- [@adinasaqib](https://github.com/adinasaqib)
- [@alizaahmadd](https://github.com/alizaahmadd)
- [@mustafaSal95](https://github.com/mustafaSal95)
