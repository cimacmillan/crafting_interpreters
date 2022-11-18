set -e

sh rebuild_all.sh

for file in lox/*
do
  echo '>' $file
  ./build/lox "$file" > test_output.txt
done

echo "Tests passed"
