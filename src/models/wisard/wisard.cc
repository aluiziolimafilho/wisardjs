#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>

using namespace std;

namespace em = emscripten;

class Wisard{
public:
  Wisard(int addressSize): addressSize(addressSize){
    bleachingActivated=true;
    verbose=false;
    ignoreZero=false;
    completeAddressing=true;
    indexes=vector<int>(0);
    base=2;
    srand(randint(0,1000000));
  }
  Wisard(int addressSize, em::val kwargs): addressSize(addressSize){
    bleachingActivated=true;
    verbose=false;
    ignoreZero=false;
    completeAddressing=true;
    indexes=vector<int>(0);
    base=2;
    srand(randint(0,1000000));

    if(kwargs.hasOwnProperty("verbose")){
      verbose = kwargs["verbose"].as<bool>();
    }
  }
  // Wisard(int addressSize, py::kwargs kwargs): addressSize(addressSize){
  //   bleachingActivated=true;
  //   verbose=false;
  //   ignoreZero=false;
  //   completeAddressing=true;
  //   indexes=vector<int>(0);
  //   base=2;
  //
  //   for(auto arg: kwargs){
  //     if(string(py::str(arg.first)).compare("bleachingActivated") == 0)
  //       bleachingActivated = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("verbose") == 0)
  //       verbose = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("ignoreZero") == 0)
  //       ignoreZero = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("completeAddressing") == 0)
  //       completeAddressing = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("indexes") == 0)
  //       indexes = arg.second.cast<vector<int>>();
  //
  //     if(string(py::str(arg.first)).compare("base") == 0)
  //       base = arg.second.cast<int>();
  //   }
  // }

  void train(const vector<int>& image, const string& label){
    if(discriminators.find(label) == discriminators.end()){
      makeDiscriminator(label, image.size());
    }
    discriminators[label].train(image);
  }

  void train(const vector<vector<int>>& images, const vector<string>& labels){
    checkInputSizes(images.size(), labels.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rtraining " << i+1 << " of " << images.size();
      train(images[i], labels[i]);
    }
    if(verbose) cout << "\r" << endl;
  }

  map<string, int>& classify(const vector<int>& image, bool searchBestConfidence=false){
    map<string,vector<int>> allvotes;

    for(map<string,Discriminator>::iterator i=discriminators.begin(); i!=discriminators.end(); ++i){
      vector<int> votes = i->second.getVotes(image);
      allvotes[i->first] = votes;
    }
    return Bleaching::make(allvotes, bleachingActivated, searchBestConfidence);
  }

  vector<string> classify(const vector<vector<int>>& images){
    bool searchBestConfidence = false;
    vector<string> labels(images.size());
    for(unsigned int i=0; i<images.size(); i++){
      if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
      map<string,int> candidates = classify(images[i],searchBestConfidence);
      labels[i] = Bleaching::getBiggestCandidate(candidates);
    }
    return labels;
  }

  // py::list classify(const vector<vector<int>>& images, py::kwargs kwargs){
  //   bool searchBestConfidence=false;
  //   bool returnConfidence=false;
  //   bool returnActivationDegree=false;
  //
  //   for(auto arg: kwargs){
  //     if(string(py::str(arg.first)).compare("searchBestConfidence") == 0)
  //       searchBestConfidence = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("returnConfidence") == 0)
  //       returnConfidence = arg.second.cast<bool>();
  //
  //     if(string(py::str(arg.first)).compare("returnActivationDegree") == 0)
  //       returnActivationDegree = arg.second.cast<bool>();
  //   }
  //
  //   py::list labels(images.size());
  //   for(unsigned int i=0; i<images.size(); i++){
  //     if(verbose) cout << "\rclassifying " << i+1 << " of " << images.size();
  //     map<string,int> candidates = classify(images[i],searchBestConfidence);
  //     string aClass = Bleaching::getBiggestCandidate(candidates);
  //
  //     if(returnActivationDegree && !returnConfidence){
  //       float activationDegree = candidates[aClass]/(float)discriminators[aClass].getNumberOfRAMS();
  //       labels[i] = py::dict(py::arg("class")=aClass, py::arg("activationDegree")=activationDegree);
  //     }
  //     if(returnConfidence && !returnActivationDegree){
  //       float confidence = Bleaching::getConfidence(candidates, candidates[aClass]);
  //       labels[i] = py::dict(py::arg("class")=aClass, py::arg("confidence")=confidence);
  //     }
  //     if(returnActivationDegree && returnConfidence){
  //       float activationDegree = candidates[aClass]/(float)discriminators[aClass].getNumberOfRAMS();
  //       float confidence = Bleaching::getConfidence(candidates, candidates[aClass]);
  //       labels[i] = py::dict(py::arg("class")=aClass, py::arg("activationDegree")=activationDegree, py::arg("confidence")=confidence);
  //     }
  //     if(!returnActivationDegree && !returnConfidence){
  //       labels[i] = aClass;
  //     }
  //   }
  //   if(verbose) cout << "\r" << endl;
  //   return labels;
  // }

  map<string,vector<int>>& getMentalImages(){
    map<string,vector<int>>* images = new map<string,vector<int>>();
    for(map<string, Discriminator>::iterator d=discriminators.begin(); d!=discriminators.end(); ++d){
      (*images)[d->first] = d->second.getMentalImage();
    }
    return *images;
  }

  void setVerbose(bool v){
    verbose = v;
  }

  bool getVerbose(){
    return verbose;
  }

protected:
  void makeDiscriminator(string label, int entrySize){
    if(indexes.size()==0){
      discriminators[label] = Discriminator(addressSize, entrySize, ignoreZero, completeAddressing, false, base);
    }
    else{
      discriminators[label] = Discriminator(indexes, addressSize, entrySize, ignoreZero, base);
    }
  }

  void checkInputSizes(const int imageSize, const int labelsSize){
    if(imageSize != labelsSize){
      throw Exception("The size of data is not the same of the size of labels!");
    }
  }

private:
  int addressSize;
  bool bleachingActivated;
  bool verbose;
  vector<int> indexes;
  bool ignoreZero;
  bool completeAddressing;
  int base;
  map<string, Discriminator> discriminators;
};
