# Aircraft Fleet Scheduling — Randomized Assignment in C++

**Computer Algorithm Team Project · Dongguk University · Fall 2024**  
**Original project team:** Satu Honka and a fellow student from Dongguk University

A C++ project exploring how to assign scheduled flights to aircraft while **covering every flight, avoiding overlapping assignments, respecting airport continuity, and reducing the number of aircraft needed**.

The original team project compared algorithm ideas, including backtracking and greedy assignment, before adopting the randomized-plan approach described in the course brief. This repository presents the project's **historical initial prototype** alongside a **new, tested reference implementation** of the randomized scheduling approach.

> **Important:** This is a heuristic fleet-assignment demonstration. The smallest aircraft count found by repeated random construction is **not necessarily a mathematically proven optimum**. The published reference implementation is newly written for portfolio presentation and is *not* claimed to be the exact original team submission.

## Overview

| | |
| --- | --- |
| **Domain** | Aircraft/flight scheduling and combinatorial optimization |
| **Language** | C++17 |
| **Approach** | Repeated randomized feasible assignment; retain best fleet size |
| **Input** | Flight departure/arrival airports, departure/arrival times; unused ticket-price field |
| **Constraints** | All flights covered exactly once; no overlapping flights on one aircraft; next flight departs from the preceding arrival airport |
| **Outputs** | Number of aircraft found and a chronological route for each aircraft |
| **Verification** | CMake build and automated black-box tests |

## Problem and algorithm

Each flight is represented by two airports and two time values. Aircraft can serve more than one flight when the preceding flight lands **at the airport from which the next departs**, and lands **no later than** the next departure.

1. Read and validate the flight list, including times expressed as `H:MM` or `HH:MM`.
2. Order flights chronologically, randomizing tie order between flights departing at the same time.
3. For each flight, find all aircraft whose last arrival airport and time permit the next assignment.
4. Randomly select one feasible aircraft, or create an additional aircraft if none is available.
5. Construct multiple complete candidate plans, validate each plan, and retain the one requiring the fewest aircraft.

This is a **randomized constructive heuristic**, inspired by the course's random-plan comparison specification. It is not a graph-based random walk or an exact optimization solver.

## Quick start

A C++17 compiler and CMake are required. No third-party C++ libraries are needed.

```bash
cmake -S . -B build
cmake --build build
./build/flight_scheduler examples/sample_flights.csv 500 42
```

On Windows with Visual Studio/CMake, the executable may be under `build/Debug/` or `build/Release/` instead.

Arguments: `flight_scheduler <file> [iterations=500] [seed=42]`. Specify the random seed to repeat a run deterministically.

The **self-created sample dataset** contains eight flights. Running with 500 iterations and seed 42 produces:

```text
Flights covered: 8/8
Best aircraft count found (not proven optimal): 2
Iterations: 500, random seed: 42
```

## Historical course exercise

The originally provided 120-flight dataset was used to check the new reference implementation locally, but is **not included** in this public-ready package because its redistribution terms have not been verified. On that dataset, the current reference implementation found a valid **30-aircraft plan covering 120/120 flights** with 500 iterations and seed 42. This is a result from the *new reference implementation*, not a reproduction of the original team submission or proof of optimality.

## Repository structure

```text
src/flight_scheduler.cpp           # New, runnable and tested C++17 reference implementation
archive/satu_initial_greedy.cpp   # Satu's original early-stage C++ prototype (historical)
examples/sample_flights.csv        # Independently created demonstration input
tests/test_scheduler.py            # Six regression tests (requires Python 3 and g++)
docs/DEVELOPMENT.md                # Course work, development path and attribution
docs/IMPLEMENTATION.md             # Algorithm details, validation and known limitations
docs/DATA.md                       # Dataset fields, provenance and rights
CMakeLists.txt                     # C++17 build setup
.github/workflows/build.yml        # Optional GitHub Actions build/test workflow
```

## Test the project

```bash
python3 -m unittest discover -s tests -v
```

The tests cover complete assignment, airport continuity, overlapping flights, single-digit hours, invalid input, and reproducible output with a fixed seed. They compile the reference C++ source with `g++`. All six tests passed locally when this portfolio package was prepared.

## Project authorship and provenance

This project originated as a **two-person course assignment** by **Satu Honka** (HAMK exchange student) and **a fellow student from Dongguk University** in Fall 2024. The historical source under `archive/` is based on Satu's supplied `flight_schedule.cpp`. The course notes and report also document subsequent joint algorithm development; those drafts are summarized rather than republished as someone else's original source code. See [development and credits](docs/DEVELOPMENT.md).

The `src/` implementation is a **new portfolio reference version**, explicitly separated from the historical course submissions. It was implemented from the supplied task constraints and development notes and tested with both the self-created sample and the course dataset. Individual contributions to the original final team submission have not been inferred where the source materials do not establish them.

**Copyright and sharing:** The full course assignment, original joint submission, and external flight dataset are not distributed in this package. No open-source license has been assigned; publication/licensing of jointly authored material should be agreed with the other team member.