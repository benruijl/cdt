/* 
 * File:   main.cpp
 * Author: ben
 *
 * Created on September 19, 2012, 3:01 PM
 */

#include <cstdlib>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Config.h"
#include "Simulation.h"
#include "observables/SizeObservable.h"
#include "observables/GridObservable.h"
#include "observables/TimeSliceObservable.h"
#include "moves/MoveFactory.h"
#include "observables/VolumeProfileObservable.h"
#include "observables/SpectralDimensionObservable.h"
#include "observables/ShapeObservable.h"
#include "observables/HausdorffObservable.h"

using namespace std;

struct ConfigStruct {
    double alpha, deltaVolume;
    unsigned int N, T, numSweeps, sweepLength,
            volume, sizeFreq, gridFreq, timeFreq, volProfFreq, specDimFreq,
            shapeFreq, haussFreq;
    std::string gridFile;
};

/**
 * Build a basic configuration structure.
 */
ConfigStruct buildConfiguration() {
    const boost::property_tree::ptree& pt = Config::getInstance().getPropertyTree();

    ConfigStruct config;
    config.N = pt.get<unsigned int>("general.N");
    config.T = pt.get<unsigned int>("general.T");
    config.alpha = pt.get<double>("general.alpha");
    config.volume = pt.get<unsigned int>("general.volume");
    config.deltaVolume = pt.get<double>("general.delta");
    config.numSweeps = pt.get<unsigned int>("general.numSweeps");
    config.sweepLength = pt.get<unsigned int>("general.sweepLength");
    config.gridFile = pt.get("general.gridFile", std::string(""));

    /* Check for observables */
    config.sizeFreq = pt.get("size.freq", 0);
    config.gridFreq = pt.get("grid.freq", 0);
    config.timeFreq = pt.get("time.freq", 0);
    config.volProfFreq = pt.get("vol.freq", 0);
    config.specDimFreq = pt.get("spec.freq", 0);
    config.shapeFreq = pt.get("shape.freq", 0);
    config.haussFreq = pt.get("hauss.freq", 0);

    return config;
}

int main(int argc, char** argv) {
    const char* configFile = "config.ini";
    if (argc > 2) {
        configFile = argv[1];
    }

    Config::getInstance().parseConfiguration(configFile);
    ConfigStruct config = buildConfiguration();

    Simulation simulation;

    /* Create observables */
    if (config.sizeFreq > 0) {
        SizeObservable* sizeObservable = new SizeObservable(config.sizeFreq, 0);
        simulation.addObservable(sizeObservable);
    }
    if (config.gridFreq > 0) {
        GridObservable* gridObservable = new GridObservable(simulation, config.gridFreq);
        simulation.addObservable(gridObservable);
    }

    if (config.timeFreq > 0) {
        TimeSliceObservable* timeSliceObservable = new TimeSliceObservable(config.timeFreq);
        simulation.addObservable(timeSliceObservable);

        if (config.volProfFreq > 0) {
            VolumeProfileObservable* volumeProfileObservable = new
                    VolumeProfileObservable(config.volProfFreq, timeSliceObservable,
                    &simulation);
            simulation.addObservable(volumeProfileObservable);
        }
    }

    if (config.specDimFreq > 0) {
        SpectralDimensionObservable* spectralDimensionObservable = new
                SpectralDimensionObservable(config.specDimFreq);
        simulation.addObservable(spectralDimensionObservable);
    }

    if (config.shapeFreq > 0) {
        ShapeObservable* shapeObservable = new
                ShapeObservable(config.shapeFreq);
        simulation.addObservable(shapeObservable);
    }

    if (config.haussFreq > 0) {
        HausdorffObservable* hausdorffObservable = new
                HausdorffObservable(&simulation, config.haussFreq);
        simulation.addObservable(hausdorffObservable);
    }

    if (config.gridFile.size() > 0) {
        simulation.readFromFile(config.gridFile.c_str());
    } else {
        simulation.generateInitialTriangulation(config.N, config.T);
    }

    simulation.Metropolis(config.alpha, config.volume, config.deltaVolume, config.numSweeps,
            config.sweepLength);
    std::cout << "Simulation ended." << std::endl;

    return 0;
}

