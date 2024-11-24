import pandas as pd


data = {"Algorithm": [], "Threads": [], "Runtime (seconds)": []}
file_path = "./results-m2.txt"
with open(file_path, "r") as file:
    lines = file.readlines()

for line in lines:
    if "," in line:  # Lines containing runtime data
        parts = line.strip().split(",")
        if len(parts) == 3:
            algorithm = parts[0]
            threads = int(parts[1])
            runtime = float(parts[2])
            data["Algorithm"].append(algorithm)
            data["Threads"].append(threads)
            data["Runtime (seconds)"].append(runtime)


df = pd.DataFrame(data)
print(df)

output_file_path = "./runtime_m2.csv"
df.to_csv(output_file_path, index=False)
print(f"Data saved to {output_file_path}")
