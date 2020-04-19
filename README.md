# pico-ping

### Overview
Pico-ping is a minimal Linux utility that emulates some of the core functionality of `ping` using a more modern style of C++; 

Specific functionality implemented at this time:
* Positional argument for either hostname or ipaddress
    - `pico_ping google.com` or `pico_ping 8.8.8.8` 
    
* Optional argument for specifying timeout value in seconds
    - `pico_ping google.com -W 5` or `pico_ping 8.8.8.8 --timeout 5`
    
* Calculates and displays RTT of each packet  / lost packets
    - `````bash
      patrick@lu:~$ sudo ./pico_ping 8.8.8.8
           64 bytes from 8.8.8.8: icmp_seq=1 time=9.42
           64 bytes from 8.8.8.8: icmp_seq=2 time=11.71
           64 bytes from 8.8.8.8: icmp_seq=3 time=10.21
           64 bytes from 8.8.8.8: icmp_seq=4 time=9.33
           64 bytes from 8.8.8.8: icmp_seq=5 time=9.11
           64 bytes from 8.8.8.8: icmp_seq=6 time=9.17
           64 bytes from 8.8.8.8: icmp_seq=7 time=9.31
           Request Timed Out
           Request Timed Out
      `````
      
         **Sudo warning:** Notice the snippet above includes `sudo`, this is because the specific type of socket
         created requires special access. In order to execute this program you have to set the sysctl value: `sysctl -w net.ipv4.ping_group_range="0 0"`
         
         
---
### Build and Setup


1) Clone repo

    ```git clone https://github.com/patrick--/pico-ping```
    
2) Move into clone repo

    ```cd pico-ping```
    
3) Create build directory and generate build dependencies
     ```sh
      mkdir build && cd build
      cmake ..
      ```
      
4) Build and run tests
    ```sh
     make TestAll
     sudo ./test/TestAll
     ```
     
5) Build and run pico_ping
   ```sh
     make pico_ping
     sudo ./app/pico_ping google.com --timeout 5
     ```
    