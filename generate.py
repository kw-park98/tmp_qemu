import random
import string
import os

def fill_file_with_random_numbers(file_path, file_size_MB):
    # The size of the file in bytes
    file_size = file_size_MB * 1024 * 1024  # MB
    # The size of the string that will be written at a time
    str_size = 1024  # 1KB

    # The set of characters to be used
    characters = string.digits

    # Open the file in write mode
    with open(file_path, 'w') as f:
        while os.path.getsize(file_path) < file_size:
            # Generate a random string of the specified size
            random_str = ''.join(random.choice(characters) for _ in range(str_size))
            # Write the string to the file
            f.write(random_str)

if __name__ == "__main__":
    fill_file_with_random_numbers('tmp.txt', 50)
