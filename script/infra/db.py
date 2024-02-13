import sqlite3


class BlissDB:
    def __init__(self, db_path: str) -> None:
        self.db_con = sqlite3.connect(db_path)
        cursor = self.db_con.cursor()
        cursor.execute(
            """
            CREATE TABLE IF NOT EXISTS bliss_bench (
                id INTEGER PRIMARY KEY AUTOINCREMENT
                k_pt REAL
                l_pt REAL
                index TEXT
                preload_time INT
                write_time INT
                mixed_time INT
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
        preload_time: int,
        write_time: int,
        mixed_time: int,
        read_time: int,
    ) -> None:
        cursor = self.db_con.cursor()
        cursor.execute(
            """
               INSERT INTO bliss_bench (k_pt, l_pt, index, preload_time,
                                        write_time, mixed_write, read_time)
               VALUES (?, ?, ?, ?, ?, ?, ?)'
            """,
            (k_pt, l_pt, index, preload_time, write_time, mixed_time, read_time),
        )
        return
