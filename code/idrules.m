nV = 10;
nRules = 3;
% Model graph
G = getGraph(nv);
% H graphs and their roles
nRules = 3;
H = getCharacteristicGraphs(nRules);
% Induced subgraphs Ki
K = getInducedSubgraphs(H,G);
% Precedence relations between abstraction rules
prec = getPrecedenceRelations(H);
% rho(x) gives the triplets (i,k,s): x plays role k in rule i on subgraph s
rho = theatre(G,H,K);
paramUpdateFnts = getParamUpdateFnts(H);
% Conditions for rule applications


% Create the problem structures

    
    