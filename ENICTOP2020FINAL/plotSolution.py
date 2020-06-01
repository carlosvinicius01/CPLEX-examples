import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random

f = open("solutions/solucoes2.txt", 'r')

acabou_flag = False

line_count = 0
session_list = []
slots_list = []

for line in f:
    if line_count % 12 == 0 and line_count > 0:
        session_list.append(slots_list)
        slots_list = []
    else:
        slots_list.append(line)
    line_count = line_count + 1

for session in session_list:
    color_dict = {}
    fig = plt.figure()

    y_pos = 0
    
    ax = fig.add_subplot(111, aspect='equal')


    for slot in session:


    plt.ylim((0,12))
    plt.xlim((0,3))
    plt.show()


# for line in f:
    
#     if i == 0 or line == '\n':
#         color_dict = {}
#         y_pos = 0

#     if(line == '\n'):
#         plt.ylim((0,12))
#         plt.xlim((0,3))
#         plt.show()

#         color_dict = {}
#         fig = plt.figure()
#         ax = fig.add_subplot(111, aspect='equal')
#         y_pos = 0

#     else:  
#         orient = line.split(" ")[0]
#         av1 = line.split(" ")[1]
#         av2 = line.split(" ")[2]
#         x_pos = 0
        
#         for prof in line.split(" "):
#             if not (prof in color_dict):
#                 color_dict[prof] = (random.uniform(0, 1), random.uniform(0, 1), random.uniform(0, 1))
            
#             ax.add_patch(patches.Rectangle((x_pos, y_pos), 1,1, color=color_dict[prof]))
#             x_pos = x_pos + 1
#         y_pos = y_pos + 1




f.close()

# a=[0,0]
# b=[0,1]
# c=[1,0]
# d=[1,1]

# width = c[0] - a[0]
# height = d[1] - a[1]
# lims = (0,12)

# fig1 = plt.figure()
# ax1 = fig1.add_subplot(111,aspect='equal')
# ax1.add_patch(patches.Rectangle((1,1), 1, 1, color=(0.5,1.0,0.7)))
# ax1.add_patch(patches.Rectangle((2,1), 1, 1, color='b'))

# plt.ylim(lims)
# plt.xlim((0,3))

# plt.show()