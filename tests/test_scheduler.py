"""Black-box regression checks for the modern reference solver."""
import os
import pathlib
import subprocess
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[1]
BINARY = pathlib.Path(tempfile.gettempdir()) / "flight_scheduler_demo_test"


class ScheduleTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = os.environ.get("CXX", "g++")
        subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-pedantic",
                        str(ROOT / "src/flight_scheduler.cpp"), "-o", str(BINARY)],
                       check=True, capture_output=True, text=True)

    def run_on(self, text, iterations=100, seed=42):
        with tempfile.TemporaryDirectory() as d:
            path = pathlib.Path(d) / "flights.csv"
            path.write_text(text, encoding="utf-8")
            return subprocess.run([str(BINARY), str(path), str(iterations), str(seed)],
                                  capture_output=True, text=True)

    def test_demo_is_complete_and_valid(self):
        text = (ROOT / "examples/sample_flights.csv").read_text()
        p = self.run_on(text)
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn("Flights covered: 8/8", p.stdout)
        self.assertIn("Best aircraft count found (not proven optimal): 2", p.stdout)

    def test_cannot_teleport_aircraft(self):
        p = self.run_on("HEL,ARN,06:00,07:00,1\nOSL,HEL,08:00,09:00,1\n")
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn("Best aircraft count found (not proven optimal): 2", p.stdout)

    def test_single_digit_hour_is_parsed_correctly(self):
        p = self.run_on("HEL,ARN,6:19,8:13,1\nARN,HEL,08:13,10:00,1\n")
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn("Best aircraft count found (not proven optimal): 1", p.stdout)
        self.assertIn("06:19", p.stdout)

    def test_overlap_requires_more_aircraft(self):
        p = self.run_on("HEL,ARN,06:00,09:00,1\nHEL,ARN,07:00,08:00,1\n")
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertIn("Best aircraft count found (not proven optimal): 2", p.stdout)

    def test_bad_input_and_empty_input_fail(self):
        for text in ("", "HEL,ARN,25:00,26:00,1\n", "HEL,ARN,08:00,07:00,1\n"):
            with self.subTest(text=text):
                self.assertNotEqual(self.run_on(text).returncode, 0)

    def test_repeatable_given_same_seed(self):
        example = (ROOT / "examples/sample_flights.csv").read_text()
        a = self.run_on(example, seed=2024)
        b = self.run_on(example, seed=2024)
        self.assertEqual(a.stdout, b.stdout)


if __name__ == "__main__":
    unittest.main()
