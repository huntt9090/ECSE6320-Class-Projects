echo L1 Single-Access Baseline> baseline.txt
:: load 16kB to stay in L1, specify single request with x0
mlc.exe --idle_latency -b16 -c8 -x0>> baseline.txt

:: load 256kB -> larger than L1, smaller than L3
echo L2 Single-Access Baseline>> baseline.txt
mlc.exe --idle_latency -b256 -c8 -x0>> baseline.txt

:: load 3MB -> can only fit in L3
echo L3 Single-Access Baseline>> baseline.txt
mlc.exe --idle_latency -b3m -c8 -x0>> baseline.txt

echo L1 Active Baseline>> baseline.txt
:: load 16kB to stay in L1, specify single request with x0
mlc.exe --idle_latency -b16 -c8 -x1>> baseline.txt

:: load 256kB -> larger than L1, smaller than L3
echo L2 Active Baseline>> baseline.txt
mlc.exe --idle_latency -b256 -c8 -x1>> baseline.txt

:: load 3MB -> can only fit in L3
echo L3 Active Baseline>> baseline.txt
mlc.exe --idle_latency -b3m -c8 -x1>> baseline.txt



::Pattern & granularity sweep

::Sequential
echo 64B Stride Sequential> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l64 -d0>> pattern_stride.txt

echo 256B Stride Sequential>> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l256 -d0>> pattern_stride.txt

echo 1024B Stride Sequential>> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l1024 -d0>> pattern_stride.txt

::Random latency and bandwidth pattern
echo 64B Stride Random>> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l64 -d0 -r -U>> pattern_stride.txt

echo 256B Stride Random>> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l256 -d0 -r -U>> pattern_stride.txt

echo 1024B Stride Random>> pattern_stride.txt
mlc.exe --loaded_latency -c8 -l1024 -d0 -r -U>> pattern_stride.txt



::R/W Mix sweep
echo 100% Read Sequential> rw_mix.txt
mlc.exe --loaded_latency -c8 -d0>> rw_mix.txt

echo 100% Write Sequential>> rw_mix.txt
mlc.exe --loaded_latency -c8 -d0 -W6>> rw_mix.txt

echo 2:1 R/W Sequential>> rw_mix.txt
mlc.exe --loaded_latency -c8 -d0 -W2>> rw_mix.txt

echo 1:1 R/W Sequential>> rw_mix.txt
mlc.exe --loaded_latency -c8 -d0 -W5>> rw_mix.txt



::Intensity Sweep
echo 1 Thread> intensity.txt
mlc.exe --loaded_latency -c8 -d0 -m200>> intensity.txt

echo 2 Threads>> intensity.txt
mlc.exe --loaded_latency -c8 -d0 -m600>> intensity.txt

echo 4 Threads>> intensity.txt
mlc.exe --loaded_latency -c8 -d0 -m1E00>> intensity.txt

echo 6 Threads>> intensity.txt
mlc.exe --loaded_latency -c8 -d0 -m7E00>> intensity.txt