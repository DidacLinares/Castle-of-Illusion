import json
import numpy as np

def main():
    file_name = input("Enter the file name: ")
    print(file_name)

    with open(file_name) as file:
        data = json.load(file)
    
    map_width = data["width"]
    map_height = data["height"]
    layers = data["layers"]

    for layer in layers:
        matrix = np.full((map_height, map_width), 0)


        x = 0
        y = 0
        for tile in layer['data']:
            tile_id = int(tile)

            matrix[y][x] = tile_id
            x += 1
            if x == map_width:
                x = 0
                y += 1
        
        output_file = f"{layer['name']}.txt"
        np.savetxt(output_file, matrix, fmt='%d')
        print(f"Written matrix for layer {layer['name']} to {output_file}")

if __name__ == "__main__":
    main()