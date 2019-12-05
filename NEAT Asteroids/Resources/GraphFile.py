from matplotlib import pyplot
import matplotlib.patches as mpatches
import sys

colorIndex = 0
colors = ["red", "green", "blue", "orange", "purple"]

items = []
labels = []

path = sys.argv[0]
path = path[:path.rfind('/')+1]

if len(sys.argv) != 2:
    raise ValueError("No file specified")

fileName = path + sys.argv[1]

with open(fileName) as file:
    lines = file.readlines()
    print("lines", lines)
    for line in lines:
        parts = line.split(' ')
        label = parts[0]
        data = [float(data) for data in parts[1:]]
        items.append({label: data})

print(items)

pyplot.clf()
for item in items:
    for inner in item:
        labels.append(mpatches.Patch(color=colors[colorIndex], label=inner))
        pyplot.plot(item[inner], colors[colorIndex])
    colorIndex += 1
    
pyplot.legend(handles=labels)
pyplot.show()