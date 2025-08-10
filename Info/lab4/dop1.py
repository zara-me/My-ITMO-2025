import yaml
import xml.etree.ElementTree as ET
import csv
import time
from typing import Any, Dict

# Updated YAML input data
yaml_input = """
schedule:
  day:
    - name: "Wednesday"
      subject:
        - name: "Informatics"
          type: "Lecture"
          time: "08:20-09:50"
          instructor: "Balakshin Pavel Valeryevich"
          room: "1216"
          address: "Lomonosova Street, Building 9"
        - name: "Fundamentals of Professional Activity"
          type: "Lecture"
          time: "10:00-11:30"
          instructor: "Klimenkov Sergey Viktorovich"
          room: "1216"
          address: "Lomonosova Street, Building 9"
    - name: "Saturday"
      subject:
        - name: "Linear Algebra"
          type: "Lab Work"
          time: "08:20 - 09:50"
          instructor: "Isaeva Tatyana Timofeevna"
          room: "2113"
          address: "Kronversky Prospect 49"
        - name: "Fundamentals of Professional Activity"
          type: "Lab Work"
          time: "09:50 - 11:30"
          instructor: "Oblyashevsky Sevastian Alexandrovich"
          room: "2308"
          address: "Kronversky Prospect 49"
"""

# YAML to XML Conversion using a Library


def yaml_to_xml(yaml_data: str) -> str:
    """Convert YAML to XML."""
    data = yaml.safe_load(yaml_data)
    root = ET.Element("schedule")

    for day_data in data.get("schedule", {}).get("day", []):
        day_elem = ET.SubElement(root, "day", name=day_data["name"])
        for subject in day_data.get("subject", []):
            subject_elem = ET.SubElement(day_elem, "subject")
            for key, value in subject.items():
                key_elem = ET.SubElement(subject_elem, key)
                key_elem.text = str(value)

    return ET.tostring(root, encoding="unicode")


xml_result = yaml_to_xml(yaml_input)
print("--- Task 1: YAML to XML ---")
print(xml_result)
