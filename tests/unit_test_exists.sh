
# boolean flag to check if test passed
test_passed=true

# loop through all files in src/bliss directory excluding subdirectories
for file in $(find ../src/bliss -type f -name "bench_*.h"); do
    # get the filename without the path
    filename=$(basename $file)
    # get the filename without the extension
    filename_no_ext="${filename%.*}"
    # get index name from filename which is of format bench_<index>.h
    index=$(echo $filename_no_ext | cut -d'_' -f2)
    
    # check if the test folder exists 
    if [ ! -d "test_$index" ]; then
        echo "tests/test_$index/ directory does not exist"
        test_passed=false
        break
    else 
        # check if test folder contains a CMakeLists.txt or <index>_tests.cpp file
        if [ ! -f "test_$index/CMakeLists.txt" ] && [ ! -f "test_$index/${index}_tests.cpp" ]; then
            echo "tests/test_$index/ directory does not contain CMakeLists.txt or ${index}_tests.cpp file"
            test_passed=false
            break
        fi
    fi
done

if [ "$test_passed" = true ]; then
    echo "unit tests exist for all indexes"
fi