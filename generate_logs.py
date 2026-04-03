import os

target_size = 3.6 * 1024 * 1024 * 1024  # 3.6 GB
line = "[ERROR] System failure at node 0x4F\n"
line_bytes = line.encode()
chunk = line_bytes * 100000  # Write in large chunks for speed

print("Generating 3.6GB log file... please wait.")
with open("data.txt", "wb") as f:
    written = 0
    while written < target_size:
        f.write(chunk)
        written += len(chunk)
print("Done. data.txt is now 3.6GB.")
