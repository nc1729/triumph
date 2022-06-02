#!/usr/bin/python3

import subprocess
import compare_files
import os
import sys

def run_test(test_name, test_dir, tas_path):
    test_input = test_dir + "/" + test_name + ".tas"
    test_output = test_dir + "/out.tri"
    expected_output = test_dir + "/expected"
    output = subprocess.run([tas_path, "-i", test_input, "-o", test_output], capture_output=True, text=True)
    test_result = compare_files.compare_files(test_output, expected_output)
    if test_result:
        os.remove(test_output)
        return True
    else:
        return False

def run_all_tests(test_name_list, test_path, tas_path):
    number_of_passing_tests = 0
    for test in test_name_list:
        if run_test(test, test_path + "/" + test, tas_path):
            number_of_passing_tests += 1
        else:
            print("Test " + test + " failed.")

    number_of_tests = len(test_name_list)
    print(f"{number_of_passing_tests} of {number_of_tests} tests passed.")

def main():
    if len(sys.argv) != 2:
        return 1
    else:
        tas_path = sys.argv[1]
        cwd = os.getcwd()
        instr_tests = ["set", "add", "load", "save", "bank", "gank", "swap", "adc", "and"]
        instr_test_path = cwd + "/instr_tests"
        run_all_tests(instr_tests, instr_test_path, tas_path)
    return 0


if __name__ == "__main__":
    main()