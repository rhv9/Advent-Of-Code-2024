import  sys
import subprocess
import os

if len(sys.argv) < 2:
    print("You did not pass the file to run :/", file=sys.stderr)
    exit()

val = ""
if os.name != 'nt':
    val = subprocess.run(["g++", "-std=c++23", sys.argv[1], "-o", "./a.out"])
else:
    val = subprocess.run(["g++", "-std=c++23", sys.argv[1], "-lstdc++exp","-o", "./a.exe"])
    

if val.returncode != 0:
    exit()

print("\nRunning...\n")

if os.name != "nt":
    subprocess.run(["./a.out"])
else:
    subprocess.run(["./a.exe"])

