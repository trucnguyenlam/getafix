/*************************************************************************************************/
/********************                 DESIGN FOR 3 THREADS            ****************************/
/*************************************************************************************************/
#timer go;

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/******                                                                                    *******/
/******                               Reachability Algorithm                               *******/
/******                                                                                    *******/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
mu bool Init_Reach(
 PrCount s_pc,      // Program counter
 Local   s_CL,      // local variable
 Global  s_G        // global variable
)
 s_pc    <  s_CL,
 s_CL    <  s_G
(false
  // initial conf
  | initPC(s_pc)    // simply mean init PC
  // forward propagation on internal transitions
  |  ( exists
           PrCount t_pc,
           Local   t_CL,
           Global  t_G.
           (   Init_Reach( t_pc, t_CL, t_G )
               // &(t_G.v1=0 | ( t_G.v1=1 & t_CL.v0=1 ) )
               &( ( programInt1(0, t_pc, s_pc, t_CL, s_CL, t_G, s_G )    // Init module
                    & CopyVariables_ProgramInt( 2, t_pc, t_CL, s_CL, t_G, s_G )
                  )
                  | programInt3(0, t_pc, s_pc, t_CL, s_CL, t_G, s_G )      // Init module
                )
           )
     )

  | ( exists PrCount t_pc.
           (     Init_Reach( t_pc, s_CL, s_G )
               // &(s_G.v1=0 | ( s_G.v1=1 & s_CL.v0=1 ) )
               & programInt2(0, t_pc, s_pc, s_CL, s_G )                // Transitions that only use control flow
           )
      )

);
#size Init_Reach;     //? Show the size of formula
#timer;


// ###   #  #  ###   ###
//  #    ## #   #     #
//  #    ## #   #     #
//  #    # ##   #     #
//  #    # ##   #     #
// ###   #  #  ###    #

bool GlobalInit(Global CG)           // Call Init on Global (shared) variable
( exists
         Module  s_mod,
         PrCount s_pc,
         Local   s_CL.
              (   Init_Reach( s_pc, s_CL, CG )
                & Target_Init(s_pc )
              )
);

// ###   #  #  ###   ###    ##         ###   ###    ##   #  #   ##   ###   ###   ###    ##   #  #
//  #    ## #   #    #  #  #  #         #    #  #  #  #  ## #  #  #   #     #     #    #  #  ## #
//  #    ## #   #    #  #  #  #         #    #  #  #  #  ## #   #     #     #     #    #  #  ## #
//  #    # ##   #    ###   ####  ####   #    ###   ####  # ##    #    #     #     #    #  #  # ##
//  #    # ##   #    # #   #  #         #    # #   #  #  # ##  #  #   #     #     #    #  #  # ##
// ###   #  #   #    #  #  #  #         #    #  #  #  #  #  #   ##   ###    #    ###    ##   #  #

bool programInt1_1(        // For module1
 PrCount cp,
 PrCount dp,
 Local   L,
 Local   dL,
 Global  G,
 Global  dG
)
 cp  ~+ dp,
 cp  <  L,
 L   ~+ dL,
 L   <  G,
 G   ~+ dG
(
  programInt1(1, cp, dp, L, dL, G, dG)
);
#size programInt1_1;
#timer;

bool programInt1_2(        // For module2
 PrCount cp,
 PrCount dp,
 Local   L,
 Local   dL,
 Global  G,
 Global  dG
)
 cp  ~+ dp,
 cp  <  L,
 L   ~+ dL,
 L   <  G,
 G   ~+ dG
(
  programInt1(2, cp, dp, L, dL, G, dG)
);
#size programInt1_2;
#timer;

bool programInt1_3(        // For module3
 PrCount cp,
 PrCount dp,
 Local   L,
 Local   dL,
 Global  G,
 Global  dG
)
 cp  ~+ dp,
 cp  <  L,
 L   ~+ dL,
 L   <  G,
 G   ~+ dG
(
  programInt1(3, cp, dp, L, dL, G, dG)
);
#size programInt1_3;
#timer;

bool programInt2_1(        // For module1
 PrCount cp,
 PrCount dp,
 Local   L,
 Global  G
)
 cp  ~+ dp,
 cp  <  L,
 L  <  G
(
  programInt2(1, cp, dp, L, G)
);
#size programInt2_1;
#timer;

bool programInt2_2(        // For module2
 PrCount cp,
 PrCount dp,
 Local   L,
 Global  G
)
 cp  ~+ dp,
 cp  <  L,
 L  <  G
(
  programInt2(2, cp, dp, L, G)
);
#size programInt2_2;
#timer;

