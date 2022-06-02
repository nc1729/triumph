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
        with open(expected_output, 'r') as file:
            expected = file.readline()
        print(f"Expected output:  {expected}")
        print(f"Test output:      {output.stdout}")
        return False


def run_test_family(test_root_dir, tas_path):

    # fetch test names
    test_names = []
    for subdir, dirs, files in os.walk(test_root_dir):
        test_name = subdir[len(test_root_dir)+1:]
        if len(test_name) != 0:
            test_names.append(test_name)
    
    number_of_passing_tests = 0
    for test in test_names:
        if run_test(test, test_root_dir + "/" + test, tas_path):
            number_of_passing_tests += 1
        else:
            print("Test " + test + " failed.\n")

    number_of_tests = len(test_names)
    print(f"{number_of_passing_tests} of {number_of_tests} tests passed.")


def loop_through_dirs():
    rootdir = os.getcwd() + "/instr_tests"
    for subdir, dirs, files in os.walk(rootdir):
        print(subdir)
    """
    for subdir, dirs, files in os.walk(rootdir):
        print(subdir)
        print(dirs)
        print(files)
        print("\n\n")
    """

def main():
    if len(sys.argv) != 2:
        print("Usage: run_tests.py [TAS_EXECUTABLE]")
        return 1
    
    # fetch path to tas executable
    tas_path = sys.argv[1]
    
    # run instr tests
    print("Running instr tests...")
    instr_test_dir = os.getcwd() + "/instr_tests"
    run_test_family(instr_test_dir, tas_path)

    # run macro tests
    print("Running macro tests...")
    macro_test_dir = os.getcwd() + "/macro_tests"
    run_test_family(macro_test_dir, tas_path)


if __name__ == "__main__":
    main()