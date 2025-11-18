#!/bin/bash

PUSH_SWAP="./push_swap"
NB_TESTS=5

generate_random_list() {
	seq 0 $(($1 - 1)) | sort -R | tr '\n' ' '
}

evaluate_score() {
	local size=$1
	local moves=$2
	local score=0

	if [ "$size" -eq 3 ]; then
		[ "$moves" -le 3 ] && score=1 || score=0
	elif [ "$size" -eq 5 ]; then
		[ "$moves" -le 12 ] && score=1 || score=0
	elif [ "$size" -eq 100 ]; then
		if   [ "$moves" -lt 700 ]; then score=5
		elif [ "$moves" -ge 700 ] && [ "$moves" -lt 900 ]; then score=4
		elif [ "$moves" -ge 900 ] && [ "$moves" -lt 1100 ]; then score=3
		elif [ "$moves" -ge 1100 ] && [ "$moves" -lt 1300 ]; then score=2
		elif [ "$moves" -ge 1300 ] && [ "$moves" -lt 1500 ]; then score=1
		else score=0; fi
	elif [ "$size" -eq 500 ]; then
		if   [ "$moves" -lt 5500 ]; then score=5
		elif [ "$moves" -ge 5500 ] && [ "$moves" -lt 7000 ]; then score=4
		elif [ "$moves" -ge 7000 ] && [ "$moves" -lt 8500 ]; then score=3
		elif [ "$moves" -ge 8500 ] && [ "$moves" -lt 10000 ]; then score=2
		elif [ "$moves" -ge 10000 ] && [ "$moves" -lt 11500 ]; then score=1
		else score=0; fi
	fi

	echo $score
}

run_tests() {
	local size=$1
	local total=0

	echo "========== Testing $size values =========="
	for i in $(seq 1 $NB_TESTS); do
		args=$(generate_random_list $size)
		output=$($PUSH_SWAP $args)
		move_count=$(echo "$output" | wc -l)
		points=$(evaluate_score $size $move_count)
		echo "Test $i: $move_count moves → $points point(s)"
		total=$((total + points))
	done

	average=$(echo "scale=2; $total / $NB_TESTS" | bc)

	if [ "$size" -eq 3 ] || [ "$size" -eq 5 ]; then
		echo "== Total : $total / $NB_TESTS point(s) = moyenne $average / 1.00 =="
	else
		echo "== Moyenne sur $NB_TESTS tests : $average / 5.00 =="
	fi
	echo
}

run_tests 3
run_tests 5
run_tests 100
run_tests 500
echo "========== All tests completed =========="