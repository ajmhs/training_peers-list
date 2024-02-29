/*
* (c) Copyright, Real-Time Innovations, 2020.  All rights reserved.
* RTI grants Licensee a license to use, modify, compile, and create derivative
* works of the software solely for use with RTI Connext DDS. Licensee may
* redistribute copies of the software provided that all such copies are subject
* to this license. The software is provided "as is", with no warranty of any
* type, including any warranty for fitness for any purpose. RTI is under no
* obligation to maintain or support the software. RTI shall not be liable for
* any incidental or consequential damages arising out of the use or inability
* to use the software.
*/

#include <iostream>

#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp>      // for sleep()
#include <rti/config/Logger.hpp>  // for logging
// alternatively, to include all the standard APIs:
//  <dds/dds.hpp>
// or to include both the standard APIs and extensions:
//  <rti/rti.hpp>
//
// For more information about the headers and namespaces, see:
//    https://community.rti.com/static/documentation/connext-dds/7.2.0/doc/api/connext_dds/api_cpp2/group__DDSNamespaceModule.html
// For information on how to use extensions, see:
//    https://community.rti.com/static/documentation/connext-dds/7.2.0/doc/api/connext_dds/api_cpp2/group__DDSCpp2Conventions.html

#include "application.hpp"  // for command line parsing and ctrl-c
#include "shapes.hpp"
#include <vector>
#include <string>

using std::cout;
using std::endl;
using stringvector_t = std::vector<std::string>;


void run_publisher_application(unsigned int domain_id, unsigned int sample_count, bool config_peers)
{
    // DDS objects behave like shared pointers or value types
    // (see https://community.rti.com/best-practices/use-modern-c-types-correctly)

    dds::core::QosProvider qos_provider = dds::core::QosProvider::Default();

    // Dump the initial peers list (this is from either XML QoS or from NDDS_DISCOVERY_PEERS)
    cout << "Dumping default initial peers:" << endl;
    for (auto peer : qos_provider.participant_qos().policy<rti::core::policy::Discovery>().initial_peers()) {
        cout << "\t" << peer << endl;
    }

    // Get the participant qos from the qos provider
    dds::domain::qos::DomainParticipantQos participant_qos = qos_provider.participant_qos();

    if (config_peers) {
        // Create our own Discovery object and configure the peers 
        rti::core::policy::Discovery discovery;
        discovery.initial_peers({"5@builtin.udpv4://172.17.0.5"});

        // and update it with our discovery object        
        participant_qos << discovery;
    }

    // Instantiate the domain participant, using the participant qos 
    dds::domain::DomainParticipant participant(domain_id, participant_qos);
    // Peers can be added after startup with as so:
    // participant.extensions().add_peer("5@builtin.udpv4://172.17.0.5");
    
    
    // Dump the participants initial peers list
    cout << "Dumping the participant initial peers:" << endl;
    for (auto peer : participant.qos().policy<rti::core::policy::Discovery>().initial_peers()) {
        cout << "\t" << peer << endl;
    }

    // Create a Topic with a name and a datatype
    dds::topic::Topic< ::ShapeTypeExtended> topic(participant, "ShapeType");

    // Create a Publisher
    dds::pub::Publisher publisher(participant);

    // Create a DataWriter with default QoS
    dds::pub::DataWriter< ::ShapeTypeExtended> writer(publisher, topic);

    ::ShapeTypeExtended data;
    // Main loop, write data
    for (unsigned int samples_written = 0;
    !application::shutdown_requested && samples_written < sample_count;
    samples_written++) {
        // Modify the data to be written here
        std::cout << "Writing ::ShapeTypeExtended, count " << samples_written << std::endl;

        writer.write(data);

        // Send once every second
        rti::util::sleep(dds::core::Duration(1));
    }
}

int main(int argc, char *argv[])
{

    using namespace application;

    // Parse arguments and handle control-C
    auto arguments = parse_arguments(argc, argv);
    if (arguments.parse_result == ParseReturn::exit) {
        return EXIT_SUCCESS;
    } else if (arguments.parse_result == ParseReturn::failure) {
        return EXIT_FAILURE;
    }
    setup_signal_handlers();

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    try {
        run_publisher_application(arguments.domain_id, arguments.sample_count, arguments.config_peers);
    } catch (const std::exception& ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in run_publisher_application(): " << ex.what()
        << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application exit
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}
