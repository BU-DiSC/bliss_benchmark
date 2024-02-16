#!/usr/bin/env python

import argparse
import random
from infra.db import BlissDB
from infra.pybliss import BlissRunner
from infra.pybods import PyBods

NUM_EXPS = 1000
K_CHOICES = [1, 2, 5, 10, 25, 50]
L_CHOICES = [1, 2, 5, 10, 25, 50]
INDEXES = ["alex"]


def main(args):
    bliss = BlissRunner(
        bliss_execute_path=args.bliss,
        smoke_test=args.smoke_test,
    )
    _ = PyBods(bods_execute_path=args.bods, smoke_test=args.smoke_test)
    db = BlissDB(args.result_db)

    for _ in range(NUM_EXPS if not args.smoke_test else 10):
        # Generate data OR check that we already have this workload file
        k_pt, l_pt = random.choice(K_CHOICES), random.choice(L_CHOICES)
        index = random.choice(INDEXES)

        # TODO: Data pathway code

        # Run a single benchmark
        result = bliss.run_single_bliss_bench(
            data_file="data/example.data",
            index=index,
            preload_factor=0.5,  # TODO: Decide on how we want to pick these
            write_factor=0.25,
            read_factor=0.25,
            mixed_ratio=0.5,
            seed=0,
        )

        db.log_row(
            k_pt=k_pt,
            l_pt=l_pt,
            index=index,
            preload_time=result[0],
            write_time=result[1],
            mixed_time=result[2],
            read_time=result[3],
        )

    db.display_last_rows(10)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # parser.add_argument(
    #     "--bods",
    #     type=str,
    #     required=True,
    #     help="path to bods executable",
    # )
    parser.add_argument(
        "--bliss",
        type=str,
        required=True,
        help="path to bliss executable",
    )
    parser.add_argument(
        "--result_db",
        type=str,
        required=False,
        help="path to results database",
        default="./results.db",
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
