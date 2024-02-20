import sqlite3
from datetime import datetime, timezone


class BlissDB:
    def __init__(self, db_path: str) -> None:
        self.db_con = sqlite3.connect(db_path)
        cursor = self.db_con.cursor()
        cursor.execute(
            """
            CREATE TABLE IF NOT EXISTS bliss_bench (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                time_stamp TXT,
                k_pt REAL,
                l_pt REAL,
                index_type TEXT,
                preload_time INT,
                write_time INT,
                mixed_time INT,
                read_time INT
            );
            """
        )
        self.db_con.commit()

    def log_row(
        self,
        k_pt: float,
        l_pt: float,
        index: str,
        preload_time: str,
        write_time: str,
        mixed_time: str,
        read_time: str,
    ) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
               INSERT INTO bliss_bench (
                   time_stamp, k_pt, l_pt, index_type, preload_time,
                   write_time, mixed_time, read_time
               ) VALUES (?, ?, ?, ?, ?, ?, ?, ?);
            """,
            (
                datetime.now(timezone.utc),
                k_pt,
                l_pt,
                index,
                preload_time,
                write_time,
                mixed_time,
                read_time,
            ),
        )
        self.db_con.commit()

    def display_last_rows(self, num_rows: int = 10) -> None:
        cursor = self.db_con.cursor()
        rows = cursor.execute(
            """
            SELECT * FROM bliss_bench ORDER BY time_stamp DESC LIMIT ?;
            """,
            (num_rows,),
        )
        for row in rows:
            print(row)
