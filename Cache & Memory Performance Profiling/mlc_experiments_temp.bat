::Working Set Sweep
echo L1 Single-Access Baseline> ws_sweep.txt
mlc.exe --loaded_latency -b8 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b16 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b32 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b64 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b128 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b256 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b512 -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b1m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b2m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b4m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b8m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b16m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b32m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b64m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b128m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b256m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b512m -c8 -d0>> ws_sweep.txt
mlc.exe --loaded_latency -b1024m -c8 -d0>> ws_sweep.txt