#!/usr/bin/env python
import os
import argparse
from infra.pybods import PyBods, BodsArgs

# K_CHOICES = [1, 3, 5, 10, 25, 50, 100]
# L_CHOICES = [1, 3, 5, 10, 25, 50, 100]
# SPECIAL_KL = [(0, 0), (100, 100)]
K_CHOICES = [3, 5, 10]
L_CHOICES = [10]
SPECIAL_KL = []

NUM_KEYS = 500_000_000
BINARY_FILE_FORMAT = True


def main(args: argparse.Namespace):
    bods = PyBods(bods_execute_path=args.bods, smoke_test=args.smoke_test)

    kl_vals = ((k_pt, l_pt) for k_pt in K_CHOICES for l_pt in L_CHOICES)
    create_files = os.listdir(args.output_folder)
    for k_pt, l_pt in list(kl_vals) + SPECIAL_KL:
        file_name = f"data_N{NUM_KEYS}_k{k_pt}_l{l_pt}{'.bin' if BINARY_FILE_FORMAT else '.txt'}"
        if file_name in create_files:
            print(f"File {file_name} already created, skipping")
            continue
        print(f"Creating {file_name}")
        bods_args = BodsArgs(
            output_file=os.path.join(args.output_folder, file_name),
            total_entries=NUM_KEYS,
            k_pt=k_pt,
            l_pt=l_pt,
            binary_file_format=BINARY_FILE_FORMAT,
            seed=2169,
        )
        results = bods.gen_data(bods_args)
        print(results)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--bods",
        type=str,
        required=True,
        help="path to bods executable",
    )
    parser.add_argument(
        "--smoke_test",
        action="store_true",
        default=False,
        help="Smoke test",
    )
    parser.add_argument(
        "--output_folder",
        type=str,
        required=False,
        default="./data",
        help="Directory to output data",
    )

    args = parser.parse_args()
    main(args)

    pass
