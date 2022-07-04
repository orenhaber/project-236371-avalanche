import os
import sys
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MaxNLocator
import pandas as pd


def get_files(how_many):
    return [f"workers_results/worker-envars-fieldref-statefulset-{i}_res.csv" for i in range(int(how_many))]


def parse_data(results_dict, f, times):
    first_row = f.readline()
    times.append(int(first_row))
    for line in f.readlines()[1:]:
        round_number, accepted_color = line.strip().split(",")
        results_dict[str(int(round_number)+1)][accepted_color] += 1


def plot(results_dict, how_many):
    #import pdb;pdb.set_trace()

    df = pd.DataFrame.from_dict(results_dict, orient="index")
    print(df.index)
    ax = df.plot(kind="bar")
    ax.set_xlabel("round_number")
    ax.set_ylabel("number of acceptors")
    plt.legend(loc='upper left')
    ax.yaxis.set_major_locator(MaxNLocator(integer=True))
    ax.set_ylim((0, how_many + 10))
    ax.set_xticklabels(ax.get_xticks(), rotation=0)

    plt.savefig('data.png')
    plt.show()
    plt.close()


def build_dict(results_dict, number_of_rounds):
    for i in range(int(number_of_rounds)):
        results_dict[str(i+1)] = {"0":0, "1":0}


def main():
    results_dict = {}
    how_many = int(sys.argv[1])
    number_of_rounds = int(sys.argv[2])
    build_dict(results_dict, number_of_rounds)
    files = get_files(how_many)
    times = []
    counter = 0
    for output_file in files:
        if os.stat(output_file).st_size == 0:
            counter += 1
            continue
        with open(output_file) as f:
            parse_data(results_dict, f, times)
    print(results_dict)
    total_dis_agree = 0
    for round in results_dict.values():
        total_dis_agree += min(round.values())
    perc = (total_dis_agree * 100) / ((how_many-counter)*number_of_rounds)
    print(f"dis agree {perc}%")
    # plot(results_dict, int(how_many))
    df = pd.DataFrame(times)
    print(f"the avg is :{df.apply(np.average)}")


if __name__ == '__main__':
    main()
