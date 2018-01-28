cd ..
cd lab1
./analizator.exe < "../lab3/kodovi/%1" > "../lab2/analizator/out.txt"
cd ..
cd lab2
cd analizator
./analizator.exe < "out.txt" > "../../lab3/out.txt"
cd ..
cd ..
cd lab3
./generator.exe <out.txt
cd ..
cd lab4
./generator.exe <"../lab3/out.txt" >out.txt
./node main.js out.txt >iz.txt