bool programInt2_3(        // For module3
 PrCount cp,
 PrCount dp,
 Local   L,
 Global  G
)
 cp  ~+ dp,
 cp  <  L,
 L  <  G
(
  programInt2(3, cp, dp, L, G)
);
#size programInt2_3;
#timer;

bool CopyVariables_ProgramInt_1(
 PrCount p,
 Local   cL,
 Local   dL,
 Global  cG,
 Global  dG
)
 p  <  cL,
 cL  ~+ dL,
 cL  <  cG,
 cG  ~+ dG
(
  CopyVariables_ProgramInt(1, p, cL, dL, cG, dG)
);
#size CopyVariables_ProgramInt_1;
#timer;

bool CopyVariables_ProgramInt_2(
 PrCount p,
 Local   cL,
 Local   dL,
 Global  cG,
 Global  dG
)
 p  <  cL,
 cL  ~+ dL,
 cL  <  cG,
 cG  ~+ dG
(
  CopyVariables_ProgramInt(2, p, cL, dL, cG, dG)
);
#size CopyVariables_ProgramInt_2;
#timer;

bool CopyVariables_ProgramInt_3(
 PrCount p,
 Local   cL,
 Local   dL,
 Global  cG,
 Global  dG
)
 p  <  cL,
 cL  ~+ dL,
 cL  <  cG,
 cG  ~+ dG
(
  CopyVariables_ProgramInt(3, p, cL, dL, cG, dG)
);
#size CopyVariables_ProgramInt_3;
#timer;

bool FakeOrderingPCLocalGlobal(
 PrCount    t_pc1,
 PrCount    s_pc1,
 Local      t_CL1,
 Local      s_CL1,
 PrCount    t_pc2,
 PrCount    s_pc2,
 Local      t_CL2,
 Local      s_CL2,
 PrCount    t_pc3,
 PrCount    s_pc3,
 Local      t_CL3,
 Local      s_CL3,
 Global     t_G,
 Global     s_G
)
 s_pc1    ~+   t_pc1,
 s_pc1    <    s_CL1,
 s_CL1    ~+   t_CL1,
 s_CL1    <    s_pc2,

 s_pc2    ~+   t_pc2,
 s_pc2    <    s_CL2,
 s_CL2    ~+   t_CL2,
 s_CL2    <    s_pc3,

 s_pc3    ~+   t_pc3,
 s_pc3    <    s_CL3,
 s_CL3    ~+   t_CL3,
 s_CL3    <    s_G,

 s_G      ~+   t_G
(true
  );

bool FakeOrderingPCLocal(
 PrCount    t_pc1,                  // Program counter
 PrCount    s_pc1,                  // Program counter
 Local      t_CL1,
 Local      s_CL1,                  // Local variable
 PrCount    t_pc2,                  // Program counter
 PrCount    s_pc2,                  // Program counter
 Local      t_CL2,                  // Local variable
 Local      s_CL2,                  // Local variable
 PrCount    t_pc3,                  // Program counter
 PrCount    s_pc3,                  // Program counter
 Local      t_CL3,                  // Local variable
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    ~+  t_pc1,
 s_pc1    <  s_CL1,
 s_CL1    ~+  t_CL1,
 s_CL1    <  s_pc2,

 s_pc2    ~+  t_pc2,
 s_pc2    <  s_CL2,
 s_CL2    ~+  t_CL2,
 s_CL2    <  s_pc3,

 s_pc3    ~+  t_pc3,
 s_pc3    <  s_CL3,
 s_CL3    ~+  t_CL3,
 s_CL3    <  s_G

(true
);



bool FakeOrderingLocalGlobal(
 PrCount    s_pc1,                  // Program counter
 Local      t_CL1,
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      t_CL2,                  // Local variable
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      t_CL3,                  // Local variable
 Local      s_CL3,                  // Local variable
 Global     t_G,                    // Global variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    ~+  t_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    ~+  t_CL2,
 s_CL2    <  s_pc3,
 s_CL3    ~+  t_CL3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G,
 s_G      ~+  t_G
(true
);

bool FakeOrderingLocal(
 PrCount    s_pc1,                  // Program counter
 Local      t_CL1,
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      t_CL2,                  // Local variable
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      t_CL3,                  // Local variable
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    ~+  t_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    ~+  t_CL2,
 s_CL2    <  s_pc3,
 s_CL3    ~+  t_CL3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
(true
);

// Formula Learning
#load formula_layer0;

#load formula_abstraction_layer1;
#load formula_layer1;
#load formula_abstraction_layer2;
#load formula_layer2;

#load formula_layer3;

#load formula_query_layer3;

#timer stop;

