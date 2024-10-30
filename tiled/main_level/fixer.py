with open("map.txt", "r") as file:
    data = file.read()

lines = data.split("\n")
matrix = []

for line in lines:
    matrix.append([int(x) for x in line.split()])

lowest = matrix[0][0]

for y in range(len(matrix)):
    for x in range(len(matrix[y])):
        if matrix[y][x] != 0 and matrix[y][x] < lowest:
            print(f"Position ({x}, {y}) has a lower value: {matrix[y][x]}")
            lowest = matrix[y][x]

print(f"Lowest value: {lowest}")

for y in range(len(matrix)):
    for x in range(len(matrix[y])):
        if matrix[y][x] != 0:
            matrix[y][x] -= lowest
            matrix[y][x] += 2
            matrix[y][x] += 1

with open("map_fixed.txt", "w") as file:
    for row in matrix:
        file.write(" ".join(str(x) for x in row) + "\n")

print("Fixed map written to map_fixed.txt")