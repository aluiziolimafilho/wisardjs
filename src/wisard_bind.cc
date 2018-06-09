#include <emscripten/bind.h>

#include "wisardpkg.h"

namespace em = emscripten;


float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

EMSCRIPTEN_BINDINGS(wisardpkg)
{

    // binarizations
    // em::class_<KernelCanvas>(m, "KernelCanvas")
    //   .def(em::init<int, int>())
    //   .def(em::init<int, int, int>())
    //   .def("show", &KernelCanvas::show)
    //   .def("__call__", (vector<int>& (KernelCanvas::*)(const vector<int>&)) &KernelCanvas::operator())
    //   .def("__call__", (vector<vector<int>>& (KernelCanvas::*)(const vector<vector<int>>&)) &KernelCanvas::operator())
    // ;
    //
    // em::class_<AverageEntry>(m, "AverageEntry")
    //   .def(em::init())
    //   .def("__call__", (vector<int>& (AverageEntry::*)(const vector<int>&)) &AverageEntry::operator())
    //   .def("__call__", (vector<double>& (AverageEntry::*)(const vector<double>&)) &AverageEntry::operator())
    // ;
    //
    // //sythesizer
    // em::class_<Synthesizer>(m, "Synthesizer")
    //   .def(em::init<vector<int>>())
    //   .def("make", &Synthesizer::make)
    // ;
    //
    // //base to models
    // em::class_<Discriminator>(m, "Discriminator")
    //   .def(em::init<int,int,em::kwargs>())
    //   .def("train", (void (Discriminator::*)(const vector<int>&)) &Discriminator::train)
    //   .def("train", (void (Discriminator::*)(const vector<vector<int>>&)) &Discriminator::train)
    //   .def("classify", &Discriminator::getVotes)
    // ;

    // models
    em::function("lerp", &lerp);

    em::class_<Wisard>("Wisard")
      .constructor<int>()
      .function("train", (void (Wisard::*)(const std::vector<vector<int>>&, const vector<string>&)) &Wisard::train)
      //.function("classify", (em::list (Wisard::*)(const vector<vector<int>>&, em::kwargs)) &Wisard::classify)
      .function("getMentalImages", &Wisard::getMentalImages)
    ;

    // em::class_<ClusWisard>(m, "ClusWisard")
    //   .def(em::init<int, float, int, int, em::kwargs>())
    //   .def("train", (void (ClusWisard::*)(const vector<vector<int>>&, const vector<string>&)) &ClusWisard::train)
    //   .def("train", (void (ClusWisard::*)(const vector<vector<int>>&, map<int, string>&)) &ClusWisard::train)
    //   .def("trainUnsupervised", &ClusWisard::trainUnsupervised)
    //   .def("classify", (vector<string>& (ClusWisard::*)(const vector<vector<int>>&)) &ClusWisard::classify)
    //   .def("classifyUnsupervised", (vector<string>& (ClusWisard::*)(const vector<vector<int>>&)) &ClusWisard::classifyUnsupervised)
    //   .def("getMentalImage", &ClusWisard::getMentalImage)
    //   .def("getMentalImages", &ClusWisard::getMentalImages)
    // ;

}
