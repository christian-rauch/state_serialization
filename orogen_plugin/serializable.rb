class TaskSerializationPlugin <  OroGen::Spec::TaskModelExtension

    attr_accessor :archive_type
    
    def register_for_generation(task)
        # header for polymorphic archives
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_oarchive.hpp>")
        task.add_base_header_code("#include <boost/archive/polymorphic_#{archive_type}_iarchive.hpp>")
        task.add_base_header_code("#include <fstream>")
        
        # operations for getting/setting state via raw data or file
        getstate_body =
'    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);
    boost::archive::polymorphic_'+archive_type+'_oarchive oa(ss);
    
    serialize(oa);
    
    std::vector<boost::int8_t> state;
    const std::streampos s_end = ss.tellp();
    state.resize(s_end);
    
    ss.read(reinterpret_cast<char*>(state.data()), s_end);
    
    return state;'

        setstate_body =
'    std::stringstream ss(std::ios::binary | std::ios::in | std::ios::out);
    ss.write(reinterpret_cast<const char*>(data.data()), data.size());
    
    boost::archive::polymorphic_'+archive_type+'_iarchive ia(ss);
    
    deserialize(ia);'
        
        getstatefile_body =
'    const std::string statepath = path.empty() ? tmpnam(NULL) : path;

    std::ofstream ofs(statepath.c_str());
    boost::archive::polymorphic_'+archive_type+'_oarchive oa(ofs);
    
    serialize(oa);
    
    return statepath;'
        
        setstatefile_body =
'    std::ifstream ifs(path.c_str());
    boost::archive::polymorphic_'+archive_type+'_iarchive ia(ifs);
    
    deserialize(ia);'
        
        task.hidden_operation('getState', getstate_body).
            returns('/std/vector<char>').
            register_for_generation
        task.hidden_operation('setState', setstate_body).
            argument('data', '/std/vector<char>').
            register_for_generation
        
        task.hidden_operation('getStateFile', getstatefile_body).
            argument('path', 'string').
            returns('string').
            register_for_generation
        task.hidden_operation('setStateFile', setstatefile_body).
            argument('path', 'string').
            register_for_generation
        
        # virtual methods for de-/serialization in users tasks
        task.add_base_method("void", "serialize", "boost::archive::polymorphic_oarchive &oa")
        task.add_base_method("void", "deserialize", "boost::archive::polymorphic_iarchive &ia")
    end
end

class Orocos::Spec::TaskContext
    def serializable(archive_type)
        plugin_name = "TaskSerializationPlugin"
        if !find_extension(plugin_name)
            plugin = TaskSerializationPlugin.new(plugin_name)
            plugin.archive_type = archive_type
            register_extension(plugin)
        end
    end
end

