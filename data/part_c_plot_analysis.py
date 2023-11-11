import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
plt.style.use('seaborn-deep')

def plot(tree_used, tree_not_used, fanout, team_info, pdf_pages):
    plt.bar(list(tree_used.keys()), tree_used.values(),
            color='b', label='With B+Tree')
    plt.bar(list(tree_not_used.keys()), tree_not_used.values(), color='y', label='Without B+Tree')
    plt.legend(loc='upper left')
    plt.xlabel('Number of Block Accesses')
    plt.ylabel('Count')
    plt.title(f'Distribution of Block Accesses needed for FANOUT = {fanout}')
    pdf_pages.savefig()
    plt.close()

if __name__ == "__main__":
    fanout_values = [3, 5, 6, 7, 12]
    team_info = "Part C Analysis, Team=25"
    pdf_filename = 'Part_C_Analysis.pdf'

    # Delete the existing PDF file if it exists
    if os.path.exists(pdf_filename):
        os.remove(pdf_filename)

    with PdfPages(pdf_filename) as pdf_pages:
        for fanout in fanout_values:
            tree_used = {}
            tree_not_used = {}
            fname = f"fanout_{fanout}_block_Access_Count.txt"
            with open(fname) as file:
                for line in file:
                    baTree, baHeap = line.split()
                    if baTree not in tree_used.keys():
                        tree_used[baTree] = 0
                    if baHeap not in tree_not_used.keys():
                        tree_not_used[baHeap] = 0

                    tree_used[baTree] = tree_used[baTree] + 1
                    tree_not_used[baHeap] = tree_not_used[baHeap] + 1

                tree_used = dict(sorted(tree_used.items()))
                tree_not_used = dict(sorted(tree_not_used.items()))

                print(tree_used)
                print(tree_not_used)
                plot(tree_used, tree_not_used, fanout, team_info, pdf_pages)

        # Print the last line in large, bold, and underlined yellow text
print("\033[1;33;40m\033[4;1m\033[6m See Part_C_Analysis.pdf for result\033[0m")
