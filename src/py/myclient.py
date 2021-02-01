#!/usr/bin/python3
# -*-coding:utf-8-*-

from __future__ import unicode_literals
from __future__ import print_function

import argparse
import pymysql


def execute(conn, stmt):
    cur = conn.cursor()
    cur.execute(stmt)
    rows = cur.fetchall()
    cur.close()
    return rows

def run(conn):
    conn.commit()
    while True:
        try:
            stmt = input("mysql>")
            rows = execute(conn, stmt + ";")
            for row in rows:
                print(row)
        except pymysql.err.MySQLError as e:
            print(e)

    conn.close()


def main():
    parser = argparse.ArgumentParser(description="A simple MySQL client tool")
    parser.add_argument("-H", "--host", default="127.0.0.1", help="Connect host")
    parser.add_argument("-P", "--port", default=3306, help="Port number to use for connection to")
    parser.add_argument("-u", "--user", default="root", help="User for login if not current user")
    parser.add_argument("-p", "--password", help="Password to use when connecting to server")
    parser.add_argument("-D", "--database", help="Database to use")
    args = parser.parse_args()

    conn = pymysql.connect(host=args.host, port=args.port, user=args.user, passwd=args.password,
                        db=args.database, charset="utf8mb4")
    run(conn)



if __name__ == '__main__':
    main()
