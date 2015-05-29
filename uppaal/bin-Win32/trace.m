setenv('UPPAAL_COMPILE_ONLY','1');
setenv('UPPAAL_OLD_SYNTAX','false');
dos('verifyta ../models/testing_DDD.xml - > ../models/model.if');
[x,y]=dos('tracer ../models/model.if testresult-1.xtr >../traces/test_DDD.txt');
% fid=fopen('../traces/trace3.txt','w');
% fprintf(fid,'%s',y);
% fclose(fid);
