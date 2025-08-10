import re
from dop1 import yaml_input
import xml.etree.ElementTree as ET

# Task 2: Adding regex for parsing YAML to XML


def yaml_to_xml_with_regex(yaml_data: str) -> str:
    """Convert YAML to XML using regex (simplified)."""

    yaml_lines = yaml_data.strip().split("\n")
    root = ET.Element("schedule")
    current_day = None

    day_pattern = re.compile(r"^\s*-\sname:\s\"(.*?)\"$")
    subject_pattern = re.compile(r"^\s*-\sname:\s\"(.*?)\"$")
    key_value_pattern = re.compile(r"^\s*(\w+):\s\"(.*?)\"$")

    for line in yaml_lines:
        day_match = day_pattern.match(line)
        if day_match:
            current_day = ET.SubElement(root, "day", name=day_match.group(1))
            continue

        if current_day:
            subject_match = subject_pattern.match(line)
            if subject_match:
                subject_elem = ET.SubElement(current_day, "subject")
                continue

            key_value_match = key_value_pattern.match(line)
            if key_value_match and subject_elem is not None:
                key_elem = ET.SubElement(
                    subject_elem, key_value_match.group(1))
                key_elem.text = key_value_match.group(2)

    return ET.tostring(root, encoding="unicode")


# Run Task 2
xml_result_regex = yaml_to_xml_with_regex(yaml_input)

print("\n--- Task 2: YAML to XML with Regex ---")
print(xml_result_regex)
