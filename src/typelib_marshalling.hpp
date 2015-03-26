#include <rtt/typelib/TypelibMarshaller.hpp>
#include <base/logging.h>

namespace serialization {

template<typename Type>
std::vector<uint8_t>* getMarshalledTypelib(const std::string type_name, Type& data) {
    orogen_transports::TypelibMarshallerBase *marshaller = 0;
    try {
        marshaller = orogen_transports::getMarshallerFor(type_name);
    }
    catch(std::exception& e) {
        LOG_ERROR_S<<"Exception when fetching marshaller: "<<e.what();
    }
    
    LOG_DEBUG_S<<"Marshalled type: "<<marshaller->getMarshallingType();
    
    orogen_transports::TypelibMarshallerBase::Handle data_handle(marshaller, &data);
    
    // do not destroy samples when Handle is deconstructed
    data_handle.owns_typelib = false;
    data_handle.owns_orocos = false;

    std::vector<uint8_t> *data_serialized = new std::vector<uint8_t>();
    
    try {
        marshaller->marshal(*data_serialized, &data_handle);
    }
    catch(std::exception& e) {
        LOG_ERROR_S<<"Exception during marshalling: "<<e.what();
    }
    
    return data_serialized;
};

template<typename Type>
Type* getUnmarshalledTypelib(const std::string type_name, std::vector<uint8_t>& data_serialized) {
    orogen_transports::TypelibMarshallerBase *marshaller = 0;
    try {
        marshaller = orogen_transports::getMarshallerFor(type_name);
    }
    catch(std::exception& e) {
        LOG_DEBUG_S<<"Exception when fetching marshaller: "<<e.what();
    }
    
    LOG_ERROR_S<<"Marshalled type: "<<marshaller->getMarshallingType();
    
    Type *data = new Type();
    
    orogen_transports::TypelibMarshallerBase::Handle data_serialized_handle(marshaller, data);
    
    // do not destroy samples when Handle is deconstructed
    data_serialized_handle.owns_typelib = false;
    data_serialized_handle.owns_orocos = false;
    
    try {
        marshaller->unmarshal(data_serialized, &data_serialized_handle);
    }
    catch(std::exception& e) {
        LOG_ERROR_S<<"Exception during unmarshalling: "<<e.what();
    }
    
    return data;
};

}
