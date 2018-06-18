#include <emscripten/bind.h>

#include "wisardpkg.h"

namespace em = emscripten;


EMSCRIPTEN_BINDINGS(wisardpkg)
{
    // basic types
    em::register_vector<string>("VectorStr");
    em::register_vector<int>("VectorInt");

    // complex types
    em::register_vector<vector<int>>("MatrixInt");
    em::register_map<string,vector<int>>("MentalImage");
    em::register_map<string,vector<vector<int>>>("MentalImages");

    // binarizations
    em::class_<KernelCanvas>("KernelCanvas")
      .constructor<int, int>()
      .constructor<int, int, int>()
      .function("show", &KernelCanvas::show)
      .function("__call__", (vector<vector<int>>& (KernelCanvas::*)(const em::val&)) &KernelCanvas::operator())
    ;

    em::class_<AverageEntry>("AverageEntry")
      .constructor()
      .function("__call__", (vector<int>& (AverageEntry::*)(const vector<int>&)) &AverageEntry::operator())
      .function("__call__", (vector<double>& (AverageEntry::*)(const vector<double>&)) &AverageEntry::operator())
    ;

    //sythesizer
    em::class_<Synthesizer>("Synthesizer")
      .constructor<vector<int>>()
      .function("make", &Synthesizer::make)
    ;

    // //base to models
    em::class_<Discriminator>("Discriminator")
      .constructor<int,int>()
      .constructor<int,int,const em::val&>()
      .function("train", (void (Discriminator::*)(const vector<int>&)) &Discriminator::train)
      .function("train", (void (Discriminator::*)(const vector<vector<int>>&)) &Discriminator::train)
      .function("classify", &Discriminator::getVotes)
    ;

    // models
    em::class_<Wisard>("Wisard")
      .constructor<int>()
      .constructor<int, const em::val&>()
      .function("train", (void (Wisard::*)(const em::val&, const em::val&)) &Wisard::train)
      .function("classify", (vector<string> (Wisard::*)(const em::val&)) &Wisard::classify)
      .function("getMentalImages", &Wisard::getMentalImages)
    ;

    em::class_<ClusWisard>("ClusWisard")
      .constructor<int, float, int, int>()
      .constructor<int, float, int, int, const em::val&>()
      .function("train", (void (ClusWisard::*)(const em::val&, const em::val&)) &ClusWisard::train)
      .function("trainUnsupervised", &ClusWisard::trainUnsupervised)
      .function("classify", (vector<string>& (ClusWisard::*)(const em::val&)) &ClusWisard::classify)
      .function("classifyUnsupervised", (vector<string>& (ClusWisard::*)(const em::val&)) &ClusWisard::classifyUnsupervised)
      .function("getMentalImage", &ClusWisard::getMentalImage)
      .function("getMentalImages", &ClusWisard::getMentalImages)
    ;

}
