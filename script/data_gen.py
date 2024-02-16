#!/usr/bin/env python

import argparse
from infra.pybods import PyBods

K_CHOICES = [1, 3, 5, 10, 25, 50]
L_CHOICES = [1, 3, 5, 10, 25, 50]
SPECIAL_KL = ((0, 0), (100, 100))

NUM_KEYS = 500_000_000
# NUM_KEYS = 1_000
DOMAIN_SIZE = 1_000_000_000
WINDOW = 2
ALPHA = 1
BETA = 1
PAYLOAD = 0
SEED = 0


def main(args):
    bods = PyBods(bods_execute_path=args.bods, smoke_test=args.smoke_test)

    kl_vals = ((k_pt, l_pt) for k_pt in K_CHOICES for l_pt in L_CHOICES)

    for k_pt, l_pt in kl_vals:
        results = bods.gen_data(
            output_file=f"data/data_k{k_pt}_l{l_pt}.data",
            num_entry=NUM_KEYS,
            k=k_pt,
            l=l_pt,
            alpha=ALPHA,
            beta=BETA,
            domain=DOMAIN_SIZE,
            window_size=WINDOW,
            payload=PAYLOAD,
            seed=SEED,
        )
        print(results)

    for k_pt, l_pt in SPECIAL_KL:
        results = bods.gen_data(
            output_file=f"data/data_k{k_pt}_l{l_pt}.data",
            num_entry=NUM_KEYS,
            k=k_pt,
            l=l_pt,
            alpha=ALPHA,
            beta=BETA,
            domain=DOMAIN_SIZE,
            window_size=WINDOW,
            payload=PAYLOAD,
            seed=SEED,
        )
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

    args = parser.parse_args()
    main(args)

    pass
