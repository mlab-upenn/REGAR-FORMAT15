param nvertices integer >= 1;
param nedges    integer >= 1;
param nRules    integer >= 1;
param N         integer >= 1;
set ZERO; 

set ELEMENTS   ;
set RULES      ;
set SUBGRAPHS  ;
set TIME       ;
set TIME0      ;

# The initial values of vertex parameters
param Px0 {ELEMENTS} >= 0;
# Terms for the linear predicate Phi
param C {RULES};
param B {RULES, ELEMENTS};
# Preserved vertices
set VPHI within ELEMENTS;
# Element list of induced subgraphs (element = vertex or edge) 
# (s,x) means x (given by its identity in G) is in subgraph Ks
set KX within {SUBGRAPHS, ELEMENTS};
# (R,Ks) in KRULES means that Ks is a stage for R
set KRULES within {RULES, SUBGRAPHS};
# Stages destroyed by application of R_i
set DESTROYED {KRULES} within {SUBGRAPHS};
# Abstractness for every rule
param deltaRV {RULES} integer >= 0;
param deltaRE {RULES} integer >= 0;

# (i,x) in FUCKED means that vertex x (given by its identity in G) is disabled by applying Ri.
# This is regardless of which subgraph Ri is applied to, we're looking at all ogf them
#set FUCKED  = {(i,x) in RULES cross ELEMENTS: 
#					exists {subgraph in setof {(i,s) in KRULES} s} (subgraph,x) in KX 
#               };

# optimization var: 
var a {KRULES, TIME0} binary := 0;

# Parameter update equations
# These have to be input manually since the update functions change and we can not model that in AMPL (or generated from MATLAB)
# Specifically, there will be one such equation for every x and i, as we need one for every f_{i,k}
# The following is an example expression where f_{i,k} is the max function
set mySUBGRAPHS {x in ELEMENTS} = {s in SUBGRAPHS : (s,x) in KX};
#var Px {(x,j) in ELEMENTS cross 1..N} = Px[x,j] + sum {(i,s) in KRULES: s in mySUBGRAPHS[x]}  a[i,s,j-1]*(-Px[x,j] + max( Px[x,j], Px[x, j-1]));
var Px {(x,j) in ELEMENTS cross TIME0} = if j = 0 then Px0[x] else Px[x,j-1] + sum {(i,s) in KRULES: s in mySUBGRAPHS[x]}  a[i,s,j-1]*(-Px[x,j-1] + max( 430, Px[x, j-1]));

# Abstracness per stage
var deltaV {j in TIME} = sum {i in RULES} (deltaRV[i]*(sum{s in SUBGRAPHS: (i,s) in KRULES} a[i,s,j]) );
var deltaE {j in TIME} = sum {i in RULES} (deltaRE[i]*(sum{s in SUBGRAPHS: (i,s) in KRULES} a[i,s,j]) );

minimize abstractness: 
	nvertices + nedges - sum {j in TIME} (deltaV[j] + deltaE[j]) ;

subject to One_rule_every_time {j in TIME}:
	sum{(i,s) in KRULES} a[i,s,j] = 1;

subject to Disabling_by_structure {(i,s) in KRULES, j in TIME}:
	a[i,s,j]*(sum {jp in {j+1..N}, ip in RULES, sp in DESTROYED[i,s] : (ip,sp) in KRULES  }
	               a[ip,sp,jp]) = 0;

# subject to Precedence:


subject to Applicability_conditions {(i,s) in KRULES, j in TIME}:
	a[i,s,j]*(sum {x in {setof {(s,v) in KX} v} } B[i,x]*Px[x, j] - C[i]) <= 0;

# Model appropriateness
set KX2{s in SUBGRAPHS} = {x in ELEMENTS : (s,x) in KX};
set PROHIBITED_APPS = {(i,s) in KRULES : 
				exists {sp in DESTROYED[i,s]} exists {v in KX2[sp]} v in VPHI
				};
subject to Model_appropriateness {j in TIME, (i,s) in PROHIBITED_APPS}:
	a[i,s,j]=0;
	


