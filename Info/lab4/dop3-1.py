import re
import xml.etree.ElementTree as ET
#from lab2 import file #from folder import file_name
#from folder.file import name
from dop1 import yaml_input
from pyparsing import Word, alphas, nums, Group, OneOrMore, Suppress, Literal, Dict, QuotedString

# Task 3: Using formal grammars for parsing YAML to XML
def yaml_to_xml_with_grammar(yaml_data: str) -> str:
    """Convert YAML to XML using a formal grammar approach."""

    # Grammar definitions
    colon = Suppress(":")
    dash = Suppress("-")
    string = QuotedString('"')
    key_value = Group(Word(alphas) + colon + string)
    subject = Group(dash + Dict(OneOrMore(key_value)))
    day = Group(dash + Suppress("name:") + string("name") + Suppress("subject:") + OneOrMore(subject))
    schedule = Group(Literal("schedule:") + Suppress("day:") + OneOrMore(day))

    # Parse the YAML data
    parsed_data = schedule.parseString(yaml_data)
    root = ET.Element("schedule")

    for day in parsed_data[1:]:
        day_elem = ET.SubElement(root, "day", name=day["name"])
        for subject in day["subject"]:
            subject_elem = ET.SubElement(day_elem, "subject")
            for key, value in subject.items():
                key_elem = ET.SubElement(subject_elem, key)
                key_elem.text = value

    return ET.tostring(root, encoding="unicode")

# Run Task 3
xml_result_grammar = yaml_to_xml_with_grammar(yaml_input)
print("\n--- Task 3: YAML to XML with Formal Grammar ---")
print(xml_result_grammar)