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
                file_name TXT,
                k_pt REAL,
                l_pt REAL,
                preload_factor REAL,
                write_factor REAL,
                read_factor REAL,
                mixed_ratio REAL,
                index_type TEXT,
                preload_time INT,
                write_time INT,
                mixed_time INT,
                read_time INT,
                use_preload INT
            );
            """
        )
        self.db_con.commit()

    def log_row(
        self,
        file_name: str,
        k_pt: float,
        l_pt: float,
        index: str,
        preload_time: str,
        write_time: str,
        mixed_time: str,
        read_time: str,
        preload_factor: float,
        write_factor: float,
        read_factor: float,
        mixed_ratio: float,
        use_preload: bool,
    ) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
               INSERT INTO bliss_bench (
                   time_stamp, file_name, k_pt, l_pt, index_type, preload_time,
                   write_time, mixed_time, read_time, preload_factor,
                   write_factor, read_factor, mixed_ratio, use_preload
               ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
            """,
            (
                datetime.now(timezone.utc),
                file_name,
                k_pt,
                l_pt,
                index,
                preload_time,
                write_time,
                mixed_time,
                read_time,
                preload_factor,
                write_factor,
                read_factor,
                mixed_ratio,
                use_preload
            ),
        )
        self.db_con.commit()

    def get_last_rows(self, num_rows: int = 10):
        cursor = self.db_con.cursor()
        rows = cursor.execute(
            """
            SELECT * FROM bliss_bench ORDER BY time_stamp DESC LIMIT ?;
            """,
            (num_rows,),
        )
        return rows.fetchall()
