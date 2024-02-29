#!/bin/bash

SAMPLE_COUNT=2


##### Initial waffle

echo "The environment variable has the initial peer set to: 2@builtin.udpv4://172.17.0.2"
echo "The Peers file has the initial peer set to: 3@builtin.udpv4://172.17.0.3"
echo "The QoS xml file has the initial peer set to: 4@builtin.udpv4://172.17.0.4"
echo "The hardcoded initial peer is set to: 5@builtin.udpv4://172.17.0.5"
echo 
echo "Press any key to continue"
read -rsn1


##### TEST_ONE

# make the text file is absent
rm -f -- NDDS_DISCOVERY_PEERS

# Make sure the environment variable isn't set
unset NDDS_DISCOVERY_PEERS

# make sure the USER_QOS_PROFILES symlink popints to the version without initial peers
rm USER_QOS_PROFILES.xml
ln -s USER_QOS_PROFILES_default.xml USER_QOS_PROFILES.xml

echo "Starting the app with NO Environment variable, NO text file, NO peers defined in xml and bypassing the hardcoded peer configuration"
echo
echo

# Start the application without the -p or --peers parameter for n samples
./objs/x64Linux4gcc7.3.0/shapes_publisher -s $SAMPLE_COUNT

echo
echo
echo "Press any key to continue"
read -rsn1



##### TEST_TWO

# Make sure the environment variable is now set
export NDDS_DISCOVERY_PEERS=2@builtin.udpv4://172.17.0.2

echo "Starting the app WITH Environment variable, NO text file, NO peers defined in xml and bypassing the hardcoded peer configuration"
echo
echo

# Start the application without the -p or --peers parameter for n samples
./objs/x64Linux4gcc7.3.0/shapes_publisher -s $SAMPLE_COUNT

echo
echo
echo "Press any key to continue"
read -rsn1


##### TEST THREE
# If both the file and environment variable are found, the file takes precedence and the environment variable will be ignored.1

# make sure the NDDS_DISCOVERY_PEERS symlink exists
ln -s NDDS_DISCOVERY_PEERS.txt NDDS_DISCOVERY_PEERS

echo "Starting the app WITH Environment variable, WITH text file, NO peers defined in xml and bypassing the hardcoded peer configuration"
echo
echo

# Start the application without the -p or --peers parameter for n samples
./objs/x64Linux4gcc7.3.0/shapes_publisher -s $SAMPLE_COUNT

echo
echo
echo "Press any key to continue"
read -rsn1



##### TEST FOUR
# The settings in the default XML QoS Profile take precedence over the file and environment variable.

# make sure the USER_QOS_PROFILES symlink points to the version with initial peers defined
rm USER_QOS_PROFILES.xml
ln -s USER_QOS_PROFILES_peers.xml USER_QOS_PROFILES.xml

echo "Starting the app WITH Environment variable, WITH text file, WITH peers defined in xml and bypassing the hardcoded peer configuration"
echo 
echo

# Start the application without the -p or --peers parameter for n samples
./objs/x64Linux4gcc7.3.0/shapes_publisher -s $SAMPLE_COUNT

echo
echo
echo "Press any key to continue"
read -rsn1


##### TEST FIVE
# The hardcoded settings take precedence over all other methods of defining the peers list

echo "Starting the app WITH Environment variable, WITH text file, WITH peers defined in xml and WITH hardcoded peer configuration"
echo
echo

# Start the application with the -p parameter for n samples
./objs/x64Linux4gcc7.3.0/shapes_publisher -s $SAMPLE_COUNT -p

echo
echo
echo "Press any key to continue"
read -rsn1


