import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import re

def read_data(filename, delimiter):
    try:
        data = np.loadtxt(filename, delimiter=delimiter)
    except ValueError:
        data = np.loadtxt(filename)
    return data

# 设置画图开关
file_num = 3  # 画地图文件，输入地图文件数目
draw_estimate_pose = 0  # 画定位pose
draw_obs = 0  # 画障碍物轮廓
draw_trajectory = 1  # 画实际工作路径
draw_bow_line = 0  # 画规划的弓形线
print_data1 = 0; data1_divider = ' '
print_data2 = 0; data2_divider = ' '
print_data3 = 0; data3_divider = ' '
print_data4 = 0; data4_divider = ' '
print_data5 = 0; data5_divider = ' '
print_data6 = 0; data6_divider = ' '
print_data7 = 0; data7_divider = ' '
print_data8 = 0; data8_divider = ' '
print_data9 = 0; data9_divider = ' '
print_data10 = 0; data10_divider = ' '

# 设置图形
plt.figure(1)
plt.grid(True)
plt.axis('equal')

# 画点
if print_data1 == 1:
    data1 = read_data('data/data1.txt', data1_divider)
    plt.scatter(data1[:, 0], data1[:, 1], facecolor='r', edgecolor='r')

if print_data2 == 1:
    data2 = read_data('data/data2.txt', data2_divider)
    plt.scatter(data2[:, 0], data2[:, 1], facecolor='b', edgecolor='b')

if print_data3 == 1:
    data3 = read_data('data/data3.txt', data3_divider)
    plt.scatter(data3[:, 0], data3[:, 1], facecolor='r', edgecolor='r')

if print_data4 == 1:
    data4 = read_data('data/data4.txt', data4_divider)
    plt.scatter(data4[:, 0], data4[:, 1], facecolor='g', edgecolor='g')

if print_data5 == 1:
    data5 = read_data('data/data5.txt', data5_divider)
    plt.scatter(data5[:, 0], data5[:, 1], facecolor='k', edgecolor='k')

# 画折线
if print_data6 == 1:
    data6 = read_data('data/data6.txt', data6_divider)
    plt.plot(data6[:, 0], data6[:, 1], '--ob')

if print_data7 == 1:
    data7 = read_data('data/data7.txt', data7_divider)
    plt.plot(data7[:, 0], data7[:, 1], '--og')

if print_data8 == 1:
    data8 = read_data('data/data8.txt', data8_divider)
    plt.plot(data8[:, 0], data8[:, 1], '--og')

if print_data9 == 1:
    data9 = read_data('data/data9.txt', data9_divider)
    plt.plot(data9[:, 0], data9[:, 1], '--og')

if print_data10 == 1:
    data10 = read_data('data/data10.txt', data10_divider)
    plt.plot(data10[:, 0], data10[:, 1], '--og')

colors = [
    'red',  'blue', 'orange', 'purple',
    'cyan', 'magenta', 'yellow', 'black', 'gray',
    'red',  'blue', 'orange', 'purple',
    'cyan', 'magenta', 'yellow', 'black', 'gray'
    ]
# 确保颜色列表至少和文件数量一样长
assert len(colors) >= file_num, "颜色列表长度不足以为每个文件指定一个颜色"

# 画地图文件
for i in range(1, file_num + 1):
    file_name = f"data/{i}.txt"
    region = np.loadtxt(file_name)

    # 选择颜色
    color = colors[i - 1]  # 注意：列表索引从0开始，而文件索引从1开始
    plt.plot(region[:, 0], region[:, 1], linestyle='--', marker='o', color=color, markersize=3)

# 画定位pose
if draw_estimate_pose == 1:
    with open('data/pose.txt', 'r') as file:
        pose = []
        for line in file:
            match = re.match(r't:\s*(-?\d+\.\d+)\s*(-?\d+\.\d+)\s*(-?\d+\.\d+)', line)
            if match:
                pose.append([float(match.group(1)), float(match.group(2)), float(match.group(3))])
    pose = np.array(pose)
    blockSize = 10000
    numBlocks = int(np.ceil(pose.shape[0] / blockSize))
    for i in range(numBlocks):
        startIndex = i * blockSize
        endIndex = min((i + 1) * blockSize, pose.shape[0])
        blockData = pose[startIndex:endIndex, :]
        plt.plot(blockData[:, 0], blockData[:, 1], '--og')

