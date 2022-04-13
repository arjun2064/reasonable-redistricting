import json

input = open('wisconsin2020_graph.json', 'r')
output = open('cdata.txt', 'w')
data = json.load(input)

# [number of precincts]
output.write(str(len(data['nodes'])))
output.write('\n')

# For each precinct
for id, precinct in enumerate(data['nodes']):
    population = int(precinct['TOTPOP'])
    democrat_votes = int(precinct['PRES16D'])
    republican_votes = int(precinct['PRES16R'])
    # [population, democrat_votes, republican_votes]
    output.write('  ') # a bit of spacing to look nicer
    output.write(' '.join( str(item) for item in [population, democrat_votes, republican_votes] ))
    output.write('\n')

    # [number of adjacencies, list of adjacent indexes]
    output.write('    ') # a bit of spacing to look nicer
    output.write(str(len(data['adjacency'][id])))
    output.write('  ')
    output.write(' '.join( str(int(adj['id'])) for adj in data['adjacency'][id] ))
    output.write('\n')