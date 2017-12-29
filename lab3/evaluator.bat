cd ..
cd lab1
analizator.exe < "kod.txt" > "../lab2/analizator/out.txt"
cd ..
cd lab2
cd analizator
analizator.exe < "out.txt" > "../../lab3/out.txt"
cd ..
cd ..
cd lab3
generator <out.txt
