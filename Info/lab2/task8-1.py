def validate_input(string):
    if bool(set(string) - {'1', '0'}) or len(string) != 7:
        print('The entered string must be a set of 7 digits "0" and "1".')
        exit(1)

def input_to_bits(string):
    return list(map(int, list(string)))

def syndrome(arr):
    s1 = (arr[0] + arr[2] + arr[4] + arr[6]) % 2
    s2 = (arr[1] + arr[2] + arr[5] + arr[6]) % 2
    s3 = (arr[3] + arr[4] + arr[5] + arr[6]) % 2
    return (s1, s2, s3)

def has_error(arr):
    return syndrome(arr) != (0, 0, 0)

def error_index(arr):
    return int(''.join(map(str, syndrome(arr)[::-1])), 2)

def error_symbol(arr):
    return {1: 'r1', 2: 'r2', 3: 'i1', 4: 'r3', 5: 'i2', 6: 'i3', 7: 'i4'}[error_index(arr)]

def inf_bits(arr):
    return [arr[2], arr[4], arr[5], arr[6]]

def make_result_message(bits):
    return ''.join(map(str, bits))

def fixed_message(arr):
    if not has_error(arr) or error_symbol(arr)[0] == 'r':
        return make_result_message(inf_bits(arr))
    ind = int(error_symbol(arr)[1]) - 1
    result = inf_bits(arr)
    result[ind] = (result[ind] + 1) % 2
    return make_result_message(result)


inp = input('Enter a set of 7 digits "0" and "1" written in a row:')
validate_input(inp)
bits = input_to_bits(inp)

if has_error(bits):
    print(f'> There is an error in the message!\nError in the symbol {error_symbol(bits)}')
else:
    print('> There are no errors in the message!')

print(f'Correct message: {fixed_message(bits)}')
