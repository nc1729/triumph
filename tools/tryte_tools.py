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

def unsigned_tryte_to_int(tryte):
    return tryte_to_int(tryte) + 9841

def string_to_trytes(in_string):
    out_string = ""
    for char in in_string:
        char_ascii_val = ord(char)
        out_string += int_to_tryte(char_ascii_val)
    return out_string

def tryte_to_char(tryte):
    tryte_int = tryte_to_int(tryte)
    return chr(tryte_int)

def trytes_to_string(tryte_string):
    out_string = ""
    for i in range(len(tryte_string) // 3):
        tryte = tryte_string[(3*i):(3*i + 3)]
        print(tryte)
        out_string += tryte_to_char(tryte)
    return out_string