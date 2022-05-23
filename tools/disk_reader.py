import sys

import tryte_tools

def main():
    disk_filename = sys.argv[1]
    with open(disk_filename, "r") as f:
        disk_contents = f.readline()
    
    disk_ptr = 0
    is_bootable = True
    
    # fetch signature
    sig_length = 9
    signature = disk_contents[:3*sig_length]
    sig_string = tryte_tools.trytes_to_string(signature)
    if sig_string == "TRIUMPH\0\0":
        print("Signature is valid!")
    else:
        print(f"Invalid signature: {sig_string}")
        is_bootable = False
    disk_ptr += 3*sig_length

    # fetch disk name
    name_length = 27
    disk_name_trytes = disk_contents[disk_ptr:(disk_ptr+3*name_length)]
    disk_name = tryte_tools.trytes_to_string(disk_name_trytes)
    print(f"Disk name: {disk_name}")
    disk_ptr += 3*name_length

    # fetch disk size
    size_length = 1
    size_tryte = disk_contents[disk_ptr:disk_ptr+3*size_length]
    disk_size = tryte_tools.unsigned_tryte_to_int(size_tryte)
    print(f"Disk size: {disk_size} pages")
    if len(disk_contents) != disk_size * 2187:
        if disk_contents % 2187 != 0:
            print(f"Warning - expected 729 Trytes in page {disk_size - 1} but found {(disk_contents % 2187) // 3} Trytes")
            is_bootable = False
        if disk_contents // 2187 != disk_size:
            print(f"Warning: disk size value {disk_size} does not match real disk size {disk_contents // 2187}")
            is_bootable = False
    disk_ptr += 3*size_length

    # fetch disk r/w permissions
    rw_length = 1
    rw_tryte = disk_contents[disk_ptr:disk_ptr+3*rw_length]
    rw_int = tryte_tools.tryte_to_int(rw_tryte)
    if rw_int % 3 == 1:
        print("Disk is readable and writable")
    elif rw_int % 3 == 0:
        print("Disk is read-only")
    else:
        print("Disk is write-only.")
    disk_ptr += 3*rw_length

    # fetch disk tilemap page start
    tilemap_ptr_length = 1
    tilemap_ptr = disk_contents[disk_ptr:disk_ptr+3*tilemap_ptr_length]
    tilemap_length = tryte_tools.unsigned_tryte_to_int(tilemap_ptr)
    print(f"Tilemap pages: {tilemap_length}-{tilemap_length+8}")
    if tilemap_length + 9 > disk_size:
        print("Warning - no space on disk for full tilemap")
        is_bootable = False
    disk_ptr += 3*tilemap_ptr_length

    # fetch boot code ptr
    boot_ptr_length = 1
    boot_ptr = disk_contents[disk_ptr:disk_ptr+3*boot_ptr_length]
    print(f"Boot pointer: {boot_ptr}")
    disk_ptr += 3*boot_ptr_length

    # fetch palettes
    palette_length = 3
    number_of_palettes = 27
    palette_grid_length = palette_length * number_of_palettes
    for i in range(number_of_palettes):
        palette = disk_contents[disk_ptr:disk_ptr+3*palette_length]
        palette_list = [palette[3*j:3*j+3] for j in range(palette_length)]
        print(f"Palette {i - 13}: {palette_list} ")
        disk_ptr += 3*palette_length
    
    # start of free space
    print(f"Start of free: {disk_ptr // 3}")
    disk_ptr += ((364 - disk_ptr // 3) * 3)

    # fetch boot code
    print("Boot code sep dump: ")
    boot_code = ""
    boot_code_ptr = tryte_tools.tryte_to_int("M00")
    line_addr = "M00"
    while disk_ptr < 2187:
        if boot_code_ptr % 27 == 0:
            boot_code += line_addr + ": "
        boot_code += disk_contents[disk_ptr:disk_ptr+3]
        boot_code += " "
        boot_code_ptr += 1
        if boot_code_ptr % 27 == 0:
            boot_code += "\n"
            line_addr = tryte_tools.int_to_tryte(boot_code_ptr)
        disk_ptr += 3
    print(boot_code)
        


if __name__ == "__main__":
    main()