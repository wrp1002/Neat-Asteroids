from matplotlib import pyplot
import matplotlib.patches as mpatches
import sys

colorIndex = 0
colors = ["red", "green", "blue", "orange", "purple"]

items = []
labels = []

for arg in sys.argv[1:]:
    if arg.replace('.','',1).isdigit():
        if len(items) == 0:
            raise ValueError("Error with input")
        key = list(items[-1].keys())[0]
        items[-1][key].append(float(arg))
    else:
        items.append({arg: []})

print(items)

pyplot.clf()
for item in items:
    for inner in item:
        labels.append(mpatches.Patch(color=colors[colorIndex], label=inner))
        pyplot.plot(item[inner], colors[colorIndex])
    colorIndex += 1
    
pyplot.legend(handles=labels)
pyplot.show()