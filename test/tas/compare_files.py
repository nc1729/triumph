#!/usr/bin/python3

def compare_files(filename1, filename2):
    file1_lines = []
    file2_lines = []
    with open(filename1, 'r') as file1:
        file1_lines = file1.readlines()
    with open(filename2, 'r') as file2:
        file2_lines = file2.readlines()
    
    if len(file1_lines) != len(file2_lines):
        return False
    
    for index in range(len(file1_lines)):
        if file1_lines[index] != file2_lines[index]:
            return False
    
    return True