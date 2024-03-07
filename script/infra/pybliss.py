from dataclasses import dataclass
import re
import logging
import subprocess
import random


@dataclass
class BlissArgs:
    data_file: str
    index_type: str
    preload_factor: float
    write_factor: float
    read_factor: float
    mixed_ratio: float
    seed: int = 0
    file_type: str = "binary"
    use_preload: bool = False


@dataclass
class BlissStats:
    preload_time: int
    preload_creation_time: int
    write_time: int
    read_time: int
    mixed_time: int


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
        self.preload_creation_time_regex = re.compile(
            r"\[[0-9 :.-]+\] \[info\] Preload Creation Time \(ns\): (\d+)"
        )

    def run_single_bliss_bench(self, args: BlissArgs) -> BlissStats:
        if self.smoke_test:
            return BlissStats(*(random.randint(0, 2 << 16) for _ in range(5)))

        cmd = [
            self.bliss_execute_path,
            f"--data_file {args.data_file}",
            f"--index {args.index_type}",
            f"--preload_factor {args.preload_factor}",
            f"--write_factor {args.write_factor}",
            f"--read_factor {args.read_factor}",
            f"--mixed_read_write_ratio {args.mixed_ratio}",
            f"--seed {args.seed}",
            f"--file_type {'binary' if args.file_type else 'txt'}",
            "--use_preload" if args.use_preload else "",
        ]
        process = subprocess.Popen(
            " ".join(cmd),
            stdout=subprocess.PIPE,
            universal_newlines=True,
            shell=True,
        )
        assert process.stdout is not None
        proc_results, _ = process.communicate()
        logging.debug(f"{proc_results}")

        preload_time = self.preload_time_regex.search(proc_results)
        preload_time = int(preload_time.group(1)) if preload_time else 0

        preload_creation = self.preload_creation_time_regex.search(proc_results)
        preload_creation = int(preload_creation.group(1)) if preload_creation else 0

        write_time = self.write_time_regex.search(proc_results)
        write_time = int(write_time.group(1)) if write_time else 0

        mixed_time = self.mixed_time_regex.search(proc_results)
        mixed_time = int(mixed_time.group(1)) if mixed_time else 0

        read_time = self.read_time_regex.search(proc_results)
        read_time = int(read_time.group(1)) if read_time else 0

        return BlissStats(
            preload_time=preload_time,
            preload_creation_time=preload_creation,
            write_time=write_time,
            read_time=read_time,
            mixed_time=mixed_time,
        )
