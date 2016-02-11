/*************************************************************************************************/
/********************                 DESIGN FOR 3 THREADS            ****************************/
/*************************************************************************************************/
#timer go;
/**
 * Compute first 2 rounds forward
 * Compute 4th round backward
 * Compute 3rd round backward with abstraction
 * Compute 3rd round forward with intersection with the configuration at the end of 2nd round
 */


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
(
  false

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


 // ####    ###   #   #  #   #  ####            #
 // #   #  #   #  #   #  #   #   #  #          # #
 // #   #  #   #  #   #  ##  #   #  #         #   #
 // ####   #   #  #   #  # # #   #  #         #   #
 // # #    #   #  #   #  #  ##   #  #         #   #
 // #  #   #   #  #   #  #   #   #  #          # #
 // #   #   ###    ###   #   #  ####            #



// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_0(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
  s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread1_0( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 1, t_pc1 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
           initPC(s_pc1)                 // Initial PC
         & GlobalInit(s_G)           // INIT runs FIRST
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_0( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL1.v0=1 ) )   // Start, so when it ends?
               &(
                  (
                    programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (

          true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL1.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc1.
             (     Thread1_0( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_0;
#timer;




 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_0(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread2_0( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 2, t_pc2 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
           initPC(s_pc2)                 // Initial PC
        &  Thread1_0( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_0( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL2.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL2.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc2.
             (     Thread2_0( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_0;
#timer;



// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_0(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread3_0( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 3, t_pc3 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
           initPC(s_pc3)                 // Initial PC
        &  Thread2_0( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_0( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL3.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL3.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc3.
             (     Thread3_0( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_0;
#timer;




 // ####    ###   #   #  #   #  ####                   #
 // #   #  #   #  #   #  #   #   #  #                 ##
 // #   #  #   #  #   #  ##  #   #  #                # #
 // ####   #   #  #   #  # # #   #  #                  #
 // # #    #   #  #   #  #  ##   #  #                  #
 // #  #   #   #  #   #  #   #   #  #                  #
 // #   #   ###    ###   #   #  ####                 #####







// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_1(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
  s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread1_1( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 1, t_pc1 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread3_0( s_pc1, s_CL1, s_pc2, s_CL2, s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_3(s_pc3)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_1( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL1.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (

          true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL1.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc1.
             (     Thread1_1( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_1;
#timer;



 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_1(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread2_1( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 2, t_pc2 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread1_1( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )





//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_1( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL2.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL2.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc2.
             (     Thread2_1( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_1;
#timer;







// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_1(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                      // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread3_1( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 3, t_pc3 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread2_1( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_1( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL3.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL3.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc3.
             (     Thread3_1( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_1;
#timer;



 // ####    ###   #   #  #   #  ####          #####
 // #   #  #   #  #   #  #   #   #  #             #
 // #   #  #   #  #   #  ##  #   #  #            #
 // ####   #   #  #   #  # # #   #  #           ##
 // # #    #   #  #   #  #  ##   #  #             #
 // #  #   #   #  #   #  #   #   #  #         #   #
 // #   #   ###    ###   #   #  ####           ###


// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_3(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  | (
      // target
      (
          target(1, s_pc1)
        | target(2, s_pc2)
        | target(3, s_pc3)
      )
    )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (
                  Thread3_3( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL3.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_3(s_pc3, t_pc3, s_CL3, t_CL3, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(s_pc3, s_CL3, t_CL3, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(3, s_pc3, t_pc3, s_CL3, t_CL3, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (
                Thread3_3( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, s_CL3, s_G )
                 & programInt2_3(s_pc3, t_pc3, s_CL3, s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_3;
#timer;

 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_3(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
          Thread3_3( s_pc1, s_CL1, s_pc2, s_CL2, s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
        &  ContextSwitch_3(s_pc3)
     )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_3( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL2.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_2(s_pc2, t_pc2, s_CL2, t_CL2, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(s_pc2, s_CL2, t_CL2, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(2, s_pc2, t_pc2, s_CL2, t_CL2, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL2.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc2.
             (     Thread2_3( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(s_pc2, t_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_3;
#timer;



// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_3(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
  s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (
        Thread2_3( s_pc1, s_CL1, s_pc2, s_CL2, s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
        &  ContextSwitch_2(s_pc2)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_3( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL1.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_1(s_pc1, t_pc1, s_CL1, t_CL1, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(s_pc1, s_CL1, t_CL1, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(1, s_pc1, t_pc1, s_CL1, t_CL1, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (

          true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL1.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc1.
             (     Thread1_3( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(s_pc1,t_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_3;
#timer;



 // ####    ###   #   #  #   #  ####           ###
 // #   #  #   #  #   #  #   #   #  #         #   #
 // #   #  #   #  #   #  ##  #   #  #             #
 // ####   #   #  #   #  # # #   #  #           ##
 // # #    #   #  #   #  #  ##   #  #          #
 // #  #   #   #  #   #  #   #   #  #         #
 // #   #   ###    ###   #   #  ####          #####


// BACKWARD



// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_2_backward(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global     s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  | (exists
        Local   t_CL1,
        Local   t_CL2,
        Local   t_CL3,
        Global  t_G.
        (
          Thread1_3(s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, t_G)
          & t_G.v1 = 0
        )
        & ContextSwitch_1(s_pc1)
        & ContextSwitch_3(s_pc3)
    )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (
                  Thread3_2_backward( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL3.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_3(s_pc3, t_pc3, s_CL3, t_CL3, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(s_pc3, s_CL3, t_CL3, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(3, s_pc3, t_pc3, s_CL3, t_CL3, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (
         true
       & (exists PrCount t_pc3.
             (
                Thread3_2_backward( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, s_CL3, s_G )
                 & programInt2_3(s_pc3, t_pc3, s_CL3, s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_2_backward;
#timer;

 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_2_backward(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  ( exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3,
          Global  t_G.
        (
          Thread3_2_backward( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, t_G )
          &  t_G.v1 = 0
        )
        &  ContextSwitch_2(s_pc2)
        &  ContextSwitch_3(s_pc3)
     )


//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_2_backward( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL2.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_2(s_pc2, t_pc2, s_CL2, t_CL2, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(s_pc2, s_CL2, t_CL2, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(2, s_pc2, t_pc2, s_CL2, t_CL2, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL2.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc2.
             (     Thread2_2_backward( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(s_pc2, t_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_2_backward;
#timer;



// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_2_backward(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // initial configuration (init)
  |  (exists
          Local   t_CL1,
          Local   t_CL2,
          Local   t_CL3,
          Global  t_G.
        (
          Thread2_2_backward( s_pc1, t_CL1, s_pc2, t_CL2, s_pc3, t_CL3, t_G )
          &  t_G.v1 = 0
        )
        &  ContextSwitch_1(s_pc1)
        &  ContextSwitch_2(s_pc2)
     )

//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_2_backward( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL1.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_1(s_pc1, t_pc1, s_CL1, t_CL1, s_G, t_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(s_pc1, s_CL1, t_CL1, s_G, t_G)   //  Copy others global variable
                  )
                  | programInt3(1, s_pc1, t_pc1, s_CL1, t_CL1, s_G, t_G )   // constrain
                )
           )
      )
    )

  | (

          true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL1.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc1.
             (     Thread1_2_backward( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(s_pc1,t_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_2_backward;
#timer;


// ####    ###   #   #  #   #  ####                  ###
 // #   #  #   #  #   #  #   #   #  #                #   #
 // #   #  #   #  #   #  ##  #   #  #                    #
 // ####   #   #  #   #  # # #   #  #                  ##
 // # #    #   #  #   #  #  ##   #  #                 #
 // #  #   #   #  #   #  #   #   #  #                #
 // #   #   ###    ###   #   #  ####                 #####




// ###   #  #  ###   ####   ##   ###          #
//  #    #  #  #  #  #     #  #  #  #        ##
//  #    ####  #  #  ###   #  #  #  #         #
//  #    #  #  ###   #     ####  #  #         #
//  #    #  #  # #   #     #  #  #  #         #
//  #    #  #  #  #  ####  #  #  ###         ###

mu bool Thread1_2(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable
 Global    s_G                    // Global variable
)
  s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread1_2( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 1, t_pc1 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread3_1( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  Thread1_2_backward(s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G)
        &  s_G.v1 = 0
        &  ContextSwitch_3(s_pc3)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
         true
      & (exists                  // There exists an internal state that
           PrCount t_pc1,
           Local   t_CL1,
           Global t_G.
           (    (   Thread1_2( t_pc1, t_CL1, s_pc2, s_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL1.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_1(t_pc1,s_pc1,t_CL1,s_CL1,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_1(t_pc1,t_CL1,s_CL1,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(1, t_pc1, s_pc1, t_CL1, s_CL1, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (

          true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL1.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc1.
             (     Thread1_2( t_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_1(t_pc1,s_pc1,s_CL1,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread1_2;
#timer;



 // #####  #   #  ####   #####    #    ####          ###
 //   #    #   #  #   #  #       # #    #  #        #   #
 //   #    #   #  #   #  #      #   #   #  #            #
 //   #    #####  ####   ####   #   #   #  #          ##
 //   #    #   #  # #    #      #####   #  #         #
 //   #    #   #  #  #   #      #   #   #  #        #
 //   #    #   #  #   #  #####  #   #  ####         #####

mu bool Thread2_2(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread2_2( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 2, t_pc2 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread1_2( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_1(s_pc1)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc2,
           Local   t_CL2,
           Global  t_G.
           (    (   Thread2_2( s_pc1, s_CL1, t_pc2, t_CL2,s_pc3, s_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL2.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_2(t_pc2,s_pc2,t_CL2,s_CL2,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_2(t_pc2,t_CL2,s_CL2,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(2, t_pc2, s_pc2, t_CL2, s_CL2, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL2.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc2.
             (     Thread2_2( s_pc1, s_CL1, t_pc2, s_CL2,s_pc3, s_CL3, s_G )
                 & programInt2_2(t_pc2,s_pc2,s_CL2,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread2_2;
#timer;







// ###   #  #  ###   ####   ##   ###         ####
//  #    #  #  #  #  #     #  #  #  #           #
//  #    ####  #  #  ###   #  #  #  #         ##
//  #    #  #  ###   #     ####  #  #           #
//  #    #  #  # #   #     #  #  #  #        #  #
//  #    #  #  #  #  ####  #  #  ###          ##

mu bool Thread3_2(
 PrCount    s_pc1,                  // Program counter
 Local      s_CL1,                  // Local variable
 PrCount    s_pc2,                  // Program counter
 Local      s_CL2,                  // Local variable
 PrCount    s_pc3,                  // Program counter
 Local      s_CL3,                  // Local variable

 Global    s_G                    // Global variable
)
 s_pc1    <  s_CL1,
 s_CL1    <  s_pc2,
 s_pc2    <  s_CL2,
 s_CL2    <  s_pc3,
 s_pc3    <  s_CL3,
 s_CL3    <  s_G
( false

  // early termination
  | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (   Thread3_2( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &    target( 3, t_pc3 )                // target is reached
        )
     )


  // initial configuration (init)
  |  (
        Thread2_2( s_pc1, s_CL1, s_pc2, s_CL2,s_pc3, s_CL3, s_G )
        &  s_G.v1 = 0
        &  ContextSwitch_2(s_pc2)
     )




//*********************************************************************************/
// forward propagation on internal transitions
//*********************************************************************************/

  |  (
      true
      & (exists                  // There exists an internal state that
           PrCount t_pc3,
           Local   t_CL3,
           Global  t_G.
           (    (   Thread3_2( s_pc1, s_CL1, s_pc2, s_CL2, t_pc3, t_CL3, t_G )
                )
               // &( t_G.v1=0 | ( t_G.v1=1 & t_CL3.v0=1 ) )   // Start, so when it ends?
               &(
                  ( programInt1_3(t_pc3,s_pc3,t_CL3,s_CL3,t_G,s_G)      // Assignment related
                    & CopyVariables_ProgramInt_3(t_pc3,t_CL3,s_CL3,t_G,s_G)   //  Copy others global variable
                  )
                  | programInt3(3, t_pc3, s_pc3, t_CL3, s_CL3, t_G, s_G )   // constrain
                )
           )
      )
    )

  | (
         true
       // & (s_G.v1=0 | ( s_G.v1=1 & s_CL3.v0=1 ) )     // Atomic condition
       & (exists PrCount t_pc3.
             (     Thread3_2( s_pc1, s_CL1, s_pc2, s_CL2,t_pc3, s_CL3, s_G )
                 & programInt2_3(t_pc3,s_pc3,s_CL3,s_G)    // Control flow statement
             )
         )
    )

);
#size Thread3_2;
#timer;





/******************************************************************************************************/
//                                       Reachabibility check                                         *
/******************************************************************************************************/


(
    ( exists
            PrCount t_pc,
            Local   t_CL,
            Global  t_G.
        (   Init_Reach( t_pc, t_CL, t_G )
          & target( 0, t_pc)   // ??????????
        )
     )

   | ( exists                   // There exists a state such that
            PrCount    t_pc1,
            Local      t_CL1,
            PrCount    t_pc2,
            Local      t_CL2,
            PrCount    t_pc3,
            Local      t_CL3,
            Global    t_G.
        (
                Thread1_3( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )    // That state in fixed point and ...
          &     Thread3_2( t_pc1, t_CL1, t_pc2, t_CL2, t_pc3, t_CL3, t_G )
        )
     )
);

#timer stop;

