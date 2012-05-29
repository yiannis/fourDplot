#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

enum Mode {
  PLOT,
  IMAGE,
};

class Arguments {
  private:
    po::options_description m_optsDesc;
    po::variables_map m_varMap;

  public:
    Arguments();
    void parse(int argc, char** argv);
    void printHelp() const;
    void printGivenValues() const;
    void printAllValues() const;

    Mode mode;
    int points, drawMode, duration;
    float xmin, xmax, ymin, ymax, resample;
    bool lights, axis, falseColors, imageColors;
    bool renderVideo, verbose, loop, help;
    std::string drawModeStr, saveDir, textureImg, functionCode;
    std::vector<std::string> images;
};

#endif // ARGUMENTS_H
