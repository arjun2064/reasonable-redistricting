import matplotlib.pyplot as plt

# MST
#define data
data = list(map(lambda x:float(x), open('./output/mean-median-mst.txt').readlines()))
# data = list(map(lambda x:float(x), open('./output/mean-median-mst-pa.txt').readlines()))

#create histogram with specific number of bins
plt.hist(data, bins=50)
plt.title("Histogram of Mean-Median for MST")
plt.savefig('plots/mean-median-mst.png')
# plt.savefig('plots/mean-median-mst-pa.png')
plt.clf()

# Wilson's
#define data
data = list(map(lambda x:float(x), open('./output/mean-median-wilson.txt').readlines()))
# data = list(map(lambda x:float(x), open('./output/mean-median-wilson-pa.txt').readlines()))

#create histogram with specific number of bins
plt.hist(data, bins=50) 
plt.title("Histogram of Mean-Median for UST/Wilson's")
plt.savefig('plots/mean-median-wilson.png')
# plt.savefig('plots/mean-median-wilson-pa.png')


# Overlapping
# MST
#define data
mst_data = list(map(lambda x:float(x), open('./output/mean-median-mst.txt').readlines()))
# data = list(map(lambda x:float(x), open('./output/mean-median-mst-pa.txt').readlines()))
wilson_data = list(map(lambda x:float(x), open('./output/mean-median-wilson.txt').readlines()))
# data = list(map(lambda x:float(x), open('./output/mean-median-wilson-pa.txt').readlines()))

#create histogram with specific number of bins
plt.hist(mst_data, bins=50, alpha=0.5, label="MST")
plt.hist(wilson_data, bins=50, alpha=0.5, label="Wilson's")
plt.title("Histogram of Mean-Median for MST/UST")
plt.legend(loc='upper right')
plt.savefig('plots/mean-median-overlapping.png')
plt.clf()

exit()

# plot to compare with different existing plans
# data = list(map(lambda x:float(x), open('./output/mean-median-mst.txt').readlines()))
# data = list(map(lambda x:float(x), open('./output/mean-median-mst-pa.txt').readlines()))

# #create histogram with specific number of bins
# plt.hist(data, bins=50)
# plt.title("Histogram of Mean-Median with MST ")

# _, max_ylim = plt.ylim()
# with open("./output/PA-mean-median.txt") as f:
#     for i, line in enumerate(f):
#         planKey, metric = line.split(" ")
#         if planKey not in ["GOV", "538DEM", "538GOP", "538CMPCT"]:
#             continue
#         metric = float(metric)
#         plt.axvline(metric, color='orange', linestyle='dashed', linewidth=2)
#         # plt.axvline(metric, linestyle='dashed', linewidth=2)
#         plt.text(metric*1.1, max_ylim*(0.9-i*0.1), f"{planKey}")

# plt.savefig('plots/compare.png')
# plt.clf()