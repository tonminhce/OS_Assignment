#	Student: Nguyen Ton Minh - 2052600
#	Operating Systems Lab 1: Introduction to Linux Programming
#	Lecturer: Le Thanh Van

#				Exercise 3.6

while true
do
read -p '>> ' a op b
flag=1
if [ -z "$op" -o -z "$b" ]; then
# if the op and b is not available so that we will check the input a if it is HIST or EXIT
    case $a in
        HIST) 
            hist=$(cat HIST | tail -n 5)
            echo "$hist"
        ;;
        EXIT)
        	exit
        ;;
        *) echo "Invalid input";;
    esac
else
    if [ $a == 'ANS' ]; then
# if a is used in the calculator as a earlier result        
	a=$(cat ANS)
        if [ -z "$a" ]; then
            echo "NO ANS"
            exit
        fi

        flag=0
    fi
# check the operator for the calculation
    case $op in
        +) res=$(echo $a + $b | bc)
        	echo "$res"
	;;
        -) res=$(echo $a - $b | bc)
       		echo "$res"
	;;
	%) res=$(echo $a / $b | bc)
		echo "$res"
	;;
        *) res=$(echo $a \* $b | bc)
        	echo "$res"
	;;
        /) if [ $b -eq 0 ]; then
            echo "MATH ERROR"
            exit
        else
            res=$(echo "scale=2; $a / $b" | bc)
        fi;;
        *) echo "SYNTAX ERROR";;
    esac
fi
# save a to the ANS
if [ $flag -eq 0 ]; then
    a='ANS'
fi
echo "$a $op $b = $res" >> HIST
echo $res > ANS
done
