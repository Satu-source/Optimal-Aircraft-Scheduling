# Input data and redistribution

## Record format

Each non-empty line is a flight record with four required comma-separated fields and an optional ignored fifth field:

```text
departure_airport,arrival_airport,departure_time,arrival_time,optional_ticket_price
```

For example, the independently authored demonstration dataset begins with:

```text
HEL,ARN,8:00,9:00,99
ARN,HEL,10:00,11:00,80
```

Time values can be `H:MM` or `HH:MM`; this reference implementation assumes all flights are on one day and rejects flights that end at or before their departure. Airport codes are treated as literal strings. The optional ticket-price column is ignored, as specified in the original task.

## Included demonstration data

`examples/sample_flights.csv` contains **eight newly created illustrative flights**, with routes and times constructed solely to demonstrate airport continuity, chronological chaining and multiple aircraft.

## Original course dataset

The source project used an externally sourced 120-flight dataset provided for the Dongguk University course; the course assignment describes its origin as Kaggle. The source file supplied for this portfolio preparation was `flights.txt` and contained 120 flight rows. **It is intentionally omitted from this public-ready repository**, because the relevant dataset's redistribution license was not established.

To test locally with a copy you are authorized to use, place it outside the Git repository and pass its file path to the executable:

```bash
./build/flight_scheduler /path/to/your/flights.txt 500 42
```

The public repository does not include the instructor's original assignment handout or another team member's source code. When adding any material later, verify authorship and redistribution permissions.
