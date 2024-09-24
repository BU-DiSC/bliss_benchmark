#!/usr/bin/env python

import os
import argparse
import logging
from infra.db import BlissDB
from infra.pybliss import BlissArgs, PyBliss
from infra.util import get_file_params

INDEXES = ["btree", "art"]
PRELOAD_FACTOR = 0.4
WRITE_FACTOR = 0.4
READ_FACTOR = 0.2
MIXED_RATIO = 0.5
PRELOAD = False


def main(args):
    bliss = PyBliss(args.bliss, args.smoke_test)
    db = BlissDB(args.result_db)
    files = os.listdir(args.data_folder)
    kwargs = {
        "preload_factor": PRELOAD_FACTOR,
        "write_factor": WRITE_FACTOR,
        "read_factor": READ_FACTOR,
        "mixed_ratio": MIXED_RATIO,
        "file_type": "binary",
        "seed": 0,
        "use_preload": PRELOAD,
    }

    exp_pairs = ((file, index) for file in files for index in INDEXES)
    for file, index in exp_pairs:
        _, k_pt, l_pt = get_file_params(file)
        logging.info(f"Running bliss ({index}, {file})")
        bliss_args = BlissArgs(
            data_file=os.path.join(args.data_folder, file), index_type=index, **kwargs
        )
        logging.debug(f"BlissArgs: {bliss_args}")
        stats = bliss.run_single_bliss_bench(bliss_args)
        db.log_row(k_pt, l_pt, bliss_args, stats)

    for row in db.get_last_rows(10):
        logging.info(f"Row: {row}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--bliss",
        type=str,
        required=True,
        help="path to bliss executable",
    )
    parser.add_argument(
        "--data_folder", type=str, required=True, help="Folder containing workload"
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
    parser.add_argument(
        "--verbose",
        "-v",
        action="count",
        help="Verbosity logging",
        default=0,
    )

    args = parser.parse_args()
    log_level = logging.WARNING
    if args.verbose == 1:
        log_level = logging.INFO
    elif args.verbose > 1:
        log_level = logging.DEBUG
    logging.basicConfig(
        format="[%(levelname)s][%(asctime)-15s][%(filename)s] %(message)s",
        datefmt="%d-%m-%y:%H:%M:%S",
        level=log_level
    )
    logging.info(f"Logger level: {logging.getLevelName(log_level)}")
    main(args)