# 画障碍物轮廓
if draw_obs == 1:
    obs = np.loadtxt('data/obs.txt', delimiter=',')
    x_obs_polygon1 = obs[:, 0]
    y_obs_polygon1 = obs[:, 1]
    x_obs_polygon0 = x_obs_polygon1[0]
    y_obs_polygon0 = y_obs_polygon1[0]
    for i in range(len(x_obs_polygon1) - 1):
        if x_obs_polygon1[i] == 9999.0 and i < len(x_obs_polygon1):
            x_obs_polygon0 = x_obs_polygon1[i + 1]
            y_obs_polygon0 = y_obs_polygon1[i + 1]
            continue
        if x_obs_polygon1[i + 1] != 9999.0:
            plt.plot([x_obs_polygon1[i], x_obs_polygon1[i + 1]], [y_obs_polygon1[i], y_obs_polygon1[i + 1]], 'r')
        elif x_obs_polygon1[i + 1] == 9999.0:
            plt.plot([x_obs_polygon1[i], x_obs_polygon0], [y_obs_polygon1[i], y_obs_polygon0], 'r')

# 解析路径
if draw_trajectory == 1:
    with open('data/trajectory.txt', 'r') as file:
        lines = file.readlines()[1:]
        trajectory = np.array([list(map(float, line.split())) for line in lines])
        print(trajectory)
    # plt.scatter(trajectory[:, 0], trajectory[:, 1], color='green', marker='o',s=10)


    # 创建一个空的散点图对象
    scatter = plt.scatter([], [], color='red', marker='o', s=10)

    # 存储更新的点
    points = []

    # 存储更新的点的最大数量
    max_points = 2000  # 可以根据需要调整显示的点的数量

    # 初始化函数：仅设置散点图数据为空
    def init():
        scatter.set_offsets([])
        scatter.set_color(['red'])  # 初始化时设置第一个点为红色
        scatter.set_sizes([10])       # 初始化大小
        return scatter,

    # def update(frame):
    #     step = 10
    #     end = min(frame + step, len(trajectory))
    #     new_points = trajectory[frame:end]
    #     points.extend(new_points)

    #     # 更新散点图的数据
    #     scatter.set_offsets(points)

    #     # 更新颜色数组，确保最后一个点始终为红色，其他点为绿色
    #     colors = ['green'] * (len(points) - 1) + ['red']
    #     scatter.set_color(colors)

    #     # 更新大小数组，最后一个点的大小不同
    #     sizes = [10] * (len(points) - 1) + [50]
    #     scatter.set_sizes(sizes)

    #     return scatter,

    # # 创建动画
    # ani = FuncAnimation(plt.gcf(), update, frames=np.arange(0, len(trajectory), 10),
    #                     init_func=init, blit=True, interval=50, repeat=False)


    def update(frame):
        global points  # 声明 points 为全局变量
        step = 20
        end = min(frame + step, len(trajectory))
        new_points = trajectory[frame:end]
        # 将新点添加到点列表中，并保持点列表不超过最大点数
        points = (points[-max_points:] if len(points) > max_points else points) + list(new_points)

        # 更新散点图的数据
        scatter.set_offsets(points)

        # 更新颜色数组，确保最后一个点始终为红色，其他点为绿色
        colors = ['green'] * (len(points) - 1) + ['red']
        scatter.set_color(colors)

        # 更新大小数组，最后一个点的大小不同
        sizes = [10] * (len(points) - 1) + [50]
        scatter.set_sizes(sizes)

        return scatter,

    # 创建动画
    ani = FuncAnimation(plt.gcf(), update, frames=np.arange(0, len(trajectory), 20),
                        init_func=init, blit=True, interval=20, repeat=False)



# 画弓形线
if draw_bow_line == 1:
    with open('data/line.txt', 'r') as file:
        data = []
        split_idx = []
        for line in file:
            if '----' not in line:
                current_data = list(map(float, line.split()))
                data.append(current_data)
            else:
                split_idx.append(len(data))
    start_idx = 0
    for end_idx in split_idx:
        current_data = np.array(data[start_idx:end_idx])
        plt.plot(current_data[:, 0], current_data[:, 1], 'o-')
        start_idx = end_idx
    data_array = np.array(data)
    plt.scatter(data_array[0, 0], data_array[0, 1], s=100, c='g', marker='o')  # 首数据点，绿色
    plt.scatter(data_array[-1, 0], data_array[-1, 1], s=100, c='r', marker='o')  # 尾数据点，红色

plt.show()