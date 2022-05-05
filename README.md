# Reasonable Redistricting

## Video Presentation
https://drive.google.com/file/d/10hNdb1X00A8rWSLcZQzB4ZIWKJbnvjXg/view?usp=sharing

## Usage

**Note: Only running C++ code can be run on EWS without docker. Docker provides python3 and matplotlib needed for pre/post processing**

Open project in Docker with provided `Dockerfile`. If using an M1 mac change first line of `FROM itsjustin/cs225-container-nextgen:legacy` to `FROM itsjustin/cs225-container-nextgen:legacy-arm`

#### Preprocessing
```
python3 data-parser.py
```

### **Run Algorithms (for EWS grading)**
```
make && ./recombination <input txt file> <output prefix>
```

Using datasets in repo
```
make && ./recombination cdata.txt ./output/mean-median
```
```
make && ./recombination pa-data.txt ./output/mean-median
```

#### Post-processing plots
```
python3 createPlots.py
```
Plots will then be available under the `plots` folder

## Tests
Run tests with
```
make test && ./test
```

## Development
Debug friendly binary
```
make debug && ./debug
```


## Code Organization
- `data` for input data for algorithms
- `output` for output data from algorithms
- `plots` for plots created by postprocessing matplotlib
- `testing` for all testing code
- `documents` for all documents