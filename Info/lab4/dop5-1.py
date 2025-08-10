
import re
import xml.etree.ElementTree as ET
from dop1 import xml_result
import time
import csv
# Task 5: Convert YAML to CSV
def xml_to_csv(xml_data: str, csv_file: str) -> None:
    """Convert XML to CSV."""
    root = ET.fromstring(xml_data)
    with open(csv_file, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["Day", "Subject Name", "Type", "Time", "Instructor", "Room", "Address"])
        for day_elem in root:
            day = day_elem.get("name")
            for subject_elem in day_elem:
                name = subject_elem.find("name").text
                type_ = subject_elem.find("type").text
                time = subject_elem.find("time").text
                instructor = subject_elem.find("instructor").text
                room = subject_elem.find("room").text
                address = subject_elem.find("address").text
                writer.writerow([day, name, type_, time, instructor, room, address])

csv_file_path = "output.csv"
xml_to_csv(xml_result, csv_file_path)
print("\n--- Task 5: YAML to CSV ---")
print(f"CSV File Created at {csv_file_path}")