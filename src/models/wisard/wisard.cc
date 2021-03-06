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
  Wisard(int addressSize, const em::val& kwargs=em::val::object()): addressSize(addressSize){
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
    if(kwargs.hasOwnProperty("bleachingActivated")){
      bleachingActivated = kwargs["bleachingActivated"].as<bool>();
    }
    if(kwargs.hasOwnProperty("ignoreZero")){
      ignoreZero = kwargs["ignoreZero"].as<bool>();
    }
    if(kwargs.hasOwnProperty("completeAddressing")){
      completeAddressing = kwargs["completeAddressing"].as<bool>();
    }
    if(kwargs.hasOwnProperty("base")){
      base = kwargs["base"].as<int>();
    }
    if(kwargs.hasOwnProperty("indexes")){
      indexes = em::vecFromJSArray<int>(kwargs["indexes"]);
    }
  }

  void train(const vector<int>& image, const string& label){
    if(discriminators.find(label) == discriminators.end()){
      makeDiscriminator(label, image.size());
    }
    discriminators[label].train(image);
  }

  void train(const em::val& images_val, const em::val& labels_val){
    vector<string> labels = em::vecFromJSArray<string>(labels_val);
    vector<vector<int>> images = matrixFromJSMatrix<int>(images_val);

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

  vector<string> classify(const em::val& images_val){

    vector<vector<int>> images = matrixFromJSMatrix<int>(images_val);
    vector<string> labels(images.size());
    bool searchBestConfidence = false;

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
