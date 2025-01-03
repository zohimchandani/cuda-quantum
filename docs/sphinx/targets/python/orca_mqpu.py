import cudaq
import numpy as np
import os

# See accompanying example `orca.py` for detailed explanation.

# To use the ORCA Computing target you will need to set the ORCA_ACCESS_URL
# environment variable or pass URL(s).
orca_urls = os.getenv("ORCA_ACCESS_URL",
                      "http://localhost:3035,http://localhost:3037")

cudaq.set_target("orca", url=orca_urls)

qpu_count = cudaq.get_target().num_qpus()
print("Number of virtual QPUs:", qpu_count)

# A time-bin boson sampling experiment
input_state = [1, 0, 1, 0, 1, 0, 1, 0]
loop_lengths = [1, 1]
n_beam_splitters = len(loop_lengths) * len(input_state) - sum(loop_lengths)
bs_angles = np.linspace(np.pi / 3, np.pi / 6, n_beam_splitters)
n_samples = 10000

count_futures = []
for i in range(qpu_count):
    result = cudaq.orca.sample_async(input_state,
                                     loop_lengths,
                                     bs_angles,
                                     n_samples,
                                     qpu_id=i)
    count_futures.append(result)

print("Sampling jobs launched for asynchronous processing.")

for counts in count_futures:
    counts.get().dump()
