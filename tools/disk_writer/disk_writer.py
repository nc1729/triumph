#!/usr/bin/python3
import sys

def tryte_to_int(tryte_string):
    septavingt_chars = "MLKJIHGFEDCBA0abcdefghijklm"
    high = septavingt_chars.find(tryte_string[0]) - 13
    mid  = septavingt_chars.find(tryte_string[1]) - 13
    low  = septavingt_chars.find(tryte_string[2]) - 13

    return 729 * high + 27 * mid + low

def int_to_tryte(integer):
    triple_array = [0, 0, 0]
    div = integer
    for i in range(3):
        div, rem = divmod(div, 27)
        if rem > 13:
            rem -= 27
            div += 1
        elif rem < -13:
            rem += 27
            div -= 1
        triple_array[2 - i] = rem

    septavingt_chars = "MLKJIHGFEDCBA0abcdefghijklm"
    return "".join([septavingt_chars[triple_array[i] + 13] for i in range(3)])

def int_to_unsigned_tryte(integer):
    return int_to_tryte(integer - 9841)

def string_to_trytes(in_string):
    out_string = ""
    for char in in_string:
        char_ascii_val = ord(char)
        out_string += int_to_tryte(char_ascii_val)
    return out_string

def main():
    tilemap_file = sys.argv[1]
    output_file = sys.argv[2]
    disk_contents = ""
    # required signature (in tryte ASCII)
    signature = string_to_trytes("TRIUMPH") + "000000"
    disk_contents += signature
    
    # disk name
    disk_name = "TRIUMPH TEST DISK"
    disk_contents += (string_to_trytes(disk_name) + (27 - len(disk_name)) * "000")

    # disk size (in pages)
    disk_size = 10 # 9 for tilemap, 1 for boot sector
    disk_contents += int_to_unsigned_tryte(disk_size)

    # disk read/write permissions
    disk_permissions = 1 # read/write allowed
    disk_contents += int_to_tryte(disk_permissions)

    # disk tilemap page start
    disk_tile_ptr = 1
    disk_contents += int_to_unsigned_tryte(disk_tile_ptr)

    # disk boot code start
    disk_contents += "M00" # Tryte 364

    # disk palettes
    # set these to zero for now (tweak with boot code)
    disk_contents += ("000" * 27)

    # free space
    disk_contents += ("000" * (364 - (len(disk_contents) // 3)))

    # boot code
    boot_code = ""
    # write palette
    boot_code += "Mj000C" # M00: BANK -3 (work RAM - where palettes are stored)
    boot_code += "mdbEmm" # LOAD B, #Emm
    boot_code += "mcc00a" # CMP A, 1
    boot_code += "jjiM0j" # JPZ $M0j
    boot_code += "jj0M0b" # JP $M0b
    boot_code += "maaMMM" # SET A, #MMM
    boot_code += "macMLA" # SET C, #MLA - pointer to palette 0
    boot_code += "Dac"    # SAVE A, [C]
    boot_code += "maammM" # SET A, #mmM - yellow colour
    boot_code += "mlc"    # INC C
    boot_code += "Dac"    # SAVE A, $C
    boot_code += "maammM" # SET A, #mmM
    boot_code += "mlc"    # INC C
    boot_code += "Dac"    # SAVE A, $C
    # now write to framebuffer
    boot_code += "Mj000A" # BANK -1
    boot_code += "macLlg" # SET C, #Llg - pointer to row 26
    boot_code += "maa0aF" # SET A, 21 - 21 is 'H' in tilemap.tri, with palette 0
    boot_code += "Dac"    # SAVE A, $C
    boot_code += "mlc"    # INC C
    boot_code += "maa0bI" # SET A, 45 - 'e'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0bB" # SET A, 52 - 'l'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0bB" # SET A, 52 - 'l'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0ba" # SET A, 55 - 'o'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "mlc"    # INC C - leave a space
    boot_code += "maa0bi" # SET A, 63 - 'w'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0ba" # SET A, 55 - 'o'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0bd" # SET A, 58 - 'r'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0bB" # SET A, 52 - 'l'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0bJ" # SET A, 44 - 'd'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "maa0cm" # SET A, 94 - '!'
    boot_code += "Dacmlc" # SAVE A, $C; INC C
    boot_code += "jj0M00" # JP $M00 - loop bootcode infinitely

    padding = (365 - (len(boot_code) // 3)) * "000"

    disk_contents += boot_code
    disk_contents += padding

    with open(tilemap_file, "r") as f:
        tilemap = f.readline()
    
    disk_contents += tilemap

    with open(output_file, "w") as f:
        f.write(disk_contents)


if __name__ == "__main__":
    main()