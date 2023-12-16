import matplotlib.colors
import pandas as pd
import matplotlib.pyplot as plt
csv1 = pd.read_csv("data/n_big.csv")
# csv2 = pd.read_csv("m_longer.csv")
# csv3 = pd.read_csv("m_long.csv")
# data = pd.concat([csv1,csv2,csv3])
# data = data[data["count"] <= 15]
data = csv1

exact = data[data["algo"] == "exact"]
bound = data[data["algo"] == "bound"]
tree = data[data["algo"] == "tree"]
star = data[data["algo"] == "star"]

# cm = {"exact":0,"bound":1,"tree":2,"star":3}
cm = {"tree":0,"star":1}

# colors = ["red", "green", "blue", "orange"]
colors = ["blue","orange"]

plt.scatter(data["count"], data["time_us"]/1000, c=[cm[x] for x in data["algo"]], cmap=matplotlib.colors.ListedColormap(colors))
cb = plt.colorbar()
# cb.set_ticks([0.125*3,0.375*3,0.625*3,0.875*3])
# cb.set_ticklabels(["exact","bound","tree","star"])
cb.set_ticks([0.25,0.75])
cb.set_ticklabels(["tree","star"])
plt.xlabel("count")
plt.ylabel("time(ms)")
plt.yscale("log")
plt.title("N Sequences: time(ms) vs count")
# plt.scatter(exact["len"], exact["score"])
# plt.scatter(bound["len"], bound["score"])
# plt.scatter(tree["len"], tree["score"])
# plt.scatter(star["len"], star["score"])
plt.show()


