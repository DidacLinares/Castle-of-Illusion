import json
import argparse

def convert_json_to_custom(json_file, output_file):
    with open(json_file, 'r') as f:
        data = json.load(f)

    tilemap = data['layers'][0]['data']
    width = data['width']
    height = data['height']

    with open(output_file, 'w') as f:
        f.write("TILEMAP\n")
        f.write(f"{width} {height} -- Size of tile map in tiles\n")
        f.write("16 16 -- Tile size & block size\n")
        f.write("images/blocks.png -- Tilesheet\n") # Canviar manualment al necesari
        f.write("2 2 -- Number of tiles in tilesheet\n") # Canviar manualment al necesari

        for y in range(height):
            # Sumem 1, perque 0 es espai, i les ids a tiled van de 0 a n.
            row = tilemap[y * width:(y + 1) * width]
            formatted_row = ' '.join(str(tile) for tile in row)
            f.write(formatted_row + '\n')

def main():
    parser = argparse.ArgumentParser(description="Convert JSON tilemap to custom format.")
    parser.add_argument('json_file', type=str, help="The input JSON file.")
    parser.add_argument('output_file', type=str, help="The output text file.")
    
    args = parser.parse_args()

    convert_json_to_custom(args.json_file, args.output_file)

if __name__ == '__main__':
    main()
