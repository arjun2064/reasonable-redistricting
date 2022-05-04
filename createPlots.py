import matplotlib.pyplot as plt

# MST
#define data
data = list(map(lambda x:float(x), open('./output/mean-median-mst.txt').readlines()))

#create histogram with specific number of bins
plt.hist(data, bins=50)
plt.title("Histogram of Mean-Median for MST")
plt.savefig('plots/mean-median-mst.png')
plt.clf()

# Wilson's
#define data
data = list(map(lambda x:float(x), open('./output/mean-median-wilson.txt').readlines()))

#create histogram with specific number of bins
plt.hist(data, bins=50) 
plt.title("Histogram of Mean-Median for UST/Wilson's")
plt.savefig('plots/mean-median-wilson.png')