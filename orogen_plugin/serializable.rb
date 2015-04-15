class TaskSerializationPlugin <  OroGen::Spec::TaskModelExtension

    attr_accessor :archive_type
    
    def register_for_generation(task)
        # header for polymorphic archives
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_oarchive.hpp>")
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_iarchive.hpp>")
        task.add_base_header_code("#include <fstream>")
        
        # virtual methods for de-/serialization in users tasks
        task.add_base_method("void", "serialize", "boost::archive::polymorphic_oarchive &oa")
        task.add_base_method("void", "deserialize", "boost::archive::polymorphic_iarchive &ia")
    end
    
    ## code for operations
    
    def op_getstate_body
        getstate_body =
'    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);
    boost::archive::polymorphic_'+archive_type+'_oarchive oa(ss);
    
    serialize(oa);
    
    std::vector<boost::int8_t> state;
    const std::streampos s_end = ss.tellp();
    state.resize(s_end);
    
    ss.read(reinterpret_cast<char*>(state.data()), s_end);
    
    return state;'
        return getstate_body
    end
    
    def op_setstate_body
        setstate_body =
'    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);
    ss.write(reinterpret_cast<const char*>(data.data()), data.size());
    
    boost::archive::polymorphic_'+archive_type+'_iarchive ia(ss);
    
    deserialize(ia);'
        return setstate_body
    end
    
    def op_getstatefile_body
        getstatefile_body =
'    const std::string statepath = path.empty() ? tmpnam(NULL) : path;

    std::ofstream ofs(statepath.c_str());
    boost::archive::polymorphic_'+archive_type+'_oarchive oa(ofs);
    
    serialize(oa);
    
    return statepath;'
        return getstatefile_body
    end
    
    def op_setstatefile_body
        setstatefile_body =
'    std::ifstream ifs(path.c_str());
    boost::archive::polymorphic_'+archive_type+'_iarchive ia(ifs);
    
    deserialize(ia);'
        return setstatefile_body
    end
    
end

class Orocos::Spec::TaskContext
    def serializable(archive_type)
        plugin_name = "TaskSerializationPlugin"
        if !find_extension(plugin_name)
            # plugin setup
            plugin = TaskSerializationPlugin.new(plugin_name)
            plugin.archive_type = archive_type
            
            #operations
            hidden_operation('getState', plugin.op_getstate_body).
                returns('/std/vector<char>')
            
            hidden_operation('setState', plugin.op_setstate_body).
                argument('data', '/std/vector<char>')
                
            hidden_operation('getStateFile', plugin.op_getstatefile_body).
                argument('path', 'string').returns('string')
                
            hidden_operation('setStateFile', plugin.op_setstatefile_body).
                argument('path', 'string')
            
            # generate code
            register_extension(plugin)
        end
    end
end

