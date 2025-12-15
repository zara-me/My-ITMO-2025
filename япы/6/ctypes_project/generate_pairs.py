import random

def generate_pairs(path="pairs.txt", count=2000, value_range=(-10000, 10000)):
    with open(path, "w") as f:
        for _ in range(count):
            x1 = random.randint(*value_range)
            y1 = random.randint(*value_range)
            x2 = random.randint(*value_range)
            y2 = random.randint(*value_range)

            f.write(f"{x1},{y1} {x2},{y2}\n")
    print(f"Generated {count} pairs into {path}")

if __name__ == "__main__":
    generate_pairs()








# захра дарабзадех