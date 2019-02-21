# SJTU-CS339-Project: HyDRO Reimplementatio
This project is finished by Chen Wang, Yanjun Fu, Ke Li and Yongqing Xu. The C++ code is finished by Chen Wang and Yanjun Fu, the NS2 simulation is finished by Ke Li, the PPT is finished by Yongqing Xu, and the report is finished by Chen Wang, Yanjun Fu and Yongqing Xu.
## Description
**HyDRO**, which is the short of **H**arvesting-aware **D**ata **RO**uting, is proposed by Basagni et al. Our project is to reimplement this paper ''*Harnessing HyDRO: Harvesting-aware Data ROuting for Underwater Wireless Sensor Networks, MobiHoc 2018* ''. Since the simulator SUNSET mentioned in this paper is not available and more detailed parameters' settings are not available, our reimplementation makes some reasonable simplifications.

![1](./figure/energy_harvest.jpg)

## Reinforcement Learning Based Routing

The routing algorithm utilizes the traditional reinforcement learning algorithm: **Q-Learning**. The algorithm chooses relays, taking both residual energy and fore- seeable harvestable energy into account.

## Multi-thread Programming
Our program is using multi-thread techniques.

## Implicit ACK
The pipeline is shown below.
<div align="left">
    <img src="./figure/1.png" width="400">
</div>

## Simulation Parameters
| **Parameter** | **Value** |
| -------- | :------: |
| Simulation duration | 20min |
| Number of nodes | 20 |
| Size of deployment area | 2km<sup>2</sup> |
| Depth of deployment | 10m~240m |
| Bit rate | 4000b/s |
| Packet payload size | 1000B |
| Packet header size | 15B |
| Packet inter-arrival time | \[5,4,3\](low to high traffic) |
| Number of retransmissions K | \[5,4,3\](low to high) |
| Discount factor \gamma | 0.95 |

## Simulation & Visualization by NS2
For the video of our simulation, please refer to the PPT.
<div align="left">
    <img src="./figure/visual.png" width="500">
</div>

## Results
- Energy consumption statistics
<div align="left">
    <img src="./figure/routing-change.png" width="500">
</div>

## Make & Run
You can adjust parameters in file `./code/HyDRO/include/parameters.h`
- Make
```bash
cd ./code/HyDRO/include/parameters.h
rm -rf build
mkdir build && cd build
cmake..
make
```
- Run
```bash
./main
```
All the intermediate results are available in `./code/HyDRO/build/out.txt`
