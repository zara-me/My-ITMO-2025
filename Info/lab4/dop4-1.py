import re
import xml.etree.ElementTree as ET
from dop1 import yaml_input
from dop1 import yaml_to_xml
import time
# Task 4: Benchmarking execution time

def benchmark_conversion(func, yaml_data: str, iterations: int = 100) -> float:
    """Benchmark the YAML to XML conversion."""
    start_time = time.time()
    for _ in range(iterations):
        func(yaml_data)
    return time.time() - start_time

# Run Task 4
execution_time = benchmark_conversion(yaml_to_xml, yaml_input)
print("\n--- Task 4: Benchmarking ---")
print(f"Execution time for 100 iterations: {execution_time:.2f} seconds")