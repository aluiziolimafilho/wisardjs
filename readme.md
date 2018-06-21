# wisardjs

## Description
This is a project to make available the different models based on WiSARD, with high performance, easy to use and to install 
and following a pattern of use. These provided models are machine learning models, with supervised, unsupervised and 
semi-supervised learning.

## to install

    npm i @iazero/wisardjs

## to use

### WiSARD

```javascript
    var lib = require('@iazero/wisardjs'); // import the module
    
    var w = new lib.Wisard(3); // instanciate a wisard object
    // The first parameter is the address size
    // the second parameter is optional as {verbose: true} can be passed
    
    // load input data, just zeros and ones  
    var X = [
          [1,1,1,0,0,0,0,0],
          [1,1,1,1,0,0,0,0],
          [0,0,0,0,1,1,1,1],
          [0,0,0,0,0,1,1,1]
        ];

    // load label data, which must be a string array
    var y = [
          "cold",
          "cold",
          "hot",
          "hot"
        ];
        
     w.train(X,y); // train data
     
     var output_val = w.classify(X); // classify data
     
     var output = lib.utils.vecFromVal(output_val); // convert the output_val object to a common vector in js;
```

### ClusWisard

```javascript
    var lib = require('@iazero/wisardjs'); // import the module
    
    var clus = new lib.ClusWisard(3, 0.1, 10, 3); // instanciate a wisard object
    // The first parameter is the address size
    // The second parameter is the min score accept to train
    // The third parameter is the maximum number of trainings in each class
    // The forth parameter is the maximum number of discriminator by cluster, for limitate the cosume of memory.
    // the last parameter is optional as {verbose: true} can be passed
    
    // load input data, just zeros and ones  
    var X = [
          [1,1,1,0,0,0,0,0],
          [1,1,1,1,0,0,0,0],
          [0,0,0,0,1,1,1,1],
          [0,0,0,0,0,1,1,1]
        ];

    // load label data, which must be a string array
    var y = [
          "cold",
          "cold",
          "hot",
          "hot"
        ];
        
     clus.train(X,y); // train data supervised
     
     // load label data, for semi-supervised learning
     var y2 = {
        '1': "cold",
        '3': "hot"
     };
      
     clus.train(X,y2); // train data semi-supervised
      
     // or you can train unsupervised too
     clus.trainUnsupervised(X);
     
     var output_val = clus.classify(X); // classify data to supervised and semi-supervised
     
     var output_val = clus.classifyUnsupervised(X); // classify data to unsupervised
     
     var output = lib.utils.vecFromVal(output_val); // convert the output_val object to a common vector in js;
```

## to uninstall

    npm uninstall @iazero/wisardjs

## Documentation
documentation in development...
