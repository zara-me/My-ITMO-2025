import ctypes
import os
import sys
from typing import List, Tuple

class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int), ("y", ctypes.c_int)]

class PointPair(ctypes.Structure):
    _fields_ = [("a", Point), ("b", Point)]

def load_lib():
    lib_name = "point_lib.dll" if os.name == "nt" else "point_lib.so"
    lib_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), lib_name)
    if not os.path.exists(lib_path):
        raise FileNotFoundError(f"Shared library not found: {lib_path}")
    return ctypes.CDLL(lib_path)

def parse_pairs_from_file(path: str) -> List[Tuple[Tuple[int,int], Tuple[int,int]]]:
    pairs = []
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            # "x1,y1 x2,y2"
            try:
                left, right = line.split()
                x1, y1 = left.split(",")
                x2, y2 = right.split(",")
                pairs.append(((int(x1), int(y1)), (int(x2), int(y2))))
            except Exception as e:
            # If it was another format, you can log it here.
                print(f"Skipping malformed line: {line} ({e})", file=sys.stderr)
    return pairs

def main(input_file="pairs.txt", out_csv="distances.csv", print_first=10):
    pairs = parse_pairs_from_file(input_file)
    n = len(pairs)
    print(f"Parsed {n} pairs from {input_file}")

    ## Создать массив ctypes из PointPair 
    PairArrayType = PointPair * n
    pair_array = PairArrayType()

    for i, ((x1,y1),(x2,y2)) in enumerate(pairs):
        pair_array[i].a.x = x1
        pair_array[i].a.y = y1
        pair_array[i].b.x = x2
        pair_array[i].b.y = y2

    #load librery
    c_lib = load_lib()

    # prototype  C
    c_lib.compute_distances.argtypes = [ctypes.POINTER(PointPair), ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
    c_lib.compute_distances.restype = None

    OutArrayType = ctypes.c_double * n
    out_array = OutArrayType()

    # calling
    c_lib.compute_distances(pair_array, n, out_array)

    # Print several samples and save to csv
    print(f"First {min(print_first, n)} distances:")
    for i in range(min(print_first, n)):
        print(f"{i}: {out_array[i]}")

    # save to CSV
    with open(out_csv, "w") as f:
        f.write("index,distance,x1,y1,x2,y2\n")
        for i in range(n):
            p = pairs[i]
            f.write(f"{i},{out_array[i]},{p[0][0]},{p[0][1]},{p[1][0]},{p[1][1]}\n")
    print(f"Saved all distances to {out_csv}")

if __name__ == "__main__":
    main()
