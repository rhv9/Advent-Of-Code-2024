import  sys
import subprocess

if len(sys.argv) < 2:
    print("You did not pass the file to run :/", file=sys.stderr)
    exit()

val = subprocess.run(["g++", "-std=c++23", sys.argv[1], "-o", "./a.out"])

if val.returncode != 0:
    exit()

print("\nRunning...\n")

val = subprocess.run(["./a.out"])

