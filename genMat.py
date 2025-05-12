import random

def generate_matrix_file(filename, rows, cols):
    with open(filename, 'w') as f:
        # Write number of rows and columns
        f.write(f"{rows} {cols}\n")
        
        # Write matrix rows
        for _ in range(rows):
            row = [str(random.randint(0, 99)) for _ in range(cols)]
            f.write(' '.join(row) + '\n')

if __name__ == "__main__":
    rows, cols = 100, 100
    name = input("enter name of the file")
    generate_matrix_file(name, rows, cols)
    print("File 'matrix.txt' created with a 1000x1000 matrix.")

