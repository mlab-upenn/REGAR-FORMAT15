param nvertices integer >= 1;
param nedges   integer >= 1;
param nRules   integer >= 1;
param nStages integer >= 1;
param N integer >= 1;

set VERTICES  = 1..nvertices;
set RULES        = 1..nRules;
set STAGES      = 1..nStages;
set SUBGRAPHS   = 1..nStages;
set TIME            = 1..N;

# Problem data
set XI        within {VERTICES, RULES};
# Preserved vertices
set VPHI within VERTICES;
# (R,Ks) in KRULES means that Ks is a stage for R
set KRULES within {RULES, SUBGRAPHS};


# rho3(x)  = set of stages where x plays a role  <==> (x,s) in rho3 iff x plays a role on s
set RHO3 within {VERTICES, STAGES};

param ROLES {x in VERTICES} ; 

# <i>
set AFFECTED_ROLES dimen 2;

# Stages destroyed by application of R_i
set DESTROYED within {RULES, SUBGRAPHS};
 
# Parameter update equations
# These have to be input manually since the update functions change and we can not model that in AMPL (or generated from MATLAB)
# Specifically, there will be one such equation for every x and i, as we need one for every f_{i,k}
# The following is an example expression where f_{i,k} is the max function

var Px {XI, TIME};
# var Px[x,i,j] = sum {s in mySTAGES[x]}  a[i,j-1,s]*max( Px[x,i,j], Px[x,i, j-1], Px[x+1, i,  j-1])
# var Px[x,i,j] = sum {s in SUBGRAPHS}           a[i,j-1,s]*Indicator[s,x]*max( Px[x,i, j-1], Px[x+1, i,  j-1])

#var Ps {s in 1..nStages, j in 1..N} = 

# optimization var: 
var a {KRULES, TIME} binary := 0;

param deltaRV {RULES} integer >= 0;
param deltaRE {RULES} integer >= 0;
# Abstracness per stage
var deltaV {j in 1..N} = sum {i in RULES} (deltaRV[i]*(sum{s in SUBGRAPHS} a[i,s,j]) );
var deltaE {j in 1..N} = sum {i in RULES} (deltaRE[i]*(sum{s in SUBGRAPHS} a[i,s,j]) );

minimize abstractness: 
	nvertices + nedges - sum {j in TIME} (deltaV[j] + deltaE[j]) ;

subject to One_rule_every_time {j in TIME}:
	sum{(i,s) in KRULES} a[i,s,j] = 1;

subject to Disabling_by_structure {i in 1..nRules, j in 1..N, s in 1..nStages}:
	a[i,j,s]*(sum {(ip,sp) in DESTROYED, jp in {j+1..N} } a[ip,sp,jp]) = 0;

# subject to Precedence:

#subject to Applicability_conditions {j in TIME,  s in 1..nStages, i in 1..nRules}:
#	a[i,s,j]*(B[i]*Ps[s, j] - C[i]) <= 0;

# Model appropriateness
# Real (in G) vertices disabled by an application of Ri
set FUCKED within {RULES, VERTICES};
set PRESERVED = VPHI inter {i: (i,s) in FUCKED};
subject to Model_appropriateness {j in TIME, (i,s) in FUCKED}:
	a[i,s,j]=0;
	



# Time domain for maximization
#set max_domain =
#(if s >= 0 then
#	1..N-s
#else
#	1-s..N
#);

