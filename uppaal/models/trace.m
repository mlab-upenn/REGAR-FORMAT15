       
dos('set UPPAAL_COMPILE_ONLY=1');
dos('verifyta ../models/STTT.xml - > ../models/model.if');
[x,y]=dos('tracer ../models/model.if ../traces/R0_MS.xtr');
