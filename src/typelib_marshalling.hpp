#ifndef SERIALIZATION_TYPELIB_HPP
#define SERIALIZATION_TYPELIB_HPP

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
    
    orogen_transports::TypelibMarshallerBase::Handle *data_handle = marshaller->createHandle();
    marshaller->setOrocosSample(data_handle, &data);

    std::vector<uint8_t> *data_serialized = new std::vector<uint8_t>();
    
    try {
        marshaller->marshal(*data_serialized, data_handle);
    }
    catch(std::exception& e) {
        LOG_ERROR_S<<"Exception during marshalling: "<<e.what();
    }
    
    marshaller->deleteHandle(data_handle);
    
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
    
    LOG_DEBUG_S<<"Marshalled type: "<<marshaller->getMarshallingType();
    
    Type *data = new Type();
    
    orogen_transports::TypelibMarshallerBase::Handle *data_serialized_handle = marshaller->createHandle();
    marshaller->setOrocosSample(data_serialized_handle, data);
    
    try {
        marshaller->unmarshal(data_serialized, data_serialized_handle);
    }
    catch(std::exception& e) {
        LOG_ERROR_S<<"Exception during unmarshalling: "<<e.what();
    }
    
    marshaller->deleteHandle(data_serialized_handle);
    
    return data;
};

}

#endif // SERIALIZATION_TYPELIB_HPP
