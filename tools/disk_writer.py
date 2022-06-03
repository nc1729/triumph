#!/usr/bin/python3
import sys

import tryte_tools

def main():
    tilemap_file = sys.argv[1]
    code_file = sys.argv[2]
    output_file = sys.argv[3]
    disk_contents = ""
    # required signature (in tryte ASCII)
    signature = tryte_tools.string_to_trytes("TRIUMPH") + "000000"
    disk_contents += signature
    
    # disk name
    disk_name = "TRIUMPH TEST DISK"
    disk_contents += (tryte_tools.string_to_trytes(disk_name) + (27 - len(disk_name)) * "000")

    # disk size (in pages)
    disk_size = 10 # 9 for tilemap, 1 for boot sector
    disk_contents += tryte_tools.int_to_tryte(disk_size)

    # disk read/write permissions
    # 1 - read/write allowed
    # 0 - read only
    # -1 - write only
    disk_permissions = 1 # read/write allowed
    disk_contents += tryte_tools.int_to_tryte(disk_permissions)

    # disk tilemap page start
    disk_tile_ptr = 1
    disk_contents += tryte_tools.int_to_tryte(disk_tile_ptr)

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
    # copy boot code sector to 'main memory' at $DMM
    # this is required as we want to access other banks, but we're executing code from this one...
    boot_code += "macMaK" # M00 M0a: SET C, #MaK - beginning of block to copy
    boot_code += "madDMM" # M0b M0c: SET D, #DMM - beginning of new block
    boot_code += "mab0mm" # M0d M0e: SET B, #0mm - copy 364 blocks $MaK-LMb to $DMM-D00
    # boot code copy loop begins here
    boot_code += "mCb"    # M0f:     CPZ B - set compare flag to sign(B)
    boot_code += "jCjDMM" # M0g M0h: JPZ $DMM - if B == 0, go to new block and begin execution of boot code proper
    boot_code += "dac"    # M0i:     LOAD A, [C]
    boot_code += "Dad"    # M0j:     SAVE A, [D]
    boot_code += "mlc"    # M0k:     INC C
    boot_code += "mld"    # M0l:     INC D
    boot_code += "mLb"    # M0m:     DEC B
    boot_code += "jMjM0f" # MaM MaL: JP $M0f
    # boot code copy loop ends here

    # now add main boot code (up to 364 trytes here)
    with open(code_file, 'r') as main_code_file:
        main_code = main_code_file.readline()
    boot_code += main_code

    padding = (365 - (len(boot_code) // 3)) * "000"
    boot_code += padding
    disk_contents += boot_code

    # open tilemap file (6561 Trytes) and write to next 9 pages
    with open(tilemap_file, "r") as f:
        tilemap = f.readline()
    disk_contents += tilemap

    # write to output disk file
    with open(output_file, "w") as f:
        f.write(disk_contents)

if __name__ == "__main__":
    main()