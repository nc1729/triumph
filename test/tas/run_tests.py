#!/usr/bin/python3

import subprocess
import compare_files
import os

tests = ["set", "add"]
number_of_tests = len(tests)
tas_path = "../bin/tas"

def run_test(test_name):
    test_dir = "./" + test_name
    test_input = test_dir + "/" + test_name + ".tas"
    test_output = test_dir + "/out.tri"
    expected_output = "./" + test_name + "/expected"
    output = subprocess.run([tas_path, "-i", test_input, "-o", test_output])
    test_result = compare_files.compare_files(test_output, expected_output)
    if test_result:
        os.remove(test_output)
        return True
    else:
        return False

number_of_passing_tests = 0
for test in tests:
    if run_test(test):
        number_of_passing_tests += 1
    else:
        print("Test " + test + " failed.")

print(f"{number_of_passing_tests} of {number_of_tests} tests passed.")

