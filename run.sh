echo "Enter M Value: "
read m_value
echo "Compiling simple_filter.c ..."
c++ simple_filter.c -o simple_filter
echo "Done."
echo "Running simple_filter.c ..."
./simple_filter ${m_value} hL.txt hR.txt YL.txt YR.txt blue_giant_fragment.wav out.wav
echo "Done."
echo "Running show_data.py ..."
python show_data.py
echo "Done. run.sh is done."
read -p "Press any key to exit ..."
