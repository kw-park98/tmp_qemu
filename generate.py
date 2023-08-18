import random
import string
import os

def fill_file_with_random_numbers(file_path, file_size_MB):
    file_size = file_size_MB * 1024 * 1024  # 1MB
    str_size = 4 * 1024  # 4KB

    characters = string.digits

    with open(file_path, 'w') as f:
        while os.path.getsize(file_path) < file_size:
            random_str = ''.join(random.choice(characters) for _ in range(str_size))
            f.write(random_str)

if __name__ == "__main__":
#1GB -> 1024
#1MB ->    1
    fill_file_with_random_numbers('tmp.txt', 1024)
