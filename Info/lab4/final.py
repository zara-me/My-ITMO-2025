def yaml_to_xml(yaml_content):
    def process_line(line, indent_level):
        stripped_line = line.strip()
        if ": " in stripped_line:  # Ключ-значение
            key, value = stripped_line.split(": ", 1)
            return f"{'  ' * indent_level}<{key}>{value}</{key}>\n"
        elif ":" in stripped_line:  # Ключ (начало вложенной структуры)
            key = stripped_line[:-1]
            return f"{'  ' * indent_level}<{key}>\n"
        else:
            return ""

    lines = yaml_content.splitlines()
    xml_output = "<root>\n"
    indent_level = 1
    stack = []

    for line in lines:
        if not line.strip():  # Пропуск пустых строк
            continue

        current_indent = len(line) - len(line.lstrip())

        # Закрытие тегов при уменьшении вложенности
        while stack and stack[-1][1] >= current_indent:
            last_key = stack.pop()[0]
            xml_output += f"{'  ' * indent_level}</{last_key}>\n"
            indent_level -= 1

        # Обработка строки
        if ": " in line.strip():
            key, _ = line.strip().split(": ", 1)
            xml_output += process_line(line, indent_level)
        elif ":" in line.strip():
            key = line.strip()[:-1]
            xml_output += process_line(line, indent_level)
            stack.append((key, current_indent))
            indent_level += 1

    # Закрытие оставшихся открытых тегов
    while stack:
        last_key = stack.pop()[0]
        xml_output += f"{'  ' * indent_level}</{last_key}>\n"
        indent_level -= 1

    xml_output += "</root>"
    return xml_output


# Чтение данных из файла YAML
yaml_file_path = "C:/Users/just DO it !/Desktop/lesson/inf/lab4/schedule.yaml"
with open(yaml_file_path, "r") as yaml_file:
    yaml_content = yaml_file.read()

    # Конвертация в XML
xml_result = yaml_to_xml(yaml_content)

# Сохранение результата в файл XML
xml_file_path = "result.xml"
with open(xml_file_path, "w") as xml_file:
    xml_file.write(xml_result)
    #print(xml_result)
