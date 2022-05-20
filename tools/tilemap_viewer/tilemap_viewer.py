import sys

ternary_chars = {"M" : "---", "L" : "--0", "K" : "--+", "J" : "-0-", "I" : "-00", "H" : "-0+",
"G" : "-+-", "F" : "-+0", "E" : "-++", "D" : "0--", "C" : "0-0", "B" : "0-+", "A" : "00-",
"0" : "000", "a" : "00+", "b" : "0+-", "c" : "0+0", "d" : "0++", "e" : "+--", "f" : "+-0",
"g" : "+-+", "h" : "+0-", "i" : "+00", "j" : "+0+", "k" : "++-", "l" : "++0", "m" : "+++"}

def input_loop():
    valid_input = False
    while not valid_input:
        try:
            x = int(input("Input x-coord of tile to view: "))
            y = int(input("Input y-coord of tile to view: "))
            if x in range(27) and y in range(27):
                valid_input = True
            else:
                print("coords must be between 0 and 26 inclusive.")
        except ValueError:
            print("Invalid input...")
    
    return x, y

def valid_tilemap(tilemap):
    # if tilemap contains invalid characters, return False
    for triple in tilemap:
        if triple not in ternary_chars:
            return False
    return True

def fetch_tile_trytes(x, y, tilemap):
    # given an (x, y) coord of a tile, return the 9 Trytes (27 chars) that make up the tile 
    offset = (729 * y) + (27 * x)
    return tilemap[offset:(offset+27)]

def print_tile(tile_trytes):
    index = 0
    while index < 27:
        tryte = tile_trytes[index:(index+3)]
        print(ternary_chars[tryte[0]] + ternary_chars[tryte[1]] + ternary_chars[tryte[2]])
        index += 3

def main():
    tilemap_filename = sys.argv[1]
    with open(tilemap_filename, "r") as f:
        tilemap = f.readline()
    if len(tilemap) != 19683:
        # tilemaps are 27x27 tiles, each of which is 9 Trytes, each of which is 3 chars
        print("Tilemap must be a file with 19683 chars")
        return
    if not valid_tilemap(tilemap):
        print("Tilemap contains non-septavingtesmal chars")
        return
    while True:
        x, y = input_loop()
        tile_trytes = fetch_tile_trytes(x, y, tilemap)
        print_tile(tile_trytes)


        
        


if __name__ == "__main__":
    main()

