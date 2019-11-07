#!/bin/bash
file="test/test.txt"
total=0
success=0
failed=0

while read -r line; do
    touch ref.txt
    touch stud.txt
    find $line > ref.txt
    ./myfind $line > stud.txt

    diff ref.txt stud.txt --color > diff.txt
    if [ $? -eq 0 ]; then
        echo -e "\e[1;32m[SUCCESS] ./myfind $line\e[0m"
        let "success+=1"
    else
        echo -e "\e[1;31m[FAILED] ./myfind $line\e[0m"
        cat diff.txt
        let "failed+=1"
    fi
    let "total+=1"
done < $file

echo -e "\e[1;96mTotal of $total tests, $success tests succeeded, $failed tests failed\e[0m"

rm ref.txt stud.txt
exit 0
