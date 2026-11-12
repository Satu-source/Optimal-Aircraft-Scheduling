# Project development and attribution

## Origin

The original work was produced for the **Computer Algorithm** course at **Dongguk University (Fall 2024)**. The two-person team named in the project report was:

- **Satu Honka**, exchange student at Dongguk University (HAMK)
- **A fellow student from Dongguk University** (name withheld)

The task was to design an aircraft-flight assignment plan minimizing the fleet size while assigning every scheduled flight, respecting time and airport-connection constraints.

## What the submitted materials show

1. **Initial problem exploration:** The first report discussed backtracking and a greedy strategy, including the possible performance trade-off of greedy decisions.
2. **Early C++ prototype:** The supplied `flight_schedule.cpp`, documented among Satu's early code versions, parses flights and assigns them to aircraft using a first-fit greedy process. It is preserved unchanged in `archive/satu_initial_greedy.cpp`.
3. **Method revised for course requirements:** The second report explicitly notes that the team revisited the assignment specification and adapted its approach to repeated random aircraft-flight plans, comparing the number of aircraft used.
4. **Later code in project documents:** The file `modified_code.docx` includes C++ code using a randomized allocation process, iterations and the best observed plan. It also contains a machine-specific file path. It is **not republished in this package** because it belongs to the original collaborative context and the author's permission to republish that specific version has not been confirmed.
5. **Portfolio reference:** `src/flight_scheduler.cpp` is an independent, newly written C++17 implementation of the key scheduling constraints and repeated randomized construction. It is labeled and tested separately so readers do not confuse it with a historical submission.

## Original source limitations

The archived initial C++ version does **not** check whether an aircraft's previous arrival airport matches its next flight's departure airport. It compares time strings lexically, which is unreliable for non-zero-padded hours, and its printed "optimal" aircraft count is not justified. The archived file is included as development history, **not a recommended executable**.

## Public attribution

The project should be described as a **team project**, not an individual project. A precise line-by-line breakdown of every teammate's contributions is not established by the provided materials; this repository deliberately does not invent one.

No direct copy of the Korean student's draft code or the original professor's assignment handout is included. Confirm the team's consent before publishing any jointly owned final source or granting it a license.
