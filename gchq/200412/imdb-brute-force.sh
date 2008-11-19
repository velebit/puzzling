#!/bin/sh

while [ $# -gt 0 ]; do
    case "$1" in
	-actor|--actor)
	    job="--job actor" ;;
	-actress|--actress)
	    job="--job actress" ;;
	-star|--star)
	    job="--job act.*" ;;
	-*) echo "unknown flag " >&2; exit 1 ;;
	*)  break ;;
    esac
    shift
done

for name in "$@"; do
    echo "===== $name ====="
    set -- $name
    if [ $# -lt 2 ]; then
        echo "'$name' is not a full encrypted name!" >&2
        continue
    fi
    first="$1"
    while [ $# -gt 0 ]; do last="$1"; shift; done

    echo "  --- $first (first) ---"
    for x in `./words-like.pl -L --first "$first"`; do
        ./imdb-name-search.pl $job $x "$name"
    done

    echo "  --- $last (last) ---"
    for x in `./words-like.pl -L --last "$last"`; do
        ./imdb-name-search.pl $job $x "$name"
    done

done
