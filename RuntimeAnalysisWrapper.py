import subprocess

def runTest(program, prototein):
    filePath = ".\Executables\\" + program
    result = subprocess.run([filePath, prototein], stdout=subprocess.PIPE)
    maxy, duration = result.stdout.decode('utf-8').split()
    return maxy, duration
    

files = ["Optimized_Parallel_Prototein.exe", "Optimized_Sequential_Prototein.exe", "Parallel_Prototein_no_output.exe"]
protoDict = {"HHHP":                    4,
             "PHHHP":                   4,
             "HPHPPH":                  4,
             "HPHHPHP":                 6,
             "PHHPPHPH":                6,
             "PPHHPPHPH":               6,
             "HHPPPPHHHP":             10,
             "HPHHHPHPPHP":            12,
             "HPHHHPHHPPPH":           14,
             "PHHHPPPHPHHPP":          12,
             "PHHPHHPHPPHHHH":         22,
             "HHPPHPHPPPHHPHH":        16,
             "PHPPPPPPHHPHPPHP":       10,
             "PHHPHHPPPPHHHPHPP":      18,
             "HPHHHPPHPHPPHHHPHH":     26,
             "HHPPPHPPPPPPHHPHHHP":    18,
             "PHPPHHHPHHPPHPPPPPPH":   18}
prototeins = list(protoDict.keys())

print(protoDict)

# Max, Duration = runTest(files[0], prototeins[0])

# print(subprocess.check_output([".\Executables\Optimized_Parallel_Prototein.exe", "HPPHPHP"]))