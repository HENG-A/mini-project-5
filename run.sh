echo "Enter M Value: "
read m_value
c++ simple_filter.c -o simple_filter
./simple_filter ${m_value} hL.txt hR.txt YL.txt YR.txt blue_giant_fragment.wav out.wav
python show_data.py
