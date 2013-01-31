/* 
 * File:   Config.h
 * Author: bruijl
 *
 * Created on January 31, 2013, 10:51 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#define READ_CONF(name, def) Config::getInstance().getPropertyTree().get(name, def)

/**
 * Singleton configuration file. All modules are allowed to request specific
 * data from this class.
 */
class Config {
private:

    Config() {

    }
    Config(Config const& copy); // not implemented
    Config& operator=(Config const& copy); // not implemented

    boost::property_tree::ptree pt;
public:

    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    /**
     * Reads a configuration file. Should be in the INI format.
     * @param filename Filename
     */
    void parseConfiguration(const char* filename) {
        std::cout << "Reading configuration file " << filename << std::endl;
        boost::property_tree::ini_parser::read_ini(filename, pt);
    }
    
    const boost::property_tree::ptree& getPropertyTree() {
        return pt;
    }
};


#endif	/* CONFIG_H */

