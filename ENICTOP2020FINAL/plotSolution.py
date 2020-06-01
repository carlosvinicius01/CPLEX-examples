import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random

print("a")

f = open("solutions/solucoes.txt", 'r')

num_slots = 12

acabou_flag = False

line_count = 1
session_list = []
slots_list = []

for line in f:
    if line != '\n':
        slots_list.append(line)

    if line_count % num_slots == 0 and line_count > 1:
        session_list.append(slots_list)
        slots_list = []

    line_count = line_count + 1

f.close()

# print(session_list)

session_index = 0
for session in session_list:
    color_dict = {}        
    av_dict = {}
    orient_dict = {}
        
    fig = plt.figure()

    y_pos = 0
    
    ax = fig.add_subplot(111, aspect='equal')

    for slot in session:
        orient = slot.split(" ")[0]
        av1 = slot.split(" ")[1]
        av2 = slot.split(" ")[2]

        if not (orient in orient_dict):
            orient_dict[orient] = 1
        else:
            orient_dict[orient] = orient_dict[orient] + 1

        x_pos = 0

        
        for prof in slot.split(" "):
            if prof == "\n":
                continue

            if not (prof in av_dict):
                av_dict[prof] = 1
            else:
                av_dict[prof] = av_dict[prof] + 1

            if not (prof in color_dict):
                color_dict[prof] = (random.uniform(0, 1), random.uniform(0, 1), random.uniform(0, 1))
            
            ax.add_patch(patches.Rectangle((x_pos, y_pos), 1,1, color=color_dict[prof], edgecolor = (0,0,0)))
            x_pos = x_pos + 1
        y_pos = y_pos + 1

    prof_patch_list = []

    for k,v in color_dict.items():
        prof_patch = patches.Patch(color=v, label= "Orientações: " + str(orient_dict[k]) + ", Avaliações: " + str(av_dict[k]))
        prof_patch_list.append(prof_patch)

    plt.legend(bbox_to_anchor=(1.05,1),loc='upper left', borderaxespad=0.,handles=prof_patch_list)
    
    plt.ylim((0,num_slots))
    plt.xlim((0,3))
    # plt.show()
    plt.savefig("plotted_solutions/" + str(num_slots) + "_" + str(session_index) + ".png")

    session_index = session_index + 1


