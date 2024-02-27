#!/usr/bin/env python
import os
import argparse
from infra.pybods import PyBods

# K_CHOICES = [1, 3, 5, 10, 25, 50]
K_CHOICES = [1, 3, 5, 10, 25, 50]
L_CHOICES = [100]
# SPECIAL_KL = ((0, 0), (100, 100))

NUM_KEYS = 500_000_000
DOMAIN_SIZE = 500_000_000
WINDOW = 1
ALPHA = 1
BETA = 1
PAYLOAD = 0
SEED = 0
BINARY_FILE_FORMAT = True

def create_file_str(
    num_entry: int,
    k: float,
    l: float,
    binary: bool = False,
) -> str:
    file_str = (
        f"data_N{num_entry}"
        f"_k{k}"
        f"_l{l}"
        ".bin" if binary else ".txt"
    )

    return file_str


def main(args):
    bods = PyBods(bods_execute_path=args.bods, smoke_test=args.smoke_test)

    kl_vals = ((k_pt, l_pt) for k_pt in K_CHOICES for l_pt in L_CHOICES)

    for k_pt, l_pt in kl_vals:
        file_name = create_file_str(
            num_entry=NUM_KEYS,
            k=k_pt,
            l=l_pt,
            binary=BINARY_FILE_FORMAT
        )
        results = bods.gen_data(
            output_file=os.path.join(args.output_folder, file_name),
            num_entry=NUM_KEYS,
            k=k_pt,
            l=l_pt,
            alpha=ALPHA,
            beta=BETA,
            domain=DOMAIN_SIZE,
            window_size=WINDOW,
            payload=PAYLOAD,
            seed=SEED,
            binary_file_format=BINARY_FILE_FORMAT,
        )
        print(results)

    # for k_pt, l_pt in SPECIAL_KL:
    #     file_name = create_file_str(
    #         num_entry=NUM_KEYS,
    #         k=k_pt,
    #         l=l_pt,
    #         binary=BINARY_FILE_FORMAT
    #     )
    #     results = bods.gen_data(
    #         output_file=os.path.join(args.output_folder, file_name),
    #         num_entry=NUM_KEYS,
    #         k=k_pt,
    #         l=l_pt,
    #         alpha=ALPHA,
    #         beta=BETA,
    #         domain=DOMAIN_SIZE,
    #         window_size=WINDOW,
    #         payload=PAYLOAD,
    #         seed=SEED,
    #         binary_file_format=BINARY_FILE_FORMAT,
    #     )
    #     print(results)


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
