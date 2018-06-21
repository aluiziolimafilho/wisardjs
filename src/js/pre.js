

var path = false;

if(process){
  var folder_origin = process.cwd();
  var node_module_path = 'node_modules/@iazero/wisardjs/dist';
  var dist_path = 'dist';
  try {
    process.chdir(node_module_path);
    path = true;
  } catch(e){
    try {
      process.chdir(dist_path);
      path = true;
    } catch(e){}
  }
}
