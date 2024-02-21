import subprocess


class PyBods:
    def __init__(
        self,
        bods_execute_path: str,
        smoke_test: bool = False,
    ) -> None:
        self.bods_execute_path = bods_execute_path
        self.smoke_test = smoke_test

    def gen_data(
        self,
        output_file: str,  # file to store data
        num_entry: int,  # total number of entries
        k: float,  # % of out of order entries
        l: float,  # % max displacement of entries
        alpha: float,  # beta dist arg1
        beta: float,  # beta dist arg2
        domain: float,  # right half of key space [0, domain)
        window_size: int,  # size of skips
        payload: int,  # payload in BYTES
        seed: int = 0,  # random seed
        start_idx: int = 0,  # start index
        fixed: bool = False,  # fixed window size
        binary_file_format: bool = False,
    ):
        if self.smoke_test:
            return True

        cmd = [
            self.bods_execute_path,
            f"--output_file {output_file}",
            f"--total_entries {num_entry}",
            f"--k_pt {k}",
            f"--l_pt {l}",
            f"--alpha {alpha}",
            f"--beta {beta}",
            f"--domain {domain}",
            f"--window {window_size}",
            f"--payload {payload}",
            f"--start {start_idx}",
            f"--seed {seed}",
            f"--fixed" if fixed else "",
            f"--binary" if binary_file_format else "",
        ]
        process = subprocess.Popen(
            " ".join(cmd),
            stdout=subprocess.PIPE,
            universal_newlines=True,
            shell=True,
        )
        res, _ = process.communicate()

        return res

