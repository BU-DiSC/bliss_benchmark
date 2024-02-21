#!/usr/bin/env python

import re
import os
import argparse
import logging
from infra.db import BlissDB
from infra.pybliss import PyBliss

K_CHOICES = [1, 3, 5, 10, 25, 50]
L_CHOICES = [1, 3, 5, 10, 25, 50]
SPECIAL_KL = ((0, 0), (100, 100))
INDEXES = ["alex"]

def get_file_params(file_name):
    file_regex_str = r"data_N(\d+)_k(\d+)_l(\d+)\.bin"
    res = re.search(file_regex_str, file_name)
    if res is None:
        raise LookupError
    num_keys, k_pt, l_pt = res.groups()

    return int(num_keys), float(k_pt), float(l_pt)


def main(args):
    bliss = PyBliss(
        bliss_execute_path=args.bliss,
        smoke_test=args.smoke_test,
    )
    db = BlissDB(args.result_db)
    files = os.listdir(args.data_folder)
    exp_pairs = ((file, index) for file in files for index in INDEXES)

    for file, index in exp_pairs:
        _, k_pt, l_pt = get_file_params(file)
        logging.info("Running bliss ({}, {})", index, file)
        result = bliss.run_single_bliss_bench(
            data_file=os.path.join(args.data_folder, file),
            index=index,
            preload_factor=0.5,  # TODO: Decide on how we want to pick these
            write_factor=0.25,
            read_factor=0.25,
            mixed_ratio=0.5,
            seed=0,
            binary=True,
        )
        db.log_row(
            file_name=file,
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
    parser.add_argument(
        "--bliss",
        type=str,
        required=True,
        help="path to bliss executable",
    )
    parser.add_argument(
        "--data_folder",
        type=str,
        required=True,
        help="Folder containing workload"
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
