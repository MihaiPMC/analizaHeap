import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from glob import glob

def read_csv_data(file_path):
    with open(file_path, 'r') as f:
        first_line = f.readline().strip().split(',')
        test_name = first_line[1]

    skiplist_insert = 0
    skiplist_search = 0
    skiplist_delete = 0
    treap_insert = 0
    treap_search = 0
    treap_delete = 0
    
    with open(file_path, 'r') as f:
        lines = f.readlines()
        
        header_index = -1
        for i, line in enumerate(lines):
            if line.startswith("Operation"):
                header_index = i
                break
        
        if header_index == -1:
            raise ValueError("Nu s-a găsit header-ul pentru operații în fișier")
        
        insert_line = lines[header_index + 1].strip().split(',')
        search_line = lines[header_index + 2].strip().split(',')
        delete_line = lines[header_index + 3].strip().split(',')
        
        skiplist_insert = float(insert_line[1])
        skiplist_search = float(search_line[1])
        skiplist_delete = float(delete_line[1])
        
        treap_insert = float(insert_line[3])
        treap_search = float(search_line[3])
        treap_delete = float(delete_line[3])
    
    return {
        'test_name': test_name,
        'operations': {
            'Insert': {'skiplist': skiplist_insert, 'treap': treap_insert},
            'Search': {'skiplist': skiplist_search, 'treap': treap_search},
            'Delete': {'skiplist': skiplist_delete, 'treap': treap_delete}
        }
    }

def generate_combined_chart_for_file(file_data, output_dir="charts"):
    os.makedirs(output_dir, exist_ok=True)
    
    test_name = file_data['test_name']
    operations = ['Insert', 'Search', 'Delete']
    
    skiplist_values = [file_data['operations'][op]['skiplist'] for op in operations]
    treap_values = [file_data['operations'][op]['treap'] for op in operations]
    
    fig, ax = plt.subplots(figsize=(12, 8))
    
    bar_width = 0.35
    x = np.arange(len(operations))
    
    rects1 = ax.bar(x - bar_width/2, skiplist_values, bar_width, label='SkipList', color='blue', alpha=0.7)
    rects2 = ax.bar(x + bar_width/2, treap_values, bar_width, label='Treap', color='green', alpha=0.7)
    
    ax.set_title(f'Comparison of SkipList vs Treap for {test_name}')
    ax.set_ylabel('Time (ms)')
    ax.set_xticks(x)
    ax.set_xticklabels(operations)
    ax.legend()
    
    def add_labels(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate(f'{height:.2f}',
                        xy=(rect.get_x() + rect.get_width() / 2, height),
                        xytext=(0, 3),
                        textcoords="offset points",
                        ha='center', va='bottom', fontsize=9)
    
    add_labels(rects1)
    add_labels(rects2)
    
    plt.tight_layout()
    
    plt.savefig(f"{output_dir}/{test_name}_comparison.png", dpi=300)
    plt.close()
    
    print(f"Grafic generat pentru testul {test_name}")

def generate_summary_total_chart(summary_file, output_dir="charts"):
    os.makedirs(output_dir, exist_ok=True)
    
    df = pd.read_csv(summary_file)
    
    test_names = df.iloc[:, 0].tolist()
    
    skiplist_total = df['SkipList Total (ms)'].tolist()
    treap_total = df['Treap Total (ms)'].tolist()
    
    bar_width = 0.35
    
    x = np.arange(len(test_names))
    
    plt.figure(figsize=(14, 8))
    
    plt.bar(x - bar_width/2, skiplist_total, bar_width, label='SkipList', color='blue', alpha=0.7)
    plt.bar(x + bar_width/2, treap_total, bar_width, label='Treap', color='green', alpha=0.7)
    
    plt.xlabel('Test Case')
    plt.ylabel('Time (ms)')
    plt.title('Comparison of SkipList vs Treap for Total Execution Time (Summary)')
    plt.xticks(x, test_names, rotation=45, ha='right')
    plt.legend()
    
    for i, v in enumerate(skiplist_total):
        if v > 0.1:
            plt.text(i - bar_width/2, v * 1.02, f'{v:.2f}', ha='center', va='bottom', fontsize=8)
    
    for i, v in enumerate(treap_total):
        if v > 0.1:
            plt.text(i + bar_width/2, v * 1.02, f'{v:.2f}', ha='center', va='bottom', fontsize=8)
    
    plt.tight_layout()
    
    plt.savefig(f"{output_dir}/summary_total_comparison.png", dpi=300)
    plt.close()
    
    print("Grafic de sumar generat pentru timpul total de execuție")

def main():
    os.makedirs("charts", exist_ok=True)
    
    summary_file = 'results/summary.csv'
    if os.path.exists(summary_file):
        generate_summary_total_chart(summary_file)
    else:
        print(f"Fișierul {summary_file} nu a fost găsit!")
    
    csv_files = glob('results/*.csv')
    csv_files = [f for f in csv_files if 'summary' not in f]
    
    if not csv_files:
        print("Nu au fost găsite fișiere CSV pentru procesare (excluzând summary.csv).")
        return
    
    for csv_file in csv_files:
        try:
            file_data = read_csv_data(csv_file)
            generate_combined_chart_for_file(file_data)
        except Exception as e:
            print(f"Eroare la procesarea fișierului {csv_file}: {e}")
    
    print("Toate graficele au fost generate în directorul 'charts'")

if __name__ == "__main__":
    main()