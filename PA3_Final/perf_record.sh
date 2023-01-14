#!/bin/bash
# Run perf record 20 times and save the results in perf.data
for i in {1..20}
do
    perf record -o perf_reports/perf.data.32C.$i ./bin/k_means 10000000 32 $i
done

