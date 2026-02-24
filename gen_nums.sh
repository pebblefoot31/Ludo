count=20

while getopts "n:" opt; do
    case $opt in 
        n) count=$OPTARG ;;
        *) echo "Usage: $0 [-n number]" >&2
            exit 1 ;;
    esac
done


for ((i=1; i<=count;i++)); do
    num=$(( $RANDOM % 6 + 1))
    echo "$num"

done
