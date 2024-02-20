import re
import subprocess
import random
from typing import Tuple


class PyBliss:
    def __init__(
        self,
        bliss_execute_path: str,
        smoke_test: bool = False,
    ) -> None:
        self.bliss_execute_path = bliss_execute_path
        self.smoke_test = smoke_test
        self.preload_time_regex = re.compile(
            r"\[[0-9 :.-]+\] \[info\] Preload Time \(ns\): (\d+)"
        )
        self.write_time_regex = re.compile(
            r"\[[0-9 :.-]+\] \[info\] Write Time \(ns\): (\d+)"
        )
        self.mixed_time_regex = re.compile(
            r"\[[0-9 :.-]+\] \[info\] Mix Time \(ns\): (\d+)"
        )
        self.read_time_regex = re.compile(
            r"\[[0-9 :.-]+\] \[info\] Read Time \(ns\): (\d+)"
        )

    def run_single_bliss_bench(
        self,
        data_file: str,
        index: str,
        preload_factor: float,
        write_factor: float,
        read_factor: float,
        mixed_ratio: float,
        seed: int = 0,
    ) -> Tuple[str, str, str, str]:
        if self.smoke_test:
            res = tuple(str(random.randint(0, 1000)) for _ in range(4))
            return (res[0], res[1], res[2], res[3])

        cmd = [
            self.bliss_execute_path,
            f"--data_file {data_file}",
            f"--index {index}",
            f"--preload_factor {preload_factor}",
            f"--write_factor {write_factor}",
            f"--read_factor {read_factor}",
            f"--mixed_read_write_ratio {mixed_ratio}",
            f"--seed {seed}",
        ]
        process = subprocess.Popen(
            " ".join(cmd),
            stdout=subprocess.PIPE,
            universal_newlines=True,
            shell=True,
        )
        assert process.stdout is not None
        proc_results, _ = process.communicate()

        preload_time = self.preload_time_regex.search(proc_results)
        preload_time = preload_time.group(1) if preload_time else "0"

        write_time = self.write_time_regex.search(proc_results)
        write_time = write_time.group(1) if write_time else "0"

        mixed_time = self.mixed_time_regex.search(proc_results)
        mixed_time = mixed_time.group(1) if mixed_time else "0"

        read_time = self.read_time_regex.search(proc_results)
        read_time = read_time.group(1) if read_time else "0"

        return (preload_time, write_time, mixed_time, read_time)
