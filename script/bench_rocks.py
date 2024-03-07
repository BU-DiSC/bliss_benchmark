#!/usr/bin/env python

import argparse
import logging
import os
import re
import sqlite3
import subprocess
from dataclasses import dataclass
from datetime import datetime, timezone
from random import randint


@dataclass
class PyRocksStats:
    input_file: str
    ingestion_time: int
    num_inserts: int
    bytes_written: int
    num_compactions: int
    num_files_trivial: int
    bytes_moved_trivial: int
    trivial_if_cond_accesses: int


class PyRocks:
    def __init__(
        self,
        rocks_executable: str,
        db_path: str,
        smoke_test: bool = False,
    ) -> None:
        self.rocks_executable = rocks_executable
        self.db_con = sqlite3.connect(db_path)
        self.smoke_test = smoke_test
        self.ingestion_time_prog = re.compile(
            r"\|\s+Ingestion time\s+\|\s+(\d+)[\s\w]+\|"
        )
        self.num_inserts_prog = re.compile(r"\|\s+Num inserts\s+\|\s+(\d+)[\s\w]+\|")
        self.bytes_written_prog = re.compile(
            r"\|\s+Bytes Written\s+\|\s+(\d+)[\s\w]+\|"
        )
        self.num_compactions_prog = re.compile(
            r"\|\s+#\. Compactions\s+\|\s+(\d+)[\s\w]+\|"
        )
        self.num_files_trivial_prog = re.compile(
            r"\|\s+#\. Files moved trivially\s+\|\s+(\d+)[\s\w]+\|"
        )
        self.bytes_moved_trivial_prog = re.compile(
            r"\|\s+Bytes moved trivially\s+\|\s+(\d+)[\s\w]+\|"
        )
        self.trivial_if_cond_accesses_prog = re.compile(
            r"\|\s+#\. Trivial if cond\. accesses\s+\|\s+(\d+)[\s\w]+\|"
        )

    def create_db(self) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
            CREATE TABLE IF NOT EXISTS rocksdb_bliss (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                time_stamp TXT,
                file_name TXT,
                k_pt REAL,
                l_pt REAL,
                ingestion_time INT,
                num_inserts INT,
                bytes_written INT,
                num_compactions INT,
                num_files_trivial INT,
                bytes_moved_trivial INT,
                trivial_if_cond_accesses INT
            );
            """
        )
        self.db_con.commit()

    def log_row(self, stats: PyRocksStats, k_pt: int, l_pt: int) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
               INSERT INTO rocksdb_bliss (
                   time_stamp, file_name, k_pt, l_pt, ingestion_time,
                   num_inserts, bytes_written, num_compactions, num_files_trivial,
                   bytes_moved_trivial, trivial_if_cond_accesses
               ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
            """,
            (
                datetime.now(timezone.utc),
                stats.input_file,
                k_pt,
                l_pt,
                stats.ingestion_time,
                stats.num_inserts,
                stats.bytes_written,
                stats.num_compactions,
                stats.num_files_trivial,
                stats.bytes_moved_trivial,
                stats.trivial_if_cond_accesses,
            ),
        )
        self.db_con.commit()

    def run_experiment(
        self,
        input_file: str,  # input data file
    ) -> PyRocksStats:
        if self.smoke_test:
            a, b = (0, 2 << 16)
            return PyRocksStats(
                input_file,
                randint(a, b),
                randint(a, b),
                randint(a, b),
                randint(a, b),
                randint(a, b),
                randint(a, b),
                randint(a, b),
            )

        cmd = [
            self.rocks_executable,
            f"--ingestionPath {input_file}",
        ]
        process = subprocess.Popen(
            " ".join(cmd),
            stdout=subprocess.PIPE,
            universal_newlines=True,
            shell=True,
        )
        res, _ = process.communicate()
        logging.debug(res)
        with open(input_file + ".log", "w") as fid:
            fid.write(res)

        ingestion_time = self.ingestion_time_prog.search(res)
        ingestion_time = int(ingestion_time.group(1)) if ingestion_time else 0

        num_inserts = self.num_inserts_prog.search(res)
        num_inserts = int(num_inserts.group(1)) if num_inserts else 0

        bytes_written = self.bytes_written_prog.search(res)
        bytes_written = int(bytes_written.group(1)) if bytes_written else 0

        num_compactions = self.num_compactions_prog.search(res)
        num_compactions = int(num_compactions.group(1)) if num_compactions else 0

        num_files_trivial = self.num_files_trivial_prog.search(res)
        num_files_trivial = int(num_files_trivial.group(1)) if num_files_trivial else 0

        bytes_moved_trivial = self.bytes_moved_trivial_prog.search(res)
        bytes_moved_trivial = (
            int(bytes_moved_trivial.group(1)) if bytes_moved_trivial else 0
        )

        trivial_if_cond_accesses = self.trivial_if_cond_accesses_prog.search(res)
        trivial_if_cond_accesses = (
            int(trivial_if_cond_accesses.group(1)) if trivial_if_cond_accesses else 0
        )

        return PyRocksStats(
            input_file=input_file,
            ingestion_time=ingestion_time,
            num_inserts=num_inserts,
            bytes_written=bytes_written,
            num_compactions=num_compactions,
            num_files_trivial=num_files_trivial,
            bytes_moved_trivial=bytes_moved_trivial,
            trivial_if_cond_accesses=trivial_if_cond_accesses,
        )


def get_file_params(file_name):
    file_regex_str = r"data_N(\d+)_k(\d+)_l(\d+)\.bin"
    res = re.search(file_regex_str, file_name)
    if res is None:
        raise LookupError
    num_keys, k_pt, l_pt = res.groups()

    return int(num_keys), int(k_pt), int(l_pt)


def main(args) -> None:
    rocks = PyRocks(
        rocks_executable=args.rocks,
        db_path=args.result_db,
        smoke_test=args.smoke_test,
    )
    rocks.create_db()
    files = os.listdir(args.data_folder)
    for file in files:
        _, k_pt, l_pt = get_file_params(file)
        logging.info(f"Running BlissRocks ({file})")
        stats = rocks.run_experiment(os.path.join(args.data_folder, file))
        logging.info(f"{stats=}")
        rocks.log_row(stats, k_pt, l_pt)


    return


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--rocks",
        type=str,
        required=True,
        help="path to rocksdb executable",
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
        level=log_level,
    )
    main(args)
