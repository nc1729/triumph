#!/usr/bin/python3
import sys

import tryte_tools

def main():
    tilemap_file = sys.argv[1]
    output_file = sys.argv[2]
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
    # copy boot code sector to 'main memory' at $000
    # this is required as we want to access other banks, but we're executing code from this one...
    boot_code += "macM00" # M00 M0a: SET C, #M00 - beginning of block to copy
    boot_code += "mad000" # M0b M0c: SET D, #000 - beginning of new block
    boot_code += "mab0mm" # M0d M0e: SET B, #0mm - copy 364 blocks $M00-Mmm to $000-0mm
    # boot code copy loop begins here
    boot_code += "mCb"    # M0f:     CPZ B - set compare flag to sign(B)
    boot_code += "jC00aK" # M0g M0h: JPZ $0aK - if B == 0, go to new block and begin execution of boot code proper
    boot_code += "dca"    # M0i:     LOAD [C], A
    boot_code += "Dad"    # M0j:     SAVE A, [D]
    boot_code += "mlc"    # M0k:     INC C
    boot_code += "mld"    # M0l:     INC D
    boot_code += "mLb"    # M0m:     DEC B
    boot_code += "jMJM0f" # MaM MaL: JP $M0f
    # boot code copy loop ends here

    main_code_ptr = "0aK"
    print(f"Main code ptr: {main_code_ptr}")
    main_size = 8 # four function CALLs

    write_to_framebuffer_ptr = tryte_tools.add_int_to_tryte(main_code_ptr, main_size)
    print(f"write_to_framebuffer ptr: {write_to_framebuffer_ptr}")
    write_to_framebuffer_code = ""
    # write_to_framebuffer:
    #   PUSH A; PUSH C;
    write_to_framebuffer_code += "mfamfc"
    #   PUSH G; GANK G
    write_to_framebuffer_code += "mfgmJg"
    #   BANK -1                 # switch to framebuffer
    write_to_framebuffer_code += "Mj000A"
    #   SET C, #Llg             # set to row 26
    write_to_framebuffer_code += "macLlg"
    #   SET A, 21               # 21 is 'H' in tilemap.tri, with palette 0
    write_to_framebuffer_code += "maa0aF"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 45               # 'e'
    write_to_framebuffer_code += "maa0bI"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 52               # 'l'
    write_to_framebuffer_code += "maa0bB"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 52               # 'l'
    write_to_framebuffer_code += "maa0bB"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 55               # 'o'
    write_to_framebuffer_code += "maa0ba"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   INC C                   # leave a space
    write_to_framebuffer_code += "mlc"
    #   SET A, 63               # 'w'
    write_to_framebuffer_code += "maa0bi"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 55               # 'o'
    write_to_framebuffer_code += "maa0ba"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 58               # 'r'
    write_to_framebuffer_code += "maa0bd"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 52               # 'l'
    write_to_framebuffer_code += "maa0bB"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 44               # 'd'
    write_to_framebuffer_code += "maa0bJ"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   SET A, 94               # '!'
    write_to_framebuffer_code += "maa0cm"
    #   SAVE A, [C]; INC C
    write_to_framebuffer_code += "Dacmlc"
    #   POP G; POP C; POP A;
    write_to_framebuffer_code += "mFgmFcmFa"
    #   PJP
    write_to_framebuffer_code += "j0j"
    write_to_framebuffer_size = len(write_to_framebuffer_code) // 3

    write_to_palette_0_ptr = tryte_tools.add_int_to_tryte(write_to_framebuffer_ptr, write_to_framebuffer_size)
    print(f"write_to_palette_0 ptr: {write_to_palette_0_ptr}")
    write_to_palette_0_code = ""
    # write_to_palette_0:
    #   PUSH A; PUSH C;
    write_to_palette_0_code += "mfamfc"
    #   PUSH G; GANK G  # set current bank to G
    write_to_palette_0_code += "mfgmJg"
    #   BANK -3         # switch to work_RAM
    write_to_palette_0_code += "Mj000C"
    #   SET C, #MLA     # pointer to palette 0
    write_to_palette_0_code += "macMLA"
    #   SET A, #MMM     # negative trits: black
    write_to_palette_0_code += "maaMMM"
    #   SAVE A, [C]; INC C
    write_to_palette_0_code += "Dacmlc"
    #   SET A, #mmM     # zero trits: yellow
    write_to_palette_0_code += "maammM"
    #   SAVE A, [C]; INC C
    write_to_palette_0_code += "Dacmlc"
    #   SET A, #mmM     # positive trits: yellow
    write_to_palette_0_code += "maammM"
    #   SAVE A, [C]
    write_to_palette_0_code += "Dac"
    #   BANK G
    write_to_palette_0_code += "mjg"
    #   POP G; POP C; POP A;
    write_to_palette_0_code += "mFgmFcmFa"
    #   PJP
    write_to_palette_0_code += "j0j"
    write_to_palette_0_size = len(write_to_palette_0_code) // 3
    
    
    load_page_ptr = tryte_tools.add_int_to_tryte(write_to_palette_0_ptr, write_to_palette_0_size)
    print(f"load_page ptr: {load_page_ptr}")
    load_page_code = ""
    # load_page(bank_number, page_number):
    #   function with 2 arguments - B = bank_number, C = page_number
    #   PUSH D
    load_page_code += "mfd"
    #   PUSH G; GANK G
    load_page_code += "mfgmJg"
    #   SAVE C, $Eml        # current_page = page_number
    load_page_code += "mDcEml"
    #   BANK B              # current_bank = bank_number
    load_page_code += "mjb"
    #   LOAD $Emm, D        # D = disk_flag
    load_page_code += "mddEmm"
    #   OR D, #MMG          # (set read request trit to 1)
    load_page_code += "mBdMMG"
    #   SAVE D, $Emm        # (send read request)
    load_page_code += "mDdEmm"
    #   !send_loop
    send_loop_ptr = tryte_tools.add_int_to_tryte(load_page_ptr, len(load_page_code) // 3)
    #   LOAD D, $Emm
    load_page_code += "mddEmm"
    #   STAR D, 3
    load_page_code += "mkd00c"
    #   CPZ D
    load_page_code += "mCd"
    #   JPP send_loop       # if D > 0, disk hasn't started reading, go back to send_loop
    load_page_code += "jA0" + send_loop_ptr
    #   !wait_loop
    wait_loop_ptr = tryte_tools.add_int_to_tryte(load_page_ptr, len(load_page_code) // 3)
    #   LOAD D, $Emm
    load_page_code += "mddEmm"
    #   STAR D, 1
    load_page_code += "mkd00a"
    #   CPZ D
    load_page_code += "mCd"
    #   JPZ wait_loop       # if D = 0, disk is still busy, go back to wait_loop
    load_page_code += "jC0" + wait_loop_ptr
    #   BANK G
    load_page_code += "mjg"
    #   POP G; POP D;
    load_page_code += "mFgmFd"
    #   PJP
    load_page_code += "j0j"
    load_page_size = len(load_page_code) // 3
    


    memcpy_ptr = tryte_tools.add_int_to_tryte(load_page_ptr, load_page_size)
    print(f"memcpy ptr: {memcpy_ptr}")
    memcpy_code = ""
    # memcpy(start_ptr, dest_ptr, length):
    #   function with 3 arguments - 
    #   B = start_ptr, C = dest_ptr, D = length
    #   !loop
    loop_ptr = memcpy_ptr
    #   CPZ D
    memcpy_code += "mCd"
    #   JPZ done
    done_start_ptr = tryte_tools.add_int_to_tryte(memcpy_ptr, len(memcpy_code) // 3)
    memcpy_code += "jC0" + tryte_tools.add_int_to_tryte(done_start_ptr, 11)
    #   PUSH E
    memcpy_code += "mfe"
    #   LOAD [B], E
    memcpy_code += "dbe"
    #   SAVE E, [C]
    memcpy_code += "Dec"
    #   POP E
    memcpy_code += "mFe"
    #   INC B; INC C; DEC D
    memcpy_code += "mlbmlcmLd"
    #   JP loop
    memcpy_code += "jMJ" + loop_ptr
    #   !done
    #   PJP
    memcpy_code += "j0j"
    memcpy_size = len(memcpy_code) // 3

    copy_to_tilemap_ptr = tryte_tools.add_int_to_tryte(memcpy_ptr, memcpy_size)
    print(f"copy_to_tilemap ptr: {copy_to_tilemap_ptr}")
    copy_to_tilemap_code = ""
    # copy_to_tilemap:
    #   PUSH B; PUSH C; PUSH D;
    copy_to_tilemap_code += "mfbmfcmfd"
    #   SET B, 1            # bank number (points to boot disk)
    copy_to_tilemap_code += "mab00a"
    #   SET C, 1           # page number
    copy_to_tilemap_code += "mac00a"
    #   SET D, #MMM           # data pointer
    copy_to_tilemap_code += "madMMM"
    #   PUSH G; GANK G      # set G to curent bank
    copy_to_tilemap_code += "mfgmJg"
    #   !start
    start_ptr = tryte_tools.add_int_to_tryte(copy_to_tilemap_ptr, len(copy_to_tilemap_code) // 3)
    #   BANK B             # set bank to disk
    copy_to_tilemap_code += "mjb"
    #   CALL load_page(B, C)
    #   B and C are already in place - just need to JPS to load_page
    copy_to_tilemap_code += "j0J" + load_page_ptr
    #   CALL memcpy($MMM, $AMM, 729)
    #   need to PUSH B; PUSH C; PUSH D
    copy_to_tilemap_code += "mfbmfcmfd"
    #   then SET B, #MMM; SET C, #AMM; SET D, #a00
    copy_to_tilemap_code += "mabMMMmacAMMmada00"
    #   then JPS to memcpy
    copy_to_tilemap_code += "j0J" + memcpy_ptr
    #   on function return, recover B, C and D
    #   POP D; POP C; POP B
    copy_to_tilemap_code += "mFdmFcmFb"
    #   BANK -2             # switch to tilemap
    copy_to_tilemap_code += "Mj000B"
    #   CALL memcpy($AMM, D, 729)
    copy_to_tilemap_code += "mfbmfcmfd"
    copy_to_tilemap_code += "mabAMMacdmada00"
    copy_to_tilemap_code += "j0J" + memcpy_ptr
    copy_to_tilemap_code += "mFdmFcmFb"
    #   ADD D, 729          # move data pointer to next page in framebuffer
    copy_to_tilemap_code += "meda00"
    #   INC C
    copy_to_tilemap_code += "mlc"
    #   CMP C, 10          # loop to start 9 times
    copy_to_tilemap_code += "mcc00j"
    #   JPN start
    copy_to_tilemap_code += "jI0" + start_ptr
    #   BANK G
    copy_to_tilemap_code += "mjg"
    #   POP G; POP D; POP C; POP B;
    copy_to_tilemap_code += "mFgmFdmFcmFb"
    #   PJP
    copy_to_tilemap_code += "j0j"
    copy_to_tilemap_size = len(copy_to_tilemap_code) // 3

    infinite_loop_ptr = tryte_tools.add_int_to_tryte(copy_to_tilemap_ptr, copy_to_tilemap_size)
    print(f"infinite_loop ptr: {infinite_loop_ptr}")
    infinite_loop_code = ""
    # infinite_loop:
    #   JP infinite_loop
    infinite_loop_code += "jMJ" + infinite_loop_ptr
    infinite_loop_size = len(infinite_loop_code)

    main_code = ""
    # main:
    #   CALL copy_into_tilemap
    #   CALL write_to_palette_0
    #   CALL write_to_framebuffer
    #   CALL infinite_loop
    #   
    #   CALL func -> JPS $func
    main_code += "j0J" + copy_to_tilemap_ptr
    main_code += "j0J" + write_to_palette_0_ptr
    main_code += "j0J" + write_to_framebuffer_ptr
    main_code += "j0J" + infinite_loop_ptr

    # put it all together!
    boot_code += (main_code + write_to_framebuffer_code 
    + write_to_palette_0_code + load_page_code + memcpy_code + copy_to_tilemap_code
    + infinite_loop_code)

    print(f"Length of boot code: {len(boot_code) // 3}")


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