if(path){
  process.chdir(folder_origin);
}

function vecFromVal(vectorVal){
  var output = [];
  for(var i=0; i<vectorVal.size(); i++){
    output.push(vectorVal.get(i));
  }
  return output;
}

function matrixFromVal(matrixVal){
  var output = [];
  for(var i=0; i<matrixVal.size(); i++){
    output.push(vecFromVal(matrixVal.get(i)));
  }
  return output;
}

var utils = {
  vecFromVal: vecFromVal,
  matrixFromVal: matrixFromVal
};

Module.utils = utils;
