import pandas as pd

# تابع برای تبدیل عدد به دودویی با علامت (16 بیتی) با استفاده از مکمل دو
def dec_to_bin_with_sign(num, bit_size=16):
    if num >= 0:
        # برای اعداد مثبت یا صفر، تبدیل مستقیم به دودویی
        return format(num, f'0{bit_size}b')
    else:
        # برای اعداد منفی، استفاده از مکمل دو
        # تبدیل عدد مثبت به دودویی
        bin_num = format(abs(num), f'0{bit_size}b')
        # معکوس کردن بیت‌ها
        inverted_bin = ''.join('1' if x == '0' else '0' for x in bin_num)
        # افزودن 1 به نتیجه معکوس شده
        complement_two = bin(int(inverted_bin, 2) + 1)[2:].zfill(bit_size)
        return complement_two

# داده‌های اولیه (A و C)
A = 4207
C = 14708

# محاسبه X1 تا X12
X = {
    'X1': A,
    'X2': C,
    'X3': A + C,
    'X4': A + C + C,
    'X5': C - A,
    'X6': 65536 - (A + C + C),
    'X7': -A,
    'X8': -C,
    'X9': -(A + C),
    'X10': -(A + C + C),
    'X11': -(C - A),
    'X12': -(65536 - (A + C + C))
}

# تبدیل اعداد دهدهی به دودویی 16 بیتی
X_bin = {key: dec_to_bin_with_sign(value) for key, value in X.items()}

# ساخت DataFrame برای نمایش نتایج
df = pd.DataFrame(list(X_bin.items()), columns=['X', 'Binary'])

# ذخیره DataFrame به فایل Excel
file_name = "output.xlsx"
df.to_excel(file_name, index=False, engine='openpyxl')

print(f"Data has been saved to {file_name}")