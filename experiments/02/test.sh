i=0

while [ $i -lt 10000000 ]; do
    echo "test $i"
    outcome=$(./$1 2>&1)
    echo $outcome
    if [[ $outcome =~ 'failed' ]]; then
        echo "====Found bug===="
        exit 0
    fi
    let i=i+1
done

