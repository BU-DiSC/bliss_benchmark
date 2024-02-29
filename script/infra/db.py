import sqlite3
from datetime import datetime, timezone
from .pybliss import BlissArgs, BlissStats


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
                index_type TEXT,
                k_pt REAL,
                l_pt REAL,
                preload_factor REAL,
                write_factor REAL,
                read_factor REAL,
                mixed_ratio REAL,
                use_preload INT
                preload_time INT,
                preload_creation_time INT,
                write_time INT,
                mixed_time INT,
                read_time INT,
            );
            """
        )
        self.db_con.commit()

    def log_row(
        self,
        k_pt: float,
        l_pt: float,
        args: BlissArgs,
        stats: BlissStats,
    ) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
               INSERT INTO bliss_bench (
                   time_stamp,
                   file_name,
                   index_type,
                   k_pt,
                   l_pt,
                   preload_factor,
                   write_factor,
                   read_factor,
                   mixed_ratio,
                   use_preload
                   preload_time,
                   preload_creation_time,
                   write_time,
                   mixed_time,
                   read_time,
               ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
            """,
            (
                datetime.now(timezone.utc),
                args.data_file,
                args.index_type,
                k_pt,
                l_pt,
                args.preload_factor,
                args.write_factor,
                args.read_factor,
                args.mixed_ratio,
                args.use_preload,
                stats.preload_time,
                stats.preload_creation_time,
                stats.write_time,
                stats.mixed_time,
                stats.read_time,
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
