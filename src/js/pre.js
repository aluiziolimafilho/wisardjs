

var path = false;

if(typeof process != 'undefined'){
  var folder_origin = process.cwd();
  var node_module_path = __dirname;
  try {
    process.chdir(node_module_path);
    path = true;
  } catch(e){}
}
