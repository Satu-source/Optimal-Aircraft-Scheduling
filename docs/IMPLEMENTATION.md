# Reference implementation — algorithm and checks

## What is implemented

`src/flight_scheduler.cpp` is a portable C++17 program that searches over multiple valid aircraft-assignment plans. Flight records are loaded from a comma-separated text file and validated before constructing plans.

**Flight model:** `Flight {id, departure, arrival, start, end}` where times are integer minutes after midnight. Unlike string comparison, integer minute comparisons correctly handle both `6:19` and `16:19`.

**Aircraft route:** A vector of indices pointing to flights, in chronological order. An additional flight can follow the last flight of a route only if:

```text
previous.arrivalAirport == next.departureAirport
previous.arrivalTime    <= next.departureTime
```

All flights are treated as taking place on the **same calendar day**. Overnight flights, airport repositioning, minimum turnaround times, and timezone conversions are out of scope.

## Search strategy

For each iteration, the implementation constructs a plan from scratch. It shuffles flight indices to vary the ordering of simultaneous departures, sorts by departure time, and randomly chooses among feasible existing aircraft. If none can operate the flight, a new aircraft is introduced. It then keeps the candidate with the fewest aircraft.

The method always returns a **feasible** plan if the input is valid. It does not guarantee a globally minimal fleet or sample all possible permutations. Its mathematical description is **randomized greedy construction with best-of-N selection**, not a proof-producing optimizer. This distinction matters because the course referred to its random-plan search as a *random walk*.

## Validation and reproducibility

Every candidate plan is checked for all of the following:

- Each input flight is assigned **exactly once**.
- Flights on the same aircraft do not overlap.
- An aircraft's next departure airport matches its previous arrival airport.
- Routes follow chronological order.

A fixed seed allows the same assignment to be reproduced for the same input and iteration count. The code returns errors for malformed times, missing files, invalid same-day intervals and empty datasets.

## Tests and local checks

The six automated tests use a small, independently authored eight-flight example and targeted edge cases. The tests passed in the preparation environment using `g++ -std=c++17`. The original 120-flight course dataset was also tested locally with 500 iterations and seed 42: **120/120 flights assigned, 30 aircraft found**. This observation is not a claim of optimality and is not an original 2024 run.

## Development limitations / future work

- Exact fleet-size minimum would require a suitable optimality-verifying formulation and solver or a proof of a matching lower bound.
- Random selection can produce variable results, especially when the seed or iteration count changes.
- No aircraft capacities, maintenance, ground turnaround, repositioning, weather or multi-day scheduling are modeled.
- Compare with deterministic baselines and run repeated seeds/iteration budgets to characterize performance.
