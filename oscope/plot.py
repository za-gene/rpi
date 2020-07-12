import matplotlib.pyplot as plt


fp = open("capture.dat", "r")
xs=[]
ys=[]
idx = 0
for line in fp.readlines():
    x, y = line.split('\t')
    xs.append(float(x))
    ys.append(float(y))
fp.close()    

def show():
    plt.plot(xs, ys)    
    plt.show()
show()


