import csv

def main():
    input_csv = "parameter.csv"
    output_header = "config.h"

    with open(input_csv, newline='') as csvfile:
        reader = csv.reader(csvfile)
        lines = [row for row in reader if len(row) == 2]

    # Find the maximum macro name length for alignment
    max_key_length = max(len(row[0].strip()) for row in lines)

    with open(output_header, 'w') as hfile:
        hfile.write("// Auto-generated config header file\n\n")
        for key, value in lines:
            key = key.strip()
            value = value.strip()
            padding = " " * (max_key_length - len(key) + 4)
            hfile.write(f"#define {key}{padding}{value}\n")

    print("[OK] Header file 'config.h' generated from 'parameter.csv'.")

if __name__ == "__main__":
    main()