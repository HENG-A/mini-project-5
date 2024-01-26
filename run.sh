#!/bin/bash

m_value="$1"


echo "Enter M Value: "
read -r m_value

echo "Compiling simple_filter.c ..."
g++ simple_filter.c -o simple_filter
echo "Done."

echo "Running simple_filter.c ..."
./simple_filter "${m_value}" hL.txt hR.txt YL.txt YR.txt blue_giant_fragment.wav out.wav
echo "Done."

echo "Running show_data.py ..."
python3 show_data.py
echo "Done. run.sh is done."

read -p "Press Enter to exit ..."
