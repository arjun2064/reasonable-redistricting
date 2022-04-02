import json

with open('wisconsin2020_graph.json', 'r') as f:
    data = json.load(f)
    print(len(data['nodes']))
    # print(len(data['adjacency']))
    total = 0
    for adjacency in data['adjacency']:
        total += len(adjacency)
    print(total)
