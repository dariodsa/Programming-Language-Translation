for filename in ../lab3/kodovi/*.out; 
 do 
    echo $filename
    name=${filename##*/}
    base=${name%.out}
    ./evaluator.bat $base".txt"
    ./compare iz.txt $filename status.txt
    fileSize=$(stat -c%s "status.txt")
    if [ $fileSize -ne 0 ]
    then
       echo "KRIVO $filename"
       exit -1
    fi
 done
