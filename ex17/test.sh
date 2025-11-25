#!/bin/bash
set -e

DB=./test.db

echo "== Create database =="
./ex17_1 $DB c 32 10

echo "== Add records =="
./ex17_1 $DB s 1 Jin Email1
./ex17_1 $DB s 2 Yuan Email2
./ex17_1 $DB s 3 Alice alice@example.com

echo "== List all records =="
./ex17_1 $DB l

echo "== Get specific records =="
./ex17_1 $DB g 1
./ex17_1 $DB g 3

echo "== Find by name/email =="
./ex17_1 $DB f Jin
./ex17_1 $DB f alice@example.com
./ex17_1 $DB f Bob   # 测试找不到的情况

echo "== Delete a record =="
./ex17_1 $DB d 2

echo "== List after deletion =="
./ex17_1 $DB l

echo "== Set a record that was deleted =="
./ex17_1 $DB s 2 Bob bob@example.com

echo "== List after re-adding =="
./ex17_1 $DB l

echo "== Try to get invalid id =="
set +e  # 允许失败，不让脚本退出
./ex17_1 $DB g 99  # 超出范围
set -e

echo "All tests passed!"
