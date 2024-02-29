from dataclasses import dataclass
import subprocess
from typing import Optional


@dataclass
class BodsArgs:
    output_file: str
    total_entries: int
    k_pt: float
    l_pt: float
    alpha: int = 1
    beta: int = 1
    domain: Optional[int] = None
    window_size: int = 1
    payload: int = 0
    seed: int = 0
    start_idx: int = 0
    fixed: bool = False
    binary_file_format: bool = False


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
        args: BodsArgs,
    ) -> str:
        if self.smoke_test:
            return "Smoke test"

        cmd = [
            self.bods_execute_path,
            f"--output_file {args.output_file}",
            f"--total_entries {args.total_entries}",
            f"--k_pt {args.k_pt}",
            f"--l_pt {args.l_pt}",
            f"--alpha {args.alpha}",
            f"--beta {args.beta}",
            f"--domain {args.domain if args.domain is not None else args.total_entries}",
            f"--window {args.window_size}",
            f"--payload {args.payload}",
            f"--start {args.start_idx}",
            f"--seed {args.seed}",
        ]
        if args.binary_file_format:
            cmd.append("--binary")
        if args.fixed:
            cmd.append("--fixed")
        print(" ".join(cmd))
        process = subprocess.Popen(
            " ".join(cmd),
            stdout=subprocess.PIPE,
            universal_newlines=True,
            shell=True,
        )
        res, _ = process.communicate()

        return res
