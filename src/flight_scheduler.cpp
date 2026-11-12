// Portfolio reference implementation based on a 2024 two-person course project.
// This is a new, reviewed implementation of the randomized assignment idea;
// it is NOT the source file originally submitted for the course.
// Contributors to the original course project: Satu Honka and a fellow student from Dongguk University.

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Flight {
    std::size_t id;
    std::string departure;
    std::string arrival;
    int start;
    int end;
};

using Route = std::vector<std::size_t>;  // indices into the flight vector
using Plan = std::vector<Route>;

int parse_minutes(const std::string& time) {
    auto colon = time.find(':');
    if (colon == std::string::npos || colon == 0 || colon + 1 == time.size() ||
        time.find(':', colon + 1) != std::string::npos) {
        throw std::runtime_error("Expected H:MM or HH:MM time: " + time);
    }
    auto all_digits = [](const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) {
            return c >= '0' && c <= '9';
        });
    };
    std::string h = time.substr(0, colon), m = time.substr(colon + 1);
    if (!all_digits(h) || !all_digits(m) || h.size() > 2 || m.size() != 2)
        throw std::runtime_error("Invalid time format: " + time);
    int hour = std::stoi(h), minute = std::stoi(m);
    if (hour > 23 || minute > 59)
        throw std::runtime_error("Time outside a 24-hour clock: " + time);
    return 60 * hour + minute;
}

std::string format_time(int value) {
    std::ostringstream out;
    out << std::setw(2) << std::setfill('0') << value / 60
        << ':' << std::setw(2) << std::setfill('0') << value % 60;
    return out.str();
}

std::vector<Flight> load_flights(const std::string& filename) {
    std::ifstream stream(filename);
    if (!stream) throw std::runtime_error("Cannot open input file: " + filename);
    std::vector<Flight> flights;
    std::string line;
    std::size_t lineno = 0;
    while (std::getline(stream, line)) {
        ++lineno;
        if (line.empty()) continue;
        std::istringstream input(line);
        std::string from, to, start, end, unused_price;
        if (!std::getline(input, from, ',') || !std::getline(input, to, ',') ||
            !std::getline(input, start, ',') || !std::getline(input, end, ',')) {
            throw std::runtime_error("Invalid CSV row at line " + std::to_string(lineno));
        }
        try {
            int depart = parse_minutes(start), arrive = parse_minutes(end);
            if (from.empty() || to.empty() || depart >= arrive)
                throw std::runtime_error("Invalid airport or same-day flight time");
            flights.push_back({flights.size(), from, to, depart, arrive});
        } catch (const std::exception& e) {
            throw std::runtime_error("Input line " + std::to_string(lineno) + ": " + e.what());
        }
        // Optional fifth column (ticket price) is intentionally ignored.
    }
    if (flights.empty()) throw std::runtime_error("No flights in input file");
    return flights;
}

// Generate one randomized feasible plan, considering flights chronologically.
// Random tie-breaking and randomized choice among available aircraft introduce
// variation; this is a heuristic, NOT an optimality proof.
Plan generate_plan(const std::vector<Flight>& flights, std::mt19937& rng) {
    std::vector<std::size_t> order(flights.size());
    std::iota(order.begin(), order.end(), 0);
    std::shuffle(order.begin(), order.end(), rng);
    std::stable_sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) {
        return flights[a].start < flights[b].start;
    });

    Plan plan;
    for (std::size_t idx : order) {
        const Flight& current = flights[idx];
        std::vector<std::size_t> feasible;
        for (std::size_t p = 0; p < plan.size(); ++p) {
            const Flight& prev = flights[plan[p].back()];
            if (prev.arrival == current.departure && prev.end <= current.start)
                feasible.push_back(p);
        }
        if (feasible.empty()) {
            plan.push_back(Route{idx});
        } else {
            std::uniform_int_distribution<std::size_t> pick(0, feasible.size() - 1);
            plan[feasible[pick(rng)]].push_back(idx);
        }
    }
    return plan;
}

bool validate_plan(const std::vector<Flight>& flights, const Plan& plan) {
    std::vector<unsigned> seen(flights.size(), 0);
    for (const Route& route : plan) {
        if (route.empty()) return false;
        for (std::size_t i = 0; i < route.size(); ++i) {
            if (route[i] >= flights.size() || ++seen[route[i]] != 1) return false;
            if (i > 0) {
                const Flight& earlier = flights[route[i - 1]];
                const Flight& later = flights[route[i]];
                if (earlier.arrival != later.departure || earlier.end > later.start)
                    return false;
            }
        }
    }
    return std::all_of(seen.begin(), seen.end(), [](unsigned n) { return n == 1; });
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2 || argc > 4) {
            std::cerr << "Usage: " << argv[0] << " <flight-file> [iterations=500] [seed=42]\n";
            return 2;
        }
        auto flights = load_flights(argv[1]);
        int iterations = argc >= 3 ? std::stoi(argv[2]) : 500;
        if (iterations < 1 || iterations > 100000)
            throw std::runtime_error("Iterations must be between 1 and 100000");
        std::uint32_t seed = argc >= 4 ? static_cast<std::uint32_t>(std::stoul(argv[3])) : 42;
        std::mt19937 rng(seed);
        Plan best;
        for (int i = 0; i < iterations; ++i) {
            Plan candidate = generate_plan(flights, rng);
            if (!validate_plan(flights, candidate))
                throw std::runtime_error("Internal error: invalid assignment generated");
            if (best.empty() || candidate.size() < best.size()) best = std::move(candidate);
        }
        std::cout << "Flights covered: " << flights.size() << "/" << flights.size() << "\n";
        std::cout << "Best aircraft count found (not proven optimal): " << best.size() << "\n";
        std::cout << "Iterations: " << iterations << ", random seed: " << seed << "\n";
        for (std::size_t i = 0; i < best.size(); ++i) {
            std::cout << "Aircraft " << (i + 1) << ": ";
            for (std::size_t idx : best[i]) {
                const Flight& f = flights[idx];
                std::cout << "[" << f.id + 1 << " " << f.departure << " " << format_time(f.start)
                          << " -> " << f.arrival << " " << format_time(f.end) << "] ";
            }
            std::cout << '\n';
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
