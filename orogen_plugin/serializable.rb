class TaskSerializationPlugin <  OroGen::Spec::TaskModelExtension
    attr_accessor :archive_type
    
    def early_register_for_generation(task)
        # header for polymorphic archives
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_oarchive.hpp>")
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_iarchive.hpp>")
        task.add_base_header_code("#include <fstream>")
        
        # virtual methods for de-/serialization in users tasks
        task.add_base_method("void", "serialize", "boost::archive::polymorphic_oarchive &oa")
        task.add_base_method("void", "deserialize", "boost::archive::polymorphic_iarchive &ia")
        
        # implement operations
        
        getstate_body =
        "    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);\n"\
        "    boost::archive::polymorphic_#{archive_type}_oarchive oa(ss);\n"\
        "    \n"\
        "    serialize(oa);\n"\
        "    \n"\
        "    std::vector<boost::int8_t> state;\n"\
        "    const std::streampos s_end = ss.tellp();\n"\
        "    state.resize(s_end);\n"\
        "    \n"\
        "    ss.read(reinterpret_cast<char*>(state.data()), s_end);\n"\
        "    \n"\
        "    return state;"
        
        task.operations['getState'].
            base_body(getstate_body)
        
        
        setstate_body =
        "    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);\n"\
        "    ss.write(reinterpret_cast<const char*>(data.data()), data.size());\n"\
        "    \n"\
        "    boost::archive::polymorphic_#{archive_type}_iarchive ia(ss);\n"\
        "    \n"\
        "    deserialize(ia);"
    
        task.operations['setState'].
            base_body(setstate_body)
        
        
        getstatefile_body =
        "    const std::string statepath = path.empty() ? tmpnam(NULL) : path;\n"\
        "    \n"\
        "    std::ofstream ofs(statepath.c_str());\n"\
        "    boost::archive::polymorphic_#{archive_type}_oarchive oa(ofs);\n"\
        "    \n"\
        "    serialize(oa);\n"\
        "    \n"\
        "    return statepath;"
    
        task.operations['getStateFile'].
            base_body(getstatefile_body)
    
    
        setstatefile_body =
        "    std::ifstream ifs(path.c_str());\n"\
        "    boost::archive::polymorphic_#{archive_type}_iarchive ia(ifs);\n"\
        "    \n"\
        "    deserialize(ia);"
    
        task.operations['setStateFile'].
            base_body(setstatefile_body)
    
    end
end

class OroGen::Spec::TaskContext
    def serializable(archive_type)
        plugin_name = "TaskSerializationPlugin"
        if !find_extension(plugin_name)
            # plugin setup
            plugin = TaskSerializationPlugin.new(plugin_name)
            plugin.archive_type = archive_type
            
            # operations
            hidden_operation('getState').
                returns('/std/vector<char>')
            
            hidden_operation('setState').
                argument('data', '/std/vector<char>')
                
            hidden_operation('getStateFile').
                argument('path', 'string').returns('string')
                
            hidden_operation('setStateFile').
                argument('path', 'string')
            
            # generate code
            register_extension(plugin)
        end
    end
end

