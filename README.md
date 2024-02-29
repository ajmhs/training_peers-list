# Console application based on Shapes which demonstrates the initial peers list

This example demonstrates the multiple ways to setup the initial peers list 

Skeleton created via
 
`rtiddsgen -language C++11 -platform x64Linux4gcc7.3.0 -example x64Linux4gcc7.3.0 -create makefiles -create typefiles -d c++11 shapes.idl`

This example attempts to demonstrate the different ways of configuring the initial peers list, and the order of priority. Ths publisher prints to screen the default QoS initial peer list, then starts the participant, and then proceeds to print out the participants initial peer list. If the new parameter "-p" or "--peers" is present on the command line, then the application configures a hard-coded peer. 
 
The methods of setting the intial peers each have a single unique peer defined as follows:

The environment variable has the initial peer set to: `2@builtin.udpv4://172.17.0.2`  
The Peers file has the initial peer set to: `3@builtin.udpv4://172.17.0.3`  
The QoS xml file has the initial peer set to: `4@builtin.udpv4://172.17.0.4`  
The hardcoded initial peer is set to: `5@builtin.udpv4://172.17.0.5`  


The `run_demo.sh` script, controls the demonstration; it runs the application five times, each time with differing configuration.

1. Starts the app with NO environment variable, NO text file, NO peers defined in xml and bypassing the hardcoded peer configuration.
2. Starts the app WITH environment variable, NO text file, NO peers defined in xml and bypassing the hardcoded peer configuration.
3. Starts the app WITH environment variable, WITH text file, NO peers defined in xml and bypassing the hardcoded peer configuration
4. Starts the app WITH environment variable, WITH text file, WITH peers defined in xml and bypassing the hardcoded peer configuration.
5. Starts the app WITH environment variable, WITH text file, WITH peers defined in xml and WITH hardcoded peer configuration

The script waits for a keypress between each run, and the application itself shows the configured initial peer list.

The expected result is that the environment variable has the lowest priority, and is overridden by the file (even when empty). Next comes the XML QoS file defined peers, and finally those defined in code have the highest priority